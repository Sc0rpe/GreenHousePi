all:	togglefan togglelamp toggleservo

togglefan:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglefan.cc ../action.cc -o togglefan
togglelamp:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglelamp.cc ../action.cc -o togglelamp
toggleservo:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_toggleservo.cc ../action.cc -o toggleservo
togglepump:
	g++ -std=c++0x -I/usr/local/inclu -I../ -I/usr/local/include/boost -L/usr/local/lib -lwiringPi -lrt modules/shm_togglepump.cc ../action.cc -o togglepump
install:
    mkdir /var/www/html/modules
    mkdir /var/www/html/images
    cp togglefan /var/www/html/modules
    cp togglelamp /var/www/html/modules
    cp toggleservo /var/www/html/modules
    cp index.html /var/www/html
    cp togglefan.php /var/www/html
    cp toggleservo.php /var/www/html
    cp togglelamp.php /var/www/html