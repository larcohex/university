#include "writeOutput.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;


sem_t enterLock, exitLock;
vector <pthread_t *> threads;
vector <pthread_cond_t *> conds;
vector <pthread_mutex_t *> locks;

class Rail
{
	private:
		int *limit;
		DIRECTION direction;
		vector <int> east;
		vector <int> west;
		vector <int> in;
		bool empty;
		bool allow;
	public:
		Rail (int _limit);
		void Enter (DIRECTION d, int trainCode);
		void Exit (DIRECTION d, int trainCode);
};


Rail :: Rail (int _limit)
{
	this->limit = new int;
	*(this->limit) = _limit;
	this->empty = true;
	this->allow = true;
}


void Rail :: Enter (DIRECTION d, int trainCode)
{
	if (this->empty)
	{	
		sem_wait (&enterLock);
			writeOutput (1, trainCode, d);
			this->direction = d;
			this->empty = false;
			this->in.push_back (trainCode);
		sem_post (&enterLock);
	}
	else if (d != this->direction)
	{
		if (d == EAST)
		{
			this->east.push_back (trainCode);
		}
		else
		{
			this->west.push_back (trainCode);
		}
		pthread_cond_wait (conds[trainCode - 1], locks[trainCode - 1]);
		this->Enter (d, trainCode);
	}
	else
	{
		if ((d == EAST && this->west.empty()) || (d == WEST && this->east.empty()))
		{
			sem_wait (&enterLock);
				this->empty = false;
				writeOutput (1, trainCode, d);
				this->in.push_back (trainCode);
			sem_post (&enterLock);
		}
		else if (this->in.size() >= *(this->limit) || !this->allow)
		{
			if (d == EAST)
			{
				this->east.push_back (trainCode);
			}
			else
			{
				this->west.push_back (trainCode);
			}
			pthread_cond_wait (conds[trainCode - 1], locks[trainCode - 1]);
			this->Enter (d, trainCode);
		}
		else
		{
			sem_wait (&enterLock);
				writeOutput (1, trainCode, d);
				this->direction = d;
				this->empty = false;
				this->in.push_back (trainCode);
				if (this->in.size() >= *(this->limit))
				{
					this->allow = false;
				}
			sem_post (&enterLock);
		}
	}
}


void Rail :: Exit (DIRECTION d, int trainCode)
{
	writeOutput (2, trainCode, d);
	this->in.erase (this->in.begin());
	if (this->in.empty())
	{
		this->empty = true;
		this->allow = true;
		if (d == EAST)
		{
			if (this->west.size() <= *(this->limit))
			{
				for (int i = 0; i < this->west.size(); ++i)
				{
					pthread_cond_signal (conds[this->west[i] - 1]);
				}
				this->west.erase (this->west.begin(), this->west.begin() + this->west.size());
			}
			else
			{
				for (int i = 0; i < *(this->limit); ++i)
				{
					pthread_cond_signal (conds[this->west[i] - 1]);
				}
				this->west.erase (this->west.begin(), this->west.begin() + *(this->limit));
			}
		}
		else
		{
			if (this->east.size() <= *(this->limit))
			{
				for (int i = 0; i < this->east.size(); ++i)
				{
					pthread_cond_signal (conds[this->east[i] - 1]);
				}
				this->east.erase (this->east.begin(), this->east.begin() + this->east.size());
			}
			else
			{
				for (int i = 0; i < *(this->limit); ++i)
				{
					pthread_cond_signal (conds[this->east[i] - 1]);
				}
				this->east.erase (this->east.begin(), this->east.begin() + *(this->limit));
			}
		}
		if (this->east.empty() && !this->west.empty())
		{
			for (int i = 0; i < this->west.size(); ++i)
			{
				pthread_cond_signal (conds[this->west[i] - 1]);
			}
			this->west.clear();
		}
		else if (!this->east.empty() && this->west.empty())
		{
			for (int i = 0; i < this->east.size(); ++i)
			{
				pthread_cond_signal (conds[this->east[i] - 1]);
			}
			this->east.clear();
		}
	}
}

struct Train
{
	int arrTime;
	int code;
	DIRECTION direction;
	int duration;
	Rail *rail;
};

vector <Train *> trains;

void *train (void *arg)
{
	Train *train = (Train *)(arg);
	writeOutput (0, train->code, train->direction);
	train->rail->Enter (train->direction, train->code);
	sem_wait (&exitLock);
		usleep (train->duration * 10000);
		train->rail->Exit (train->direction, train->code);
	sem_post (&exitLock);
	
}

int main (int argc, char** argv)
{
	int limit = strtol (argv[1], NULL, 10);
	Rail *rail  = new Rail (limit);
	string par;
	int temp = 0;
	int arrTime, code, duration;
	DIRECTION direction;
	sem_init (&enterLock, 0, 1);
	sem_init (&exitLock, 0, 1);
	while (cin >> par)
	{
		switch (temp)
		{
			case 0:
				arrTime = strtol (par.c_str(), NULL, 10);
				++temp;
				break;
			case 1:
				code = strtol (par.c_str(), NULL, 10);
				++temp;
				break;
			case 2:
				if (par == "EAST")
				{
					direction = EAST;
				}
				else
				{
					direction = WEST;
				}
				++temp;
				break;
			case 3:
				duration = strtol (par.c_str(), NULL, 10);
				Train *newTrain = new Train;
				newTrain->arrTime = arrTime;
				newTrain->code = code;
				newTrain->direction = direction;
				newTrain->duration = duration;
				newTrain->rail = rail;
				trains.push_back (newTrain);
				pthread_mutex_t *newLock = new pthread_mutex_t;
				pthread_mutex_init (newLock, 0);
				locks.push_back (newLock);
				pthread_cond_t *newCond = new pthread_cond_t;
				pthread_cond_init (newCond, 0);
				conds.push_back (newCond);
				usleep (newTrain->arrTime * 1000);
				pthread_t *newThread = new pthread_t;
				threads.push_back (newThread);
				pthread_attr_t attr; 
				pthread_attr_init (&attr);
				pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
 				pthread_create (newThread, &attr, train, (void *)newTrain);
				temp = 0;
				break;
		}
	}
	for (int i = 0; i < threads.size(); ++i)
	{
		pthread_join (*(threads[i]), 0);
		pthread_exit ((void *)(&threads));
	}
	for (int i = 0; i < locks.size(); ++i)
	{
		pthread_mutex_destroy (locks[i]);
		delete locks[i];
	}
	for (int i = 0; i < conds.size(); ++i)
	{
		pthread_cond_destroy (conds[i]);
		delete conds[i];
	}
	sem_destroy (&enterLock);
	sem_destroy (&exitLock);
	for (int i = 0; i < threads.size(); ++i)
	{
		delete threads[i];
	}
	for (int i = 0; i < trains.size(); ++i)
	{
		delete trains[i];
	}
	pthread_exit (NULL);
}