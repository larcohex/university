#include "university.h"

using namespace std;


Department::Department (string name)
{
	this->name = name;
}



string Department::getDeptName()
{
	return this->name;
}



vector <Instructor*> Department::getInstructors()
{
	return this->instructors;
}



vector <Student*> Department::getStudents()
{
	return this->students;
}



vector <Offering*> Department::getOfferings()
{
	return this->offeredCourses;
}




vector <Course*> Department::getCurriculum()
{
	return this->curriculum;
}



Instructor *Department::getInstructor (int id)
{
	Instructor *result = NULL;
	for (unsigned int i = 0; i < (this->getInstructors()).size(); i++)
	{
		if (((this->getInstructors())[i])->getID() == id)
		{
			result = (this->getInstructors())[i];
			break;
		}
	}
	return result;
}




Student *Department::getStudent (int id)
{
	Student *result = NULL;
	for (unsigned int i = 0; i < (this->getStudents()).size(); i++)
	{
		if ((this->getStudents())[i]->getID() == id)
		{
			result = (this->getStudents())[i];
			break;
		}
	}
	return result;
}






Course *Department::getCourse (int code)
{
	Course *result = NULL;
	for (unsigned int i = 0; i < (this->getCurriculum()).size(); i++)
	{
		if ((this->getCurriculum())[i]->getCourseCode() == code)
		{
			result = (this->getCurriculum())[i];
			break;
		}
	}
	return result;
}








Offering *Department::getOffering (int code, int section)
{
	unsigned int result;
	for (unsigned int i = 0; i < (this->getOfferings()).size(); i++)
	{
		if (((this->getOfferings())[i])->getCourse()->getCourseCode() == code && ((this->getOfferings())[i])->getSection() == section)
		{
			result = i ; 
			break;
		}
	}
	return  (this->getOfferings())[result];
}







void Department::addInstructor (Instructor *newIns)
{
	for (unsigned int i = 0; i < (this->getInstructors()).size(); i++)
	{
		if ((this->getInstructors())[i] == newIns)
		{
			throw (ERR_INSTRUCTOR_ALREADY_EXISTS);
		}
	}
	this->instructors.push_back(newIns);
}






void Department::removeInstructor (int oldInsID)
{
	unsigned int i, j;
	int ins = -1; 
	vector <Instructor*>::iterator it;
	for (i = 0, it = (this->getInstructors()).begin(); it < (this->getInstructors()).end(); i++, it++)
	{
		if ((this->getInstructors())[i]->getID() == oldInsID)
		{		
			ins = i;
			break;
		}
	}
	if (ins != -1)
	{
		for (i = 0; i < (this->getStudents()).size(); i++)
		{
			for (j = 0; j < (((this->instructors)[ins])->getOfferedCourses()).size(); j++)
			{
				try
				{
					(this->students)[i]->dropCourse((((this->instructors)[ins])->getOfferedCourses())[j]);
				}
				catch (SystemError e)
				{
				}
			}
		}
		for (i = 0; i < (this->getOfferings()).size(); i++)
		{
			for (j = 0; j < (((this->instructors)[ins])->getOfferedCourses()).size(); j++)
			{
				if ((this->getOfferings())[i] == (((this->instructors)[ins])->getOfferedCourses())[j])
				{
					(this->offeredCourses).erase ((this->offeredCourses).begin() + i);
				}
			}
		}
		(((this->instructors)[ins])->getOfferedCourses()).clear();
		delete (this->instructors)[ins];
		(this->instructors).erase (this->instructors.begin() + ins);
	}
}






void Department::addStudent (Student *newStds)
{
	for (unsigned int i = 0; i < (this->getStudents()).size(); i++)
	{
		if ((this->getStudents())[i] == newStds)
		{
			throw (ERR_STUDENT_ALREADY_EXISTS);
		}
	}
	(this->students).push_back(newStds);
}






void Department::removeStudent (int gradutedStdID)
{
	unsigned int i, j;
	int student = -1;
	vector <Student*>::iterator it;
	for (i = 0, it = (this->getStudents()).begin(); it < (this->getStudents()).end(); i++, it++)
	{
		if ((this->getStudents())[i]->getID() == gradutedStdID)
		{
			student = i;
			break;
		}
	}
	if (student != -1)
	{
		for (i = 0; i < (this->getOfferings()).size(); i++)
		{
			try
			{
				(this->students)[student]->dropCourse((this->offeredCourses)[i]);
			}
			catch (SystemError e)
			{
			}
		}
		(this->students).erase((this->students).begin() + student);
	}
}








void Department::offerNewCourse (Offering *no)
{
	for (unsigned int i = 0; i < (this->getOfferings()).size(); i++)
	{
		if ((this->getOfferings())[i] == no)
		{
			throw (ERR_OFFERING_ALREADY_EXISTS);
		}
	}
	(this->offeredCourses).push_back(no);
}










void Department::cancelOffering (Offering *co)
{
	unsigned int i;
	vector <Offering*>::iterator it;
	for (i = 0, it = (this->getOfferings()).begin(); it < (this->getOfferings()).end(); i++, it++)
	{
		if ((this->getOfferings())[i] == co)
		{
			(this->offeredCourses).erase(it);
			break;
		}
	}
	if (it == (this->getOfferings()).end())
	{
		throw (ERR_OFFERING_DOES_NOT_EXIST);
	}
}









void Department::addCourseToCurriculum (Course *curr)
{
	for (unsigned int i = 0; i < (this->getCurriculum()).size(); i++)
	{
		if ((this->getCurriculum())[i] == curr)
		{
			throw (ERR_COURSE_ALREADY_EXISTS);
		}
	}
	(this->curriculum).push_back(curr);
}









ostream &operator<< (ostream & out , Department & dept)
{
	unsigned int i;
	out << dept.getDeptName() << endl;
	out << "Instructors" << endl;
	for (i = 0; i < (dept.getInstructors()).size(); i++)
	{
		out << *((dept.getInstructors())[i]);
	}
	out << "All courses:" << endl;
	for (i = 0; i < (dept.getCurriculum()).size(); i++)
	{
		out << *((dept.getCurriculum())[i]);
	}
	out << "Offerings:" << endl;
	for (i = 0; i < (dept.getOfferings()).size(); i++)
	{
		out << *((dept.getOfferings())[i]) << endl;
	}
	out << "Students:" << endl;
	for (i = 0; i < (dept.getStudents()).size(); i++)
	{
		out << *((dept.getStudents())[i]);
	}
	out << endl;
	return out;
}
