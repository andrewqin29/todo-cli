# CLI Todo List

A simple, lightweight command-line based task manager built with C++.

## Features

- Add, remove, and display tasks.
- Mark tasks as complete or incomplete.
- Assign priorities (high, med, low) with color-coding.
- Automatically clears the task list daily at 4 AM.

## Prerequisites

To build and run this project, you will need the following tools installed on your system:

- A C++20 compliant compiler (e.g., g++, clang++)
- The `make` tool

For most Linux and macOS systems, these tools are likely already installed. For Windows, it is recommended to use WSL.

## Build Instructions

1.  **Clone the repository:**
    ```sh
    git clone git@github.com:andrewqin29/todo-cli.git
    ```
2.  **Navigate to the project directory:**
    ```sh
    cd todo_cli
    ```
3.  **Compile the project:**
    ```sh
    make
    ```
4. **Add to PATH:**
	```
	sudo cp todo /usr/local/bin/
	```
    
## Usage

Once built, you can use the program with the following commands:

- **Display tasks:** `todo`
- **Add a task:** `todo add "My new task"`
- **Add a task with priority:** `todo add "An urgent task" --priority high` other options are: med, low (default)
- **Mark a task as done:** `todo done 1` where 1 is the task index
- **Undo a completed task:** `todo undone 1`
- **Remove a task:** `todo remove 1`
- **Clear all tasks:** `todo clear`
- **Show help:** `todo help`
