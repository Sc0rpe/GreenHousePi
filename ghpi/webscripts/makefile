all:	togglefan togglelamp

togglefan:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglefan.cc ../action.cc -o togglefan
togglelamp:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglelamp.cc ../action.cc -o togglelamp
togglepump:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglepump.cc ../action.cc -o togglepump
