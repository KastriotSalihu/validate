//Materials
// https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/
// https://www.geeksforgeeks.org/vector-in-cpp-stl/
// https://www.geeksforgeeks.org/pair-in-cpp-stl/
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <sstream>
#include <fstream>
using namespace std;

struct file_names{
	string instance_name;
	string instance_path;
	string solution_path;

	file_names(){	}
	file_names(string _instance_name){
		instance_name = _instance_name;
		instance_path = "dataset/" + instance_name + ".in";
		solution_path = "solution/" + instance_name + ".out";
	}
};

struct ride {
	int a, b, x, y, s, f;
	vector<ride*> close_next_rides;

	ride() {	}
	ride(int _a, int _b, int _x, int _y, int _s, int _f) {
		a = _a; b = _b; x = _x; y = _y; s = _s; f = _f;
	}
};

struct data_set {
	int R, C, F, N, B, T;

	vector<ride*> rides;
	map<int, vector<ride*>> fleets;

	string instance;
	string solution;

	data_set(file_names files) {
		//initialize the file names and paths
		instance = files.instance_path;
		solution = files.solution_path;

		// Read and place the data from the files into the respective variables and arrays  
		read_instance_file(instance);
		read_solution_file(solution);
	}


	vector<int> split_string_to_ints(string line) {
		// Used to split string around spaces. 
		stringstream ss_line(line);
		vector<int> values;
		// Read all words (numbers)
		string word;
		do {
			ss_line >> word;
			//Check if still in the middle of the line
			if (word != "") {
				int value = stoi(word); //Convert string to int
				values.push_back(value);
			}
			//While there is more to read
		} while (ss_line);
		return values;  //return vector with string words(numbers) -> int values
	}


	void read_instance_file(string instance) {
		ifstream wf(instance);
		if (wf.is_open()) {
			//read first line of instance
			string first_line;
			getline(wf, first_line);
			process_first_line(first_line);
			//read the rest of instance lines
			string line;
			while (getline(wf, line)) {
				//cout << line << "\n";	//View the content of the file 'instance' while it's being read
				process_ride_lines(line);
			}
			wf.close();
		}
	}

	void process_first_line(string first_line) {
		vector<int> values = split_string_to_ints(first_line);
		//initiate data_set  
		R = values[0];
		C = values[1];
		F = values[2];
		N = values[3];
		B = values[4];
		T = values[5];
	}

	void process_ride_lines(string line) {
		vector<int> values = split_string_to_ints(line);
		//get a new "ride" initiated with values read on current line
		ride* new_ride = new ride(values[0], values[1], values[2], values[3], values[4], values[5]);
		rides.push_back(new_ride);
	}



	void read_solution_file(string solution){
		ifstream wf(solution);
		if (wf.is_open()) {
			//read solution lines
			string line;
			unsigned int line_number = 1;
			while (getline(wf, line)) {
				//cout << line << "\n";	//View the content of the file 'solution' while it's being read
				process_solution_lines(line, line_number);
				line_number++;
			}
			wf.close();
		}
	}

	void process_solution_lines(string line, unsigned int line_number) {
		//first get a vector containing the values of the line
		vector<int> values = split_string_to_ints(line);

		//check if this line has the correct format
		int number_of_rides = values[0];
		//an artifact from split_string_to_ints is causing the last number to be doubled, so size()-2 is used			
		if(number_of_rides != values.size()-2)
			cout<<"ERROR: Vehicle number: "<< line_number 
				<< " has more rides than declared!\n";

		//check if ride has already been assigned
		


		//for each ride number get the corresponding ride and then register it as a ride of the current vehicle
		//i.e. transform from the index of the ride to the actual ride
		vector<ride*> current_vehicle_rides;
		for(int i = 1; i <= values.size()-2; i++){
			cout<<"values["<<i<<"]: '"<<values[i]<<"'\n";
			current_vehicle_rides.push_back(rides[values[i]]);
		}

		//insert this vehicle to the fleet
		//(here line_number indicates the index of the vehicle)
		fleets.insert(make_pair(line_number, current_vehicle_rides));
	}

};



void print_map(map<int, vector<ride*>> fleets) {
	for(auto itr = fleets.begin(); itr != fleets.end(); itr++){
		for (ride* d_ride : itr->second) {
		//ride from [0, 0] to [1, 3], earliest start 2, latest finish 9
			cout << "Ride from [" << d_ride->a << ", " << d_ride->b << "] to [" << d_ride->x << ", " << d_ride->y
				<< "], earliest start " << d_ride->s << ", latest finish " << d_ride->f << endl;
			}
		cout<<endl;
	}
}

int main(int argc, char* argv[]) {
	//get the names of the files from the arguments of the call
	file_names files(argv[1]);
	//pass the file names to the data set
	data_set d1(files);

	print_map(d1.fleets);
	return 0;
}