#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
//exit()
#include <stdlib.h> 
//helper functions such as abs()
#include <math.h>
#include "../include/data_set_h.h"
#include "../include/global_variables_h.h"

using namespace std;

bool operator==(pair<int&, int&> lhs, pair<int, int> rhs){
	return lhs.first == rhs.first and lhs.second == rhs.second;
}
bool operator!=(pair<int&, int&> lhs, pair<int, int> rhs){
	return !(lhs.first == rhs.first and lhs.second == rhs.second);
}

data_set::data_set(file_names files) {
	//initialize the file names and paths
	instance = files.instance_path;
	solution = files.solution_path;

	// Read and place the data from the files into the respective variables and arrays  
	read_instance_file(instance);
	read_solution_file(solution);

	//optimize storage
	rides.shrink_to_fit();
}
//helper function for: process_first_line, process_ride_lines and process_solution_files
//takes a given string and returns a vector containing the values in the line
vector<int> data_set::split_string_to_ints(string line) {
	stringstream ss_line(line);
	vector<int> values;
    for (int number; ss_line >> number;) {
        values.push_back(number);
    }
	return values;
}

//	====================================== INSTANCE FILE ========================
void data_set::read_instance_file(string instance) {
	ifstream wf(instance);
	if (wf.is_open()) {
		string first_line;
		getline(wf, first_line);
		process_first_line(first_line);
		string line;
		while (getline(wf, line)) {
			process_ride_lines(line);
		}
		wf.close();
	}
}
void data_set::process_first_line(string first_line) {
	vector<int> values = split_string_to_ints(first_line);
	R = values[0]; C = values[1]; F = values[2];
	N = values[3]; B = values[4]; T = values[5];

	rides.reserve(N); //optimize the vector rides to allocate accordingly

	//pass the number of rides N to the error checking structure
	error_check.set_N(N);
}
void data_set::process_ride_lines(string line) {
	vector<int> values = split_string_to_ints(line);
	//get a new "ride" initiated with values read on current line
	ride* new_ride = new ride(values[0], values[1], values[2], values[3], values[4], values[5]);
	rides.push_back(new_ride);
}
//	===================================== SOLUTION FILE =========================
void data_set::read_solution_file(string solution){
	ifstream wf(solution);
	if (wf.is_open()) {
		string line;
		int line_number = 1;   // INITIAL VALUE
		while (getline(wf, line)) {
			try{
				process_solution_lines(line, line_number);
			}
			catch(int error){
				cerr << "\n\nError reading file, Line number: "<< line_number<<"\n\n";
			}
			line_number++;
		}
		wf.close();
	}
	else{
		cerr<<"\n\nCould not open solution file!\n\n";
		exit(EXIT_FAILURE);
	}
}
void data_set::process_solution_lines(string line, int line_number) {
	vector<int> values = split_string_to_ints(line);

	if(!error_check.test_solution_format(line, line_number, values))
		throw -1; //if there's an error that can't be ignored

	vector<ride*> current_vehicle_rides;
	//transform from the index of the ride to the actual ride
	for(int i = 1; i <= values.size()-1; i++)
		current_vehicle_rides.push_back(rides[values[i]]);

	//(here line_number indicates the index of the vehicle)
	fleets.insert(make_pair(line_number, current_vehicle_rides));
}
// ========================================== TRAVEL =============================
//helper functions to travel
int data_set::distance(pair<int, int> lhs, pair<int, int> rhs){
	return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
}
int data_set::distance(pair<int&, int&> lhs, pair<int, int> rhs){
	return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
}

string data_set::travel(ride* current_ride, int &vehicle_position_row, int &vehicle_position_col, int &elapsed_time, int &score){

	pair<int&, int&> current_position(vehicle_position_row, vehicle_position_col);
	pair<int, int> starting_position = current_ride->starting_position();
	pair<int, int> ending_postion = current_ride->ending_postion();
	string status = ""; //Empty string indicates no errors
	bool bonus = false;
	
	try{
		if(elapsed_time > current_ride->f)
			throw '1';	 
	}
	catch (char error){
		error_check.error_message[error].second++;
		status.push_back(error);
	}

	// to start the ride, the vehicle needs to be at starting point of the ride
	if(current_position != starting_position){			
		elapsed_time += distance(current_position, starting_position);	
		current_position = starting_position;
	}

	// vehicle is now at the starting point

	//wait until earliest start time
	if(elapsed_time < current_ride->s)
		elapsed_time += (current_ride->s - elapsed_time); 

	if(elapsed_time >= current_ride->s){
		if(elapsed_time == current_ride->s)	
			bonus = true;	
		//do the ride
		elapsed_time += distance(current_position, ending_postion);
		current_position = ending_postion;
		try{
			//only rides completed before or at the same time as latest finish time count
			if(elapsed_time <= current_ride->f) {
				score += distance(starting_position, ending_postion);  //SCORE KEEP
				if(bonus)
					score += B;		//SCORE KEEP
			}
			else 
				throw '2';
		}
		catch (char error) {
			error_check.error_message[error].second++;
			status.push_back(error);
		}
	}
	return status;
}

// ========================================== VERIFY =================================
// Main function to call
// Does the traveling of all the rides assigned to all the vehicles
// Keeps score, Indicates any non-optimal rides assigned
void data_set::verify(){
	int score = 0;
	string status; // Empty string indicates successful travel of ride

	cout<<"\n=====================\n"
		<<"|Traveling the rides|"
		<<"\n=====================\n";
	//traverse the entire fleet of type map<int, vector<ride*>>
	for(auto map_iterator = fleets.begin(); map_iterator != fleets.end(); map_iterator++){
		if(debug)
			cout<< "\n\t\t\tVehicle number: " << map_iterator->first << endl <<endl;

		//at time 0 vehicle starts from the position (0,0)
		int vehicle_position_row = 0;
		int vehicle_position_col = 0;
		int elapsed_time = 0;

		if(debug)
			cerr<<"Position: ("<<vehicle_position_row<<", "<<vehicle_position_col
				<<") score: "<<score <<" elapsed time: "<<elapsed_time<<endl;

		//Keep track of the index of the ride for error output formatting
		int current_ride_index = 1; // INITIAL VALUE

		for(ride *current_ride : map_iterator->second){
			if(elapsed_time >= T) break;  
			status = travel(current_ride, vehicle_position_row, vehicle_position_col,
			 				elapsed_time, score);
			if(status != "")
				error_check.record_travel_errors(status, map_iterator->first, current_ride_index);
			if(debug)
				cerr<<"Position: ("<<vehicle_position_row<<", "<<vehicle_position_col
					<<") score: "<<score <<" elapsed time: "<<elapsed_time<<endl;

			current_ride_index++;
		}
	}
	cout<<"\nPoints collected: " << score << endl;
	this->score = score;
}