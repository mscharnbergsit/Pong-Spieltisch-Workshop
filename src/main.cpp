#include <Arduino.h>

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initInputs();
}

void loop() {
  // put your main code here, to run repeatedly:
  printInputs();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}