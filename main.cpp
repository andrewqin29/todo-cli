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
const std::string COLOR_TITLE_RED = "\033[31m";				// title red
const std::string COLOR_HIGH_PRIORITY = "\033[38;5;217m";	// pastel red
const std::string COLOR_MED_PRIORITY = "\033[38;5;229m";	// pastel yellow
const std::string COLOR_LOW_PRIORITY = "\033[37m";			// standard white/default
const std::string STYLE_COMPLETED = "\033[9;90m";			// strikethrough and grey
const std::string COLOR_RESET = "\033[0m";

// function prototypes
std::filesystem::path get_task_path();
void save_tasks_to_file(const std::vector<Task>& tasks);
std::vector<Task> load_tasks_from_file();
void display_tasks(const std::vector<Task>& tasks);
void display_help();
void display_command_err();

// ------- MAIN -------
int main(int argc, char* argv[]) {

	// **** DAILY RESET FUNCTIONALITY ****
	std::filesystem::path tasks_path = get_task_path();

	if (!std::filesystem::exists(tasks_path)) {
		return;
	}

	auto last_modified = std::filesystem::last_write_time(tasks_path);
	auto curr_time = std::chrono::system_clock::now()
	auto today_midnight = std::chrono::floor<std::chrono::days>(curr_time);
	auto reset_time = today_midnight + std::chrono::hours(4); // 4am

	if (last_modified < reset_time && curr_time >= reset_time) {
		std::ofstream ofs;
		ofs.open(tasks_path, std::ofstream::out | std::ofstream::trunc); // erase all
		ofs.close();
	}

	// **** LOAD TASK VECTOR ****
	std::vector<Task> tasks = load_tasks_from_file();

	// **** COMMAND HANDLING ****
	if (argc < 2) { // 'todo'
		display_tasks(tasks);
		return 0
	}

	std::string command = argv[1];

	if (command == "add") {
		if (argc < 3) {
            std::cout << "Error: Missing task description." << std::endl;
            std::cout << "Usage: todo add \"<description>\" [--priority <level>]" << std::endl;
            return 1;
        }

        std::string description = argv[2];
        Priority priority = Priority::Low;

        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--priority" && i + 1 < argc) {
                std::string p_level = argv[i + 1];
                if (p_level == "high") {
                    priority = Priority::High;
                } else if (p_level == "med") {
                    priority = Priority::Med;
                } else if (p_level == "low") {
                    priority = Priority::Low;
                }
                
                // we've processed the flag and its value, so skip the next argument
                i++; 
            }
        }
	} else if (command == "display") {
		if (argc == 2) {
			display_tasks(tasks);
		} else {
			std::string flag = argv[2];
			if (flag == "priority") {
				// sort tasks based on priority (high > med > low)
				std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
					return static_cast<int>(a.get_priority()) > static_cast<int>(b.get_priority());
				});
				display_tasks(tasks);
			} else if (flag == "pending") {
				// create new tasks vector only containing pending tasks
				std::vector<Task> pending_tasks;
				for (const Task &task : tasks) {
					if (!task.is_complete) {
						pending_tasks.push_back(task);
					}
				}
				display_tasks(pending_tasks);
			} else {
				display_command_err();
			}
		}
		return 0;
		
	} else if (command == "done" || command == "undone") {
		if (argc < 3) {
            std::cout << "Error: Please provide a task index." << std::endl;
            return 1;
        }
        try {
            int index = std::stoi(argv[2]);
            if (index <= 0 || static_cast<size_t>(index) > tasks.size()) {
                std:: cout << "Error: index out of bounds." << std::endl;
                return 1;
            }

            Task& task_to_modify = tasks.at(index - 1);

            if (command == "done") {
                task_to_modify.mark_is_complete(true);
                std::cout << "Task " << index << " marked as complete." << std::endl;
            } else { // command == "undone"
                task_to_modify.mark_is_complete(false);
                std::cout << "Task " << index << " marked as pending." << std::endl;
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "Error: Invalid index. Please provide a number." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Error: The index you entered is too large." << std::endl;
        }
        
	} else if (command == "remove") {
		if (argc == 3) {
			try {
				index = std::stoi(argv[2]);
				if (index <= 0 || static_cast<size_t>(index) > tasks.size()) {
				    std:: cout << "Error: index out of bounds." << std::endl;
				    return 1; 
				}
				tasks.erase(tasks.begin() + index - 1);
				std::cout << "Task at index " << argv[2] << " removed." << std::endl;
			} catch (...) {
				std::cout << "Error: invalid index specifier." << std::endl;
			}
		} else {
			display_command_err();
		}

	} else if (command == "help") {
		display_help();
		return 0;
	} else {
		display_command_err();
	}
	
	return 0;
}

