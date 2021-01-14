#include <iostream>
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
//EDF and RMS schedular Thomas Hackett tph1g17@soton.ac.uk
//Compilation : g++ schedular.cpp -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
#include <stdio.h>
#include <string.h>
#include <math.h>
const int width = 1080;
const int height = 900;

int GetPrime(int s);
int DisplayGfx(int NoTasks, int RMSDetails[100][10], int EDFDetails[100][10], int TimeMax);
int main(int argc, char *argv[])
{

	char buff[255];
	FILE *FPointer;
	char FName[20];
	int PFactors[100][100] = { 0 };
	int n;
	int count = 0;
	int EDFCheck[100][10] = { 0 };
	int RMSCheck[100][10] = { 0 };



	FPointer = fopen("TestText.txt", "r");

	fgets(buff, 255, (FILE*)FPointer);
	//printf("%s",buff);
	n = buff[9] - 48;
	float TaskArr[100][2]; // Tried this with 1 but caused funky stuff with array.
	while (count < n) {	//For loop loads array with key information for each task
		//printf(" %f ",count);
		fgets(buff, 255, (FILE*)FPointer);

		if (buff[9] == 0) {
			buff[9] = 10;
		}
		if (buff[7] == 32) {
			TaskArr[count][0] = buff[6] - 48;
		}
		else {
			TaskArr[count][0] = (buff[6] - 48) * 10 + buff[7] - 48;

		};

		if (buff[9] == 10) {
			TaskArr[count][1] = buff[8] - 48;
		}
		else {
			TaskArr[count][1] = (buff[8] - 48) * 10 + buff[9] - 48;

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
	for (count; count < n; count++) {

		SchedCheck = SchedCheck + TaskArr[count][0] / TaskArr[count][1];
		//printf("%f \n ",TaskArr[count][1]);	
	}

	if (SchedCheck > 1) {
		printf(" Not schedulable using EDF");
	}
	else {
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
		check = TaskArr[0][1] * TaskArr[1][1];
		printf(" %d ", check);
		//Now we have limit we can schedule the file using EDF.		
		/*EDF process:
			The task with the smallest deadline is assigned the highest priority,
			at any time the highest priority task is executed
		*/
		int DynamicDeadline[100][3];
		int missedCount = 0;
		count = 0;
		while (count < n) {
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

		while (count < check) {
			//Find lowest priority:
			for (int i = 0; i < n; i++) {
				if (DynamicDeadline[i][1] < DynamicDeadline[lowest][1]) {
					lowest = i;
					printf("Check i =%d", DynamicDeadline[lowest][1]);
				}
			}
			//Execute lowest task
			EDFCheck[count][lowest] = 1;
			printf("\nStuff for display:\n%d %d\n", EDFCheck[count][lowest],lowest);
			printf(" %d: Executing Task: %d\n ", count, lowest);

			DynamicDeadline[lowest][0] = DynamicDeadline[lowest][0] - 1;

			if (DynamicDeadline[lowest][0] == 0) {
				printf("%d: Task %d completed\n", count, lowest);
				EDFCheck[count][lowest] = 2;
				DynamicDeadline[lowest][2] = DynamicDeadline[lowest][2] + 1;
				DynamicDeadline[lowest][1] = TaskArr[lowest][1] * DynamicDeadline[lowest][2];
				//printf(" TaskArr value: %f ",TaskArr[lowest][1]);
				DynamicDeadline[lowest][0] = TaskArr[lowest][0];
			}



			for (int i = 0; i < n; i++) {
				float x;
				x = DynamicDeadline[i][1];
				printf("x is %f", x);
				if (DynamicDeadline[i][0] > 0 && x / (count + 1) == 1) {
					EDFCheck[count][i] = 3;
					printf("%d:Task %d missed deadling\n", count, i);
					DynamicDeadline[i][2] = DynamicDeadline[i][2] + 1;
					DynamicDeadline[i][1] = TaskArr[i][1] * DynamicDeadline[i][2];
					DynamicDeadline[i][0] = TaskArr[i][0];
					missedCount = missedCount + 1;

				}
			}

			count = count + 1;
		}
		printf("\n\n\n\n\n %d Missed Deadline count", missedCount);
		//RMS Schedular:
	}
	printf("\n\n\n\n\n\n-------------------------------------------------- \n \n \n \n \n Beginning RMS Schedular: \n\n\n\n\n");
	//RMS is a schedular which operates by performing the task with the smallest period first. 
	count = 0;
	int lowest = 0;
	int check = 0;
	int DynamicDeadline[100][3];
	check = TaskArr[0][1] * TaskArr[1][1];
	while (count < n) {
		DynamicDeadline[count][0] = TaskArr[count][0];
		DynamicDeadline[count][1] = TaskArr[count][1];
		DynamicDeadline[count][2] = 1;
		printf(" Check:%d ", DynamicDeadline[count][1]);
		count = count + 1;
	}

	count = 0;
	lowest = 0;
	for (int i = 0; i < n; i++) {
		if (DynamicDeadline[i][1] < DynamicDeadline[lowest][1]) {
			lowest = i;
			printf("Check i =%d", DynamicDeadline[lowest][1]);
		}
	}
	int TaskExecuted = lowest;
	int f = 0;
	int rem = 0;

	while (count < check) {

		if ((count%DynamicDeadline[lowest][1] == 0)) {
			printf("HERE");
			TaskExecuted = lowest;
			DynamicDeadline[0][0] = TaskArr[0][0];
			DynamicDeadline[0][1] = TaskArr[0][1];

		}
		//Sets lowest priority task to execute

		printf("%d: Task executing %d\n", count, TaskExecuted);
		RMSCheck[count][TaskExecuted] = 1;

		DynamicDeadline[TaskExecuted][0] = DynamicDeadline[TaskExecuted][0] - 1;
		if (DynamicDeadline[TaskExecuted][0] == 0 && DynamicDeadline[TaskExecuted][1] > 0) {
			printf("%d Task %d Completed \n", count, TaskExecuted);
			DynamicDeadline[count][TaskExecuted] = 2;
			RMSCheck[count][TaskExecuted] = 1;
			DynamicDeadline[TaskExecuted][2] = DynamicDeadline[TaskExecuted][2] + 1;
			for (int i = 0; i < n; i++) {

				if (DynamicDeadline[i][0] != 0) {
					if (DynamicDeadline[i][1] < DynamicDeadline[f][1]) {
						f = i;
						printf("HERE Bigman");
					}
				}
			}
			DynamicDeadline[TaskExecuted][0] = TaskArr[TaskExecuted][0];
			DynamicDeadline[TaskExecuted][1] = TaskArr[TaskExecuted][1];
			TaskExecuted = f;
		}
		for (int i = 0; i < n; i++) {
			if (count / (DynamicDeadline[i][2] * DynamicDeadline[i][1]) == 1) {
				printf("Deadline for task %d missed\n", i);
				RMSCheck[count][i] = 3;
				DynamicDeadline[i][2] = DynamicDeadline[i][2] + 1;
			}
		}

		count = count + 1;
	}

	DisplayGfx(n,RMSCheck,EDFCheck,check);
	return EXIT_SUCCESS;
}

int GetPrime(int s) {
	int count = 2;
	int max = 0;
	int maxR = 0;
	int i = 2;
	int n = 1001;
	int IsPrimes[1000] = { 0 };
	//Do algoritm
	while (count < n) {
		max = count / 2;
		//printf(" %d ",max);
		//printf(" %d ",maxR);
		while (i < max) {

			//printf(" n= %d ", count%i);
			if (count%i == 0) {
				i = max;
			}
			else {
				IsPrimes[count] = 1;
				printf(" count is = %d ", IsPrimes[count]);
			}
			i = i + 1;
		}
		IsPrimes[3] = 1;
		IsPrimes[2] = 1;
		IsPrimes[1] = 1;

		count = count + 1;
	}
	printf(" %d ", IsPrimes[s]);
	return IsPrimes[s];
	//Prime number help: https://www.educba.com/prime-numbers-in-c/ 		
}

int DisplayGfx(int NoTasks, int RMSDetails[100][10], int EDFDetails[100][10], int TimeMax) {
	int textW = 0;
	int textH = 0;
	printf("%d \n %d \n %d\n %d", NoTasks, RMSDetails[1][0], EDFDetails[2][0], TimeMax);

	TTF_Init();
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		//std::cout << "SDL could not initialise! SDL Error:" << SDL_GetError();
	};

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Color Black = { 0,0,0 };
	SDL_Color White = { 255,255,255 };
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color Blue = { 0, 0, 255 };
	SDL_Color Green = { 0, 255, 0 };
	TTF_Font* font = TTF_OpenFont("Font.ttf", 20);
	TTF_Font* font2 = TTF_OpenFont("Font2.ttf", 11);

	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "EDF", Black);
	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, "RMS", Black);
	SDL_Surface* surfaceMessage3 = TTF_RenderText_Solid(font, "Ticks", Black);
	SDL_Surface* surfaceMessage4 = TTF_RenderText_Solid(font, "Tasks", Black);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
	SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, surfaceMessage3);
	SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, surfaceMessage4);




	if (NULL == window)
	{
		std::cout << "Not working" << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}



	SDL_Event windowEvent;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);



	SDL_RenderPresent(renderer);


	//Dynamic X axis:
	int x = 0;
	while (x < 700 / TimeMax) {
		SDL_RenderDrawLine(renderer, 100, 450, 100 + (700 / TimeMax)*x, 450);
		SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*x, 448, 100 + (700 / TimeMax)*x, 452);
		x = x + 1;
	} //While loop sets X scale for graph and adds dashes corresponding to each tick.

	SDL_QueryTexture(texture3, NULL, NULL, &textW, &textH);
	SDL_Rect text3{ (700 / TimeMax)*x,460,textW,textH };
	SDL_RenderCopy(renderer, texture3, NULL, &text3);
	SDL_RenderPresent(renderer);
	//The 4 sdl lines above add a text label to the end of the x axis.

	//Dynamic Y axis:

	x = 1;


	std::string y;

	while (x <= NoTasks) {
		y = std::to_string(x);
		const char *ystring = y.c_str();
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font2, ystring, Black);
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		SDL_Rect text5{ 80,450 - 50 * x ,textW,textH };
		SDL_RenderCopy(renderer, texture, NULL, &text5);
		SDL_Rect text6{ 80,450 + 50 * x ,textW,textH };
		SDL_RenderCopy(renderer, texture, NULL, &text6);

		SDL_RenderPresent(renderer);
		x = x + 1;
	}

	SDL_QueryTexture(texture4, NULL, NULL, &textW, &textH);
	SDL_Rect text4{ 15,100 * x,textW,textH };
	SDL_RenderCopy(renderer, texture4, NULL, &text4);

	SDL_RenderDrawLine(renderer, 100, 450, 100, (100 * x));
	SDL_RenderDrawLine(renderer, 100, 450, 100, 300 + (100 * x));

	SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
	SDL_Rect text{ 250,450 - 50 * x,textW,textH };
	SDL_QueryTexture(texture2, NULL, NULL, &textW, &textH);
	SDL_Rect text2{ 250,470,textW,textH };
	SDL_RenderCopy(renderer, texture, NULL, &text);
	SDL_RenderCopy(renderer, texture2, NULL, &text2);
	// Dynamic Axis and labelling complete.
	int check = 0;
	int t = -1;
	int t2 = 3;
	printf("\n EDF check \n %d", EDFDetails[1][0]);

	//EDF Code
	while (t < 700 / TimeMax) {
		t = t + 1;
		while (check <= NoTasks) {
			if (check == 0) {
				if (EDFDetails[t][check] == 1) {
					//Draw red straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t+1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				if (EDFDetails[t][check] == 2) {
					//Draw red straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					//Draw X"
						
					SDL_RenderPresent(renderer);
				}
				if (EDFDetails[t][check] == 3) {
					//Draw red straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				

				
			}
			if (check == 1) {

				if (EDFDetails[t][check] == 1) {
					//Draw green straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 2), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 2));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t+1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				if (EDFDetails[t][check] == 2) {
					//Draw green straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 2), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 2));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);

				}
				if (EDFDetails[t][check] == 3) {
					//Draw green straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 2), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 2));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				//Draw blue straight line
				//SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
			}

			if (check == 2) {
				/*printf("incase2");
				if (EDFDetails[t][check] == 1) {
					//Draw red straight line
					//printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 2), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 2));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(m - 1), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);*/
			}
			//Draw green straight line 
			//SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
			check = check + 1;
		}
	check = 0;
	
}

//RMS Draw:
check = 0;
t = -1;
t2 = 3;

while (t < 700 / TimeMax) {
	t = t + 1;
	while (check <= NoTasks) {
		if (check == 0) {
			if (RMSDetails[t][check] == 1) {
				//Draw red straight line
				printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				if (t == 0) {
					SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				else {
					SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				SDL_RenderPresent(renderer);
			}
			if (RMSDetails[t][check] == 2) {
				//Draw red straight line
				printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				if (t == 0) {
					SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				else {
					SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				//Draw X"

				SDL_RenderPresent(renderer);
			}
			if (RMSDetails[t][check] == 3) {
				//Draw red straight line
				printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				if (t == 0) {
					SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				else {
					SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				SDL_RenderPresent(renderer);
			}



		}
		if (check == 1) {

			if (RMSDetails[t][check] == 1) {
				//Draw green straight line
				printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				if (t == 0) {
					SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				else {
					SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				SDL_RenderPresent(renderer);
			}
			if (RMSDetails[t][check] == 2) {
				//Draw green straight line
				printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				if (t == 0) {
					SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				else {
					SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				SDL_RenderPresent(renderer);

			}
			if (RMSDetails[t][check] == 3) {
				//Draw green straight line
				printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				if (t == 0) {
					SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 2));
				}
				else {
					SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(t), 455 + 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 + 50 * (check + 1));
				}
				SDL_RenderPresent(renderer);
			}
			//Draw blue straight line
			//SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		}

		if (check == 2) {
			/*printf("incase2");
			if (EDFDetails[t][check] == 1) {
				//Draw red straight line
				//printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				if (t == 0) {
					SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 2), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 2));
				}
				else {
					SDL_RenderDrawLine(renderer, 100 + (700 / TimeMax)*(m - 1), 455 - 50 * (check + 1), 100 + (700 / TimeMax)*(t + 1), 455 - 50 * (check + 1));
				}
				SDL_RenderPresent(renderer);*/
		}
		//Draw green straight line 
		//SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		check = check + 1;
	}
	check = 0;

}




//SDL closing
		SDL_RenderPresent(renderer);
		while (true)
		{
			if (SDL_PollEvent(&windowEvent))
			{
				if (SDL_QUIT == windowEvent.type)
				{
					break;
				}
			}
		}
		TTF_CloseFont(font);
		TTF_CloseFont(font2);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();


}


//Testing

//File handling https://www.tutorialspoint.com/cprogramming/c_file_io.htm


