#include "university.h"

using namespace std;





Schedule::Schedule()
{
	this->weeklySchedule = new OfferingPair [DAYS_WORKING];
	for (unsigned int i = 0; i < DAYS_WORKING; i++)
	{
		(this->weeklySchedule)[i].nextOff = NULL;
		(this->weeklySchedule)[i].off = NULL;
	}
}




OfferingPair * Schedule::getSchedule()
{
	return this->weeklySchedule;
}







Offering *Schedule::getCurrentOffering (int courseCode)
{
	OfferingPair *temp = new OfferingPair;
	for (unsigned int i = 0; i < DAYS_WORKING; i++)
	{
		for (temp = &((this->weeklySchedule)[i]); temp->nextOff; temp = temp->nextOff)
		{
			if (temp->nextOff->off->getCourse()->getCourseCode() == courseCode)
			{
				return temp->nextOff->off;
			}
		}
	}
	return NULL;
}



void Schedule::addOffering (Offering *o)
{
	int d = o->getDay();
	OfferingPair *newNode = new OfferingPair;
	OfferingPair *temp = new OfferingPair;		
	for (temp = &(this->weeklySchedule[d]); temp->nextOff != NULL; temp = temp->nextOff)
	{
		if (temp->nextOff->off->getCourse()->getCourseCode() == o->getCourse()->getCourseCode() && temp->nextOff->off->getSection() == o->getSection())
		{
			throw (ERR_OFFERING_ALREADY_ADDED);
		}	
	}
	newNode->off = o;
	newNode->nextOff = NULL;
	temp->nextOff = newNode;

	for (bool sorted = false; sorted == false;)
	{
		bool check = true;
		temp = &(this->weeklySchedule[d]);
		if (temp->nextOff->nextOff)
		{
			for (temp = &(this->weeklySchedule[d]); temp->nextOff->nextOff; temp = temp->nextOff)
			{
				if (temp->nextOff->off->getWhichHour() > temp->nextOff->nextOff->off->getWhichHour())
				{
					check = false;
					break;
				}
			}
		}
		if (check)
		{
			sorted = true;
		}
		else
		{
			for (temp = &(this->weeklySchedule[d]); temp->nextOff->nextOff; temp = temp->nextOff)
			{
				if (temp->nextOff->off->getWhichHour() > temp->nextOff->nextOff->off->getWhichHour())
				{
					OfferingPair *temp2 = new OfferingPair;
					OfferingPair *temp3 = new OfferingPair;
					temp2 = temp->nextOff;
					temp3 = temp->nextOff->nextOff;
					temp2->nextOff = temp3->nextOff;
					temp->nextOff = temp3;
					temp3->nextOff = temp2;
				}
			}
		}
	}
}




void Schedule::removeOffering (Offering *o)
{
	OfferingPair *temp = new OfferingPair;
	bool result = false;
	if ((this->getSchedule()+o->getDay())->nextOff)
	{
		for (OfferingPair *temp = &((this->weeklySchedule)[o->getDay()]); temp->nextOff; temp = temp->nextOff)
		{
			if (temp->nextOff->off == o )
			{
				temp->nextOff = temp->nextOff->nextOff;
				result = true;
				break;
			}
		}
	}
	if (result == false)
	{
		throw (ERR_OFFERING_DOES_NOT_EXIST);
	}
}





ostream &operator<< (ostream &out, Schedule &schedule)
{
	out << endl;
	for (unsigned int i = 0; i < DAYS_WORKING; i++)
	{
		if ((schedule.getSchedule())[i].nextOff)
		{
			OfferingPair *temp = new OfferingPair;
			switch (i)
			{
				case (0):
				{
					out << "Mon:";
					for (temp = &((schedule.getSchedule())[i]); temp->nextOff; temp = temp->nextOff)
					{
						out << temp->nextOff->off->getWhichHour() << "-" << temp->nextOff->off->getWhichHour() + temp->nextOff->off->getDuration() - 1;
						out << ":" << *(temp->nextOff->off)<<"\t";
					}
					out << endl;
					break;
				}
				case (1):
				{
					out << "Tue:";
					for (temp = &((schedule.getSchedule())[i]); temp->nextOff; temp = temp->nextOff)
					{
						out << temp->nextOff->off->getWhichHour() << "-" << temp->nextOff->off->getWhichHour() + temp->nextOff->off->getDuration() - 1;
						out << ":" << *(temp->nextOff->off)<<"\t";

					}
					out << endl;
					break;
				}
				case (2):
				{
					out << "Wed:";
					for (temp = &((schedule.getSchedule())[i]); temp->nextOff; temp = temp->nextOff)
					{
						out << temp->nextOff->off->getWhichHour() << "-" << temp->nextOff->off->getWhichHour() + temp->nextOff->off->getDuration() - 1;
						out << ":" << *(temp->nextOff->off)<<"\t";
					}
					out << endl;
					break;
				}
				case (3):
				{
					out << "Thu:";
					for (temp = &((schedule.getSchedule())[i]); temp->nextOff; temp = temp->nextOff)
					{
						out << temp->nextOff->off->getWhichHour() << "-" << temp->nextOff->off->getWhichHour() + temp->nextOff->off->getDuration() - 1;
						out << ":" << *(temp->nextOff->off)<<"\t";
					}
					out << endl;
					break;
				}
				case (4):
				{
					out << "Fri:";
					for (temp = &((schedule.getSchedule())[i]); temp->nextOff; temp = temp->nextOff)
					{
						out << temp->nextOff->off->getWhichHour() << "-" << temp->nextOff->off->getWhichHour() + temp->nextOff->off->getDuration() - 1;
						out << ":" << *(temp->nextOff->off)<<"\t";
					}
					out << endl;
					break;
				}
			}
		}
	}
	return out;
}
