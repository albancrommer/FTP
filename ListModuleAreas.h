enum Debug                  { DEV, PROD };
#define DEBUG               0
#define WEIGHT_INTERIOR     8
#define WEIGHT_UNDERGROUND  8
#define WEIGHT_NATURAL      8
#define WEIGHT_TRANSITION   16



std::string Implode(  std::string &glue,  std::set<std::string> pieces )
{
    std::string a;
    std::set<std::string>::iterator   SetIt;
    int leng=pieces.size();
    int i=0;
    for(SetIt=pieces.begin();SetIt!=pieces.end();SetIt++ )
    {
        a+= *SetIt;
        if (  i < (leng-1) ){
            a+= glue;
        }
      i++;
    }
    return a;
}




class PrintfTextOut : public IDebugTextOut
{

  FILE * fLogFile;

public:

	inline
	PrintfTextOut(
		)
	{
	
    fLogFile = fopen ("log.txt","w");
		AllocConsole( );
		
	}

	inline
	~PrintfTextOut(
		)
	{
	
		FreeConsole( );
		fclose (fLogFile);


	}


	enum { STD_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };


  
	inline
	virtual
	void
	WriteText(
		__in __format_string const char* fmt,
		...
		)
	{
		va_list ap;

		va_start( ap, fmt );
		WriteTextV( STD_COLOR, fmt, ap );
		WriteLog( fmt, ap );
		va_end( ap );
	}

	inline
	virtual
	void
	WriteText(
		__in WORD Attributes,
		__in __format_string const char* fmt,
		...
		)
	{
		va_list ap;

		va_start( ap, fmt );
		WriteTextV( Attributes, fmt, ap );
		WriteLog( fmt, ap );
		va_end( ap );

		UNREFERENCED_PARAMETER( Attributes );
	}

/*
	inline
	virtual
	void
	WriteText(
		__in bool DebugInfo,
		__in __format_string const char* fmt,
		...
		)
	{
	  // Debugging purpose, skip if production bin
	  if( DebugInfo == true && DEBUG != 1 ) return;
		va_list ap;

		va_start( ap, fmt );
		WriteTextV( STD_COLOR, fmt, ap );
		WriteLog( fmt, ap );
		va_end( ap );
	}
*/

	inline
	virtual
	void
	WriteTextV(
		__in __format_string const char* fmt,
		__in va_list ap
		)
	{
		WriteTextV( STD_COLOR, fmt, ap );
	}

	inline
	virtual
	void
	WriteTextV(
		__in WORD Attributes,
		__in const char *fmt,
		__in va_list argptr
		)
	/*++

	Routine Description:

		This routine displays text to the log file and the debug console.

		The console output may have color attributes supplied, as per the standard
		SetConsoleTextAttribute API.

	Arguments:

		Attributes - Supplies color attributes for the text as per the standard
					 SetConsoleTextAttribute API (e.g. FOREGROUND_RED).

		fmt - Supplies the printf-style format string to use to display text.

		argptr - Supplies format inserts.

	Return Value:

		None.

	Environment:

		User mode.

	--*/
	{
		HANDLE console = GetStdHandle( STD_OUTPUT_HANDLE );
		char buf[8193];
		StringCbVPrintfA(buf, sizeof( buf ), fmt, argptr);
		DWORD n = (DWORD)strlen(buf);
		SetConsoleTextAttribute( console, Attributes );
		WriteConsoleA(console, buf, n, &n, 0);
	}
	/**
	--*/
  	
  inline
  virtual
  void
  WriteLog(
		__in const char *fmt,
		__in va_list argptr
  )
  {
  	char buf[8193];
		StringCbVPrintfA(buf, sizeof( buf ), fmt, argptr);
    fprintf  (fLogFile, buf);

  }
  
  
};



/*
// Node Class
// @AreaFrom
// @AreaTo
*/


class Node {

	public :
	
	int      AreaFrom;
	int      AreaTo;
	long     Key;
	
	Node( const int A, const int B):
	 AreaFrom(A), 
   AreaTo(B), 
   Key(A<<16|B)
   {}
	 
	Node(const long A):
  Key(A)
  {}
  
	Node()
  {}
  
  void SetKey( long K ){
    Key = K;
    AreaFrom  = GetAreaFrom();
    AreaTo    = GetAreaTo();
  }
  void SetAreas( int A, int B ){
	 AreaFrom   = A; 
   AreaTo     = B; 
   Key        = A<<16|B;
  }
  
  int GetAreaFrom(){
    return( Key >> 16 );}
    
  int GetAreaTo(){
    return( Key & 0x0000FFFF );}
  
	
	long GetKey() { return Key = AreaFrom << 16 | AreaTo; }
	
};
typedef Node NodeClass;




/*
// Path Class
*/
enum PathType            { INVALID, PATH, COMPLEX, NODE };

class PathClass{

  public :

  int                         mDepth;
  PathType                    Type;
	NodeClass                   mNode;			
	std::vector<PathClass *>    mSubNodes;
  std::vector<PathClass *>    mSubPath;			
	std::set<int>               mPreviouslySearchedAreas;
	std::set<int>::iterator     mPSAIt;
   
  PathClass(){ 
    mDepth                    = 0;
    Type                      = NODE;
  }
  
  PathClass( const int A, const int B ){ 
    mDepth                    = 0;
    Type                      = NODE;
    mNode.SetAreas( A, B );
  }
  PathClass( const int A ){ 
    mDepth                    = A;
    Type                      = NODE;
  }
  
  void SetDepth( int NewDepth ){
  
    mDepth                    = NewDepth;
    
  }
  void SetSimplePath( int LinkAreaId,__in IDebugTextOut * TextOut);

  void SetNode( __in IDebugTextOut * TextOut);
  
  void SetComplexPath( std::set<int> Solution,__in IDebugTextOut * TextOut );
  
  void AddSubPath( PathClass ThePath );
  
  std::vector< std::vector< NodeClass > > ToNodes( __in IDebugTextOut * TextOut );
  

};


typedef PathClass PathClass;

/*
// Area
// @IsUnderground
// @IsNatural
// @IsInterior
// @Size
// @Tag
// @Resref
*/

struct Area{

  public :
	bool		     	  IsUnderground;
	bool		     	  IsNatural;
	bool			      IsInterior;
	int				      Size;
	
	Area( 
    const bool A, 
    const bool B, 
    const bool C, 
    const int S  
  ):
		IsUnderground(A), 
    IsNatural(B), 
    IsInterior(C),
    Size(S)
  {}
		
	Area( ){}
	
};
typedef Area AreaSTRUCT;

/*
// WeightedNode
// @Weight
// @Transitions
*/

struct WeightedNode {

	int                    Weight;
	std::set<std::string>    Transitions;
	
	WeightedNode( ){
    Weight    = 0;
    
  }
	
};
typedef WeightedNode WeightedNodeClass;





//AreasList( int id, string areaResRef )

std::map <int,std::string>              AreasList;
std::map <int,std::string >::iterator   AreasListIt;

//ObjectsList( string tag, int area )

std::multimap <std::string, int>             ObjectsList;
std::multimap <std::string, int>::iterator   ObjectsListIt;
	
//UnlinkedAreasList( int area )

std::vector <int> UnlinkedAreasList;

//InvalidTransitionsList( int area, string sTransition )

std::vector <std::string> InvalidTransitionsList;
std::vector <std::string>::iterator ITLIt;

//InitList( int area, int size, bool IsInterior, bool IsUnderground, bool IsNatural ) //*

std::map <int, AreaSTRUCT>            InitList;
typedef std::map <int, AreaSTRUCT>::iterator  InitListIt;

//NodesList( int areaFrom, int areaTo, string sTransitions ) // *
//NodesWeightList( int areaFrom, int areaTo, int weight ) // *

std::map <int, WeightedNodeClass>             WeightedNodesList;
std::map <int, WeightedNodeClass>::iterator   WeightedNodesListIt;


//PathTable( int areaFrom, int areaTo, int weight, string sTransitions ) 

std::map <int, WeightedNodeClass> PathTable ;
std::map <int, WeightedNodeClass>::iterator PathTableIt ;


// AdjacentNodesList( int Area, list Areas)

