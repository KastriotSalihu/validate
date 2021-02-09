#ifndef ERRORS_H
#define ERRORS_H

#include <map>
#include <utility>
#include <string>
#include <vector>
#include <set>

struct errors
{
	//Goals:
	//Check for solution file formatting
	//Record all errors
	//Print summary and detailed errors

	errors(int N) : N(N) {}
	int N; //total number of rides

	std::set<int> assigned_rides;

	//line_number, ride, status
	std::map<int, std::pair<int, std::string>> solution_file_errors;
	// (vehicle, ride), status
	std::map<std::pair<int, int>, std::string> travel_errors;

	std::map<char, std::pair<std::string, int>> error_message = {
		//error, error_message, number_of_occurrences
		{'w', std::make_pair("    Contains double whitespaces!\n", 0)},
		{'t', std::make_pair("    Contains trailing non eol whitespace!\n", 0)},
		{'N', std::make_pair("    Number of assigned rides must be positive and smaller or equal the number of all rides!\n", 0)},
		{'n', std::make_pair("    The number of rides is different than declared!\n", 0)},
		{'c', std::make_pair("    Line contains letters or symbols!\n", 0)},
		{'u', std::make_pair("    Ride has already been assigned!\n", 0)},
		{'e', std::make_pair("    Ride does not/cannot exist!\n", 0)},
		{'1', std::make_pair("    Elapsed time before ride is greater than latest finish time!\n", 0)},
		{'2', std::make_pair("    Elapsed time after finish is greater than latest finish time!\n", 0)}};

	void count_error(char);

	//helper function to simplify recording errors from ride travel
	void record_travel_errors(std::string status, int vehicle_number, int current_ride_index)
	{
		travel_errors[std::make_pair(vehicle_number, current_ride_index)] = status;
	}

	// ======================== TEST SOLUTION FILE FORMAT ERRORS ===================
	bool test_solution_format(std::string &line, int line_number, const std::vector<int> &values);
	std::string check_whitespace(std::string line);
	std::string check_number_of_rides(std::string line, const std::vector<int> &values);
	std::string check_characters(std::string line);
	std::string check_uniqueness(const std::vector<int> &values, int &ride_index);

	// ============================== PRINT ERRORS DETECTED ========================
	void print_solution_errors(std::string status, int line_number, int ride_index);
	void print_travel_errors(std::string status, int vehicle_number, int current_ride_index);
	// Read status and print appropriate errors
	void print_status(std::string status);
	void print_summary();

	// Main function
	void print_all_errors();
};

#endif