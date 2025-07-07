#include "Task.h"

// Constructor
Task::Task(uint64_t id, const std::string& description, Priority priority)
	 : m_id(id),
	 description(description), 
	 m_is_complete(false),
	 priority(priority)
{}

// Getters

uint64_t Task::get_id() const {
	return this->m_id;
}

std::string Task::get_description() const {
	return this->description;
}

bool Task::is_complete() const {
	return this->m_is_complete;
}

Priority Task::get_priority() const {
	return this->priority;
}

// Setters
void Task::mark_is_complete(bool status) {
	this->m_is_complete = status;
}
