# ESP32 CODE implementation of FOC control method for BLDC MOTOR

Work in progress for the esp32 code of the FOC algorithm for actual control of a BLDC motor.

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
