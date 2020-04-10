# Keybinds

The keybinds file allows you to specify various controls. It is a simple json file that can be edited to set different commands for different keys.

There are four variables:
- `pressed` lists out what keys should do when they are first pressed
- `held` lists out what keys should do every frame they continue to be held, including when they are first pressed
- `released` list out what keys should do the frame they stop being held
- `unheld` lists out what keys should do every frame they are not pressed or held (but released counts as unheld)

An example of some keybinds would be:

```
	"pressed": {
		"space": [ "togglespecial" ],
		"leftclick": [ "select" ],
		"controller_a": [ "select", "startgame", "leavescoreboard" ],
		"controller_b": [ "startgame", "loadgame", "leavescoreboard" ],
		"controller_x": [ "scoreboard" ],
		"controller_y": [ "togglespecial" ],
		"controller_rb": [ "switchunit" ],
		"controller_lb": [ "switchunit" ],
		"f": [ "switchunit" ]
	},
```

Of note, multiple commands can be bound to the same key: `controller_a` and `controller_b` for example.

## Key List

All letter and number keys (alphanumeric) are supported for keybinds. In addition, the following named keys are also allowed:
```
space
leftclick
rightclick
controller_a
controller_b
controller_x
controller_y
controller_lb
controller_rb
```