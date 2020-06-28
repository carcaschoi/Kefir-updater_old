#include <stdio.h>
#include <minizip/unzip.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>

#include "util.h"
#include "unzip.h"
#include "menu.h"

#define WRITEBUFFERSIZE 500000 // 500KB
#define MAXFILENAME     256

int unzip(const char *output, int cursor)
{
    unzFile zfile = unzOpen(output);
    unz_global_info gi;
    unzGetGlobalInfo(zfile, &gi);

    for (int i = 0; i < gi.number_entry; i++)
    {
        printOptionList(cursor);
        popUpBox(appFonts.fntSmall, 350, 250, SDL_GetColour(white), "Unzipping...");

        char filename_inzip[MAXFILENAME];
        unz_file_info file_info;

        unzOpenCurrentFile(zfile);
        unzGetCurrentFileInfo(zfile, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

        // prevents /config/BCT.ini from be overwritten
        if (strstr(filename_inzip, "/config/BCT.ini"))
        {
            FILE *f = fopen(filename_inzip, "r");
            if (f)
            {
                if(yesNoBox(cursor, 390, 250, "Overwrite BCT.ini?") == NO)
                {
                    fclose(f);
                    goto jump_to_end;
                }
            }
            fclose(f);
        }

        // check if the string ends with a /, if so, then its a directory.
        if ((filename_inzip[strlen(filename_inzip) - 1]) == '/')
        {
            // check if directory exists
            DIR *dir = opendir(filename_inzip);
            if (dir) closedir(dir);
            else
            {
                drawText(appFonts.fntSmall, 325, 350, SDL_GetColour(white), filename_inzip);
                mkdir(filename_inzip, 0777);
            }
        }

        else
        {
            const char *write_filename = filename_inzip;
            void *buf = malloc(WRITEBUFFERSIZE);

            FILE *outfile = fopen(write_filename, "wb");

            drawText(appFonts.fntSmall, 350, 350, SDL_GetColour(white), write_filename);

            for (int j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE); j > 0; j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE))
                fwrite(buf, 1, j, outfile);

            fclose(outfile);
            free(buf);
            // Reboot to hekate?
            if (strstr(filename_inzip, "hekate_ctcaer_"))
            {
                if(yesNoBox(cursor, 325, 250, "Copy Hekate to reboot_payload?") == YES)
                {
                    if (yesNoBox(UP_HEKATE, 325, 250, "Copy current payload to payloads?") == YES)
                    {
                        // check if directory exists
                        DIR *dir = opendir("/bootloader/payloads/");
                        if (dir) closedir(dir);
                        else
                        {
                            mkdir("/bootloader/", 0777);
                            mkdir("/bootloader/payloads/", 0777);
                        }
                        copyFile("/atmosphere/reboot_payload.bin", "/bootloader/payloads/reboot_payload.bin");
                        errorBox(330, 250, "Added reboot_payload to hekate");
                    }
                    copyFile(filename_inzip,"/atmosphere/reboot_payload.bin");
                    errorBox(330, 250, "Added hekate to reboot_payload");
                }
            }
        }

        updateRenderer();

        jump_to_end: // goto
        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
    }

    unzClose(zfile);
    remove(output);
    return 0;
}
