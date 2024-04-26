#include <iostream>
#include <stdexcept>
#include <map>


using Id = unsigned int;
using Age = unsigned int;
using Name = std::string_view;

typedef struct {
    Id id;
    Name name;
    Age age;
} Student;


class Students {
    private:
        std::map<Id, Student> students_;
    
    public:
        void add(Id id, Name name, Age age) {
            if (students_.find(id) == students_.end()) {
                students_.insert({id, Student{id, name, age}});
            } else {
                throw std::invalid_argument("Студент с таким ID: " + std::to_string(id) + " уже существует!");
            }
        }

        void remove(Id id) {
            students_.erase(id);
        }

        const Student& get_student_info(Id id) const {
            return students_.at(id);
        }
};


int main()
{
    Students students;

    students.add(0, "Fedya", 21);
    students.add(1, "Marysya", 25);
    students.add(2, "Rewit", 19);
    std::cout << students.get_student_info(0).name << std::endl;

    return 0;
}
