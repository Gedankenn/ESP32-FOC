# ESP32 CODE implementation of FOC control method for BLDC MOTOR

Work in progress for the esp32 code of the FOC algorithm for actual control of a BLDC motor.


## What the firmware does today

The current firmware runs **open-loop V/F (volts-per-hertz) control** of a BLDC motor, as a first step towards FOC:

- A potentiometer on **GPIO 36 (ADC1 channel 0)** sets the speed reference (0–4095 → 0–10 000 rpm), read as a raw value with the ESP-IDF `adc_oneshot` driver.
- A FreeRTOS task (`OpenLoopVF_Control`, `main/application/src/VF.c`) integrates the electrical angle from the reference speed, computes the phase voltage with the V/F gain (`Gf = 12 V / 10 000 rpm`, clamped at 12 V) and updates the duty cycles.
- **MCPWM unit 0** (legacy `driver/mcpwm.h` API) drives three half-bridges at **1 kHz**, with a short delay between switching off one side and switching on the other.
- **GPIO 22** is configured as the driver-enable output (set low at boot).

### Pin map (`main/driver/inc/gpio.h`)

| Signal | GPIO |
|--------|------|
| PWM0A / PWM0B (phase A high / low) | 15 / 2 |
| PWM1A / PWM1B (phase B high / low) | 4 / 16 |
| PWM2A / PWM2B (phase C high / low) | 17 / 5 |
| Driver enable | 22 |
| Speed reference (potentiometer, ADC1_CH0) | 36 |

