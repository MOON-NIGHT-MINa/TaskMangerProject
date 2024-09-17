#include "TaskManagerApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace TaskManagerApp {

    Task::Task(const std::string& title, Priority priority, Category category)
        : title(title), priority(priority), category(category) {}

    std::string Task::getTitle() const { return title; }
    Priority Task::getPriority() const { return priority; }
    Category Task::getCategory() const { return category; }

    void Task::setTitle(const std::string& title) { this->title = title; }
    void Task::setPriority(Priority priority) { this->priority = priority; }
    void Task::setCategory(Category category) { this->category = category; }

    std::string Task::toString() const {
        std::string priorityStr;
        switch (priority) {
        case Priority::Low: priorityStr = "Low"; break;
        case Priority::Medium: priorityStr = "Medium"; break;
        case Priority::High: priorityStr = "High"; break;
        }

        std::string categoryStr;
        switch (category) {
        case Category::Work: categoryStr = "Work"; break;
        case Category::Personal: categoryStr = "Personal"; break;
        case Category::Other: categoryStr = "Other"; break;
        }

        return title + " | " + priorityStr + " | " + categoryStr;
    }

    Priority Task::stringToPriority(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        if (lowerStr == "low") return Priority::Low;
        if (lowerStr == "medium") return Priority::Medium;
        if (lowerStr == "high") return Priority::High;
        throw std::invalid_argument("Invalid priority string");
    }

    Category Task::stringToCategory(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        if (lowerStr == "work") return Category::Work;
        if (lowerStr == "personal") return Category::Personal;
        if (lowerStr == "other") return Category::Other;
        throw std::invalid_argument("Invalid category string");
    }

    void TaskManager::addTask(const Task& task) {
        tasks.push_back(task);
    }

    void TaskManager::viewTasks() const {
        for (const auto& task : tasks) {
            std::cout << task.toString() << std::endl;
        }
    }

    void TaskManager::editTask(const std::string& title) {
        try {
            Task* task = findTaskByTitle(title);
            if (!task) {
                throw TaskNotFoundException("Task with title '" + title + "' not found");
            }

            std::string newTitle, priorityStr, categoryStr;
            std::cout << "Enter new title: ";
            std::getline(std::cin, newTitle);
            std::cout << "Enter new priority (Low, Medium, High): ";
            std::getline(std::cin, priorityStr);
            std::cout << "Enter new category (Work, Personal, Other): ";
            std::getline(std::cin, categoryStr);

            try {
                Priority newPriority = Task::stringToPriority(priorityStr);
                Category newCategory = Task::stringToCategory(categoryStr);

                task->setTitle(newTitle);
                task->setPriority(newPriority);
                task->setCategory(newCategory);
            }
            catch (const std::invalid_argument& e) {
                throw TaskEditException("Error updating task: " + std::string(e.what()));
            }
        }
        catch (const TaskNotFoundException& e) {
            throw;
        }
        catch (const TaskEditException& e) {
            throw;
        }
        catch (const std::exception& e) {
            throw TaskEditException("Unexpected error while editing task: " + std::string(e.what()));
        }
    }

    void TaskManager::deleteTask(const std::string& title) {
        auto it = std::remove_if(tasks.begin(), tasks.end(), [&title](const Task& task) {
            return task.getTitle() == title;
            });
        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
        }
        else {
            throw TaskNotFoundException("Task with title '" + title + "' not found");
        }
    }

    void TaskManager::saveTasksToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }
        for (const auto& task : tasks) {
            file << task.toString() << std::endl;
        }
    }

    void TaskManager::loadTasksFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open file for reading: " + filename);
        }
        tasks.clear();
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string title, priorityStr, categoryStr;
            std::getline(iss, title, '|');
            std::getline(iss, priorityStr, '|');
            std::getline(iss, categoryStr);

            // Trim whitespace
            auto trim = [](std::string& s) {
                s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                    }));
                s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                    }).base(), s.end());
                };
            trim(title);
            trim(priorityStr);
            trim(categoryStr);

            try {
                tasks.emplace_back(title, Task::stringToPriority(priorityStr), Task::stringToCategory(categoryStr));
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Skipping invalid task entry: " << line << " (" << e.what() << ")" << std::endl;
            }
        }
    }

    Task* TaskManager::findTaskByTitle(const std::string& title) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [&title](Task& task) {
            return task.getTitle() == title;
            });
        return it != tasks.end() ? &(*it) : nullptr;
    }

} // namespace TaskManagerApp