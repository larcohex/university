//
//  AddressBook.h
//  HW3
//
//  CEng 213 Fall 2012, METU
//
//  Barney's address book, a hash table holding people and phone numbers.
//
#ifndef _AddressBook_h
#define _AddressBook_h
#include "person.h"
#include <iostream>
class AddressBook{
private:
    // members & methods
    class List
    {
	private:
		struct Node
		{
			Node()
			{
				this->next = NULL;
			}
			Person person;
			string phoneNumber;
			Node *next;
		};
		Node *head;
		Node *tail;
	public:
		List()
		{
			this->head = new Node;
			this->head->next = NULL;
			this->tail = this->head;
		}
		~List()
		{
			Node *node = this->head;
			while (node)
			{
				Node *temp = node;
				node = node->next;
				delete temp;
				temp = NULL;
			}
			this->head = NULL;
			this->tail = NULL;
		}
		void pushBack (Person &person, string phoneNumber)
		{
			Node *newNode = new Node;
			newNode->person = person;
			newNode->phoneNumber = phoneNumber;
			this->tail->next = newNode;
			this->tail = newNode;
		}
		void pop (Person &person)
		{
			Node *prev = findPrevious (person);
			if (prev)
			{
				Node *newNext = prev->next->next;
				delete prev->next;
				prev->next = newNext;
				if (!prev->next)
				{
					this->tail = prev;
				}
			}
		}
		Node *findPrevious (Person &person)
		{
			Node *temp;
			for (temp = this->head; temp->next; temp = temp->next)
			{
				if (temp->next->person.getName() == person.getName() && temp->next->person.getAge() == person.getAge() && temp->next->person.getHomeTown() == person.getHomeTown())
				{
					return temp;
				}
			}
			if (!temp)
			{
				return NULL;
			}
		}
		int count()
		{
			int result = 0;
			for (Node *temp = this->head; temp->next; temp = temp->next)
			{
				++result;
			}
			return result;
		}
		string number (Person &person)
		{
			Node *location = findPrevious (person);
			if (location)
			{
				return location->next->phoneNumber;
			}
			else
			{
				return "";
			}
		}
		int order (Person &person)
		{
			int counter = 0;
			for (Node *temp = this->head; temp->next; temp = temp->next)
			{
				if (temp->next->person.getName() == person.getName() && temp->next->person.getAge() == person.getAge() && temp->next->person.getHomeTown() == person.getHomeTown())
				{
					return counter;
				}
				else
				{
					++counter;
				}
			}
			return -1;
		}
    };
    List *hashTable;
    int tableSize;
public:
    // Constructors & Destructor. Careful about memory leaks.
    AddressBook(); // Default table size: 151.
    // numberOfBuckets is a prime number.
    AddressBook(int numberOfBuckets);
    ~AddressBook();
    
    // Hash a person to a non-negative integer (a hash value 
    // independent of total number of buckets). 
    // use hashValue (mod numberOfBuckets) as bucket number.
    // Return -1 on invalid parameters.
    int hashPerson(Person& person);
    
    // Put a person to the hash table, along with the phone number.
    // Do nothing on invalid parameters.
    // Invalid phoneNumber : ""
    void addEntry(Person& person, string phoneNumber);
    
    // Remove the entry from the address book.
    // Do nothing on invalid parameters.
    void removeEntry(Person& person);
    
    // Get the total number of people in a specified bucket.
    // Return -1 on invalid bucket number.
    int getNumberOfPeopleInBucket(int bucket);
    
    // Get phone number of a person.
    // Try to improve speed of this function (and quality of 
    // hashPerson() function).
    // For invalid parameters, return empty string.
    string getPhoneNumber(Person& person);



    List *getHashTable();
};
#endif
