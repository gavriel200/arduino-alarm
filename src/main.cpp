#include "config.h"
#include "alarm.h"
#include "timer.h"


Alarm alarm(BLUE_LED_1, BLUE_LED_2, BLUE_LED_3, BLUE_LED_4,
            RED_LED, BUZZER, TRIG_PIN, ECHO_PIN, POWER_PIN, ROW_1, ROW_2, ROW_3, ROW_4,
            COL_1, COL_2, COL_3, COL_4);

int main()
{
  Timer::init();
  alarm.run();
  return 0;
}