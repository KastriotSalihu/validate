#ifndef DATA_SET_H
#define DATA_SET_H

#include "ride_h.h"
#include "errors_h.h"
#include "file_names_h.h"

struct data_set {
	int R, C, F, N, B, T;

	std::vector<ride*> rides;  //load instance
	std::map<int, std::vector<ride*>> fleets; //load solution

	std::string instance; 	//path to instance file
	std::string solution;	//path to solution file

	errors error_check;

	// initialize the variables
	data_set(file_names files);

	std::vector<int> split_string_to_ints(std::string line);

	void read_instance_file(std::string instance);
	void process_first_line(std::string first_line);
	void process_ride_lines(std::string line);

	void read_solution_file(std::string solution);
	void process_solution_lines(std::string line, int line_number);

	//helper functions to travel
	int distance(std::pair<int, int> lhs, std::pair<int, int> rhs);
	int distance(std::pair<int&, int&> lhs, std::pair<int, int> rhs);
	//travels a given ride
	std::string travel(ride* current_ride, int &vehicle_position_row,
				 int &vehicle_position_col, int &elapsed_time, int &score);

	// Main function
	// Does the traveling of all the rides assigned to all the vehicles
	// Keeps score for the points collected by the vehicles
	// Indicates possible non-optimal rides assigned
	void verify();
};

#endif