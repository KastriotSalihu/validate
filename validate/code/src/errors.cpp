#include <set>
#include <iostream>

#include "../include/errors_h.h"

using namespace std;

// ======================== SOLUTION FILE FORMAT ERRORS =========================
bool errors::test_solution_format(string &line, int line_number, const vector<int> &values){

	string status = ""; //empty indicates no errors
	int ride = -1;  //indicates that no errors produced are specific to just one ride
	bool ignorable = true;   //false indicates error can be ignored, no need to throw away the line

	//run tests for		
	number_of_rides(values, status, ignorable);
	whitespace(line, status);	//ignorable by default
	uniqueness(values, ride, status, ignorable);		

	if(status != "")
		//record errors
		solution_file_errors[line_number] = std::make_pair(ride, status);
	return ignorable;
}

//test whitespace formatting
void errors::whitespace(string line, string &status){
	//check if file has correct number of spaces between numbers: 1 whitespace	    
    bool previous = false;	//if previous and current character are spaces, the file format is incorrect
    string::const_iterator it = line.begin();
    while (it != line.end() && !line.empty()){	    	
    	//check if this character is a whitespace:true or a number:false
    	if (!isdigit(*it)){
    		//check if previous character was a whitespace
    		try {if (previous) throw 'w';} catch (char error){ status.push_back(error);}
    		previous = true;
    	}
    	else previous = false;
    	++it;
    }
    //error handling if line has trailing whitespace
    try {if(line.back() == ' ') throw 't';} catch (char error){	status.push_back(error);}
}

//test number of rides formatting
void errors::number_of_rides(const vector<int> &values, string &status, bool &ignorable){
	int number_of_rides = values[0];
	//check if declared number of rides is valid
	try{
		if(number_of_rides < 0 || number_of_rides > N)
			throw 'N';
	}
	catch (char error){
		status.push_back(error);
		ignorable = false;
	}
	//check if number of declared rides is consistent within the line
	//i.e. the declared number in the beginning of the line vs number of rides declared during the line
	try{
		if(number_of_rides != values.size()-1)
			throw 'n';
	}
	catch (char error){
		status.push_back(error);
		ignorable = false;
	}

	//check if the rides in this line exist 
	for(int ride : values){
		try{
			if(ride > N)
				throw 'e';
		}
		catch (char error){
			status.push_back(error); // record the error
			ignorable = false;
		}
	}
}

	//test if any of the rides has already been assigned
void errors::uniqueness(const vector<int> &values,int &ride, string &status, bool &ignorable){
	pair<set<int>::iterator, bool> return_value; //for use with return value of std::set::insert
	//begin looping from second element
	for(auto itr = ++values.begin(); itr != values.end(); itr++){
		return_value = assigned_rides.insert(*itr);
		//check if ride has already been assigned
		try{
			if(!return_value.second){
				ride = *itr;
				ignorable = false;
				throw 'u';
			}
		}
		catch (char error){
			status.push_back(error);
		}
	}
}


	// ============================== PRINT ERRORS DETECTED ======================== 

//report the errors that have been detected from reading the solution file
void errors::report_solution_format(string status, int line_number, int ride){
	cerr <<"ERROR:"<<endl;
	cerr <<"  Line number: " << line_number << endl;
	if(ride != -1)	cerr <<"  Ride number: " << ride + 1 <<endl;
	print_errors(status);
}

//report the errors that have been detected from traveling the rides
void errors::report_status(string status, int vehicle_number, int current_ride_index){
	cerr <<"ERROR:"<<endl;
	cerr<<"  Vehicle number: " << vehicle_number << endl;
	cerr<<"  Ride number: " <<current_ride_index << endl;
	print_errors(status);
}

//print the appropriate errors
void errors::print_errors(string status){
	for(char error : status){
		switch (error){
			//travel errors DETECTED IN data_set.verify()
			case '1': cerr << "    Elapsed time before ride is greater than latest finish time!\n";
			break;
			case '2': cerr << "    Elapsed time after finish is greater than latest finish time!\n";
			break;
			//solution file format errors
			//Hard errors
			case 'w': cerr << "    Contains double whitespaces!\n";
			break;
			case 't': cerr << "    Contains trailing non eol whitespace!\n";
			break;
			case 'N': cerr << "    Number of assigned rides must be positive"
									<<" and smaller or equal the number of all rides: "<< N << "!\n";
			break;
			case 'n': cerr << "    The number of rides is different than declared!\n";
			break;
			case 'u': cerr << "    Ride has already been assigned!\n";
			break;
			case 'e': cerr << "    Ride does not exist!\n";
			break;
			default : cerr << "\nError reading errors\n";
		}
	}
}


// =========================================== ERRORS =========================

// Main function
// Prints additional information regarding errors recorded during reading solution file
// Prints additional information regarding errors recorded during traveling the rides
void errors::see_errors(){
	cout<<" ---------------------------" 
		<<"Errors reading solution file: \n";
	cout<<"Total errors : "<<solution_file_errors.size()<<endl;
	for(auto map_iterator = solution_file_errors.begin(); map_iterator != solution_file_errors.end(); map_iterator++){
		report_solution_format(map_iterator->second.second, map_iterator->first, map_iterator->second.first);
	}
	cout<<" ---------------------------" 
		<<"Errors traveling the rides: \n";
	cout<<"Total errors : "<<travel_errors.size()<<endl;
	for(auto map_iterator = travel_errors.begin(); map_iterator != travel_errors.end(); map_iterator++){
		report_status(map_iterator->second, map_iterator->first.first, map_iterator->first.second);
	}
}