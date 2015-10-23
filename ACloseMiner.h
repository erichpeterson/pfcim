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

#ifndef ACLOSEMINER_H_
#define ACLOSEMINER_H_

#include <vector>
#include <math.h>
#include <iomanip>
#include "Global.h"
#include "Commandline.h"
#include "Database.h"

using namespace std;

//----------------------------------------------------------------------
// Class:
//    Mine
//
// Used to mine the actual database for closed frequent probabilistic
// itemsets---using the A-Close algorithm.
//----------------------------------------------------------------------

class ACloseMiner  {
	unsigned int numObjects;     // A copy of the number of objects in the database
	unsigned int largestItem;	 // A copy of the largest item found in the database
	vector<itemset> L;			 // Temporary storage vector for frequent prob. itemsets, 
								 // that might me closed.
	vector<itemset> Lprime;		 // Stores the current iterations frequent prob. itemsets.
	vector<itemset> C;			 // A copy of the unique itemsets in the database.
	FP ** P;				 // A pointer to P, from a Database object.
	vector<itemset> Cprime;		 // Used during the apriori generation step. A place to
								 // store valid itemset candidates.
	unsigned int numFound;		 // The number of closed prob. freq. itemsets found
    
    double * poiMinsup;
public:
	void aprioriGen(vector<itemset> &, bool);
	unsigned int ExactPoiBiMaxProbSupp(itemset &, vector<itemset>&);
	unsigned int ExactPoiBiMaxProbSupp(itemset &);
    unsigned int PoiApproxMaxProbSupp(itemset &, vector<itemset>&);
    unsigned int PoiApproxMaxProbSupp(itemset & x);
	unsigned int indexPrefixX(vector<itemset> &, itemset&);
	Bitmap * aprioriGenPower(unsigned int);

	ACloseMiner(Database&);
	void StartMining();
	unsigned int getNumFound();

};

#endif // ACLOSEMINER_H_