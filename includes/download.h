#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define AMS_URL     "https://api.github.com/repos/JackInTheShop/Atmosphere/releases"
#define HEKATE_URL  "https://api.github.com/repos/CTCaer/hekate/releases"
#define PATCH_URL   "https://github.com/ITotalJustice/patches/releases/latest/download/fusee.zip"
#define APP_URL     "https://github.com/JackInTheShop/AIO-atmosphere-updater/releases/latest/download/AIO-atmosphere-updater.nro"

int downloadFile(const char *url, const char *output, int api_mode);

#endif