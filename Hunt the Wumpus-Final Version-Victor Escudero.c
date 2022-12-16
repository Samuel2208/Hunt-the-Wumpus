/* ---------------------------------------------
Prog 2: Wumpus (bats, arrow, and all arrays are dynamic)

Course: CS 211, Fall 2022. Tues 8am lab
System: Mac using Replit 
Author: Victor Escudero
---------------------------------------------
*/
#include <stdio.h>
#include <stdbool.h>	// for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()
#include <stdlib.h>   // Needed for malloc

#define NUMBER_OF_ROOMS 20

// Used to more conveniently pass all game information between functions.
struct GameInfo {
	int moveNumber;  // Counts up from 1, incrementing for each move
	int personRoom;  // Room 1..20 the person currently is in
	int wumpusRoom;  // Room 1..20 the Wumpus is in
	int pitRoom1;    // Room 1..20 the first pit is in
	int pitRoom2;    // Room 1..20 the second pit is in
  int bat1;        // Room 1..20 the first bat is in
  int bat2;        // Room 1..20 the second bat is in
  int arrowRoom;       // Room 1..20 the arrow is in
};

// Function prototype needed to allow calls in any order between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *personIsAlive, int **pArray);

//--------------------------------------------------------------------------------
void displayCave()
{
  printf( "\n"
    "       ______18______             \n"
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


//--------------------------------------------------------------------------------
void displayInstructions()
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
      "7. Enter 'X' to exit the game.                                                  \n"
      "                                                                                \n"
      "Good luck!                                                                      \n"
      " \n\n");
}//end displayInstructions()


//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue,      // New number we're checking
                 int randomNumbers[],  // Set of numbers previously found
                 int limit)            // How many numbers previously found
{
  int returnValue = false;
  
  // compare random value against all previously found values
  for( int i = 0; i<limit; i++) {
    if( randomValue == randomNumbers[i]) {
      returnValue = true;   // It is already there
      break;
    }
  }
  
  return returnValue;
} 


//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int randomNumbers[],   // Array of random numbers
                     int size)              // Size of random numbers array
{
  int randomValue = -1;
  
  for( int i = 0; i<size; i++) {
    do {
      randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
    } while (alreadyFound(randomValue, randomNumbers, i));
    randomNumbers[i] = randomValue;
  }
} 


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(struct GameInfo *gameInfo)   // All game settings variables
{
  // Array of 7 unique values 1..20, to be used in initializing cave hazards locations
  int *randomNumbers = (int)malloc(7*sizeof(int));
  
  // Initialize cave room connections
  //       ______18______
  //      /      |       \
  //     /      _9__      \
  //    /      /    \      \
  //   /      /      \      \
  //  17     8        10    19
  // |  \   / \      /  \   / |
  // |   \ /   \    /    \ /  |
  // |    7     1---2     11  |
  // |    |    /     \    |   |
  // |    6----5     3---12   |
  // |    |     \   /     |   |
  // |    \       4      /    |
  // |     \      |     /     |
  //  \     15---14---13     /
  //   \   /            \   /
  //    \ /              \ /
  //    16---------------20
  
    
  // Select some unique random values 1..20 to be used for 2 bats rooms, 2
  // pits rooms, Wumpus room, and initial player room
  setUniqueValues(randomNumbers, 7);
  // Use the unique random numbers to set initial locations of hazards, which
  //    should be non-overlapping.

  gameInfo->personRoom = randomNumbers[0];
  gameInfo->wumpusRoom = randomNumbers[1];
  gameInfo->pitRoom1 = randomNumbers[2];   
  gameInfo->pitRoom2 = randomNumbers[3];
  gameInfo->bat1 = randomNumbers[4];
  gameInfo->bat2 = randomNumbers[5];
  gameInfo->arrowRoom = randomNumbers[6];
  
  gameInfo->moveNumber = 1;

  free(randomNumbers);
}// end initializeBoard(...)


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[ 3],     // Array of adjacent tunnels
                    int nextRoom)        // Desired room to move to
{
  return( tunnels[0] == nextRoom ||
          tunnels[1] == nextRoom ||
          tunnels[2] == nextRoom
        );
}

//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(struct GameInfo gameInfo)
{
  printf( "Cheating! Game elements are in the following rooms: \n"
      "Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
      "%4d %5d %6d %5d %5d %5d %5d \n\n", 
    gameInfo.personRoom,
    gameInfo.wumpusRoom,
    gameInfo.pitRoom1,
    gameInfo.pitRoom2,
    gameInfo.bat1,
    gameInfo.bat2,
    gameInfo.arrowRoom
  );
}// end displayCheatInfo(...)