The inverter hardware is documented in [3-phase-inverter](https://github.com/Gedankenn/3-phase-inverter).

## Repository layout

```
ESP32-FOC/
├── CMakeLists.txt              # ESP-IDF project file
├── sdkconfig                   # ESP32 target, generated with ESP-IDF 5.1.0
├── main/
│   ├── main.c                  # app_main: init ADC, GPIO, PWM; start the V/F task
│   ├── application/            # VF.c / VF.h: open-loop V/F control task
│   └── driver/                 # adc.c, gpio.c, PWM.c (+ headers in inc/)
├── figures/                    # ESP32 DevKit pinout
└── .devcontainer/              # VS Code dev container based on the espressif/idf image
```

## Build and flash

ESP-IDF project for the **ESP32** (the `sdkconfig` was generated with ESP-IDF 5.1.0; the code uses the legacy MCPWM driver, which is deprecated in newer ESP-IDF releases).

```bash
. $IDF_PATH/export.sh
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor   # replace with your serial port
```

A VS Code dev container (`.devcontainer/`, based on `espressif/idf`) is included as an alternative build environment.

## ESP-32 Pin-Out
![pinout](figures/ESP32-DOIT-DEVKIT-V1-Board-Pinout-36-GPIOs-updated.webp)

## Mathematical model for the BLDC motor

$$
\frac{d}{dt}
\left(\begin{array}{c}
I_a\\
I_b \\
I_c \\
\omega_r \\
\theta_e
\end{array}\right)
= \left(\begin{array}{ccccc}
-\frac{R}{L-M} & 0 & 0 & -\frac{P_p\phi_mF_a(\theta_e)}{L-M} & 0\\
0 & -\frac{R}{L-M} & 0 & -\frac{P_p\phi_mF_b(\theta_e)}{L-M} & 0\\
0 & 0 & -\frac{R}{L-M} & -\frac{P_p\phi_mF_c(\theta_e)}{L-M} & 0\\
0 & 0 & -\frac{R}{L-M} & -\frac{P_p\phi_mF_c(\theta_e)}{L-M} & 0\\
\frac{P_p\phi_mF_A(\theta_e)}{J} & \frac{P_p\phi_mF_B(\theta_e)}{J} & \frac{P_p\phi_mF_C(\theta_e)}{J} & -\frac{B}{J} & 0 \\
0 & 0 & 0 & P_p & 0
\end{array}\right)
\left(\begin{array}{c}
I_a \\
I_b \\
I_c \\
\omega_r \\
\theta_e
\end{array}\right)
+
\left(\begin{array}{cccc}
\frac{1}{L-M} & 0 & 0 & 0\\
0 & \frac{1}{L-M} & 0 & 0\\
0 & 0 & \frac{1}{L-M} & 0\\
0 & 0 & 0 & -\frac{1}{J} \\
0 & 0 & 0 & 0
\end{array}\right)
\left(\begin{array}{c}
V_an \\
V_bn \\
V_cn \\
T_c
\end{array}\right)
$$

## FOC mathematical operations

### Clarke transform

$$
    \left(\begin{array}{c}
    V_0 \\
    V_\alpha \\
    V_\beta
    \end{array}\right)
    =
    \sqrt{\frac{2}{3}}
    \left(\begin{array}{ccc}
    \sqrt{\frac{1}{2}} & \sqrt{\frac{1}{2}} & \sqrt{\frac{1}{2}} \\
    1 & -\frac{1}{2} & -\frac{1}{2} \\
    0  & \frac{\sqrt{3}}{2} & -\frac{\sqrt{3}}{2}
    \end{array}\right)
    \left(\begin{array}{c}
    V_A \\
    V_B \\
    V_C
    \end{array}\right)
$$

### Clarke Inverse Transform

$$
    \left(\begin{array}{c}
    V_A \\
    V_B \\
    V_C
    \end{array}\right)
    =
    \left(\begin{array}{ccc}
    \frac{\sqrt{2}}{2} & 1 & 0\\
    \frac{\sqrt{2}}{2} & -\frac{1}{2} & \frac{\sqrt{3}}{2} \\
    \frac{\sqrt{2}}{2} & -\frac{1}{2} & -\frac{\sqrt{3}}{2}
    \end{array}\right)
    \left(\begin{array}{c}
    V_0 \\
    V_\alpha \\
    V_\beta
    \end{array}\right)
$$

### Park Transform

$$
    \left(\begin{array}{c}
    V_d \\
    V_q
    \end{array}\right)
    =
    \left(\begin{array}{cc}
    \cos(\theta) & \sin(\theta) \\
    -\sin(\theta) & \cos(\theta)
    \end{array}\right)
    \left(\begin{array}{c}
    V_\alpha \\
    V_\beta
    \end{array}\right)
$$

### Inverse Park Transform

$$
    \left(\begin{array}{c}
    V_\alpha \\
    V_\beta
    \end{array}\right)
    =
    \left(\begin{array}{cc}
    \cos(\theta) & -\sin(\theta) \\
    \sin(\theta) & \cos(\theta)
    \end{array}\right)
    \left(\begin{array}{c}
    V_d \\
    V_q
    \end{array}\right)
$$

# V/F Control tactic
For the first run tests, the V/F OpenLoop method was choose.

V/F Control is a open loop method that consist of aplying a gain value for the Voltage of the motor pins.

$$
    G_f = \frac{V_{max}}{\omega_{max}} \\
    V_{max} = G_f \omega_{max}
$$

Code example:
```
Volts = ref*Gf*gain_adc;
comp_value = Volts*BLDC_MCPWM_PERIOD/Tmax;

//here is where de duty cycle is defined
duty_A = comp_value*FEM_sin(theta);
duty_B = comp_value*FEM_sin(theta + pi/3.0);
duty_C = comp_value*FEM_sin(theta - pi/3.0);
```

by changing the value of the $\omega$ we can adjust the Voltage value so the motor can run. 

Another necessary step is muntiplying the $\omega_{ref}$ that comes form the reference to the Gain of the V/F method by all the 3 phases FEM curve.

### Motor specs

For this work i will be using a A2212/13T brushless motor.

* Resistance 0.090$\Omega$
* Poles 14
* Inductance 0.1mH

## Calculations
$
K_v = \frac{\omega_{rpm}}{V_P0.95}
$

$K_v$ - is the voltage constante of a motor, rought means how many rotations the motor will go for every Volt.

$
K_p = \frac{60}{2 \pi  K_v}
$
if Kv in rpm

for Kv in rad it simply is the inverse of Kp

$
K_p = \frac{1}{K_v}
$