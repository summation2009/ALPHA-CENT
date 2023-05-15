#define TFT_WIDTH 320
#define TFT_HEIGH 240

int16_t PNG_XPOS;
int16_t PNG_YPOS;

#define FONT_FS_AVAILABLE

#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI    tft = TFT_eSPI();         // Create object "tft"


// Include the PNG decoder library
#include <PNGdec.h>
PNG png;
#define MAX_IMAGE_WDITH 240 // Adjust for your images

#include "png_fs_support.h"   // ไปแก้บรรทัด SPIFFS ให้เป็น LITTLEFS ด้วย
//#include "frame.h"


#define CALIBRATION_FILE "/TouchCalData1"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false



#define KEY_TEXTSIZE 1   // Font size multiplier

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

//#define TH_sarabunPSK32 &Lth_sarabunPSK32    // 

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button MyButton;


void drawButton(){
   tft.loadFont("THNiramitAS-36", LITTLEFS);
  //tft.setFreeFont(LABEL2_FONT);

  //tft.fillScreen(TFT_BLACK);

                          // x, y, w, h, outline, fill, text
  MyButton.initButton(&tft, 120 ,285,  190, 45, TFT_YELLOW, TFT_YELLOW, TFT_RED, "", KEY_TEXTSIZE);
  MyButton.drawButton();

  tft.setCursor(45,280);
  tft.setTextColor(TFT_RED, TFT_YELLOW);
  tft.print("เริ่มการทำงาน");

}

void drawThaiText() {
  tft.loadFont("THNiramitAS-36", LITTLEFS);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5,30);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("ธรรมใดก็ไร้ค่า ถ้าไม่ทำ");
  tft.unloadFont();
}


// void drawMachineID() {
//   tft.setCursor(150, 60);
//   tft.setTextColor(TFT_DARKGREY);
//   //tft.print(device_id);
// }

//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display


#define MAX_IMAGE_WDITH 480 // Adjust for your images

//int16_t xpos = 0;
//int16_t ypos = 0;

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(PNG_XPOS, PNG_YPOS + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}


void drawPNGimage(String fname) {
  // Scan LittleFS and load any *.png files


  //Serial.println("drawPNGimage ");

   File f = LITTLEFS.open(fname, "r");

  // if (f) Serial.println("f ok ");

  // f.close();


  //while (File file = root.openNextFile()) {
    //String strname = file.name();
    //strname = "/" + strname;
    //fname = "/" + fname;
    //Serial.println(file.name());
    // If it is not a directory and filename ends in .png then load it
    if (!f.isDirectory() && fname.endsWith(".png")) {
      // Pass support callback function names to library
      //Serial.println(fname);

      int16_t rc = png.open(fname.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw);
      //int16_t rc = png.open("start_wash.png", pngOpen, pngClose, pngRead, pngSeek, pngDraw);
      if (rc == PNG_SUCCESS) {
        tft.startWrite();
        Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
        uint32_t dt = millis();
        if (png.getWidth() > MAX_IMAGE_WDITH) {
          //Serial.println("Image too wide for allocated line buffer size!");
        }
        else {
          rc = png.decode(NULL, 0);
          png.close();
        }
        tft.endWrite();
        // How long did rendering take...
        //Serial.print(millis()-dt); Serial.println("ms");
      }
    }
    //delay(3000);
    //tft.fillScreen(random(0x10000));
  //} // end while
}

// // void showStartWash() {
// //   drawPNGimage("/start_wash_bg.png");

// //   tft.setCursor(45,280);
// //   tft.setTextColor(TFT_RED, TFT_YELLOW);
// //   tft.print(WASH_PRICE);
// // }



// bool InAreaPress (int touch_x, int touch_y, int x, int y, int width, int height) {
//   for (int i = x; i <= x + width; i++) {
//     for (int j = y; j<= y + height; j++) {
//       if (touch_x == i && touch_y == j) return 1;   // touch press in check area
//     }
//   }
//   return 0;
// }


// void CheckHomeStarPress (void) {


//   uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

//   // Pressed will be set true is there is a valid touch on the screen
//   bool pressed = tft.getTouch(&t_x, &t_y);

