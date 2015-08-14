#include "addressbook.h"


bool isPerson (Person &person)
{
	if (person.getName() == "" || person.getAge() < 18 || person.getHomeTown() == "")
	{
		return false;
	}
	else
	{
		return true;
	}
}



/*
		FNV1A_MEIYAN HASH FUNCTION
		I don't have rights for the following hash function.
		All acclaims are for Georgiy Marinov a.k.a. Sanmayce.
		For further information:
		http://www.sanmayce.com/Fastest_Hash/
*/
		


unsigned int hashMeiyan (string word)
{
	const unsigned int prime = 709607;
	unsigned int hash = 2166136261;
	const char *str = word.c_str();
	unsigned int len = word.size();
	for(; len >= 2 * sizeof (unsigned int); len = len - 2 * sizeof (unsigned int), str = str + 2 * sizeof(unsigned int))
	{
		hash = prime * (hash ^ (((*(unsigned int *)(str) << 5) | (*(unsigned int *)(str) >> 27)) ^ (*(unsigned int *)(str + 4))));        
	}
	if (len & sizeof (unsigned int))
	{
		hash = prime * (hash ^ (*(unsigned short *)(str)));
		str = str + sizeof (unsigned short);
		hash = prime * (hash ^ (*(unsigned short *)(str)));
		str = str + sizeof (unsigned short);
	}
	if (len & sizeof (unsigned short))
	{
		hash = prime * (hash ^ (*(unsigned short *)(str)));
		str = str + sizeof (unsigned short);
	}
	if (len & 1)
	{ 
		hash = prime * (hash ^ *str);
	}
	return hash ^ (hash >> 16);
}


/*----------------------------------------------------------------*/


AddressBook::AddressBook()
{
	this->hashTable = new List [151];
	this->tableSize = 151;
}




AddressBook::AddressBook (int numberOfBuckets)
{
	this->hashTable = new List [numberOfBuckets];
	this->tableSize = numberOfBuckets;
}




AddressBook::~AddressBook()
{
	delete [] this->hashTable;
}





int AddressBook::hashPerson (Person &person)
{
	if (!isPerson (person))
	{
		return -1;
	}
	else
	{
		int hash;
		unsigned int hash1, hash2;
		hash1 = hashMeiyan (person.getName());
		hash2 = hashMeiyan (person.getHomeTown());
		hash = hash1 + hash2 + person.getAge();
		return hash;
	}
}











void AddressBook::addEntry (Person &person, string phoneNumber)
{
	if (isPerson (person) && !(phoneNumber.empty()))
	{
		unsigned int hash = hashPerson (person);
		int key = hash % this->tableSize;
		this->hashTable[key].pushBack (person, phoneNumber);
	}
}









void AddressBook::removeEntry (Person &person)
{
	if (isPerson (person))
	{
		unsigned int hash = hashPerson (person);
		int key = hash % this->tableSize;
		this->hashTable[key].pop (person);
	}
}






int AddressBook::getNumberOfPeopleInBucket(int bucket)
{
	if (bucket >= this->tableSize || bucket < 0)
	{
		return -1;
	}
	else
	{
		return this->hashTable[bucket].count();
	}
}







string AddressBook::getPhoneNumber (Person &person)
{
	if (!isPerson (person))
	{
		return "";
	}
	else
	{
		unsigned int hash = hashPerson (person);
		int key = hash % this->tableSize;
		return this->hashTable[key].number (person);
	}
}







AddressBook::List *AddressBook::getHashTable()
{
	return this->hashTable;
}
