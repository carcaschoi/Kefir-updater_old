#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define AMS_URL     "https://api.github.com/repos/JackInTheShop/Atmosphere/releases"
#define HEKATE_URL  "https://api.github.com/repos/CTCaer/hekate/releases"
#define PATCH_URL   "https://www.googleapis.com/drive/v3/files/10j-9vr2ByLsWU-1eifryyse3rY_sBSJK/?key=apikey&alt=media"
#define APP_URL     "https://www.googleapis.com/drive/v3/files/1WRnwLCXCc3s_4AYI8h6KvBOsQLTSdJ6a/?key=apikey&alt=media"

int downloadFile(const char *url, const char *output, int api_mode);

#endif