/*++

For Reference :

http://aryvandaar.swordcoast.net/~elven/check_all_transitions.rb.html
http://valera-ext.nynaeve.net/~skywing/nwn2/NWModuleLoader.h
http://valera-ext.nynaeve.net/~skywing/nwn2/NWModuleLoader.cpp

Stopped : 

Todo :
 report InvalidTransitionsList
--*/

#include "Precomp.h"
#include "../NWN2DataLib/TextOut.h"
#include "../NWN2DataLib/ResourceManager.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include "./ListModuleAreas.h"




int
__cdecl
main(
	__in int argc,
	__in_ecount( argc ) const char * * argv
)
{
	const char * ModuleName;
	const char * NWN2Home;
	const char * InstallDir;

	PrintfTextOut   TextOut;
	ResourceManager ResMan( &TextOut );
	

//   int index = 1;     
//   AreaList[index] = "The second";
//     for ( it=AreaList.begin() ; it != AreaList.end(); it++ )
//   			TextOut.WteText(char *) it->second );
//   return 0;          

	//
	// First, check that we've got the necessary arguments.
	//

	if (argc < 4)
	{
		wprintf(
			L"Usage: %S <module> <nwn2 home directory> <nwn2 install directory>\n",
			argv[ 0 ] );

		return 0;
	}

	ModuleName = argv[ 1 ];
	NWN2Home   = argv[ 2 ];
	InstallDir = argv[ 3 ];

	//
	// Now spin up a resource manager instance.
	//

//	PrintfTextOut   TextOut;
//	ResourceManager ResMan( &TextOut );



	try
	{
	

		ResMan.LoadModuleResources(
			ModuleName,
			"",
			NWN2Home,
			InstallDir,
			std::vector< NWN::ResRef32 >( )
			);

	
		DemandResourceStr                ModuleIfoFile( ResMan, "module", NWN::ResIFO );
		GffFileReader                    ModuleIfo( ModuleIfoFile, ResMan );
		const GffFileReader::GffStruct * RootStruct = ModuleIfo.GetRootStruct( );
		std::string                      ModName;
		GffFileReader::GffStruct         Struct;
		NWN::ResRef32 AreaResRef;

		if (RootStruct->GetCExoLocString( "Mod_Name", ModName ))
			TextOut.WriteText( "The module name is: %s.\n", ModName.c_str( ) );
			
  TextOut.WriteText( "The module name is: %s.\n", ModName.c_str( ) );

		// Report
    TextOut.WriteText( "# 1. Recording  areas data and objects \n");
    
    // First pass 
		for (size_t i = 0; i <= ULONG_MAX; i += 1)
		{

			if (!RootStruct->GetListElement( "Mod_Area_list", i, Struct ))
				break;

			if (!Struct.GetResRef( "Area_Name", AreaResRef ))
				throw std::runtime_error( "Mod_Area_list element is missing Area_Name.");

      // Now reference each area in AreasList
       RecordAreaData( AreaResRef, ResMan, &TextOut, i  );
       
       
       // Reference all objects from module in ObjectsList
 		   RecordAreasObjects( AreaResRef, ResMan, &TextOut,  i );
 		   
 		   // Count Areas for Research cap
 		   AreaCount++;
 		   
 	  	// Report
 	    TextOut.WriteText( ".");
	   
		}
		
		// Report
    TextOut.WriteText( "Done \n");


		
		// Report
    TextOut.WriteText( "# 2. Recording area transitions\n" );
	
    // Second pass 
		for (size_t i = 0; i <= ULONG_MAX; i += 1)
		{
			if (!RootStruct->GetListElement( "Mod_Area_list", i, Struct ))
				break;

			if (!Struct.GetResRef( "Area_Name", AreaResRef ))
			throw std::runtime_error( "Mod_Area_list element is missing Area_Name.");
			
      // Now reference each transition in NodesList
      RecordTransitions( AreaResRef, ResMan, &TextOut, i  );
        
      // Report
      TextOut.WriteText( ".");
	
		}
		
		// Report
    TextOut.WriteText( "Done \n");
		
		// 
		// Now compute each node using area list and node list
		//
    TextOut.WriteText( "# 3. Building Nodes Weights.\n");
		SetNodesWeights(&TextOut);
		
		// 
		// Now compute the pathtable
		// 
   TextOut.WriteText( "# 4. Building the pathtable.\n");
		SetPathTable(&TextOut);
		
		ExportPathTable(ModName, &TextOut);
		
//		ReportInvalidTransitions(&TextOut);
		
	}
	catch (std::exception &e)
	{
		//
		// Simple print an error message and abort if we went wrong, such as if
		// we couldn't load the module.
		//

		TextOut.WriteText( "ERROR: Exception '%s'.\n", e.what( ) );
		
	}
/*
 for ( AreasListIt=AreasList.begin() ; AreasListIt != AreasList.end(); AreasListIt++ ){
  TextOut.WriteText(  "Area #%i : %s .\n",  AreasListIt->first,AreasListIt->second.c_str( ) ); }

  
 for ( ObjectsListIt=ObjectsList.begin() ; ObjectsListIt != ObjectsList.end(); ObjectsListIt++ ){
	  TextOut.WriteText(  "Object #%s : %i \n",  ObjectsListIt->first.c_str( ),ObjectsListIt->second  ); }

 for ( ITLIt=InvalidTransitionsList.begin() ; ITLIt != InvalidTransitionsList.end(); ITLIt++ ){
	  TextOut.WriteText(  "Invalid Transition #%s\n",  (*ITLIt).c_str( ) ); }
 */ 
/*  	  
for ( WeightedNodesListIt=WeightedNodesList.begin() ; WeightedNodesListIt != WeightedNodesList.end(); WeightedNodesListIt++ ){
    TextOut.WriteText(  "(SWN:final) #%i : %i -> %i \n",  WeightedNodesListIt->first,WeightedNodesListIt->second.Transitions.size( ),
    WeightedNodesListIt->second.Weight );  }

  */
/*
for ( InitListIt=InitList.begin() ; InitListIt != InitList.end(); InitListIt++ ){
  
 TextOut.WriteText("([InitList] %i IsInt %i IsUnd %i IsNat %i Size %i.\n", 
  InitListIt->first,
  InitListIt->second.IsInterior,
  InitListIt->second.IsUnderground,
  InitListIt->second.IsNatural,
  InitListIt->second.Size
 ); }
 */
/*
for ( PathTableIt=PathTable.begin() ; PathTableIt != PathTable.end(); PathTableIt++ ){
  
 TextOut.WriteText("([PathTable] %i : Weight %i Transitions %i .\n", 
  PathTableIt->first,
  PathTableIt->second.Weight,
  PathTableIt->second.Transitions.size( ) 
 ); }
 
 */

/* 
for ( AdjacentNodesListIt=AdjacentNodesList.begin() ; AdjacentNodesListIt != AdjacentNodesList.end(); AdjacentNodesListIt++ )
{
  
  std::set<int> TheList = AdjacentNodesListIt->second;
  std::set<int>::iterator it;
  int AreaFrom = AdjacentNodesListIt->first;
  
  for( it=TheList.begin(); it!=TheList.end();it++ )
  {
    TextOut.WriteText("([AdjacentNodesList] %i : %i \n", 
    AreaFrom,
    *it
    ); 
  }
}
 */
 
 /*  
  for( IncrementalAdjacentNodesIt = IncrementalAdjacentNodes.begin();
      IncrementalAdjacentNodesIt != IncrementalAdjacentNodes.end();
      IncrementalAdjacentNodesIt++ )
  {
    TextOut.WriteText( "\nA. IncrementalAdjacentNodesIt : %i\n", IncrementalAdjacentNodesIt->first );

    std::map<int, std::set<int>>::iterator It;
    for( It = IncrementalAdjacentNodesIt->second.begin();
        It != IncrementalAdjacentNodesIt->second.end();
        It++ ){
        
        TextOut.WriteText( "\n  B. It : %i", It->first );  
        TextOut.WriteText( "    C. I2 : ");
        std::set<int>::iterator I2;
        for( I2= It->second.begin(); I2!=It->second.end(); I2++ ){
        
          TextOut.WriteText( "%i ", *I2 );  
        }
        
        }
  
  
  }
*/

	TextOut.WriteText( "\nEnd. Mission Accomplished\n" );

	return 0;
}



/*
FTP.exe "Rural-Inn" "C:\Documents and Settings\albancrommer\Mes documents\Neverwinter Nights 2" "C:\Program Files\Atari\Neverwinter Nights 2"
FTP.exe "FTP-FastTransareaPathfinder" "C:\Documents and Settings\albancrommer\Mes documents\Neverwinter Nights 2" "C:\Program Files\Atari\Neverwinter Nights 2"
FTP.exe "ms" "C:\Documents and Settings\albancrommer\Mes documents\Neverwinter Nights 2" "C:\Program Files\Atari\Neverwinter Nights 2"
FTP.exe "witchwood" "C:\Documents and Settings\albancrommer\Mes documents\Neverwinter Nights 2 p" "C:\Program Files\Atari\Neverwinter Nights 2"
FTP.exe "Rykil" "C:\Documents and Settings\albancrommer\Mes documents\Neverwinter Nights 2 k" "C:\Program Files\Atari\Neverwinter Nights 2"

*/