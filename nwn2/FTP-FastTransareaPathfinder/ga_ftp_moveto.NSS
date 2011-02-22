// ga_ftp_moveto
/*
	Description: Directs NPC to object with sDestination tag
	
	Parameters:
		sTarget -  Tag of the target object.  Default is OWNER.

*/
// Name_Date

#include "ginc_param_const"
#include "ftpath_library"

void main(string sTarget)
{

	object oNPC 			= OBJECT_SELF;
	
	object oTarget 			= GetObjectByTag( sTarget );

	// If debug text is on, this will let you know the script fired.
	// PrettyDebug("Script Fired on : " + GetName(oNPC) + " aiming at "+sTarget);
	
	ftp_SetTarget( oNPC, "" );
	ftp_ActionForceMoveToObject( oTarget, TRUE, oNPC );

}