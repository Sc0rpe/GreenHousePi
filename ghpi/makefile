all :	device sensor pump actuator analogsensor ldr hygrometer dht operator pin servo action main

device:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi device.cc
sensor: 
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi sensor.cc
pump:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi pump.cc
actuator:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi actuator.cc
analogsensor:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi analogsensor.cc
adconverter:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi adconverter.cc
ldr:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi ldr.cc
hygrometer:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi hygrometer.cc
dht:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi dht22.cc
operator:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi operator.cc
constraint:
	g++ -c -std=c++0x constraint.cc
pin:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi pin.cc
servo:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi servo.cc
action:
	g++ -c -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi action.cc
main : 
	g++ -std=c++0x -I/usr/local/inclu -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt ghpi.cc operator.cc device.cc sensor.cc pump.cc actuator.cc analogsensor.cc ldr.cc hygrometer.cc dht22.cc pin.cc servo.cc action.cc constraint.cc adconverter.cc -o ghpi.o
