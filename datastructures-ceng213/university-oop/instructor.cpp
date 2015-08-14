#include "university.h"

using namespace std ;

//---------------------------------------------------------------------------------------------------------------------------------------------------

Instructor::Instructor ( int id , string name )
{
  this->id   = id   ;
  this->name = name ;

  // initialize busyhours

  this->busyhours = new int * [DAYS_WORKING] ;

  for ( int i = 0 ; i < DAYS_WORKING ; i++ )
  {
    busyhours[i] = new int[HOURS_PER_DAY] ;
	for ( int j = 0 ; j < HOURS_PER_DAY ; j++ )
			busyhours[i][j] = 0 ;
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

Instructor::~Instructor ()
{
	for ( int i = 0 ; i < DAYS_WORKING ; i++ )
			delete [] busyhours[i] ;
		delete [] busyhours ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

string Instructor::getName ()
{
  return this->name ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

int Instructor::getID ()
{
  return this->id ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

int ** Instructor::getBusyHours ()
{
  return this->busyhours ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

vector<Offering*> Instructor::getOfferedCourses ()
{
  return this->offeredCourses ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void Instructor::offerNewCourse ( Offering * newOffering )
{
  // fill the hours of the offering

  for( int i = 0 ; i < newOffering->getDuration() ; i++ )
  {
		//check for conflict
		if( busyhours[newOffering->getDay()][newOffering->getWhichHour()+i] == 1 )
		{
			//check if already added
			for( unsigned int j = 0 ; j < offeredCourses.size() ; j++ )
			{
				if(offeredCourses[j]->getCourse()->getCourseCode() == newOffering->getCourse()->getCourseCode()
						&& offeredCourses[j]->getSection() == newOffering->getSection()
						&& offeredCourses[j]->getInstructor() == newOffering->getInstructor() )

					throw(ERR_OFFERING_ALREADY_ADDED);
			}


			//conflict! restore old values in busy hour array
			if(i != 0)
			{
				for( int k = 0 ; k < i; k++ )
				{
					busyhours[newOffering->getDay()][newOffering->getWhichHour()+k] = 0;
				}

			}
			//then throw exception
			throw(ERR_CONFLICT) ;
		}
		else
			this->busyhours[newOffering->getDay()][newOffering->getWhichHour()+i] = 1 ;
  }

  this->offeredCourses.push_back(newOffering) ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void Instructor::cancelOffering ( Offering * cancelledOffering )
{
	bool exists = false ;
	// empty the hours of the offering

	for ( int i = 0 ; i < cancelledOffering->getDuration() ; i++ )
	{
		this->busyhours[cancelledOffering->getDay()][cancelledOffering->getWhichHour()+i] = 0 ;
	}

	for ( unsigned int i = 0 ; i < offeredCourses.size() ; i++ )
	{
		if ( offeredCourses[i]->getCourse()->getCourseCode() == cancelledOffering->getCourse()->getCourseCode()
				&&	offeredCourses[i]->getSection() == cancelledOffering->getSection()
				&&  offeredCourses[i]->getInstructor() == cancelledOffering->getInstructor())
		{
			exists = true ;
			offeredCourses.erase( offeredCourses.begin()+i ) ;
		}
	}
	if( !exists )
			throw( ERR_OFFERING_DOES_NOT_EXIST ) ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

ostream & operator<< ( ostream & out , Instructor & instructor )
{
  out <<  "ID:" << instructor.getID() << "\t  NAME:" << instructor.getName() << "\t OFFERED_COURSES:" ;

  for ( unsigned int i = 0 ; i < instructor.getOfferedCourses().size() ; i++ )
  {
    out << *(instructor.getOfferedCourses()[i]) << "\t" ;
  }

  return out << endl ;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
