#ifndef _INC_STRUTILS_
#define _INC_STRUTILS_

#include <string>
#include <vector>
using std::string;

int SplitString(const string& str, std::vector<string>& ret_, string sep = ",");

#endif