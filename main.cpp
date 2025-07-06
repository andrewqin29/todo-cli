#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>		// string parsing
#include <algorithm>	// for std::sort
#include <chrono> 		// for 4am reset functionality
#include <filesystem>	// ^^

#include "Task.h"

// ANSI escape codes for color and style
const std::string COLOR_HIGH_PRIORITY = "\033[38;5;217m";	// pastel red
const std::string COLOR_MED_PRIORITY = "\033[38;5;229m";	// pastel yellow
const std::string COLOR_LOW_PRIORITY = "\033[37m";			// standard white/default
const std::string STYLE_COMPLETED = "\033[9;90m";			// strikethrough and grey
const std::string COLOR_RESET = "\033[0m";

// function prototypes
void save_tasks_to_file(const std::vector<Task>& tasks);
std::vector<Task> load_tasks_from_file();
void display_tasks(const std::vector<Task>& tasks);

// main
int main(int argc, char* argv[]) {
	
}

// function implementations
void save_tasks_to_file(const std::vector<Task>& tasks) {
	
}

std::vector<Task> load_tasks_from_file() {
	
}

void display_tasks(const std::vector<Task>& tasks) {
	
}
