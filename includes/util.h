#ifndef _UTIL_H_
#define _UTIL_H_

#define ROOT                    "/"
#define APP_PATH                "/switch/kefirupdater/"
#define AMS_OUTPUT              "/switch/kefirupdater/ams.zip"
#define HEKATE_OUTPUT           "/switch/kefirupdater/hekate.zip"
#define KEFIR_OUTPUT            "/switch/kefirupdater/kefir.zip"
#define KEF_LOCAL               "/switch/kefirupdater/version"
#define APP_OUTPUT              "/switch/kefirupdater/kefirupdater.nro"
#define OLD_APP_PATH            "/switch/kefirupdater.nro"

#define YES                     10
#define NO                      20
#define ON                      1
#define OFF                     0

void writeSysVersion();                                                 // writes the sys version.
// void writeAmsVersion();                                                 // writes the ams version with hash.
void writeLatestKefirVersion();                                    // writes the latest patchable Atmosphere version.
void writeKefVersion();                                                 // writes the kefir version
char *getSysVersion();                                                  // returns sys version.
char *getAmsVersion();                                                  // returns ams version.
char *getLatestKefirVersion();                                    	   // returns the latest Kefir version.
char *getKefVersion();                                                  // returns ams version.

void copyFile(char *src, char *dest);                                   // basic copy file. Use malloc if you need dynamic mem
int parseSearch(char *phare_string, char *filter, char* new_string);    // hacky way to parse a file for a string
// void update_hekate();                                                   // update hekate
void update_kefir(int cursor);                                     // update sigpatches
// void update_app();                                                      // update the app

#endif