#include "mbed.h"

static UnbufferedSerial serial_port(USBTX, USBRX, 57600);

AnalogIn sensor(A0); 
DigitalOut green_led(LED1);
DigitalOut red_led(LED3);

int main() {
    char buffer[50]; 
    const uint16_t lower_threshold = 32768 - 7000;
    const uint16_t upper_threshold = 32768 + 7000;
    
    while(1) {
        uint16_t reading = sensor.read_u16();
        
        if (lower_threshold < reading && reading < upper_threshold) {
            green_led = true;
            red_led = false;
        } else {
            green_led = false;
            red_led = true;
        }
        int length = snprintf(buffer, 50, "%d\n", reading);
        serial_port.write((uint8_t *) buffer, length);
   }
}
