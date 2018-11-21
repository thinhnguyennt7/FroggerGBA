#include "gba.h"
#include "logic.h"
#include "graphics.h"
#include "images/frogger.h"
#include "images/gameover.h"
#include "images/victory.h"
#include "images/water.h"
#include <stdio.h>
#include <stdlib.h>

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    UNUSED(appState);
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// void startGame() {
//     sprintf(str, "Press Enter To Start Game");
    // drawFullScreenImageDMA(frogger);
//     drawString(80, 60, str, GREEN);
//     state = START;
//     deathCount = 0; // Inilize = 0
// }

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);

    return nextAppState;
}

void drawEnemy(ENEMY* enemy, unsigned short color) {
    volatile unsigned short Ecolor = color;
    u16 size = enemy -> size;
    drawRectDMA(enemy -> row - size/2, enemy -> col - size/2, size, size, Ecolor);
}

void drawPlayer(PLAYER* player, unsigned short insideP) {
    volatile unsigned short inside = insideP;
    u16 size = player -> size;
    drawRectDMA(player -> row - size/2, player -> col - size/2, size, size, inside);
}

void erasePlayer(PLAYER* player, unsigned short insideP) {
    volatile unsigned short inside = insideP;
    u16 size = player -> size;
    drawRectDMA(player -> row - size/2, player -> col - size/2, size, size, inside);
}

void drawWater(int row, int col, u16* image) {
    drawImageDMA(row,col, 35, 35, image);
}