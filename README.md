# GTA-V-Launcher


READ THIS IF YOU HAVE UPDATED FROM 1.0.6 to 1.0.7. The mods you disabled won't appear anymore due to folder change.
To resolve this, go to your game directory (you can do this by launching the new version of this launcher then, settings > Open GTA V Game Directory)
Then go to Launcher\disabledMods. Copy all the files in this directory then go back to the game directory and paste those here.
Get in the launcher and go to "Choose Mods" > Reset from config > Confirm > Yes > No
Then, you can delete the Launcher directory.

If you downloaded 1.0.7, there is a bug with steam version. To quick fix this, create an empty PlayGTAV.exe file in your GTAV directory then start the launcher

Every released version is digitally signed with a certificate given to bigcoding.com. 
Its fingerprint is 1ead2bb682d89753d68789e02e6b8e63da7571dc
From 1.0.10, Issuer name is moffa13.com. and the fingerprint is 13dff05f4e35015240ea02a10379252041372da3

Changelog :

V 1.0.3
- Added compatibility for cracked launchers (set it in settings)
- Added auto updater (Launcher can update itself) and added a check for updates button
- Reduced executable size (~5MB)
- Fixed some bugs

V 1.0.4 
- Added play offline contextual menu (by right-clicking in the play gta button)
- Fixed some bugs
- Translated "yes" and "no" buttons

V 1.0.5
- Full steam support
- Full Online mode support, mods are completly disabled so NO RISK to be banned

V 1.0.6
- Fixed a bug that you can't start gta v steam version

V [1.0.7]
- Translations
- Added useful buttons in settings (kill gta process, open/change gta directory)
- Added button to enable/disable all mods directly
- Launcher now automatically finds your game
- Show mods versions after the name if any
- Resets the old mods state when getting back to offline (no need to re-enable the mods)
- The launcher can auto update ScriptHookV & dinput8.dll
- Fixed many bugs

V [1.0.8]
- Fixed bugs (memory leaks, scripthookv says out-of-date, yes-no buttons not translated, can't remove a mod with pe version, ...)
- More security to prevent mods from being accidentally enabled in online mode
- Problem detecting steam version of the game (PlayGTAV.exe doesn't exist with steam)
- Added an option to close the launcher when game starts
- Added functionality to install a mod from asi, dll, zip, rar also copying its config files by detecting them
- Added button in settings to uninstall the launcher
- Added some ui infos (gta version, gta current directory)

V [1.0.9]
- Fixed bugs (crash installing a mod, no version in mod list ...)
- Fixed bug in ScripthookV auto update
- Added option to disable updates auto check in settings
- Added warning if you overwrite an existing mod
- If you run an outdated game version, the launcher will block updates from Rockstar

V 1.0.10
- Missing translations
- Changed executable signature to moffa13.com


[1.0.7]: https://github.com/moffa13/GTA-V-Launcher/releases/tag/1.0.7
[1.0.8]: https://github.com/moffa13/GTA-V-Launcher/releases/tag/1.0.8
[1.0.9]: https://github.com/moffa13/GTA-V-Launcher/releases/tag/1.0.9