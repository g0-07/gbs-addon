#include <AltSoftSerial.h>
#include <IRremote.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>

/*
 * gbs-addon - ct 20.05.2021
 * 
 * Idee + IRRemote Part von Sanni
 * 
 * TODO:
 * - IRRemote: mehrere FBs? Lernfunktion?
 * - OLED: Feedback wenn über IR gesteuert wird
 * - OLED: Anzeige Preset
 * - ...
 * 
 */

#define DECODE_NEC

#define button_0            0x41
#define button_1            0x15
#define button_2            0x16
#define button_3            0x17
#define button_4            0x19
#define button_5            0x1A
#define button_6            0x1B
#define button_7            0x1D
#define button_8            0x1E
#define button_9            0x1F
#define button_menu         0xA
#define button_down         0xF
#define button_back         0x44
#define button_left         0x49
#define button_select       0xD
#define button_right        0x4A
#define button_title        0x7  //EXIT
#define button_up           0xB
#define button_info         0x58 //GUIDE
#define button_skip_minus   0x18 //CH+
#define button_stop         0xC  //VOL+
#define button_pause        0x10 //VOL-
#define button_skip_plus    0x1C //CH-
#define button_reverse      0x4C //RED
#define button_play         0x4D //GREEN
#define button_forward      0x4E //YELLOW
#define button_display      0x4F //BLUE

