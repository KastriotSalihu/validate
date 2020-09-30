#ifndef SOME_FUNCTIONS_H
#define SOME_FUNCTIONS_H

#include <unordered_map>
#include <vector>
#include <utility>

struct file_names;
struct data_set;

class Solution{
private:
	std::string solution;

	std::vector<int> split_string_to_ints(std::string line);
	void process_solution_lines(std::string line, int line_number);
	void read_solution_file(std::string solution);
public:
	std::unordered_map<int, std::vector<int>> fleets;
	Solution(file_names files);
};

std::unordered_map<int, std::vector<int>> read_solution_file(std::string solution_name);

int get_score(const data_set&, const std::unordered_map<int, std::vector<int>>&);
int get_score_for_one_vehicle(const data_set&, const std::unordered_map<int, std::vector<int>>&, const int& vehicle_index);
int get_score_for_one_ride(const data_set&, const int& ride_index, std::pair<int, int>& position, int& step);

#endif
