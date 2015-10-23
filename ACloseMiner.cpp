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

#include "ACloseMiner.h"

//----------------------------------------------------------------------
// Method:
//    Mine::Mine(Database& objDB)
//
// Constructor to initialize a ACloseMiner object.
//
// Arguments:
//   "objDB" is the Database object used to initialize this object.
//----------------------------------------------------------------------
ACloseMiner::ACloseMiner(Database& objDB) {
	numObjects = objDB.getNumObjects();
	largestItem = objDB.getLargestItem();
	C = objDB.getC();
	P = objDB.getP();
	numFound = 0;
    
    if (Commandline::calc == "approx") {
        poiMinsup = new double[numObjects - Commandline::eta + 1];
        for(unsigned int i = 0; i <= numObjects - Commandline::eta; i++) {
            poiMinsup[i] = boost::math::gamma_q_inv(Commandline::eta + i, 1.0 - Commandline::tau);
        }
        /*
        for(unsigned int i = 0; i <= numObjects - Commandline::eta; i++) {
            cout << "poiMinsup[" << i << "]: " << poiMinsup[i] << endl;
        }
         */
    }
}

//----------------------------------------------------------------------
// Method:
//    Mine::StartMining()
//
// Main data mining algorith.
//----------------------------------------------------------------------
void 
ACloseMiner::StartMining() {	
	for(int i = 0; i < C.size(); i++) {
        if (Commandline::calc == "" || Commandline::calc == "approx") {
            C[i].probSupp = PoiApproxMaxProbSupp(C[i]);
        }
        else if(Commandline::calc == "dynamic") {
            C[i].probSupp = ExactPoiBiMaxProbSupp(C[i]);
        }
		if(C[i].probSupp >= Commandline::eta) {
			L.push_back(C[i]);
		}
	}
	
	aprioriGen(L, true);
	
	while(Cprime.size()) {
		for(unsigned int i = 0; i < Cprime.size(); i++) {
			unsigned int MS = 0;
			if (Commandline::calc == "" || Commandline::calc == "approx") {
                MS = PoiApproxMaxProbSupp(Cprime[i]);
            }
            else if(Commandline::calc == "dynamic") {
                MS = ExactPoiBiMaxProbSupp(Cprime[i], L);
            }
            
			if(MS >= Commandline::eta) {
				Cprime[i].probSupp = MS;
				Lprime.push_back(Cprime[i]);
			}
		}
		
		for(unsigned int i = 0; i < L.size(); i++) {
			bool flag = true;
			for (unsigned int j = 0; j < Lprime.size(); j++) {
				if (L[i].probSupp == Lprime[j].probSupp) {
					unsigned int k;
					for(k = 0; k < L[i].items.getNumWords(); k++) {
						if ((~(~(*(L[i].items.getBM() + k)) | 
							   ((*(L[i].items.getBM() + k)) 
								& (*(Lprime[j].items.getBM() + k)))) != 0)) {
								   break;
							   }
					}
					if (k == L[i].items.getNumWords()) {
						flag = false;
					}
				}
				
				if (!flag) {
					break;
				}
			}
			if(flag) {
				numFound ++;
			}
			if(flag && Commandline::print) {
				// Print out L[i] as closed prob. freq. itemset
				for (int p = L[i].items.getFirstNonZero(); p >= 0; --p) {
					if (L[i].items.test(p)) {
						cout << p << " ";
					}
				}
				cout << endl;//": " << L[i].probSupp << endl;
			}
		}
		
		// L <- L'
		L.clear();
		L = Lprime;
		Lprime.clear();
		Cprime.clear();
		aprioriGen(L, false);
	}
	
	numFound += L.size();
	if(Commandline::print) {
		for (unsigned int i = 0; i < L.size(); i++) {
			// Print out L[i] as closed prob. freq. itemset
			for (int p = L[i].items.getFirstNonZero(); p >= 0; --p) {
				if (L[i].items.test(p)) {
					cout << p << " ";
				}
			}
			cout << endl;//": " << L[i].probSupp << endl;
		}
	}
	
}

