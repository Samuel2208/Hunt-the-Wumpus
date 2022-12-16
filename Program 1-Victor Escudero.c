/* ---------------------------------------------
Program 1: Wumpus, version 1 (fixed size array)

Course: CS 211, Fall 2022. Tues 8am lab
System: Mac using Replit 
Author: Victor Escudero
---------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>		// for srand
#include <ctype.h> // for tolower()
#include <time.h> // for time()
#include <stdbool.h>// for bool variables
//--------------------------------------------------------------------------------
const int maxRooms = 21;
const int maxTunnels = 3;

int rooms[maxRooms][maxTunnels] = {{0,0,0},
                                    {2,5,8},
                                    {1,3,10},
                                    {2,4,12},
                                    {5,3,14},
                                    {1,4,6},
                                    {5,7,15},
                                    {6,8,17},
                                    {1,7,9},
                                    {8,10,18},
                                    {2,9,11},
                                    {10,12,19},
                                    {3,11,13},
                                    {12,14,20},
                                    {4,13,15},
                                    {6,14,16},
                                    {15,17,20},
                                    {7,16,18},
                                    {9,17,19},
                                    {11,18,20},
                                    {13,16,19},
};


void displayCave() {//Void function that displays the map.
  printf("       ______18______             \n"
         "      /      |       \\           \n"
         "     /      _9__      \\          \n"
         "    /      /    \\      \\        \n"
         "   /      /      \\      \\       \n"
         "  17     8        10     19       \n"
         "  | \\   / \\      /  \\   / |    \n"
         "  |  \\ /   \\    /    \\ /  |    \n"
         "  |   7     1---2     11  |       \n"
         "  |   |    /     \\    |   |      \n"
         "  |   6----5     3---12   |       \n"
         "  |   |     \\   /     |   |      \n"
         "  |   \\       4      /    |      \n"
         "  |    \\      |     /     |      \n"
         "  \\     15---14---13     /       \n"
         "   \\   /            \\   /       \n"
         "    \\ /              \\ /        \n"
         "    16---------------20           \n"
         "\n");
}


void displayInstructions()//Void function that displays the instructions.
{
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
    		"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
    		"2. The Wumpus is not bothered by the pits, as he has sucker feet. Usually he is \n"
			"   asleep. He will wake up if you enter his room. When you move into the Wumpus'\n"
			"   room, then he wakes and moves if he is in an odd-numbered room, but stays    \n"
			"   still otherwise.  After that, if he is in your room, he snaps your neck and  \n"
			"   you die!                                                                     \n"
    		"                                                                                \n"
    		"Moves:                                                                          \n"
    		"On each move you can do the following, where input can be upper or lower-case:  \n"
    		"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
    		"   then a room number.                                                          \n"
    		"2. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
    		"3. Enter 'C' to cheat and display current board positions.                      \n"
    		"4. Enter 'D' to display this set of instructions.                               \n"
    		"5. Enter 'P' to print the maze room layout.                                     \n"
        "6. Enter 'G' to guess which room Wumpus is in, to win or lose the game!         \n"                       
    		"7. Enter 'X' to exit the game.                                                  \n"
    		"                                                                                \n"
    		"Good luck!                                                                      \n"
    		" \n\n");
}

//Bool function that takes a 2D array with the stored rooms, the current room, and the new room. The function when called checks if the new room is adjacent to the current room if it is, it will return true otherwise it will return false.
bool adjacentRoomCheck(int array[21][3], int currentRoom, int newRoom)
{
  bool valid = false;
  for(int i = currentRoom; i < currentRoom + 1; i++ )
  {
    for(int j = 0; j < 3; j++)
    {
      if(newRoom == array[i][j])
      {
        valid = true;
      }
    }
  }
  return valid;
}

//Bool function that takes an array of game components and the index of this array. The function when called in main checks that the random room assignment to this components dont repeat and if they do it will return true.
bool repeatCheck(int array[], int index)
{
  bool valid = false;
  for(int i = index; i >= 0; i--)
  {  
    for(int j = i-1; j >= 0; j--)
    {
      if(array[i] == array[j])
      {
        valid = true;
      }
    }
  }
  return valid;
}

//Structure of game components
struct objects {
  int pit1;
  int pit2;
  int wumpus;
  int currentRoom;
};

//This function takes a structure that contains game components such as pit1, pit2, wumpus, and player position. It also takes a 2D array containg the stored rooms. The function when called in main checks if a game component is adjacent to the current room and if it is it will return 1 for wumpus, 2 for pits and 3 for both.
int gameComponentCheck(struct objects ob, int array[21][3])
{
  int wumpus = 0;
  int pits = 0;
  int finalPick = 0;
  
  for(int i = ob.currentRoom; i < ob.currentRoom + 1; i++ )
  {
    for(int j = 0; j < 3; j++)
    { 
      if(ob.wumpus == array[i][j])
      {
        wumpus = 1;
        finalPick = wumpus;
      }

      if(ob.pit1 == array[i][j] || ob.pit2 == array[i][j])
      {
        pits = 2;
        finalPick = pits;
      }
    }
  }

  if(wumpus == 1 && pits == 2)
  {
    finalPick = 3;
  }

  return finalPick;
}

//--------------------------------------------------------------------------------
int main(void) {
  srand(0); // uncoment for test cases
  //srand(time(0));//for regular random 

  struct objects ob;
  int hazards[4];
  
  //random int assigments to game components.
  for(int i = 0; i < 4; i++)
  {
    int randomValue = rand() % 20 + 1; 
    hazards[i] = randomValue;
    while(repeatCheck(hazards, i))//If random assignment repeats assign a new one
      {
        hazards[i] = rand() % 20 + 1;
      }
  }
  //Storing final random assignments to structure.
  ob.pit1 = hazards[0];
  ob.pit2 = hazards[1];
  ob.wumpus = hazards[2];
  ob.currentRoom = hazards[3];
  
  int guessRoom;
  int newRoom;
  int counter = 1;
  bool game = true;
  char userChoice;
  //Start of the game loop.
  while(game == true)
  {
    bool invalidMove = false;
    int gameComponent = gameComponentCheck(ob, rooms);//Check if player room is adjacent to any game component and store specific int variable for comparison.
    //Switch statement to check whether wumpus or pits are adjacent to current room and print respective sentences.
    switch(gameComponent){
      case 1:
        printf("You are in room %d. ", ob.currentRoom);
        printf("You smell a stench. ");
        printf("\n");
      break;

      case 2:
        printf("You are in room %d. ", ob.currentRoom);
        printf("You feel a draft. ");
        printf("\n");
      break;

      case 3:
        printf("You are in room %d. ", ob.currentRoom);
        printf("You smell a stench. ");
        printf("You feel a draft. ");
        printf("\n");
      break;

      default:
        printf("You are in room %d. ", ob.currentRoom);
        printf("\n");
      break;
    }
    //Get user input.
    printf("\n%d. Enter your move (or 'D' for directions): ", counter);
    scanf(" %c", &userChoice);
    userChoice = tolower(userChoice);
    //Switch statement to perform comands depending of users choise.
    switch(userChoice){
      case 'm'://Move option
        scanf("%d", &newRoom);
        
        bool validMove = false;
        validMove = adjacentRoomCheck(rooms, ob.currentRoom, newRoom);//Check if new room is adjacent to current one.

        if(validMove == true)
        {
          ob.currentRoom = newRoom;
        }
          
        if(validMove == false)
          {
            printf("Invalid move.  Please retry. \n");
            invalidMove = true;
          }

        if(ob.currentRoom == ob.pit1 || ob.currentRoom == ob.pit2)
        {
          printf("Aaaaaaaaahhhhhh....   \n");
	        printf("    You fall into a pit and die. \n");
          game = false;
        }
        
        if(ob.currentRoom == ob.wumpus && ob.wumpus % 2 != 0)
        {
          printf( "You hear a slithering sound, as the Wumpus slips away. \n"
			"Whew, that was close! \n");
          ob.wumpus = rooms[ob.currentRoom][0];
        }
        else if(ob.currentRoom == ob.wumpus)
        {
          printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
			"It is over.\n");

          game = false;
        }
        
      break;
  
      case 'r'://Reset option.
        printf("Enter the room locations (1..20) for player, wumpus, pit1, and pit2: \n\n");
        scanf("%d %d %d %d", &ob.currentRoom, &ob.wumpus, &ob.pit1, &ob.pit2);

        counter--;
      break;
        
      case 'c'://Cheat option.
        printf( "Cheating! Game elements are in the following rooms: \n"
      	"Player Wumpus Pit1 Pit2  \n"
      	"%4d %7d %5d %5d \n\n", 
  			ob.currentRoom,
  			ob.wumpus,
  			ob.pit1,
  			ob.pit2
  		  );

        counter--;
      break;
        
      case 'd'://Directions option.
        printf("\n");
        displayCave();
        displayInstructions();
        counter--;
      break;
  
      case 'p'://Map print option.
        printf("\n");
        displayCave();
        counter--;
      break;
  
      case 'g'://Guess option.
        printf("Enter room (1..20) you think Wumpus is in: ");
        scanf("%d", &guessRoom);

        if(guessRoom == ob.wumpus)
        {
          printf("You won!\n");
          game = false;
        }
        else
        {
          printf("You lost.\n");
          game = false;
        }
      break;
      
      case 'x'://Exit program option.
        game = false;
      break;
      
      default:
        printf("Invalid move.  Please retry. \n");
        invalidMove = true;
      break;
    }
    
    if(invalidMove == false)
    {
      counter++;
    }
  
  }

  printf("\nExiting Program ...\n");
  
	return 0;
}