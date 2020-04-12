# Localization Files

Localization files are used to provide translated and dynamic text in the Beowolf engine.

The localization file begins with a header line, that specifies what languages are supported. They are tab-separated.
```
STRING	ENGLISH	FRENCH
```

All lines that follow are custom strings. Much like the header line, they are tab-separated.
```
Engine_Lang	English	Francais
Engine_Name	Beowolf Engine	Moteur de Jeu Beowolf
```

Localized strings can also contain variables. These variables support dynamic strings that can get set in code. They can be seen as text surrounded by `$` characters, such as `$fps$`.
```
Engine_FPS	FPS: $fps$ ($deltaMS$ms)	FPS: $fps$ ($deltaMS$ms)
Loading_Percent	$Loading_Percent$%	$Loading_Percent$%
Loading_File	Loading $Loading_File$	Loading $Loading_File$
```