/* 
 
 Copyright 2010, 2011, 2012 Erich A. Peterson
 
 This file is part of PFCIM.
 
 PFCIM is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 PFCIM is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with PFCIM.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#include "Util.h"

//----------------------------------------------------------------------
// Method: 
//    Util::errorMsg(string msg)
// 
// Outputs error messages to the standard error output stream.
//
// Arguments:
//    "msg" is the message to be outputted.
//----------------------------------------------------------------------
void 
Util::errorMsg(string msg) {
	cerr << msg << endl;
}

//----------------------------------------------------------------------
// Method: 
//    Util::stringSplit(string str, string delim, vector<string>& results)
// 
// Splits a string into pieces, based on the desired delimiter, and
// places the resulting pieces in a vector.
//
// Arguments:
//    "str" is the string to be split.
//    "delim" is the delimiter used to determine on what to split the
//            string.
//    "results" is the vector into which the resuls are placed after
//            after the split.
//----------------------------------------------------------------------
void 
Util::stringSplit(string str, string delim, vector<string>& results) {
	int cutAt;
	while( (cutAt = str.find_first_of(delim)) != str.npos )
	{
		if(cutAt > 0)
		{
			results.push_back(str.substr(0,cutAt));
		}
		str = str.substr(cutAt+1);
	}
	if(str.length() > 0)
	{
		results.push_back(str);
	}
}