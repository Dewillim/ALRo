#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

#define US1_TRIGGER 19
#define US1_ECHO 17
#define US2_TRIGGER 18
#define US2_ECHO 16

#define M1 4
#define M2 23

#define LED_1 25
#define LED_2 26

#define S_WIDTH 128
#define S_HEIGHT 64

int M1_STATE;
int M2_STATE;
long TIME1;
int DIST_CM1;
long TIME2;
int DIST_CM2;

Adafruit_SSD1306 display(S_WIDTH, S_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Ultrasonic Sensor
  pinMode(US1_TRIGGER, OUTPUT);
  pinMode(US1_ECHO, INPUT);
  pinMode(US2_TRIGGER, OUTPUT);
  pinMode(US2_ECHO, INPUT);

  // Motors
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  // LEDs
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  // OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  display.setFont(&FreeSerif9pt7b);
}

void loop() {
  // Ultrasonic Sensor
  DIST_CM1 = measureDist1();
  DIST_CM2 = measureDist2();

  // Serial Monitor
  printDist(DIST_CM1, DIST_CM2);

  // Motors + LEDs
  if (DIST_CM1 > 35 && DIST_CM2 < 35) {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
  } 
  if (DIST_CM2 > 35 && DIST_CM1 < 35) {
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
  }
  // OLED Display
  displayDist(DIST_CM1, DIST_CM2);
}

// Measure Distance
int measureDist1(){
  // Clearing the trigger
  digitalWrite(US1_TRIGGER, LOW);
  delayMicroseconds(2);
  // Setting the trigger on HIGH state for 10 usec
  digitalWrite(US1_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(US1_TRIGGER, LOW);
  // Reading the echo, returns the sound wave travel time in usec
  TIME1 = pulseIn(US1_ECHO, HIGH);
  // Calculating the distance
  DIST_CM1 = TIME1*0.034/2;
  return DIST_CM1;
}

int measureDist2(){
  // Clearing the trigger
  digitalWrite(US2_TRIGGER, LOW);
  delayMicroseconds(2);
  // Setting the trigger on HIGH state for 10 usec
  digitalWrite(US2_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(US2_TRIGGER, LOW);
  // Reading the echo, returns the sound wave travel time in usec
  TIME2 = pulseIn(US2_ECHO, HIGH);
  // Calculating the distance
  DIST_CM2 = TIME2*0.034/2;
  return DIST_CM2;
}

// Print Distance
void printDist(int a, int b){
  // Printing the distance on the Serial Monitor
  Serial.print("Dist_1: ");
  Serial.println(a);
  Serial.print("Dist_2: ");
  Serial.println(b);
}

// Display Distance
void displayDist(int a, int b){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("Distancia:");
  display.setCursor(30, 40);
  display.println(a);
  display.setCursor(30, 60);
  display.println(b);
  display.display();
}
