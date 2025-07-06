#include "Task.h"

// Constructor
Task::Task(const std::string& description, Priority priority) : description(description), priority(priority),
is_complete(false) {}

// Getters
std::string Task::get_description() const {
	return this->description;
}

bool Task::is_complete() const {
	return this->is_complete;
}

Priority Task::get_priority() const {
	return this->priority;
}

// Setters
void Task::mark_is_complete(bool status) {
	this->is_complete = status;
}
