#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define AMS_URL    	 		"https://api.github.com/repos/rashevskyv/kefir/releases"
#define KEFIR_URL 	 		"https://github.com/rashevskyv/kefir/releases/latest/download/kefir.zip"
#define APP_URL   	 	 	"https://github.com/JackInTheShop/kefirupdater/releases/latest/download/kefirupdater.nro"


int downloadFile(const char *url, const char *output, int api_mode);

#endif