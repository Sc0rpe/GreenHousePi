//
// 2018 Rico Schulz
//
#pragma once
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <boost/log/trivial.hpp>
#include "operator.h"
#include "dht22.h"
#include "ldr.h"
#include "hygrometer.h"
#include "servo.h"
#include "pump.h"
#include "debug.h"


#ifdef DEBUG
#define SERVO_PIN 1
#define DHT_PIN 6
#define HYGRO_PIN 42
#define PUMP_PIN 42
#define LAMP_PIN 16
#define FAN_PIN 5
#define INTERVALL 10000
#endif

using namespace ghpi;

bool STOP = false;

// Set STOP to true, so we will break
// out of the infinite loop and exit the main function
// this will call the destructors of our object and the pins will
// set to their default init state
void exiting(int i) {
	STOP = true;
}

int main() {
  
    wiringPiSetup();
    Operator ghoperator;
    
    signal(SIGABRT, exiting);
    signal(SIGTERM, exiting);
    //signal(SIGTSTP, exiting);
    signal(SIGINT, exiting);	
    
    // Instantiate some devices for testing
    DHT22 dht;
    ADConverter adcon(14, 12, 13, 10, 0, 1023);
    Hygrometer hygro(&adcon, 7);
    LDR ldr1(&adcon, 0);
    LDR ldr2(&adcon, 1);
    Servo servo(270, 0);
    Pump pump;
    Actuator lamp("Lamp");
    Actuator fan("Fan");
    LCDDisplay* lcd = nullptr;
      
    try {
      // Write welcome screen to display
      lcd = new LCDDisplay();
      lcd->writeLine("GreenHousePi V1.0", 1);
      lcd->writeLine("Made by", 2);
      lcd->writeLine(" Tobi          Pei ", 3);
      lcd->writeLine(" Rico        Marcos", 4);
    }
    catch (...) {
      std::cout << "LCD has thrown an error" << std::endl;
    }
      
    ghoperator.Set_LCDDisplay(lcd);
      
    Pin dht_data_pin(DHT_PIN, PinMode::input, PinState::low);
    Pin hygro_pin(HYGRO_PIN, PinMode::output, PinState::high);
    Pin servo_pin(SERVO_PIN, PinMode::pwmoutput, PinState::low);
    Pin pump_pin(PUMP_PIN, PinMode::output, PinState::high);
    Pin lamp_pin(LAMP_PIN, PinMode::output, PinState::high);
    Pin fan_pin(FAN_PIN, PinMode::output, PinState::high);
      
      
    // Register pins for the devices
    dht.RegisterPin(&dht_data_pin, PinUsage::BI_DATA, OnState::OS_NONE);
    hygro.RegisterPin(&hygro_pin, PinUsage::SWITCH, OnState::OS_LOW);
    servo.RegisterPin(&servo_pin, PinUsage::PWM, OnState::OS_NONE);
    pump.RegisterPin(&pump_pin, PinUsage::SWITCH, OnState::OS_LOW);
    lamp.RegisterPin(&lamp_pin, PinUsage::SWITCH, OnState::OS_LOW);
    fan.RegisterPin(&fan_pin, PinUsage::SWITCH, OnState::OS_LOW);
      
    // Register Devices
    ghoperator.RegisterDevice(&dht);
    ghoperator.RegisterDevice(&hygro);
    ghoperator.RegisterDevice(&servo);
    ghoperator.RegisterDevice(&pump);
    ghoperator.RegisterDevice(&ldr1);
    ghoperator.RegisterDevice(&ldr2);
    ghoperator.RegisterDevice(&lamp);
    ghoperator.RegisterDevice(&fan);

    // Creating Constraints
    Constraint temp("TempBelow18°C", EnvironmentValueStrings[EnvironmentValue::TEMPERATURE],
                      18.f, ConstraintConditionStrings[ConstraintCondition::BELOW]);
    Constraint hum("HumBelow80%", EnvironmentValueStrings[EnvironmentValue::HUMIDITY],
                      80.f, ConstraintConditionStrings[ConstraintCondition::BELOW]);
    Constraint lightlow("LightIntOver300", EnvironmentValueStrings[EnvironmentValue::LIGHT_INTENSITY],
                      300.f, ConstraintConditionStrings[ConstraintCondition::OVER]);
    Constraint lighthigh("LightIntUnder500", EnvironmentValueStrings[EnvironmentValue::LIGHT_INTENSITY],
                      500.f, ConstraintConditionStrings[ConstraintCondition::BELOW]); 
                        
    // Creating Actions for Devices
    Action open_roof("OpenRoof", ActionFn::AFN_ON, NULL);
    Action close_roof("CloseRoof", ActionFn::AFN_OFF, NULL);
    Action turn_on_light("TurnOnLight", ActionFn::AFN_ON, NULL);
    Action turn_off_light("TurnOffLight", ActionFn::AFN_OFF, NULL);
    Action turn_on_fan("TurnOnFan", ActionFn::AFN_ON, NULL);
    Action toggle_servo("ToggleServo", ActionFn::AFN_TOGGLE, NULL);
    Action toggle_lamp("ToggleLamp", ActionFn::AFN_TOGGLE, NULL);
    Action toggle_fan("ToggleFan", ActionFn::AFN_TOGGLE, NULL);
		Action op_man("OperateManually", ActionFn::AFN_OP_MANU, NULL);
		Action op_auto("OperateAutonomous", ActionFn::AFN_OP_AUTO, NULL);
      
    // Register Actions on devices
    servo.RegisterAction(open_roof);
    servo.RegisterAction(close_roof);
    servo.RegisterAction(toggle_servo);
    fan.RegisterAction(turn_on_fan);
    fan.RegisterAction(toggle_fan);
    lamp.RegisterAction(turn_off_light);
    lamp.RegisterAction(turn_on_light);
    lamp.RegisterAction(toggle_lamp);
		servo.RegisterAction(op_man);
		lamp.RegisterAction(op_man);
		fan.RegisterAction(op_man);
		servo.RegisterAction(op_auto);
		lamp.RegisterAction(op_auto);
		fan.RegisterAction(op_auto);
      
    // Register Constraints
    ghoperator.RegisterConstraint(temp, open_roof);
    ghoperator.RegisterConstraint(hum, turn_on_fan);
    ghoperator.RegisterConstraint(lightlow, turn_on_light);
    ghoperator.RegisterConstraint(lighthigh, turn_off_light);

    // Initialize Devices
    servo.Initialize();
      
    ghoperator.PrintDevices();
    ghoperator.PrintConstraints();
    std::cout << "Start running operator" << std::endl;
    std::cout << "Running in intervalls of " << INTERVALL << " milliseconds" << std::endl;
      
    while (!STOP) {
      ghoperator.Run();
      delay(INTERVALL);
    }
    std::cout << "Exiting..." << std::endl;
    lcd->ClrLcd();
    lcd->writeLine("Goodbye...", 2);
    delay(2000);
    lcd->ClrLcd();
    delete lcd;
  
    
    return 0;
}