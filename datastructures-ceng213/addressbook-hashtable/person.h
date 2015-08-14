//
//  Person.h
//  CENG 213 - Fall 2012 HW3
//

#ifndef _Person_h
#define _Person_h
#include <string>
using namespace std;

class Person{
private:
    string name; // Invalid: ""
    int age;    // Invalid: age<18
    string homeTown;   // Invalid: ""
public:
    // Constructors and destructor
    Person();
    Person(string name, int age, string homeTown);
    Person(const Person& person);
    
    // Getters and setters
    string getName();
    void setName(string name);
    int getAge();
    void setAge(int age);
    string getHomeTown();
    void setHomeTown(string homeTown);
};
#endif
