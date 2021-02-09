#include <set>
#include <iostream>
#include <ctype.h>
#include <string>

#include "../include/errors_h.h"

using namespace std;

void errors::count_error(char error)
{
	this->error_message[error].second++;
}

// ======================== SOLUTION FILE FORMAT ERRORS =========================
bool errors::test_solution_format(string &line, int line_number, const vector<int> &values)
{
	string status = "";	  //empty indicates no errors
	int ride_index = -1;  //-1 indicates that errors are not specific to just one ride
	bool invalid = false; //true indicates error can be ignored; line is usable

	status.append(check_number_of_rides(line, values));
	status.append(check_whitespace(line));
	status.append(check_uniqueness(values, ride_index));

	if (status != "")
	{ //record errors
		solution_file_errors[line_number] = std::make_pair(ride_index, status);
		invalid = true;
	}
	return invalid;
}

//test whitespace formatting
string errors::check_whitespace(string line)
{
	string status = "";
	//check if file has correct number of spaces between numbers: 1 whitespace
	bool previous = false; //if previous and current character are spaces, the file format is incorrect

	for (char c : line)
	{
		if (isspace(c))
		{
			if (previous)
			{
				this->count_error('w');
				status.push_back('w'); //two whitespaces in a row
			}
			previous = true;
		}
		else
			previous = false
	}
	//error handling if line has trailing whitespace
	if (line.back() == ' ')
	{
		this->count_error('t');
		status.push_back('t');
	}
	return status;
}

string errors::check_number_of_rides(string line, const vector<int> &values)
{
	string status = "";
	status.append(check_characters(line));

	if (values[0] < 0 || values[0] > N)
	{
		this->count_error('N');
		status.push_back('N');
	}
	if ((size_t)values[0] != values.size() - 1)
	{
		this->count_error('n');
		status.push_back('n');
	}
	for (int ride_index : values)
		if (ride_index > N)
		{
			this->count_error('e');
			status.push_back('e');
		}
	return status;
}
string errors::check_characters(string line)
{
	string status = "";
	//line should contain only digits and whitespaces
	for (char c : line)
	{
		if (!(c == ' ' || isdigit(c)))
		{
			this->count_error('c');
			status.push_back('c');
		}
	}
	return status;
}

//test if any of the rides has already been assigned
string errors::check_uniqueness(const vector<int> &values, int &ride_index)
{
	string status;
	pair<set<int>::iterator, bool> return_value; //for use with return value of std::set::insert
	//begin iterating from second element
	for (auto itr = ++values.begin(); itr != values.end(); itr++)
	{
		return_value = assigned_rides.insert(*itr);
		if (!return_value.second)
		{
			ride_index = *itr;
			error_message['u'].second++;
			status.push_back('u');
			throw -1;
		}
	}
	return status;
}

// ============================== PRINT ERRORS DETECTED ========================

//report the errors that have been detected from reading the solution file
void errors::print_solution_errors(string status, int line_number, int ride_index)
{
	cout << "HARD ERROR:\n  Line number: " << line_number;
	if (ride_index != -1)
		cout << "  \tRide number: " << ride_index << endl;
	else
		cout << endl;
	print_status(status);
}
//report the errors that have been detected from traveling the rides
void errors::print_travel_errors(string status, int vehicle_number, int current_ride_index)
{
	cout << "ERROR:\n  Vehicle number: " << vehicle_number;
	cout << "  \tRide number: " << current_ride_index << endl;
	print_status(status);
}
//print the appropriate error message
void errors::print_status(string status)
{
	for (char error : status)
	{
		cout << error_message[error].first;
	}
}
// =========================================== ERRORS =========================

// Main function
// Prints additional information regarding errors recorded during reading solution file
// Prints additional information regarding errors recorded during traveling the rides
void errors::print_all_errors()
{
	cout << " ---------------------------"
		 << "Errors reading solution file: \n";
	cout << "Total errors : " << solution_file_errors.size() << endl;
	for (auto map_iterator = solution_file_errors.begin(); map_iterator != solution_file_errors.end(); map_iterator++)
	{
		print_solution_errors(map_iterator->second.second, map_iterator->first, map_iterator->second.first);
	}
	cout << " ---------------------------"
		 << "Errors traveling the rides: \n";
	cout << "Total errors : " << travel_errors.size() << endl;
	for (auto map_iterator = travel_errors.begin(); map_iterator != travel_errors.end(); map_iterator++)
	{
		print_travel_errors(map_iterator->second, map_iterator->first.first, map_iterator->first.second);
	}
}

void errors::print_summary()
{
	cout << "\nTotal lines with errors reading solution file: " << solution_file_errors.size() << endl;
	for (auto iter = error_message.begin(); iter != error_message.find('1'); iter++)
	{
		cout << iter->second.second << iter->second.first;
	}
	cout << "\nTotal rides with errors during the travel: " << travel_errors.size() << endl;
	for (auto iter = error_message.find('1'); iter != error_message.end(); iter++)
	{
		cout << iter->second.second << iter->second.first;
	}
}
