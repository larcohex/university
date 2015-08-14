#include "person.h"


Person::Person()
{
	this->name = "noname";
	this->age = 0;
	this->homeTown = "notown";
}




Person::Person (string name, int age, string homeTown)
{
	this->name = name;
	this->age = age;
	this->homeTown = homeTown;
}



Person::Person (const Person& person)
	:name (person.name),
	 age (person.age),
	 homeTown (person.homeTown)
{
}




string Person::getName()
{
	return this->name;
}




void Person::setName (string name)
{
	this->name = name;
}






int Person::getAge()
{
	return this->age;
}




void Person::setAge (int age)
{
	this->age = age;
}




string Person::getHomeTown()
{
	return this->homeTown;
}



void Person::setHomeTown (string homeTown)
{
	this->homeTown = homeTown;
}
