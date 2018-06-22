all:  togglefan togglelamp toggleservo
togglefan:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglefan.cc ../action.cc -o togglefan
togglelamp:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglelamp.cc ../action.cc -o togglelamp
toggleservo:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_toggleservo.cc ../action.cc -o toggleservo
togglepump:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglepump.cc ../action.cc -o togglepump
operatefan:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_operatefan.cc ../action.cc -o operatefan
operatelamp:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_operatelamp.cc ../action.cc -o operatelamp
operateservo:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_operateservo.cc ../action.cc -o operateservo
install:
	mkdir -p /var/www/html/modules
	mkdir -p /var/www/html/images
	cp togglefan /var/www/html/modules
	cp togglelamp /var/www/html/modules
	cp toggleservo /var/www/html/modules
	cp operateservo /var/www/html/modules
	cp operatelamp /var/www/html/modules
	cp operatefan /var/www/html/modules
	cp index.html /var/www/html
	cp togglefan.php /var/www/html
	cp toggleservo.php /var/www/html
	cp togglelamp.php /var/www/html
	cp operatelamp.php /var/www/html
	cp operateservo.php /var/www/html
	cp operatefan.php /var/www/html
	cp images/fanON.png /var/www/html/images
	cp images/pump.png /var/www/html/images
	cp images/lampON.png /var/www/html/images
	cp images/servo.png /var/www/html/images