//--------------------------------------------------------------------------------
// Display arrow info and return how many rooms we want to shoot
int * displayArrowInfo(int *numRooms, struct GameInfo *gameInfo)
{
  int *roomsToShoot = (int)malloc(3*sizeof(int));
  int currentRoom = gameInfo->personRoom;
  
  printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");

  scanf("%d", &*numRooms);
  //Check if user picks a greater number of rooms than the limit of 3 rooms
  if(*numRooms > 3)
  {
    printf("Sorry, the max number of rooms is 3. Setting that value to 3.");
    *numRooms = 3;
  }

  //check the number of rooms we want to shoot at and store the rooms in a an array
  if(*numRooms == 1)
  {
    scanf("%d", &roomsToShoot[0]);
  }
  else if(*numRooms == 2)
  {
    scanf("%d %d", &roomsToShoot[0], &roomsToShoot[1]);
  }
  else if(*numRooms == 3)
  {
    scanf("%d %d %d", &roomsToShoot[0], &roomsToShoot[1], &roomsToShoot[2]);
  }
  
  return roomsToShoot;// return array of rooms to shoot at
}// end displayArrowInfo(...)

//--------------------------------------------------------------------------------
// 
void arrowComponents(struct GameInfo *gameInfo,bool *personIsAlive, int roomToShoot, int **pArray)
{
  int currentRoom = gameInfo->personRoom;//current room
  int arrowRoom = gameInfo->arrowRoom;//current arrow room
  int wumpusRoom = gameInfo->wumpusRoom;//wumpus room
  int rooms[3];//rooms to compare
  
  // Retrieve index values of all 3 adjacent rooms
  rooms[0] = pArray[arrowRoom][0];
  rooms[1] = pArray[arrowRoom][1];
  rooms[2] = pArray[arrowRoom][2];

  //check if desired room to shoot is adjacent to current one. If not arrow ricochets
  if(roomIsAdjacent(rooms, roomToShoot))// Instance where arrow does not ricochet
  {
    gameInfo->arrowRoom = roomToShoot;
    // Arrow kills the Wumpus
    if( gameInfo->arrowRoom == wumpusRoom)//Arrow kills Wumpus
    {
  	  printf( "Wumpus has just been pierced by your deadly arrow!\n"
                "Congratulations on your victory, you awesome hunter you.\n");
      *personIsAlive = false;
    }
    if(gameInfo->arrowRoom == currentRoom)//Arrow kills player
    {
  	  printf( "You just shot yourself.  \n"
              "Maybe Darwin was right.  You're dead.\n");
      *personIsAlive = false;
    }
  }
  else// Instance where arrow ricochets
  {
    gameInfo->arrowRoom = rooms[0];//Arrow ricochets to lowest adjacent room

    // Attempt to shoot arrow through room that is not adjacent
	  printf("Room %d is not adjacent.  Arrow ricochets...\n", roomToShoot);
    
    if(gameInfo->arrowRoom == wumpusRoom)// Arrow ricochets, and kills Wumpus
    {
	    printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
            "Accidental victory, but still you win!\n");
      *personIsAlive = false;
    }
    if(gameInfo->arrowRoom == currentRoom)// Arrow ricochets, and kills person
    {
  	  printf( "You just shot yourself, and are dying.\n"
              "It didn't take long, you're dead.\n");
      *personIsAlive = false;
    }
  }
  
}// end displayArrowInfo(...)

//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo( struct GameInfo *gameInfo, int **pArray)
{
  // Retrieve player's current room number and display it
  int currentRoom = gameInfo->personRoom;

  // Display arrow message to walking into arrow room
  int arrowRoom = gameInfo->arrowRoom;
  if( currentRoom == arrowRoom) 
  {
    printf("Congratulations, you found the arrow and can once again shoot.\n");
    gameInfo->arrowRoom = -1;
  }
  
  printf("You are in room %d. ", currentRoom);
  
  // Retrieve index values of all 3 adjacent rooms
  int room1 = pArray[currentRoom][0];
  int room2 = pArray[currentRoom][1];
  int room3 = pArray[currentRoom][2];
  
  // Display hazard detection message if Wumpus is in an adjacent room
  int wumpusRoom = gameInfo->wumpusRoom;
  if( room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
      printf("You smell a stench. ");
  }
  
  // Display hazard detection message if a pit is in an adjacent room
  int pit1Room = gameInfo->pitRoom1;
  int pit2Room = gameInfo->pitRoom2;
  if( room1 == pit1Room || room1 == pit2Room ||
      room2 == pit1Room || room2 == pit2Room ||
      room3 == pit1Room || room3 == pit2Room
    ) {
      printf("You feel a draft. ");
  }

  // Display hazard detection message if a bat is in an adjacent room
  int bat1Room = gameInfo->bat1;
  int bat2Room = gameInfo->bat2;
  if( room1 == bat1Room || room1 == bat2Room ||
      room2 == bat1Room || room2 == bat2Room ||
      room3 == bat1Room || room3 == bat2Room
    ) {
      printf("You hear rustling of bat wings. ");
  }

  printf("\n\n");
}//end displayRoomInfo(...)

