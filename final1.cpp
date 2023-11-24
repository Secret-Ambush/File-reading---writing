#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Base class Employee
class Employee {
public:
    string employeeID;
    string employeeName;
    string pay;

    Employee() {}

    Employee(string empID, string empName, string pay) {
        employeeID = empID;
        employeeName = empName;
        this->pay = pay;
    }

    // Method to display employee information
    void displayInfo() const {
        cout << "Employee ID: " << employeeID << "\tName: " << employeeName << "\tPay: " << pay << endl;
    }
};

// Student class definition
class Student {
public:
    string studentID;
    string studentName;
    string tuitionFeesPaid;
    vector<string> coursesStudying;

    // Constructor
    Student() {}

    Student(string stuID, string stuName, string feesPaid, const vector<string>& courses)
        : studentID(stuID), studentName(stuName), tuitionFeesPaid(feesPaid), coursesStudying(courses) {}

    // Method to display student information
    void displayInfo() const {
        cout << studentID << " " << studentName << " " << tuitionFeesPaid
            << " ";
        for (const auto& course : coursesStudying) {
            cout << course << " ";
        }
        cout << endl;
    }

    void displayInfo(std::ofstream& outputFile) const {
        if (outputFile.is_open()) {
            // Output student information to the file
            outputFile << studentID << " " << studentName << " " << tuitionFeesPaid << " ";
            for (const auto& course : coursesStudying) {
                outputFile << course << " ";
            }
            outputFile << std::endl;
        } else {
            std::cerr << "File stream is not open." << std::endl;
        }
    }
};

// Derived class Instructor from Employee
class Instructor : public Employee {
public:
    vector<string> coursesTeaching;

    Instructor() {}

    Instructor(string empID, string empName, string pay, const vector<string>& courses)
        : Employee(empID, empName, pay), coursesTeaching(courses) {}

    // Method to display instructor information including courses they teach
    void displayInfo() const {
        Employee::displayInfo();
        cout << "Courses Teaching: ";
        for (const auto& course : coursesTeaching) {
            cout << course << " ";
        }
        cout << endl;
    }
};

class TeachingAssistant : public Student, public Instructor {
public:
    TeachingAssistant(string empID, string empName, string pay, const string& feesPaid, const vector<string>& coursesStudying, const vector<string>& coursesTeaching)
        : Student(empID, empName, feesPaid, coursesStudying), Instructor(empID, empName, pay, coursesTeaching) {}

    void displayInfo() const {
        Student::displayInfo();
        Instructor::displayInfo();
    }
};

// Function to read Employee data from file
vector<Employee> getEmpData(const string& filename, int& size) {
    vector<Employee> employees;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file: " << filename << endl;
        return employees;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string empID, empName, payStr;
        if (ss >> empID >> empName >> payStr) {
            Employee emp(empID, empName, payStr);
            employees.push_back(emp);
        }
    }

    file.close();

    size = employees.size();
    return employees;
}
// Function to read Student data from file
vector<Student> getStudData(const string& filename, int& size) 
{
    vector<Student> students;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file: " << filename << endl;
        return students;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string stuID, stuName, feesPaidStr, course;
        vector<string> courses;

        if (ss >> stuID >> stuName >> feesPaidStr) {
            // Extracting feesPaid directly as a string
            Student stu(stuID, stuName, feesPaidStr, courses);

            // Read courses
            while (ss >> course) {
                courses.push_back(course);
            }
            // Assign courses to student object
            stu = Student(stuID, stuName, feesPaidStr, courses);
            students.push_back(stu);
        }
    }

    file.close();

    size = students.size();
    return students;
}

