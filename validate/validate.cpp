//Validate the solution of self-driving-rides algorithm 
//input/output 
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
//containers
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <set>
//helper functions such as abs()
#include <math.h>

using namespace std;

set<int> assigned_rides; //keep track of assigned rides

//operator overload
bool operator==(pair<int&, int&> lhs, pair<int, int> rhs){
	return lhs.first == rhs.first and lhs.second == rhs.second;
}
bool operator!=(pair<int&, int&> lhs, pair<int, int> rhs){
	return !(lhs.first == rhs.first and lhs.second == rhs.second);
}


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

	pair<int, int> starting_position (){	return make_pair(a, b);	}
	pair<int, int> ending_postion(){	return make_pair(x, y);	}
};

struct errors {
	//line_number, ride, status
	map<int, pair<int, string>> solution_file_errors; //errors found during reading solution file

	// (vehicle, ride), status
	map<pair<int, int>, string> travel_errors; 	//errors found during traveling the rides

	//total number of rides
	int N;

	void initialize(int _N){	N = _N;	}


	//helper function to simplify recording errors from ride travel
	//while traveling the rides of any vehicle keep a record of errors/the status
	void record_status(string status, int vehicle_number, int current_ride_index){
		travel_errors[make_pair(vehicle_number,current_ride_index)] =  status;
	}

	// ======================== SOLUTION FILE FORMAT ERRORS =========================
	void test_solution_format(string &line, int line_number, const vector<int> &values){
		string status = ""; //empty indicates no errors
		int ride = -1;  //  indicates that no errors produced are specific to just one ride
		//run tests for
		whitespace(line, status);
		number_of_rides(line_number, values, status);
		uniqueness(values, line_number, ride, status);
		if(status != "")
			//record errors
			solution_file_errors[line_number] = make_pair(ride, status);
	}

	//test whitespace formatting
	void whitespace(string line, string &status){
		//check if file has correct number of spaces between numbers: 1 whitespace	    
	    bool previous = false;	//if previous and current character are spaces, the file format is incorrect
	    string::const_iterator it = line.begin();
	    while (it != line.end() && !line.empty()){	    	
	    	//check if this character is a whitespace:true or a number:false
	    	if (!isdigit(*it)){
	    		//check if previous character was a whitespace
	    		try {if (previous) throw 'w';} catch (char error){ status.push_back(error);}
	    		previous = true;
	    	}
	    	else previous = false;
	    	++it;
	    }
	    //error handling if line has trailing whitespace
	    try {if(line.back() == ' ') throw 't';} catch (char error){	status.push_back(error);}
	}

	//test number of rides formatting
	void number_of_rides(int line_number, const vector<int> &values, string &status){
		int number_of_rides = values[0];
		//check if declared number of rides is valid
		try{
			if(number_of_rides < 0 || number_of_rides > N)
				throw 'N';
		}
		catch (char error){
			status.push_back(error);
		}
		//check if number of declared rides is consistent within the line
		//i.e. the declared number in the beginning of the line vs number of rides declared during the line
		try{
			if(number_of_rides != values.size()-1)
				throw 'n';
		}
		catch (char error){
			status.push_back(error);
		}
	}

	//test if any of the rides has already been assigned
	void uniqueness(const vector<int> &values, int line_number, int &ride, string &status){
		pair<set<int>::iterator, bool> return_value; //for use with return value of std::set::insert
		//begin looping from second element
		for(auto itr = ++values.begin(); itr != values.end(); itr++){
			return_value = assigned_rides.insert(*itr);
			//check if ride has already been assigned
			try{
				if(!return_value.second){
					ride = *itr;
					throw 'u';
				}
			}
			catch (char error){
				status.push_back(error);
			}
		}
	}

	// ============================== PRINT ERRORS DETECTED ======================== 

	//report the errors that have been detected from reading the solution file
	void report_solution_format(string status, int line_number, int ride){
		cerr <<"ERROR:"<<endl;
		cerr <<"  Line number: " << line_number << endl;
		if(ride != -1)	cerr <<"  Ride number: " << ride + 1 <<endl;
		print_errors(status);
	}

	//report the errors that have been detected from traveling the rides
	void report_status(string status, int vehicle_number, int current_ride_index){
		cerr <<"ERROR:"<<endl;
		cerr<<"  Vehicle number: " << vehicle_number << endl;
		cerr<<"  Ride number: " <<current_ride_index << endl;
		print_errors(status);
	}

	//print the appropriate errors
	void print_errors(string status){
		for(char error : status){
			switch (error){
				//travel errors DETECTED IN data_set.verify()
				case '1': cerr << "    Elapsed time before ride is greater than latest finish time!\n";
				break;
				case '2': cerr << "    Elapsed time after finish is greater than latest finish time!\n";
				break;
				//solution file format errors
				//Hard errors
				case 'w': cerr << "    Contains double whitespaces!\n";
				break;
				case 't': cerr << "    Contains trailing non eol whitespace!\n";
				break;
				case 'N': cerr << "    Number of assigned rides must be positive"
										<<" and smaller or equal the number of all rides: "<< N << "!\n";
				break;
				case 'n': cerr << "    The number of rides is different than declared!\n";
				break;
				case 'u': cerr << "    Ride has already been assigned!\n";
				break;
				default: cerr<<"\nError reading errors\n";
			}
		}
	}



