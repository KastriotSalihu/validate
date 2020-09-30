#ifndef RIDE_H
#define RIDE_H

#include <utility>
#include <vector>

struct ride {
	//Goals:
	//Store ride information
	int a, b, x, y, s, f;
	std::vector<ride*> close_next_rides;

	ride() {	}
	ride(int _a, int _b, int _x, int _y, int _s, int _f) {
		a = _a; b = _b; x = _x; y = _y; s = _s; f = _f;
	}

	std::pair<int, int> starting_position (){	return std::make_pair(a, b);	}
	std::pair<int, int> ending_postion(){	return std::make_pair(x, y);	}
};

#endif