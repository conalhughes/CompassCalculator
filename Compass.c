#include <stdio.h>
#include <ctype.h>

// constants
#define ARRAY_LENGTH 10
const int SCALE_SIZE = 1000;

// global tracker
int gEnd = 0;

#define TRUE 1
#define FALSE 0

// functions
void setup();
void loop();
void addToFifo(char * FIFO , char newChar, int len);
void readInput(char* FIFO);
void calculatePosition(int* xPos, int *yPos, char* FIFO, int len);
void findDirectionAndSlope(int x , int y);
void noSlope(int x , int y);
void pSlope(int x, int y);

void main()
{
    // run setup
    setup();

    // run superloop
    for(;;)
    {
      loop();
      // if user has cancelled the program, end
      if(gEnd == 1)
        break;
    }

}
void setup() 
{
  // setup code here, to run once:
  printf("This program reads in letters i,j,k and l as steps in the directions N,W,E and S respectively\nThe deirection is calculated using the 1000*slope method\n");
  printf("Please note this program will run until e is inputted\n");
  printf("\n\nAverageDirection Starting\n");
}

void loop() 
{
  // main code here, to run repeatedly:
  char FIFO [ARRAY_LENGTH] ={0x00};
  int x = 0;
  int y = 0;

  printf("\nType direction characters: J, K, L, or I (upper or lower case) and hit return \nReceived: ");

  readInput(FIFO);
  // at this point we have FIFO filled with whatever values we need

  printf("\nBuffer Contents: ");
  // now calculate x and y values 
  calculatePosition(&x, &y, FIFO , ARRAY_LENGTH);
  /*
  * we now have FIFO filled and the values for x and y
  * slopes can easily be got by1000*y/x if y != 0
  * if y = 0 the direction is either north or south
  * NE NW SE and SW are all at 45 degrees, or slope +/- 1000
  * so to find whats the nearest direction we need to find tan(45 +/-(45/2)) which is a slope of 2414 or 414.
  * find slope, find quarand and youll find the direction
  */
 findDirectionAndSlope(x , y);
 
  
}
void readInput(char* FIFO)
{
  while(TRUE) // loop to read serial input
  {
    char c;
    scanf("%c", &c); // read the next character

    // if its a whitespace character then break loop
    if(c == '\n' || c == '\r' || c == isspace(c))
      break;
    
    // if its uppercase change to lowercase
    if(isupper(c))
      c = tolower(c);

    // if the user ends the program, change gEnd
    if(c == 'e')
    {
      gEnd = 1;
      break;
    }

    // if its i j k or l add to FIFO buffer
    if(c == 'i' || c == 'j' || c == 'k' || c == 'l')
      addToFifo(FIFO, c, ARRAY_LENGTH);
    
    printf("%c",c);
  }
}
void addToFifo(char * FIFO , char newChar, int len)
{
  char *p;
  for(p = FIFO + len-2; p >= FIFO; p--)
  {
    *(p+1) = *p;
    if(p == FIFO)
      *p = newChar;
  }
}
void calculatePosition(int *xPos, int *yPos, char* FIFO, int len)
{
  char *p;
  for(p = FIFO; p < FIFO+len; p++)
  {
    if(*p == 'i')
      *yPos = *yPos + 1;
    else if(*p == 'k')
      *yPos = *yPos - 1;
    else if(*p == 'l')
      *xPos = *xPos + 1;
    else if(*p == 'j')
      *xPos = *xPos - 1;
    printf("%c", *p);
  }
  printf("\n");
}
void findDirectionAndSlope(int x , int y)
{
  int slope;
  
  printf("Resultant: x= %d, y= %d\n", x, y);
//   print("%d", x);
//   printf(", y=");
//   print(y);
//   printf("\n");

  if(x == 0 || y == 0)
  {
    noSlope(x , y); 
    return;
  }
  slope = (y*1000)/x;
  printf("Direction : ");
  if(x > 0)
    printf("RHS");
  else
    printf("LHS");
  printf(", 1000*slope = %d", slope);
  /*
   * slope between 414 and - 414 --> east or west
   * slope between -414 and -2414 --> NW or SE
   * slope between 414 and 2414 --> NE or SW
   * slope > 2414 or < -2414 --> North or south
   */
   printf(", Compass: ");
   if(slope > -414 && slope < 414)
   {
      if(x > 0)
        printf("East\n");
      else
        printf("West\n");
   }
   else if(slope > 414 && slope < 2414)
   {
     if(x > 0)
       printf("NorthEast\n");
     else
       printf("SouthWest\n");
   }
   else if(slope < -414 && slope > -2414)
   {
     if(x < 0)
       printf("NorthWest\n");
     else
       printf("SouthEast\n");
   }
   else if(y > 0)
     printf("North\n");
   else
     printf("South\n");
}
void noSlope(int x , int y)
{
  printf("no slope. Compass: ");
  if(x == 0 && y == 0)
    printf("No Compass\n");
  else if(x == 0)
  {
    if(y > 0)
      printf("North\n");
    else
      printf("South\n");
  }
  else
  {
    if(x > 0)
      printf("East\n");
    else
      printf("West\n");
  }
}
