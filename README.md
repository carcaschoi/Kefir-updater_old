# Atmosphere-Updater

Update Atmosphere patches from within your switch!

![Img](images/example.jpg)

----

## Install

Download the latest release from [here](https://github.com/JackInTheShop/AIO-atmosphere-updater/releases).

Move the downloaded .nro onto you Nintendo Switch SD Card inside the folder **/switch/AIO-atmosphere-updater/**.

That's it!

----

## Usage

__**The app currently has 4 main options.**__

**Update Atmosphere patches:**

* Downloads the latest Atmosphere patches. 
* It is no longer possible to update Atmosphere while you are running Atmosphere. [Atmosphere now maintains exclusive access to fusee-secondary archive.](https://github.com/Atmosphere-NX/Atmosphere/commit/5b02c77400e8551199fefdbc36b1eb3ff86374dc)

**Update Hekate:**

* Installs the latest release of Hekate. The UMS feature may be a convenient way to update Atmosphere now.
* Offers to replace your **/atmosphere/reboot_payload.bin** with Hekate, making Atmosphere reboot to Hekate. It will offer to copy your current **/atmosphere/reboot_payload.bin** to **/bootloader/payloads/reboot_payload.bin** in order to avoid booting to hekate after install with no payloads. If you ran **Update Atmosphere + sigpatches** first, **/atmosphere/reboot_payload.bin** is the latest **fusee-primary.bin**.

![Img](images/reboot_hekate.jpg)

**Update App:**

* Downloads the latest version of this app!
* Deletes the previous old version.

**Reboot (reboot to payload):**

* Reboots the switch, loading the payload from **/atmosphere/reboot_payload.bin**.
* This is recommended after install Atmosphere.

----

## Build

Install the devkitpro tool chain from [here](https://devkitpro.org/wiki/Getting_Started).

 Using pacman (installed with devkitpro) install the following libraries:
* [switch-ex-curl](https://www.github.com/eXhumer/switch-ex-curl/)
* switch-freetype
* switch-sdl2
* switch-sdl2_gfx
* switch-sdl2_image
* switch-sdl2_ttf
* switch-zlib

If you have any problems building, feel free to open an issue including any build errors.
 
----

## Special Thanks!

[ITotalJustice](ITotalJustice) for originally developing the app.

[eXhumer](eXhumer) for recently updating the app with libnx 3.2.0 support, notably adding sigpatches support and the BCT.ini prompt. 

[toph](https://github.com/sudot0ph) for the design of the app icon, as well as the most icons used within the app!

[Black Rabbit](https://github.com/BlackRabbit22) for the design of the plus and error icon (and the idea for this app)!

[bandithedoge](https://github.com/bandithedoge) for the background!

[KrankRival](https://github.com/KranKRival) for the initial sys / ams version pr (and for motivating me to work on touch controls)!
