# Characterize RPM of Spinning Devices

## Experiment 4

For more details, please see my blog posts on Element14. (blog post #6)

- https://www.element14.com/community/community/design-challenges/experimenting-with-vibration-sensors/blog/authors/zst123

### Mbed Firmware

The code in the `Mbed-FFT-Dashboard` diectory is for the **NUCLEO-H743ZI2** board using Mbed OS 6.
It depends on the [Mbed-DSP library](https://developer.mbed.org/users/mbed_official/code/mbed-dsp/).

Note that in the directory `mbed-dsp/cmsis_dsp/TransformFunctions/`, I have replaced `arm_bitreversal2.S` (the assembly version) with `my_arm_bitreversal2.c` (the C version). The file has been [taken from here](https://github.com/Masmiseim36/nxpSDK/blob/5a6a51d64b/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal2.c).

This is needed because the online Mbed compiler does not support building of assembly files. If you are using the local compiler, the assembly version is preferred as it has more speed optimisations.


### Hardware Setup

The [Adafruit 2.4 inch TFT LCD display](https://www.adafruit.com/product/1770) is used in this project

Connect it to the Nucleo board with the following pin connections.

Nucleo Board | LCD Display
--- | ---
PD_4 | lite
PE_5 | cs
PE_4 | cd
PE_2 | wr
PD_6 | rd
PD_3 | rst
PG_3 | d0
PG_2 | d1
PD_2 | d2
PC_12 | d3
PC_11 | d4
PC_10 | d5
PC_9 | d6
PC_8 | d7
