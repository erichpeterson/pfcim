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

#include "Commandline.h"

// Set static variables
double Commandline::tau = -1;
unsigned int Commandline::eta = -1;
string Commandline::output;
string Commandline::input;
string Commandline::exec;
unsigned int Commandline::print = 0;

//----------------------------------------------------------------------
// Method 
//    Commandline::commandline()
//
// Used to print out legal command line arguments to screen. 
//----------------------------------------------------------------------
void 
Commandline::commandLine() {
	cerr << "Command Line Options:\n";
	cerr << "  -eta Eta min support: [1-Size of DB] (Required)\n";
	cerr << "  -tau Tau frequent probability threshold: (0-1] (Required)\n";
	cerr << "  -input Input file name (Required)\n";
	cerr << "  -output Output file name default: none\n";
	cerr << "  -exec Execution stats file name default: none\n";
	cerr << "  -print Print found clusters: {0 | 1} (0 = false, 1 = true) default 0\n";
}


//----------------------------------------------------------------------
// Method: 
//    Commandline::getArgs(int argc, char ** const argv)
// 
// Parses through the provided arguments and validates the values
//
// Arguments:
//    "argc" is the number of arguments passed at the command line.
//    "argv" is the char array of strings containing the actual command
//        line argument.
//
// Return value:
//    0 - Success
//    -1 - Failure
//----------------------------------------------------------------------
int 
Commandline::getArgs(int argc, char ** const argv) {
	int argPos = 1;
	
	while (argPos < argc) {
		if (strcmp(argv[argPos], "-tau") == 0) {
			tau = atof(argv[++argPos]);
			argPos++;
			if (tau < 0.0 || tau > 1.0) {
				Util::errorMsg("tau must be >= 0 and <= 1\n");
				return -1;
			}
			continue;
		}
		else if (strcmp(argv[argPos], "-eta") == 0) {
			eta = (unsigned int)atoi(argv[++argPos]);
			argPos++;
			if (eta <= 0) {
				Util::errorMsg("eta must be > 0\n");
				return -1;
			}
			continue;
		}
		else if (strcmp(argv[argPos], "-input") == 0) {
			input = argv[++argPos];
			argPos++;
			continue;
		}
		else if (strcmp(argv[argPos], "-output") == 0) {
			output = argv[++argPos];
			argPos++;
			continue;
		}
		else if (strcmp(argv[argPos], "-exec") == 0) {
			exec = argv[++argPos];
			argPos++;
			continue;
		}
		else if (strcmp(argv[argPos], "-print") == 0) {
			print = atoi(argv[++argPos]);
			argPos++;
			continue;
		}
		else if (strcmp(argv[argPos], "-calc") == 0) {
			calc = argv[++argPos];
			argPos++;
			continue;
		}
		else {
			commandLine();
			return -1;
		}
	}  // end while
	
	if (input == "") {
		Util::errorMsg("-input is required\n");
		return -1;
	}
    if (eta == -1) {
        Util::errorMsg("-eta must be >= 1\n"); 
        commandLine();
        return -1;
    }
	if (tau == -1) {
        Util::errorMsg("-tau must be between 0 and 1\n"); 
        commandLine();
        return -1;
	}
	
	return 0;
}

