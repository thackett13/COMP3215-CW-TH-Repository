
#include <stdio.h>
#include <graphics.h>

int main( int argc, char* args[] )
{
     initwindow(400, 300, "First Sample");
     circle(100, 50, 40);
     while (!kbhit( ))
     {
         delay(200);
     }
     return 0;
}