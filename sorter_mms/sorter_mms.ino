#include <Stepper.h>
#include <Servo.h>
#include <Arduino_APDS9960.h>

#define STEPS 32 // ilsoc krokow silnika krokowego

#define SERVO_TIME 360 // czas (ms) obrotu serwo o 180 stopni

#define TRIES_UNTIL_STOP 100 // liczba prob zeskanowania mmmsa przed wylaczeniem programu

#define BUTTON_PIN 3 // pin przycisku sterującego
#define SCAN_LED 7 // pin oswietlenia skanu

#define COLORS 6 // liczba kolorów
#define GREEN 0
#define RED 1
#define BLUE 2
#define BROWN 3
#define YELLOW 4
#define ORANGE 5
#define UNKNOWN_COLOR 6

Stepper dispenserStepper(STEPS, 9, 11, 10, 12);
Servo latch, director;

bool machineStarted = false;
int color = 0;
bool mmmsPresent = true;

void setup()
{
  Serial.begin(9600);
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }
  dispenserStepper.setSpeed(1000); // pełna prędkość
  pinMode(BUTTON_PIN, INPUT);
  pinMode(SCAN_LED, OUTPUT);
  pinMode(5, OUTPUT);
  latch.attach(6);
  director.attach(5);
  closeLatch();
  directTo(0);
}

void loop()
{
   listenForSignal(); // da sie to jakos asynchronicznie? Bo trzeba bedzie miec wcisniety przycisk idealnie w czasie.
   unsigned short i = 0;
   while(i++ <= TRIES_UNTIL_STOP && machineStarted) {
     getNextMmms();
     scan();
     if(isMmmsPresentInChamber()) {
       i = 0;
       directTo(currentMmmsColor());
       openLatch();
       closeLatch();
     }
   }
   stopMachine();
}

void listenForSignal() {
  if (!machineStarted) {
    Serial.println("Czy start (y/n)");  
    machineStarted = readYesNoFromSerial();
  }
//  int input = digitalRead(BUTTON_PIN);
//  if(input == HIGH) {
//    machineStarted =! machineStarted;
//  }
}

void startMachine() {
    machineStarted = true;
}

void stopMachine() {
    machineStarted = false;
}

void scan() {
  return;
  Serial.println("Skanowanie...");
  digitalWrite(SCAN_LED, HIGH); 
  
  while (! APDS.colorAvailable()) {
    delay(5);
  }
  int r, g, b;
  APDS.readColor(r, g, b);

  // empty chamber values
  r -= 58+11-45;
  g -= 84+16-61;
  b -= 120+22-192+101;
  
  int colorSum = r + g + b;
  float redPercentage = (float)r/(float)colorSum*100.0f;
  float greenPercentage = (float)g/(float)colorSum*100.0f;
  float bluePercentage = (float)b/(float)colorSum*100.0f;
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
//  
//  Serial.println(redPercentage);
//  Serial.println(greenPercentage);
//  Serial.println(bluePercentage);
  Serial.println(colorSum);
  
  char guess[25] = "Empty";
  bool mmmsPresent = false;
  
  if (colorSum > 50) {
    mmmsPresent = true;
    if (colorSum < 500 && around(redPercentage, 30.0f) && around(greenPercentage, 30.0f) && around(bluePercentage, 38.0f)) {
      color = BROWN;
        strcpy(guess, "brown");
    } else if (around(redPercentage, 50.0f) && around(greenPercentage, 30.0f) && around(bluePercentage, 20.0f)) {
      color = ORANGE;
        strcpy(guess, "orange");
    } else if (around(redPercentage, 40.0f) && around(greenPercentage, 40.0f) && around(bluePercentage, 20.0f)) {
      color = YELLOW;
        strcpy(guess, "yellow");
    } else if(redPercentage > 45.0f) {
      color = RED;
        strcpy(guess, "red");
    } else if(greenPercentage > 45.0f) {
      color = GREEN;
        strcpy(guess, "green");
    } else if(bluePercentage > 45.0f) {
      color = BLUE;
        strcpy(guess, "blue");
    } else {
      color = UNKNOWN_COLOR;
        strcpy(guess, "unknown");
    }
  }
  
  char message[50] = {0};
  sprintf(message, "Guess: %s", guess);
  Serial.println(message);
  
  digitalWrite(SCAN_LED, LOW);
}

bool around (float a, float b) {
  return abs(a-b) < 7.5f;
}

bool isMmmsPresentInChamber() { // porownanie odczytu czytnika koloru do koloru pustej komory? 
//  Serial.println("Czy MMMs wpadł (y/n)");
//  return readYesNoFromSerial();
  return true;
  return mmmsPresent;
}

int currentMmmsColor() { // Komora najlepiej czarna bo nie odbija światła aż tak
//  return random(0,6);
  Serial.println("Jaki kolor MMMs (int)");
  while(Serial.available() == 0) {
    delay(1);
  }
  int input = Serial.parseInt();
  Serial.println(input);
  return input;   
  return color;
}

void getNextMmms(){
  dispenserStepper.step(2048/6);
}

void openLatch() {
  latch.write(0);
  delay(SERVO_TIME);  
}

void closeLatch() {
  latch.write(180);
  delay(SERVO_TIME);
}

void directTo(int color) {
  int position = 180/(COLORS)*color;
  director.write(position);
//  analogWrite(5, position);
  delay(SERVO_TIME);
}

bool readYesNoFromSerial() {
  while(true) {
    if(Serial.available() > 0) {
      char input = (char)Serial.read();
      if (input == 'y') {
        return true;
      } else if (input == 'n') {
        return false;
      }
      clearSerialBuffer();
    }
  }
}

void clearSerialBuffer(){
  while(Serial.available() > 0 ) {
    Serial.read();  
  }  
}
