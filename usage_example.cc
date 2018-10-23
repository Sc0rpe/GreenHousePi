Operator ghoperator;

Actuator lamp("Lamp");

Pin lamp_pin(LAMP_PIN, PinMode::output, PinState::high);

lamp.RegisterPin(&lamp_pin, PinUsage::SWITCH, OnState::OS_LOW);

ghoperator.RegisterDevice(&lamp);

Action turn_on_lamp("TurnOnLight", ActionFn::AFN_ON, NULL);
Action turn_off_lamp("TurnOffLight", ActionFn::AFN_OFF, NULL);
Action toggle_lamp("ToggleLamp", ActionFn::AFN_TOGGLE, NULL);

lamp.RegisterAction(turn_off_lamp);
lamp.RegisterAction(turn_on_lamp);
lamp.RegisterAction(toggle_lamp);

Constraint lightlow("LightIntOver300", EnvironmentValueStrings[EnvironmentValue::LIGHT_INTENSITY],
                300.f, ConstraintConditionStrings[ConstraintCondition::OVER]);
Constraint lighthigh("LightIntUnder300", EnvironmentValueStrings[EnvironmentValue::LIGHT_INTENSITY],
                300.f, ConstraintConditionStrings[ConstraintCondition::BELOW]); 
                
ghoperator.RegisterConstraint(lighthigh, turn_off_lamp);
ghoperator.RegisterConstraint(lightlow, turn_on_lamp);