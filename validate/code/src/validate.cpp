//Validate the solution of self-driving-rides algorithm 
//input/output 
#include <iostream>
//exit()
#include <stdlib.h> 
//structures
#include "../include/errors_h.h"
#include "../include/ride_h.h"
#include "../include/data_set_h.h"
#include "../include/file_names_h.h"

using namespace std;


int main(int argc, char* argv[]) {
	if(argc == 1){
		cerr <<"ERROR \nUsage: validate <filename>\n";
		exit(EXIT_FAILURE);
	}

	//get the names of the files from the arguments of the call
	file_names files(argv[1]);

	//pass the file names to the data set
	data_set d1(files);

	d1.verify();

	char input;
	cout <<"\nWould you like to see a more detailed description of errors? ";
	cin >>input;
	if(i=='y'){
		d1.error_check.see_errors();
	}

	//print_map(d1.fleets);
	return 0;
}
