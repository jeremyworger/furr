#include "lutconvert.hpp"
#include <iostream>

/*
 * FUNCTION: LutConvert_ReverseLUT()
 *
 * SYNOPSIS: Reverses the order of pixel intensity values
 * in a LUT file, for negative/postive film.
 *
 * ARGUMENTS: Path to the input LUT file, and the path to the
 * file to be written after the conversion.
 *
 * RETURN VALUE: Boolean success value
 *
*/
bool LutConvert_ReverseLUT(const string &strLUT, const string &strLUTOut) {

  InStream in;
  OutStream out;

 /*
  * Open both the source and destination files.
  */
  in.open(strLUT.c_str(), ios_base::binary | ios_base::in);
  out.open(strLUTOut.c_str(), ios_base::binary | ios_base::out);

  if ((!in.is_open())) {
    cout << "Failed to open input LUT file [" << strLUT.c_str() << "]" << endl;
    return false;
  }

  if (!out.is_open()) {
    cout << "Failed to open destination LUT file [" << strLUTOut.c_str() << "]" << endl;
    return false;
  }

 /*
  * Swap RED channel
  */
  _SwapChannelData(in, out);

 /*
  * Swap GREEN channel
  */
  _SwapChannelData(in, out);

 /*
  * Swap BLUE channel
  */
  _SwapChannelData(in, out);

  in.close();
  out.close();

  cout << "[" << strLUT.c_str() << "] was successfully";
  cout << " reversed, and saved as [" << strLUTOut.c_str() << "]" << endl;

  return true;
}

void _SwapChannelData(InStream &in, OutStream &out) {

 /*
  * Files are open. Read LUT_ENTRIES
  * of size LUT_BYTES from the source file,
  * for each channel, then write the data
  * in reverse data to the destination file.
  */
  short sBuffer[LUT_ENTRIES] = {0};

 /*
  * First LUT_ENTRIES * LUT_BYTES bytes from source file
  * read into stack buffer
  */
  int n;

  for (n = 0; n < LUT_ENTRIES; n++) { in.read(&sBuffer[n], 1); }

 /*
  * Write in reverse order to destination file
  */
  for (n = LUT_ENTRIES - 1; n >= 0; n--) { out.write(&sBuffer[n], 1); }

}
