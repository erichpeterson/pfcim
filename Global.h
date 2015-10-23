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

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define numDigitsPrecision1 900

#include <map>
#ifdef HAVE_ARPREC_MP_REAL_H 
#ifdef HAVE_LIBARPREC
#include <arprec/mp_real.h>
#endif
#endif
#include <boost/math/special_functions/gamma.hpp>
#include "Bitmap.h"

using namespace std;

#ifdef HAVE_ARPREC_MP_REAL_H 
#ifdef HAVE_LIBARPREC
typedef mp_real FP;
#endif
#else
typedef double FP;
#endif

// Structure used to hold each bitmap (representing an itemset),
// the cumulative probability of that itemset being frequent in j
// transactions, and its maximum support.
struct itemsetStruct {
	Bitmap items;
	FP * P;
	unsigned int probSupp;
};

typedef struct itemsetStruct itemset;

#endif // GLOBAL_H_