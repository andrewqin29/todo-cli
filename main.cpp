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

// ------- MAIN -------
int main(int argc, char* argv[]) {

	// **** DAILY RESET FUNCTIONALITY ****
	const char *home_dir = getenv("HOME")
	if (home_dir == nullptr) {
		std::cerr << "Error. Could not locate home directory." << endl;
		return;
	}

	std::string home_dir_str(home_dir);
	std::filesystem::path tasks_path = home_dir_str / ".todo-cli-tasks.csv" //path to tasks csv should be /home/username/.todo-cli-tasks.csv

	if (!std::filesystem::exists(tasks_path)) {
		return;
	}

	// time vars
	auto last_modified = std::filesystem::last_write_time(tasks_path);
	auto curr_time = std::chrono::system_clock::now()
	
	auto today_midnight = std::chrono::floor<std::chrono::days>(curr_time);
	auto reset_time = today_midnight + std::chrono::hours(4); // 4am

	// clear todo list
	if (last_modified < reset_time && curr_time >= reset_time) {
		std::ofstream ofs;
		ofs.open(tasks_path, std::ofstream::out | std::ofstream::trunc); // open it in write mode and truncate (remove contents)
		ofs.close();
	}
	
}

// function implementations
void save_tasks_to_file(const std::vector<Task>& tasks) {
	
}

std::vector<Task> load_tasks_from_file() {
	
}

void display_tasks(const std::vector<Task>& tasks) {
	
}
