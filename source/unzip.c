#include <stdio.h>
#include <minizip/unzip.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>

#include "util.h"
#include "unzip.h"
#include "menu.h"

#define WRITEBUFFERSIZE 500000 // 500KB
#define MAXFILENAME 256

int unzip(const char *output, int cursor)
{
    unzFile zfile = unzOpen(output);
    unz_global_info gi;
    unzGetGlobalInfo(zfile, &gi);

    for (int i = 0; i < gi.number_entry; i++)
    {
        printOptionList(cursor);
        popUpBox(appFonts.fntMedium_small, POS_X, POS_Y, SDL_GetColour(black), "Unzipping...");

        char filename_inzip[MAXFILENAME];
        unz_file_info file_info;

        unzOpenCurrentFile(zfile);
        unzGetCurrentFileInfo(zfile, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

        // check if the string ends with a /, if so, then its a directory.
        if ((filename_inzip[strlen(filename_inzip) - 1]) == '/')
        {
            // check if directory exists
            DIR *dir = opendir(filename_inzip);
            if (dir)
                closedir(dir);
            else
            {
                drawText(appFonts.fntTiny, POS_X, POS_Y + 100, SDL_GetColour(black), filename_inzip);
                mkdir(filename_inzip, 0777);
            }
        }

        else
        {
            // const int MAX_LENGTH = 45;
            // const int LAST_PART = MAX_LENGTH / 2 - 5;
            char write_filename[46];
            // const char FILLER[5] = " ... ";

            if (strlen(filename_inzip) < 50)
            {
                strncpy(write_filename, filename_inzip, 44);
                write_filename[45] = '\0';
            }
            else
            {
                strncpy(write_filename, filename_inzip, 22);
                strncpy(&(write_filename[22]), " ... ", 5);
                strncpy(&(write_filename[27]), &(filename_inzip[strlen(filename_inzip) - 18]), 19);
                write_filename[45] = '\0';
            }

            void *buf = malloc(WRITEBUFFERSIZE);

            FILE *outfile = fopen(write_filename, "wb");

            drawText(appFonts.fntTiny, POS_X - 50, POS_Y + 100, SDL_GetColour(black), write_filename);

            for (int j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE); j > 0; j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE))
                fwrite(buf, 1, j, outfile);

            fclose(outfile);
            free(buf);
        }

        updateRenderer();

    jump_to_end: // goto
        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
    }

    unzClose(zfile);
    // remove(output);
    return 0;
}
