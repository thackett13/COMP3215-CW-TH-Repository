//EDF and RMS schedular Thomas Hackett tph1g17@soton.ac.uk
#include <iostream>
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <numeric>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int width = 1080;
const int height = 900;
const int scaler = 15;
const int array_size = 3000;

void DrawX(SDL_Renderer *&renderer, int TimeMax, int t, int check, int RMSCheck);
void DrawT(SDL_Renderer *&renderer, int TimeMax, int t, int check, int RMSCheck);

void DisplayGfx(int NoTasks, int RMSDetails[array_size][10], int EDFDetails[array_size][10], int TimeMax);
int main(int argc, char *argv[])
{

	char buff[255];
	FILE *FPointer;
	int n;
	int count = 0;
	int EDFCheck[array_size][10] = { 0 };
	int RMSCheck[array_size][10] = { 0 };
	int LCM1, LCM2 = 1;


	FPointer = fopen("TestText.txt", "r"); //Open file used to load tasks

	fgets(buff, 255, (FILE*)FPointer);
	n = buff[9] - 48; 
	//The two lines above loads the first line of the file into the buffer and sets n to the asicc value of the number of tasks 
	float TaskArr[array_size][2]; //Declaration for large array with taks held inside
	while (count < n) {	//For loop loads array with key information for each task
		//printf(" %f ",count);
		fgets(buff, 255, (FILE*)FPointer);//Gets 1 line from the file. 

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
		printf(" %f \n", TaskArr[count][1]); //
		count = count + 1;
	} //While loop counts trhough the number of tasks, loading the parameters for each tasks into TaskArray TaskArr.

	
	fclose(FPointer);
	FPointer = fopen("SchedularOutput.txt","w"); //opens the file for the schedularoutput in writemode.
	if (FPointer == NULL) {
		printf("File not open");
	}
	//Algorithms:
	//EDF:
	float SchedCheck = 0;
	count = 0;
	//printf(" %f \n",TaskArr[0][1]);	
	for (count; count < n; count++) {

		SchedCheck = SchedCheck + TaskArr[count][0] / TaskArr[count][1];
		printf("%f", SchedCheck);
			
	} //Loops through the number of tasks summing the divisions of each execution time by deadline.

	int check = 0;//Check value stores the LCM total for the schedular.
	LCM1 = (int)TaskArr[0][1];
	LCM2 = (int)TaskArr[1][1];
	check = std::lcm(LCM1, LCM2); // Sets check to equal the value of lcm for the tasks.
	if (n > 2) {
		for (int i = 2; i < n; i++) {
			check = std::lcm(check, (int)TaskArr[i][1]);
		}
	}//For loops finds the LCM of n number of tasks to allow for the y axis value to be found for schedulling.
	double t = 0;//T variable used to hold result for SchedCheck to compare its result against.

	if (SchedCheck > 1) {
		printf(" Not schedulable using EDF");
		fprintf(FPointer, " Not schedulable using EDF");

	} 
	else {
		
	
		
		//https://www.calculatorsoup.com/calculators/math/lcm.php useful for lcm properties.
		printf(" %d ", check);
		
		//Now we have limit we can schedule the file using EDF.		
		/*EDF process:
			The task with the smallest deadline is assigned the highest priority,
			at any time the highest priority task is executed
		*/
		int DynamicDeadline[array_size][3];
		int missedCount = 0;
		count = 0;
		while (count < n) {
			DynamicDeadline[count][0] = TaskArr[count][0];
			DynamicDeadline[count][1] = TaskArr[count][1];
			DynamicDeadline[count][2] = 1; //Multiplier used to find deadline for subsequent executions of each task.
			printf(" Check:%d ", DynamicDeadline[count][1]);
			count = count + 1;

		} //While loops loops through the number of tasks and dynamicDeadline array its values required for each task ; where DynamicDeadline[count][0] stores execution time of taskArr[count][0] and so on for each task.
		//printf(" check: %d ",DynamicDeadline[0][2]);

		printf("\n\n\n\n\n\n-------------------------------------------------- \n \n \n \n \n Beginning EDF Schedular: \n\n\n\n\n");
		fprintf(FPointer, " Beginning EDF Schedular: \n "); //Writes to file that the EDF schedular is starting.
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
			EDFCheck[count][lowest] = 1;//Stores state which task is being executed for use by graphics function.
			printf("\nStuff for display:\n%d %d\n", EDFCheck[count][lowest],lowest);
			printf(" %d: Executing Task: %d\n ", count, lowest);
			//Consol outputs for bug fixing

			fprintf(FPointer, " %d: Executing Task: %d\n ", count, lowest); //Writes to file which Task is being executed.

			DynamicDeadline[lowest][0] = DynamicDeadline[lowest][0] - 1; //Decriments the execution time

			if (DynamicDeadline[lowest][0] == 0) {
				printf("%d: Task %d completed\n", count, lowest);
				fprintf(FPointer, " %d: Task %d completed\n", count, lowest);

				EDFCheck[count][lowest] = 2;
				DynamicDeadline[lowest][2] = DynamicDeadline[lowest][2] + 1;
				DynamicDeadline[lowest][1] = TaskArr[lowest][1] * DynamicDeadline[lowest][2];
				//printf(" TaskArr value: %f ",TaskArr[lowest][1]);
				DynamicDeadline[lowest][0] = TaskArr[lowest][0];
			} //If statement checks if a task is completed, if it is will assign a new deadline to the task and reset it



			for (int i = 0; i < n; i++) { //For loop checks if any deadlines have been missed.
				float x;
				x = DynamicDeadline[i][1];
				printf("x is %f", x);
				if (DynamicDeadline[i][0] > 0 && x / (count + 1) == 1) {
					EDFCheck[count][i] = 3;
					printf("%d:Task %d missed deadling\n", count, i);
					fprintf(FPointer, " %d:Task %d missed deadling\n", count, i); //Writes to output file the program has missed.

					DynamicDeadline[i][2] = DynamicDeadline[i][2] + 1;
					DynamicDeadline[i][1] = TaskArr[i][1] * DynamicDeadline[i][2];
					DynamicDeadline[i][0] = TaskArr[i][0];

					missedCount = missedCount + 1; //increments missed counter 

				} //If statement checks the execution time is greater than zero and checks if the current deadline is equal to the value fo the nextdeadline, if this is the case then the task has 
				//missed.
			}

			count = count + 1;
		}
		printf("\n\n\n\n\n %d Missed Deadline count", missedCount);
		fprintf(FPointer, " \n\n\n\n\n %d Missed Deadline count", missedCount);
		//RMS Schedular:
	}

		printf("\n\n\n\n\n\n-------------------------------------------------- \n \n \n \n \n Beginning RMS Schedular: \n\n\n\n\n");
		//RMS is a schedular which operates by performing the task with the smallest period first. 
		fprintf(FPointer, " \n\n\n\n\n RMS Schedular: \n\n\n\n\n");

		count = 0;
		int missedCount = 0;
		int lowest = 0;
		int DynamicDeadline[array_size][3]; //Used to track the state of the values of the task 


		while (count < n) {
			DynamicDeadline[count][0] = TaskArr[count][0];
			DynamicDeadline[count][1] = TaskArr[count][1];
			DynamicDeadline[count][2] = 1;
			printf(" Check:%d ", DynamicDeadline[count][1]);
			count = count + 1;
		}//While loop loads the dynamicDeadline array again with fresh values fom the TaskArr;


		count = 0;
		lowest = 0;
		for (int i = 0; i < n; i++) {
			if (DynamicDeadline[i][1] < DynamicDeadline[lowest][1]) {
				lowest = i;
				printf("Check i =%d", DynamicDeadline[lowest][1]);
			}
		}//For loop finds the smallest deadline and makes this the highest priority task.
		int TaskExecuted = lowest;
		int f = 0;
	
		while (count < check) {

			if ((count + 1) % (int)TaskArr[lowest][1] == 0) {
				printf("HERE");
				TaskExecuted = lowest;
				DynamicDeadline[lowest][0] = TaskArr[lowest][0];
				DynamicDeadline[lowest][1] = TaskArr[lowest][1];
			}//If statement sets the highest priority task to execute each time it is required to be executed.

			//Sets lowest priority task to execute

			printf("%d: Task executing %d\n", (count), TaskExecuted);
			fprintf(FPointer, " %d: Task executing %d\n", (count), TaskExecuted);


			RMSCheck[count][TaskExecuted] = 1; //Stores indicator for a task being executed at this tisk for use in the graphics section.

			DynamicDeadline[TaskExecuted][0] = DynamicDeadline[TaskExecuted][0] - 1; //Decrements execution period to track progress 

			if (DynamicDeadline[TaskExecuted][0] == 0 && DynamicDeadline[TaskExecuted][1] > 0) {
				printf("%d Task %d Completed \n", (count), TaskExecuted);
				fprintf(FPointer, " %d Task %d Completed \n", (count), TaskExecuted);

				RMSCheck[count][TaskExecuted] = 2;//Stores completed task value in array used by graphics.
				DynamicDeadline[TaskExecuted][0] = TaskArr[TaskExecuted][0]; //Resets execution time for completed task.
				DynamicDeadline[TaskExecuted][2] = DynamicDeadline[TaskExecuted][2] + 1;//Increments task deadline multiplier
				DynamicDeadline[TaskExecuted][1] = DynamicDeadline[TaskExecuted][2] * DynamicDeadline[TaskExecuted][1];//Sets new deadline
				for (int i = 0; i < n; i++) {

					if (DynamicDeadline[i][0] != 0) {
						if (i == lowest) {}//If statement skips already executed highest priority task
						else {

							if (DynamicDeadline[i][1] < DynamicDeadline[f][1]) {
								f = i;

							}//If statement sets variable f(signals lowest) to i if the deadline for the variable is smaller.
						}
					}


				}//For loop finds the task with the next closest deadline and sets this to be executed.


				DynamicDeadline[TaskExecuted][0] = TaskArr[TaskExecuted][0];//Resets completed task.
				TaskExecuted = f;//Sets taskexecuted to new lowest

			}//IF statement checks that a task has been completed and outputs this to file and console.
			for (int i = 0; i < n; i++) {
				if ((count + 1) / (DynamicDeadline[i][2] * DynamicDeadline[i][1]) == 1) {
					printf("Deadline for task %d missed\n", i);
					fprintf(FPointer, " Deadline for task %d missed\n", i);
					RMSCheck[count][i] = 3;
					DynamicDeadline[i][0] = TaskArr[i][0];
					DynamicDeadline[i][2] = DynamicDeadline[i][2] + 1;
					missedCount = missedCount + 1;
				}//If statement checks if the value of the next tick(count) would be equal to the deadline of the task.
			}//For loop checks for missed deadlines.

			count = count + 1;
		}
		fprintf(FPointer, " \n\n\n Number of missed Deadlines %d ", missedCount);

	
	
	
		if (check > 50) {
			fprintf(FPointer, " \n\n\n Cannot be displayed in full");
			fprintf(FPointer, " \n\n\nEnd of file");
		}
		else {
			fprintf(FPointer, " \n\n\nEnd of file");
			fclose(FPointer);//Closes fpointer
			DisplayGfx(n, RMSCheck, EDFCheck, check); //Calls function to graph output data.
		} //If statement checks if the data can be shown in graphical form.
	
	return EXIT_SUCCESS;
}



