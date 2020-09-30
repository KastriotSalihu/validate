#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept> // std::runtime_error
#include <math.h>
#include <iostream>
#include <utility>
#include <time.h> //clock_t, clock, CLOCKS_PER_SEC 
#include <stdlib.h> //exit()

#include "../include/imported_functions_h.h"
#include "../include/data_set_h.h"
#include "../include/file_names_h.h"
#include "../include/global_variables_h.h"
#include "../include/ride_h.h"

using namespace std;

// unordered_map<ride_index, pair<ride_score, starting_time>>
std::unordered_map<int, std::pair<int, int>> ride_info;
// unordered_map<vehicle_index, vehicle_score>
std::unordered_map<int, int> vehicle_scores;

vector<int> Solution::split_string_to_ints(string line) {
	stringstream ss_line(line);
	vector<int> values;
    for (int number; ss_line >> number;) {
        values.push_back(number);
    }
	return values;
}

void Solution::process_solution_lines(string line, int line_number) {
	vector<int> values = split_string_to_ints(line);
	fleets.insert(make_pair(line_number, values));
}

void Solution::read_solution_file(string solution){
	ifstream wf(solution);
	if (wf.is_open()) {
		string line;
		int line_number = 1;   // INITIAL VALUE
		while (getline(wf, line)) {
			try{process_solution_lines(line, line_number);}
			catch(int error){
				cout << "\n\nError reading file, Line number: "<< line_number<<"\n\n";
			}
			line_number++;
		}
		wf.close();
	}
	else{
		cout<<"\n\nCould not open solution file!\n\n";
		exit(EXIT_FAILURE);
	}
}

Solution::Solution(file_names files) {
	solution = files.solution_path;
	read_solution_file(solution);
}

int get_score(const data_set& ds, const unordered_map<int, vector<int>>& fleets) {
	int score = 0;
	int vehicle_index = 0;
	int vehicle_score = 0;
	for (auto it = fleets.begin(); it != fleets.end(); it++)
	{
		vehicle_index = it->first;
		vehicle_score = get_score_for_one_vehicle(ds, fleets, vehicle_index);
		vehicle_scores.insert(make_pair(vehicle_index, vehicle_score));

		score += vehicle_score;
	}
	return score;
}

int get_score_for_one_vehicle(const data_set& ds, const unordered_map<int, vector<int>>& fleets, const int& vehicle_index) {
	int vehicle_score = 0; // Contains the score of the current vehicle

	int ride_score = 0;
	int ride_index = 0;

	pair<int, int> position(0, 0); // Starting position of each vehicle
	int step = 0; // The starting step for each vehicle

	int starting_time = 0;
	int distance_from_start_to_finish = 0;

	for (size_t i = 1; i < fleets.at(vehicle_index).size(); i++) // Iterate for each ride of the current vehicle
	{
		ride_index = fleets.at(vehicle_index)[i];
		ride_score = get_score_for_one_ride(ds, ride_index, position, step); // Position and step are passed by reference therefore this function changes their value

		distance_from_start_to_finish = abs(ds.rides[ride_index]->x - ds.rides[ride_index]->a) + abs(ds.rides[ride_index]->y - ds.rides[ride_index]->b);
		starting_time = step - distance_from_start_to_finish;

		if (step <= ds.T) {
			ride_info.insert(pair<int, pair<int, int>>(ride_index, make_pair(ride_score, starting_time)));
			vehicle_score += ride_score;
		}
		else {
			for (size_t j = i; j < fleets.at(vehicle_index).size(); j++) {
				ride_index = fleets.at(vehicle_index)[j];
				ride_info.insert(pair<int, pair<int, int>>(ride_index, make_pair(0, -1)));
			}
			break;
		}
	}
	return vehicle_score;
}

int get_score_for_one_ride(const data_set& ds, const int& ride_index, pair<int, int>& position, int& step) {
	int ride_score = 0; // Contains the score of the current ride
	ride* current_ride = ds.rides[ride_index];

	int a = current_ride->a;	int x = current_ride->x;	int earliest_start = current_ride->s;
	int b = current_ride->b;	int y = current_ride->y;	int latest_finish = current_ride->f;

	int distance_from_position_to_start = abs(a - position.first) + abs(b - position.second); // Distance from current position to the start intersection of the ride
	step += distance_from_position_to_start;

	if (step < earliest_start) // Including the waiting time until the earliest start
		step = earliest_start;

	int distance_from_start_to_finish = abs(x - a) + abs(y - b); // Distance from the start intersection to the finish intersection of the ride
	step += distance_from_start_to_finish;

	if (step <= latest_finish) {
		ride_score += distance_from_start_to_finish;
		if ((step - distance_from_start_to_finish) == earliest_start)
			ride_score += ds.B;
	}

	position = make_pair(x, y); // At the end of the ride the vehicle is located at the finish intersection

	return ride_score;
}
