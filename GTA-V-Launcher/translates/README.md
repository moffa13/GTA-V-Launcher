# Adding a new language

Go to **GTA-V-Launcher/translates**, run update.bat and copy **fr.ts** to your language code (in IETF language tag).
For example, if you want to translate it in german, you will call the file **de_XX.ts**
where **XX** (uppercase) is the place where the language is spoken. 
You can decide either if you want to specify it or not. If not, simply call it **de.ts**

Edit compile_lang.bat, copy the last line and change the **.qm** file to yours.

You can now make your translations. Once finished, call compile_lang.bat.

Go to **GTA-V-Launcher/translates/flags** and add a **68x48px png** file containing the language's flag accordingly.

Go to **GTA-V-Launcher/src/TranslatorAliases.cpp** and add to the **_aliases** variable every aliases for your language if there's any.
For example, if you decide that **de** is the same as **de_CH**, add *{"de_CH",  "de"}* to the entries. This will make people speaking german from Switzerland also have your translation.

Also add your language to the **_trFilesToString** variable like so *{"Deutsch",  "de"}*. The name of your language should not be in english but in the native language.

Go to **GTA-V-Launcher/GTA-V-Launcher.qrc** and add an entry for your language 
e.g. `<file>translates/flags/de.png</file>`
and `<file>translates/de.qm</file>`

Go to **GTA-V-Launcher/GTA-V-Launcher.pro** and add your language to the **TRANSLATIONS** variable.
e.g. *translates/de.ts*

You can now compile the project and the newly added language should appear.