std::map<int, std::set<int>>            AdjacentNodesList;
std::map<int, std::set<int>>::iterator  AdjacentNodesListIt;
std::map<int,std::map<int,std::set<int>>> IncrementalAdjacentNodes;
std::map<int,std::map<int,std::set<int>>>::iterator IncrementalAdjacentNodesIt;

int AreaCount; 
int SearchIterations; 

std::set<int>                           PreviouslySearchedAreas;
std::set<int>::iterator                 PSAIt;





void
RecordAreaData(
	__in const NWN::ResRef32 & AreaResRef,
	__in ResourceManager & ResMan,
	__in IDebugTextOut * TextOut,
	__in int n
	)
/*
 n is the area key
 
Area.Tag

Area.Flags 
  0x0001 = interior (exterior if unset)
  0x0002 = underground (aboveground if unset)
  0x0004 = natural (urban if unset)

Area.Width
Area.Height

*/
{
  DemandResource32                 AreFile( ResMan, AreaResRef, NWN::ResARE );
	GffFileReader                    Are( AreFile, ResMan );
	const GffFileReader::GffStruct * RootStruct;
	std::string                      AreaTag;
	unsigned int                     Flags;
	bool                             IsUnderground;
	bool                             IsInterior;
	bool                             IsNatural;
	int                              Width;
	int                              Height;

	RootStruct = Are.GetRootStruct( );

	if (!RootStruct->GetCExoString( "Tag", AreaTag ))
		throw std::runtime_error( "Failed to read area Tag" );
	
//	TextOut->WriteDebugText("\nReturning area tag %s.\n", AreaTag.c_str( ));
		
	AreasList[n] = AreaTag;
	
	if (!RootStruct->GetDWORD( "Flags", Flags ))
		throw std::runtime_error( "Failed to read area Flags" );
	if (!RootStruct->GetINT( "Width", Width ))
		throw std::runtime_error( "Failed to read area Width" );
	if (!RootStruct->GetINT( "Height", Height ))
		throw std::runtime_error( "Failed to read area Height" );
		
	IsInterior       = Flags & 0x0001;
	IsUnderground    = Flags & 0x0002;
	IsNatural        = Flags & 0x0004;
	
	AreaSTRUCT       TheArea(
	 IsInterior,
	 IsUnderground,
	 IsNatural,
	 Height*Width
  );
  /*
 TextOut->WriteText(0,("\(RAD) %s IsInt %i IsUnd %i IsNat %i Size %i.\n", 
  AreaTag.c_str( ),
  IsInterior,
  IsUnderground,
  IsNatural,
  Height*Width
 );
 */
 
  InitList.insert(std::pair<int,AreaSTRUCT>(n,TheArea));

}


void
RecordAreasObjects(
	__in const NWN::ResRef32 & AreaResRef,
	__in ResourceManager & ResMan,
	__in IDebugTextOut * TextOut,
	__in int n
	)
/*
 n is the area key
*/
{
	DemandResource32                 GitFile( ResMan, AreaResRef, NWN::ResGIT );
	GffFileReader                    Git( GitFile, ResMan );
	const GffFileReader::GffStruct * RootStruct;
	std::string                      ObjectTag;


	RootStruct = Git.GetRootStruct( );
	GffFileReader::GffStruct DataStruct;

  // doors 
	for (size_t i = 0; i <= ULONG_MAX; i += 1)
	{
 		if (!RootStruct->GetListElement( "Door List", i, DataStruct ))
			break;
    DataStruct.GetCExoString( "Tag", ObjectTag );
    ObjectsList.insert(std::pair<std::string,int>(ObjectTag,n) );
//    TextOut->WriteText(0,("(RecordAreasObjects) Door : %s.\n", ObjectTag.c_str( ));
	}

  // Waypoints 
	for (size_t i = 0; i <= ULONG_MAX; i += 1)
	{
 		if (!RootStruct->GetListElement( "WaypointList", i, DataStruct ))
			break;
    DataStruct.GetCExoString( "Tag", ObjectTag );
    ObjectsList.insert(std::pair<std::string,int>(ObjectTag,n) );
//    TextOut->WriteText("(RecordAreasObjects) WP : %s.\n", ObjectTag.c_str( ));
	}
  // Triggers 
	for (size_t i = 0; i <= ULONG_MAX; i += 1)
	{
 		if (!RootStruct->GetListElement( "TriggerList", i, DataStruct ))
			break;
    DataStruct.GetCExoString( "Tag", ObjectTag );
    ObjectsList.insert(std::pair<std::string,int>(ObjectTag,n) );
//    TextOut->WriteText("(RecordAreasObjects) Trigger : %s.\n", ObjectTag.c_str( ));
	}
	
	// Record more if necessary

}

void
RecordTransitions(
	__in const NWN::ResRef32 & AreaResRef,
	__in ResourceManager & ResMan,
	__in IDebugTextOut * TextOut,
	__in int n
	)
/*
 n is the area key
 
areaFrom.getTransitions( as transitionFrom )

  -> null ? unlinkedAreasList.add( area )
  
  -> transitionFrom.hasValidTransition( as transitionTo ) => as areaTo
    
    -> no : invalidTransitionsList.add( area, transitionTo )

    -> yes : nodesList.add( areaFrom, areaTo, transitions.add( transitionFrom ) ) 

areaFrom.getData( areaData )

        initList.add( areaFrom, areaData )
        
*/
{
	DemandResource32                   GitFile( ResMan, AreaResRef, NWN::ResGIT );
	GffFileReader                      Git( GitFile, ResMan );
	const GffFileReader::GffStruct *   RootStruct;
  std::string                        TypeList;
  bool                               LinkedToFlags;
  std::string                        LinkedTo;
	std::string                        ObjectTag;
  std::set<std::string>              TmpTagList;
  std::vector<std::string>           TransitionsObjects;
  std::vector<std::string>::iterator it;
  int                                AreaFrom = n;
  int                                AreaTo;
  int                                Links = 0;
  int                                CountKeys = 0;
  long                               Key;
  
  TransitionsObjects.push_back("Door List");
  TransitionsObjects.push_back("TriggerList");
  
	RootStruct = Git.GetRootStruct( );
	GffFileReader::GffStruct DataStruct;

  for( it = TransitionsObjects.begin(); it < TransitionsObjects.end(); it++ )
  {
    
    TypeList = *it;
    for (size_t i = 0; i <= ULONG_MAX; i += 1)
    {
      
    	if (!RootStruct->GetListElement( TypeList.c_str(), i, DataStruct ))
    		break;
    	
      DataStruct.GetBYTEAsBool( "LinkedToFlags", LinkedToFlags );
      
      if( LinkedToFlags == 0 ) 
        continue;
      
      DataStruct.GetCExoString( "Tag", ObjectTag );
      DataStruct.GetCExoString( "LinkedTo", LinkedTo );
      ObjectsListIt   = ObjectsList.find( LinkedTo.c_str() );
     
      //
      // If AreaTo doesn't exist add to broken transitions and get next object
      //
      
      if( ObjectsListIt == ObjectsList.end() ){
        InvalidTransitionsList.push_back( ObjectTag + " has no transition target.\n" ); 
        continue;}
        
      AreaTo          = ObjectsListIt->second;

      //
      // If more than one result, report broken transition
      //
      
      CountKeys   = ObjectsList.count( LinkedTo.c_str() );
      
      if( CountKeys > 1 ){
        InvalidTransitionsList.push_back( ObjectTag + " has more than one transition targets.\n" ); }
      
      NodeClass       Node( AreaFrom, AreaTo);
      Key             = Node.GetKey();
      
      //
      // Now record node transition
      //
      
      WeightedNodesListIt     = WeightedNodesList.find(Key);
      WeightedNodeClass TheNode;
      
      if( WeightedNodesListIt == WeightedNodesList.end() ){
          TheNode.Transitions.insert( ObjectTag );
          
      }else{
        TmpTagList            = WeightedNodesListIt->second.Transitions;
        TmpTagList.insert( ObjectTag );
        TheNode.Transitions   = TmpTagList;
      }
      
      WeightedNodesList.insert( std::pair<long,WeightedNodeClass>(Key,TheNode) );
 
      // 
      // Add the Adjacent Node
      
      
      AdjacentNodesList[AreaFrom].insert(AreaTo);
      /*
      std::set<int>           TheAdjacentNodeList;
      AdjacentNodesListIt     = AdjacentNodesList.find( AreaFrom );
      TheAdjacentNodeList     = AdjacentNodesListIt->second;
      AdjacentNodesList.erase( AdjacentNodesListIt );
      TheAdjacentNodeList.insert(AreaTo);
      AdjacentNodesList.insert(
        std::pair<int,std::set<int>>
        (AreaFrom,TheAdjacentNodeList)
      );
      */
 
/*
    TextOut->WriteText("(tr.)  %i -> %i : %s -> %s \n", 
      AreaFrom, 
      AreaTo, 
      ObjectTag.c_str( ), 
      LinkedTo.c_str( )
        );
 */       
       
       Links++;
   }
    
    //
    // record unlinked areas for logging purpose 
    //
    if( !Links )
      UnlinkedAreasList.push_back(n); 
     

	}
	// Record more if necessary

}


