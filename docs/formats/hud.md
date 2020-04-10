# Huds

Hud files come in the form of `hud.json`.

## Localization

Localization files are loaded through the hud. A default language is specified:

```
	"defaultLanguage": "ENGLISH",
```

as well as a list of localization files to load:

```
	"localization": [
		{
			"file": "resources/localization/engine.txt",
			"file": "resources/localization/game.txt"
		}
	],
```

## Fonts

Font files are also loaded through the hud:

```
	"fonts": [
		{
			"name": "OpenSans",
			"folder": "resources/fonts/",
			"file": "opensans.fnt"
		}
	],
```

## Elements

Elements consist of several required fields:

```
			"type": "textbox",
			"xpos": 0,
			"ypos": 850,
			"zpos": 0,
			"wide": 1,
			"tall": 50,
```

And several optional fields:

```
			"tags": [
				"mainmenu", "loadbutton"
			],
			"name": "Example",
			"wideRelative": true,
			"tallRelative": true,
```

Elements can take several forms, specified by their `type` field.

### image

Image panels also include:

```
			"image": "hud/healthbar_bg_left.tga"
```

### fillcolor

Fillcolor panels also include:

```
			"color": "0.7 0.0 0.0 1.0"
```

### textbox

Textbox panels also include:

```
			"label": "UnitHP_1",
			"labelRaw": false,
			"alignment": "right",
			
			"font": "OpenSans",
			"fontSize": 14,
			"color": "1.0 1.0 1.0 1.0"
```

You can also specify subpixel rendering:

```
			"subpixel": true,
			"subpixelBG": "0.3 0.3 0.3"
```

### gradient

Gradient panels also include:

```
			"topLeft": "0.0 0.0 0.0 0.0",
			"topRight": "0.0 0.0 0.0 0.4",
			"bottomLeft": "0.0 0.0 0.0 0.0",
			"bottomRight": "0.0 0.0 0.0 0.4"
```

### button

Button panels don't include any additional fields.