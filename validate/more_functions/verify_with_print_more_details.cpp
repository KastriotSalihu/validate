void travel(ride* current_ride, int &vehicle_position_row, int &vehicle_position_col,
				 int &elapsed_time, int &score){

		pair<int&, int&> current_position(vehicle_position_row, vehicle_position_col);
		pair<int, int> starting_position = current_ride->starting_position();
		pair<int, int> ending_postion = current_ride->ending_postion();

		bool debug = true;

		if(debug) cout<<"First check of elapsed_time: "<<elapsed_time<<endl;

		// if(elapsed_time > current_ride->f )		

		// to start the ride, the vehicle needs to be at starting point
		if(current_position != starting_position){			
			elapsed_time += distance(current_position, starting_position);	
			current_position = starting_position;

			if(debug) cout<<"distance currnet, starting: "<<distance(current_position, starting_position)<<endl;
		}

		if(debug) cout<<"get to starting pos elapsed_time: "<<elapsed_time<<endl;

		// vehicle is at the starting point

		 //wait until earliest start time
		if(elapsed_time < current_ride->s)
			elapsed_time += (current_ride->s - elapsed_time); 

		if(debug) cout<<"Wait earliest elapsed_time: "<<elapsed_time<<endl;

		if(elapsed_time >= current_ride->s){

			// bonus points if ride starts at earlieset start time
			if(elapsed_time == current_ride->s)		
				score += B;		//SCORE KEEP

			//do the ride
			elapsed_time += distance(current_position, ending_postion);
			current_position = ending_postion;
			
			//only rides completed before or at the same time as latest finish time count
			if(elapsed_time <= current_ride->f) 
				score += distance(starting_position, ending_postion);  //SCORE KEEP
		}

		if(debug) cout<<"End score: "<<score<<endl<<"------------\n";

	}