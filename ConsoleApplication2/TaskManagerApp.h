#ifndef TASKMANAGERAPP_H
#define TASKMANAGERAPP_H

#include <string>
#include <vector>
#include <stdexcept>

namespace TaskManagerApp {

    enum class Priority { Low, Medium, High };
    enum class Category { Work, Personal, Other };

    class Task {
    public:
        Task(const std::string& title, Priority priority, Category category);

        std::string getTitle() const;
        Priority getPriority() const;
        Category getCategory() const;

        void setTitle(const std::string& title);
        void setPriority(Priority priority);
        void setCategory(Category category);

        std::string toString() const;

        static Priority stringToPriority(const std::string& str);
        static Category stringToCategory(const std::string& str);

    private:
        std::string title;
        Priority priority;
        Category category;
    };

    class TaskNotFoundException : public std::runtime_error {
    public:
        TaskNotFoundException(const std::string& message) : std::runtime_error(message) {}
    };

    class TaskEditException : public std::runtime_error {
    public:
        TaskEditException(const std::string& message) : std::runtime_error(message) {}
    };

    class TaskManager {
    public:
        void addTask(const Task& task);
        void viewTasks() const;
        void editTask(const std::string& title);
        void deleteTask(const std::string& title);
        void saveTasksToFile(const std::string& filename) const;
        void loadTasksFromFile(const std::string& filename);
        const std::vector<Task>& getTasks() const { return tasks; }

    private:
        std::vector<Task> tasks;
        Task* findTaskByTitle(const std::string& title);
    };

} // namespace TaskManagerApp

#endif // TASKMANAGERAPP_H