#pragma once

#include <string>

enum class Priority {
	Low,
	Med,
	High
};

class Task {
	private:
		std::string description;
		bool is_complete;
		Priority priority;
		
	public:
		// Constructor
		Task(const std::string& description, Priority priority);

		// Getters -- retrieving task fields
		std::string get_description() const;
		bool is_complete() const;
		Priority get_priority() const;
		
		// Setters -- modifying task states
		void mark_is_complete(bool status);
}
