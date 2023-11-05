#include <Arduino.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

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

struct Ball {
  int x;
  int y;
  int dirX;
  int dirY;
  int speed = 1;
} ball;

struct Paddle {
  int x;
  double y;
  int width = 1;
  double speed = 1;
} paddle1, paddle2;

bool gameIsRunning = false;
int score[2];

CRGB leds[NUM_LEDS];
LiquidCrystal_I2C lcd(0x27, 16, 2);

CRGB ballColor = CRGB::Red;
CRGB paddleColor = CRGB::Blue;

unsigned long gameStepInterval = 100;
unsigned long lastGameStepTime = 0;

unsigned long inputStepInterval = 50;
unsigned long lastInputStepTime = 0;

uint16_t XY( uint8_t x, uint8_t y);
void initInputs();
void initPlayers();
void initBall();
void movePaddle(Paddle &paddle, int dirY);
void drawPaddle(Paddle paddle);
void removePaddle(Paddle paddle);
void moveBall();
void bounceBallWall();
void bounceBallPaddle();
void drawBall();
void removeBall();
bool isCollidingWithPaddle(int x, int y);
void updatePlayer1();
void updatePlayer2();
void updateBall();
void checkGameStart(unsigned long currentMillis);
void checkGameEnd();
void checkGoal();
void startGame();
void endGame();

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

  lcd.init();
  lcd.backlight();

  initInputs();

  lcd.setCursor(0, 0);
  lcd.print("Press green");
  lcd.setCursor(0, 1);
  lcd.print("to start game");
  srand(time(NULL));
}

void loop() {
  unsigned long currentMillis = millis();
  if(!gameIsRunning) {
    checkGameStart(currentMillis);
  }
  else {
    if(currentMillis - lastInputStepTime >= inputStepInterval) {
      updatePlayer1();
      updatePlayer2();
      FastLED.show();
      lastInputStepTime = currentMillis;
    }
    if(currentMillis - lastGameStepTime >= gameStepInterval) {
      updateBall();
      FastLED.show();
      checkGoal(); 
      lastGameStepTime = currentMillis;
    } 
    checkGameEnd();
  }
}

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

void checkGameStart(unsigned long currentMillis) {
  if (!digitalRead(PLAYER_1_GREEN_PIN)) {
    startGame();
    lastGameStepTime = currentMillis;
  }
}
void checkGameEnd() {
  if (!digitalRead(PLAYER_1_RED_PIN)) {
    endGame();
  }
}
void checkGoal() {
  if(ball.x == 0 || ball.x == MATRIX_WIDTH - 1) {
    Serial.println("Goal");
    if(ball.x == 0) {
      score[1]++;
    }
    else {
      score[0]++;
    }
    //goal animation
    removeBall();
    char buffer[6];
    snprintf_P(buffer, sizeof(buffer), PSTR("%.2i:%.2i"), score[0], score[1]);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
    delay(500);
    initBall();
  }
}
void startGame() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score:");
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  score[0] = 0;
  score[1] = 0;
  initPlayers();
  initBall();
  FastLED.show();
  gameIsRunning = true;
  delay(500);
  
}
void endGame() {
  removePaddle(paddle1);
  removePaddle(paddle2);
  removeBall();
  FastLED.clearData();
  FastLED.show();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press green");
  lcd.setCursor(0, 1);
  lcd.print("to start game");
  gameIsRunning = false;
}
void updatePlayer1() {
  removePaddle(paddle1);
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
  int player1Y = analogRead(PLAYER_1_JOYSTICK_X_AXIS);
  if (player1Y < 450) {
    movePaddle(paddle1, 1);
  }
  if (player1Y > 580) {
    movePaddle(paddle1, -1);
  }
  drawPaddle(paddle1);
}
void updatePlayer2() {
  removePaddle(paddle2);
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
  int player2Y = analogRead(PLAYER_2_JOYSTICK_X_AXIS);
  if (player2Y < 450) {
    movePaddle(paddle2, 1);
  }
  if (player2Y > 580) {
    movePaddle(paddle2, -1);
  }

  drawPaddle(paddle2);
}
void updateBall() {
  removeBall();
  moveBall();
  drawBall();
}
void initPlayers() {
  paddle1.y = (MATRIX_HEIGHT - 1) / 2;
  paddle1.x = 1;
  paddle1.width = 3;
  paddle2.y = (MATRIX_HEIGHT - 1) / 2;
  paddle2.x = MATRIX_WIDTH - 2;
  paddle2.width = 3;
}
void initBall() {
  ball.dirX = rand() % 2 == 1 ? 1 : -1;
  ball.dirY = rand() % 3 - 1;
  ball.x = (MATRIX_WIDTH - 1) / 2 + (ball.dirX == 1 ? 0 : 1);
  ball.y = rand() % (MATRIX_HEIGHT - 2) + 1;
}
void movePaddle(Paddle &paddle, int dirY) {
  double newPosition = paddle.y + (dirY * paddle.speed);
  bool isValidPosition =  newPosition >= 0 && newPosition + paddle.width <= MATRIX_HEIGHT;
  paddle.y = isValidPosition ? newPosition : paddle.y;
}
void drawPaddle(Paddle paddle) {
  CRGB color = paddleColor;
  if(paddle.speed < 0)
    color = blend(color, CRGB::Red, 125);
  if(abs(paddle.speed) < 1)
    color = blend(color, CRGB::Yellow, 125);
  for (int i = 0; i < paddle.width; i++)
  {
    leds[XY(paddle.x, round(paddle.y) + i)] = color;
  } 
}
void removePaddle(Paddle paddle) {
  for (int i = 0; i < paddle.width; i++)
  {
    leds[XY(paddle.x, round(paddle.y) + i)] = CRGB::Black;
  }
}
void moveBall() {
  int nextX = ball.x + ball.dirX;
  int nextY = ball.y + ball.dirY;
  if(nextY < 0 || nextY > MATRIX_HEIGHT - 1) {
    bounceBallWall();
    nextY = ball.y + ball.dirY;
  }
  if(isCollidingWithPaddle(nextX, nextY)) {
    bounceBallPaddle();
    nextX = ball.x + ball.dirX;
    nextY = ball.y + ball.dirY;
  }
  if(nextY < 0 || nextY > MATRIX_HEIGHT - 1) {
    bounceBallWall();
    nextY = ball.y + ball.dirY;
  }
  ball.x = nextX;
  ball.y = nextY;
}
bool isCollidingWithPaddle(int x, int y) {
  return (x == paddle1.x && y >= round(paddle1.y) && y <= round(paddle1.y) + paddle1.width - 1 ) || (x == paddle2.x && y >= round(paddle2.y) && y <= round(paddle2.y) + paddle2.width - 1 );
}
void bounceBallWall() {
  Serial.println("Bounce Wall");
  ball.dirY = ball.dirY * -1;
}
void bounceBallPaddle() {
  Serial.println("Bounce Paddle");
  ball.dirY = (rand() % 3) - 1;
  ball.dirX = ball.dirX * -1;
}
void removeBall() {
  uint16_t index = XY(ball.x, ball.y);
  if(leds[index] == ballColor)
    leds[index] = CRGB::Black;
}
void drawBall() {
  leds[XY(ball.x, ball.y)] = ballColor;
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