/* 
        getNodeWeight
        input : node [A,B]
        output : int weight
 
        setNodeWeight retrieve data from A and B and compares them adding weight where
                The larger B size is 
                A and B are different regarding : 
                        - isInterior
                        - isUnderground
                        - isNatural
 
 
        The basics could be 
 
                sqrt( size = (area.height * area.width) )       => 2 to 32 
                diff.isInterior                                 => 8
                diff.isUnderground                              => 8
                diff.isNatural                                  => 8
 
        minimal weight : 2 ( from a 2x2 to another with same properties )
        maximal weight : 58 ( from an interior underground natural to a 32x32 exterior notUnderground notNatural )
 
*/

int
GetNodeWeight( 
  NodeClass TheNode,
  __in IDebugTextOut *TextOut ){

  int             Solution;
  int             AreaFromId;
  int             AreaToId;
  AreaSTRUCT      AreaFrom;
  AreaSTRUCT      AreaTo;
  double          SqrtSize;
  InitListIt      It;
  
  AreaFromId      = TheNode.GetAreaFrom();
  AreaToId        = TheNode.GetAreaTo();
  
  It      = InitList.find(AreaFromId);
  AreaFrom        = It->second;
  
  It      = InitList.find(AreaToId);
  AreaTo          = It->second;
  
  Solution        = (int) sqrt( (double) AreaTo.Size ) /2 ;
/*  
  TextOut->WriteText("(GetNodeWeight)K:%i #%i(size:%i)->#%i(size:%i) \n",  
  TheNode.Key,
  AreaFromId,
  AreaFrom.Size,
  AreaToId,
  AreaTo.Size ); 
*/ 
  if( AreaFrom.IsInterior != AreaTo.IsInterior )
    Solution += WEIGHT_INTERIOR;
  
  if( AreaFrom.IsUnderground != AreaTo.IsUnderground )
    Solution += WEIGHT_UNDERGROUND;
  
  if( AreaFrom.IsNatural != AreaTo.IsNatural )
    Solution += WEIGHT_NATURAL;

  return Solution;

}

void
SetNodesWeights(
  __in IDebugTextOut * TextOut
){


  NodeClass                         TheNode;
  WeightedNodeClass                 TheWeightedNode;

  for ( WeightedNodesListIt  = WeightedNodesList.begin() ; 
        WeightedNodesListIt != WeightedNodesList.end(); 
        WeightedNodesListIt++ ){

  
    TheNode.SetKey( WeightedNodesListIt->first );
    TheWeightedNode                 = WeightedNodesListIt->second;
    TheWeightedNode.Weight          = GetNodeWeight( TheNode, TextOut ) ;
    WeightedNodesList.erase(WeightedNodesListIt);
    WeightedNodesList.insert(
      std::pair<long,WeightedNodeClass>(WeightedNodesListIt->first, TheWeightedNode ) 
    );

 /*   
    TextOut->WriteText("(SetNodesWeights) #%i/%i : %s -> %i \n",  
        WeightedNodesListIt->first,
        TheNode.Key,
        WeightedNodesListIt->second.Transitions.c_str( ),
        TheWeightedNode.Weight ); 
 */
 
  }
    
}









/*!! Returns the areas connected to the Area 

*/

std::set<int>
GetAdjacentNodes(
  int AreaId,
  int nLevel,
  __in IDebugTextOut *  TextOut 
  
)
{

  std::set<int>             Solution;
  Solution                  = IncrementalAdjacentNodes[AreaId][nLevel];


// TextOut->WriteText("(GetAdjacentNodes) AreaId : %i, nLevel:%i\n", AreaId, nLevel );

  // 
  // The solution cache is returned if exists
  //
  if( Solution.size() > 0 ){
// TextOut->WriteText("(GetAdjacentNodes) Solution cached AreaId : %i, nLevel:%i\n", AreaId, nLevel);
 
    return                  Solution;
  }

// TextOut->WriteText("(GetAdjacentNodes) Solution not cached AreaId : %i, nLevel:%i\n", AreaId, nLevel);
  //  
  // The solution is the area
  // 
  if( nLevel == 1 ){

    IncrementalAdjacentNodes[AreaId][1].insert(AreaId);
    Solution.insert(AreaId);
    
    return                  Solution;
  
  }
  
// TextOut->WriteText("(GetAdjacentNodes) Search through Subnodes\n", AreaId, nLevel );

  //
  // The solution is equal to expansion of sublevel areas 
  // which is cached and returned
  //
  std::set<int>             SubSolution,TmpSet;
  std::set<int>::iterator   it;
  std::set<int>::iterator   it2;
  SubSolution               = IncrementalAdjacentNodes[AreaId][nLevel-1];
// TextOut->WriteText("(GetAdjacentNodes) SubSolution size : %i, area #1:%i\n", SubSolution.size(), *SubSolution.begin() );

  for( it = SubSolution.begin(); it != SubSolution.end(); it++ ){

    AdjacentNodesListIt      = AdjacentNodesList.find( *it );
    
    if( AdjacentNodesListIt == AdjacentNodesList.end() ){
// TextOut->WriteText("(GetAdjacentNodes) AdjacentNodesListIt at the end of array\n");
      return Solution;
    
    }
    
    
/*   
for ( AdjacentNodesListIt=AdjacentNodesList.begin() ; AdjacentNodesListIt != AdjacentNodesList.end(); AdjacentNodesListIt++ )
{
  AdjacentNodesListIt      = AdjacentNodesList.find( 5 );
  std::set<int> TheList = AdjacentNodesListIt->second;
  std::set<int>::iterator it;
  int AreaFrom = AdjacentNodesListIt->first;
  
  for( it=TheList.begin(); it!=TheList.end();it++ )
  {
    TextOut->WriteText("([AdjacentNodesList] %i : %i \n", 
    AreaFrom,
    *it
    ); 
  }
}
*/
    TmpSet                  = AdjacentNodesListIt->second;
    int a                   = AdjacentNodesListIt->first;
//    std::set<int> TheList = AdjacentNodesListIt->second;

/*    
    if( AdjacentNodesListIt->second.empty() )
      TextOut->WriteText("(GetAdjacentNodes) TmpSet empty\n" );

    else TextOut->WriteText("(GetAdjacentNodes) TmpSet not empty\n" );
*/
// TextOut->WriteText("(GetAdjacentNodes) TmpSet size:%i\n", AdjacentNodesListIt->second.size() );

    for( it2 = TmpSet.begin() ; it2 != TmpSet.end() ; it2++ ){
// TextOut->WriteText("(GetAdjacentNodes) TmpSet area:%i\n", *it2 );
    
      Solution.insert( *it2 );
      
    }
    /* */ 
  }
  
  IncrementalAdjacentNodes[AreaId][nLevel] = Solution;
//  TextOut->WriteText("(GetAdjacentNodes) Solution size:%i\n", Solution.size() );
 
  return Solution;
  
}


