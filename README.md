# Task
```
Connect an ultrasonic sensor to detect any objects from the front side and display the distance on LCD screen , also the module should fire an alarm when the object is 30 cm or closer using buzzer so that the siren sound is related to the distance (smaller distance generate faster beeps).
```

# Components

* Arduino Uno
* Ultrasonic Sensor
* 16x2 LCD Screen ( I2C )
* Buzzer
* Breadboard
* Jumper Wires
* Resistors

# Circuit Diagram

![Circuit Diagram]()

# Thoughts

In order to improve the stabilty of the ultrasonic sensor, instead of normal instantaneous value readings, a moving average of the last 10 readings is used. This is done by storing the last 8 readings in an array and then calculating the average of the array. Which was implemented in the `getDistance()` function.

# Initial Implementation

```c
// function that reads 10 readings and removes max and min readings then returns the average
int read_distance_average() 
{
  int readings[10];
  int i;
  int j;
  int temp;
  int average;
  int total;

  // store 10 readings
  for (i = 0; i < 10; i++) {
    readings[i] = read_distance();
  }
   
  // sort the readings
  for (i = 0; i < 9; i++) {
    for (j = i + 1; j < 10; j++) {
      if (readings[i] > readings[j]) {
        temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }

  total = 0;

  // remove max and min readings
  for (i = 3; i < 7; i++) {
    total += readings[i];
  }

  average = total / 4;

  return average;
}
```

# Final Implementation

used an arduino library that implements a moving average of the last N readings while removing anomalties using a median filter [link](https://github.com/daPhoosa/MedianFilter)

```