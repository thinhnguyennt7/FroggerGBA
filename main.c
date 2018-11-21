#include "gba.h"
#include "logic.h"
#include "graphics.h"
#include "images/frogger.h"
#include "images/gameover.h"
#include "images/victory.h"
#include "images/water.h"
#include "images/logo.h"
#include <stdio.h>
#include <stdlib.h>

// AppState enum definition
typedef enum {
    START,
    START_NODRAW,
    APP_INIT,
    APP,
    APP_EXIT,
    APP_WIN
} GBAState;

// Initlize variables
#define NENEMIES 5
char str[60];
ENEMY enemies[NENEMIES];
ENEMY oldenemies[NENEMIES];
PLAYER P;
u16 row = 15;
u16 col = 25;
unsigned int deathCount;
unsigned int stageCount;
int winGame = 0;
int nextStage = 0;
int loseGame = 0;
unsigned int oldDeath = 5;

// Start the game with basic setup
void startGame(void) {
    drawFullScreenImageDMA((u16*)frogger);
    sprintf(str, "Welcome to My Game - Thinh Nguyen");
    drawStringHorizontal(128, 0, str, RED);
    deathCount = 5;
    stageCount = 1;
}

// Set up the player size and location appear
void setUpPlayer(void) {
    P.row = row + 130;
    P.col = col + 100;
    P.size = 10;
    drawPlayer(&P, GREEN);
}

// Make the enemies auto move
void moveEnemies(void) {
    for (int i = 0; i < NENEMIES; i++) {
        ENEMY *enemy = &enemies[i];
        enemies[i].col = enemy->col + enemy->cd;
        if(enemy->col < 10 && enemy->col > 5) {
            enemy->col = 10;
            enemy->cd = -enemy->cd;
        }
        if(enemy->col > 230) {
            enemy->col = 230;
            enemy->cd = -enemy->cd;
        }
    }
    waitForVBlank();
    for (int i = 0; i < NENEMIES; i++) {
        drawEnemy(&oldenemies[i], BLACK);
    }
    for (int i = 0; i < NENEMIES; i++) {
        drawEnemy(&enemies[i], RED);
        oldenemies[i] = enemies[i];
    }
}

// Run entire the app
void runApp(void) {
    movePlayer();
    moveEnemies();
}

// Move Player around by button
void movePlayer(void) {
    int rd = 0;
    int cd = 0;
    int dist = 2;
    if (KEY_DOWN(BUTTON_DOWN, BUTTONS)) {
        rd = dist;
    } else if (KEY_DOWN(BUTTON_LEFT, BUTTONS)) {
        cd = -dist;
    } else if (KEY_DOWN(BUTTON_RIGHT, BUTTONS)) {
        cd = dist;
    } else if (KEY_DOWN(BUTTON_UP, BUTTONS)) {
        rd = -dist;
    }

    drawPlayer(&P, BLACK);
    P.row += rd;
    P.col += cd;
    drawPlayer(&P, GREEN);

    if (checkIfCollision() == 1) {
        oldDeath = deathCount;
        deathCount--;
        if (deathCount == 0) {
            loseGame = 1;
        }
    }

    // Handle case when player win
    if(P.row < 30) {
        stageCount++;
        if (stageCount == 5) {
            winGame = 1;
        } else {
            nextStage = 1;
        }
    }
}

// Check if the player hit with the items
int checkIfCollision(void) {
    if(videoBuffer[OFFSET(P.row, P.col + P.size/2 + 1, 240)] == RED ||
        videoBuffer[OFFSET(P.row, P.col - P.size/2 - 1, 240)] == RED ||
        videoBuffer[OFFSET(P.row + P.size/2 + 1, P.col, 240)] == RED ||
        videoBuffer[OFFSET(P.row - P.size/2 - 1, P.col, 240)] == RED)
        return 1;
    return 0;
}