/*!! GetPath performs the search for nodes connecting ThePath.Node areas 

  Returns : 
    [-1]    There is no valid solution
    [x]     There is a single link
    [x,...] There are more than one link
    
  
    
    
*/
std::set<int>
GetPath( 
  NodeClass             TheNode,
  __in IDebugTextOut *  TextOut 
  
)
{
/* 
  //TextOut->WriteText("\n(GetPath): Init Run for TheNode #%i From %i to %i with prev.searc.area:", TheNode.Key,TheNode.AreaFrom,TheNode.AreaTo );

  for( PSAIt=PreviouslySearchedAreas.begin();PSAIt!=PreviouslySearchedAreas.end();PSAIt++  )
  {//TextOut->WriteText("%i:", *PSAIt );}
  //TextOut->WriteText("\n");
*/

  // Temporary : contains areas currently added to the research process
  std::set<int>               Current; 
  std::set<int>::iterator     CurrentIt; 
  
  // Temporary : contains areas previously searched, 
  // equal to FromSearchedAreas or ToSearchedAreas during search
  std::set<int>               *CompareAltPt;
  std::set<int>               CompareAlt;
  std::set<int>               *CompareOwnPt;
  std::set<int>               CompareOwn;
  
  // Stores areas previously searched
  std::set<int>               FromSearchedAreas;
  std::set<int>               ToSearchedAreas;
  std::set<int>*              CompleteList[]  = {&FromSearchedAreas,&ToSearchedAreas};
  
  // The set of areas finally returned
  std::set<int>               Solution;
  std::set<int>::iterator     SolutionIt;
  
  // The set of areas added to comparison cache 
  std::set<int>               Union;

  int i                       = 1;
  int j                       = 1;    // floor(i/2)
  int k                       = 0;    // i%2
  int Search[]                = {TheNode.AreaFrom,TheNode.AreaTo};
     std::set<int>::iterator TmpIt;
  

  int Needle                  = Search[1];
  int ExpandArea              = Search[1];
  int PreviousArea;
  
  

  while( i < AreaCount ){
  
    Solution.clear();
    Union.clear();
    
    PreviousArea = ExpandArea;
    
    // Get Next area to search from
    i++;
    j                               = (int) floor( (float) i / 2 ); // 1, 1, 2, 2, etc.
    k                               = i%2; // 01010101...
    
    ExpandArea                      = Search[ k ];


// //TextOut->WriteText("(GetPath)i:%i|j:%i|k:%i / Expanding %i\n", i, j,k,ExpandArea );
  
      
    // Get the last areas expanded for the searched area
    Current                         = GetAdjacentNodes( ExpandArea, j, TextOut );
  
   /* ++ del      
//TextOut->WriteText("(GetPath): Current size:%i areas ", Current.size() );
    for ( TmpIt=Current.begin(); TmpIt != Current.end(); TmpIt++ )
//TextOut->WriteText(" : %i ", *TmpIt );
//TextOut->WriteText("\n");

  
   // -- del
   */
   
    // There is no solution
    if( Current.size() < 1 ){ 
//TextOut->WriteText("(GetPath): No solution found.\n");
      Solution.insert(-1);
      return Solution;
      }
    
    // Special case : the landing area is in the result
    // yet we need to determine which areas from previous adjacent iteration
    // is connected to it
    //TextOut->WriteText("(GetPath): Searching for presence of alternate Area  %i\n", PreviousArea );
    CurrentIt                   = Current.find( PreviousArea );
    if( CurrentIt != Current.end() &&  *CurrentIt != ExpandArea ){
      std::set<int>             PreviousSearchResult;
      std::set<int>::iterator   PSRIt;
      std::set<int>             SubResults;
      std::set<int>::iterator   SRIt;
      PreviousSearchResult      = GetAdjacentNodes( ExpandArea, j-1, TextOut );
//TextOut->WriteText("(GetPath): Area within results, PreviousSearchResult size %i\n", PreviousSearchResult.size() );
      for( PSRIt = PreviousSearchResult.begin(); PSRIt!= PreviousSearchResult.end(); PSRIt++ ){
          
          SubResults            = GetAdjacentNodes( *PSRIt, 1, TextOut );
          SubResults            = GetAdjacentNodes( *PSRIt, 2, TextOut );
//TextOut->WriteText("(GetPath): Area within results, Backtracking %i, result size %i\n", *PSRIt, SubResults.size() );
          SRIt                  = SubResults.find( PreviousArea );
          if( SRIt != SubResults.end() &&  *PSRIt != ExpandArea){
//TextOut->WriteText("(GetPath): Area within results, Found source %i\n", *PSRIt);

            // Verify the solution is not in Previously Searched Areas
            PSAIt               = PreviouslySearchedAreas.find( *PSRIt );
            if( PSAIt == PreviouslySearchedAreas.end() ){
              Solution.insert( *PSRIt );
            }
          }
      }
      if( Solution.size() > 0 ) {
      //TextOut->WriteText("(GetPath): Solution found : Area within results, solution size %i\n", Solution.size() );
        return Solution;
      }

    } // End of Special case : the landing area is in the result
    
    //
    // Get the comparison set build of already searched areas
    // that are connected to the alternate area
    // ie. ToSearchedAreas if the Current relates to FromSearchedAreas
    // if i is even, we're on at FromSearchedAreas and want to 
    // retrieve ToSearchedAreas, ie (i+1)%2
    //
    int KeyAlt                          = (i+1)%2;
    CompareAltPt                        = CompleteList[ KeyAlt ];
    CompareAlt                          = *CompareAltPt;
    
    //
    // Same than above but now we want the set we might get added to
    // ie (i+1)%2
    //
    int KeyOwn                          = i%2;
    CompareOwnPt                        = CompleteList[ KeyOwn ];
    CompareOwn                          = *CompareOwnPt;
    
    //
    // set_intersection didn't work 
    //
   std::set<int> TmpSetBig, TmpSetSmall;
    if( Current.size() > CompareAlt.size() ) 
    { TmpSetBig=Current; TmpSetSmall=CompareAlt;} 
    else 
    { TmpSetBig=CompareAlt; TmpSetSmall=Current;} 
    for ( TmpIt=TmpSetBig.begin(); TmpIt != TmpSetBig.end(); TmpIt++ ) {
      if( TmpSetSmall.end() != TmpSetSmall.find( *TmpIt ) ){
        Solution.insert( *TmpIt );
      }
    }      
   
    
   // If intersection, return the result
    if( Solution.size() > 0 ){
    
      // Check if no result was already found
      for(SolutionIt = Solution.begin(); SolutionIt != Solution.end(); SolutionIt++)
      {
          PSAIt               = PreviouslySearchedAreas.find( *SolutionIt );
          if( PSAIt != PreviouslySearchedAreas.end() ){
//TextOut->WriteText("(GetPath): Oops, a previously searched area was found  %i Areas\n", *PSAIt );
            Solution.erase( SolutionIt );
          }
      }
      
      
//TextOut->WriteText("(GetPath): Solution found for (%i->%i): %i Areas. ID", TheNode.AreaFrom,TheNode.AreaTo, Solution.size() );
      for ( TmpIt=Solution.begin(); TmpIt != Solution.end(); TmpIt++ )
//TextOut->WriteText(": %i ", *TmpIt );
//TextOut->WriteText("\n");
         
      if( Solution.size() > 0 ) {
        return Solution;
      }
      
    } 
    
    // Else store the data and search again
    if( CompareOwn.size() > 0 ) {
        /*
      set_union(  Current.begin(), Current.end(),
                  CompareOwn.begin(), CompareOwn.end(),
                  Union.begin());
         */        
//TextOut->WriteText("(GetPath): Union avoids inserting %i and %i\n ", TheNode.AreaFrom, TheNode.AreaTo);
        for ( TmpIt=Current.begin(); TmpIt != Current.end(); TmpIt++ )  {    
            if( *TmpIt != TheNode.AreaFrom && *TmpIt != TheNode.AreaTo ){
//TextOut->WriteText("(GetPath): Union insert[%i]\n ", *TmpIt );
              Union.insert( *TmpIt );     
            }  
        }  
        for ( TmpIt=CompareOwn.begin(); TmpIt != CompareOwn.end(); TmpIt++ )  {    
            if( *TmpIt != TheNode.AreaFrom && *TmpIt != TheNode.AreaTo ){
//TextOut->WriteText("(GetPath): Union insert[%i]\n ", *TmpIt );
              Union.insert( *TmpIt );  
              }   
        }  
/* 
//TextOut->WriteText("(GetPath): CompareOwn[%i] exists with size:%i areas ", KeyOwn, CompareOwn.size() );
        for ( TmpIt=CompareOwn.begin(); TmpIt != CompareOwn.end(); TmpIt++ )        
//TextOut->WriteText(" : %i ", *TmpIt );          
//TextOut->WriteText("\n");
*/               
    } else {
    
//TextOut->WriteText("(GetPath): CompareOwn[%i] empty, storing current only \n", KeyOwn, CompareOwn.size() );   
       Union = Current;
            
    }  
    
    
//TextOut->WriteText("(GetPath): New CompareOwn[%i] size:%i areas ", KeyOwn, Union.size() );
for ( TmpIt=Union.begin(); TmpIt != Union.end(); TmpIt++ )
//TextOut->WriteText(" : %i ", *TmpIt );
//TextOut->WriteText("\n");
/* */ 
      *CompareOwnPt                      = Union;
//TextOut->WriteText("(GetPath): Keep searching.\n" );
           
    
    }
    
  //TextOut->WriteText("(GetPath): Ending Search, returning.\n" );

  return Solution;
    
  
  
}

 


