#include "stdafx.h"
#include "../../furr/furramscomm.h"
#include <iostream>

using namespace std;

/*
 * Function signatures
 */
ANSISTRING Prompt(const ANSISTRING &prompt);
void Route(const ANSISTRING &strCmd);
void DoOpen(const ANSISTRING &strCmd);
void DoClose(const ANSISTRING &strCmd);
void DoSetAxis(const ANSISTRING &strCmd);
void DoCommand(const ANSISTRING &strCmd);
void Callback(CTSTRING &ts, DWORD dw);
ANSISTRING ArgFromCmdLine(const ANSISTRING &strCmd);
ANSISTRING CmdFromCmdLine(const ANSISTRING &strCmd);

/*
 * Typedefs
 */
typedef void (*CMDFN)(const ANSISTRING&);

typedef struct tagLOOKUP {

  ANSISTRING strCommand;
  CMDFN pfnRoute;

} LOOKUP, *PLOOKUP;

/*
 * Global variables
 */
FURRAMSComm g_Comm;                   /* Comm class */

ANSISTRING g_strPort    = "COM1"; /* Communications port (Default = COM1) */
char g_cAxis        = 'C';        /* Axis name (Default = 'C') */
bool g_bOpen        = false;      /* Is port open? */
const int g_NumFns  = 5;          /* Number of functions */

static LOOKUP g_Table[] = {       /* Function lookup table */

  {"OPEN", DoOpen},
  {"QUIT", DoClose},
  {"EXIT", DoClose},
  {"CLOSE", DoClose},
  {"SETAXIS", DoSetAxis}

};

/*
 * Entry point
 */
int _tmain(int argc, _TCHAR* argv[])
{

  cout << "AMS driver test console" << endl;
  cout << "(c) 2004 Optical Camera Services" << endl << endl;
  cout << "Commands:" << endl;
  cout << "-- open <port>\t\t\t" << "Opens <port> for serial communication" << endl;
  cout << "-- setaxis <axis>\t\t" << "Changes the axis name to <axis>" << endl;
  cout << "-- <command|value>\t\t" << "Sends the command/value pair to the driver" << endl;
  cout << "-- close\t\t\t" << "Ends the communication session" << endl;
  cout << "-- quit/exit\t\t\t" << "Quit the application" << endl << endl;
  cout << "Defaults: Port = COM1, Axis name = 'C'" << endl << endl;

  ANSISTRING prompt;

  while (("quit" != prompt) && ("exit" != prompt) &&
         ("QUIT" != prompt) && ("EXIT" != prompt)) {

    Route(prompt = Prompt("AMS>"));

  }

  cout << "Exiting." << endl;

	return 0;
}

ANSISTRING Prompt(const ANSISTRING &prompt) {

  char ret[256] = {0};

  cout << prompt;
  cin.getline(ret, 256);

  return ret;
}

void Route(const ANSISTRING &strCmd) {

  bool bFound = false;

  ANSISTRING strUcaseCmd = CmdFromCmdLine(strCmd);

  StrToUpper(strUcaseCmd);

  for (int n = 0; n < g_NumFns; n++) {

    if (g_Table[n].strCommand == strUcaseCmd) {

      g_Table[n].pfnRoute(strCmd);
      bFound = true;
      break;

    }

  }

  if (!bFound) {

    DoCommand(strCmd);

  }

}

ANSISTRING ArgFromCmdLine(const ANSISTRING &strCmd) {

  ANSISTRING::size_type stPos = strCmd.find(' ');

  if (ANSISTRING::npos != stPos) {

    return strCmd.substr(stPos + 1);

  } else {

    return "";

  }

}

ANSISTRING CmdFromCmdLine(const ANSISTRING &strCmd) {

  ANSISTRING::size_type stPos = strCmd.find(' ');

  if (ANSISTRING::npos == stPos) {

    return strCmd;

  } else {

    return strCmd.substr(0, stPos);

  }

}

/*
 * Command handlers
 */

void DoOpen(const ANSISTRING &strCmd) {

  if (!g_bOpen) {

    ANSISTRING strArg = ArgFromCmdLine(strCmd);

    if (0 == strArg.size()) {

      cout << "No port given; using default (COM1)" << endl;
   
    } else {

      g_strPort = strArg;

    }

    cout << "Attempting to open serial port ";
    cout << "[" << g_strPort << "] ... " << endl;

    if (!g_Comm.Open(g_strPort)) {

      cout << "Couldn't open port [" << g_strPort;
      cout << "] for serial communication!" << endl;

    } else {

      g_bOpen = true;

      cout << "Successfully opened port [" << g_strPort;
      cout << "] for serial communication" << endl;

    }

  } else {

    cout << "Port [" << g_strPort << "]";
    cout << " is already open; use 'close'" << endl;

  }

}

void DoClose(const ANSISTRING &strCmd) {

  if (g_bOpen) {

    if (!g_Comm.Close()) {

      cout << "Error closing serial connection!" << endl;

    } else {

      g_bOpen = false;

      cout << "Successfully closed serial connection" << endl;

    }

  } else {

    cout << "No port currently open." << endl;

  }

}

void DoSetAxis(const ANSISTRING &strCmd) {

  ANSISTRING strArg = ArgFromCmdLine(strCmd);

  if (0 == strArg.size()) {

    cout << "No axis name supplied." << endl;

  } else if (1 != strArg.size()) {

    cout << "Incorrect format; axis name is a single character." << endl;

  } else {

    g_cAxis = toupper(*strArg.c_str());

    cout << "Changed axis name to '" << g_cAxis << "'" << endl;

  }

}

void DoCommand(const ANSISTRING &strCmd) {

  
  if (g_bOpen) {

    ANSISTRING strResult;

    cout << "Writing '" << strCmd << "'";
    cout << " to axis '" << g_cAxis << "' on";
    cout << " serial port [" << g_strPort << "] ..." << endl;

    switch (g_Comm.Write(g_cAxis, const_cast<ANSISTRING &>(strCmd), strResult)) {

      case AMS_OK:

        cout << "------------------" << endl;
        cout << "Result string = '" << strResult << "'" << endl;
        cout << "------------------" << endl;

      break;
      case AMS_TXTIMEOUT:

        cout << "Transmission of data to driver";
        cout << " has failed; check serial connection";
        cout << " and driver." << endl;

      break;
      case AMS_RXTIMEOUT:

        cout << "Reciept of data from driver has failed";
        cout << " multiple times. Gave up!";
        cout << " check serial connection and driver." << endl;

      break;
      case AMS_ERROR:

        cout << "An error has occurred!" << endl;

      break;
     
    }

  } else {

    cout << "No serial port open; use 'open'" << endl;

  }

}

void Callback(CTSTRING &ts, DWORD dw) {

  cout << ts << endl;

}