// function implementations
std::filesystem::path get_task_path() {
	const char *home_dir = getenv("HOME")
	if (home_dir == nullptr) {
		std::cerr << "Error: Could not locate home directory." << std::endl;
		return;
	}
	
	std::string home_dir_str(home_dir);
	std::filesystem::path tasks_path = home_dir_str / ".todo-cli-tasks.csv" //path to tasks csv should be /home/username/.todo-cli-tasks.csv
	return tasks_path
}

void save_tasks_to_file(const std::vector<Task>& tasks) {
	// clear the current tasks file
	std::filesystem::path tasks_path = get_task_path();
	std::ofstream ofs;
	ofs.open(tasks_path, std::ofstream::out | std::ofstream::trunc); // erase all

	if (!ofs.is_open()) {
		std::cerr << "Error: Could not open tasks file." << std::endl;
		return;
	}
	
	for(const Task& curr : tasks) {
		int complete = curr.is_complete();
		int p_int = static_cast<int>(curr.get_priority());

		// format: is_complete,priority_int,description
		ofs << complete << "," << p_int << "," << curr.get_description() << "\n";
	}
	ofs.close();
}

std::vector<Task> load_tasks_from_file() {
	std::filesystem::path tasks_path = get_task_path();
	std::vector<Task> ret;

	std::ifstream file(tasks_path);
	if (!file.is_open()) { 
		return ret;
	}

	std::string curr;
	while(std::getline(file, curr)) {
		std::stringstream ss(curr);
		std::string part;

		bool is_complete;
		int priority_int;
		std::string description;

		// parse is_complete (0 or 1)
		std::getline(ss, part, ',');
		is_complete = (std::stoi(part) == 1);

		// parse priority
		std::getline(ss, part, ',');
		priority_int = std::stoi(part);
		Priority priority = static_cast<Priority>(priority_int);

		// parse description
		std::getline(ss, part);
		description = part;

		Task curr_task(description, priority);
		if (is_complete) {
			curr_task.mark_is_complete(true);
		}

		// add to ret vector
		ret.push_back(curr_task);
		
	}
	file.close();
	return ret;
}

void display_tasks(const std::vector<Task>& tasks) {
	if (tasks.size() == 0) {
		std::cout << "No tasks to complete :)" << std::endl;
		return;
	}

	std::cout << COLOR_TITLE_RED << "TODO:" << COLOR_RESET << std::endl;
	std::cout << "------" << std::endl;

	for (size_t i=0; i<tasks.size(); i++) {
		const Task &curr = tasks[i];
		std::string description = curr.get_description();
		bool complete = curr.is_complete();
		Priority p = curr.get_priority();

		std::string FILL_COLOR;
		switch (p) {
			case Priority::Low:
				FILL_COLOR = COLOR_LOW_PRIORITY;
				break;
			case Priority::Med:
				FILL_COLOR = COLOR_MED_PRIORITY;
				break;
			case Priority::High:
				FILL_COLOR = COLOR_HIGH_PRIORITY;
				break;
			default:
				FILL_COLOR = COLOR_LOW_PRIORITY;
				break;
		}
		std::string ticker = "[ ]";
		if (complete) {
			ticker = "[x]";
			FILL_COLOR = STYLE_COMPLETED;
		}

		// output final string
		std::cout 	<< (i+1) << ". "
					<< ticker 
					<< "  " 
					<< FILL_COLOR 
					<< description 
					<< COLOR_RESET 
					<< "\n";
	}
}

void display_help() {
    std::cout << COLOR_TITLE_RED << "Todo CLI Help" << COLOR_RESET << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "A simple command-line task manager." << std::endl << std::endl;
    std::cout << "USAGE:" << std::endl;
    std::cout << "  todo <command> [arguments]" << std::endl << std::endl;
    std::cout << "COMMANDS:" << std::endl;
    std::cout << "  add \"<description>\" [--priority <level>]" << std::endl;
    std::cout << "    Adds a new task. Priority can be 'low', 'med', or 'high'." << std::endl << std::endl;
    std::cout << "  display [priority|pending]" << std::endl;
    std::cout << "    Displays tasks. Default is all. 'priority' sorts by priority, 'pending' shows only incomplete tasks." << std::endl << std::endl;
    std::cout << "  done <index>" << std::endl;
    std::cout << "    Marks the task at the given index as complete." << std::endl << std::endl;
    std::cout << "  undone <index>" << std::endl;
    std::cout << "    Marks the task at the given index as incomplete." << std::endl << std::endl;
    std::cout << "  remove <index>" << std::endl;
    std::cout << "    Permanently removes the task at the given index." << std::endl << std::endl;
    std::cout << "  help" << std::endl;
    std::cout << "    Shows this help message." << std::endl;
}

void display_command_err() {
	std::cout << "Error: Unknown command '" << command << "'" << std::endl;
	std::cout << "Run 'todo help' to see a list of available commands." << std::endl;
}
