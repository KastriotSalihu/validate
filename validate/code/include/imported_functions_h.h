#ifndef SOME_FUNCTIONS_H
#define SOME_FUNCTIONS_H

#include <map>
#include <vector>

#include "data_set_h.h"


class Score{
private:
	std::string solution;

	std::vector<int> split_string_to_ints(std::string line);
	void process_solution_lines(std::string line, int line_number);
	void read_solution_file(std::string solution);

public:
	std::map<int, std::vector<int>> fleets;
	Score(file_names files);
};

std::map<int, std::vector<int>> read_solution_file(std::string solution_name);
int get_score(data_set ds, std::map<int, std::vector<int>> fleets);

#endif
