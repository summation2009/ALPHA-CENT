# ALPHA-CENT development board

![image](https://github.com/summation2009/ALPHA-CENT/assets/100085999/aa0a80d6-8100-4502-9774-1b3d95c4f699)

![image](https://github.com/summation2009/ALPHA-CENT/assets/100085999/ae96a182-3212-4878-b11d-84e32300a7b5)

![image](https://github.com/summation2009/ALPHA-CENT/assets/100085999/10cff141-b609-43a8-a4f4-91718b9a4f13)


// Display type

**#define ILI9488_DRIVER**

 #define TFT_WIDTH  480 // ST7789 240 x 240 and 240 x 320
 
 #define TFT_HEIGHT 320 // ST7789 240 x 320

#define TFT_MISO 19

#define TFT_MOSI 23

#define TFT_SCLK 18

#define TFT_CS   33  // Chip select control pin

#define TFT_DC    25  // Data Command control pin

#define TFT_RST   -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

#define TOUCH_CS 0 //PIN_D2



//#define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue

#define TFT_RGB_ORDER TFT_BGR // Colour order Blue-Green-Red

// Black and white swap option

//#define TFT_INVERSION_ON

#define TFT_INVERSION_OFF

