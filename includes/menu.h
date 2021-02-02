#ifndef _MENU_H_
#define _MENU_H_

#include "sdl.h"

#define CURSOR_LIST_MAX     2
#define HIGHLIGHT_BOX_MIN   20
#define HIGHLIGHT_BOX_MAX   70
#define NEWLINE             110
#define FIRST_LINE          130
#define MAX_STRLEN          512

#define POS_X               390
#define POS_Y               250

#define YES_POS_X           380
#define NO_POS_X            705
#define OK_POS_X            540
#define BT_POS_Y            425 
#define BT_SIZE_W           200
#define BT_SIZE_H           65

void refreshScreen(char loaded);                                            // clear screen, display background with title
void printOptionList(int cursor);                                           // print list of options
void popUpBox(TTF_Font *font, int x, int y, SDL_Colour colour, char *text); // pop-up box in center of screen
int yesNoBox(int cursor, int x, int y, char *question);                     // box with yes or no option
void errorBox(int x, int y, char *errorText);                               // used for displaying error messages

#endif