/*!! SearchPath performs a complete search for all valid paths

  @param TheNode : a couple {AreaFrom, AreaTo}, direct node or not
  
 
*/
PathClass
SearchPath( 
  NodeClass TheNode,
  __in IDebugTextOut *  TextOut )
{


   //TextOut->WriteText("\n(SearchPath):(%i->%i)#%i \n", TheNode.AreaFrom,TheNode.AreaTo, TheNode.Key );

  PathClass               ThePath;
  long                    Key;
  std::set<int>           Solution;
  NodeClass               InnerNode;
 
  Key                     = TheNode.Key;
  ThePath.mNode           = TheNode;
 
  WeightedNodesListIt     = WeightedNodesList.find( Key );
  
 if( SearchIterations > ( AreaCount / 2  ) ){
      
      ThePath.Type = INVALID;
      return ThePath;
    
  } 
  //
  // path is a direct node, search is over
  // 
  if( WeightedNodesListIt != WeightedNodesList.end() ) {
  
   //TextOut->WriteText("(SearchPath):  Solution (%i->%i)#%i = simple node\n", TheNode.AreaFrom,TheNode.AreaTo,TheNode.Key );
   return ThePath;
  
  }
  
//TextOut->WriteText("(SearchPath): Node #%i not simple node\n", TheNode.Key );
  
  //
  // Search the solution(s)
  // 
  Solution                = GetPath( TheNode, TextOut );
 
  if( Solution.size() == 1 ) {
    
    int                   Result = *Solution.begin();
    //
    // There is no valid result
    //
    if( Result == -1 ){
      ThePath.Type = INVALID;
      return ThePath;
    }
  }
  
  // 
  // The Path is complex : starts as a linear pair, ends with fork of paths
  //
  //TextOut->WriteText("(SearchPath) Solution (%i->%i)#%i is complex \n", ThePath.mNode.AreaFrom,ThePath.mNode.AreaTo,ThePath.mNode.Key  );
  ThePath.SetComplexPath( Solution, TextOut );

  return ThePath;
  
}



// 
// returns the nodes contained by a path and its subsequent paths
// 
/*

*/
std::vector< std::vector< NodeClass > > 
PathClass::ToNodes( 
  __in IDebugTextOut * TextOut
)
{
  std::vector<std::vector<NodeClass>>       Solution; 
  std::vector<std::vector<NodeClass>>::iterator  SolutionIt;
 
  std::vector<std::vector<NodeClass>>       TempSolution; 
  std::vector<std::vector<NodeClass>>::iterator  TempSolutionIt;
 
  std::vector<std::vector<NodeClass>>       ForkSolution;
  std::vector<std::vector<NodeClass>>::iterator  ForkSolutionIt;
  
  std::vector< NodeClass >                  TmpNodSolution;
  std::vector< NodeClass >                  SrcNodSolution;
  std::vector< NodeClass >                  TgtNodSolution;
  std::vector< NodeClass >::iterator        NodSolutionIt;
        
  // Iterator used to parse this.mSubPath
  std::vector <PathClass *>::iterator       VecPatIt;			
  std::vector< NodeClass >                  SimpleNodeVector;
  
  NodeClass                                 TheNode;
  PathClass  *                              ThePathPtr = new PathClass();
  PathClass                                 ThePath;
  
  switch( Type ){
  
    case NODE :
    
      //////TextOut->WriteText( "::ToNodes ; NODE #%i\n", mNode.Key );
      SimpleNodeVector.push_back( mNode );
      Solution.push_back( SimpleNodeVector );

    break;

    case PATH :
    
      //////TextOut->WriteText( "::ToNodes ; PATH, depth:%i subnodes:%i NODE #%i(%i->%i).\n", mDepth, mSubNodes.size(), mNode.Key, mNode.AreaFrom, mNode.AreaTo );
      
      // Parse each of 2 sub paths
      for( VecPatIt = mSubNodes.begin(); VecPatIt != mSubNodes.end(); VecPatIt++ ){
      
        ThePathPtr          = *VecPatIt;
        ThePath             = *ThePathPtr;
        //////TextOut->WriteText( "::ToNodes PATH digging, depth:%i subpaths:%i NODE #%i(%i->%i).\n", ThePath.mDepth, ThePath.mSubPath.size(), ThePath.mNode.Key, ThePath.mNode.AreaFrom, ThePath.mNode.AreaTo  );
        
        // Convert the sub path to nodes lists
        ForkSolution        = ThePath.ToNodes( TextOut );
        
        // Store 1st solution if null by now
        if( TempSolution.size() ==  0 ){
          TempSolution      = ForkSolution;
        }
        
        // perform a matrix multiplication combining first solution and second
        // a,b x c,d => a,c - a,d - b,c - b,d
        else {
          
          // for each existing 1st path solution 
          for( TempSolutionIt = TempSolution.begin();TempSolutionIt!=TempSolution.end();TempSolutionIt++){
          
            SrcNodSolution  = * TempSolutionIt;
          
            // for each existing 2nd path solution
            for( ForkSolutionIt = ForkSolution.begin();ForkSolutionIt!=ForkSolution.end();ForkSolutionIt++){
        
              TmpNodSolution.clear();
              TgtNodSolution  = * ForkSolutionIt;
              // meh. Insert( iterator, start, end ) doesn't work :/
              for(NodSolutionIt=SrcNodSolution.begin();NodSolutionIt!=SrcNodSolution.end();NodSolutionIt++){
                TmpNodSolution.push_back( *NodSolutionIt );
              }
              // meh. Insert( iterator, start, end ) doesn't work :/
              for(NodSolutionIt=TgtNodSolution.begin();NodSolutionIt!=TgtNodSolution.end();NodSolutionIt++){
                TmpNodSolution.push_back( *NodSolutionIt );
              }
              
              Solution.push_back( TmpNodSolution );
              
            }
        
          }
        }
      }

    break;

    case COMPLEX :
    
      //////TextOut->WriteText( "::ToNodes Complex, depth:%i subpaths:%i NODE #%i(%i->%i).\n", mDepth, mSubPath.size(), mNode.Key, mNode.AreaFrom, mNode.AreaTo  );

      // Get Solutions for each sub path
      for( VecPatIt = mSubPath.begin(); VecPatIt != mSubPath.end(); VecPatIt++ ){
      
        ThePathPtr          = *VecPatIt;
        ThePath             = *ThePathPtr;
        //////TextOut->WriteText( "::ToNodes Complex digging, depth:%i subpaths:%i NODE #%i(%i->%i).\n", ThePath.mDepth, ThePath.mSubPath.size(), ThePath.mNode.Key, ThePath.mNode.AreaFrom, ThePath.mNode.AreaTo  );
       
        ForkSolution        = ThePath.ToNodes( TextOut );
        // Each sub path solution is pushed back to the final solution
        // (a,(b,c),c) -> (a,b,c,d)
        for( ForkSolutionIt = ForkSolution.begin();ForkSolutionIt!=ForkSolution.end();ForkSolutionIt++){
          Solution.push_back( *ForkSolutionIt );
        }
      }
       
    break;

 
    default :
        
    //////TextOut->WriteText( "::ToNodes ; INVALID depth:%i subpaths:%i NODE #%i(%i->%i).\n", mDepth, mSubPath.size(), mNode.Key, mNode.AreaFrom, mNode.AreaTo  );
        
    break;
  }
  
std::string str;

for( SolutionIt = Solution.begin();SolutionIt != Solution.end();SolutionIt++)
  {
    str.append("*");
    //////TextOut->WriteText( "\n%s\n::ToNodes Solution for (%i->%i)#%i : Nodes ", str.c_str( ),mNode.AreaFrom, mNode.AreaTo, mNode.Key);
        
    for( std::vector< NodeClass>::iterator SomeIt = (*SolutionIt).begin();
    SomeIt < (*SolutionIt).end();
    SomeIt ++ )
    {
      //////TextOut->WriteText( " %i(%i->%i): ", SomeIt->Key, SomeIt->AreaFrom, SomeIt->AreaTo );
    }
    //////TextOut->WriteText( "\n");
    
  }
  //////TextOut->WriteText( "::ToNodes returning Solution Size %i NODE #%i(%i->%i).\n%s\n", Solution.size(),mNode.Key, mNode.AreaFrom, mNode.AreaTo,str.c_str()  );

  return Solution;

}





