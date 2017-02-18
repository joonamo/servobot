#include <Servo.h> 
#include "FastLED.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

# include <Metro.h>

Metro Rotate_Update = Metro(1);
Metro Eyes_Update = Metro(2000);
Metro Nose_Update = Metro(10003);
Metro Mouth_Update = Metro(3007);

//#define MIN_POS 16
#define MIN_POS 0
#define MAX_POS 180

Adafruit_PCD8544 display = Adafruit_PCD8544(18, 19, 20);
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 90;    // variable to store the servo position 

#define NUM_EYES 15
#define FACE_PART_SIZE 7
#define EYE_PART_SIZE 4
static const char PROGMEM eyes[] =
{
//"1234567"
  "0  0"
  "$  $"
  "-  -"
  "u  u"
  "0  -"
  "-  0"
  "*  *"
  "?  ?"
  "!  !"
  "0  o"
  ".  ."
  "\\  /"
  "^  ^"
  "O  O"
  ">  <"
};
uint8_t eye_part = 0;

#define NUM_NOSES 8
static const char PROGMEM noses[] =
{
//"1234567"
  "   c   "
  "   u   "
  "   .   "
  "\"  u  \""
  "   ^   "
  "   U   "
  "   v   "
  "   -   "
};
uint8_t nose_part = 0;

#define NUM_MOUTHS 14
static const char PROGMEM mouths[] =
{
//"1234567"
  " \\___/ "
  " v---v "
  " \" v \" "
  " ==.== "
  "   w   "
  "  WWW  "
  "  \\_/  "
  "  ___  "
  "  ___/ "
  " >---< "
  "   .   "
  "   v   "
  "   _   "
  "    3  "
};
uint8_t mouth_part = 0;
 
void setup() 
{ 
  for (size_t i = 0; i < 4; ++i)
    pinMode(i, INPUT_PULLUP);
  myservo.attach(17);  // attaches the servo on pin 9 to the servo object 
  random16_set_seed(analogRead(0));

  display.begin();
  display.setContrast(50);

  display.setRotation(2);

  display.setTextSize(2);
  display.setTextWrap(false);
  display.setTextColor(BLACK);

  Serial.begin(9600);

  draw_face();
} 
 
void loop() 
{ 
  if (!digitalRead(0))
  {
    myservo.write(pos);
    delay(100);
  } 
  if (Rotate_Update.check() == 1)
  {
    myservo.write(map(random8(), 0,255, MIN_POS, MAX_POS));
    Rotate_Update.interval(500 + random8() * 5);
    Rotate_Update.reset();
  }

  if (Eyes_Update.check() == 1)
  {
    if (random8() < 160)
      eye_part = 0;
    else
      eye_part = (random8() % NUM_EYES) * EYE_PART_SIZE;
    draw_face();
  }

  if (Nose_Update.check() == 1)
  {
    nose_part = (random8() % NUM_NOSES) * FACE_PART_SIZE;
    draw_face();
  }

  if (Mouth_Update.check() == 1)
  {
    if (random8() < 180)
      mouth_part = 0;
    else
      mouth_part = (random8() % NUM_MOUTHS) * FACE_PART_SIZE;
    draw_face();
  }
} 

void draw_face()
{
  display.clearDisplay();

  // if (random8() == 128)
  // {
  //   display.drawBitmap(18, 0,  sprite, 48, 48, 1);
  //   display.display();
  //   return;
  // }

  display.clearDisplay();
  display.setCursor(7,2);
  display.setTextSize(3);

  // Eyes
  uint8_t end = eye_part + EYE_PART_SIZE;
  for (uint8_t i = eye_part; i < end; ++i)
    display.print(eyes[i]);

  display.setCursor(0, 15);

  display.setTextSize(2);
  // Nose
  end = nose_part + FACE_PART_SIZE;
  for (uint8_t i = nose_part; i < end; ++i)
    display.print(noses[i]);
  display.println();

  end = mouth_part + FACE_PART_SIZE;
  for (uint8_t i = mouth_part; i < end; ++i)
    display.print(mouths[i]);

  display.display();
}

