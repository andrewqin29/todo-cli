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
const std::string COLOR_TITLE_RED = "\033[31m";				
const std::string COLOR_HIGH_PRIORITY = "\033[38;5;217m";	// pastel red
const std::string COLOR_MED_PRIORITY = "\033[38;5;229m";	// pastel yellow
const std::string COLOR_LOW_PRIORITY = "\033[37m";			// standard white/default
const std::string STYLE_COMPLETED = "\033[9;90m";			// strikethrough and grey
const std::string COLOR_RESET = "\033[0m";

const char* const TASKS_FILENAME = ".todo-cli-tasks.csv";

// function prototypes
void check_and_perform_daily_reset();
std::filesystem::path get_task_path();
void save_tasks_to_file(const std::vector<Task>& tasks);
std::vector<Task> load_tasks_from_file();
void display_tasks(const std::vector<Task>& tasks);
void display_help();

// ------- MAIN -------
int main(int argc, char* argv[]) {
	std::cout << std::unitbuf;
	
	// **** DAILY RESET FUNCTIONALITY ****
	check_and_perform_daily_reset();

	// **** LOAD TASK VECTOR ****
	std::vector<Task> tasks = load_tasks_from_file();

	// **** COMMAND HANDLING ****
	if (argc < 2) { // 'todo' with no arguments
		display_tasks(tasks);
		return 0;
	}

	std::string command = argv[1];

	if (command == "add") {
		if (argc < 3) {
            std::cerr << "Error: Missing task description." << std::endl;
            std::cerr << "Usage: todo add \"<description>\" [--priority <level>]" << std::endl;
            return 1;
        }

        std::string description = argv[2];
        Priority priority = Priority::Low;

        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--priority" && i + 1 < argc) {
                std::string p_level = argv[i + 1];
                if (p_level == "high") priority = Priority::High;
                else if (p_level == "med") priority = Priority::Med;
                else if (p_level == "low") priority = Priority::Low;

                i++; 
            }
        }
        tasks.emplace_back(description, priority);
        std::cout << "Added new task: \"" << description << "\"" << std::endl;
        
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
				for (const auto &task : tasks) {
					if (!task.is_complete()) {
						pending_tasks.push_back(task);
					}
				}
				display_tasks(pending_tasks);
			} else {
				std::cerr << "Error: Unknown flag '" << flag << "' for display command." << std::endl;
                std::cerr << "Run 'todo help' for usage details." << std::endl;
			}
		}
		return 0;
		
	} else if (command == "done" || command == "undone") {
		if (argc < 3) {
            std::cerr << "Error: Please provide a task index." << std::endl;
            return 1;
        }
        try {
            size_t index = std::stoul(argv[2]);
            if (index == 0 || index > tasks.size()) {
                std::cerr << "Error: Index " << index << " is out of bounds." << std::endl;
                return 1;
            }

            Task& task_to_modify = tasks.at(index - 1);

            if (command == "done") {
                task_to_modify.mark_is_complete(true);
                std::cout << "Task " << index << " marked as complete." << std::endl;
            } else {
                task_to_modify.mark_is_complete(false);
                std::cout << "Task " << index << " marked as pending." << std::endl;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid index. Please provide a number." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: The index you entered is too large." << std::endl;
        }
        
	} else if (command == "remove") {
		if (argc < 3) {
            std::cerr << "Error: Please provide a task index to remove." << std::endl;
            return 1;
        }

        try {
            size_t index = std::stoul(argv[2]);
            if (index == 0 || index > tasks.size()) {
                std::cerr << "Error: Index " << index << " is out of bounds." << std::endl;
                return 1;
            }
            tasks.erase(tasks.begin() + (index - 1));
            std::cout << "Task " << index << " removed." << std::endl;

        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid index. Please provide a positive number." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: The index you entered is too large." << std::endl;
        }

	} else if (command == "help") {
		display_help();
		return 0;
		
	} else {
		std::cout << "Error: Invalid command. See 'todo help' for more info." << std::endl;
	}

	// save tasks after command modifications
	save_tasks_to_file(tasks);
	return 0;
}

// ------- FUNCTION IMPLEMENTATIONS -------

