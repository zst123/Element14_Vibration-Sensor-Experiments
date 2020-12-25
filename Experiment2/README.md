# Characterize RPM of Spinning Devices

## Experiment 2

### Mbed Firmware

The code is for the **NUCLEO-H743ZI2** board using Mbed OS 6.

The Mbed code is similar to that in Experiment 1 except that it uses an UnbufferedSerial instead of the build in printf serial port interface. This is so that we can set a custom baud rate.

### Plotter Program

The plotter program uses PyQt to do high performance and low-latency graph plotting. 

Install the dependencies using PIP

	pip3 install PyQt5 pyqtgraph pyserial

In `plotter.py`, you may need to change this line in the code. It is to point to the serial port of your device. It should look like `COM1` for Windows or `/dev/ttyUSB0` for Linux. The serial port baud rate can also be changed.

	SERIAL_PORT_NAME = "/dev/ttyACM0"
	SERIAL_BAUD_RATE = 57600


### Hardware Setup

For the hardware setup, I have written a detailed walkthrough of the hardware setup on Element14 (blog post #3)

- https://www.element14.com/community/community/design-challenges/experimenting-with-vibration-sensors/blog/authors/zst123

