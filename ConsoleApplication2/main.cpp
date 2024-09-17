#include <iostream>
#include <limits>
#include "TaskManagerApp.h"

using namespace TaskManagerApp;
using namespace std;

void displayMenu() {
    cout << "\n===== Task Manager =====\n";
    cout << "1. Add Task\n";
    cout << "2. View Tasks\n";
    cout << "3. Edit Task\n";
    cout << "4. Delete Task\n";
    cout << "5. Save Tasks to File\n";
    cout << "6. Load Tasks from File\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    TaskManager taskManager;
    bool running = true;

    while (running) {
        displayMenu();
        int choice;

        try {
            if (!(cin >> choice)) {
                throw invalid_argument("Invalid input. Please enter a number.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            switch (choice) {
            case 1: {
                string title, priorityStr, categoryStr;
                Priority priority;
                Category category;

                cout << "Enter task title: ";
                getline(cin, title);

                while (true) {
                    cout << "Enter priority (Low, Medium, High): ";
                    getline(cin, priorityStr);

                    try {
                        priority = Task::stringToPriority(priorityStr);
                        break;
                    }
                    catch (const invalid_argument& e) {
                        cout << "Error: " << e.what() << " Please enter a valid priority (Low, Medium, High).\n";
                    }
                }

                while (true) {
                    cout << "Enter category (Work, Personal, Other): ";
                    getline(cin, categoryStr);

                    try {
                        category = Task::stringToCategory(categoryStr);
                        break;
                    }
                    catch (const invalid_argument& e) {
                        cout << "Error: " << e.what() << " Please enter a valid category (Work, Personal, Other).\n";
                    }
                }

                try {
                    taskManager.addTask(Task(title, priority, category));
                    cout << "Task added successfully.\n";
                }
                catch (const runtime_error& e) {
                    cout << "Error adding task: " << e.what() << '\n';
                }
                break;
            }

            case 2:
                if (taskManager.getTasks().empty()) {
                    cout << "No tasks to display.\n";
                }
                else {
                    cout << "\n===== Tasks =====\n";
                    taskManager.viewTasks();
                }
                break;
            case 3: {
                string title;
                cout << "Enter the title of the task to edit: ";
                getline(cin, title);
                try {
                    taskManager.editTask(title);
                    cout << "Task edited successfully.\n";
                }
                catch (const TaskNotFoundException& e) {
                    cout << "Error: " << e.what() << '\n';
                }
                catch (const TaskEditException& e) {
                    cout << "Error editing task: " << e.what() << '\n';
                }
                break;
            }
            case 4: {
                string title;
                cout << "Enter the title of the task to delete: ";
                getline(cin, title);
                try {
                    taskManager.deleteTask(title);
                    cout << "Task deleted successfully.\n";
                }
                catch (const TaskNotFoundException& e) {
                    cout << "Error: " << e.what() << '\n';
                }
                break;
            }
            case 5: {
                string filename;
                cout << "Enter filename to save tasks: ";
                getline(cin, filename);
                try {
                    taskManager.saveTasksToFile(filename);
                    cout << "Tasks saved successfully.\n";
                }
                catch (const runtime_error& e) {
                    cout << "Error saving tasks: " << e.what() << '\n';
                }
                break;
            }
            case 6: {
                string filename;
                cout << "Enter filename to load tasks: ";
                getline(cin, filename);
                try {
                    taskManager.loadTasksFromFile(filename);
                    cout << "Tasks loaded successfully.\n";
                }
                catch (const runtime_error& e) {
                    cout << "Error loading tasks: " << e.what() << '\n';
                }
                break;
            }
            case 7:
                running = false;
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 7.\n";
            }
        }
        catch (const invalid_argument& e) {
            cout << "Input error: " << e.what() << '\n';
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
        catch (const exception& e) {
            cout << "An unexpected error occurred: " << e.what() << '\n';
        }

        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return 0;
}