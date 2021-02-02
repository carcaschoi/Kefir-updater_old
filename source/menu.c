#include <unistd.h>
#include <switch.h>

#include "menu.h"
#include "touch.h"
#include "util.h"

#define APP_VERSION "Kefir Updater: 1.0.0"

void refreshScreen(char loaded)
{
    clearRenderer();

    // app version.
    drawText(appFonts.fntMedium, 40, 40, SDL_GetColour(black), APP_VERSION);

    // system version.
    drawText(appFonts.fntSmall, 25, 150, SDL_GetColour(black), getSysVersion());

    // kefir version.
    drawText(appFonts.fntSmall, 25, 200, SDL_GetColour(black), getKefVersion());

    if (loaded)
    {
      // write the latest version number, if an update is available
      drawText(appFonts.fntSmall, 25, 260, SDL_GetColour(black), getLatestKefirVersion());

      //drawText(appFonts.fntMedium, 120, 225, SDL_GetColour(black), "Menu Here"); // menu options
      drawButton(appFonts.fntButton, BUTTON_A, 970, 672, SDL_GetColour(black));
      drawText(appFonts.fntSmall, 1010, 675, SDL_GetColour(black), "Select");

      drawButton(appFonts.fntButton, BUTTON_PLUS, 1145, 672, SDL_GetColour(black));
      drawText(appFonts.fntSmall, 1185, 675, SDL_GetColour(black), "Exit");
    }
}

void printOptionList(int cursor)
{
    refreshScreen(/*loaded=*/1);

    char *option_list[]      = {    "Update kefir", \
                                    "Update app", \
                                    "Reboot (reboot to payload)" };

    char *description_list[] = {    "Update kefir to latest", \
                                    "Updates app. Restart app to apply.", \
                                    "Reboots switch to your current reboot payload." };

    SDL_Texture *textureArray[] = { appTextures.ams_icon, appTextures.app_icon, appTextures.reboot_icon };

    for (int i=0, nl=0; i < (CURSOR_LIST_MAX+1); i++, nl+=NEWLINE)
    {
        if (cursor != i) drawText(appFonts.fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(black), option_list[i]);
        else
        {
            // icon for the option selected.
            drawImage(textureArray[i], 125, 350);
            // highlight box.
            drawShape(SDL_GetColour(dark_grey), 530, (FIRST_LINE + nl - HIGHLIGHT_BOX_MIN), 700, HIGHLIGHT_BOX_MAX);
            // option text.
            drawText(appFonts.fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(grey), option_list[i]);
            // description.
            drawText(appFonts.fntSmall, 25, 675, SDL_GetColour(black), description_list[i]);
        }
    }
}

void popUpBox(TTF_Font *font, int x, int y, SDL_Colour colour, char *text)
{
    // outline. box
    drawShape(SDL_GetColour(black), (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box.
    drawShape(SDL_GetColour(not_so_dark_grey), SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
    // text to draw.
    drawText(font, x, y, colour, text);
}

void no_button(){
    drawShape(SDL_GetColour(dark_grey), YES_POS_X, BT_POS_Y, BT_SIZE_W, BT_SIZE_H);
    drawButton(appFonts.fntButtonBig, BUTTON_A, (YES_POS_X+42), (BT_POS_Y+15), SDL_GetColour(grey));
    drawText(appFonts.fntMedium, (YES_POS_X+91), (BT_POS_Y+15), SDL_GetColour(grey), "Yes");
}
void yes_button(){
    drawShape(SDL_GetColour(dark_grey), NO_POS_X, BT_POS_Y, BT_SIZE_W, BT_SIZE_H);
    drawButton(appFonts.fntButtonBig, BUTTON_B, (NO_POS_X+50), (BT_POS_Y+15), SDL_GetColour(grey));
    drawText(appFonts.fntMedium, (NO_POS_X+97), (BT_POS_Y+15), SDL_GetColour(grey), "No");
}
void ok_button(){
    drawShape(SDL_GetColour(dark_grey), OK_POS_X, BT_POS_Y, BT_SIZE_W, BT_SIZE_H);
    drawButton(appFonts.fntButtonBig, BUTTON_A, (OK_POS_X+50), (BT_POS_Y+15), SDL_GetColour(grey));
    drawText(appFonts.fntMedium, (OK_POS_X+97), (BT_POS_Y+15), SDL_GetColour(grey), "OK");
}

int yesNoBox(int cursor, int x, int y, char *question)
{
    printOptionList(cursor);
    popUpBox(appFonts.fntMedium, x, y, SDL_GetColour(black), question);

    no_button();
    yes_button();
    updateRenderer();

    int res = 0;
    int touch_lock = OFF;
    touchPosition touch;
    u32 tch = 0;
    u32 touch_count = hidTouchCount();

    // check if the user is already touching the screen.
    if (touch_count > 0) touch_lock = ON;

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidTouchRead(&touch, tch);
        touch_count = hidTouchCount();

        if (touch_count == 0) touch_lock = OFF;

        if (touch_count > 0 && touch_lock == OFF)
            res = touch_yes_no_option(touch.px, touch.py);

        if (kDown & KEY_A || res == YES)
            return YES;

        if (kDown & KEY_B || res == NO)
            return NO;
    }
}

void errorBox(int x, int y, char *errorText)
{
    popUpBox(appFonts.fntMedium, x, y, SDL_GetColour(black), errorText);
    drawImageScale(appTextures.error_icon, 570, 340, 128, 128);
    updateRenderer();

    sleep(3);
}