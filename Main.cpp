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
 
#include <iostream>
#include <sys/resource.h>

#include "Commandline.h"
#include "Database.h"
#include "ACloseMiner.h"

using namespace std;


//----------------------------------------------------------------------
// Function 
//    main(int argc, char * argv[])
//
// Main execution starting point.
//
// Arguments:
//    "argc" is the number of arguments passed at the command line.
//    "argv" is the char array of strings containing the actual command
//           line argument.
//----------------------------------------------------------------------

int main (int argc, char * argv[]) { 
	// Timing variables
	float  UserTime, SysTime;
	
	struct rusage MyTime1, MyTime2;
	
	// Start timing for building the tree
	getrusage(RUSAGE_SELF,&MyTime1);
#ifdef HAVE_ARPREC_MP_REAL_H 
#ifdef HAVE_LIBARPREC
    mp::mp_init(numDigitsPrecision1 + 2); 
    mp::mpsetprec(numDigitsPrecision1 ); 
    mp::mpsetoutputprec(numDigitsPrecision1 ); 
    cout.precision(numDigitsPrecision1 ) ; 
    
#endif
#endif
	// Read in command line arguments
	if (Commandline::getArgs(argc, argv) != 0) {
		exit(1);
	}
	
	// Create Database object and read dataase in file into memory
	Database objDB;
	objDB.readDBFromFile();
	
	// Create IMiner object and pass it the Databases object containing 
	// the database
	
	ACloseMiner* minerObj = new ACloseMiner(objDB);
	
	// Call the main algorithm (which does the actual mining)
	minerObj->StartMining();
	
	// End timing 
	getrusage(RUSAGE_SELF, &MyTime2);
	
	UserTime =
	((float) (MyTime2.ru_utime.tv_sec  - MyTime1.ru_utime.tv_sec)) +
	((float) (MyTime2.ru_utime.tv_usec - MyTime1.ru_utime.tv_usec)) * 1e-6;
	SysTime =
	((float) (MyTime2.ru_stime.tv_sec  - MyTime1.ru_stime.tv_sec)) +
	((float) (MyTime2.ru_stime.tv_usec - MyTime1.ru_stime.tv_usec)) * 1e-6;
	
	//printf("Total time : %f seconds\n", UserTime + SysTime);
	
	//cout << endl << "Num. itemsets found: " << minerObj->getNumFound() << endl;
	
	if(Commandline::exec != "") {
		ofstream execFile;
		execFile.open(Commandline::exec.c_str(), ios::app);
		
		if(!execFile.is_open()) {
			Util::errorMsg("Could not open the exec file.");
			exit(1);
		}
		
		execFile << UserTime + SysTime << "\t";
	}
#ifdef HAVE_ARPREC_MP_REAL_H 
#ifdef HAVE_LIBARPREC
    mp::mp_finalize();
#endif
#endif
    return 0;
}
