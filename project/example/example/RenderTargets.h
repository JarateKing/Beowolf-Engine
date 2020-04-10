unsigned int depthMapFrameBuf;
unsigned int depthMapTex;
unsigned int depthMapFrameBuf2;
unsigned int depthMapTex2;
const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 2048;

unsigned int depthFieldMapBuf;
unsigned int depthFieldMapTex;

unsigned int reflectionFrameBuf;
unsigned int reflectionRenderBuf;
unsigned int reflectionTex;
const unsigned int REFLECTION_WIDTH = 512, REFLECTION_HEIGHT = 512;

unsigned int refractionFrameBuf;
unsigned int refractionRenderBuf;
unsigned int refractionTex;
const unsigned int REFRACTION_WIDTH = 512, REFRACTION_HEIGHT = 512;

unsigned int fogFrameBuf;
unsigned int fogRenderBuf;
unsigned int fogTex;
const unsigned int FOG_WIDTH = 512, FOG_HEIGHT = 512;

unsigned int postFrameBuf1;
unsigned int postFrameBuf2;
unsigned int postDepthBuf1;
unsigned int postDepthBuf2;
unsigned int postTex1;
unsigned int postTex2;
const unsigned int POST_TEX_WIDTH = 2048, POST_TEX_HEIGHT = 1024;