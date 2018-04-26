#define DEBUG true
#define SILENT true //silent mode, do not activate the pump or fan at night

#define WATERING_ACTIVE false
#define FAN_ACTIVE false
#define LIGHT_ACTIVE true

#define CDWN_POS_FAN 1
#define CDWN_POS_PUMP 2
#define CDWN_POS_LOG 3
#define CDWN_POS_LIGHT 4
#define CDWN_POS_SOIL 5

//The CDWN (cooldown) Pos specifies the cooldown position in the cooldown file
//because all cooldowns sharing one file so we need to know the index for accessing the right one