#ifndef ERRORS_H
#define ERRORS_H

#include <map>
#include <utility>
#include <string>
#include <vector>
#include <set>

struct errors {
	std::set<int> assigned_rides; //keep track of assigned rides

	//line_number, ride, status
	std::map<int, std::pair<int, std::string>> solution_file_errors; //errors found during reading solution file

	// (vehicle, ride), status
	std::map<std::pair<int, int>, std::string> travel_errors; 	//errors found during traveling the rides

	//total number of rides
	int N;
	void initialize_N(int _N){	N = _N;	}

	//helper function to simplify recording errors from ride travel
	//(while traveling the rides of any vehicle keep a record of errors/the status)
	void record_status(std::string status, int vehicle_number, int current_ride_index){
		travel_errors[std::make_pair(vehicle_number,current_ride_index)] =  status;
	}

	// ======================== SOLUTION FILE FORMAT ERRORS =========================
	bool test_solution_format(std::string &line, int line_number, const std::vector<int> &values);

	//test whitespace formatting
	void whitespace(std::string line, std::string &status);

	//test number of rides formatting
	void number_of_rides(const std::vector<int> &values, std::string &status, bool &ignorable);

	//test if any of the rides has already been assigned
	void uniqueness(const std::vector<int> &values,int &ride, std::string &status, bool &ignorable);

	// ============================== PRINT ERRORS DETECTED ======================== 

	//report the errors that have been detected from reading the solution file
	void report_solution_format(std::string status, int line_number, int ride);

	//report the errors that have been detected from traveling the rides
	void report_status(std::string status, int vehicle_number, int current_ride_index);

	//print the appropriate errors
	void print_errors(std::string status);

	// ================================== ERRORS ===================================

	// Main function
	// Prints additional information regarding errors recorded during reading solution file
	// Prints additional information regarding errors recorded during traveling the rides
	void see_errors();
};


#endif