/*****************************************************************************
*                                                                            *
*       Source title:   Lab5_LNguyenKSareen.H
*                       main header file for the application                 *
*         Written by:   nnnnn n. nnnnn                                       *
*   Last Modified by:                                                        *
*               Date:   mm/dd/yy                                             *
*        Current Rev:   r.rr                                                 *
*                                                                            *
*        Description:                                                        *
*                                                                            *
*   Revision History:                                                        *
*                                                                            *
*     mm/dd/yy  r.rr  - Original coding.                                     *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// CLab5_LNguyenKSareen:
// See Lab5_LNguyenKSareen.cpp for the implementation of this class
//
//{{AFX_DEFINE_ACE_GROUP( Datasheet Array Size )
#define DSSIZE 39
//}}AFX_DEFINE_ACE_GROUP
//{{AFX_ETS_FUNCTION_PROTOTYPES
ETS_PRGFLOW_FUNC DropVThreshold( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC LoadReg( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC LineReg( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC NoLoad( int DSIndex, LPCTSTR TestLabel );
//ETS_PRGFLOW_FUNC UserTest( int DSIndex, LPCTSTR TestLabel );
//}}AFX_ETS_FUNCTION_PROTOTYPES


//{{AFX_ETS_PIN_DEFINITION
// Caution::  The following pin definitions are filled in by the PinMap Editor.
#define ADD_PIN( pinname, notes, resource )  \
	PINMAP_T( #pinname, notes, pinname, resource, \
		pinname##_S0 ),

#define      PIN_DEFINITION( pinname, pingrpval, site0 ) \
	int const pinname##_S0 = site0; \
	int const pinname = pingrpval;

	PIN_DEFINITION (Vout, 8193 , 7)	// defines  Vout_S0
	PIN_DEFINITION (Vin, 8192 , 6)	// defines  Vin_S0
//}}AFX_ETS_PIN_DEFINITION

//{{AFX_ETS_GROUP_DEFINITION
// Caution:: The following group definitions are filled in by the PinMap Editor
//}}AFX_ETS_GROUP_DEFINITION
class CLab5_LNguyenKSareen : public CTestMain
{
public:
    CLab5_LNguyenKSareen(PRG_FLOW *UserFlow) : CTestMain(UserFlow) {};
    ~CLab5_LNguyenKSareen(){};

//////////////////////////////////////////////////////////////////////////////
// Overridable Functions
//
//  To eliminate the need for any of these functions,
//  Remove their declaration below, as well as the corresponding
//  Function body in the Lab5_LNguyenKSareen.cpp file
//
    virtual BOOL  UserInit();                   // Called when user starts Application Program
    virtual BOOL  UserExit();                   // Called when user quits Application Program
    virtual BOOL  OnTestStartup();              // Called Before Program Flow Structure Execution
    virtual BOOL  OnTestCompletion();           // Called After Program Flow Structure Execution
    virtual BOOL  OnTestInit();                 // Called before 'GO / QUIT' buttons Displayed
    virtual BOOL  OnTestExit();                 // Called after 'QUIT' button pressed
    virtual BOOL  OnLogData();                  // Called during Data logging
    virtual BOOL  OnFailSite(int site);         // Called during the msFailSite() utility
    virtual int   OnSot(int SotValue);          // Called while waiting for Start Of Test signal
    virtual BOOL  OnUtilError(int errnum);      // Called by the Utilities when an ETS Util Error occurs
    virtual BOOL  OnInterlock(ULONG intlocks);  // Called during the interlock check...
};

//////////////////////////////////////////////////////////////////////////////
//
// ACE Define Block
//
// ETS AceWizard generated define blocks
//
//{{AFX_DEFINE_ACE_GROUP( Miscellaneous Defines )
    #define FOR_EACH_SITE(s,m) \
                for ( (s) = 0; (s) < (m); (s)++ ) \
                    if ( msSiteStat( (s) ) )
//}}AFX_DEFINE_ACE_GROUP

//{{AFX_DEFINE_ACE_GROUP( Group Block 1 )
    // NOTE: Add define members here for the group "Group Block 1"
#define NUM_SITES 1
//}}AFX_DEFINE_ACE_GROUP

//{{AFX_DEFINE_ACE_GROUP( Group Block 2 )
    // NOTE: Add define members here for the group "Group Block 2"
//}}AFX_DEFINE_ACE_GROUP

#include "Lab5_LNguyenKSareen_Awg.h"

