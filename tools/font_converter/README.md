# Font Converter

This is a font converter based off of multi-channel signed distance fields that turns a font into an atlased texture.

## Dependencies

* msdfgen: https://github.com/Chlumsky/msdfgen
* image magick: https://imagemagick.org/index.php
* hiero: https://github.com/libgdx/libgdx/wiki/Hiero

## Usage

1. Drag `.ttf` file over `convert_font.bat`
2. Convert font using hiero:
2. 1. Set Size to 56
2. 2. Set Rendering to Java
2. 3. Select File > Save BMFont files (text)...
3. Run `patch_hiero.py`