void PathClass::SetSimplePath( 
  int LinkAreaId,
  __in IDebugTextOut * TextOut
)
{

  Type = PATH;
  int newDepth            = ( mDepth + 1 ); 

  NodeClass               InnerNode; 
  
  // Defining the node elements for the new path  
  InnerNode.SetAreas( mNode.AreaFrom, LinkAreaId );
  
  // Instanciate and assign node
  PathClass *             FromPath = new PathClass(newDepth);
  FromPath->mPreviouslySearchedAreas = mPreviouslySearchedAreas;
  PreviouslySearchedAreas = mPreviouslySearchedAreas;
  
   // Perform search for path
  * FromPath              = SearchPath( InnerNode, TextOut );
   mSubNodes.push_back( FromPath );
 
  // Defining the node elements for the new path  
  InnerNode.SetAreas( LinkAreaId, mNode.AreaTo );
  
  // Instanciate and assign node
  PathClass *             ToPath = new PathClass(newDepth);
  ToPath->mPreviouslySearchedAreas = mPreviouslySearchedAreas;
  
  * ToPath                = SearchPath( InnerNode, TextOut );
  mSubNodes.push_back( ToPath );


}


/** 
 * 
 * @input : int set, each representing a solution for the current path
 */ 
void PathClass::SetComplexPath( 
  std::set<int> Solution,
  __in IDebugTextOut * TextOut 
){


  SearchIterations++;
  
  int                     LinkAreaId;
  int                     i;
  int newDepth            = mDepth + 1;
  std::set<int>::iterator it;
  Type = COMPLEX;      
  
  // Add my nodes to my PSA 
  mPreviouslySearchedAreas.insert( mNode.AreaFrom );
  mPreviouslySearchedAreas.insert( mNode.AreaTo );
  for(it = Solution.begin(); it != Solution.end(); it++)   
  {
      mPreviouslySearchedAreas.insert(*it);
  }
  
  
  // ////TextOut->WriteText( "\n::SetComplexPath : Areas %i->%i Start  \n", mNode.AreaFrom, mNode.AreaTo  );
  
  for(it = Solution.begin(); it != Solution.end(); it++)   
  {
    
    LinkAreaId           = *it;
    PathClass *           ForkPath = new PathClass(newDepth);
    ForkPath->Type        = COMPLEX;
    ForkPath->mNode       = this->mNode;
    
    // Transfer my PSA to my child 
    ForkPath->mPreviouslySearchedAreas = mPreviouslySearchedAreas;
    
    //TextOut->WriteText( "::SetComplexPath : Areas %i->%i->%i Depth:%i  \n", mNode.AreaFrom, LinkAreaId,  mNode.AreaTo, ForkPath->mDepth );
    ForkPath->SetSimplePath( LinkAreaId, TextOut );
     mSubPath.push_back( ForkPath );
    //TextOut->WriteText( "\n::SetComplexPath : Areas %i->%i->%i %i Subpaths %i SubNodes  \n", mNode.AreaFrom, LinkAreaId,  mNode.AreaTo, ForkPath->mSubPath.size(), ForkPath->mSubNodes.size()  );
  }
    
  //TextOut->WriteText( "::SetComplexPath : Areas %i->%i End :%i Subpaths  %i SubNodes \n", mNode.AreaFrom, mNode.AreaTo, mSubPath.size(), mSubNodes.size() );
  for( std::vector< PathClass *>::iterator mSPIt = mSubPath.begin(); mSPIt != mSubPath.end(); mSPIt++ )
  {
    PathClass * Path = *mSPIt;
    //TextOut->WriteText( "\n::SetComplexPath Added Path with Node #%i(%i->%i) %i Subpaths %i SubNodes\n", Path->mNode.Key,Path->mNode.AreaFrom,Path->mNode.AreaTo, Path->mSubPath.size(),Path->mSubNodes.size() );
  }
  /* */
  
  
}





/*!! 
        Performs a simplification of complex arrays including nodes, paths and forks
 
        Ex : let's say the [Q-Z] path is solved like that :
 
              - C -   - E -
        Q - A - J - D       G - Z
              - B -   - F -
 
        Q-Z : Fork
          Q-D : Fork
            Q-A : Node    
            A-D : Fork
              A-C, C-D : Simple Path
                A-C : Node
                C-D : Node
              A-J, J-D : Simple Path
                A-J : Node
                J-D : Node
              A-B, B-D : Simple Path
                A-B : Node
                B-D : Node
          D-Z : Fork
            D-E, E-G : Simple Path
              D-E : Node
              E-G : Node
            D-F, F-G : Simple Path
              D-F : Node
              F-G : Node
            G-Z : Node
            
        In array form :
        [[Q,A],[[[A,B],[B,D]],[[A,C],[C,D]],[[A,J],[J,D]]],[[[D,E],[E,G]],[[D,F],[F,G]]],[G,Z]]
 
        pathSolver will return the list :
        [
        [[Q,A],[A,B],[B,D],[D,E],[E,G],[G,Z]],
        [[Q,A],[A,C],[C,D],[D,E],[E,G],[G,Z]],
        [[Q,A],[A,J],[J,D],[D,E],[E,G],[G,Z]],
        [[Q,A],[A,B],[B,D],[D,F],[F,G],[G,Z]],
        [[Q,A],[A,C],[C,D],[D,F],[F,G],[G,Z]],
        [[Q,A],[A,J],[J,D],[D,F],[F,G],[G,Z]]
        ]

*/

std::vector<std::vector<NodeClass>>
GetFullPath( 
  PathClass ThePath,
  __in IDebugTextOut *  TextOut
)
{

  std::vector<std::vector<NodeClass>>  Solution;
  
  Solution = ThePath.ToNodes( TextOut );
  
   ////TextOut->WriteText( "\n(GetFullPath) Solution size:%i\n\n", Solution.size() );
 

  for(std::vector<std::vector<NodeClass>>::iterator SolutionIt = Solution.begin();SolutionIt != Solution.end();SolutionIt++)
  {
  
    ////TextOut->WriteText( "(GetFullPath)  : Nodes");
        
    for( std::vector< NodeClass>::iterator SomeIt = (*SolutionIt).begin();
    SomeIt < (*SolutionIt).end();
    SomeIt ++ )
    {
      ////TextOut->WriteText( "(%i->%i):", SomeIt->AreaFrom, SomeIt->AreaTo );
    }
    ////TextOut->WriteText( "\n");
    
  }

  return Solution;
  
}


