#include <Stepper.h>
#include <Servo.h>

#define STEPS 32 // ilsoc krokow silnika krokowego

#define SERVO_TIME 360 // czas (ms) obrotu serwo o 180 stopni

#define TRIES_UNTIL_STOP 6 // liczba prob zeskanowania mmmsa przed wylaczeniem programu

#define BUTTON_PIN 3 // pin przycisku sterującego

#define COLORS 6 // liczba kolorów
#define GREEN 0
#define RED 1
#define BLUE 2
#define BROWN 3
#define YELLOW 4
#define ORANGE 5

Stepper dispenserStepper(STEPS, 9, 11, 10, 12);
Servo latch, director;

bool machineStarted = false;

void setup()
{
  Serial.begin(9600);
  dispenserStepper.setSpeed(1000); // pełna prędkość
  pinMode(BUTTON_PIN, INPUT);
  latch.attach(6);
  director.attach(5);
  closeLatch();
}

void loop()
{
   listenForSignal(); // da sie to jakos asynchronicznie? Bo trzeba bedzie miec wcisniety przycisk idealnie w czasie.
   unsigned short i = 0;
   while(i++ <= TRIES_UNTIL_STOP && machineStarted) {
     getNextMmms();
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

bool isMmmsPresentInChamber() { // porownanie odczytu czytnika koloru do koloru pustej komory? 
  Serial.println("Czy MMMs wpadł (y/n)");
  return readYesNoFromSerial();
}

int currentMmmsColor() { // Komora najlepiej czarna bo nie odbija światła aż tak
  Serial.println("Jaki kolor MMMs (int)");
  while(Serial.available() == 0) {
    delay(1);
  }
  int input = Serial.parseInt();
  Serial.println(input);
  return input;   
}

void getNextMmms(){
  dispenserStepper.step(2048/6);
}

void openLatch() {
  latch.write(180);
  delay(SERVO_TIME);  
}

void closeLatch() {
  latch.write(0);
  delay(SERVO_TIME);
}

void directTo(int color) {
  int position = 180/(COLORS-1)*color;
  director.write(position);
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
