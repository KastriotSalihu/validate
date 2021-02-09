#ifndef DATA_SET_H
#define DATA_SET_H

#include "errors_h.h"

struct ride;
struct file_names;

struct data_set {
	//Goals:
	//Store instance and solution
	//Verify the solution is correct
	int R, C, F, N, B, T;

	std::vector<ride*> rides;  //load instance
	std::map<int, std::vector<ride*>> fleets; //load solution

	std::string instance;
	std::string solution;

	errors error_check;

	int score;

	data_set(file_names files);

	std::vector<int> split_string_to_ints(std::string line);

	void read_instance_file(std::string instance);
	void process_first_line(std::string first_line);
	void process_ride_lines(std::string line);

	void read_solution_file(std::string solution);
	bool process_solution_lines(std::string line, int line_number);

	//helper functions to travel
	int distance(std::pair<int, int> lhs, std::pair<int, int> rhs);
	int distance(std::pair<int&, int&> lhs, std::pair<int, int> rhs);

	//travels a given ride
	std::string travel(ride* current_ride, int &vehicle_position_row,
				 int &vehicle_position_col, int &elapsed_time, int &score);

	// Main function to call
	// Travels all the rides assigned to all the vehicles
	// Keeps score, Indicates any non-optimal rides assigned
	void verify();
};

#endif