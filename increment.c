//increment.c - Program which increments the user inputted number of htreads by a thousand
//Author: Alicja Gornicka

# include <getopt.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <pthread.h>

int counter; //counter
pthread_mutex_t lock; //mutex

//arguements for the threads
struct threadArgs{
	int count;
};

//function to make sure the thread is safe and does not run into a race condition
void threadSafe()
{
        pthread_mutex_lock(&lock); //locks mutex
        counter++; //counter is incremented
        pthread_mutex_unlock(&lock); //unlocks mutex
	return;
}

//function to increment the thread
void *incrementThread(void *sArgs)
{
	struct threadArgs *a = (struct threadArgs*)sArgs;
	int count = a->count; 

	for (int i = 0; i < count; i++) //increments it by the count
	{
		threadSafe();
	}

	return NULL;
}

int checkFlag(int a, char **x)
{
	int n; //thread count
	int flags; //checks for command line inputs
	int c; //used for getopt
	
	while ((c = getopt(a,x,"n:")) != -1)
	{
		if (c == 'n')
		{
			flags = 1;
			n = atoi(optarg); //gets the number of threads from the user
		}	
	}
	
	if (flags != 1) //checks to make sure a valid option was inputted
	{
		printf("Usage: ./increment -n <number of threads>\n");
		return -1;
	}
	else if (n < 0) //makes sure thread count is greater than zero
	{
		printf("Thread count must be greater than zero\n");
		return -1;
	}
	
	
	return n; //returns the thread count
}

int main(int argc, char **argv)
{
	int n = checkFlag(argc,argv); //n is the number of threads
	
	//initialize threads and args and dynamically allocate memory
	pthread_attr_t attribute; //googled to find out how this is used
	pthread_t *threads = malloc(sizeof(*threads) * n);
	struct threadArgs *sArgs = malloc(sizeof(struct threadArgs) * n);

	//initialize attributes
	pthread_attr_init(&attribute);
	
	//initialize Mutex
	pthread_mutex_init(&lock,NULL);
	
	//Set arguments and create threads
	for (int i = 0; i < n; i++)
	{
		sArgs[i].count = 1000; //n is incremented a thousand times
		pthread_create(&threads[i], &attribute, incrementThread, (void *)(sArgs + i)); 
	}

	//join threada after each one is created and executes its instructions
	for (int i = 0; i < n; i++)
	{
		pthread_join(threads[i], NULL);
	}

	//destroy mutex
	pthread_mutex_destroy(&lock);
	
	//print the number of threads * 1000
	printf("%d\n", counter);	

	//free allocated memory
	free(sArgs);
	free(threads);
	
	return 0;
}
