#!/usr/bin/env python3

'''
Created by:
  Manzel Seet (@zst123)
  December 2020

Dependencies:
  $ pip3 install PyQt5 pyqtgraph pyserial

References:
  https://org-technology.com/posts/mbed-pyqt-matplotlib-realtime-plot.html
  https://www.learnpyqt.com/tutorials/plotting-pyqtgraph/
  https://swharden.com/blog/2016-07-31-real-time-audio-monitor-with-pyqt/

'''

from PyQt5 import QtWidgets, QtCore, QtGui
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
import serial
import sys
import threading
import time
from datetime import datetime
import signal
import os
from collections import deque


SERIAL_PORT_NAME = "/dev/ttyACM0"
SERIAL_BAUD_RATE = 57600
AXIS_LIMIT = 5000 # number of samples to display

# Use of deque instead of arrays because it is faster
axis_time = deque(maxlen=AXIS_LIMIT)
axis_amplitude = deque(maxlen=AXIS_LIMIT)


class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        # graph line pen color
        self.pen = pg.mkPen(color=(0, 255, 255), width=2)

        # graph plot widget
        self.graphWidget = pg.PlotWidget()
        self.setCentralWidget(self.graphWidget)

        # data line plot
        self.data_line = self.graphWidget.plot(pen=self.pen)
        self.graphWidget.getPlotItem().setYRange(min=0, max=65535)
        self.graphWidget.showGrid(x=True, y=True)
        #self.graphWidget.getPlotItem().ctrl.fftCheck.setChecked(True)

    def plot(self, x, y):
        # update data line
        self.data_line.setData(x, y)


def main():
    # Create the application window
    app = QtWidgets.QApplication(sys.argv)
    main = MainWindow()

    # Handle command line Ctrl+C event to quit app
    signal.signal(signal.SIGINT, signal.SIG_DFL)

    # Handle main window close button to quit app
    def kill_app(arg=None):
        os.kill(os.getpid(),signal.SIGKILL)
    main.closeEvent = kill_app

    # Start a timer to update every 100ms
    def timer_update():
        global axis_time, axis_amplitude
        main.plot(x=axis_time, y=axis_amplitude)

    timer = QtCore.QTimer(main)
    timer.timeout.connect(timer_update)
    timer.start(2)

    # initial value for plot arrays
    timestamp_start = datetime.now()

    # handler method to plot the graph
    def handle_data(reading):
        global axis_time, axis_amplitude
        
        try:
            reading = int(reading.strip())
            elapsed_timedelta = datetime.now() - timestamp_start
            elapsed_time = elapsed_timedelta.total_seconds()

            axis_time.append(elapsed_time)
            axis_amplitude.append(reading)

        except Exception as e:
            print(e)

    # Create thread for handling serial port
    serial_port = serial.Serial(port=SERIAL_PORT_NAME, baudrate=SERIAL_BAUD_RATE)
    serial_port.flushInput()
    serial_port.flushOutput()

    def read_from_port(ser):
        global axis_time, axis_amplitude
        while True:
            try:
                data = ser.readline()
                handle_data(data.decode())
                # print(data, len(axis_amplitude))
            except UnicodeDecodeError as e:
            	print(e)
            	ser.flushInput()
            except serial.serialutil.SerialException as e:
                print(e)
                kill_app()

    thread = threading.Thread(target=read_from_port, args=(serial_port,))
    thread.start()

    # Start application (blocking)
    main.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