void check_and_perform_daily_reset() {
    std::filesystem::path tasks_path = get_task_path();

    if (!std::filesystem::exists(tasks_path)) {
        return; // file doesn't exist, nothing to reset.
    }

    auto last_modified_time_point = std::filesystem::last_write_time(tasks_path);
    auto last_modified_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        last_modified_time_point - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
    );
    
    auto current_time = std::chrono::system_clock::now();
    auto today_midnight = std::chrono::floor<std::chrono::days>(current_time);
    auto four_am_today = today_midnight + std::chrono::hours(4);

    if (last_modified_time < four_am_today && current_time >= four_am_today) {
        std::ofstream ofs(tasks_path, std::ios::trunc);
        ofs.close();
    }
}

std::filesystem::path get_task_path() {
	const char *home_dir = getenv("HOME");
	if (home_dir == nullptr) {
		std::cerr << "Fatal Error: HOME environment variable not set. Cannot determine file path." << std::endl;
		exit(1);
	}
	
	std::string home_dir_str(home_dir);
	std::filesystem::path tasks_path = std::filesystem::path(home_dir) / TASKS_FILENAME; //path to tasks csv should be /home/username/.todo-cli-tasks.csv
	return tasks_path;
}

void save_tasks_to_file(const std::vector<Task>& tasks) {
	// clear the current tasks file
	std::filesystem::path tasks_path = get_task_path();
    std::ofstream ofs(tasks_path, std::ios::trunc); // erase all

	if (!ofs.is_open()) {
		std::cerr << "Error: Could not open tasks file." << std::endl;
		return;
	}
	
	for(const Task& task : tasks) {
		std::string description = task.get_description();
		std::string processed_description;
		for (char c : description) {
			if (c == '"') {
				processed_description += "\"\"";
			} else {
				processed_description += c;
			}
		}

		ofs << task.is_complete() << ","
            << static_cast<int>(task.get_priority()) << ","
            << "\"" << processed_description << "\"" // Enclose in quotes
            << "\n";
	}
}

std::vector<Task> load_tasks_from_file() {
	std::filesystem::path tasks_path = get_task_path();
	std::vector<Task> loaded_tasks;
	std::vector<std::string> valid_lines; // valid lines in case of rewrite
	bool is_file_dirty = false; // flag for rewriting file

	std::ifstream file(tasks_path);
	if (!file.is_open()) { 
		return loaded_tasks;
	}

	std::string line;
	while(std::getline(file, line)) {
		if (line.empty()) continue;
		
		try {
			std::stringstream ss(line);
			std::string part;

			// parse is_complete
			std::getline(ss, part, ',');
			bool is_complete = (std::stoi(part) == 1);

			// parse priority
			std::getline(ss, part, ',');
			Priority priority = static_cast<Priority>(std::stoi(part));

			// parse description
			std::string description;
			std::getline(ss, description);
			
			if (!description.empty() && description.front() == '"' && description.back() == '"') {
                // 1. Remove the outer quotes.
                description = description.substr(1, description.length() - 2);
                // 2. Unescape any doubled-up quotes ("") back to a single quote (").
                std::string final_description;
                for (size_t i = 0; i < description.length(); ++i) {
                    if (description[i] == '"' && i + 1 < description.length() && description[i+1] == '"') {
                        final_description += '"';
                        i++; // Skip the next quote
                    } else {
                        final_description += description[i];
                    }
                }
                description = final_description;
            }

            Task current_task(description, priority);
            if (is_complete) {
                current_task.mark_is_complete(true);
            }
            
            loaded_tasks.push_back(current_task);
            valid_lines.push_back(line);
            
		} catch (const std::exception &e) {
			std::cerr << "Warning: Malformed line found in tasks file. Please don't edit the file directly. It will be removed." << std::endl;
            std::cerr << "  > " << line << std::endl;
            is_file_dirty = true;
		}
		
	}
	
	file.close();

	if(is_file_dirty) {
		std::ofstream ofs(tasks_path, std::ios::trunc);
		for (const auto& valid : valid_lines) {
			ofs << valid << "\n";
		}
		std::cout << "Tasks file has been cleaned." << std::endl;
	}

	return loaded_tasks;
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
					<< ticker << " " 
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

