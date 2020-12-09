#include "mbed.h"

AnalogIn sensor(A0); 
DigitalOut green_led(LED1);
DigitalOut red_led(LED3);

int main() {    
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
        printf("%d\n", reading);
        
        ThisThread::sleep_for(100ms);
   }
}
