#include <stdio.h>
#include <string.h>
#include <switch.h>

#include "util.h"
#include "menu.h"
#include "unzip.h"
#include "download.h"
#include "reboot_payload.h"

#define TEMP_FILE "/switch/kefirupdater/temp"
#define FILTER_STRING "browser_download_url\":\""
#define VERSION_FILTER_STRING "tag_name\":\""

char g_sysVersion[50];
char g_kefVersion[50];
char g_amsVersionWithoutHash[15];
char g_latestKefirVersion[50];

char *getSysVersion() { return g_sysVersion; }
char *getKefVersion() { return g_kefVersion; }
char *getLatestKefirVersion() { return g_latestKefirVersion; }

void writeSysVersion()
{
    Result ret = 0;
    SetSysFirmwareVersion ver;

    if (R_FAILED(ret = setsysGetFirmwareVersion(&ver)))
    {
        printf("GetFirmwareVersion() failed: 0x%x.\n\n", ret);
        return;
    }

    char sysVersionBuffer[20];
    snprintf(sysVersionBuffer, 20, "%u.%u.%u", ver.major, ver.minor, ver.micro);
    snprintf(g_sysVersion, sizeof(g_sysVersion), "Firmware Ver: %s", sysVersionBuffer);
}

void writeKefVersion()
{

    FILE *fp;
    char kefir_local[128];
    if ((fp = fopen(KEF_LOCAL, "r")) == NULL)
    {
        printf("Cannot open file.\n");
        exit(1);
    }
    while (!feof(fp))
    {
        fgets(kefir_local, 126, fp);
    }
    fclose(fp);

    snprintf(g_kefVersion, sizeof(g_kefVersion), "Kefir: %s", kefir_local);
}

void writeLatestKefirVersion()
{

    typedef enum
    {
        F,
        T
    } boolean;

    // Download the github API file and then parse out the version number.
    char *updateString = "- Up to date";
    boolean is_update = F;

    if (!downloadFile(AMS_URL, TEMP_FILE, ON))
    {
        char latestVersionNumber[10];
        if (!parseSearch(TEMP_FILE, VERSION_FILTER_STRING, latestVersionNumber))
        {
            char buffer[50];
            snprintf(buffer, sizeof(buffer), "- Update available: %s", latestVersionNumber);

            // char kefirUpdateString = "kefir%s available. Update?", latestVersionNumber;

            // if (yesNoBox(UP_KEFIR, 360, 250, "New kefir version available. Update?") == YES){update_kefir(UP_KEFIR);}
            is_update = T;
            updateString = buffer;
        }
    }
    else
    {
        errorBox(350, 250, "Check internet connection");
        updateString = "Check internet connection";
    }
    snprintf(g_latestKefirVersion, sizeof(g_latestKefirVersion), updateString);
}

void copyFile(char *src, char *dest)
{
    FILE *srcfile = fopen(src, "rb");
    FILE *newfile = fopen(dest, "wb");

    if (srcfile && newfile)
    {
        char buffer[10000]; // 10kb per write, which is fast
        size_t bytes;       // size of the file to write (10kb or filesize max)

        while (0 < (bytes = fread(buffer, 1, sizeof(buffer), srcfile)))
        {
            fwrite(buffer, 1, bytes, newfile);
        }
    }
    fclose(srcfile);
    fclose(newfile);
}

int parseSearch(char *parse_string, char *filter, char *new_string)
{
    FILE *fp = fopen(parse_string, "r");

    if (fp)
    {
        char c;
        while ((c = fgetc(fp)) != EOF)
        {
            if (c == *filter)
            {
                for (int i = 0, len = strlen(filter) - 1; c == filter[i]; i++)
                {
                    c = fgetc(fp);
                    if (i == len)
                    {
                        for (int j = 0; c != '\"'; j++)
                        {
                            new_string[j] = c;
                            new_string[j + 1] = '\0';
                            c = fgetc(fp);
                        }
                        fclose(fp);
                        remove(parse_string);
                        return 0;
                    }
                }
            }
        }
    }

    errorBox(350, 250, "Failed to find parse url!");
    fclose(fp);
    return 1;
}

void update_kefir(int cursor)
{
    popUpBox(appFonts.fntMedium, 350, 250, SDL_GetColour(white), "Downloading kefir...");
    drawImageScale(appTextures.error_icon, 570, 340, 128, 128);
    updateRenderer();
    unzip(KEFIR_OUTPUT, cursor);

    // if (!downloadFile(KEFIR_URL, KEFIR_OUTPUT, OFF))
    // {
    //     unzip(KEFIR_OUTPUT, cursor);
    //     remove(KEFIR_OUTPUT);
    //     errorBox(400, 250, "      Update complete!\nReboot Switch to take effect!");
    //     if (yesNoBox(cursor, 390, 250, "Reboot console") == YES)
    //     reboot_payload("/payload.bin");
    // }
}

// void update_app()
// {
//     popUpBox(appFonts.fntMedium, 350, 250, SDL_GetColour(white), "Downloading app update...");
//     drawImageScale(appTextures.error_icon, 570, 340, 128, 128);
//     updateRenderer();
//     // download new nro as a tempfile.
//     if (!downloadFile(APP_URL, TEMP_FILE, OFF))
//     {
//         // remove current nro file.
//         remove(APP_OUTPUT);
//         // remove nro from /switch/.
//         remove(OLD_APP_PATH);
//         // rename the downloaded temp_file with the correct nro name.
//         rename(TEMP_FILE, APP_OUTPUT);
//         // using errorBox as a message window on this occasion.
//         errorBox(400, 250, "      Update complete!\nRestart app to take effect!");
//     }
// }
