all :	main Soil Writer DHT

main :
	g++ -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi -o Hydro.out main.cpp ds1302.c
Soil :
	g++ -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi -o Soil.out Soil.cpp
DHT: 
	g++ -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi -o DHT.out dht22.cpp
BtnShutdown:
	g++ -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi -o ButtonShutdown.out buttonshutdown.cpp
BtnFanControl:
	g++ -std=c++0x -lwiringPi -o ButtonFanControl.out buttonFanControl.cpp
Pump:
	g++ -std=c++0x -I/usr/local/inclu -L/usr/local/lib -lwiringPi -o Pump.out Pump.cpp

