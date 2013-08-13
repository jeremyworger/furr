// ParallelTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\..\furr\furrcappingshutter.h"

using namespace std;

string Prompt(void);

int _tmain(int argc, _TCHAR* argv[])
{

  cout << "Capping shutter test application" << endl;

  FURRCappingShutter Shutter;
  string prompt;

  while (1) {

    prompt = Prompt();

    if (("h" == prompt) || ("H" == prompt)) {

      cout << "Setting pin to HIGH" << endl;

      Shutter.Uncap();

    } else if (("l" == prompt) || ("L" == prompt)) {

      cout << "Setting pin to LOW" << endl;

      Shutter.Cap();

    } else if (("q" == prompt) || ("Q" == prompt)) {

      return 0;

    } else {

      cout << "What? 'L' = Low, 'H' = High, 'Q' = Quit" << endl;

    }

    Sleep(0);

  }

	return 0;
}

string Prompt(void) {

  cout << "CapShutter>";

  char szBuf[256] = {0};

  cin.getline(szBuf, 256);

  return szBuf;
}