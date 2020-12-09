# Characterize RPM of Spinning Devices

## Experiment 1

The code is for the **NUCLEO-H743ZI2** board using Mbed OS 6.

It is a continuous loop which will do the following task indefinitely:

1. Sample the vibration sensor using the ADC
2. Check if the vibration detected is about a certain threshold, then toggle an LED
3. Send the vibration sensor values to the PC using the serial port (printf)

For the hardware setup, I have written a detailed walkthrough of the hardware setup on Element14 (blog post #3)

- https://www.element14.com/community/community/design-challenges/experimenting-with-vibration-sensors/blog/authors/zst123
