#include "ftpath_library"

void main() {


	// This is the standard NWN2 Module on load script
	ExecuteScript("x2_mod_def_load", GetModule() );

	// Build our Areas ID cache for Fast Transarea Pathfinding. YAY!
	ftp_BuildCache();
	
	
}