/*!! getBestPath performs the selection of the best paths.
    Will weight every path in the Path parameter and compare it
    in order to return the transitions representing the lesser weight.
    If two or more paths have an equal weight the transitions are cumulated
    
        float W
        string T
        list solution
 
        P.forEach( as subPath )
 
                W = getPathWeight( subPath )
 
                if( W <= solution[weight] )
 
                        solution[weight]        = W;
                        solution[transition].addIfNotExists( getTransition( subPath.firstNode ) )
 
        return solution
 
*/
WeightedNodeClass
GetBestPath( 
  PathClass             ThePath,
  __in IDebugTextOut *  TextOut
)
{

  WeightedNodeClass                               Solution;
  std::vector<std::vector<NodeClass>>             AllPath;
  std::vector<std::vector<NodeClass>>::iterator   APIt;
  std::vector<NodeClass>                          SubPath;
  std::vector<NodeClass>                          BestPath;
  std::vector<NodeClass>::iterator                SubPathIt;
  int                                             Weight;
  NodeClass                                       TheNode;
  WeightedNodeClass                               TheWeightedNode;
  int                                             BestWeight        = 0;
  std::set<std::string>                           Transitions;
  std::set<std::string>::iterator                 TransitionsIt;
  
  //
  // Reduce the provided Path to a simple collection of individual path
  //
  
  AllPath                   = GetFullPath( ThePath, TextOut ); 
  
  // 
  // weight and compare each path
  for( APIt = AllPath.begin(); APIt != AllPath.end(); APIt++ ){
  
  ////TextOut->WriteText("\n\n(GetBestPath) new Inner Path : \n");
  
    SubPath                 = * APIt;
    
    // Get Total Area Weight
    Weight                  = SubPath.size() * WEIGHT_TRANSITION;
    ////TextOut->WriteText("(GetBestPath) Initial Weight: %i(%i->%i)\n", Weight );
    
    // Add each node weight
    for( SubPathIt = SubPath.begin(); SubPathIt!=SubPath.end(); SubPathIt++ ){
      
      TheNode               = *SubPathIt;
      ////TextOut->WriteText("(GetBestPath) TheNode #%i(%i->%i)\n", TheNode.Key, TheNode.AreaFrom, TheNode.AreaTo);
      WeightedNodesListIt   = WeightedNodesList.find(TheNode.Key);
      TheWeightedNode       = WeightedNodesListIt->second;
      ////TextOut->WriteText("(GetBestPath) TheWeightedNode Weight:%i Transitions:%i\n", TheWeightedNode.Weight, TheWeightedNode.Transitions.size() );
      Weight                += TheWeightedNode.Weight;
    }
   ////TextOut->WriteText("(GetBestPath) Inner Path Weight:%i \n", Weight);
   
    // 
    // If the path weight is better than the actual solution, replace it
    // or if the best weight is undetermined, set it
    if( Weight < BestWeight || BestWeight == 0 ){
      ////TextOut->WriteText("(GetBestPath) New Best Weight:%i \n", Weight);
      BestWeight            = Weight;
      SubPathIt             = SubPath.begin();
      TheNode               = *SubPathIt;
      WeightedNodesListIt   = WeightedNodesList.find(TheNode.Key);
      TheWeightedNode       = WeightedNodesListIt->second;
      Transitions           = TheWeightedNode.Transitions;
    }
    //
    // If the path weight is equal to the actual solution, cumulate transitions
    else if( Weight == BestWeight )
    {
      SubPathIt             = SubPath.begin();
      TheNode               = *SubPathIt;
      WeightedNodesListIt   = WeightedNodesList.find(TheNode.Key);
      TheWeightedNode       = WeightedNodesListIt->second;
      for( TransitionsIt=TheWeightedNode.Transitions.begin();TransitionsIt!=TheWeightedNode.Transitions.end();TransitionsIt++){
        Transitions.insert( *TransitionsIt );
      }
    }
    ////TextOut->WriteText("(GetBestPath) TheNode:#%i(%i->%i) \n", TheNode.Key, TheNode.AreaFrom, TheNode.AreaTo);

  }
  Solution.Weight           = BestWeight;
  Solution.Transitions      = Transitions;
//  ////TextOut->WriteText("(GBP): Area #%i\n", ThePath.Node.Key );
  return Solution;

}



void 
SetPathTable(
  __in IDebugTextOut * TextOut
){

  InitListIt                   I1;
  InitListIt                   I2;
  int                          AreaFromId;
  int                          AreaToId;
  NodeClass                    TheNode;
  PathClass                    ThePath;
  long                         Key;

  for( I1 = InitList.begin(); 
       I1 != InitList.end(); 
       I1 ++){
         AreaFromId            = I1->first;
         TextOut->WriteText("Area #%i/%i: %s\n[", (I1->first +1), AreaCount, AreasList[I1->first].c_str() );
       
    for( I2 = InitList.begin(); 
         I2 != InitList.end(); 
         I2 ++){
         TextOut->WriteText(".");
         WeightedNodeClass     TheWeightedNode;
         AreaToId              = I2->first;
         
         if( AreaFromId == AreaToId ) continue;
                    
         TheNode.SetAreas( AreaFromId, AreaToId );
         Key                   = TheNode.GetKey();
         WeightedNodesListIt   = WeightedNodesList.find( Key );
         
         //TextOut->WriteText( "(SetPathTable) %i => #%i -> #%i \n", Key, AreaFromId, AreaToId); 

         if( WeightedNodesListIt != WeightedNodesList.end() ) {
         
           //TextOut->WriteText(  "(SetPathTable) Node #%i : W:%i, T:%i \n",  WeightedNodesListIt->first,WeightedNodesListIt->second.Weight,WeightedNodesListIt->second.Transitions.size( ) ); 

           TheWeightedNode     = WeightedNodesListIt->second;
           
         } else {
          
            //TextOut->WriteText( "\n\n######################################################################\n"); 
            //TextOut->WriteText( "\n(SetPathTable) Initiating New Complete Search \n", Key, AreaFromId, AreaToId); 
            
            
            //
            // This is where the path calculation happens. Spaghetti time !
            //
            
            PreviouslySearchedAreas.clear();
            SearchIterations     = 0;
            ThePath              = SearchPath( TheNode, TextOut );
            
            

            // 
            // Safety Drop : invalid path don't need to be resolved
            //
            if( INVALID != ThePath.Type ){
    
              TheWeightedNode      = GetBestPath( ThePath, TextOut );
              
            } 
            
            //TextOut->WriteText(  "(SetPathTable) Path Searched \n \n");         
            //TextOut->WriteText("\n######################################################################\n\n"); 

         }
         
         PathTable.insert(
          std::pair<int,WeightedNodeClass>(Key, TheWeightedNode ) 
         );
         
          //TextOut->WriteText("(SetPathTable) Not connected Areas\n");
         

        //TextOut->WriteText("(SetPathTable)");
         
    }
    
    TextOut->WriteText("]\n");
  }

}


/**
 * Exports to files
 **/ 
