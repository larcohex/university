#include "university.h"

using namespace std;




Offering::Offering (Course *course, int section, Instructor * lec, int day, int hour, int dur)
{
	this->course = course;
	this->section = section;
	this->lecturer = lec;
	this->day = day;
	this->whichHour = hour;
	this->duration = dur;
	lecturer->offerNewCourse(this);
}





Course *Offering::getCourse()
{
	return this->course;
}




int Offering::getSection()
{
	return this->section;
}



int Offering::getDay()
{
	return this->day;
}



int Offering::getWhichHour()
{
	return this->whichHour;
}


int Offering::getDuration()
{
	return this->duration;
}



Instructor *Offering::getInstructor()
{
	return this->lecturer;
}




void Offering::setDay (int d)
{
	this->day = d;
}




void Offering::setWhichHour (int wh)
{
	this->whichHour = wh;
}




void Offering::setDuration (int dur)
{
	this->duration = dur;
}




void Offering::setInstructor (Instructor *ins)
{
	this->lecturer = ins;
}




ostream &operator<< (ostream &out, Offering &offering)
{
	string::iterator it = offering.getInstructor()->getName().begin();
	unsigned int i = 0;
	out << (offering.getCourse())->getCourseCode() << "-" << offering.getSection() << "-";
	for (; i < offering.getInstructor()->getName().size(); )
	{
		for (; offering.getInstructor()->getName()[i] == ' ' || offering.getInstructor()->getName()[i] == '\t'; i++)
		{
			if (i == offering.getInstructor()->getName().size())
			{
				break;
			}
		}
		out << offering.getInstructor()->getName()[i];
		for (; offering.getInstructor()->getName()[i] != ' ' && offering.getInstructor()->getName()[i] != '\t'; i++)
		{
			if (i == offering.getInstructor()->getName().size())
			{
				break;
			}
		}
	}
	return out;
}
