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


#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//----------------------------------------------------------------------
// Class: 
//    Util
// 
// Contains utility methods.
//----------------------------------------------------------------------

class Util {
public:
	static void errorMsg(string);
	static void stringSplit(string, string, vector<string>&);
};

#endif /*UTIL_H_*/