const unsigned char logo [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 
  0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 
  0x80, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 
  0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 
  0xc0, 0x80, 0x00, 0x00, 0xc0, 0xf0, 0xf0, 0xf8, 0xf8, 0xc0, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 
  0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 
  0xe0, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x7f, 0x7b, 0x71, 0x71, 
  0x31, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x39, 0x71, 0x71, 0x7b, 0x7f, 0x3f, 
  0x1f, 0x00, 0x00, 0x61, 0x63, 0x67, 0x7f, 0x7c, 0x7c, 0x38, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 
  0x00, 0x1f, 0x3f, 0x7f, 0x7b, 0x71, 0x71, 0x71, 0x71, 0x00, 0x00, 0x1f, 0x3f, 0x7f, 0x73, 0x71, 
  0x71, 0x7b, 0x7f, 0x3f, 0x1f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x03, 0x01, 0x01, 0x7f, 0x7f, 
  0x7f, 0x7f, 0x00, 0x00, 0x01, 0x3f, 0x7f, 0x7f, 0x71, 0x71, 0x01, 0x00, 0x7f, 0x7f, 0x7f, 0x07, 
  0x01, 0x01, 0x01, 0x00, 0x1f, 0x3f, 0x7f, 0x73, 0x71, 0x71, 0x7b, 0x7f, 0x3f, 0x1f, 0x00, 0x00, 
  0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 
  0x07, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

SSD1306AsciiAvrI2c oled;
AltSoftSerial altSerial;

boolean firstrun = true;
boolean stage1 = false;
boolean stage2 = false;

const int inputsize = 27;
int inputpos = 0;
char input[inputsize + 1];

void handleDisplay() {
  char* tok = strtok(input, "|");
  int tokpos = 0;

  if (firstrun == true) {
    oled.clear();
    firstrun = false;
  }
  
  while (tok != NULL) {
      switch (tokpos) {
        case 0:
          oled.set1X();
          oled.setCol(64);
          oled.setRow(1);
          if (strcmp(tok, "0") == 0) {
            oled.print((char *)"      RGB"); 
          }
          else if (strcmp(tok, "1") == 0) {
            oled.print((char *)"    YPbPr"); 
          }
          else if (strcmp(tok, "no signal") == 0) {
            oled.setCol(3);
            oled.setRow(1);
            oled.print((char *)"   ");            
            oled.setCol(64);
            oled.setRow(1);
            oled.print((char *)"NO SIGNAL"); 
            oled.set2X();
            oled.setCol(0);
            oled.setRow(3);
            oled.print((char *)" *********");
            oled.setCol(0);
            oled.setRow(5);
            oled.print((char *)" *********");
          }
          else if (strcmp(tok, "passthrough") == 0) {
            oled.setCol(3);
            oled.setRow(1);
            oled.print((char *)"   ");            
            oled.setCol(64);
            oled.setRow(1);
            oled.print((char *)"         "); 
            oled.set2X();
            oled.setCol(0);
            oled.setRow(3);
            oled.print((char *)" PASS-    ");
            oled.setCol(0);
            oled.setRow(5);
            oled.print((char *)" THROUGH  ");
          }
          break;
        case 1:
          oled.set1X();
          oled.setCol(3);
          oled.setRow(1);
          if (strcmp(tok, "0") == 0) {
            oled.print((char *)"BOB");
          }
          else if (strcmp(tok, "1") == 0) {
            oled.print((char *)"MA ");
          }
          else {
            oled.print((char *)"   ");
          }
          break;
        case 2:
          oled.set2X();
          oled.setCol(0);
          oled.setRow(3);
          oled.print(tok);
          break;
        case 3:
          oled.set2X();
          oled.setCol(0);
          oled.setRow(5);
          oled.print(tok);
          break;
      }
      tokpos++;
      tok = strtok(NULL, "|");
  }
}

void setup() {
  Serial.begin(19200);
  Serial.println("Setting up gbs-addon ...");
  
  IrReceiver.begin(2, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
  
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(font5x7);
  oled.clear(); 
  int a = 0;
  for (byte b = 0; b < 4; b++) {
    oled.setCursor (0, (3 + b)); 
    for (byte i = 0; i < 128; i++) {
      oled.ssd1306WriteRam(pgm_read_byte(&logo[a]));
      a++;
    }  
  }

  altSerial.begin(19200);
}

void loop() {
  while (altSerial.available()) {
    char c;
    
    c = altSerial.read();
    Serial.write(c);
    
    if (c == '@') {
      if (stage1 == false) stage1 = true;
      else if (stage1 == true && stage2 == false) stage2 = true;
    }
    else if (c == '\n' && stage2 == true) {
      input[inputpos] = '\0';
      handleDisplay();  
      stage1 = false;
      stage2 = false;
      inputpos = 0;
    }
    else {
      if (stage2 == true) {
        if (inputpos == inputsize) {
          stage1 = false;
          stage2 = false;
          inputpos = 0;          
        }
        else {
          input[inputpos] = c;
          inputpos++;         
        }
      }
      else {
        stage1 = false;
        stage2 = false;
        inputpos = 0;
      }
    }
  }
  
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == NEC && IrReceiver.decodedIRData.address == 0x38) {
      switch (IrReceiver.decodedIRData.command) {
        case button_display:
          // force 60hz PAL
          altSerial.write('0');
          break;
  
        case button_0:
          // save preset
          altSerial.write('4');
          break;
  
        case button_info:
          // scanline strength
          altSerial.write('K');
          break;
  
        case button_skip_minus:
          // Horizontal -
          altSerial.write('Y');
          break;
  
        case button_skip_plus:
          // Horizontal +
          altSerial.write('Z');
          break;
  
        case button_title:
          // full height
          altSerial.write('v');
          break;
  
        case button_forward:
          // Boarder masking +
          altSerial.write('A');
          break;
  
        case button_reverse:
          // Boarder masking -
          altSerial.write('B');
          break;
  
        case button_left:
          // 480p preset
          altSerial.write('h');
          break;
  
        case button_select:
          // 720p preset
          altSerial.write('g');
          break;
  
        case button_up:
          // 960p preset
          altSerial.write('f');
          break;
  
        case button_right:
          // 1080p preset
          altSerial.write('s');
          break;
        case button_down:
          // downscale preset
          altSerial.write('L');
          break;
  
        case button_back:
          // Reset to default
          altSerial.write('1');
          break;
  
        case button_pause:
          // Scanlines
          altSerial.write('7');
          break;
  
        case button_stop:
          // line filter
          altSerial.write('m');
          break;
  
        case button_menu:
          // Restart
          altSerial.write('a');
          break;
  
        case button_1:
          // select preset
          altSerial.write('b');
          // load preset
          altSerial.write('3');
          break;
  
        case button_2:
          // select preset
          altSerial.write('c');
          // load preset
          altSerial.write('3');
          break;
  
        case button_3:
          // select preset
          altSerial.write('d');
          // load preset
          altSerial.write('3');
          break;
  
        case button_4:
          // select preset
          altSerial.write('j');
          // load preset
          altSerial.write('3');
          break;
  
        case button_5:
          // select preset
          altSerial.write('k');
          // load preset
          altSerial.write('3');
          break;
  
        case button_6:
          // select preset
          altSerial.write('G');
          // load preset
          altSerial.write('3');
          break;
  
        case button_7:
          // select preset
          altSerial.write('H');
          // load preset
          altSerial.write('3');
          break;
  
        case button_8:
          // select preset
          altSerial.write('I');
          // load preset
          altSerial.write('3');
          break;
  
        case button_9:
          // select preset
          altSerial.write('J');
          // load preset
          altSerial.write('3');
          break;
  
        default:
          break;
      }
      delay(300);
    }
    IrReceiver.resume();
  }
}
