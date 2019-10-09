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
```

### Nodes
The third block of data contains a nested tree structure of nodes. The format begins at the root node, and is as follows:
```
float 4*4 - transformation matrix

unsigned int - number of meshes

for each mesh:
	unsigned int - mesh id
	
unsigned int - number of children

for each child:
	node - child node (this structure nested)
```