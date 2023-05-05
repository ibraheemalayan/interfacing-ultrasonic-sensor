#include <LiquidCrystal_I2C.h>
#include <MedianFilter.h>

#define BUZZER_PIN 7
#define TRIGGER_PIN A3
#define ECHO_PIN A2

// windows size: 10, seed 0
MedianFilter filterObject(10, 0); 
LiquidCrystal_I2C lcd(0x27, 16, 2);


int duration;
int led_period;


void setup() {
  // initialize serial communication at 9600 bits per second:

  Serial.begin(9600);

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();  //initialize the lcd
  lcd.setBacklight(HIGH);

  lcd.clear();

  lcd.setBacklight(HIGH);

  lcd.print("Distance: ");
}

void loop() {

  unsigned int raw_distance = read_distance();

  unsigned int filtered_distance = filterObject.in(raw_distance);

  lcd.setCursor(0, 10);

  lcd.print(filtered_distance);
  lcd.print("    ");

  if (filtered_distance > 30) {
    noTone(BUZZER_PIN);
  } else {
    tone(BUZZER_PIN, 10000/filtered_distance );
  }

}

int read_distance() {

  unsigned int cm;

  digitalWrite(TRIGGER_PIN, LOW);

  digitalWrite(TRIGGER_PIN, HIGH);

  delayMicroseconds(10);

  duration = pulseIn(ECHO_PIN, HIGH);

  cm = duration / 2 / 29.1 + 1;

  delay(60); // 

  return cm;
}



// function that reads 10 readings and removes max and min readings then returns the average
// int read_distance_average() 
// {
//   int readings[10];
//   int i;
//   int j;
//   int temp;
//   int average;
//   int total;

//   for (i = 0; i < 10; i++) {
//     readings[i] = read_distance();
//   }

//   for (i = 0; i < 9; i++) {
//     for (j = i + 1; j < 10; j++) {
//       if (readings[i] > readings[j]) {
//         temp = readings[i];
//         readings[i] = readings[j];
//         readings[j] = temp;
//       }
//     }
//   }

//   total = 0;

//   for (i = 3; i < 7; i++) {
//     total += readings[i];
//   }

//   average = total / 4;

//   return average;
// }