# BMW File Format

The Beo-Model-Wolf File Format is a format for models.

## Format
The format stores values as binary. Values being loaded in are the size of their c++ equivalent (ie. an `int` is 32-bit and takes up 4 ASCII characters worth). All strings are null terminated arrays of characters (c-style strings).

### Header 
All files begin with the Beo-Model-Wolf Header:
```
Beo-Model-Wolf Format Version 1
```

### Textures
The first block of data is the list of textures. The format is as follows:
```
unsigned int - number of material

for each material:
	unsigned int - number of textures
	
	for each texture:
		null-terminated-string - texture filepath
```

### Meshes
The second block of data is the list of meshes. The format is as follows:
```
unsigned int - number of meshes

for each mesh:
	unsigned int - number of bone-weighted vertex ids
	
	for each bone-weighted vertex id:
		unsigned int - vertex id
		unsigned int - number of bone weights
		
		for each bone weight:
			unsigned int - node id
			float - bone weight
	
	unsigned int - number of vertices
	
	for each vertex:
		// transforms
		float x
		float y
		float z
		// color
		int r (default 255)
		int g (default 255)
		int b (default 255)
		int a (default 255)
		// texture uv
		float u (default 0.0)
		float v (default 0.0)
		// normal mapping
		float nx
		float ny
		float nz
		
	unsigned int - number of faces
	
	for each face:
		unsigned int v0
		unsigned int v1
		unsigned int v2
```

### Nodes
The third block of data contains a nested tree structure of nodes. The format begins at the root node, and is as follows:
```
unsigned int - node id

float 4*4 - transformation matrix

unsigned int - number of meshes

for each mesh:
	unsigned int - mesh id
	
unsigned int - number of children

for each child:
	node - child node (this structure nested)
```

### Animations
The fourth block of data contains the bone transforms for animations. It consists of:
```
unsigned int - number of animations

for each animation:
	unsigned int - duration (tick count)
	unsigned int - speed (ticks per second)
	unsigned int - bones affected
	
	for each bone affected:
		unsigned int - node id
		
		for duration:
			float 4*4 - transformation matrix
```

## Metadata

There is a variety of override options and additional specifications that are supplied in a `json` file. It is identified by having the same filename as the `bmw` file.

### Transform Overrides

Transform overrides can be in the form of:
```
	"rotation": 90.0,
	"rotationAngle": "0.978734 0.0 0.205133",
	"scale": "0.25 0.25 0.25",
	"translation": "1.0 -0.5 0.0"
```

### Texture Overrides

Texture overrides can be in the form of:
```
	"textureOverrides" : [
		{
			"texture": "Oak_Tree.fbm/tree_diffuse.dds"
		}
	]
```

### Animations

Animations are specified in two parts: a default animation, and a set of clips.

The default animation is specified as:
```
	"defaultAnim": "idle",
```

While clips take the form of:
```
	"clips" : [
		{
            "name": "idle",
            "start": 2,
            "end": 269,
			"loop": true
        },
        {
            "name": "death_forward",
            "start": 565,
            "end": 629,
			"hold": true
        },
        {
            "name": "spawn",
            "start": 905,
            "end": 959,
			"loop": false
        }
	]
```