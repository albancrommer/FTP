// i_it_teleporter_ac
/*
   Template for an Activate item script.
   This script will run each time an item's "custom activation" is used.
   
   How to use this script:
   Item needs an item property that will cause a custom activation such as Cast Spell:Unique Power
   Replace the word "temp" (in line 1) with the tag of the item.  Rename the script with this name.  
    
   Additional Info:
   In general, all the item "tag-based" scripts will be named as follows:
   - a prefix ("i_" by defualt)
   - the tag of the item
   - a postfix indicating the item event.
   
   This script will be called automatically (by defualt) whether it exists or not.  If if does not exist, nothing happens.
   
   Note: this script runs on the module object, an important consideration for assigning actions.
      -ChazM
*/
// 1102_Alba

#include "ginc_debug"

void main()
{
PrettyDebug( "Activated" );

    object oPC      = GetItemActivator();
    object oItem    = GetItemActivated();
    object oTarget  = GetItemActivatedTarget();
    location lTarget = GetItemActivatedTargetLocation();

 	object 	oNPC	=  GetObjectByTag( "n_aldanon");
	if( !GetIsObjectValid( oNPC ) ){
		PrettyDebug( "Oops, Aldanon is missing :]" );
	}
	
	AssignCommand( oPC, ActionJumpToObject( oNPC ) );

}