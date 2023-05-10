#include <LiquidCrystal_I2C.h>
#include <MedianFilter.h>
#include <Arduino_FreeRTOS.h>
#include <EasyBuzzer.h>
#include <semphr.h>

#define BUZZER_PIN 3
#define TRIGGER_PIN A3
#define ECHO_PIN A2

void UltrasonicTask(void *pvParameters);
void BuzzerTask(void *pvParameters);

SemaphoreHandle_t mutex;

// windows size: 6, seed 0
MedianFilter filterObject(6, 0);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int duration;
int led_period;

int isBeeping = 0;

void setup() {
  // initialize serial communication at 9600 bits per second:

  Serial.begin(9600);

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  EasyBuzzer.setPin(BUZZER_PIN);

  lcd.init();  //initialize the lcd
  lcd.setBacklight(HIGH);

  lcd.print("Distance: ");

  mutex = xSemaphoreCreateMutex();

  xTaskCreate(UltrasonicTask, "ultrsonic", 128, NULL, 1, NULL);


  vTaskStartScheduler();
}

void loop() {
}

void UltrasonicTask(void *pvParameters) {

  while (1) {

    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
      /* obtained the semaphore and can now access the
            shared resource. */

      unsigned int raw_distance = read_distance();  // overall delay between reads should be at least 60 ms

      unsigned int filtered_distance = filterObject.in(raw_distance);

      lcd.setCursor(0, 10);

      lcd.print(filtered_distance);
      lcd.print("    ");

      if (filtered_distance > 30) {
       
        noTone(BUZZER_PIN);


      } else {

        if (filtered_distance > 20) {
          tone(BUZZER_PIN, 100);
        } else if (filtered_distance > 10) {
          tone(BUZZER_PIN, 3000);
        } else {
          tone(BUZZER_PIN, 10000);
        }
      }

      xSemaphoreGive(mutex);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

int read_distance() {

  taskENTER_CRITICAL();

  unsigned int cm;

  digitalWrite(TRIGGER_PIN, LOW);

  digitalWrite(TRIGGER_PIN, HIGH);

  delayMicroseconds(10);

  duration = pulseIn(ECHO_PIN, HIGH);

  cm = duration / 2 / 29.1 + 1;

  taskEXIT_CRITICAL();

  return cm;
}


