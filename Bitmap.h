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

#ifndef BITMAP_H_
#define BITMAP_H_

#include <cstdlib>
#include "Util.h"

using namespace std;

// Divide and either round up or down 
#define DivRoundDown(n,s)  ((n) / (s))
#define DivRoundUp(n,s)    (((n) / (s)) + ((((n) % (s)) > 0) ? 1 : 0))

//----------------------------------------------------------------------
// Class: 
//    Bitmap
// 
// Contains the instance variables nesseccary for 
// representing a bitmap vector in memory and the methods for manipulating
// it.
//----------------------------------------------------------------------
class Bitmap {
public:
	Bitmap();					
	Bitmap(unsigned int);	 
	
	unsigned int getNumBits();
	unsigned int getNumWords();
	static unsigned int getBitsInWord();
	bool test(unsigned int);   	
	void markBit(unsigned int);     
	void clear(unsigned int);  	
	
	unsigned int * getBM();
	unsigned int getFirstNonZero();
	void markUInt(unsigned int);
	unsigned int getNumSet();
	unsigned int firstNonZero;
private:
	// Definitions helpful for representing a bitmap as an array of integers
	static const unsigned int bitsInByte = 8;
	static const unsigned int bitsInWord = 32;
	
	unsigned int numBits;		// number of bits in the bitmap
    unsigned int numWords;		// number of words of bitmap storage
								// (rounded up if NumBits is not a
								//  multiple of the number of bits in
								//  a word)
	
	
	unsigned * BM;
};

#endif /*BITMAP_H_*/
