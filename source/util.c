#include <stdio.h>
#include <string.h>
#include <switch.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>

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

    snprintf(g_kefVersion, sizeof(g_kefVersion), "Kefir version: %s", kefir_local);
}

void writeLatestKefirVersion()
{
    // Download the github API file and then parse out the version number.
    char *updateString = "- Up to date";

    if (!downloadFile(AMS_URL, TEMP_FILE, ON))
    {
        char latestVersionNumber[10];
        if (!parseSearch(TEMP_FILE, VERSION_FILTER_STRING, latestVersionNumber))
        {
            char buffer[50];
            snprintf(buffer, sizeof(buffer), "     - Update available: %s", latestVersionNumber);

            // char kefirUpdateString = "kefir%s available. Update?", latestVersionNumber;

            // if (yesNoBox(UP_KEFIR, 360, 250, "New kefir version available. Update?") == YES){update_kefir(UP_KEFIR);}
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

// Check if it's dir on 'path'
int is_dir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

// Remove all the shit. Please note that there should be no '/' char in the end. Otherwise update this snippet.
int remove_entry(const char *dir_name)
{
    if (!is_dir(dir_name))
        return unlink(dir_name);

    DIR *dir = opendir(dir_name);

    if (dir == NULL)
        return 1;

    size_t dSz = strlen(dir_name);
    struct dirent *s_dirent;
    char *full_name;

    while ((s_dirent = readdir(dir)) != NULL)
    {
        if ((strcmp(s_dirent->d_name, ".") == 0) || (strcmp(s_dirent->d_name, "..") == 0))
            continue;
        full_name = malloc(dSz + strlen(s_dirent->d_name) + 2); // '/'+'\0'

        strcpy(full_name, dir_name);
        strcat(full_name, "/");
        strcat(full_name, s_dirent->d_name);

        if (s_dirent->d_type == DT_DIR)
            remove_entry(full_name); // NOTE: Handle returning value
        else
            unlink(full_name); // NOTE: Add validation

        free(full_name);
    }

    closedir(dir);

    return rmdir(dir_name); // NOTE: Add validation
}

void remove_old()
{
    remove_entry("/atmosphere/exefs_patches");
    remove_entry("/atmosphere/kip_patches");
    remove_entry("/atmosphere/hekate_kips");
    remove_entry("/bootloader/debug");
    remove_entry("/modules");

    remove_entry("/atmosphere/contents/0100000000000032");
    remove_entry("/atmosphere/contents/0100000000000034");
    remove_entry("/atmosphere/contents/0100000000000037");
    remove_entry("/atmosphere/contents/0100000000000036");
    remove_entry("/atmosphere/contents/010000000000002b");
    remove_entry("/atmosphere/contents/010000000000000D");
    remove_entry("/atmosphere/contents/010000000000100D");
    remove_entry("/atmosphere/contents/4200000000000010");
    remove_entry("/atmosphere/contents/0100000000000008");
    remove_entry("/atmosphere/contents/690000000000000D");
    remove_entry("/atmosphere/contents/420000000000000E");
    remove_entry("/atmosphere/contents/010000000000100B");
    remove_entry("/atmosphere/contents/01FF415446660000");
    remove_entry("/atmosphere/contents/0100000000000352");
    remove_entry("/atmosphere/contents/00FF747765616BFF");
    remove_entry("/atmosphere/contents/00FF0012656180FF");
    remove_entry("/atmosphere/contents/0100000000001013");
    remove_entry("/atmosphere/contents/010000000007E51A");
    remove_entry("/atmosphere/contents/420000000007E51A");
    remove_entry("/atmosphere/contents/0100000000001000");
    remove_entry("/sxos/titles/");
    remove_entry("/sxos/bootloader");
    remove_entry("/sxos/switch");
    remove_entry("/sxos/exefs_patches");
    remove_entry("/sept");
    remove_entry("/switch/lithium");
    remove_entry("/switch/tinfoil");
    remove_entry("/switch/KosmosToolbox");
    remove_entry("/switch/KosmosUpdater");
    remove_entry("/switch/mercury");
    remove_entry("/switch/NX-Activity-Log/");
    remove_entry("/switch/ldnmitm_config");
    remove_entry("/switch/LinkUser/");
    remove_entry("/switch/nxmtp/");
    remove_entry("/switch/incognito");
    remove_entry("/switch/fakenews-injector");
    remove_entry("/switch/ChoiDujourNX");
    remove_entry("/pegascape");
    remove_entry("/sxos/sx");

    remove("/hbmenu.nro");
    remove("/switch/checkpoint.nro");
    remove("/switch/checkpoint/checkpoint.nro");
    remove("/switch/pplay.nro");
    remove("/switch/NX-SHELL.nro");
    remove("/switch/kefirupdater/kefirupdater.nro");
    remove("/switch/reboot_to_payload.nro");
    remove("/switch/NxThemesInstaller.nro");
    remove("/switch/NxThemesInstaller/NxThemesInstaller.nro");
    remove("/switch/sx.nro");
    remove("/switch/n1dus.nro");
    remove("/switch/ChoiDujourNX.nro");
    remove("/switch/ChoiDujourNX/ChoiDujourNX.nro");
    remove("/switch/kefirupdater/kefirupdater.nro");
    remove("/switch/daybreak.nro");
    remove("/switch/daybreak/daybreak.nro");
    remove("/switch/nx-ntpc.nro");
    remove("/switch/dbi.nro");
    remove("/switch/dbi/dbi.nro");
    remove("/switch/nxmtp.nro");
    remove("/switch/NX-Activity-Log.nro");
    remove("/switch/incognito.nro");
    remove("/switch/ultimate_updater.nro");
    remove("/switch/zerotwoxci.nro");
    remove("/switch/dOPUS.nro");
    remove("/switch/tinfoil.nro");
    remove("/switch/tinfoil_batch.nro");
    remove("/switch/tinfoil_duckbill.nro");
    remove("/switch/tinfoil_usb-fix.nro");
    remove("/switch/ldnmitm_config.nro");
    remove("/switch/fakenews-injector.nro");
    remove("/switch/gag-order.nro");
    remove("/switch/tinfoil/keys.txt");
    remove("/switch/sx/locations.conf");
    remove("/switch/.overlays/emuiibo.ovl");

    remove("/atmosphere/fusee-secondary_atmo.bin");
    remove("/atmosphere/hbl_atmo.nsp");
    remove("/atmosphere/fusee-secondary.bin.sig");
    remove("/atmosphere/hbl.nsp.sig");
    remove("/atmosphere/hbl.json");
    remove("/atmosphere/BCT.ini");
    remove("/atmosphere/system_settings.ini");
    remove("/atmosphere/system_settings.ini");
    remove("/atmosphere/kips/fs_mitm.kip");
    remove("/atmosphere/kips/ldn_mitm.kip");
    remove("/atmosphere/kips/loader.kip");
    remove("/atmosphere/kips/pm.kip");
    remove("/atmosphere/kips/sm.kip");
    remove("/atmosphere/kips/ams_mitm.kip");
    remove("/atmosphere/flags/hbl_cal_read.flag");
    remove("/atmosphere/exosphere.bin");
    remove("/atmosphere/hbl.nsp");
    remove("/atmosphere/loader.ini");
    remove("/atmosphere/reboot_payload.bin");
    remove("/atmosphere/BCT.ini");
    remove("/atmosphere/fusee-secondary.bin");

    remove("/bootloader/payloads/fusee-primary-payload.bin");
    remove("/bootloader/payloads/Lockpick_RCM.bin");
    remove("/bootloader/payloads/biskeydump.bin");
    remove("/bootloader/payloads/fusee-payload.bin");
    remove("/bootloader/payloads/fusee-primary.bin");
    remove("/bootloader/payloads/sxos.bin");
    remove("/bootloader/payloads/rajnx_ipl.bin");
    remove("/bootloader/ini/Atmosphere.ini");
    remove("/bootloader/ini/atmosphere.ini");
    remove("/bootloader/ini/sxos.ini");
    remove("/bootloader/ini/hekate_keys.ini");
    remove("/bootloader/ini/RajNX.ini");
    remove("/bootloader/hekate_ipl.ini");
    remove("/bootloader/update.bin");
    remove("/bootloader/update.bin.sig");
    remove("/bootloader/patches_template.ini");
    remove("/bootloader/patches.ini");
    remove("/bootloader/bootlogo.bmp");
    remove("/bootloader/res/icon_payload.bmp");
    remove("/bootloader/res/icon_switch.bmp");
    remove("/bootloader/payloads/rajnx_ipl.bin");

    remove("/games/hbgShop_forwarder_classic.nsp");
    remove("/games/hbgShop_forwarder_dark_v3.nsp");
    remove("/license-request.dat");
    remove("/fusee-secondary.bin");
    remove("/sxos/boot.dat");
    remove("/boot.dat");
    remove("/hekate*.bin");
    remove("/keys.dat");
    remove("/BCT.ini");
    remove("/hekate_ipl.ini");
    remove("/exosphere.ini");
}

void update_kefir(int cursor)
{
    updateRenderer();
    // if (yesNoBox(cursor, 390, 250, "Remove old?") == YES)
    // if (yesNoBox(cursor, 390, 250, "Update kefir?") == YES)
    //     unzip(KEFIR_OUTPUT, cursor);

    if (!downloadFile(KEFIR_URL, KEFIR_OUTPUT, OFF))
    {
        remove_old();
        unzip(KEFIR_OUTPUT, cursor);
        remove(KEFIR_OUTPUT);
        infoBox(POS_X-10, POS_Y, "       Update complete!\nReboot Switch to take effect!");
        if (yesNoBox(cursor, 390, 250, "Reboot console") == YES)
        reboot_payload("/payload.bin");
    }
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