void DisplayGfx(int NoTasks, int RMSDetails[array_size][10], int EDFDetails[array_size][10], int TimeMax) {
	int textW = 0;
	int textH = 0;
	printf("%d \n %d \n %d\n %d", NoTasks, RMSDetails[1][0], EDFDetails[2][0], TimeMax);//Console output for error checking.

	TTF_Init();
	//Initializs TTF to allow for fonts to functionn
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL could not initialise! SDL Error:" << SDL_GetError();
	};//If statement checks if the SDL function has correctly initialized.

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	//Configures an SDL window to allow for a display and a renderer for that window.

	SDL_Color Black = { 0,0,0 };
	SDL_Color White = { 255,255,255 };
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color Blue = { 0, 0, 255 };
	SDL_Color Green = { 0, 255, 0 };
	//Defines some usable colours as a short cut.

	TTF_Font* font = TTF_OpenFont("Font.ttf", 20);
	TTF_Font* font2 = TTF_OpenFont("Font2.ttf", 11);
	//configures two fonts for drawing the labels and numbers 
	
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	//Creates a combined window and renderer to allow for lines and text to be drawn to a renderer.

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "EDF", Black);
	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, "RMS", Black);
	SDL_Surface* surfaceMessage3 = TTF_RenderText_Solid(font, "Ticks", Black);
	SDL_Surface* surfaceMessage4 = TTF_RenderText_Solid(font, "Tasks", Black);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
	SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, surfaceMessage3);
	SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, surfaceMessage4);
	//Creates textures for fixed meassages used to label the various axis.



	


	SDL_Event windowEvent;
	//Defines a SDL event based on the windowAndrenderer command.
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderPresent(renderer);
	//Clears renderer and sets the background to white to allow for a cleaner output.


	//Dynamic X axis:
	int x = 0;
	while (x <= TimeMax) {
		SDL_RenderDrawLine(renderer, 100, 450, 100 + (scaler)*x, 450); //Draw a straight line on the x axis
		SDL_RenderDrawLine(renderer, 100 + (scaler)*x, 448, 100 + (scaler)*x, 452);//Dash at every tick
		x = x + 1;
	} //While loop sets X scale for graph and adds dashes corresponding to each tick.

	SDL_QueryTexture(texture3, NULL, NULL, &textW, &textH);
	SDL_Rect text3{ (scaler)*x,460,textW,textH };
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
		//Sets the position of each number on the y axis.

		SDL_RenderPresent(renderer); //Renders the labels on the renderer.
		x = x + 1;
	}

	SDL_QueryTexture(texture4, NULL, NULL, &textW, &textH);
	SDL_Rect text4{ 15,100 * x,textW,textH };
	SDL_RenderCopy(renderer, texture4, NULL, &text4);

	SDL_RenderDrawLine(renderer, 100, 450, 100, 600 - (100 * x));
	SDL_RenderDrawLine(renderer, 100, 450, 100, 300 + (100 * x));
	//Draws the vertical line for the Y axis.
	
	SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
	SDL_Rect text{ 250,450 - 50 * x,textW,textH };
	SDL_QueryTexture(texture2, NULL, NULL, &textW, &textH);
	SDL_Rect text2{ 250,470,textW,textH };
	SDL_RenderCopy(renderer, texture, NULL, &text);
	SDL_RenderCopy(renderer, texture2, NULL, &text2);
	//Sets fixed position text labels on the y axis.

	// Dynamic Axis and labelling complete.

	int check = 0;//used as task check for loopps
	int t = -1;//Used as loop control for tick number
	int RMSCheck = 0;//Used in drawX and drawT functions to ensure the correct symbol is drawn.
	printf("\n EDF check \n %d", EDFDetails[1][0]);

	//EDF Code
	while (t < TimeMax) {
		t = t + 1;
		while (check <= NoTasks) {
			if (check == 0) {
				if (EDFDetails[t][check] == 1) {
					//Draw red straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);//Changes colour of the line to red.
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + scaler * (t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}//If statement checks if its the first tick and adjust the scalar settins accordingly 
					SDL_RenderPresent(renderer);//Command renders the line on the renderer.
				}
				if (EDFDetails[t][check] == 2) {
					//Draw red straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					DrawT(renderer, TimeMax, t, check, RMSCheck);//Calls the draw triangle function, this signals a task has been completed. 

					SDL_RenderPresent(renderer);
				}
				if (EDFDetails[t][check] == 3) {
					//Draw red straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], check);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					DrawX(renderer, TimeMax, t, check, RMSCheck);//Calls the draw cross function, this signals a task has missed its deadline.
					SDL_RenderPresent(renderer);
				}



			}
			if (check == 1) {

				if (EDFDetails[t][check] == 1) {
					//Draw green straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				if (EDFDetails[t][check] == 2) {
					//Draw green straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					DrawT(renderer, TimeMax, t, check, RMSCheck); 
					SDL_RenderPresent(renderer);

				}
				if (EDFDetails[t][check] == 3) {
					//Draw green straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					DrawX(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);
				}
				
			}

			if (check == 2) {
				if (EDFDetails[t][check] == 1) {
					//Draw blue straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
		
				if (EDFDetails[t][check] == 2) {
						//Draw blue straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
						if (t == 0) {
							SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
						else {
							SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					DrawT(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);
				}
				if (EDFDetails[t][check] == 3) {
					//Draw blue straight line
					printf("Checking SDLInfo%d \n %d", EDFDetails[t][check], t);
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 - 50 * (check + 1), 100 + (scaler)*(t + 1), 455 - 50 * (check + 1));
					}
					DrawX(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);
				}
				
			}
			check = check + 1;

		}
		check = 0;
	}

	check = 0;
	t = -1; //Used as loop control for tick number
	RMSCheck = 1;
	printf("\n\n RMS Draw");
	while (t < TimeMax) {
		t = t + 1;
		while (check <= NoTasks) {
			if (check == 0) {
				if (RMSDetails[t][check] == 1) {
					//Draw red straight line
					
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + scaler * (t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				if (RMSDetails[t][check] == 2) {
					//Draw red straight line
					
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					DrawT(renderer, TimeMax, t, check, RMSCheck);

					SDL_RenderPresent(renderer);
				}
				if (RMSDetails[t][check] == 3) {
					//Draw red straight line
					
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					DrawX(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);
				}



			}
			if (check == 1) {

				if (RMSDetails[t][check] == 1) {
					//Draw green straight line
				
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				if (RMSDetails[t][check] == 2) {
					//Draw green straight line
					
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					DrawT(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);

				}
				if (RMSDetails[t][check] == 3) {
					//Draw green straight line
					
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 2));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					DrawX(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);
				}

			}

			if (check == 2) {
				if (RMSDetails[t][check] == 1) {
					//Draw blue straight line
					
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					SDL_RenderPresent(renderer);
				}
				if (RMSDetails[t][check] == 2) {
					//Draw blue straight line
					
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					DrawT(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);

				}
				if (RMSDetails[t][check] == 3) {
					//Draw blue straight line
				
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
					if (t == 0) {
						SDL_RenderDrawLine(renderer, 100, 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					else {
						SDL_RenderDrawLine(renderer, 100 + (scaler)*(t), 455 + 50 * (check + 1), 100 + (scaler)*(t + 1), 455 + 50 * (check + 1));
					}
					DrawX(renderer, TimeMax, t, check, RMSCheck);
					SDL_RenderPresent(renderer);
				}


			}
			check = check + 1;
		}
		check = 0;
	}//For comments see EDF while loop for graphics, its the same just a different offset for positioning.

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
		SDL_Quit();//Handles cleanup of the various SDL and TTF elements used in this function.

}


void DrawX(SDL_Renderer *&renderer,int TimeMax,int t, int check,int RMSCheck) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);//Sets draw colour to black for the marker.
	if (RMSCheck == 1) {
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t), 453 + 50 * (check + 1), 102 + (scaler)*(t), 457 + 50 * (check + 1));
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t), 457 + 50 * (check + 1), 102 + (scaler)*(t), 453 + 50 * (check + 1));
	}
	else {
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t), 453 - 50 * (check + 1), 102 + (scaler)*(t), 457 - 50 * (check + 1));
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t), 457 - 50 * (check + 1), 102 + (scaler)*(t), 453 - 50 * (check + 1));
	}//If statement checks the value of RMS, this causes the X to be drawn on either the RMS or EDF section of the output.
	SDL_RenderPresent(renderer);//Renders the X.
}
void DrawT(SDL_Renderer *&renderer, int TimeMax, int t, int check,int RMSCheck) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);//Sets draw colour to black for the marker.
	if (RMSCheck == 1) {
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t+1), 453 + 50 * (check + 1), 102 + (scaler)*(t+1), 458 + 50 * (check + 1));
		SDL_RenderDrawLine(renderer, 102 + (scaler)*(t+1), 458 + 50 * (check + 1), 104 + (scaler)*(t+1), 453 + 50 * (check + 1));
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t+1), 453 + 50 * (check + 1), 104 + (scaler)*(t+1), 453 + 50 * (check + 1));
	}
	else {
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t + 1), 453 - 50 * (check + 1), 102 + (scaler)*(t + 1), 458 - 50 * (check + 1));
		SDL_RenderDrawLine(renderer, 102 + (scaler)*(t + 1), 458 - 50 * (check + 1), 104 + (scaler)*(t + 1), 453 - 50 * (check + 1));
		SDL_RenderDrawLine(renderer, 98 + (scaler)*(t + 1), 453 - 50 * (check + 1), 104 + (scaler)*(t + 1), 453 - 50 * (check + 1));
	}//If statement checks the value of RMS, this causes the triangle to be drawn on either the RMS or EDF section of the output.
	SDL_RenderPresent(renderer);//Renders the T.
}


//Testing

//Font sources:
/* Font1 - https://fonts.google.com/specimen/Roboto?sidebar.open=true&selection.family=Roboto
Font2 - https://fonts.google.com/specimen/Ubuntu?preview.text=1&preview.text_type=custom&preview.size=45&sidebar.open=true&selection.family=Ubuntu:wght@300 */