//----------------------------------------------------------------------
// Method:
//    Mine::ExactPoiBiMaxProbSupp(itemset & x, vector<itemset>& L)
//
// Used to calculate the maximum support of a given itemset x. As a side
// effect, it will set the cummulative probability values of x being 
// frequent in j transactions.
//
// Arguments:
//   "x" is the itemset of length k, which needs to have its maximum 
//       support calculated.
//   "L" is a reference to the frequent prob. itemsets of length k - 1.
//       This variable contains the cummulated probability values for
//       each k - 1 itemset.
//
// Return values:
//    The probibilistic support of the itemset x.
//----------------------------------------------------------------------
unsigned int
ACloseMiner::ExactPoiBiMaxProbSupp(itemset & x, vector<itemset>& L) {
	FP * matrixA = new FP[numObjects + 1];
	FP * matrixB;
	
	unsigned int indexPrefix = indexPrefixX(L, x);
	
	unsigned int i;
	for(i = 0; i <= numObjects; i++) {
		if(i != 0) {
			matrixB = new FP[numObjects + 1];
		}
		unsigned int j;
		FP probPostXInTj;
		FP probPreXInTj;
		
		for(j = i; j <= numObjects - Commandline::eta + i && j <= numObjects; j++) {
			if(i == 0) {
				matrixA[j] = 1.0;
				continue;
			}
			if(j == i) {
				matrixB[j - 1] = 0.0;
			}
			
			
			probPreXInTj = L[indexPrefix].P[j - 1];
			probPostXInTj = P[x.items.getFirstNonZero()][j - 1];
			
			
			matrixB[j] = (matrixA[j - 1] * probPreXInTj * probPostXInTj) + 
			   (matrixB[j - 1] * (1 - (probPreXInTj * probPostXInTj)));
			
			if(i == 1 || j == numObjects - Commandline::eta + i) {
				x.P[j - 1] = probPreXInTj * probPostXInTj;
			}
		}
		
		if(i != 0) {
			delete[] matrixA;
			matrixA = matrixB;
		}
		
		if(matrixA[j - 1] < Commandline::tau && i != 0) {
			delete[] matrixA;
			return i - 1;
		}
	}
	delete[] matrixA;
	return i - 1;
}


//----------------------------------------------------------------------
// Method:
//    Mine::ExactPoiBiMaxProbSupp(itemset & x)
//
// Used to calculate the maximum support of a given itemset x.
//
// Arguments:
//   "x" is the itemset of length k, which needs to have its maximum 
//       support calculated.
//
// Return values:
//    The probabilisitc support of the itemset x.
//----------------------------------------------------------------------
unsigned int
ACloseMiner::ExactPoiBiMaxProbSupp(itemset & x) {
	FP * matrixA = new FP[numObjects + 1];
	FP * matrixB;
	
	unsigned int i;
	for(i = 0; i <= numObjects; i++) {
		if(i != 0) {
			matrixB = new FP[numObjects + 1];
		}
		unsigned int j;
		FP probXInTj;
		for(j = i; j <= numObjects - Commandline::eta + i && j <= numObjects; j++) {
			if(i == 0) {
				matrixA[j] = 1.0;
				continue;
			}
			if(j == i) {
				matrixB[j - 1] = 0.0;
			}
			
			probXInTj = P[x.items.getFirstNonZero()][j - 1];
			
			matrixB[j] = (matrixA[j - 1] * probXInTj) + 
			   (matrixB[j - 1] * (1 - probXInTj));
		}
		
		if(i != 0) {
			delete[] matrixA;
			matrixA = matrixB;
		}
		
		if(matrixA[j - 1] < Commandline::tau && i != 0) {
			delete[] matrixA;
			return i - 1;
		}
	}
	delete[] matrixA;
	return i - 1;
}

