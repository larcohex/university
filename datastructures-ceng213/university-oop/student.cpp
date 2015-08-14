#include "university.h"

using namespace std;


Student::Student (int id, string name)
{
	this->id = id;
	this->name = name;
	this->busyhours = new int * [DAYS_WORKING];
	for ( int i = 0 ; i < DAYS_WORKING ; i++ )
	{
		busyhours[i] = new int[HOURS_PER_DAY] ;
		for ( int j = 0 ; j < HOURS_PER_DAY ; j++ )
		{
			busyhours[i][j] = 0 ;
		}
  	}
}



Student::~Student()
{
	for ( int i = 0 ; i < DAYS_WORKING ; i++ )
	{
		delete [] busyhours[i];
	}
	delete [] busyhours;
}



string Student::getName()
{
	return this->name;
}



int Student::getID()
{
	return this->id;
}




vector <int> Student::getPrevCourses()
{
	return this->prevTakenCourses;
}




int **Student::getBusyHours()
{
	return this->busyhours;
}





Schedule *Student::getSchedule()
{
	return &(this->schedule);
}







void Student::addPrevCourse (int PrevCourseCode)
{
	(this->prevTakenCourses).push_back(PrevCourseCode);
}





int Student::addCourse (Offering *newOffering)
{
	unsigned int i, j;
	if (!((newOffering->getCourse()->getPreReqCourseCodes()).empty()))
	{
		for (i = 0; i < (newOffering->getCourse()->getPreReqCourseCodes()).size(); i++)
		{
			bool taken = false;
			for (j = 0; j < (this->getPrevCourses()).size(); j++)
			{
				if ((this->getPrevCourses())[j] == (newOffering->getCourse()->getPreReqCourseCodes())[i])
				{
					taken = true;
					break;
				}
			}
			if (!taken)
			{
				throw (ERR_PREREQISITE_IS_NOT_TAKEN_YET);
			}
		}
	}
	for (i = 0; i < (newOffering->getDuration()); i++)
	{
		int d = newOffering->getDay();
		int h = newOffering->getWhichHour();
		int c = newOffering->getCourse()->getCourseCode();
		if ((this->getBusyHours())[d][h+i] == 1)
		{
			if (i != 0)
			{
				for (unsigned int k = 0; k < (newOffering->getDuration()); k++)
				{
					(this->busyhours)[d][h+k] = 0;
				}
			}
			throw (ERR_CONFLICT);
		}
		else
		{
			if ((this->getSchedule())->getCurrentOffering(c))
			{
				throw (ERR_OFFERING_ALREADY_ADDED);
			}
			else
			{
				(this->busyhours)[d][h+i] = 1;
			}
		}
	}
	(this->schedule).addOffering(newOffering);
	return 1;
}





void Student::dropCourse (Offering *newOffering)
{
	int c = newOffering->getCourse()->getCourseCode();
	if (this->schedule.getCurrentOffering(c))
	{
		int d = newOffering->getDay();
		int h = newOffering->getWhichHour();
		(this->schedule).removeOffering(newOffering);
		for (unsigned int i = 0; i < (newOffering->getDuration()); i++)
		{
			(this->busyhours)[d][h+i] = 0;
		}
	}
	else
	{
		throw (ERR_OFFERING_DOES_NOT_EXIST);
	}
}





bool Student::changeSection(int courseCode, int newSection, vector <Offering*> Offerings)
{
	bool result = false;
	if (this->getSchedule()->getCurrentOffering(courseCode))
	{
		for (unsigned int i = 0; i < Offerings.size(); i++)
		{
			if (Offerings[i]->getCourse()->getCourseCode()==courseCode && Offerings[i]->getSection() == newSection)
			{
				this->dropCourse (this->getSchedule()->getCurrentOffering(courseCode));
				this->addCourse (Offerings[i]);
				result = true;
				break;	
			}
		}
	}
	return result;
}




ostream &operator<< (ostream &out, Student &student)
{
	out << "ID:" << student.getID() << "\t" << "NAME:" << student.getName() << "\t" << "PREV_COURSE_CODES:";
	if (student.getPrevCourses()[0])
	{
		out << student.getPrevCourses()[0];
		for (unsigned int i = 1; i < student.getPrevCourses().size(); i++)
		{
			out << "-" << student.getPrevCourses()[i];
		}
	}
	out << *(student.getSchedule()) << endl;
	return out;
}
