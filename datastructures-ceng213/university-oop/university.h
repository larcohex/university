//***************************************************************************

#ifndef UNIVERSITY_H
#define UNIVERSITY_H

//***************************************************************************

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

//***************************************************************************

#define DAYS_WORKING  5
#define HOURS_PER_DAY 12

//***************************************************************************

using namespace std ;

//***************************************************************************

enum SystemError
{
  ERR_STUDENT_ALREADY_EXISTS       = 1 ,
  ERR_INSTRUCTOR_ALREADY_EXISTS    = 2 ,
  ERR_COURSE_ALREADY_EXISTS        = 3 ,
  ERR_OFFERING_ALREADY_EXISTS      = 4 ,
  ERR_OFFERING_ALREADY_ADDED       = 5 ,
  ERR_OFFERING_IS_NOT_TAKEN 	   = 6 ,
  ERR_OFFERING_DOES_NOT_EXIST      = 7 ,
  ERR_PREREQISITE_IS_NOT_TAKEN_YET = 8 ,
  ERR_PREREQISITE_ALREADY_EXISTS   = 9 ,
  ERR_CONFLICT                     = 10

} ;

class Course     ;
class Instructor ;
class Offering   ;
class Schedule   ;
class Student    ;
class Department ;

struct OfferingPair
{
  Offering     * off     ;
  OfferingPair * nextOff ;

} ;



//***************************************************************************

// Course

class Course
{
  public:

    Course ( int code ) ; // constructor

    // methods
    int         getCourseCode        () ;
    vector<int> getPreReqCourseCodes () ;

    void addPreReqCourseCodes ( int preCode ) ;

  private:

    int         code              ;
    vector<int> preReqCourseCodes ;

  friend ostream & operator<< ( ostream & out ,  Course & course ) ;
} ;

//***************************************************************************

// Instructor

class Instructor
{
  public:

    Instructor ( int id , string name ) ;// constructor
   ~Instructor ()                       ;

    // methods
    string               getName           () ;
    int                  getID             () ;
    int               ** getBusyHours      () ;
    vector<Offering*>    getOfferedCourses () ;

    void offerNewCourse 	( Offering * newOffering       ) ; 
    void cancelOffering 	( Offering * cancelledOffering ) ; 

  private:

    int                  id             ;
    string               name           ;
    int               ** busyhours      ;
    vector<Offering*>    offeredCourses ;

  friend ostream & operator<< ( ostream & out , Instructor & instructor ) ;
} ;



//***************************************************************************

// Offering

class Offering
{
  public:

    Offering ( Course     * course  = NULL ,
               int          section = 1    ,
               Instructor * lec     = NULL ,
               int          day     = 0    ,
               int          hour    = 0    ,
               int          dur     = 1    ) ; // constructor

    Course     * getCourse     () ;
    int          getSection    () ;
    int          getDay        () ;
    int          getWhichHour  () ;
    int          getDuration   () ;
    Instructor * getInstructor () ;

    void setDay        ( int          d   ) ;
    void setWhichHour  ( int          wh  ) ;
    void setDuration   ( int          dur ) ;
    void setInstructor ( Instructor * ins ) ;

  private:

    Course     * course    ;
    int          section   ;
    Instructor * lecturer  ;
    int          day       ;
    int          whichHour ;
    int          duration  ;

  friend ostream & operator<< ( ostream & out , Offering & offering ) ;
} ;

//***************************************************************************

// Schedule

class Schedule
{
  public:

    Schedule () ;

    OfferingPair * getSchedule        ()                 ;
    Offering     * getCurrentOffering ( int courseCode ) ;

    void addOffering    ( Offering * o ) 		;
    void removeOffering ( Offering * o ) 		;

  	private:

    OfferingPair * weeklySchedule ;

  friend ostream & operator<< ( ostream & out , Schedule & schedule ) ;
} ;

//***************************************************************************

// Student

class Student
{
  public:

    Student ( int id , string name ) 	;
   ~Student ()                     		;

    // methods
    string         getName        () ;
    int            getID          () ;
    vector<int>    getPrevCourses () ;
    int         ** getBusyHours   () ;
    Schedule    *  getSchedule    () ;

    void addPrevCourse ( int prevCourseCode )		;
    int  addCourse     ( Offering * newOffering )   ;
    void dropCourse    ( Offering * newOffering )   ; 
    bool changeSection ( int courseCode , 
						 int newSection , 
						 vector<Offering*> Offerings ) ;

  private:

    int            id               ;
    string         name             ;
    vector<int>    prevTakenCourses ;
    int         ** busyhours        ;
    Schedule       schedule         ;

  friend ostream & operator<< ( ostream & out , Student & student ) ;
} ;

//***************************************************************************

// Department

class Department
{
  public:

    Department ( string name ) ;

    // methods
    string                getDeptName    ()                         ;
    vector<Instructor*>   getInstructors ()                         ;
    vector<Student*>      getStudents    ()                         ;
    vector<Offering*>     getOfferings   ()                         ;
    vector<Course*>       getCurriculum  ()                         ;
    Instructor          * getInstructor  ( int id )                 ;
    Student             * getStudent     ( int id )                 ;
    Course              * getCourse      ( int code )               ;
    Offering            * getOffering    ( int code , int section ) ;

    void addInstructor         ( Instructor * newIns        ) ;
    void removeInstructor      ( int          oldInsID      ) ;
    void addStudent            ( Student    * newStds       ) ;
    void removeStudent         ( int          gradutedStdID ) ;
    void offerNewCourse        ( Offering   * no            ) ; 
    void cancelOffering        ( Offering   * co            ) ; 
    void addCourseToCurriculum ( Course     * curr          ) ;

  private:

    string              name           ;
    vector<Instructor*> instructors    ;
    vector<Student*>    students       ;
    vector<Offering*>   offeredCourses ; 
    vector<Course*>     curriculum     ; 

  friend ostream & operator<< ( ostream & out , Department & dept ) ;
} ;

//***************************************************************************

#endif

//***************************************************************************
