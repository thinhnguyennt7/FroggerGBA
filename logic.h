#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */
    int dealthTime;

} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

typedef struct ENEMY {
    int row;
    int col;
    int rd;
    int cd;
    int size;
} ENEMY;

typedef struct PLAYER {
    int row;
    int col;
    int size;
} PLAYER;

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.
void drawEnemy(ENEMY* enemy, unsigned short color);
void erasePlayer(PLAYER* player, unsigned short insideP);
void drawPlayer(PLAYER* player, unsigned short insideP);
void drawWater(int row, int col, u16* image);
void startGame(void);
void runApp(void);
void setUpPlayer(void);
void setUp(void);
void setUpWater(void);
void movePlayer(void);
void moveEnemies(void);
void disPlayerScreen(void);
int checkIfCollision(void);
void replayMessage(void);


#endif
