#pragma once

#include <string>
#include <cstdint>

enum class Priority {
	Low,
	Med,
	High
};

class Task {
	private:
		uint64_t m_id;
		std::string description;
		bool m_is_complete;
		Priority priority;
		
	public:
		// Constructor
		Task(uint64_t id, const std::string& description, Priority priority);

		// Getters -- retrieving task fields
		uint64_t get_id() const;
		std::string get_description() const;
		bool is_complete() const;
		Priority get_priority() const;
		
		// Setters -- modifying task states
		void mark_is_complete(bool status);
};
