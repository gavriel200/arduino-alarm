#include "alarm.h"
#include "timer.h"

// Pin definitions
#define BLUE_LED_1 A0
#define BLUE_LED_2 A1
#define BLUE_LED_3 A2
#define BLUE_LED_4 A3
#define RED_LED A4
#define BUZZER A5
#define TRIG_PIN 4
#define ECHO_PIN 3
#define ROW_1 8
#define ROW_2 7
#define ROW_3 6
#define ROW_4 5
#define COL_1 9
#define COL_2 10
#define COL_3 11
#define COL_4 12

Alarm alarm(BLUE_LED_1, BLUE_LED_2, BLUE_LED_3, BLUE_LED_4,
            RED_LED, BUZZER, TRIG_PIN, ECHO_PIN,
            ROW_1, ROW_2, ROW_3, ROW_4,
            COL_1, COL_2, COL_3, COL_4);

int main()
{
  Timer::init();
  alarm.run();
  return 0;
}