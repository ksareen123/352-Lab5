/*****************************************************************************
*                                                                            *
*       Source title:   Lab5_LNguyenKSareen.CPP
*                       (xxx SHORT DESCRIPTION xxx)                          *
*         Written by:   nnnnn n. nnnnn                                       *
*   Last Modified by:                                                        *
*               Date:   03/09/17                                            *
*        Current Rev:   r.rr                                                 *
*                              s                                              *
*        Description:                                                        *
*                                                                            *
*   Revision History:                                                        *
*                                                                            *
*     mm/dd/yy  r.rr  - Original coding.                                     *
*                                                                            *
*     03/09/17        - Added ALARM_DPU_DATACOUNT alarm for DPU has more     *
*                       points in named work area than captured.             *
*                       SSSR-7324) (DS)                                      *
*****************************************************************************/

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "port500d.h"
#include "cdef500d.h"
#include "util.h"
#include "gpib500d.h"
#include "config.h"
#include "testmain.h"
#include "Lab5_LNguyenKSareen.h"

/*****************************************************************************
*                                                                            *
*                         Miscellaneous (Module) #defines                    *
*                                                                            *
*****************************************************************************/

/*****************************************************************************
*                                                                            *
*                            Local (module) variables                        *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  NOTE: The Program Flow is a structure that defines the flow of the      //
//        test program. It is defined in the "\ets\inc\testmain.h" file.    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// CLab5_LNguyenKSareen
//
// The following declaration registers the test program with the test TestMain
// DLL. The value passed into the declaration specifies the Program Flow
// structure for the application. Passing in a value of ProductDatasheetFlow
// will direct the TestMain DLL to use the flow in the project's datasheet
// file.
//
CLab5_LNguyenKSareen theApp(ProductDatasheetFlow);

//{{AFX_ETS_PRODUCT_DATASHEET_VARIABLES
DECLARE_PDS_VARIABLE( PDS_INT, TestNmbr, "Test Number" );
DECLARE_PDS_VARIABLE( PDS_INT, SubTestNmbr, "Subtest Number" );
DECLARE_PDS_VARIABLE( PDS_CHAR, DLogDesc, "Datalog Description" );
DECLARE_PDS_VARIABLE( PDS_DOUBLE, HiLim, "High Limit" );
DECLARE_PDS_VARIABLE( PDS_DOUBLE, LoLim, "Low Limit" );
DECLARE_PDS_VARIABLE( PDS_DOUBLE, DFormat, "Data Format" );
DECLARE_PDS_VARIABLE( PDS_CHAR, Units, "Units" );
DECLARE_PDS_VARIABLE( PDS_CHAR, LoFBin, "Low Fail Bin" );
DECLARE_PDS_VARIABLE( PDS_CHAR, HiFBin, "High Fail Bin" );
//}}AFX_ETS_PRODUCT_DATASHEET_VARIABLES


//{{AFX_ETS_PINMAP(sites = 1)
// Caution::  The following pin definitions are filled in by the PinMap Editor.
extern "C" DLL_EXPORT int ETS_PINMAP_VERSION[] = {1,1};
extern "C" DLL_EXPORT UINT ETS_PinMap_Options = 0;
extern "C" DLL_EXPORT int ETS_PinMap_FirstSite = 0;
	DECLARE_PINMAP
		ADD_PIN (Vout, "", NAME_SP100)
		ADD_PIN (Vin, "", NAME_SP100)
	END_PINMAP
//}}AFX_ETS_PINMAP

//{{AFX_ETS_GROUPMAP
// Caution:: The following group definitions are filled in by the PinMap Editor
	DECLARE_GROUPMAP
	END_GROUPMAP
//}}AFX_ETS_GROUPMAP
/*****************************************************************************
*                                                                            *
*                           Main-support functions                           *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The UserInit() function is called by the 'TestMain' module one time     //
//  near the beginning of test execution, after all Global variables and    //
//  Global Switches have been initialized.                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::UserInit( void )
{
    //////////////////////////////////////////////////////////////////////////
    // Turn off the Auto Convert Units for the datasheet...
    //
    SetTestMainVariable(CONVERTUNITS, FALSE);
	
	mcuset( MCU_MAIN, 50); // set clock to 50 MHz
	mcumode(MCU_CH0, MCU_CLK, 500, MCU_CLKA, MCU_TRIGNORMAL, 0.0); // frequency division for channel 0
	mcusequence(MCU_CH0, "seq_1", "400 ON"); // create sequence of 400 pulses

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for sorting datalog statements while writing results
    //              to screen/disk/printer
    //
    // Datalogging sorting modes:
    //      NO_SORT
    //      BY_TESTNUMBER (default setting)
    //      BY_PROGFLOW_NOSORT
    //      BY_PROGFLOW
    //
#if 0
    SetDlogSort( BY_TESTNUMBER);
#endif

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for loading a DSS digital vector file for use with
    // digital boards on ETS-200, ETS-300, ETS-500, ETS-564 and BT-2000 testers.
    //
    // The dssload() utility copies the vector tags from the vector file
    // into an image in computer memory.
    //
    // The utility vecload() needs to be called after dssload() to program the tag
    // information into the tester local memory.
    //
    // dssload() will not erase any vector tags in computer memory from previous
    // calls to dssload(). If there is a vector tag already in computer memory
    // and the file being loaded has a vector with the same tag, a utility error
    // will be generated and the dssload() will fail.
    //
#if 0
    if ( dssload( GetTestMainVariable(VEC_FILE)) )                      // example load vec file
    {
        etsMessageBox("Could Not Initialize Vector File !!", MB_OK | MB_ICONSTOP);
        return FALSE;   // Abort Run of xx.DLL and exit TestExecutive
    }
#endif

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for loading a DPU digital vector file for use with
    // digital boards on ETS-364 and ETS-600 testers.
    //
    // The dpinvecload() utility copies the vector patterns from the vector file
    // into an image in computer memory.
    //
#if 0
    if ( dpinvecload( GetTestMainVariable(VEC_FILE)) )                      // example load vec file
    {
        etsMessageBox("Could Not Initialize Vector File !!", MB_OK | MB_ICONSTOP);
        return FALSE;   // Abort Run of xx.DLL and exit TestExecutive
    }
#endif

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for a multisite application.
    //
    // The purpose of this function is to have the user indicate that locations
    // of the data sheet arrays to the msLogResult() and msLogResultAll()
    // functions.
    //
    // THIS FUNCTION MUST BE CALLED PRIOR TO USING THE msLogResult() AND
    // msLogResultAll() FUNCTIONS.  This function should be called in UserInit()
    //
#if 1
    msLogResultInit( TestNmbr, SubTestNmbr, DLogDesc, DFormat, LoLim, HiLim, Units, LoFBin, HiFBin );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for a multisite application.
    //
    //  - First create a boolean array indicating which sites will be active.
    //  - Then call msSetSites() passing in this array.
    //
    // Additionally, ETS suppiles the FOR_EACH_SITE macro. This macro operates
    // as a C for-loop iterating through the list if valid sites in the test
    // program. This macro shown below is defined in the project's '.h' file.
    //
    // FOR_EACH_SITE( site, num_sites )
    // where:
    //         site - an integer that contains the current valid site of the
    //                iteration. During the iteration, 'site' will only contain
    //                site numbers of 'valid' test sites.
    //    num_sites - The total number of sites contained in the test program
    //
    // Example:  Log the X,Y coordinates for the test run...
    //
    //        FOR_EACH_SITE( site, total_sites )
    //        {
    //            msLogXYCoords( site, Xcoord, Ycoord );
    //        }
    //
#if 0
    int valid[NUM_SITES];
    int count;

    for( count = 0; count < NUM_SITES; count ++ )
        valid[count] = TRUE;
    msSetSites( NUM_SITES, valid );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) setup operator instructions in the test menu.
    //
    //  - Change '0' to '1' (in '#if 0' statement) if operator instructions are
    //     desired...
    //
    //  SYNTAX:
    //      SetOperatorInstruction( int linenumber, char *op_instr);
    //
    //  There are only two operator instruction lines allowed;
    //           line number can only be 1 or 2.
    //
    //  EXAMPLE:
    //      SetOperatorInstruction( 1, "Load DUT into socket and hit 'T' to test");
    //      SetOperatorInstruction( 2, "Multiple DUT test - hit F1 key");
    //
#if 0
    SetOperatorInstruction( 1, "Operator Instruction line #1" );
    SetOperatorInstruction( 2, "Operator Instruction line #2" );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) set the security access level for 'TestMain Menu' options
    //
    //  - Change '0' to '1' (in '#if 0' statement) if security access level is
    //    desired... (Options are defined in the TestMain.h file).
    //
    //    SECURITY LEVELS: 0 ( Operator ), 1 ( System Engineer )
    //
    //      OPTION#              Description                  SECURITY LEVEL (defaults)
    //      -------------------  ---------------------------  -------------------------
    //      TM_RUN               Enter Test Execution Menu    0 - Operator
    //      TM_SUMMARY           Display Summary Information  0 - Operator
    //      TM_DATASHEET         Select Device DataSheet      1 - System Engineer
    //      TM_CONTROLS          Test Control Panel           1 - System Engineer
    //      TM_TEST_SWITCHES     Test Switches Menu           1 - System Engineer
    //      TM_FMEA_SWITCHES     Failure Mode Analysis Menu   1 - System Engineer
    //      TM_USER_SWITCHES     User Switches Menu           1 - System Engineer
    //      TM_LOG_SWITCHES      Datalog Switches Menu        1 - System Engineer
    //      TM_PRINTER_SWITCHES  Printing Switches Menu       1 - System Engineer
    //      TM_DISPLAY_SWITCHES  Display Switches Menu        1 - System Engineer
    //      TM_COMPUTER_SETTINGS Computer Speed Settings Menu 1 - System Engineer
    //      TM_EXIT              Exit Test Executive          0 - Operator
    //
    //  - To allow access to items under the Test Control Panel (Test, FMEA, User,
    //    Datalog, Printer, Display or Computer Speed Menus), Access must be given to
    //    the Test Control Panel as well. These items are sub menus of the Test control
    //    menu.
    //
    //  SYNTAX:
    //    SetTestMainMenuSecurityLvl( Option#, Security_Level);
    //    * If Security_Level is 0, then Operators and System Engineers have access to
    //      the control.
    //    * If Security_Level is greater than 3, then no access is given to the
    //      particular control.
    //
    //  EXAMPLE:
    //    // Give Operator Access to User Switches menu
    //    SetTestMainMenuSecurityLvl( TM_CONTROLS, 0 );
    //    SetTestMainMenuSecurityLvl( TM_USER_SWITCHES, 0);
    //
#if 0
    SetTestMainMenuSecurityLvl( TM_CONTROLS, 1 );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) set the names of the General Purpose Switches and/or
    // Integers via a call to 'SetGlobalSwitchName()' and 'SetGlobalIntegerName()'.
    // There are 10 Global Switches and 10 Global Integers.
    //
    //  - Change '0' to '1' (in '#if 0' statement) if setting new names for
    //     the Global Switches/Integers ...
    //
    // The General Purpose Switches can be referenced by the following
    // variable names:
    //                                   Variable
    //                                     Name
    //                                   ---------
    //        General Purpose Switches: GP1  to GP10
    //        General Purpose Integers: GPI1 to GPI10
    //
    // SYNTAX:
    //   SetGlobalSwitchName( GlobalSwitch#, Name);
    //   SetGlobalIntegerName( GlobalInteger#, Name);
    //
#if 1
	SetGlobalSwitchName( GP1, "Site0");
#endif

    //////////////////////////////////////////////////////////////////////////
    // Enables alarm checking except for Debug mode alarm checking.
    //
    // SYNTAX:
    //      int alarmset( int alarmType, int state );
    //
    // 'alarmType' valid values:
    //      ALARM_ALL         = -1 : All alarms except debug mode
    //      ALARM_DEBUGMODE   =  0 : Engineering(slow) debug mode alarms
    //      ALARM_KELVIN      =  1 : Kelvin alarms
    //      ALARM_CLAMP       =  2 : Clamp alarms
    //      ALARM_SOA         =  3 : SOA alarm, Safe Operating Range
    //      ALARM_TEMPERATURE =  4 : Temperature alarms
    //      ALARM_RAILDROOP   =  5 : Rail droop alarm
    //      ALARM_OVERRANGE   =  6 : Over Range alarms
    //      ALARM_INTERLOCK   =  7 : Interlock alarms
    //      ALARM_FATALERROR  =  8 : Fatal error alarms
    //      ALARM_VOUTERROR   =  9 : Output Voltage alarms
    //      ALARM_OVERVOLTAGETRIP = 10 : Over Voltage alarms
    //      ALARM_DPU_RCVMID  = 11 : DPU Mid-Band alarms
    //      ALARM_DPU_RCVBAD  = 12 : DPU Receive Illegal State alarm
    //      ALARM_MCU_TIMEOUT = 13 : MCU Channel timeout alarm
    //      ALARM_OVERCURRENT = 14 : Over Current alarm
    //      ALARM_PIN_MISSING = 15 : Resource pin missing on active site.
    //      ALARM_DPU_DATACOUNT = 17 : DPU more points in named work area than captured
    
    // 'state' valid values:
    //      TRUE              =  1 : Enable specified alarms
    //      FALSE             =  0 : Disable specified alarms
    //
    // Debug Alarm Checking mode will not be enabled by this call.
    // Debug Alarm Checking is meant for engineering debug work and will
    // significantly increase test time.  Debug Alarm Checking can be
    // enabled with a switch on the Testexecutive's Controls menu.
    //
    alarmset( ALARM_ALL, TRUE );

    //////////////////////////////////////////////////////////////////////////
    // Returning FALSE will abort Loading of Test program and exit Test
    // Executive.
    //
    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The UserExit() function is called by the 'TestMain' module one time     //
//  near the end of test execution, before the Global variables and         //
//  Global Switches are saved.                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::UserExit( void )
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestInit() function is called before the display of the Test      //
//  Main Menu to reinitialize the DUT and/or reinitialize any program       //
//  variables that might need to be reset.  This function will be invoked   //
//  prior to calling the first function in the progflow structure.          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnTestInit( void )
{
    //////////////////////////////////////////////////////////////////////////
    // Returning FALSE will abort Entry into the Test/Quit prompt, and
    // return to Main Menu
    //
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestExit() function is called after the the selection of the      //
//  Quit button from the "Test", "Quit" Buttons Screen to reinitialize any  //
//  program variables or system resources that need to be "cleaned-up".     //
//  This function will be invoked regardless of the pass/fail status of     //
//  the DUT.                                                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnTestExit( void )
{
	
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestStartup() function is called before each test-run to          //
//  initialize any program variables or system resources that might need to //
//  be reset each time the test is run as opposed to only at the time of    //
//  program entry as in the OnTestInit() routine. This function will be     //
//  invoked prior to calling the first function in the progflow structure.  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnTestStartup( void )
{
    etsprintf( "DUT Socket Power ON\r" );

    //////////////////////////////////////////////////////////////////////////
    // Returning FALSE will abort Program flow execution and return to
    // Test/Quit prompt.
    //
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestCompletion() function is called after each test-run to        //
//  re-initialize any program variables or system resources that might need //
//  to be reset after each test suite is run This function will be invoked  //
//  after to calling the last function in the progflow structure.           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnTestCompletion( void )
{
	sp100set( Vin, SP_OFF, 0.0, SP_30V, SP_1A );
	sp100set( Vout, SP_OFF, 0.0, SP_30V, SP_1A );
    etsprintf( "DUT Socket Power OFF\n" );
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnLogData() function is called During a call to the LogData(),      //
//  msLogData() and msLogDataAll() functions.                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnLogData( void )
{
    int PassFail;

    if ( ( PassFail = GetTestMainVariable(PASSFAIL) ) == 2 )
    {
        //////////////////////////////////////////////////////////////////////
        // An alarm condition is present.  To determine what alarm(s) are
        // present, break on this line of code in the debugger, switch
        // to Raide and call alarmstat( 0 );
        //
        PostAlarm( "ALARM" );
    }
    else if ( PassFail )
        PostAlarm( "FAIL" );
    else
        PostAlarm( "    " );

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnFailSite() function is called During a call to the msFailSite()   //
//  to enable the user to turn off and invalidate a particular site         //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnFailSite( int site )
{
	if (site == MS_SITE0) {
		sp100set( Vin, SP_OFF, 0.0, SP_30V, SP_1A );
		sp100set( Vout, SP_OFF, 0.0, SP_30V, SP_1A );
		etsprintf( "DUT Socket Power OFF\n" );
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnSot() function is called while waiting for a valid SOT            //
//  signal either from the Operator Box, Handler, or user interface.        //
//  SotValue contains the SOT event.                                        //
//                                                                          //
//    -2  =   Handler signaled SOT                                          //
//    -1  =   Operator Box SOT switch Pressed                               //
//    'T' =   Key Board 'T'/ or Test Button Pressed                         //
//    'Q' =   Key Board 'Q'/ or Quit Button Pressed                         //
//    F1_KEY - F5_KEY  = special Test Key/Button Pressed                    //
//    F6_KEY - F10_KEY = Datalog Mode/Binning Summary State                 //
//                       Change requested                                   //
//                                                                          //
//  The SotValue is the virtual key code of the key                         //
//  hit on the keyboard (NOT ASCII CODE). When trapping specific            //
//  key hits, use the windows defines for the virtual key codes. These      //
//  defines are: (alpha codes A thru Z)   VK_A, VK_B, ..., VK_Z             //
//               (numeric codes 0 thru 9) VK_0, VK_1, ..., VK_9             //
//               (function key codes F1 - F12) F1_KEY, F2_KEY, ..., F12_KEY //
//                                                                          //
//  Upon return from this function, the TestMain Framework will take action //
//  on the SotValue.                                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

int CLab5_LNguyenKSareen::OnSot(int SotValue)
{
    return SotValue;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnUtilError function is called in response to an ETS Utility        //
//  error. The ETS Utility Error Number is passed in to the user so         //
//  certain errors can be trapped.                                          //
//                                                                          //
//  Returning TRUE will pass the error into the ETS Message Handler         //
//  program, and returning FALSE will direct the Message Handler to ignore  //
//  the error.                                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnUtilError(int errnum)
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  This function will automatically be called each time the interlock is   //
//  check by the Test Executive. The interlock is checked before each DUT   //
//  tested, before the call to the user's OnTestStartup() routine. The      //
//  intlocks parameter passed in is a bitmapped value containing the state  //
//  of all interlocks in the ETS Test head.                                 //
//                                                                          //
//  Returning TRUE will pass the interlock check. Returning FALSE will fail  //
//  the interlock check. If the intlocks variable is non-zero the return    //
//  value has no effect. If the interlock check fails, the DUT will not be  //
//  tested.                                                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab5_LNguyenKSareen::OnInterlock(ULONG intlocks)
{
    return TRUE;
}

/*****************************************************************************
*                                                                            *
*                          Local (module) functions                          *
*                   User Specific Application Code Follows                   *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// ---------------------------[ Test Functions ]--------------------------- //
// The functions defined here are the building-blocks of the test program.  //
// Executed as ordered by the Program Flow structure array, each returns a  //
// status of SUCCESS (0) or FAILURE (-1) to indicate to the summary system  //
// whether the DUT passed each test (as controlled by the 'STOP_ON_FAIL'    //
// global switch).                                                          //
//                                                                          //
// Any (floating-point) data that is to be recorded by the datalog system   //
// is logged to, in calling-sequence order, via LogData() or msLogData()    //
// (for multisite applications) function calls.                             //
//                                                                          //
//  The function 'etsprintf()' is used to write to the TestMain Console     //
//  window. the syntax of etsprintf() is identical to C's printf(). This    //
//  function should be used if wanting to print to screen inside of test    //
//  program.                                                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

// Test Function: UserTest
//ETS_PRGFLOW_FUNC UserTest( int DSIndex, LPCTSTR TestLabel )
//{
//    etsprintf("This is the User's Test\n");
//    return SUCCESS;
//} // END_ETS_PRGFLOW_FUNC


// Test Function: DropVThreshold
ETS_PRGFLOW_FUNC DropVThreshold( int DSIndex, LPCTSTR TestLabel )
{
    
    // Dropout - the dropout voltage is the minimum differenc3e between the input and output voltage where Vout remains in spec according to the datasheet
	// Difference between the input and output voltages when the Vout is regular
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
	
	double dropoutv = 0.0;
	double pwr_in = 0.0;
	double pwr_out = 0.0;
	double pwr_waste = 0.0;
     
    sp100clamp( Vin, 12, 1.665, SP_NOCLAMP, SP_NOCLAMP ); //clamp negative to -0.5V
	
	sp100set( Vin, SP_FV, 7, SP_30V, SP_100MA ); //force 7 volts to Vin
	sp100set( Vout, SP_FI, -5, SP_30V, SP_100MA );//sink current

	lwait(1000);
	dropoutv = sp100mv( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	pwr_in = dropoutv;
	dropoutv = 7-dropoutv;
	pwr_in *= sp100mi( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	
	pwr_out = sp100mv( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 ) * sp100mi( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	
	msLogResult(MS_SITE0, DSIndex++, dropoutv );
	msLogResult(MS_SITE0, DSIndex++, pwr_in);
	msLogResult(MS_SITE0, DSIndex++, pwr_out);
	pwr_waste = pwr_in + pwr_out;
	msLogResult(MS_SITE0, DSIndex++, pwr_waste);
	
	sp100awgloader( Vin, "Dropout Voltage Test", SP_FV, SP_30V, SP_1A, Dropout_Voltage_Test_Data, (sizeof(Dropout_Voltage_Test_Data)/sizeof(double)), &Dropout_Voltage_Test_Regions ); //dropout awg setup

	//declare variables for storing voltage
	double dropoutvoltage;

	//connect mcu to Vout and Vin pins for awg
	mcuconnect( MCU_CH0, "OFF"); //Connect MCU
	mcuconnect( MCU_CH0, "GROUP%d,GROUP%d", Vout, Vin);

	//set awg and force approriate current
	sp100set(Vin, SP_FVAWG, 7.5, SP_30V, SP_1A); //set to follow voltage of AWG
	sp100set(Vout, SP_FI, -5, SP_30V, SP_1A); //Sink current at Vout	

	//set adc mode and selet appropriate current
	sp100adcmode(Vout, SP_MV_1X, SP_MI_OFF, 1); //config adc mode
	sp100awgselect(Vin, "Dropout Voltage Test" ); //select your awg

	//wait
	lwait(1000);

	//run mcu sequence
	mcurun("seq_1",NULL, 1);

	//read your data points and create a names array
	blockread(Vout, 400); //read all data
	namearray ("VREG_Dropout", NAMEARRAY_SP100_VADC, Vout, 1);

	//find voltage location ( whereat() )
	double datadrop = whereat("VREG_Dropout", 1, 400, 4.8); //4.8 may not be the value leaving it here for now
	etsprintf("point at which dropout occurs: %d", datadrop);
	dropoutvoltage = 10 - datadrop * (10 / 400);
	dropoutvoltage -= 7.5;
	//log the results
	msLogResult(0, DSIndex++, dropoutvoltage);

	//safe shutdown
	sp100set( Vin, SP_OFF, 0.0, SP_30V, SP_1A );
	sp100set( Vout, SP_OFF, 0.0, SP_30V, SP_1A );

	lwait(1); //breakpont here

    return( msSiteStat( MS_ALL ) ); // Return w/status

	
} // END_ETS_PRGFLOW_FUNC

// Test Function: LoadReg
ETS_PRGFLOW_FUNC LoadReg( int DSIndex, LPCTSTR TestLabel )
{
	

    // Load Regulation Test: Tests the ability of the DUT to maintain a constant output voltage when the load is varied and the input voltage is constant
	// i.e. change current make shure output voltage stays the same
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
    sp100clamp( Vin, 12, 1.665, SP_NOCLAMP, SP_NOCLAMP ); //clamp negative to -0.5V

	//declare variables
	double result = 0.0;
	double in_voltage = 0.0;
	double in_current = 0.0;
	double out_voltage = 0.0;
	double out_current = 0.0;
	double pwr_in = 0.0;
	double pwr_out = 0.0;
	double waste_min = 0.0;
	double waste_max = 0.0;
	
	sp100set(Vin, SP_FV, 8, SP_30V, SP_1A); //setting input voltage	
	sp100set(Vout, SP_FI, -500, SP_30V, SP_1A); //sink current	
	
	lwait(1000);

	result = sp100mv( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_voltage = sp100mv( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_current = sp100mi( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	out_voltage = result;
	out_current = sp100mi( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	pwr_in = in_voltage * in_current;
	pwr_out = out_voltage * out_current;
	waste_max = pwr_in + pwr_out;
	
	sp100set( Vout, SP_FI, -250, SP_30V, SP_1A ); //sink current

	lwait(1000);

	result -= sp100mv( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_voltage = sp100mv( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_current = sp100mi( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	out_voltage = result;
	out_current = sp100mi( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );

	msLogResult(MS_SITE0, DSIndex++, result);//logging results for line reg
	msLogResult(MS_SITE0, DSIndex++, pwr_in);//logging results for min before max power calculations
	msLogResult(MS_SITE0, DSIndex++, pwr_out);
	msLogResult(MS_SITE0, DSIndex++, waste_max);

	pwr_in = in_voltage * in_current;  //max power calculations
	pwr_out = out_voltage * out_current;
	waste_min = pwr_in + pwr_out;
	
	msLogResult(MS_SITE0, DSIndex++, pwr_in);//logging results for max power calculations
	msLogResult(MS_SITE0, DSIndex++, pwr_out);
	msLogResult(MS_SITE0, DSIndex++, waste_min);

	sp100awgloader( Vout, "Load regulation test", SP_FI, SP_30V, SP_1A, Load_regulation_test_Data, (sizeof(Load_regulation_test_Data)/sizeof(double)), &Load_regulation_test_Regions ); // load regulation AWG setup
	
	//Declare variables for storng peaks and differenc
	double LoadRegMax, LoadRegMin;

	//Connect MCU to Vout and Vin Pins for AWG
	mcuconnect( MCU_CH0, "OFF"); //connect mcu
	mcuconnect(MCU_CH0, "GROUP%d,GROUP%d", Vout, Vin);

	//Set AWG and force appropriate voltage
	sp100set(Vout, SP_FIAWG, -250, SP_30V, SP_1A);
	sp100set(Vin, SP_FV, 8, SP_30V, SP_1A);

	//Set adc mode and select appropriate AWG
	sp100adcmode(Vout, SP_MV_1X, SP_MI_OFF,1); //Configure Adc
	sp100awgselect(Vout, "Load regulation test"); //Select your awg

	//Wait
	lwait(1000);

	//Run MCU sequence
	mcurun("seq_1" ,NULL);

	//Read your data points and create a names arrray
	blockread(Vout, 375); //Read data before AWG returns to 0
	namearray("VREG_LoadReg", NAMEARRAY_SP100_VADC, Vout, 1); //store values into array

	//Calculate the hi and lo peaks using appropriate math functions
	LoadRegMax = peak("VREG_LoadReg", 100, 375, PEAK_HI);
	LoadRegMin = peak("VREG_LoadReg", 100, 375, PEAK_LO);

	//Log results
	msLogResult(0, DSIndex++, LoadRegMax);
	msLogResult(0, DSIndex++, LoadRegMin);

	//safe shutdown
	sp100set( Vin, SP_OFF, 0.0, SP_30V, SP_1A );
	sp100set( Vout, SP_OFF, 0.0, SP_30V, SP_1A );

	lwait(1); //breakpoint here

    return( msSiteStat( MS_ALL ) ); // Return w/status
	
} // END_ETS_PRGFLOW_FUNC

// Test Function: LineReg
ETS_PRGFLOW_FUNC LineReg( int DSIndex, LPCTSTR TestLabel )
{
   
    // Line Regulation - This tests the ability of the DUT to maintain a constant output voltage when teh input voltage is varied and the load is constant
	// Change input voltage and check if output voltage stays constant
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
    sp100clamp( Vin, 12, 1.665, SP_NOCLAMP, SP_NOCLAMP ); //clamp negative to -0.5V

	//declare variables
	double result = 0.0;
	double in_voltage = 0.0;
	double in_current = 0.0;
	double out_voltage = 0.0;
	double out_current = 0.0;
	double pwr_in = 0.0;
	double pwr_out = 0.0;
	double waste_min = 0.0;
	double waste_max = 0.0;
	
	sp100set( Vin, SP_FV, 12, SP_30V, SP_100MA ); //Vin max
	sp100set(Vout, SP_FI, -5, SP_30V, SP_100MA); //sink current

	lwait(1000);

	result = sp100mv( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_voltage = sp100mv( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_current = sp100mi( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	out_voltage = result;
	out_current = sp100mi( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	pwr_in = in_voltage * in_current;
	pwr_out = out_voltage * out_current;
	waste_max = pwr_in + pwr_out;
	
	sp100set( Vin, SP_FV, 8, SP_30V, SP_100MA ); //Vin min	
	sp100set(Vout, SP_FI, -5, SP_30V, SP_100MA); //sink current	

	lwait(1000);

	result -= sp100mv( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_voltage = sp100mv( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	in_current = sp100mi( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 );
	out_voltage = result;
	out_current = sp100mi( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );

	msLogResult(MS_SITE0, DSIndex++, result);//logging results for line reg
	msLogResult(MS_SITE0, DSIndex++, pwr_in);//logging results for min before max power calculations
	msLogResult(MS_SITE0, DSIndex++, pwr_out);
	msLogResult(MS_SITE0, DSIndex++, waste_max);

	pwr_in = in_voltage * in_current;  //max power calculations
	pwr_out = out_voltage * out_current;
	waste_min = pwr_in + pwr_out;
	
	msLogResult(MS_SITE0, DSIndex++, pwr_in);//logging results for max power calculations
	msLogResult(MS_SITE0, DSIndex++, pwr_out);
	msLogResult(MS_SITE0, DSIndex++, waste_min);
	
	sp100awgloader( Vin, "Line regulation test", SP_FV, SP_30V, SP_1A, Line_regulation_test_Data, (sizeof(Line_regulation_test_Data)/sizeof(double)), &Line_regulation_test_Regions ); // line regulation awg setup

	//Declare variables for storng peaks and difference
	double LineRegMax, LineRegMin, diff;

	//Connect MCU to Vout and Vin Pins for AWG
	mcuconnect( MCU_CH0, "OFF"); //connect mcu
	mcuconnect(MCU_CH0, "GROUP%d,GROUP%d", Vout, Vin);

	//Set AWG and force appropriate current
	sp100set(Vin, SP_FVAWG, 7.5, SP_30V, SP_1A);
	sp100set(Vout, SP_FI, -5, SP_30V, SP_1A);

	//Set adc mode and select appropriate AWG
	sp100adcmode(Vout, SP_MV_1X, SP_MI_OFF,1); //Configure AWG
	sp100awgselect(Vin, "Line regulation test"); //Select your awg

	//Wait
	lwait(1000); 

	//Run MCU sequence
	mcurun("seq_1" ,NULL);

	//Read your data points and create a names arrray
	blockread(Vout, 375); //Read data before AWG returns to 0
	namearray("VREG_LineReg", NAMEARRAY_SP100_VADC, Vout, 1); //store values into array

	//Calculate the hi and lo peaks using appropriate math functions
	LineRegMax = peak("VREG_LineReg", 100, 375, PEAK_HI);
	LineRegMin = peak("VREG_LineReg", 100, 375, PEAK_LO);

	//Log results
	msLogResult(0, DSIndex++, LineRegMax);
	msLogResult(0, DSIndex++, LineRegMin);

	
	
	//safe shutdown
	sp100set( Vin, SP_OFF, 0.0, SP_30V, SP_1A );
	sp100set( Vout, SP_OFF, 0.0, SP_30V, SP_1A );

	lwait(1); //set breakpoint here for debug; check RADE to make sure device not driven before data analysis
	
    return( msSiteStat( MS_ALL ) ); // Return w/status
	
} // END_ETS_PRGFLOW_FUNC

// Test Function: NoLoad
ETS_PRGFLOW_FUNC NoLoad( int DSIndex, LPCTSTR TestLabel )
{
    
    // No Load Min/Max - Often the data sheet requires a small min load.  This test determines the typical voltage output when virtually no current is pulled from the output pin while
	// set no current from output pin and set voltage to min and max and see if output voltage remains constant
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
    
	
    sp100clamp( Vin, 12, 1.665, SP_NOCLAMP, SP_NOCLAMP ); //clamp negative to -0.5V
	
	//declare variables
	double result = 0.0;
	double in_voltage = 0.0;
	double in_current = 0.0;
	double out_voltage = 0.0;
	double out_current = 0.0;
	double min_in_pwr = 0.0;
	double min_out_pwr = 0.0;
	double max_in_pwr = 0.0;
	double max_out_pwr = 0.0;
	double min_waste = 0.0;
	double max_waste = 0.0;
	
	//minimum load
	sp100set( Vin, SP_FV, 8, SP_30V, SP_100MA );	//set min voltage 8V
	sp100set( Vout, SP_FI, -5, SP_30V, SP_100MA ); //sink current into output pin

	lwait(1000); //small wait

	result = sp100mv( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 ); //measure output voltage at a minimum

	in_voltage = sp100mv( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 ); //measure input voltage at a minimum
	out_voltage = result; //set output voltage for power calc
	in_current = sp100mi( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 ); //measure input current
	out_current = sp100mi( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 ); //measure output current

	min_in_pwr = in_voltage * in_current; // power calculation
	min_out_pwr = out_voltage * out_current; // power calculation
	msLogResult( MS_SITE0, DSIndex++, result); //Place measured result into mslogResult - FIRST ROW
	
	//maximum load
	sp100set(Vin, SP_FV, 12, SP_30V, SP_100MA);//Force maximum voltage into input pin
	sp100set( Vout, SP_FI, -5, SP_30V, SP_100MA );//sink current inot output pin

	lwait(1000);//small wait

	result = sp100mv(Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 );//measure output voltage at a max

	in_voltage = sp100mv( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 ); //measure input voltage at a minimum
	out_voltage = result; //set output voltage for power calc
	in_current = sp100mi( Vin, SP_MV_1X, SP_MI_1X, 4000, 13.0 ); //measure input current
	out_current = sp100mi( Vout, SP_MV_1X, SP_MI_1X, 4000, 13.0 ); //measure output current

	max_in_pwr = in_voltage * in_current; // power calculation
	max_out_pwr = out_voltage * out_current; // power calculation
	msLogResult(MS_SITE0, DSIndex++, result);//place measured result into mslogresult - SECOND ROW

	msLogResult(MS_SITE0, DSIndex++, min_in_pwr); // - THIRD ROW
	msLogResult(MS_SITE0, DSIndex++, min_out_pwr); // - FOURTH ROW
	min_waste = min_in_pwr + min_out_pwr;

	msLogResult(MS_SITE0, DSIndex++, min_waste); // - FIFTH ROW

	msLogResult(MS_SITE0, DSIndex++, max_in_pwr); // - SIXTH ROW 
	msLogResult(MS_SITE0, DSIndex++, max_out_pwr); // - SEVENTH ROW
	max_waste = max_in_pwr + max_out_pwr;

	msLogResult(MS_SITE0, DSIndex++, max_waste); // - EIGHTH ROW
	

	// LAB 5.2
	sp100awgloader( Vin, "Leo and Krish AWG test", SP_FV, SP_30V, SP_1A, Leo_and_Krish_AWG_test_Data, (sizeof(Leo_and_Krish_AWG_test_Data)/sizeof(double)), &Leo_and_Krish_AWG_test_Regions );// AWG setup for this graph
	
	double NoLoadMax, NoLoadMin;
	
	//Connect MCU
	mcuconnect(MCU_CH0, "OFF");
	mcuconnect(MCU_CH0, "GROUP%d, GROUP%d", Vout, Vin); //connect to Vout and Vin
	
	//Set AWG and force appropriate current
	sp100set(Vin, SP_FVAWG, 7.5, SP_30V, SP_1A);
	sp100set(Vout, SP_FI, -5, SP_30V, SP_1A);
	
	//set adc mode and select appropriate AWG
	sp100adcmode(Vout, SP_MV_1X, SP_MI_OFF, 1 ); //configure AWG
	sp100awgselect( Vin, "Leo and Krish AWG test"); // select AWG
	    
	//wait
	lwait(1000);
	
	//Run MCU sequence
	mcurun("seq_1", NULL, 1);
	
	//read data points
	blockread(Vout, 375); //read data before AWG returns to 0
	namearray("VREG_NoLoad", NAMEARRAY_SP100_VADC, Vout, 1); //store values in array, created and named by this method
	
	//calculate hi/lo peaks
	NoLoadMax = peak("VREG_NoLoad", 100, 375, PEAK_HI); // calculate high
	NoLoadMin = peak("VREG_NoLoad", 100, 375, PEAK_LO); // calculate low

	//log results
	msLogResult(0, DSIndex++, NoLoadMax);
	msLogResult(0, DSIndex++, NoLoadMin);

	

    //safe shutdown
	sp100set( Vin, SP_OFF, 0.0, SP_30V, SP_1A );
	sp100set( Vout, SP_OFF, 0.0, SP_30V, SP_1A );

    lwait(1); //set breakpoint here
   
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
