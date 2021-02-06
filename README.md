# Kefir Updater

Update [Kefir](https://github.com/rashevskyv/kefir/) directly from console

![Img](images/menu.jpg)

----

## Install

Download the latest release from [here](https://github.com/rashevskyv/kefirupdater/releases).

Move the downloaded `.nro` onto you Nintendo Switch SD Card inside the folder **/switch/kefirupdater/**.

That's it!

**Do not use with SD formatted to exFAT!!**

----

## Usage

__**The app currently has 2 options.**__

**Update Kefir:**

* Downloads the latest [Kefir](https://github.com/rashevskyv/kefir/releases) release. 

![Img](images/update.jpg)

**Reboot Console:**

* Reboots the switch, loading the payload from **/payload.bin**.
* This is recommended after install Atmosphere.

![Img](images/reboot.jpg)

----

## Build

Install the devkitpro tool chain from [here](https://devkitpro.org/wiki/Getting_Started).

 Using pacman (installed with devkitpro) install the following libraries:
* [switch-ex-curl](https://www.github.com/eXhumer/switch-ex-curl/)
	* `sudo dkp-pacman -U https://github.com/eXhumer/switch-ex-curl/releases/download/v7.69.1-3/switch-ex-curl-7.69.1-3-any.pkg.tar.zst`
* switch-freetype
* switch-sdl2
* switch-sdl2_gfx
* switch-sdl2_image
* switch-sdl2_ttf
* switch-zlib

If you have any problems building, feel free to open an issue including any build errors.

----

## TODO

1. Download firmware 
1. Redisign
1. Refactoring

----

## Special Thanks!

[ITotalJustice](https://github.com/ITotalJustice) for originally developing the app.
[JackInTheShop](https://github.com/JackInTheShop) for fork that I forked.
[eXhumer](https://github.com/eXhumer) for recently updating the app with libnx 3.2.0 support, notably adding sigpatches support and the BCT.ini prompt. 
[toph](https://github.com/sudot0ph) for the design of the app icon, as well as the most icons used within the app!
[Black Rabbit](https://github.com/BlackRabbit22) for the design of the plus and error icon (and the idea for this app)!
[bandithedoge](https://github.com/bandithedoge) for the background!
[KrankRival](https://github.com/KranKRival) for the initial sys / ams version pr (and for motivating me to work on touch controls)!
[duckbill](https://github.com/duckbill007) for proper realloc realisation. 
[developersu](https://github.com/developersu) for remove directory realisation.