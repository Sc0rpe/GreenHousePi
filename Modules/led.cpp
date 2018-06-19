//
// 2018 Rico Schulz
//
// This program dims an LED with PWM
//
#include <wiringPi.h>
#include <iostream>
#include <cstdlib>
#include <csignal>

using namespace std;

void exiting(int i)
{
	pinMode(1, OUTPUT);
	digitalWrite(1, LOW);
	exit(0);
}

int main()
{
	wiringPiSetup();
	//atexit(exiting);
	signal(SIGABRT, exiting);
	signal(SIGTERM, exiting);
	signal(SIGTSTP, exiting);
	signal(SIGINT, exiting);	

	pinMode(1, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS); 

	pwmSetClock(192);
    //pwmSetClock(384);

	pwmSetRange(1000);

	int val=50;
	bool change_dir=false;

    //pwmWrite(1, val);
	while(true)
	{
        cout << "val= " << val << endl;
		pwmWrite(1, val);
        delay(100);
        if (change_dir)
          val = val - 5;
        else
          val = val + 5;
      if (val >= 250) {
        change_dir = true;
        val= 250;
      }
      if (val <= 50) {
        change_dir = false;
        val=50;
      }
	}

	return 0;
}