void 
ExportPathTable(
  __in std::string ModName,
  __in IDebugTextOut * TextOut
){


    FILE * The2DAFile         ;
		char *                    fmt;
		char                      filename[200];
		char                      tablename[200];
		char                      area_from[200];
		char                      area_to[200];
  	char                      buf[8193];
  	char                      tmpbuf[200];
  	WeightedNodeClass         TheWeightedNode;
  	NodeClass                 TheNode;
  	std::string               TheTransitions;
  	std::set<std::string>     TransitionsIt;
  	std::map <int,std::string>::iterator  It1,It2;
  	std::string               Glue                 = ",";
  
  
    // Normalize Module name : no spaces, less than 20 characters
    std::string::size_type pos = 0;
    bool spacesLeft = true;
    while( spacesLeft ){
      pos = ModName.find(" ");
      if( pos != std::string::npos )
      ModName.erase( pos, 1 );
      else
      spacesLeft = false;
    }
    std::string TempName      = ModName.substr (0,20);
    ModName                   = TempName;  
  
    //
    // Building the Vertical Area List 2DA 
    // ie row => id
    //
    sprintf( filename, "ftp_%s_list_v.2DA", ModName.c_str() );
    The2DAFile         = fopen(filename,"w");
  	// Lines 1&2 : default
  	std::sprintf(buf, "2DA V2.0\n\n" );
    fprintf(The2DAFile, buf);
    // Line 3 : columns
  	std::sprintf(buf, "    TAG\n" );
    fprintf(The2DAFile, buf);
 	  // Line 4+ : data
  	for( It1=AreasList.begin();It1!=AreasList.end();It1++){
  	  std::sprintf(buf, "%i    \"%s\"\n", It1->first,It1->second.c_str() );
 	    fprintf(The2DAFile, buf);
 	  }
 	  fclose (The2DAFile);
 	    
    //
    // Building the SQL Area List  
    // row => id
    //
    sprintf( filename, "ftp_%s_list.sql", ModName.c_str() );
    The2DAFile         = fopen(filename,"w");
    sprintf( tablename, "ftp_%s_areaslist", ModName.c_str() );
  	std::sprintf(buf,"DROP TABLE IF EXISTS `%s`;\n",tablename);fprintf(The2DAFile, buf);
  	std::sprintf(buf,"CREATE TABLE `%s`(\n",tablename);fprintf(The2DAFile, buf);
  	std::sprintf(buf,"`id` int unsigned NOT NULL ,\n");fprintf(The2DAFile, buf);
 	  std::sprintf(buf,"`tag` varchar(64) NOT NULL DEFAULT '',\n");fprintf(The2DAFile, buf);
 	  std::sprintf(buf,"PRIMARY KEY (`id`));\n");fprintf(The2DAFile, buf);
  	for( It1=AreasList.begin();It1!=AreasList.end();It1++){
  	  std::sprintf(buf, "INSERT INTO `%s`(`id`,`tag`) VALUES(%i,'%s');\n", tablename,It1->first,It1->second.c_str() );
 	    fprintf(The2DAFile, buf);
 	  }
 	  fclose (The2DAFile);
 	  
    //
    // Building the Horizontal Area List 2DA 
    // ie row => 0, tag, tag, tag etc.
    //
    sprintf( filename, "ftp_%s_list_h.2DA", ModName.c_str() );
    The2DAFile         = fopen(filename,"w");
  	// Lines 1&2 : default
  	std::sprintf(buf, "2DA V2.0\n\n" );
    fprintf(The2DAFile, buf);
    // Line 3 : columns
  	std::sprintf(buf, "    " );
    fprintf(The2DAFile, buf);
  	for( It1=AreasList.begin();It1!=AreasList.end();It1++){
  	  std::sprintf(buf, "%s    ", It1->second.c_str() );
 	    fprintf(The2DAFile, buf);
 	  }
  	std::sprintf(buf, "\n" );
    fprintf(The2DAFile, buf);
 	  // Line 4+ : data
  	std::sprintf(buf, "0   " );
    fprintf(The2DAFile, buf);
  	for( It1=AreasList.begin();It1!=AreasList.end();It1++){
  	  ////TextOut->WriteText("(ExportPathTable2DA) LINE Area %i name %s \n", It1->first,It1->second.c_str() );
  	  std::sprintf(buf, "%i    ", It1->first );
 	    fprintf(The2DAFile, buf);
 	  }
  	std::sprintf(buf, "\n" );
    fprintf(The2DAFile, buf);
    fclose (The2DAFile);
    
    //
    // Building the 2DA Cross Area table 
    // 
    //
    sprintf( filename, "ftp_%s_path.2DA", ModName.c_str() );
    The2DAFile         = fopen(filename,"w");
  	// Lines 1&2 : default
  	std::sprintf(buf, "2DA V2.0\n\n" );
    fprintf(The2DAFile, buf);
    // Line 3 : columns
  	std::sprintf(buf, "    " );
    fprintf(The2DAFile, buf);
   	for( It1=AreasList.begin();It1!=AreasList.end();It1++){
      std::sprintf(buf, "%i    ",It1->first );
      fprintf(The2DAFile, buf);
    } 	
  	std::sprintf(buf, "\n" );
    fprintf(The2DAFile, buf);
 	  // Line 4+ : data
  	for( It1=AreasList.begin();It1!=AreasList.end();It1++){
  	  ////TextOut->WriteText("(ExportPathTable2DA) LINE Area %i name %s \n", It1->first,It1->second.c_str() );
  	  std::sprintf(buf, "%i    ", It1->first );
 	    fprintf(The2DAFile, buf);
 	    
    	for( It2=AreasList.begin();It2!=AreasList.end();It2++){
      	 ////TextOut->WriteText("(ExportPathTable2DA) COLUMN Area %i name %s \n", It2->first,It2->second.c_str() );
      	 if( It1->first == It2->first ){
      	   ////TextOut->WriteText("(ExportPathTable2DA) LINE = COLUMN \n");
      	   std::sprintf( tmpbuf, "\****    ");
           strcpy( buf, tmpbuf );
         }
         else{
            TheTransitions = "";
            TheNode.SetAreas(It1->first,It2->first);
            ////TextOut->WriteText("(ExportPathTable2DA) Node%i(%i->%i)\n", TheNode.GetKey(),TheNode.AreaFrom,TheNode.AreaTo );
            TheWeightedNode   = PathTable[TheNode.GetKey()];
            TheTransitions        = Implode( Glue, TheWeightedNode.Transitions );
            std::sprintf( tmpbuf, "\"%i:%s\"    ", TheWeightedNode.Weight, TheTransitions.c_str() );           
            ////TextOut->WriteText("(ExportPathTable2DA) TheTransitions :%i = %s\n", TheWeightedNode.Transitions.size(), TheTransitions.c_str() );
             strcpy( buf, tmpbuf );
          }
          fprintf(The2DAFile, buf);
    	} // End of columns
    	
    	strcpy( buf, "\n" );
      fprintf(The2DAFile, buf);
     } // End of lines
    fclose (The2DAFile);
    
    //
    // Building the SQL Cross Area table 
    // 
    //
    sprintf( filename, "ftp_%s_path.sql", ModName.c_str() );
    The2DAFile         = fopen(filename,"w");
    sprintf( tablename, "ftp_%s_path", ModName.c_str() );
  	std::sprintf(buf,"DROP TABLE IF EXISTS `%s`;\n",tablename);fprintf(The2DAFile, buf);
  	std::sprintf(buf,"CREATE TABLE `%s`(\n",tablename);fprintf(The2DAFile, buf);
  	std::sprintf(buf,"`area_from` int unsigned NOT NULL ,\n");fprintf(The2DAFile, buf);
  	std::sprintf(buf,"`area_to` int unsigned NOT NULL ,\n");fprintf(The2DAFile, buf);
  	std::sprintf(buf,"`weight` int unsigned NOT NULL ,\n");fprintf(The2DAFile, buf);
 	  std::sprintf(buf,"`transitions` varchar(256) NOT NULL DEFAULT '',\n");fprintf(The2DAFile, buf);
 	  std::sprintf(buf,"PRIMARY KEY (`area_from`,`area_to`));\n");fprintf(The2DAFile, buf);
   	for( It1=AreasList.begin();It1!=AreasList.end();It1++){
  	  std::sprintf(area_from, "%s", It1->second.c_str() );
    	for( It2=AreasList.begin();It2!=AreasList.end();It2++){
      	 if( It1->first != It2->first ){
  	        std::sprintf(area_to, "%s", It2->second.c_str() );
            TheTransitions = "";
            TheNode.SetAreas(It1->first,It2->first);
            TheWeightedNode       = PathTable[TheNode.GetKey()];
            TheTransitions        = Implode( Glue, TheWeightedNode.Transitions );
  	        std::sprintf(buf, "INSERT INTO `%s`(`area_from`,`area_to`,`weight`,`transitions`) VALUES(%i,%i,%i,'%s');\n", tablename, It1->first,It2->first,TheWeightedNode.Weight, TheTransitions.c_str() );
            fprintf(The2DAFile, buf);
          }
    	} // End of area_to
    } // End of area_from
    
    fclose (The2DAFile);
    
} // End of ExportPathTable 



/**
 * Reports broken transitions
 **/ 
void 
ReportInvalidTransitions(
  __in IDebugTextOut * TextOut
){
  if( InvalidTransitionsList.size() < 1 ) {
    TextOut->WriteText(  "All the Transitions were valid.\n",  (*ITLIt).c_str( ) ); 
    return;
  }
 TextOut->WriteText(  "Invalid transitions found : .\n",  (*ITLIt).c_str( ) ); 
 for ( ITLIt=InvalidTransitionsList.begin() ; ITLIt != InvalidTransitionsList.end(); ITLIt++ ){
	  TextOut->WriteText(  " * #%s\n",  (*ITLIt).c_str( ) ); }

}