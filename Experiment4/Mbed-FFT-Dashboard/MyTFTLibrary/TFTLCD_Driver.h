// IMPORTANT: SEE COMMENTS @ LINE 15 REGARDING SHIELD VS BREAKOUT BOARD USAGE.

// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#ifndef _ADAFRUIT_TFTLCD_H_
#define _ADAFRUIT_TFTLCD_H_

#include "mbed.h"

#include "Adafruit_GFX.h"

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// **** IF USING THE LCD BREAKOUT BOARD, COMMENT OUT THIS NEXT LINE. ****
// **** IF USING THE LCD SHIELD, LEAVE THE LINE ENABLED:             ****

//#define USE_ADAFRUIT_SHIELD_PINOUT 1

class Adafruit_TFTLCD : public Adafruit_GFX {

public:
  Adafruit_TFTLCD(PinName cs, PinName cd, PinName wr, PinName rd, PinName rst,
                  PinName d0, PinName d1, PinName d2, PinName d3,
                  PinName d4, PinName d5, PinName d6, PinName d7);
  Adafruit_TFTLCD(void);

  void begin(uint16_t id = 0x9325);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
  void drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
  void fillScreen(uint16_t color);
  void reset(void);
  void setRegisters8(uint8_t *ptr, uint8_t n);
  void setRegisters16(uint16_t *ptr, uint8_t n);
  void setRotation(uint8_t x);
  // These methods are public in order for BMP examples to work:
  void setAddrWindow(int x1, int y1, int x2, int y2);
  void pushColors(uint16_t *data, uint8_t len, bool first);

  uint16_t color565(uint8_t r, uint8_t g, uint8_t b),
      readPixel(int16_t x, int16_t y), readID(void);
  uint32_t readReg(uint8_t r);

private:
  void init(),
      write8(uint8_t value),
      setWriteDir(void),
      setReadDir(void),
      writeRegister8(uint8_t a, uint8_t d),
      writeRegister16(uint16_t a, uint16_t d),
      writeRegister24(uint8_t a, uint32_t d),
      writeRegister32(uint8_t a, uint32_t d),
      writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d),
      setLR(void), flood(uint16_t color, uint32_t len);
  uint8_t driver;

  uint8_t read8fn(void);

  DigitalInOut* csPin;
  DigitalInOut* cdPin;
  DigitalInOut* wrPin;
  DigitalInOut* rdPin;
  DigitalInOut* rstPin;
  DigitalInOut* d0Pin;
  DigitalInOut* d1Pin;
  DigitalInOut* d2Pin;
  DigitalInOut* d3Pin;
  DigitalInOut* d4Pin;
  DigitalInOut* d5Pin;
  DigitalInOut* d6Pin;
  DigitalInOut* d7Pin;
};

// For compatibility with sketches written for older versions of library.
// Color function name was changed to 'color565' for parity with 2.2" LCD
// library.
#define Color565 color565

#endif