//--------------------------------------------------------------------------------
//Function that deals with bats when you go into a room with a bat in
void inRoomWithBats(struct GameInfo *gameInfo)
{
  int randomValue = -1;
  int currentRoom = gameInfo->personRoom;
  int bat1 = gameInfo->bat1;
  int bat2 = gameInfo->bat2;

  // generate a new position for player
  do {
    randomValue = rand() % NUMBER_OF_ROOMS + 1; // random number 1..20
  } while(randomValue == currentRoom);
  
  gameInfo->personRoom = randomValue;//new position for player
 
  // generate a new position for bat 1 
  if(gameInfo->bat1 == currentRoom)
  {
    do {
      randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
    } while (randomValue == gameInfo->personRoom || randomValue == bat1 || randomValue == bat2);

    gameInfo->bat1 = randomValue;
  }
  else if(gameInfo->bat2 == currentRoom)
  {
    do {
      randomValue = rand() % NUMBER_OF_ROOMS + 1; // random number 1..20
    } while (randomValue == gameInfo->personRoom || randomValue == bat1 || randomValue == bat2);

    gameInfo->bat2 = randomValue;
  }

}
//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the person dies.
// If the person just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
void checkForHazards(
         struct GameInfo *gameInfo, // Hazards location and game info
         bool *personIsAlive,       // Person is alive, but could die depending on the hazar
         int **pArray)
{
  // retrieve the room the person is in
  int personRoom = gameInfo->personRoom;

  // Check for the Wumpus
  if( personRoom == gameInfo->wumpusRoom) {
 
    if( gameInfo->wumpusRoom %2 == 1) {
      // You got lucky and the Wumpus moves away
      printf( "You hear a slithering sound, as the Wumpus slips away.\n"
    "Whew, that was close! \n");
      
      gameInfo->wumpusRoom = pArray[ personRoom][ 0];  // Choose the lowest-numbered adjacent room
    }
    else {
      // Wumpus kills you
      printf(	"You briefly feel a slimy tentacled arm as your neck is snapped.\n"
    "It is over.\n");
      *personIsAlive = false;
      return;
    }
  }
  
  // Check whether there is a pit
  if( personRoom == gameInfo->pitRoom1 || personRoom == gameInfo->pitRoom2) {
    // Person falls into pit
    printf("Aaaaaaaaahhhhhh....   \n");
    printf("    You fall into a pit and die.\n");
    *personIsAlive = false;
    return;
  }
  // Check whether there is bats
  if( personRoom == gameInfo->bat1||personRoom == gameInfo->bat2) {
    inRoomWithBats(&*gameInfo);
    printf("Woah... you're flying!\n");
    printf("You've just been transported by bats to room %d.\n", gameInfo->personRoom); 
    
    // Check again after bats dropped you off if there is bats in the new room
    if(gameInfo->personRoom == gameInfo->bat2||gameInfo->personRoom == gameInfo->bat1)
    {
      inRoomWithBats(&*gameInfo);
      printf("Woah... you're flying!\n");
      printf("You've just been transported by bats to room %d.\n", gameInfo->personRoom); 
      
      return;
    }
  }
}//end checkForHazards(...)

//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the person's location,
// useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo)
{
  printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n");

  scanf(" %d %d %d %d %d %d %d", &theGameInfo->personRoom, &theGameInfo->wumpusRoom, &theGameInfo->pitRoom1, &theGameInfo->pitRoom2, &theGameInfo->bat1, &theGameInfo->bat2, &theGameInfo->arrowRoom);
  printf("\n");
}

