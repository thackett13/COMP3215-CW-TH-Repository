#include <stdio.h>
#include <string.h>
#include <math.h>
int GetPrime(int s);
 
int main()
{

char buff[255];
FILE *FPointer;
char FName[20];
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
printf(" %f \n",TaskArr[0][1]);	
for(count; count < n; count++){
	
	SchedCheck = SchedCheck + TaskArr[count][0]/TaskArr[count][1];
	printf("%f \n ",TaskArr[count][1]);	
}

if( SchedCheck > 1){
	printf(" Not schedulable using EDF");
}else{
	//Do algorithm
	printf("Algorithmoid");
	//Find schedule time:
	int n = 6;
	int out; 
	out = GetPrime(n); 
	printf("Is prime: %d",out);
}
}
int GetPrime(int s){
	int count = 2;
	int max = 0;
	int i = 0;
	int n = 1001;
	int IsPrimes[1000] = {0};
		//Do algoritm
	while(count < n){
		max = count/2;	
		for(i = 2; i<= max; i++){
			if(count%i == 0){
				i = max+1;
			}else{
				IsPrimes[count] = 1;
			}
		}
		count = count + 1;
	}
	printf("%d",s);
	return IsPrimes[s];
			
} 
//Testing

//File handling https://www.tutorialspoint.com/cprogramming/c_file_io.htm