vector<Instructor> getInstData(const string& instructorFilename, const string& employeeFilename) {
    vector<Instructor> instructors;

    // Read employee details to map IDs to names
    unordered_map<string, string> employeeNames;
    ifstream empFile(employeeFilename);
    if (empFile.is_open()) {
        string line;
        while (getline(empFile, line)) {
            stringstream ss(line);
            string empID, empName, pay;
            if (ss >> empID >> empName >> pay) {
                employeeNames[empID] = empName;
            }
        }
        empFile.close();
    } else {
        cout << "Unable to open employee file: " << employeeFilename << endl;
        return instructors;
    }

    // Read instructor details including courses teaching
    ifstream file(instructorFilename);
    if (!file.is_open()) {
        cout << "Unable to open file: " << instructorFilename << endl;
        return instructors;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string empID, course;
        if (ss >> empID >> course) {
            vector<string> courses{ course };
            while (ss >> course) {
                courses.push_back(course);
            }
            if (employeeNames.find(empID) != employeeNames.end()) {
                string empName = employeeNames[empID];
                Instructor inst(empID, empName, "", courses);
                instructors.push_back(inst);
            }
        }
    }

    file.close();
    return instructors;
}

vector<TeachingAssistant> getTAData(const vector<Student>& students, const vector<Instructor>& instructors) {
    vector<TeachingAssistant> teachingAssistants;

    for (const auto& student : students) {
        for (const auto& instructor : instructors) {
            if (student.studentID == instructor.employeeID) {
                TeachingAssistant ta(
                    student.studentID,
                    student.studentName,
                    instructor.pay,
                    student.tuitionFeesPaid,
                    student.coursesStudying,
                    instructor.coursesTeaching
                );
                teachingAssistants.push_back(ta);
                break;
            }
        }
    }

    return teachingAssistants;
}

int main() {
    int employeeSize = 0, studentSize = 0, instructorSize = 0;
    std::ofstream outputFile("output.txt");

    cout << "Reading Employees File \n";
    // Read Employee data from file
    vector<Employee> employee_arr = getEmpData("Employee.txt", employeeSize);    

    cout << "Reading Students File \n";
    // Read Student data from file
    vector<Student> student_arr = getStudData("student.txt", studentSize);

    cout << "Reading Instructors File \n";
    // Read Instructor data from file
    vector<Instructor> instructors = getInstData("instructor.txt", "Employee.txt");

    vector<TeachingAssistant> teachersAssistants = getTAData(student_arr, instructors);

    cout << "Vectors ready ...  Creating Output file" << endl << endl;
    cout << "File name: output.txt" << endl;

    // Question 1
    std::cout << "Question 1" << std::endl;
    for (const auto& emp : employee_arr) {
        std::cout << emp.employeeID << " " << emp.pay << std::endl;
    }

    outputFile << "Question 1" << endl;
    for (const auto& emp : employee_arr) {
        outputFile << emp.employeeID << " " << emp.pay << std::endl;
    }

    // Question 2
    std::cout << std::endl;
    outputFile << std::endl;
    std::cout << "Question 2" << std::endl;
    outputFile << "Question 2" << std::endl;
    cout << "STUDENT_ID STUDENT_NAME COURSE_TEACHING" << std::endl;
    outputFile << "STUDENT_ID STUDENT_NAME COURSE_TEACHING" << std::endl;
    for (const auto& stud : teachersAssistants) {
        std::cout << stud.studentID << " " << stud.studentName << " ";
        for (const auto& course : stud.coursesTeaching) {
            std::cout << course << " ";
        }
        std::cout << std::endl;

        outputFile << stud.studentID << " " << stud.studentName << " ";
        for (const auto& course : stud.coursesTeaching) {
            outputFile << course << " ";
        }
        outputFile << std::endl;
    }


    // Question 3
    std::cout << std::endl;
    outputFile << std::endl;
    std::cout << "Question 3" << std::endl;
    outputFile << "Question 3" << std::endl;
    for (const auto& student : student_arr) {
        bool isTA = false;
        for (const auto& ta : teachersAssistants) {
            if (student.studentID == ta.studentID) {
                isTA = true;
                break;
            }
        }
        if (!isTA) {
            std::cout << student.studentID << " " << student.studentName << std::endl;
            outputFile << student.studentID << " " << student.studentName << std::endl;
        }
    }

    // Question 4
    std::cout << std::endl;
    outputFile << std::endl;
    std::cout << "Question 4" << std::endl;
    outputFile << "Question 4" << std::endl;
    for (const auto& student : student_arr) {
        student.displayInfo();
        student.displayInfo(outputFile);
    }


    // Close the output file stream
    outputFile.close();

    return 0;
}