//--------------------------------------------------------------------------------
int main(void) {
  struct GameInfo gameInfo;   // Used to more easily pass game info variables around
  bool personIsAlive = true;  // Used in checking for end of game
  bool wumpusIsAlive = true;  // Used in checking for end of game
  char typeOfMove;            // Used to handle user input letter
  int nextRoom;               // User input of destination room number, used on a 'M' type move

  //Memory allocate the rows of the 2D array
  int **pArray = (int **)malloc(21 * sizeof(int*));
  //Allocte the columns of the 2D array
  for(int i = 0; i < 21; i++)
  {
    pArray[i] = (int*)malloc(3 * sizeof(int));
  }
  //create array for all possible adjacent rooms
  int assignments[63] = {0,0,0,2,5,8,1,3,10,2,4,12,3,5,14,1,4,6,5,7,15,6,8,17,1,7,9,8,10,18,2,9,11,10,12,19,3,11,13,12,14,20,4,13,15,6,14,16,15,17,20,7,16,18,9,17,19,11,18,20,13,16,19};
  //initialize the dynamic 2D array with the rooms
  int index = 0;
  for(int i = 0;i< 21;i++)
  {
    for(int j = 0;j<3;j++)
    {
      pArray[i][j] = assignments[index];
      index++;
    }
  }
  
  // Seed the random number generator.  Change seed to time(0) to change output each time.
  // srand(time(0));
  srand(1);
  
  // Set random initial values for person, Wumpus, bats and pits
  initializeGame( &gameInfo);

// Main playing loop.  Break when player dies, or player kills Wumpus
  while (personIsAlive && wumpusIsAlive) {
          
  // Display current room information: Room number, hazards detected
  displayRoomInfo( &gameInfo, pArray);
  
  // Prompt for and handle move
  printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
    
  scanf(" %c", &typeOfMove);	
                
  typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
  
  // Check all types of user input and handle them.  
  if( typeOfMove == 'D') {
    displayCave();
    displayInstructions();
    continue;       // Loop back up to reprompt for the same move
  }
  else if( typeOfMove == 'P') {
    // To assist with play, display the cave layout
    displayCave();
    continue;       // Loop back up to reprompt for the same move
  }
  else if( typeOfMove == 'M') {
    
    scanf(" %d", &nextRoom);
    
    if( roomIsAdjacent(pArray[ gameInfo.personRoom], nextRoom)) {
      gameInfo.personRoom = nextRoom;        // move to a new room
      // Check if pit, wumpus, or bats are present in this new room
      checkForHazards(&gameInfo, &personIsAlive, pArray);
    }
    else {
      printf("Invalid move.  Please retry. \n");
      continue;        // Doesn't count as a move, so retry same move.
    }
  }
  else if( typeOfMove == 'C') {
    // Display Cheat information
    displayCheatInfo(gameInfo);
    continue;        // Doesn't count as a move, so retry same move.
  }
  else if( typeOfMove == 'R') {
    // Specify resetting the hazards and person positions, useful for testing
    resetPositions( &gameInfo);
    continue;        // Doesn't count as a move, so retry same move.
  }
  else if( typeOfMove == 'X') {
    // Exit program
    personIsAlive = false;   // Indicate person is dead as a way to exit playing loop
    break;
  }
  else if(typeOfMove == 'S')
  {
    int *roomsToShoot;
    int numRooms;
    int wumpusRoom = gameInfo.wumpusRoom;
    int lowesAdjacentRoom = pArray[wumpusRoom][0]; //Lowest adjacent room to wumpus room

    //Alert the user that you can not shoot without an arrow
    if(gameInfo.arrowRoom != -1)
    {
      // Attempt to shoot arrow you don't have
      printf("Sorry, you can't shoot an arrow you don't have. Go find it.\n");
      // Increment the move number
      gameInfo.moveNumber = gameInfo.moveNumber + 1;
      continue;
    }
    
    roomsToShoot = displayArrowInfo(&numRooms, &gameInfo);

    gameInfo.arrowRoom = gameInfo.personRoom;//make arrow be in the same room as persons room before shooting to a new one for checking purposes.

    for(int i = 0; i< numRooms; i++)
    {
      arrowComponents(&gameInfo, &personIsAlive, roomsToShoot[i], pArray);
      if(personIsAlive == false)//Check if game ended 
      {
        break;
      }
    }

    //Wumpus hears the arrow being shot and moves to the lowest adjacent room. 
    gameInfo.wumpusRoom = lowesAdjacentRoom;
  }

  // Increment the move number
  gameInfo.moveNumber = gameInfo.moveNumber + 1;
      
  }//end while(personIsAlive && wumpusIsAlive)
  
  printf("\nExiting Program ...\n");

  //free the dynamic allocated array
  for(int i = 0;i< 21;i++)
  free(pArray[i]);

  free(pArray);
	
	return 0;
}