//   // Serial.printf("x: %i     ", t_x);
//   // Serial.printf("\ty: %i     ", t_y);
//   // Serial.println();

//   //Serial.printf("z: %i \n", tft.getTouchRawZ());


// // if (pressed) Serial.println("pressed");

// // / Check if any key coordinate boxes contain the touch coordinates
// //for (uint8_t b = 0; b < 15; b++) {
//   if (pressed && InAreaPress(t_x, t_y, 19, 263, 202, 51)) {   // button เริ่มการทำงาน
//     //MyButton.press(true);  // tell the button it is pressed
//     Serial.println("Start Button press");
//     //MUST_SEND_GCPF = true;

//     SHOWWASHING = 0;
//     SHOWHOMESTARTWASH = 0;
//     SHOWCONFIRMSTARTWASH = 1;

//     tft.fillScreen(TFT_BLACK);
    
//     drawPNGimage("/confirm_start.png");
//     drawMachineID();

//     delay(1000);
//     TFT_NN = 0;
//     CHECKTOUCHPAGE = 2;
//   } else {
//     //MyButton.press(false);  // tell the button it is NOT pressed
//     //Serial.println("Button Not press");
//   }
//}

// Check if any key has changed state

  //if (b < 3) tft.setFreeFont(LABEL1_FONT);
  //else tft.setFreeFont(LABEL2_FONT);

  // if (MyButton.justReleased()) MyButton.drawButton();     // draw normal

  // if (MyButton.justPressed()) {
  //   MyButton.drawButton(true);  // draw invert
  //   delay(10);
  // }

//}

// void CheckConfirmStarWashPress (void) {

//   uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

//   // Pressed will be set true is there is a valid touch on the screen
//   bool pressed = tft.getTouch(&t_x, &t_y);

// // / Check if any key coordinate boxes contain the touch coordinates
// //for (uint8_t b = 0; b < 15; b++) {
//   if (pressed && InAreaPress(t_x, t_y, 61, 81, 113, 113)) {   // button confirm เริ่มการทำงาน -> create qr code
//     //MyButton.press(true);  // tell the button it is pressed
//     Serial.println("Confirm Button press");
//     //MUST_SEND_GCPF = true;

//     SHOWWASHING = 0;
//     SHOWHOMESTARTWASH = 0;
//     SHOWCONFIRMSTARTWASH = 0;

//     tft.fillScreen(TFT_BLACK);

//     PNG_XPOS = 0;
//     PNG_YPOS = 0;
//     CHECKTOUCHPAGE = 0;

//     drawPNGimage("/create_qrcode.png");

//     requestQR();

//     delay(1000);
//   } else if (pressed && InAreaPress(t_x, t_y, 19, 263, 202, 51)) {      // btn cancel
//     tft.fillScreen(TFT_BLACK);
//     //SHOWHOMESTARTWASH = 1;
//     Serial.println("Cancel Button press");
//     PNG_XPOS = 0;
//     PNG_YPOS = 0;
//     //drawPNGimage("/start_wash_bg.png");
//     SHOWHOMESTARTWASH = 1;
//     //delay(1000);
//     TFT_NN = 0;
//     CHECKTOUCHPAGE = 1;
//   }
//}


//}


void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  //LITTLEFS.begin();
  //File f = LITTLEFS.open("/file.txt","w");

  //File f = LITTLEFS.open(CALIBRATION_FILE, "r");
  // while(f.available()) {
  //   String line = f.readStringUntil('\n');
  //   Serial.println(line);
  // }
  // f.close();

  // check file system exists
  // if (!SPIFFS.begin()) {
  //   Serial.println("Formating file system");
  //   SPIFFS.format();
  //   SPIFFS.begin();
  // }

  File f;

  // check if calibration file exists and size is correct
  if (LITTLEFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      LITTLEFS.remove(CALIBRATION_FILE);
    }
    else
    {
      f = LITTLEFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    f = LITTLEFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}


// void draw_walk(void) {
//   for(int i=0;i<frames;i++) {
//     delay(40);
//     tft.pushImage(60, 15,animation_width  , animation_height, washing[i]);
//   }
// }