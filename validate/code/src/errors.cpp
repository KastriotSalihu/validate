#include <set>
#include <iostream>

#include "../include/errors_h.h"

using namespace std;

// ======================== SOLUTION FILE FORMAT ERRORS =========================
bool errors::test_solution_format(string &line, int line_number, const vector<int> &values){
	string status = ""; //empty indicates no errors
	int ride_index = -1;  //indicates taht errors are not specific to just one ride
	bool ignorable = true;   //true indicates error can be ignored; line is usable

	try{
		number_of_rides(line, values, status);
		whitespace(line, status);
		uniqueness(values, ride_index, status);
	}
	catch(int error){
		ignorable = false;
	}

	if(status != "")	//record errors
		solution_file_errors[line_number] = std::make_pair(ride_index, status);
	return ignorable;
}

//test whitespace formatting
void errors::whitespace(string line, string &status){
	
    //check if file has correct number of spaces between numbers: 1 whitespace	    
    bool previous = false;	//if previous and current character are spaces, the file format is incorrect
    string::const_iterator it = line.begin();
    for(char c : line){    	
    	//check if this character is a whitespace:true or a number:false
    	if (!isdigit(c)){
    		if (previous){
    			error_message['w'].second++;
	    		status.push_back('w'); //two whitespaces in a row
    		}
    		previous = true;
    	}
    	else previous = false;
    	++it;
    }
    //error handling if line has trailing whitespace
    if(line.back() == ' '){
		error_message['t'].second++;
		status.push_back('t');
	}
}
void errors::number_of_rides(string line, const vector<int> &values, string &status){
	if(values[0] < 0 || values[0] > N){
		error_message['N'].second++;
		status.push_back('N');
	}
	if(values[0] != values.size()-1){
		error_message['n'].second++;
		status.push_back('n');
		check_characters(line, status);
	}
	for(int ride_index : values)
		if(ride_index > N){
			error_message['e'].second++;
			status.push_back('e');
		}

	if(status != "")
		throw -1;
}
void errors::check_characters(string line, string &status){
	//line should contain only digits and whitespaces
	for(char c : line){
		if(!(c == ' ' || isdigit(c))){
			error_message['c'].second++;
			status.push_back('c');
		}
	}
}

//test if any of the rides has already been assigned
void errors::uniqueness(const vector<int> &values,int &ride_index, string &status){
	pair<set<int>::iterator, bool> return_value; //for use with return value of std::set::insert
	//begin iterating from second element
	for(auto itr = ++values.begin(); itr!= values.end(); itr++){
		return_value = assigned_rides.insert(*itr);
		if(!return_value.second){
			ride_index = *itr;
			error_message['u'].second++;
			status.push_back('u');
			throw -1;
		}
	}
}
	// ============================== PRINT ERRORS DETECTED ======================== 

//report the errors that have been detected from reading the solution file
void errors::print_solution_errors(string status, int line_number, int ride_index){
	cerr <<"HARD ERROR:\n  Line number: " << line_number;
	if(ride_index != -1)	cerr <<"  \tRide number: " << ride_index <<endl;
	else cerr <<endl;
	print_status(status);
}
//report the errors that have been detected from traveling the rides
void errors::print_travel_errors(string status, int vehicle_number, int current_ride_index){
	cerr <<"ERROR:\n  Vehicle number: " << vehicle_number;
	cerr<<"  \tRide number: " <<current_ride_index << endl;
	print_status(status);
}
//print the appropriate error message
void errors::print_status(string status){
for(char error : status){
	cerr << error_message[error].first;
	}
}
// =========================================== ERRORS =========================

// Main function
// Prints additional information regarding errors recorded during reading solution file
// Prints additional information regarding errors recorded during traveling the rides
void errors::print_all_errors(){
	cout<<" ---------------------------" 
		<<"Errors reading solution file: \n";
	cout<<"Total errors : "<<solution_file_errors.size()<<endl;
	for(auto map_iterator = solution_file_errors.begin(); map_iterator != solution_file_errors.end(); map_iterator++){
		print_solution_errors(map_iterator->second.second, map_iterator->first, map_iterator->second.first);
	}
	cout<<" ---------------------------" 
		<<"Errors traveling the rides: \n";
	cout<<"Total errors : "<<travel_errors.size()<<endl;
	for(auto map_iterator = travel_errors.begin(); map_iterator != travel_errors.end(); map_iterator++){
		print_travel_errors(map_iterator->second, map_iterator->first.first, map_iterator->first.second);
	}
}

void errors::print_summary(){
	cout<<"\nTotal lines with errors reading solution file: "<<solution_file_errors.size()<<endl;
	for(auto iter = error_message.begin(); iter != error_message.find('1'); iter++){
		cerr << iter->second.second << iter->second.first;
	}
	cout<<"\nTotal rides with errors during the travel: "<<travel_errors.size()<<endl;
	for(auto iter = error_message.find('1'); iter != error_message.end(); iter++){
		cerr << iter->second.second << iter->second.first;
	}
}
