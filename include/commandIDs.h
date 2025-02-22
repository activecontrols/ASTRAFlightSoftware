/*
commandIDs.h - Macros of ids for commands
Description: List out macros for commands (int32s) used by flightRouter callbacks
Author: Francis Ozua
Last Updates: Feb 22 2025
*/

/*
How to add command:

flightData::router->registerRoute(TEST_TELEM_COMMAND_ID, std::bind(&CommsManager::testTelemCommand, this, std::placeholders::_1));

This is how I registed CommsManager::testTelemCommand in CommsManger::init() function. (line 70 of comms/Comms.cpp)

Things to note:
1. Register in init() function (easier to keep track of commands)
2. List your command id here (we can have up to whatever 2^(sizeof int32) is)
3. Register that hoe like I did above
4. thanks to the c++ maintainer legends, we can easily register with std::bind

Why not use callbacks instead of std::function and std::bind?
Could not figure it out in under 15 minutes so I gave up. Maybe one day, you, the reader, will decide
to clean up this! Find a way. Defeat the problem. Get some sleep. Take a walk. Keep yourself healthy.

*/

#define TEST_TELEM_COMMAND_ID 0
#define LAND_COMMAND_ID 1
#define CALL_FRANCIS_AMAZING_COMMAND_ID 2
#define ASK_FRANCIS_Y_HE_KING_COMMAND_ID 3
#define I_LOVE_VP_COMMAND_ID 4
#define LEAP_COMMAND_COMMAND_ID 5
#define WOOF_COMMAND_ID 6