#include "mbed.h"
#include "TFTLCD_Driver.h"

// Create a UnbufferedSerial object with a default baud rate.
static UnbufferedSerial serial_port(USBTX, USBRX, 57600);

// Board peripherals
AnalogIn sensor(A0);

DigitalOut green_led(LED1);
DigitalOut yellow_led(LED2);
DigitalOut red_led(LED3);

Adafruit_TFTLCD tft(
    // PD_4,  // lite
    PE_5,  // cs
    PE_4,  // cd
    PE_2,  // wr
    PD_6,  // rd
    PD_3,  // rst
    PG_3,  // d0
    PG_2,  // d1
    PD_2,  // d2
    PC_12, // d3
    PC_11, // d4
    PC_10, // d5
    PC_9,  // d6
    PC_8   // d7
); 

/*********************************************************************************/
// ARM Math and Mbed-DSP library
#include "arm_math.h"
#include "arm_common_tables.h"
#include "arm_const_structs.h"
#include "math_helper.h"

// FFT Size = 512 = 256 real and 256 imaginary
// Sampling Frequency = 1024 Hz
// Nyquist Frequency = 512 Hz
// FFT Bin Resolution = Nyquist Freq / FFT Size
//                    = 512 Hz / 512 = 1 Hz
#define FFT_SIZE  (512)
#define SAMPLING_FREQ  (FFT_SIZE*2)

// FFT variables
float32_t FFT_Output[FFT_SIZE];
float32_t FFT_Input[FFT_SIZE*2]; // Real and imaginary parts
float     FFT_maxValue;          // Max FFT value is stored here
uint32_t  FFT_maxIndex;          // Index in Output array where max value is

void FFT_Process() {
    // Init the Complex FFT module, intFlag = 0, doBitReverse = 1
    // using predefined arm_cfft_sR_f32_lenXXX
    arm_cfft_f32(&arm_cfft_sR_f32_len512, FFT_Input, 0, 1);
    
    // Complex Magniture Module put results into Output(Half size of the Input)
    arm_cmplx_mag_f32(FFT_Input, FFT_Output, FFT_SIZE);
    
    // Calculates maxValue and returns corresponding value (up to half of sampling frequency)
    arm_max_f32(&FFT_Output[0], FFT_SIZE/2, &FFT_maxValue, &FFT_maxIndex);
    //FFT_maxIndex += 0;
}

/*********************************************************************************/
// Hardware Timer
TIM_HandleTypeDef MyTimerHandle;
volatile bool MyTimerTrigger = false;

extern "C" void MyTimer_InterruptCallback(void) {
    if (__HAL_TIM_GET_FLAG(&MyTimerHandle, TIM_FLAG_UPDATE) == SET) {
        __HAL_TIM_CLEAR_FLAG(&MyTimerHandle, TIM_FLAG_UPDATE);
        MyTimerTrigger = true;
    }
}

void MyTimer_Init(float freq) {
    __HAL_RCC_TIM3_CLK_ENABLE();
    MyTimerHandle.Instance               = TIM3;
    MyTimerHandle.Init.Prescaler         = 30 - 1;
    MyTimerHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    MyTimerHandle.Init.Period            = (SystemCoreClock / 30.0 / freq) - 1;
    MyTimerHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&MyTimerHandle);
    HAL_TIM_Base_Start_IT(&MyTimerHandle);
    NVIC_SetVector(TIM3_IRQn, (uint32_t) MyTimer_InterruptCallback);
    NVIC_EnableIRQ(TIM3_IRQn);
}

void MyTimer_Reset() {
    MyTimerTrigger = false;
    __HAL_TIM_SET_COUNTER(&MyTimerHandle, 0);
}

/*********************************************************************************/
// Main function
char buf[100];

int main() {
    // Setup TFT LCD display
    tft.reset();
    uint16_t identifier = tft.readID();
    tft.begin(identifier);
    tft.setRotation(3); // 320x240
    tft.fillScreen(BLACK);
    
    // Test TFT LCD text
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.writeString((const uint8_t *) "Hello World!");
      
    // Timer for sampling
    MyTimer_Init(SAMPLING_FREQ);

    while(1){
        // Collect samples
        red_led = true;
        MyTimer_Reset();
        for (int i = 0; i < FFT_SIZE; i++) {
            while (MyTimerTrigger == false) {}; // wait until timer has been triggered
            MyTimerTrigger = false;
            FFT_Input[i*2] = sensor.read() - 0.5f; // Real part, removing DC offset
            FFT_Input[i*2 + 1] = 0; // Imaginary Part
        }
        // Perform FFT
        yellow_led = true;
        FFT_Process();
        
        // Display graph
        green_led = true;
        for (int i = 0; i < (FFT_SIZE/2); i++) {
            int magnitude = 240.0 * FFT_Output[i] / FFT_maxValue;
            if (magnitude > 240) {
                magnitude = 240;
            } else if (magnitude < 0) {
                magnitude = 0;
            }
            
            // Clear previous line
            tft.drawLine(i, 0, i, 240, BLACK);
            
            // Draw new line
            if (FFT_maxIndex == i) {
                tft.drawLine(i, 240-magnitude, i, 240, RED);
            } else {
                tft.drawLine(i, 240-magnitude, i, 240, CYAN);
            }
        }
        // Show max frequency component on top left
        int maxFreqComponent = FFT_maxIndex * FFT_SIZE / (SAMPLING_FREQ/2);
        tft.setCursor(10, 10);
        snprintf(buf, 100, "%d Hz", maxFreqComponent);
        tft.writeString((const uint8_t *) buf);
        
        red_led = false;
        yellow_led = false;
        green_led = false;

        // Print to serial port
        for (int i = 0; i < (FFT_SIZE/2); i++) {
            int length = snprintf(buf, 100, "[%d]=%d\n", i, (int)(FFT_Output[i]*1000));
            serial_port.write((uint8_t *) buf, length);
        }
        
        int length = snprintf(buf, 100, "SystemCoreClock=%lu MHz\n", SystemCoreClock);
        serial_port.write((uint8_t *) buf, length);
        int length1 = snprintf(buf, 100, "FFT_maxIndex=%d, FFT_maxValue=%d\n", FFT_maxIndex, (int) (FFT_maxValue*1000));
        serial_port.write((uint8_t *) buf, length1);
        int length2 = snprintf(buf, 100, "============================\n");
        serial_port.write((uint8_t *) buf, length2);
    }

}
