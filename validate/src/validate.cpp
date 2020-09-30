//Validate the solution of self-driving-rides algorithm 
#include <iostream> //input/output 
#include <stdlib.h> //exit()

//structures
#include "../include/errors_h.h"
#include "../include/ride_h.h"
#include "../include/data_set_h.h"
#include "../include/file_names_h.h"
#include "../include/imported_functions_h.h"
#include "../include/global_variables_h.h"
using namespace std;

int main(int argc, char* argv[]) {
	if(argc == 1){
		cout <<"ERROR \nUsage: validate <filename>\n";
		exit(EXIT_FAILURE);
	}
	if(argc > 2){
		debug = true;
	}

	cout<<"Instance name: " << argv[1]<<endl;
	
	file_names files(argv[1]);
	data_set d1(files);

	d1.verify();
	d1.error_check.print_summary();

	char input;
	cout <<"\nWould you like to see a more detailed description of errors? ";
	cin >>input;
	if(input=='y'){
		d1.error_check.print_all_errors();
	}

	Solution solution1(files);
	int  score_1= get_score(d1, solution1.fleets);
	cout <<"\nscore_1: "<<score_1<<endl;
	cout<<"\nscore_2: "<<d1.score<<endl;
	
	return 0;
}
