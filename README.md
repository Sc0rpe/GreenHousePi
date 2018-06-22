
# GreenHousePi
This Project is from four students of the [Technische Universität Bergakademie Freiberg](https://tu-freiberg.de) in germany.
It was created for the subject "application of information and automizationsystems".
The goal of this project is to build an autonomous greenhouse with different features using a raspberry pi.

Desired features:
* Tracking of Temperature, Humidity, Soil Moisture and Light Intensity
* Ability to open the roof for cooling and ventilation
* Turning on the lights if the available light intensity is too low
* Ventilate with the Fan if the humidity is too high
* Water if the soil moisture is too low
* Ability to be operated over a backend
* Displaying values over a LCD-Screen

# Installation
1. Download the latest raspbian image from https://www.raspberrypi.org/downloads/raspbian/
    (light image because of performance recommended)
2. Install it
3. Run
	`sudo raspi-config` 
    and enable the I2C Interface under advanced options
5. Dowload the ghpi-library, therefore run 
    `git clone https://github.com/Sc0rpe/GreenHousePi`
6. Go into the folder `ghpi` and run
    `make main`
    Output will be a file `ghpi.o`
7. You can run it with
    `sudo ./ghpi.o`
8. If you want it to start after boot automaticaly add
    `./path/to/ghpi.o`
    to the end of the file /etc/rc.local
9. Also you can copy the binary into the folder `/usr/local/bin` so you can run it from everywhere by typing `sudo ghpi.o`

# Setting up the Webinterface
1. Run 
    ```
    sudo apt-get update
    sudo apt-get upgrade
    ```
2. Install the `lighttpd` Webserver
    ```
    sudo apt-get install lighttpd
    ```
3. Install `php7.0`
    ```
    sudo apt-get install php7.0
    ```
4. Activate the FastCGI Modul for php and restart the server
    ```
    sudo lighty-enable-mod fastcgi
    sudo lighty-enable-mod fastcgi-php
    sudo service lighttpd force-reload
    ```
5. Add the user `www-data` to the sudoers so it can run the software modules.
    Therefore run `sudo visudo` and add the line `www-data ALL=(ALL) NOPASSWD: ALL` to it.
    Exit with `[CTRL] + [X]`, confirm with `[Y]` and press `[ENTER]`.
6. Go into the folder `GreenHousePi/ghpi/webscripts` and run:
    ```
    make
    sudo make install
    ```
    This will compile the modules and copy them, the html and php files into the root directory of the webserver. By default this is `/var/www/html` for lighttpd. If this is different for you, you have to copy them manually or change the concerning lines in the makefile.

# Adding your own Sensor
1. Create a new class and inherit from `ghpi::Sensor`
2. Override the `Run` and `get_values` function
4. Create an instance of your class, create some instances of Pin for your device and register them using the `RegisterPin()` function
5. Register your instance to the Operator using `RegisterDevice()`
6. For better understanding have look at the `dht22.cpp` file
7. Don't forget to add your newly created file to the makefile for the main target, otherwise it won't be linked if your run `make main`

# Adding your own Actuator
1. Create a new class and inherit from `ghpi::Actuator`
2. Override `TurnOn` `TurnOff` `Toggle` and if needed the `Run` function 
3. Create instances of `ghpi::Action` and register the actions your Actuator can execute by using `RegisterAction()`
4. Connect them by registering them with a Constraint on the operater using `RegisterConstraint`
5. Don't forget to add your newly created file to the makefile for the main target, otherwise it won't be linked if your run `make main`

