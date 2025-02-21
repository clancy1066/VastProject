#pragma once
// From the problem description
#define SIM_MAX_TIME_IN_HOURS_F			72.0f
#define SIM_TRAVEL_TIME_IN_MINUTES_F	30.0f
#define SIM_WAIT_UNLOAD_IN_MINUTES_F	5.0f
#define SIM_MIN_EXTRACT_TIME_IN_HOURS_F	1.0f
#define SIM_MAX_EXTRACT_TIME_IN_HOURS_F	5.0f

// Helpers for readability later on
#define SIM_MAX_TIME_IN_SECONDS_F			(SIM_MAX_TIME_IN_HOURS_F*60.0f*60.0f)
#define SIM_WAIT_UNLOAD_IN_SECONDS_F		(SIM_WAIT_UNLOAD_IN_MINUTES_F*60.0f)
#define SIM_TRAVEL_TIME_IN_SECONDS_F		(SIM_TRAVEL_TIME_IN_MINUTES_F*60) 
#define SIM_MIN_EXTRACT_TIME_IN_SECONDS_F	(SIM_MIN_EXTRACT_TIME_IN_HOURS_F*60.0f*60.0f)
#define SIM_MAX_EXTRACT_TIME_IN_SECONDS_F	(SIM_MAX_EXTRACT_TIME_IN_HOURS_F*60.0f*60.0f)
#define SIM_MIN_MISSION_IN_SECONDS_F		(2.0f*SIM_TRAVEL_TIME_IN_SECONDS_F+SIM_MIN_EXTRACT_TIME_IN_SECONDS_F+SIM_WAIT_UNLOAD_IN_SECONDS_F)
#define SIM_MAX_MISSION_IN_SECONDS_F		(2.0f*SIM_TRAVEL_TIME_IN_SECONDS_F+SIM_MAX_EXTRACT_TIME_IN_SECONDS_F+SIM_WAIT_UNLOAD_IN_SECONDS_F)

// Used for accelerated delta time. 1 minute increment
#define	SIM_EVAL_GRANULAROTY_SECONDS_F		(SIM_WAIT_UNLOAD_IN_MINUTES_F*60.0f)

// Used for waits between simulation loops
#define SIM_WAIT_FRAME_IN_MILLISECONDS		(500)