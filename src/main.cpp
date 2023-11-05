#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 28
//Set Matrix layout, used for calculating led position using XY method
#define MATRIX_SERPENTINE_LAYOUT true
#define MATRIX_VERTICAL false
#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 8

//Player 1 pin layout
#define PLAYER_1_UP_PIN 52
#define PLAYER_1_RIGHT_PIN 50
#define PLAYER_1_DOWN_PIN 48
#define PLAYER_1_LEFT_PIN 46

#define PLAYER_1_GREEN_PIN 42
#define PLAYER_1_RED_PIN 44

#define PLAYER_1_JOYSTICK_X_AXIS A1
#define PLAYER_1_JOYSTICK_Y_AXIS A0
#define PLAYER_1_JOYSTICK_BUTTON 22

//Player 2 pin layout
#define PLAYER_2_UP_PIN 38
#define PLAYER_2_RIGHT_PIN 36
#define PLAYER_2_DOWN_PIN 34
#define PLAYER_2_LEFT_PIN 32

#define PLAYER_2_JOYSTICK_X_AXIS A3
#define PLAYER_2_JOYSTICK_Y_AXIS A2
#define PLAYER_2_JOYSTICK_BUTTON 24

#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)

CRGB leds[NUM_LEDS];

uint16_t XY( uint8_t x, uint8_t y);

void initInputs() {
  //Player 1 PIN Setup
  //configure pullups for buttons
  pinMode(PLAYER_1_UP_PIN, INPUT_PULLUP);
  pinMode(PLAYER_1_RIGHT_PIN, INPUT_PULLUP);
  pinMode(PLAYER_1_DOWN_PIN, INPUT_PULLUP);
  pinMode(PLAYER_1_LEFT_PIN, INPUT_PULLUP);

  pinMode(PLAYER_1_JOYSTICK_BUTTON, INPUT_PULLUP);

  pinMode(PLAYER_1_GREEN_PIN, INPUT_PULLUP);
  pinMode(PLAYER_1_RED_PIN, INPUT_PULLUP);

  //analog pins dont need pullup
  pinMode(PLAYER_1_JOYSTICK_X_AXIS, INPUT);
  pinMode(PLAYER_1_JOYSTICK_Y_AXIS, INPUT);


  //Player 2 PIN Setup
  //configure pullups for buttons
  pinMode(PLAYER_2_UP_PIN, INPUT_PULLUP);
  pinMode(PLAYER_2_RIGHT_PIN, INPUT_PULLUP);
  pinMode(PLAYER_2_DOWN_PIN, INPUT_PULLUP);
  pinMode(PLAYER_2_LEFT_PIN, INPUT_PULLUP);

  pinMode(PLAYER_2_JOYSTICK_BUTTON, INPUT_PULLUP);
  //analog pins dont need pullup
  pinMode(PLAYER_2_JOYSTICK_X_AXIS, INPUT);
  pinMode(PLAYER_2_JOYSTICK_Y_AXIS, INPUT);
}

void printInputs() {
  //Check all player 1 inputs and output if any is available
  if (!digitalRead(PLAYER_1_UP_PIN)) {
    Serial.println("Player 1 up");
  }
  if (!digitalRead(PLAYER_1_RIGHT_PIN)) {
    Serial.println("Player 1 right");
  }
  if (!digitalRead(PLAYER_1_DOWN_PIN)) {
    Serial.println("Player 1 down");
  }
  if (!digitalRead(PLAYER_1_LEFT_PIN)) {
    Serial.println("Player 1 left");
  }
  if (!digitalRead(PLAYER_1_GREEN_PIN)) {
    Serial.println("GREEN");
  }
  if (!digitalRead(PLAYER_1_RED_PIN)) {
    Serial.println("RED");
  }
  if (!digitalRead(PLAYER_1_JOYSTICK_BUTTON)) {
    Serial.println("Player 1 Joystick button");
  }
  int player1X = analogRead(PLAYER_1_JOYSTICK_X_AXIS);
  int player1Y = analogRead(PLAYER_1_JOYSTICK_Y_AXIS);
  if (player1X < 450 || player1X > 580) {
    Serial.print("Player 1 X: ");
    Serial.println(player1X);
  }
   if (player1Y < 450 || player1Y > 580) {
    Serial.print("Player 1 Y: ");
    Serial.println(player1Y);
  }



  //Check all player 2 inputs and output if any is available
  if (!digitalRead(PLAYER_2_UP_PIN)) {
    Serial.println("Player 2 up");
  }
  if (!digitalRead(PLAYER_2_RIGHT_PIN)) {
    Serial.println("Player 2 right");
  }
  if (!digitalRead(PLAYER_2_DOWN_PIN)) {
    Serial.println("Player 2 down");
  }
  if (!digitalRead(PLAYER_2_LEFT_PIN)) {
    Serial.println("Player 2 left");
  }
  if (!digitalRead(PLAYER_2_JOYSTICK_BUTTON)) {
    Serial.println("Player 2 Joystick button");
  }
  int player2X = analogRead(PLAYER_2_JOYSTICK_X_AXIS);
  int player2Y = analogRead(PLAYER_2_JOYSTICK_Y_AXIS);
  if (player2X < 450 || player2X > 580) {
    Serial.print("Player 2 X: ");
    Serial.println(player2X);
  }
   if (player2Y < 450 || player2Y > 580) {
    Serial.print("Player 2 Y: ");
    Serial.println(player2Y);
  }
}

void initLeds() {
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      float x = (float) i / (float) MATRIX_WIDTH;
      float y = (float) j / (float) MATRIX_HEIGHT;
      leds[ XY(i, j) ] = CRGB(0, (int)(255.0 * x), 255 - (int)(255.0 * y));
    }
    FastLED.show();
    delay(50);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  initInputs();
  initLeds();
}

void loop() {
  // put your main code here, to run repeatedly:
  printInputs();
}

/**
 * @brief converts x and y coordinates to corresponding neopixel index.
 * Taken from https://github.com/FastLED/FastLED/blob/master/examples/XYMatrix/XYMatrix.ino
 * 
 * Does NOT check if values are within bounds!
 * 
 * @param x x coordinate to be converted
 * @param y y coordinate to be converted
 * @return uint16_t index of corresponding led in fastled strip
 */
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( MATRIX_SERPENTINE_LAYOUT == false) {
    if (MATRIX_VERTICAL == false) {
      i = (y * MATRIX_WIDTH) + x;
    } else {
      i = MATRIX_HEIGHT * (MATRIX_WIDTH - (x+1))+y;
    }
  }

  if( MATRIX_SERPENTINE_LAYOUT == true) {
    if (MATRIX_VERTICAL == false) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (MATRIX_WIDTH - 1) - x;
        i = (y * MATRIX_WIDTH) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * MATRIX_WIDTH) + x;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = MATRIX_HEIGHT * (MATRIX_WIDTH - (x+1))+y;
      } else {
        i = MATRIX_HEIGHT * (MATRIX_WIDTH - x) - (y+1);
      }
    }
  }
  
  return i;
}