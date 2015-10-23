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

#include "Bitmap.h"

//----------------------------------------------------------------------
// Method: 
//    Bitmap::Bitmap()
// 
// Default constructor
//----------------------------------------------------------------------
Bitmap::Bitmap() {}

//----------------------------------------------------------------------
// Method: 
//    Bitmap::Bitmap(unsigned int numItems)
// 
// Constructor which initializes the Bitmap object, by setting the
// proper number of words and bits used in the bitmap (BM), initializing the
// bitmap to all zeros, and setting the firstNonZero variable to zero.
//
// Arguments:
//    "numItems" is the number of items that need representing in the
//               Bitmap object.
//----------------------------------------------------------------------
Bitmap::Bitmap(unsigned int numItems) {
	numWords = DivRoundUp(numItems, bitsInWord);
	numBits = numWords * bitsInWord;
    BM = new unsigned int[numWords];
	for (unsigned int i = 0; i < numBits; i++) 
        clear(i);
	firstNonZero = 0;
}

//----------------------------------------------------------------------
// Method: 
//    Bitmap::Clear(unsigned int which)
// 
// Clears the "nth" bit in a bitmap (zero based).
//
// Arguments:
//   "which" is the number of the bit to be cleared.
//----------------------------------------------------------------------
void
Bitmap::clear(unsigned int which) {
	if (!(which >= 0 && which < numBits)) {
		Util::errorMsg("Could not clear bitmap (argument out-of-range).");
		exit(1);
	}
    BM[which / bitsInWord] &= ~(1 << (which % bitsInWord));
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::MarkBit(unsigned int which)
//
// Marks the "nth" bit in a bitmap (zero based).
//
// Arguments:
//	  "which" is the number of the bit to be set.
//----------------------------------------------------------------------

void
Bitmap::markBit(unsigned int which) { 
	if(!(which >= 0 && which < numBits)) {
		Util::errorMsg("Could not mark bit (argument out-of-range).");
		exit(1);
	}
	BM[which / bitsInWord] |= 1 << (which % bitsInWord);
	
	for(int j = numBits - 1; j >= 0; j--) {
		if(test(j)) {
			firstNonZero = j;
			break;	
		}   	
    } 
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::Test(unsigned int which)
//
// Returns true if the "nth" bit is set (zero based).
//
// Arguments:
//	   "which" is the number of the bit to be tested.
//
// Return values:
//    true - if the "nith" bit is set.
//    false - if the "nth" bit is not set.
//----------------------------------------------------------------------
bool 
Bitmap::test(unsigned int which) {
	if(!(which >= 0 && which < numBits)) {
		Util::errorMsg("Could not test bit (argument out-of-range)");
	}
    if (BM[which / bitsInWord] & (1 << (which % bitsInWord)))
		return 1;
    else
		return 0;
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::getBM()
//
// Return values:
//    Returns BM (the bitmap).
//----------------------------------------------------------------------
unsigned int * 
Bitmap::getBM() {
	return BM;
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::getNumBits()
//
// Return values:
//    Returns the number of bits in the bitmap.
//----------------------------------------------------------------------
unsigned int
Bitmap::getNumBits() {
	return numBits;
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::getNumWords()
//
// Return values:
//    Returns the number of words in the bitmap.
//----------------------------------------------------------------------
unsigned int
Bitmap::getNumWords() {
	return numWords;
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::getFirstNonZero()
//
// Return values:
//    Returns index of the first non zero bit (zero based, from right
//    to left) from
//    from the left to the right.
//----------------------------------------------------------------------
unsigned int
Bitmap::getFirstNonZero() {
	return firstNonZero;
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::getBitsInWord()
//
// Return values:
//    The number of bits in a word.
//----------------------------------------------------------------------
unsigned int
Bitmap::getBitsInWord() {
	return bitsInWord;
}

//----------------------------------------------------------------------
// Method:
//    Bitmap::getNumSet()
//
// Return values:
//    The number of bits marked in the bitmap.
//----------------------------------------------------------------------
unsigned int
Bitmap::getNumSet() {
	unsigned int count = 0;
	
	for(unsigned int j = 0; j < numBits; j++) {
		if(test(j)) {
			count++;
		}
	}
	
	return count;
}