//----------------------------------------------------------------------
// Method:
//    Mine::indexPrefixX(vector<itemset> & L, itemset& x)
//
// Calculates the index in the vector L, where the prefix of x can be found.
// That is the x minus the bit set at the bitmaps firstNonZero.
//
// Return value:
//    The aforementioned index.
//----------------------------------------------------------------------
unsigned int
ACloseMiner::indexPrefixX(vector<itemset> & L, itemset& x) {
	int i = 0;
	bool flag;
	
	Bitmap tempBitmap(largestItem + 1);
	
	for(unsigned int i = 0; i < tempBitmap.getNumBits(); i++) {
		if(x.items.test(i)) {
			tempBitmap.markBit(i);
		}
	}
	
	tempBitmap.clear(x.items.getFirstNonZero());
	
	for(; i < L.size(); i++) {
		flag = true;
		for(int k = 0; k < L[i].items.getNumWords(); k++) {
			if(*(L[i].items.getBM() + k) != *(tempBitmap.getBM() + k)) {
				flag = false;
				break;
			}
		}
		if(flag) {
			break;
		}
	}
	
	return i;
}

//----------------------------------------------------------------------
// Method:
//    Mine::aprioriGen(vector<itemset> & L, bool isFirstCall)
//
// Implements the apriori algorithm. Given a vector L of previously found
// frequent prob. itemset of size k, it will generate candidates of length
// k + 1 and place them in Cprime.
//----------------------------------------------------------------------
void 
ACloseMiner::aprioriGen(vector<itemset> & L, bool isFirstCall) {
	
	for(unsigned int i = 0; i < L.size(); i++) {
		unsigned int firstNonZero_i = L[i].items.getFirstNonZero();
		for(unsigned int j = i + 1; j < L.size(); j++) {	
			unsigned int firstNonZero_j = L[j].items.getFirstNonZero();
			unsigned int minimum_i_j = min(firstNonZero_i, firstNonZero_j);
			
			Bitmap * L_power_min_minus_1 = aprioriGenPower(minimum_i_j);
			
			Bitmap L_i_And_Mask(largestItem + 1);
			Bitmap L_j_And_Mask(largestItem + 1);
			
			for (unsigned int k = 0; k < L_i_And_Mask.getNumWords(); k++) {
				L_i_And_Mask.getBM()[k] = L[i].items.getBM()[k] & L_power_min_minus_1->getBM()[k];
				L_j_And_Mask.getBM()[k] = L[j].items.getBM()[k] & L_power_min_minus_1->getBM()[k];
			}
			
			Bitmap L_ij_Mask_OR(largestItem + 1);
			
			for (unsigned int k = 0; k < L_i_And_Mask.getNumWords(); k++) {
				L_ij_Mask_OR.getBM()[k] = L_i_And_Mask.getBM()[k] ^ L_j_And_Mask.getBM()[k];
			}
			
			unsigned int count = L_ij_Mask_OR.getNumSet();
			
			if(count == 0) {
				itemset tempItemset;
				tempItemset.P = new FP[numObjects];
				Bitmap tempBitmap(largestItem + 1);
				
				if(!isFirstCall) {
					for(unsigned int k = 0; k < L_i_And_Mask.getNumWords(); k++) {
						tempBitmap.getBM()[k] = L_i_And_Mask.getBM()[k];
					}
				}
				tempBitmap.markBit(firstNonZero_i);
				tempBitmap.markBit(firstNonZero_j);
				tempItemset.items = tempBitmap;
				
				Cprime.push_back(tempItemset);
			}
			delete[] L_power_min_minus_1;
		}	
	}
	
	// prune
	
	vector<unsigned int> prunedIndexes;
	bool flag1;
	bool flag2;
	bool flag3;
	
	for(unsigned int i = 0; i < Cprime.size(); i++) {
		for(int j = Cprime[i].items.getFirstNonZero(); j >= 0; j--) {
			flag1 = false;
			Bitmap * power = new Bitmap(largestItem + 1);
			power->markBit(j);
			
			Bitmap * negationPower = new Bitmap(power->getNumBits());
			
			for (unsigned int p = 0; p < power->getNumWords(); p++) {
				negationPower->getBM()[p] = ~power->getBM()[p];
			}
			
			Bitmap * Cprime_And_Mask = new Bitmap(power->getNumBits());
			for(unsigned int p = 0; p < power->getNumWords(); p++) {
				Cprime_And_Mask->getBM()[p] = Cprime[i].items.getBM()[p] & negationPower->getBM()[p];
			}
			
			flag2 = true;
			for(unsigned int k = 0; k < Cprime_And_Mask->getNumWords(); k++) {
				if(Cprime_And_Mask->getBM()[k] != Cprime[i].items.getBM()[k]) {
					flag2 = false;
					break;
				}
			}
			
			if (!flag2) {
				for (unsigned int p = 0; p < L.size(); p++) {
					flag3 = true;
					for (unsigned int k = 0; k < L[p].items.getNumWords(); k++) {
						if(L[p].items.getBM()[k] != Cprime_And_Mask->getBM()[k]) {
							flag3 = false;
							break;
						}
					}
					if (flag3) {
						break;
					}
				}
				if (flag3) {
					flag1 = true;
					break;
				}
				
			}
			else {
				continue;
			}
			
			if(!flag1) {
				prunedIndexes.push_back(i);
				break;
			}
			
			delete[] Cprime_And_Mask;
			delete[] power;
			delete[] negationPower;
		}
	}
	
	unsigned int count = 0;
	for(unsigned int i = 0; i < prunedIndexes.size(); i++) {
		Cprime.erase(Cprime.begin() + prunedIndexes[i] - count);
		count++;
	}
}

