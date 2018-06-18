//
// 2018 Rico Schulz
//
#pragma once
#include <iostream>
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
#define INTERVALL 8000
#endif

using namespace ghpi;

int main() {
  
    wiringPiSetup();
    Operator ghoperator;
    
    #ifdef DEBUG
      // Instantiate some devices for testing
      DHT22 dht;
      ADConverter adcon(14, 12, 13, 10, 0, 1023);
      Hygrometer hygro(&adcon, 7);
      LDR ldr1(&adcon, 0);
      LDR ldr2(&adcon, 1);
      Servo servo(270, 3000);
      Pump pump;
      Actuator lamp("Lamp");
      Actuator fan("Fan");
      LCDDisplay lcd;
      
      // Write welcome Screen to display
      lcd.ClrLcd();
      lcd.lcdLoc(ghpi::LCDLINES::LINE2);
      lcd.writeLine("GreenHousePi V1.0");
      
      ghoperator.Set_LCDDisplay(&lcd);
      
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
      Constraint temp("TempBelow60°C", EnvironmentValueStrings[EnvironmentValue::TEMPERATURE],
                        60.f, ConstraintConditionStrings[ConstraintCondition::BELOW]);
      Constraint hum("HumBelow80%", EnvironmentValueStrings[EnvironmentValue::HUMIDITY],
                        80.f, ConstraintConditionStrings[ConstraintCondition::BELOW]);
      Constraint lightlow("LightIntOver300", EnvironmentValueStrings[EnvironmentValue::LIGHT_INTENSITY],
                        300.f, ConstraintConditionStrings[ConstraintCondition::OVER]);
      Constraint lighthigh("LightIntUnder300", EnvironmentValueStrings[EnvironmentValue::LIGHT_INTENSITY],
                        300.f, ConstraintConditionStrings[ConstraintCondition::BELOW]); 
                        
      // Creating Actions for Devices
      Action open_roof("OpenRoof", ActionFn::AFN_ON, NULL);
      Action close_roof("CloseRoof", ActionFn::AFN_OFF, NULL);
      Action turn_on_light("TurnOnLight", ActionFn::AFN_ON, NULL);
      Action turn_off_light("TurnOffLight", ActionFn::AFN_OFF, NULL);
      
      // Register Actions on devices
      servo.RegisterAction(open_roof);
      servo.RegisterAction(close_roof);
      lamp.RegisterAction(turn_off_light);
      lamp.RegisterAction(turn_on_light);
      
      // Register Constraints
      ghoperator.RegisterConstraint(temp, close_roof);
      ghoperator.RegisterConstraint(hum, open_roof);
      ghoperator.RegisterConstraint(lightlow, turn_on_light);
      ghoperator.RegisterConstraint(lighthigh, turn_off_light);

      
      ghoperator.PrintDevices();
      ghoperator.PrintConstraints();
      std::cout << "Start running operator" << std::endl;
      std::cout << "Running in intervalls of " << INTERVALL << " milliseconds" << std::endl;
      while (true) {
        ghoperator.Run();
        delay(INTERVALL);
      }
   
    #endif
    
    return 0;
}