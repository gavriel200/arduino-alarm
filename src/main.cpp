#include "Alarm.h"

int main()
{
  Alarm alarm;

  alarm.setup();

  while (1)
  {
    alarm.loop();
  }

  return 0;
}
