# Font File Format

Fonts consist of two different files: a `fnt` file and a `tga` file. While it isn't strictly required, it's recommended that they have the same filename.

## `fnt` file

The `fnt` file is a standard BMFont-compatible `fnt` file specification.

## `tga` file

The `tga` file consists of the font atlas. It should be a multi-channel signed distance field atlas.

## Generation

Generation of these font files refer to [this tool](https://github.com/JarateKing/Beowolf-Engine/tree/master/tools/font_converter)