unsigned int
ACloseMiner::PoiApproxMaxProbSupp(itemset & x) {
#ifdef HAVE_ARPREC_MP_REAL_H 
#ifdef HAVE_LIBARPREC
    mp_real sum("0");
#endif
#else
    double sum = 0;
#endif

    for(unsigned int j = 0; j < numObjects; j++) {
        FP probX = 1;

        for(int p = x.items.getFirstNonZero(); p >= 0; --p) {
            if (x.items.test(p)) {
                probX *= P[p][j];
            }
        }
        sum += probX;
        /*
        if (j + 1 > numObjects + 1 - floor(poiMinsup[0])) {
            //cout << poiMinsup[0] - (j + 1) << endl;
            if (sum < poiMinsup[0] - (j + 1 - numObjects + 1)) {
                return 0;
            }
        }
         */
    }
    
    if (sum < poiMinsup[0]) {
        return 0;
    }
    
    //cout << numObjects - Commandline::eta << endl;
    
    //cout << poiMinsup[957] << endl;
    
    for (int j = numObjects - Commandline::eta; j >= 0 ; j--) {
        if (poiMinsup[j] <= sum) {
            return j + Commandline::eta;
        }
    }
    
    return 0;
}


//----------------------------------------------------------------------
// Method:
//    Mine::aprioriGenPower(unsigned int power)
//
// Creates a bitmap and sets it to the value of 2^power - 1.
//
// Arguments:
//    "power" the power to raise 2 to.
//
// Return value:
//    A pointer to a Bitmap object marked with 2^power - 1.
//----------------------------------------------------------------------
Bitmap *
ACloseMiner::aprioriGenPower(unsigned int power) {
	Bitmap * bm = new Bitmap(largestItem + 1);
	
	for (unsigned int i = 0; i < power; i++) {
		bm->markBit(i);
	}
	return bm;
}

unsigned int
ACloseMiner::getNumFound() {
	return numFound;
}
