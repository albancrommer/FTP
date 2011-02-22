// ftpath_conf
/*
	Description: Fast Transarea Pathfinding System
	
	
*/
// 110219_Alban



// Delay for recursive script to activate creature
// the smaller the more responsive
// PWs might want to keep it at 5.9 for perf reasons
const float 	FTP_CREATURE_DELAY		= 1.4;

// Name of the areas Variable recording area_id 
const string 	AREA_ID					= "FTP_AREA_ID";

// Delay for doors to close automatically
const float 	DELAY_DOORS				= 15.0;

// Distance for doors to open automatically
const float 	DISTANCE_DOORS			= 3.0;

// Name of the 2DA containing the Path Table ex: ftp_%modulename%_path
const string 	FTP_PATH_2DA			= "ftp_FTPFastTransareaPath_path";

// Name of the 2DA containing the Horizontal area list ex: ftp_%modulename%_list_h
const string 	FTP_LIST_H_2DA			= "ftp_FTPFastTransareaPath_list_h";

// Name of the 2DA containing the Vertical area list ex: ftp_%modulename%_list_v
const string 	FTP_LIST_V_2DA			= "ftp_FTPFastTransareaPath_list_v";

// Name of the creatures Variable recording elapsed rounds at position 
const string 	VAR_ROUNDS				= "FTP_ROUNDS";

// Name of the creatures Variable recording position 
const string 	PREVIOUS_LOCATION		= "FTP_PREV_LOC";

// Name of the creatures Variable recording current target
const string 	NPC_TARGET				= "FTP_TARGET";

// Name of the creatures Variable recording current target
const string 	FTP_TARGET				= "FTP_TARGET";

