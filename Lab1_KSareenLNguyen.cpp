/*****************************************************************************
*                                                                            *
*       Source title:   Lab1_KSareenLNguyen.CPP
*                       (xxx SHORT DESCRIPTION xxx)                          *
*         Written by:   nnnnn n. nnnnn                                       *
*   Last Modified by:                                                        *
*               Date:   03/09/17                                            *
*        Current Rev:   r.rr                                                 *
*                                                                            *
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
#include "Lab1_KSareenLNguyen.h"

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
// CLab1_KSareenLNguyen
//
// The following declaration registers the test program with the test TestMain
// DLL. The value passed into the declaration specifies the Program Flow
// structure for the application. Passing in a value of ProductDatasheetFlow
// will direct the TestMain DLL to use the flow in the project's datasheet
// file.
//
CLab1_KSareenLNguyen theApp(ProductDatasheetFlow);

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
		ADD_PIN (RES_APU, "", NAME_APU)
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

BOOL CLab1_KSareenLNguyen::UserInit( void )
{
    //////////////////////////////////////////////////////////////////////////
    // Turn off the Auto Convert Units for the datasheet...
    //
    SetTestMainVariable(CONVERTUNITS, FALSE);

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
	SetGlobalSwitchName( GP1, "SITE0");
#if 0
    SetGlobalSwitchName( GP2, "General Purpose ON/OFF Switch#2:" );
    SetGlobalIntegerName( GPI1, "General Purpose Integer Switch#1:" );
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

BOOL CLab1_KSareenLNguyen::UserExit( void )
{
	apuset( RES_APU, APU_STANDBY, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shut of
	etsprintf("APU resistor disconnected. \n");
	etsprintf("DUT Socket Power OFF\n");
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

BOOL CLab1_KSareenLNguyen::OnTestInit( void )
{
    //////////////////////////////////////////////////////////////////////////
    // Returning FALSE will abort Entry into the Test/Quit prompt, and
    // return to Main Menu
    //

	int valid[NUM_SITES];
	valid[MS_SITE0] = GetTestMainVariable(GP1);
	msSetSites(NUM_SITES, valid);
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

BOOL CLab1_KSareenLNguyen::OnTestExit( void )
{
	apuset( RES_APU, APU_STANDBY, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shut of
	etsprintf("APU resistor disconnected. \n");
	etsprintf("DUT Socket Power OFF\n");
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

BOOL CLab1_KSareenLNguyen::OnTestStartup( void )
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

BOOL CLab1_KSareenLNguyen::OnTestCompletion( void )
{
	apuset( RES_APU, APU_STANDBY, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shutdown
	etsprintf("APU resistor disconnected. \n");
	
    etsprintf( "DUT Socket Power OFF\n" );
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnLogData() function is called During a call to the LogData(),      //
//  msLogData() and msLogDataAll() functions.                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab1_KSareenLNguyen::OnLogData( void )
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

BOOL CLab1_KSareenLNguyen::OnFailSite( int site )
{
	if(site == MS_SITE0) {
		etsprintf("FAILURE \n");

		apuset( RES_APU, APU_STANDBY, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shut off
		etsprintf("APU resistor disconnected.\n");

		etsprintf("DUT Socket Power OFF");
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

int CLab1_KSareenLNguyen::OnSot(int SotValue)
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

BOOL CLab1_KSareenLNguyen::OnUtilError(int errnum)
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

BOOL CLab1_KSareenLNguyen::OnInterlock(ULONG intlocks)
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






// Test Function: MeasureVoltage
ETS_PRGFLOW_FUNC MeasureVoltage( int DSIndex, LPCTSTR TestLabel )
{
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
    //definitions
	double resistorVoltage;

	//Force current
	apuset( RES_APU, APU_FI, 5, APU_10V, APU_100MA, PIN_TO_VI ); // Force 5 mA 
	lwait(1000); // short wait
	resistorVoltage = apumv( RES_APU, 4000, 13.0); // measure
	
	//Calculate
	msLogResult( MS_SITE0, DSIndex++, resistorVoltage ); // log values
	
	//run shutdown code
	apuset( RES_APU, APU_STANDBY, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shut off
		
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: MeasureCurrent
ETS_PRGFLOW_FUNC MeasureCurrent( int DSIndex, LPCTSTR TestLabel )
{
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
    
	// define variables
	double resistorCurrent;

	//Force voltage, measure current
	apuset( RES_APU, APU_FV, 5, APU_10V, APU_100MA, PIN_TO_VI ); // force 5 volts 
	lwait(1000); // delay
	resistorCurrent = apumi( RES_APU, 4000, 13.0 ); // measure the current through the resistor
	
	// Log
	msLogResult( MS_SITE0, DSIndex++, resistorCurrent ); // log it

	// Shutdown code
	apuset( RES_APU, APU_VIOFF, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shutdown code

    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: ResistanceForceCurrent
ETS_PRGFLOW_FUNC ResistanceForceCurrent( int DSIndex, LPCTSTR TestLabel )
{
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
    //define variables
	double resistorVoltage, resistance;

	// Force current
	apuset( RES_APU, APU_FI, 5, APU_10V, APU_100MA, PIN_TO_VI ); // force 5 mA
	lwait(1000); // delay
	resistorVoltage = apumv( RES_APU, 4000, 13.0); // measure the voltage accross the resistor
	
	//Calculate and log
	resistance = resistorVoltage / (5.0 / 1000); // calculate resistance based on voltage and current
	msLogResult( MS_SITE0, DSIndex++, resistance ); // log

	//shutdown
	apuset( RES_APU, APU_STANDBY, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shutdown
     
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: ResistanceForceVoltage
ETS_PRGFLOW_FUNC ResistanceForceVoltage( int DSIndex, LPCTSTR TestLabel )
{
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
    
	//define variables
	double resistorCurrent, resistance;

	// Force V
	apuset( RES_APU, APU_FV, 5, APU_10V, APU_100MA, PIN_TO_VI ); // force 5 volts
	lwait(1000); // delay
	resistorCurrent = apumi( RES_APU, 4000, 13.0); // measure the i accross the resistor
	
	//Calculate and log
	resistance = 5.0 / (resistorCurrent / 1000); // calculate resistance based on voltage and current
	msLogResult( MS_SITE0, DSIndex++, resistance ); // log

	//shutdown
	apuset( RES_APU, APU_STANDBY, 0.0, APU_10V, APU_100MA, PIN_TO_VI ); // shutdown
     
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
