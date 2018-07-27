# minimum_ps_plugin

Very simple Photoshop Plugin example.

## Description

Just modify RGB level independently.
No parameter.
No GUI.
Windows only.
Support RGB8bit only.

## Requirement

- Visual Studio 2017
- Photoshop CC 2017 or above
- adobe_photoshop_sdk_cc_2017_win (From Adobe site. Registration is needed to download.)

## Build
- Open Makefile then modify SPECIFY_PATH_TO_PLUGIN_SDK to "pluginsdk" directory of your Photoshop SDK, then save.
- Launch x64 command prompt of Visual Studio 2017
- Move to top directory
- Make "output" directory
- nmake
- output/minimum.8bf plugin will be generated.

## Usage

- Copy minimum.8bf to C:/Program Files/Adobe/Adobe Photoshop CC 2017/Plug-ins/ANY_SUB_DIRECTORY
- Launch Photoshop2017
- Open any RGB8bit image.
- In Filter menu, select "minimum"
- Then simple image processing will be done.

## Author

delphinus1024

## License

[MIT](https://raw.githubusercontent.com/delphinus1024/minimum_ps_plugin/master/LICENSE.txt)

