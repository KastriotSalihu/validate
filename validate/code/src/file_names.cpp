#include <string>
#include "../include/file_names_h.h"

using namespace std;

file_names::file_names(string _instance_name){
	instance_name = _instance_name;
	instance_path = "dataset/" + instance_name + ".in";
	solution_path = "solution/" + instance_name + ".out";
}