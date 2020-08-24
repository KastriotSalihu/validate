#ifndef FILE_NAMES_H
#define FILE_NAMES_H

#include <string>

struct file_names{
	//Goals:
	//Store file names and paths
	std::string instance_name;
	std::string instance_path;
	std::string solution_path;

	file_names(){}
	file_names(std::string _instance_name);
};

#endif
