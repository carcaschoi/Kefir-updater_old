#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define KEF_URL    	 		"https://api.github.com/repos/carcaschoi/ShallowSea/releases"
#define KEFIR_URL 	 		"https://github.com/carcaschoi/ShallowSea/releases/latest/download/ShallowSea-ams.zip"
#define APP_URL   	 	 	"https://github.com/carcaschoi/ShallowSea/releases/latest/download/ShallowSea-updater.nro"


int downloadFile(const char *url, const char *output, int api_mode);

#endif
