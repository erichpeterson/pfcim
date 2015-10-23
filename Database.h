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

#ifndef DATABASE_H_
#define DATABASE_H_

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Commandline.h"
#include "Bitmap.h"
#include "Util.h"
#include "Global.h"

using namespace std;

//----------------------------------------------------------------------
// Class:
//    Database
// 
// Used to represnet the database in memory
//----------------------------------------------------------------------

class Database {
public:
	vector<itemset> C;		    // Unique items in the database
	FP ** P;				// Probabilities for singletons
	unsigned int numObjects;    // Number of objects in the database
	unsigned int largestItem;   // Largest item found in the database


	void readDBFromFile();
	unsigned int getNumObjects();
	unsigned int getLargestItem();
	vector<itemset>& getC();
	FP ** getP();
	unsigned int maxSupp(itemset&, vector<itemset>&);
	unsigned int maxSupp(itemset&);
	unsigned int indexPrefixX(vector<itemset>&, itemset &x);
	
};

#endif /*DATABASE_H_*/
