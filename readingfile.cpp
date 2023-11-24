#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Student class definition
class Student {
protected:
    string studentID;
    string studentName;
    string tuitionFeesPaid;
    vector<string> coursesStudying;

public:
    // Constructor
    Student(string stuID, string stuName, string feesPaid, const vector<string>& courses)
        : studentID(stuID), studentName(stuName), tuitionFeesPaid(feesPaid), coursesStudying(courses) {}

    // Method to display student information
    void displayInfo() const {
        cout << "Student ID: " << studentID << "\tName: " << studentName << "\tTuition Fees Paid: " << tuitionFeesPaid
             << "\tCourses Studying: ";
        for (const auto& course : coursesStudying) {
            cout << course << " ";
        }
        cout << endl;
    }
};

// Function to read Student data from file
vector<Student> readStudentsFromFile(const string& filename) {
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
    return students;
}

int main() {
    // Read Student data from file
    vector<Student> students = readStudentsFromFile("Student.txt");
    cout << "Reading Students \n";
    for (const auto& stu : students) {
        stu.displayInfo();
    }
    return 0;
}
