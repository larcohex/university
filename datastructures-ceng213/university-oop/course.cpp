#include "university.h"

using namespace std;



int searchPreReqCourse (vector <int> array, int courseCode)
{
	int result = 0;
	int size = array.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (array[i] == courseCode)
		{
			result = 1;
		}
	}
	return result;
}




Course::Course(int code)
{
	this->code = code;
}




int Course::getCourseCode()
{
	return this->code;
}



vector <int> Course::getPreReqCourseCodes()
{
	return this->preReqCourseCodes;
}




void Course::addPreReqCourseCodes (int preCode)
{
	if (!searchPreReqCourse (this->getPreReqCourseCodes(), preCode))
	{
		this->preReqCourseCodes.push_back (preCode);
	}
	else
	{
		throw (ERR_PREREQISITE_ALREADY_EXISTS);
	}
}



ostream &operator<< (ostream &out, Course &course)
{
	out << course.getCourseCode() << ":" ;
	if (!((course.getPreReqCourseCodes()).empty()))
	{
		out << course.getPreReqCourseCodes()[0];
		for (unsigned int i = 1; i < course.getPreReqCourseCodes().size(); i++)
		{
			out << "-" << (course.getPreReqCourseCodes())[i];
		}
		out << endl;
	}
	else
	{
		out << endl;
	}
	return out;
}