// Message to the user if want to replay the game
void replayMessage(void) {
    sprintf(str, "To Replay hit [Z]");
    drawStringHorizontal(130, 80, str, RED);
}

// Clear the welcome screen to move to next sence
void disPlayerScreen(void) {
    drawRectDMA(0, 0, 240, 160, BLACK);
}

// Set up the water pixel "Win Location"
void setUpWater(void) {
    int height = -11;
    int width = 0;
    for (int i = 0; i <= 7; i++) {
        drawWater(height, width, (u16*)water);
        width += 35;
    }
}

// Set up the screen
void setUp(void) {
    u16 size = 18;
    unsigned int enemiestartRow = row + 20;
    setUpWater();
    setUpPlayer();
    int speed = 2;

    // Increse speed if move to next stages
    switch (stageCount) {
        case 1:
            speed = 2;
            break;
        case 2:
            speed += 5;
            break;
        case 3:
            speed += 8;
            break;
        case 4:
            speed += 12;
            break;
    }
    for (int i = 0; i < NENEMIES; i++) {
        enemies[i].row = enemiestartRow + i*20;
        enemies[i].col = 240/2;
        enemies[i].size = size;
        if (i == 4) {
            enemies[i].cd = (i - speed);
        } else if (i % 2 == 0) {
            enemies[i].cd = (i - speed + 1);
        } else if (i % 2 == 1) {
            enemies[i].cd = -(i - speed + 3);
        }
        oldenemies[i] = enemies[i];
    }
    sprintf(str, "Allow: %-1d", deathCount);
    char str1[60];
    sprintf(str1, "Stage: %-1d", stageCount);
    drawStringHorizontal(7, 0, str, RED);
    drawStringHorizontal(7, 190, str1, RED);
}


int main(void) {
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    GBAState state = START;

    // We store the "previous" and "current" states.
    AppState currentAppState, nextAppState;

    while(1) {

        switch(state) {
            case START:
                waitForVBlank();
                startGame();
                state = START_NODRAW;
                break;

            case START_NODRAW:
                if (KEY_DOWN(BUTTON_START, BUTTONS)) {
                    disPlayerScreen();
                    state = APP_INIT;
                }
                break;

            case APP_INIT:
                initializeAppState(&currentAppState);
                fullDrawAppState(&currentAppState);
                disPlayerScreen();
                setUp();
                state = APP;
                break;

            case APP:
                // Process the app for one frame, store the next state
                // nextAppState = processAppState(&currentAppState, previousButtons, currentButtons);

                // Wait for VBlank before we do any drawing.
                waitForVBlank();

                // Undraw the previous state
                undrawAppState(&currentAppState);

                // Draw the current state
                drawAppState(&nextAppState);

                // Now set the current state as the next state for the next iter.
                currentAppState = nextAppState;

                runApp();

                if (nextStage == 1) {
                    state = APP_INIT;
                    nextStage = 0;
                } else if (winGame == 1) {
                    state = APP_WIN;
                } else if (oldDeath != deathCount) {
                    state = APP_INIT;
                    oldDeath = deathCount;
                } else if (loseGame == 1) {
                    state = APP_EXIT;
                }

                //  To start the initial screen
                if (KEY_DOWN(BUTTON_A, BUTTONS)) {
                    state = START;
                }
                break;

            case APP_WIN:
                waitForVBlank();
                drawFullScreenImageDMA((u16*)victory);
                replayMessage();
                if (KEY_DOWN(BUTTON_A, BUTTONS)) {
                    state = START;
                    winGame = 0; // Reset
                }
                break;

            case APP_EXIT:
                waitForVBlank();
                drawFullScreenImageDMA((u16*)gameover);
                drawImageDMA(70, 5, 50, 50, (u16*)logo);
                replayMessage();
                if (KEY_DOWN(BUTTON_A, BUTTONS)) {
                    state = START;
                    loseGame = 0; // Reset
                }
                break;
            }
    }

    return 0;
}
