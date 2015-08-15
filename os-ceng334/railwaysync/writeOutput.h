#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
enum DIRECTION {WEST, EAST};

pthread_mutex_t mutexWrite;// This mutex is used for atomic write operation
void writeOutput (int action, int trainCode, DIRECTION direction)// This function is used to write the results of simulation
{
	// Set the appropriate parameters for the corresponding output. Output order is important so be sure you call print function in the correct step.

	pthread_mutex_lock(&mutexWrite);

	if(action == 0)//This will be called before the enter function is called
	{
		printf ("Train = %d arrived the railway towards the %s\n",trainCode, (direction) ? "east" : "west");
	}
	if(action == 1)//This will be called when the train enters the railway
	{
		printf ("Train = %d entered the railway towards the %s\n",trainCode, (direction) ? "east" : "west");
	}
	else if(action == 2)//This will be called when the train leaves the railway
	{
		printf ("Train = %d exited the railway towards the %s\n",trainCode, (direction) ? "east" : "west");
	}

	pthread_mutex_unlock(&mutexWrite);
}
