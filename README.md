# AmxModX Plugin Repo Template

Template AmxModX plugin package for building on Windows.

## `.build-config`

Config for building script `build.bat`.

```ini
; Name of output .zip file
PACKAGE_NAME=Unnemad

; Path from project root to amxmodx folder
PACKAGE_AMXMODX_FOLDER=amxmodx

; Should add README.md file to .zip
PACKAGE_README_USE=0

; Should add compiled plugins (.amxx files) to .zip
PACKAGE_COMPILED_PLUGINS_USE=1
; Should save compiled plugins in project file after build
PACKAGE_COMPILED_PLUGINS_SAVE=1

; Should generate plugins-*.ini file and add it to .zip
PACKAGE_PLUINGS_LIST_USE=0
; Should postfix of plugins-*.ini file (pastes instead *)
PACKAGE_PLUINGS_LIST_POSTFIX=unnamed
; Should save plugins-*.ini file in project file after build
PACKAGE_PLUINGS_LIST_SAVE=0

; Should add assets to .zip file
PACKAGE_ASSETS_USE=0
; Path from project root to folder with assets files
PACKAGE_ASSETS_FOLDER=assets
```

## Building

For building you must install AmxModX compiler by [AmxxCompilerInstaller](https://github.com/ArKaNeMaN/batch-AmxxCompilerInstaller).

Also, may be required download depended .inc files.
