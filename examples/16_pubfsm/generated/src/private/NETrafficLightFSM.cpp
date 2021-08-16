
//////////////////////////////////////////////////////////////////////////
// Begin generate NETrafficLightFSM.cpp file
//////////////////////////////////////////////////////////////////////////

/************************************************************************
 * (c) copyright    2021
 *                  Generated by AREGtech code generator tool.
 * Generated at     16.08.2021  12:45:39 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file			generated/src/private/NETrafficLightFSM.cpp
 * \ingroup         TrafficLight State Machine
 * \brief      		This is an automatic generated code of TrafficLight namespace.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "generated/src/private/NETrafficLightFSM.hpp"

//////////////////////////////////////////////////////////////////////////
// NETrafficLightFSM Namespace implementation
//////////////////////////////////////////////////////////////////////////

namespace NETrafficLightFSM
{


//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////

	/**
     * Returns human readable string of events
     **/
    const char* const getString( const NETrafficLightFSM::eFsmEventValue & eventValue )
    {
    	switch ( eventValue )
      	{
      	CASE_MAKE_STRING(NETrafficLightFSM::EVENT_StartTrafficLight);
      
		CASE_DEFAULT("ERR: Unexpected NETrafficLightFSM::eFsmEventValue event value!");
      	}
   	}

//////////////////////////////////////////////////////////////////////////
// Timers
//////////////////////////////////////////////////////////////////////////

	/**
     * Returns human readable string of timers
     **/
    const char* const getString(const NETrafficLightFSM::eFsmTimerValue & timerValue) 
   	{
    	switch ( timerValue )
      	{

		CASE_MAKE_STRING(NETrafficLightFSM::Red);

		CASE_MAKE_STRING(NETrafficLightFSM::YellowRed);

		CASE_MAKE_STRING(NETrafficLightFSM::Green);

		CASE_MAKE_STRING(NETrafficLightFSM::YellowGreen);

		CASE_MAKE_STRING(NETrafficLightFSM::PedestrianWalk);

		CASE_MAKE_STRING(NETrafficLightFSM::VehicleWait);

        CASE_DEFAULT("ERR: Unexpected NETrafficLightFSM::eFsmTimerValue timer value!");
      	}
   	}

};

//////////////////////////////////////////////////////////////////////////
// End generate NETrafficLightFSM.cpp file
//////////////////////////////////////////////////////////////////////////
