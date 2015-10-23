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

#include "Database.h"

//----------------------------------------------------------------------
// Method:
//    Database::readDBFromFile()
//
// Used to read in contents of input file into an in memory representation
// of the database (P). It also populates C, a vector containing a
// list of unique singleton items.
//----------------------------------------------------------------------
void
Database::readDBFromFile() {
	string line;
	string item;
	largestItem = 0;
	
	unsigned int objCount = 0;

	ifstream inputFile(Commandline::input.c_str());
	
	if(!inputFile.is_open()) {
		Util::errorMsg("Could not open the input file.");
		exit(1);
	}
	
	while(!inputFile.eof()) {
		getline(inputFile, line);
		objCount++;
		
		istringstream ss(line);
		
		while (ss.good()) {
			ss >> item;
			
			if((unsigned int)atoi(item.c_str()) > largestItem) {
				largestItem = (unsigned int)atoi(item.c_str());
			}
		}
	}
	
	numObjects = objCount;
	
    //cout << numObjects << endl;
    
	inputFile.clear();
	inputFile.seekg(0, ios::beg);
	
	bool* seenItem = new bool[largestItem + 1];
	
	for(unsigned int i = 0; i < largestItem + 1; i++) {
		seenItem[i] = false;
	}
	
	P = new FP*[largestItem + 1];
	
	for(int i = 0; i < numObjects && !inputFile.eof(); i++) {
		getline(inputFile, line);
		istringstream ss(line);
		
		while(ss.good()) {
			vector<string> splitResults;
			ss >> item;
			
			Util::stringSplit(item, ":", splitResults);
			
			if(!seenItem[(unsigned int)atoi(splitResults[0].c_str())]) {
				
				seenItem[(unsigned int)atoi(splitResults[0].c_str())] = true;				
			
				P[(unsigned int)atoi(splitResults[0].c_str())] = new FP[numObjects];
				
				for (unsigned int j = 0; j < numObjects; j++) {
					P[(unsigned int)atoi(splitResults[0].c_str())][j] = 0;
				}
			}

			
			P[(unsigned int)atoi(splitResults[0].c_str())][i] = atof(splitResults[1].c_str());
		}
	}
	
	for(unsigned int i = 0; i < largestItem + 1; i++) {
		if(seenItem[i]) {
			itemset tempItemset;
			tempItemset.P = new FP[numObjects];
				
			for(unsigned int j = 0; j < numObjects; j++) {
				tempItemset.P[j] = P[i][j];
			}
			
			Bitmap tempBitmap(largestItem + 1);
			tempBitmap.markBit(i);
			tempItemset.items = tempBitmap;
			C.push_back(tempItemset);
		}
	}
	
	delete[] seenItem;
}

//----------------------------------------------------------------------
// Method:
//    Database::getNumObjects()
//
// Returns the number of objects in the database.   
//----------------------------------------------------------------------

unsigned int 
Database::getNumObjects() {
	return numObjects;
}

//----------------------------------------------------------------------
// Method:
//    Database::getC()
//
// Returns a reference to the vector C.   
//----------------------------------------------------------------------
vector<itemset>&
Database::getC() {
	return C;
}

//----------------------------------------------------------------------
// Method:
//    Database::getP()
//
// Returns a reference to the two-dimensional array P.   
//----------------------------------------------------------------------
FP **
Database::getP() {
	return P;
}

//----------------------------------------------------------------------
// Method:
//    Database::getLargestItem()
//
// Returns the largest item seen in the database.   
//----------------------------------------------------------------------
unsigned int
Database::getLargestItem() {
	return largestItem;
}