//Tamara Alhajj 100948027

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

int morph(char *number);

int main(int argc, char **argv)
{
	if(argc == 2)
	{
		FILE * binaryFile;
		binaryFile = fopen(argv[1], "rb");
		if(!binaryFile){
			printf("file %s does not exist\n", argv[1]);
			return -1;
		}else{
			int convert, numInt, size;
			char numChar[11]; //biggest number is 10 digits + NULL
			fread(&numInt, sizeof(unsigned int), 1, binaryFile);
			convert = fseek(binaryFile, (sizeof(unsigned int)) , SEEK_SET); //Seek from beginning of file
			if (convert != 0) return -1; //err with seek
			sprintf(numChar, "%u", numInt);
			printf("The first number is %s \n", numChar);
			
			//BONUS
			fseek(binaryFile, 0, SEEK_END);
			size = ftell(binaryFile);
			//size of both, hence size*2
			unsigned int *numArr = (unsigned int *) malloc(size*2);	
			//array of numbers and array of process ids	
			

			morph(numChar);
			return 1;
		}
	}else{
		//wrong number of parameters 
		printf("Usage: singlePrime fileName\n");
		return -1;	
	}
}

int morph(char *number)
{
	//Input: unsigned number to be checked
	//Output: morph itself to the isPrime program
	char *param[3];
	int status;
	param[0] = "overwrite";	//inconsequential
	param[1] = number;	//to be passed
	param[2] = NULL; //NULL-terminater
	int pid = fork();
	if (pid == 0){ 
		int rc = execv("./isPrime", param); //single child
		if (rc == -1) return -1; //err
	}
	else {
		wait(&status);	//child process id
		if (!WIFEXITED(status)) return -1; //child process NOT terminated normally
		int exit = WEXITSTATUS(status);	//get return value of isPrime
		if (exit == 1) printf("%s is a prime number\n", number);	
		else if(exit == 0) printf("%s is not a prime number\n", number);
		else return -1; //err
		return 1; //success
	}
}
