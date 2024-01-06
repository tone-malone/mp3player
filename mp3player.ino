


// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // pixels

// You can use any (4 or) 5 pins (reset not needed)
#define SCLK_PIN 2
#define MOSI_PIN 3
#define DC_PIN   4
#define CS_PIN   5
#define RST_PIN  6
 #define outputA 10
 #define outputB 11


// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF


#include <Adafruit_GFX.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <Adafruit_SSD1351.h>

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/11, /*tx =*/12);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);  

const int NEXT_BUTTON = 7; // the pin number of the push button
const int PREV_BUTTON = 8;

int volumeLevel = 30;
int currentStateNEXT;    // the current reading from the input pin
int currentStatePREV;    
int currentStateVOLUP;
int currentStateVOLDOWN;

 int counter = 0; 
 int aState;
 int aLastState;  

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
  FPSerial.begin(9600);


  Serial.begin(115200);


 tft.begin(); //turn on display


 uint16_t time = millis();    //time 
  tft.fillRect(0, 0, 128, 128, BLACK); //fill background black
  time = millis() - time;

  Serial.println();
  Serial.println(F("MP3 Player"));
  Serial.println(F("Initializing MP3 Player ..."));

  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    while(true);
  }
  Serial.println(F("MP3 Player Ready"));  

  myDFPlayer.volume(volumeLevel);  //Set volume value (0~30).

  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  myDFPlayer.enableDAC();  //Enable On-chip DAC

pinMode(NEXT_BUTTON, INPUT_PULLUP);
pinMode(PREV_BUTTON, INPUT_PULLUP);
pinMode (outputA,INPUT);
pinMode (outputB,INPUT);


 aLastState = digitalRead(outputA);   

}

void loop()
{
tftDisplayVOL();
   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState && aState == 1){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       tft.fillRect(48,115, 20, 20, BLACK);
      //tft.fillScreen(BLACK);
       volumeLevel ++;
       
     } else {
      //tft.fillScreen(BLACK);
      tft.fillRect(48,115, 20, 20, BLACK);
      volumeLevel --;
     }
     Serial.print("VOLUME: ");
     Serial.println(volumeLevel);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
 

nextButton();
prevButton();

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

//end of loop

//begin functions

void nextButton()
{

 currentStateNEXT = digitalRead(NEXT_BUTTON);
// check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if(currentStateNEXT == LOW)
  {
    myDFPlayer.next(); 
   // Serial.println("PLAY");
  tft.setCursor(50,64);
  tft.println("NEXT");
    delay(1000);
  tft.fillScreen(BLACK);
  }
  else
  {  
  exit;
  }
} 

void prevButton()
{
currentStatePREV = digitalRead(PREV_BUTTON);
// check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if(currentStatePREV == LOW)
  {
    myDFPlayer.previous(); 
   // Serial.println("PLAY");
  tft.setCursor(40,64);
  tft.println("PREVIOUS");
  delay(1000);
  tft.fillScreen(BLACK);

  }
  else
  {  
  exit;
  }
}  

void tftDisplayTime()
{
 tft.fillScreen(BLACK);
 tft.setCursor(0, 5);
 tft.setTextColor(WHITE);
 tft.print(millis() / 1000);
 tft.setTextColor(WHITE);
 tft.print(" seconds.");
}

void tftDisplayVOL()
{
 tft.setCursor(0, 115);
 tft.setTextColor(YELLOW);
 tft.print("VOLUME = ");
 tft.setTextColor(WHITE);
 tft.println(volumeLevel);
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      tft.fillScreen(BLACK);
      tft.setCursor(32, 5);
      tft.setTextColor(WHITE);
      tft.println(F("JUST RELAX"));
      tft.setCursor(28, 20);
      tft.setTextColor(WHITE);
      tft.println(F("GOOD DREAMS"));
      tft.setCursor(28, 35);
      tft.setTextColor(WHITE);
      tft.println(F("COMING SOON"));      
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Stack Wrong!"));      
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("SD Card Penetrates")); 
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Pull Out!")); 
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Card Online!!")); 
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("USB Inserted!")); 
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("USB Removed")); 
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F(" Play Finished!")); 
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("DFPlayerError:")); 
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Card not found")); 
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Sleeping")); 
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Get Wrong Stack")); 
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Check Sum Does Not Match")); 
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("File Index Out of Bound")); 
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("Cannot Find File")); 
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          tft.fillScreen(BLACK);
      tft.setCursor(0, 5);
      tft.setTextColor(WHITE);
      tft.println(F("In Advertise")); 
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
