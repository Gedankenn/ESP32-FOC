# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/gedan/esp/esp-idf/components/bootloader/subproject"
  "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader"
  "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader-prefix"
  "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader-prefix/tmp"
  "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader-prefix/src"
  "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/gedan/ESP-CODES/ESP32-FOC/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
