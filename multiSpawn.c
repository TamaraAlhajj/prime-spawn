//Tamara Alhajj 100948027

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

//CHANGED FROM OTHER TASKS: no longer forking in morph because we now have multiple spawns.
//SO forking is now done in main

int morph(char *number);

int main(int argc, char **argv)
{
	if(argc >= 2)
	{
		FILE * binaryFile;
		
		int convert, i, check, status, childProcessIds[10];
		char numChar[11]; //biggest number is 10 digits + NULL
		unsigned int numbers[10];
		int pid=0, cpid=0;
		
		binaryFile = fopen(argv[1], "rb");
		if(!binaryFile){
			printf("file %s does not exist \n", argv[1]);
			return -1;
		}

		for(i=0; i<10; i++){
			convert = fseek(binaryFile, 4*i , SEEK_SET); //Seek from beginning of file	
			if (convert < 0) return -1; //err with seek
			fread(&(numbers[i]), sizeof(unsigned int), 1, binaryFile); //read 10 ints
		}
		pid = fork(); //child
		for(i=0; i<10; i++){			
			if(pid == 0){
				check = sprintf(numChar, "%u", numbers[i]);
				if(check <= 0) return -1; //err
				morph(numChar);
				return -1; //if here, err
			}else{
				if(pid) childProcessIds[i] = pid;
				pid = fork(); //child	
			}
		}
		while(cpid != -1){
			if(pid) cpid = waitpid(-1, &status, 0);
			if(cpid == -1) break;
			if(WIFEXITED(status)){  
				int exit = WEXITSTATUS(status);	//get return value of isPrime
				if (exit == 0){ 
					for(i=0; i<10; i++){ 
						if(cpid == childProcessIds[i]) break;
					}
					printf("%u is NOT a prime number\n", numbers[i]);
				}else{
					for(i=0; i<10; i++){ 
						if(cpid == childProcessIds[i]) break;
					}	
					printf("%u is a prime number\n", numbers[i]);										
				}
			}
		}
		fclose(binaryFile);
		return 1;
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

	param[0] = "overwrite";	//inconsequential
	param[1] = number;	//to be passed
	param[2] = NULL; //NULL-terminater

	int rc = execv("./isPrime", param); //single child
	if (rc == -1) return -1; //err
	return 1;
}

