#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
//helper functions such as abs()
#include <math.h>
#include "../include/data_set_h.h"

using namespace std;

//operator overload
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

//helper function to process_first_line, process_ride-lines and process_solution files
//takes a given string and returns a vector containing the values in the line
vector<int> data_set::split_string_to_ints(string line) {
	// Used to split string around spaces. 
	stringstream ss_line(line);
	vector<int> values;
    // read as many numbers as possible.
    for (int number; ss_line >> number;) {
        values.push_back(number);
    }
	return values;
}

//	====================================== INSTANCE FILE ========================

void data_set::read_instance_file(string instance) {
	ifstream wf(instance);
	if (wf.is_open()) {
		//read first line of instance
		string first_line;
		getline(wf, first_line);
		process_first_line(first_line);
		//read the rest of instance lines
		string line;
		while (getline(wf, line)) {
			process_ride_lines(line);
		}
		wf.close();
	}
}

void data_set::process_first_line(string first_line) {
	vector<int> values = split_string_to_ints(first_line);
	//initialize data_set  
	R = values[0]; C = values[1]; F = values[2];
	N = values[3]; B = values[4]; T = values[5];
	rides.reserve(N); //optimize the vector rides to allocate accordingly

	//pass the number of rides N to the error checking structure
	error_check.initialize_N(N);
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
		//read solution lines
		string line;
		int line_number = 1;   // INITIAL VALUE
		while (getline(wf, line)) {
			try{
				process_solution_lines(line, line_number);
			}
			catch(int error){
				cerr << "\n\nError reading file, Line number: "<< line_number<<"\n\n"; //ERROR
			}
			line_number++;
		}
		wf.close();
	}
}

void data_set::process_solution_lines(string line, int line_number) {

	//first get a vector containing the values in the line
	vector<int> values = split_string_to_ints(line);

	//check to see if solution has the correct format
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
	
	try{
		if(elapsed_time > current_ride->f)
			throw '1';	 
			//type error one: elapsed time before ride is greater than latest finish time
	}
	catch (char error){
		status.push_back(error);
	}

	// to start the ride, the vehicle needs to be at starting point of the ride
	if(current_position != starting_position){			
		elapsed_time += distance(current_position, starting_position);	
		current_position = starting_position;
	}

	// vehicle is at the starting point

	 //wait until earliest start time
	if(elapsed_time < current_ride->s)
		elapsed_time += (current_ride->s - elapsed_time); 

	if(elapsed_time >= current_ride->s){
		// bonus points if ride starts at earliest start time
		if(elapsed_time == current_ride->s)		
			score += B;		//SCORE KEEP

		//do the ride
		elapsed_time += distance(current_position, ending_postion);
		current_position = ending_postion;
		
		try{
			//only rides completed before or at the same time as latest finish time count
			if(elapsed_time <= current_ride->f) 
				score += distance(starting_position, ending_postion);  //SCORE KEEP
			else 
				throw '2'; // type error four: elapsed time after finish is greater than latest finish time
		}
		catch (char error) {
			status.push_back(error);
		}
	}
	return status;
}

// ========================================== VERIFY =================================

// Main function
// Does the traveling of the rides assigned to the vehicles
// Keeps score for the points collected by the vehicles
// Indicates possible non-optimal rides assigned
void data_set::verify(){
	int score = 0;
	string status; // Empty string indicates successful travel of ride


	cout<<"\n=====================\n"
		<<"|Traveling the rides|"
		<<"\n=====================\n";
	//traverse the entire fleet of type map<int, vector<ride*>>
	for(auto map_iterator = fleets.begin(); map_iterator != fleets.end(); map_iterator++){
		cout<< "\n\t\t\tVehicle number: " << map_iterator->first << endl <<endl;

		//at time 0 vehicle starts from the position (0,0)
		int vehicle_position_row = 0;
		int vehicle_position_col = 0;
		int elapsed_time = 0;

		//Print the vehicle data before the rides (optional)
		cerr<<"Position: ("<<vehicle_position_row<<", "<<vehicle_position_col
			<<") score: "<<score <<" elapsed time: "<<elapsed_time<<endl;

		//Keep track of the index of the ride for error output formatting
		int current_ride_index = 1; // INITIAL VALUE

		// traverse through all the rides of the current vehicle
		for(ride *current_ride : map_iterator->second){
			//stop if maximum number of simulation steps has been reached
			if(elapsed_time >= T) break;  

			//start this ride
			status = travel(current_ride, vehicle_position_row, vehicle_position_col,
			 elapsed_time, score);

			//if there were any errors record them
			if(status != "")
				error_check.record_status(status, map_iterator->first, current_ride_index);

			//Print the vehicle data after the ride (optional)
			cerr<<"Position: ("<<vehicle_position_row<<", "<<vehicle_position_col
				<<") score: "<<score <<" elapsed time: "<<elapsed_time<<endl;

			current_ride_index++;
		}
	}
	cout<<"\nPoints collected: " << score << endl;
}