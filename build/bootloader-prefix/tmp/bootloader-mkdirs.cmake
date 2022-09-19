# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/gedan/esp/esp-idf/components/bootloader/subproject"
  "C:/Codigos_esp/FOC_IMPLEMENTATION/sample_project/build/bootloader"
  "C:/Codigos_esp/FOC_IMPLEMENTATION/sample_project/build/bootloader-prefix"
  "C:/Codigos_esp/FOC_IMPLEMENTATION/sample_project/build/bootloader-prefix/tmp"
  "C:/Codigos_esp/FOC_IMPLEMENTATION/sample_project/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Codigos_esp/FOC_IMPLEMENTATION/sample_project/build/bootloader-prefix/src"
  "C:/Codigos_esp/FOC_IMPLEMENTATION/sample_project/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Codigos_esp/FOC_IMPLEMENTATION/sample_project/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
