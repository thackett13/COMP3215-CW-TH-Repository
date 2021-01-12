//EDF and RMS schedular Thomas Hackett tph1g17@soton.ac.uk


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics.h>

int GetPrime(int s);
void DisplayGfx();
 
int main()
{

	char buff[255];
	FILE *FPointer;
	char FName[20];
	int PFactors[100][100] = {0};
	int n;
	int count = 0;
	FPointer = fopen("TestText.txt","r");
	
	fgets(buff, 255, (FILE*)FPointer);
	//printf("%s",buff);
	n = buff[9] - 48;
	float TaskArr[n][2]; // Tried this with 1 but caused funky stuff with array.
	while(count < n){	//For loop loads array with key information for each task
		//printf(" %f ",count);
		fgets(buff, 255, (FILE*)FPointer);
		
		if(buff[9] == 0){
			buff[9] = 10;
		}
		if(buff[7] == 32){
			TaskArr[count][0] = buff[6]-48;
		}else{
			TaskArr[count][0] = (buff[6]-48)*10 + buff[7]-48;
			
		};
		
		if(buff[9] == 10){
			TaskArr[count][1] = buff[8]-48;
		}else{
			TaskArr[count][1] = (buff[8]-48)*10 + buff[9]-48;
			
		};
		
		printf(" %f ", TaskArr[count][0]);
		printf(" %f \n", TaskArr[count][1]);
		count = count + 1;
	}
	//printf(" %f \n",TaskArr[0][1]);	
	fclose(FPointer);
	//Algorithms:
	//EDF:
	float SchedCheck = 0;
	count = 0;
	//printf(" %f \n",TaskArr[0][1]);	
	for(count; count < n; count++){
	
		SchedCheck = SchedCheck + TaskArr[count][0]/TaskArr[count][1];
	//printf("%f \n ",TaskArr[count][1]);	
	}

	if( SchedCheck > 1){
		printf(" Not schedulable using EDF");
	}else{
	/*Do algorithm
		printf("Algorithmoid");
	//Find schedule time:
		int number = 0;
		int result = 0;
		int f = 0;
		int i;
		count = 0;
	while(count < n){
		printf(" %d ", count);
		number = TaskArr[count][0];
		printf(" %d ",number);
		if(GetPrime(number) == 1){
			printf(" IS Prime ");
		}else{
			// find prime factors
			printf(" Isn't Prime %d ",GetPrime(number));
			i = 2;
			while(number > 1){
				result = number%i;
				if(result == 0){
					PFactors[count][i] = PFactors[count][i] + 1;
					number= number/i;
				}else{
					while(f == 0){
						i = i+1;
						if(GetPrime(i) == 1){
							f = 1;
						}
					}
				}
			
			}
		}
		number = TaskArr[count][1];
		printf(" %d ",number);
		if(GetPrime(number) == 1){
			printf(" IS Prime ");
		}else{
			//Find prime factors
				i = 2;
				printf(" Isn't Prime2 %d ",GetPrime(number));
			while(number > 1){
				result = number%i;
				if(result == 0){
					PFactors[count+1][i] = PFactors[count+1][i] + 1;
					number= number/i;
				}else{
					while(f == 0){
						i = i+1;
						if(GetPrime(i) == 1){
							f = 1;
						}
					}
				}
			
			}
		}
		count = count + 1;
		printf("\n");
		}
		count = 0;
		int Multiply[100][100] = {0};
		
		i = 2;
		while(count < n){
			if(PFactors[count][i] >= PFactors[count+1][i]){
				Multiply[count][i] = PFactors[count][i];
			}else{
				Multiply[count][i] = PFactors[count+1][i];
			}
			//printf(" %d ",Multiply[count][i]);
			count = count + 1;*/
			
	//Multiply task times together.
	int check;
	check = TaskArr[0][1]*TaskArr[1][1];
	printf(" %d ",check); 
	//Now we have limit we can schedule the file using EDF.		
	/*EDF process:
		The task with the smallest deadline is assigned the highest priority,
		at any time the highest priority task is executed
	*/
	int DynamicDeadline[100][3];
	int missedCount = 0;
	count = 0;
	while(count < n){
		DynamicDeadline[count][0] = TaskArr[count][0];
		DynamicDeadline[count][1] = TaskArr[count][1];
		DynamicDeadline[count][2] = 1;
		printf(" Check:%d ", DynamicDeadline[count][1]);
		count = count + 1;
	
	}
	//printf(" check: %d ",DynamicDeadline[0][2]);
	
	printf("\n\n\n\n\n\n-------------------------------------------------- \n \n \n \n \n Beginning EDF Schedular: \n\n\n\n\n");
	
	count = 0;
	int lowest = 0;
	
	while(count < check){
		//Find lowest priority:
		for(int i =0; i<n;i++){
			if(DynamicDeadline[i][1] < DynamicDeadline[lowest][1]){
				lowest = i;
				printf("Check i =%d",DynamicDeadline[lowest][1]);
			}
		}
		//Execute lowest task
		printf(" %d: Executing Task: %d\n ", count,lowest);
		DynamicDeadline[lowest][0] = DynamicDeadline[lowest][0] - 1;
		
		if(DynamicDeadline[lowest][0] == 0){
			printf("%d: Task %d completed\n",count,lowest);
			DynamicDeadline[lowest][2] = DynamicDeadline[lowest][2] + 1;
			DynamicDeadline[lowest][1] = TaskArr[lowest][1]*DynamicDeadline[lowest][2];
			//printf(" TaskArr value: %f ",TaskArr[lowest][1]);
			DynamicDeadline[lowest][0] = TaskArr[lowest][0];
		}
		
		
		
		for(int i =0; i<n;i++){
			float x;
			x = DynamicDeadline[i][1];
			printf("x is %f",x);
			if(DynamicDeadline[i][0] > 0 && x/(count+1) == 1){
				
				printf("%d:Task %d missed deadling\n", count,i);
				DynamicDeadline[i][2] = DynamicDeadline[i][2] + 1;
				DynamicDeadline[i][1] = TaskArr[i][1]*DynamicDeadline[i][2];
				DynamicDeadline[i][0] = TaskArr[i][0];
				missedCount = missedCount + 1;
				
			}
		}

	count = count +1;
	}
	printf("\n\n\n\n\n %d Missed Deadline count",missedCount);
	//RMS Schedular:
		}
	printf("\n\n\n\n\n\n-------------------------------------------------- \n \n \n \n \n Beginning RMS Schedular: \n\n\n\n\n");
	//RMS is a schedular which operates by performing the task with the smallest period first. 
	count = 0;
	int lowest = 0;
	int check = 0;
	int DynamicDeadline[100][3];
	check = TaskArr[0][1]*TaskArr[1][1];
	while(count < n){
		DynamicDeadline[count][0] = TaskArr[count][0];
		DynamicDeadline[count][1] = TaskArr[count][1];
		DynamicDeadline[count][2] = 1;
		printf(" Check:%d ", DynamicDeadline[count][1]);
		count = count + 1;
	}
	
	count = 0;
	lowest = 0;
	for(int i =0; i<n;i++){
			if(DynamicDeadline[i][1] < DynamicDeadline[lowest][1]){
				lowest = i;
				printf("Check i =%d",DynamicDeadline[lowest][1]);
			}
	}
	int TaskExecuted = lowest;
	int f = 0;
	int rem = 0;
	while(count < check){
		
		if((count%DynamicDeadline[lowest][1] == 0)){
			printf("HERE");
			TaskExecuted = lowest;
			DynamicDeadline[0][0] = TaskArr[0][0];
			DynamicDeadline[0][1] = TaskArr[0][1];
			
		}
		//Sets lowest priority task to execute
		
		printf("%d: Task executing %d\n",count,TaskExecuted);
		DynamicDeadline[TaskExecuted][0] = DynamicDeadline[TaskExecuted][0] - 1;
		if(DynamicDeadline[TaskExecuted][0] == 0 && DynamicDeadline[TaskExecuted][1] > 0){
			printf("%d Task %d Completed \n",count,TaskExecuted);
			DynamicDeadline[TaskExecuted][1] = 100;
			DynamicDeadline[TaskExecuted][2] = DynamicDeadline[TaskExecuted][2] + 1;
			for(int i =0; i<n;i++){
				
				if(DynamicDeadline[i][0] != 0){
					if(DynamicDeadline[i][1] < DynamicDeadline[f][1]){
						f = i;
						printf("HERE Bigman");
					}
				}	
			}
			DynamicDeadline[TaskExecuted][0] = TaskArr[TaskExecuted][0];
			DynamicDeadline[TaskExecuted][1] = TaskArr[TaskExecuted][1];
			TaskExecuted = f;
		}
		for(int i =0; i<n;i++){
					if(count/(DynamicDeadline[i][2]*DynamicDeadline[i][1]) == 1){
						printf("Deadline for task %d missed\n",i);
						DynamicDeadline[i][2] = DynamicDeadline[i][2] + 1; 
					}
				}	
		
		count = count + 1;	
		}
		
	
	}
int GetPrime(int s){
	int count = 2;
	int max = 0;
	int maxR = 0;
	int i = 2;
	int n = 1001;
	int IsPrimes[1000] = {0};
		//Do algoritm
	while(count < n){
		max = count/2;
		//printf(" %d ",max);
		//printf(" %d ",maxR);
		while(i < max){
			
			//printf(" n= %d ", count%i);
			if(count%i == 0){
				i = max;
			}else{
				IsPrimes[count] = 1;
				printf(" count is = %d ",IsPrimes[count]);
			}
			i = i+ 1;
		}
		IsPrimes[3] = 1;
		IsPrimes[2] = 1;
		IsPrimes[1] = 1;
		
		count = count + 1;
	}
	printf(" %d ",IsPrimes[s]);
	return IsPrimes[s];
	//Prime number help: https://www.educba.com/prime-numbers-in-c/ 		
} 

void DisplayGfx(){
	initwindow(400, 300, "First Sample");
	circle(100,50,40);
}


//Testing

//File handling https://www.tutorialspoint.com/cprogramming/c_file_io.htm


