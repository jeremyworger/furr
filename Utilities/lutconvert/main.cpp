#include "main.hpp"
#include "lutconvert.hpp"

int main(int argc, char *argv[]) {

  if ((4 < argc) || (0 == argv[1]) || (0 == argv[2]) || (0 == argv[3])) {

    PrintUsage();
    return 1;

  }

  if (0 == strcmp("-r", argv[1])) {

    if (!LutConvert_ReverseLUT(argv[2], argv[3])) {

      cout << "Failed to convert LUT file [";
      cout << argv[2] << "]." << endl;
      return 1;

    } else {

      return 0;

    }

  }
  
  PrintUsage();

  return 0;
}

void PrintUsage(void) {

  cout << "LUT file converter v1.0.0" << endl;
  cout << "Copyright (c) 2004, Optical Camera Services, Inc." << endl;
  cout << endl;
  cout << "lutconvert -r [infile] [outfile]" << endl;

}
