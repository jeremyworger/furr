#ifndef _LUTCONVERT_H
#define _LUTCONVERT_H

#include <fstream>
#include <ios>

using namespace std;

/*
 * Constants
 */
#define LUT_ENTRIES 4096  // Number of pixel values per channel
#define LUT_BYTES 2       // Number of bytes per pixel value

/*
 * Input/Output stream types
 */
typedef basic_ifstream< short, char_traits<short> > InStream;
typedef basic_ofstream< short, char_traits<short> > OutStream;

/*
 * Function definitions
 */
bool LutConvert_ReverseLUT(const string &strLUT, const string &strLUTOut);

/*
 * Internal function definitions
 */
void _SwapChannelData(InStream &in, OutStream &out);

#endif // !_LUTCONVERT_H