	// =========================================== ERRORS =========================

	// Main function
	// Prints additional information regarding errors recorded during reading solution file
	// Prints additional information regarding errors recorded during traveling the rides
	void see_errors(){
		cout<<" ---------------------------" 
			<<"Errors during reading solution file: \n";
		cout<<"Total errors : "<<solution_file_errors.size()<<endl;
		for(auto map_iterator = solution_file_errors.begin(); map_iterator != solution_file_errors.end(); map_iterator++){
			report_solution_format(map_iterator->second.second, map_iterator->first, map_iterator->second.first);
		}
		cout<<" ---------------------------" 
			<<"Errors during traveling the rides: \n";
		cout<<"Total errors : "<<travel_errors.size()<<endl;
		for(auto map_iterator = travel_errors.begin(); map_iterator != travel_errors.end(); map_iterator++){
			report_status(map_iterator->second, map_iterator->first.first, map_iterator->first.second);
		}
	}
};

struct data_set {
	int R, C, F, N, B, T;

	vector<ride*> rides;  //load instance
	map<int, vector<ride*>> fleets; //load solution

	string instance; 	//path to instance file
	string solution;	//path to solution file

	errors error_check;

	// initialize the variables
	data_set(file_names files) {
		//initialize the file names and paths
		instance = files.instance_path;
		solution = files.solution_path;

		// Read and place the data from the files into the respective variables and arrays  
		read_instance_file(instance);
		read_solution_file(solution);

		//pass the number of rides N to the error checking structure
		error_check.initialize(N);

		//optimize storage
		rides.shrink_to_fit();
	}

	//helper function to process_first_line, process_ride-lines and process_solution files
	//takes a given string and returns a vector containing the values in the line
	vector<int> split_string_to_ints(string line) {
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
				process_ride_lines(line);
			}
			wf.close();
		}
	}

	void process_first_line(string first_line) {
		vector<int> values = split_string_to_ints(first_line);
		//initialize data_set  
		R = values[0]; C = values[1]; F = values[2];
		N = values[3]; B = values[4]; T = values[5];
		rides.reserve(N); //optimize the vector rides to allocate accordingly
	}

	void process_ride_lines(string line) {
		vector<int> values = split_string_to_ints(line);
		//get a new "ride" initiated with values read on current line
		ride* new_ride = new ride(values[0], values[1], values[2], values[3], values[4], values[5]);
		rides.push_back(new_ride);
	}

	//	===================================== SOLUTION FILE =========================
	void read_solution_file(string solution){
		ifstream wf(solution);
		if (wf.is_open()) {
			//read solution lines
			string line;
			int line_number = 1;   // INITIAL VALUE
			while (getline(wf, line)) {
				process_solution_lines(line, line_number);
				line_number++;
			}
			wf.close();
		}
	}

	void process_solution_lines(string line, int line_number) {

		//first get a vector containing the values in the line
		vector<int> values = split_string_to_ints(line);

		//check to see if solution has the correct format
		error_check.test_solution_format(line, line_number, values);

		vector<ride*> current_vehicle_rides;

		//transform from the index of the ride to the actual ride
		for(int i = 1; i <= values.size()-1; i++)
			current_vehicle_rides.push_back(rides[values[i]]);

		//(here line_number indicates the index of the vehicle)
		fleets.insert(make_pair(line_number, current_vehicle_rides));
	}



	// ========================================== TRAVEL =============================

	//helper functions to travel
	int distance(pair<int, int> lhs, pair<int, int> rhs){
		return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
	}
	int distance(pair<int&, int&> lhs, pair<int, int> rhs){
		return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
	}

	string travel(ride* current_ride, int &vehicle_position_row, int &vehicle_position_col, int &elapsed_time, int &score){

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
	void verify(){
		int score = 0;
		string status; // Empty string indicates successful travel of ride

		//traverse the entire fleet of type map<int, vector<ride*>>
		for(auto map_iterator = fleets.begin(); map_iterator != fleets.end(); map_iterator++){
			cout<< "Vehicle number: " << map_iterator->first << endl;

			//at time 0 vehicle starts from the position (0,0)
			int vehicle_position_row = 0;
			int vehicle_position_col = 0;
			int elapsed_time = 0;

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
};

int main(int argc, char* argv[]) {

	//get the names of the files from the arguments of the call
	file_names files(argv[1]);

	//pass the file names to the data set
	data_set d1(files);

	d1.verify();

	char i;
	cout <<"\nWould you like to see a more detailed description of errors? ";
	cin >>i;
	if(i=='y'){
		d1.error_check.see_errors();
	}

	//print_map(d1.fleets);
	return 0;
}
