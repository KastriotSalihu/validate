
void verify(unordered_map<int, vector<ride*>> fleets){
	int score = 0;

	//traverse the entire fleet of type map<int, vector<ride*>>
	for(auto map_iterator = fleets.begin(); map_iterator != fleets.end(); map_iterator++){
		//at time 0 vehicle starts from the position (0,0)
		int vehicle_position_row = 0;
		int vehicle_position_col = 0;
		int elapsed_time = 0;

		for(ride *current_ride : map_iterator->second){
			if(elapsed_time >= T){
				break;  
			}
			travel(current_ride, vehicle_position_row, vehicle_position_col, elapsed_time, score);
		}
	}
	cout<<"\nPoints collected: " << score << endl;
}

void travel(ride* current_ride, int &vehicle_position_row, int &vehicle_position_col, int &elapsed_time, int &score){

	pair<int&, int&> current_position(vehicle_position_row, vehicle_position_col);
	pair<int, int> starting_position = make_pair(current_ride->a, current_ride->b);
	pair<int, int> ending_postion = make_pair(current_ride->x, current_ride->y);
	bool bonus = false;

	// to start the ride, the vehicle needs to be at starting point of the ride
	if(current_position != starting_position){			
		elapsed_time += distance(current_position, starting_position);	
		current_position = starting_position;
	}
	// vehicle is now at the starting point
	//wait until earliest start time
	if(elapsed_time < current_ride->s){
		elapsed_time += (current_ride->s - elapsed_time); 
	}

	if(elapsed_time >= current_ride->s){
		if(elapsed_time == current_ride->s)	{
			bonus = true;	
		}
		//do the ride
		elapsed_time += distance(current_position, ending_postion);
		current_position = ending_postion;
		//only rides completed before or at the same time as latest finish time count
		if(elapsed_time <= current_ride->f) {
			score += distance(starting_position, ending_postion);  //SCORE KEEP
			if(bonus){
				score += B;		//SCORE KEEP
			}
		}
	}
}

//rideindex, finish_time
unordered_map<int, int> ride_info;
