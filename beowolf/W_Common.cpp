//-----------------------------------------------------------------------------
// File:			W_Common.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_Common.h"
#include "W_Types.h"

namespace wolf
{

//----------------------------------------------------------
// Loads in a whole file and returns the contents. User is 
// responsible for then delete[]-ing the data. Returns 0 if 
// file not able to be loaded.
//----------------------------------------------------------
void* LoadWholeFile(const std::string& p_strFile)
{
	FILE* fp = fopen(p_strFile.c_str(), "rb");
	if( !fp )
		return 0;

	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* pRet = new char[len+1];
	fread(pRet, sizeof(char), len, fp);
    pRet[len] = 0;
	fclose(fp);

	return pRet;
}

//----------------------------------------------------------
// Compiles a shader of the given type (GL_VERTEX_SHADER or
// GL_FRAGMENT_SHADER) from the given file and stores the 
// resulting shader object in p_pShader. Returns true on success,
// else false
//----------------------------------------------------------
bool CompileShader(GLuint* p_pShader, GLenum p_eType, const std::string& p_strFile)
{
    GLint iStatus;
    const GLchar* pSource;
    
    pSource = (GLchar *) LoadWholeFile(p_strFile);
    if( !pSource )
    {
        printf("Failed to load vertex shader\n");
        return false;
    }
    
    *p_pShader = glCreateShader(p_eType);
    glShaderSource(*p_pShader, 1, &pSource, NULL);
    glCompileShader(*p_pShader);
    
#if defined(DEBUG)
    GLint iLogLength;
    glGetShaderiv(*p_pShader, GL_INFO_LOG_LENGTH, &iLogLength);
    if( iLogLength > 0 )
    {
        GLchar* pLog = (GLchar *)malloc(iLogLength);
        glGetShaderInfoLog(*p_pShader, iLogLength, &iLogLength, pLog);
        printf("Shader compile log:\n%s\n", pLog);
        free(pLog);
    }
#endif
    
    glGetShaderiv(*p_pShader, GL_COMPILE_STATUS, &iStatus);
    if( iStatus == 0 )
    {
        glDeleteShader(*p_pShader);
        return false;
    }
    
    return true;
}

//----------------------------------------------------------
// Performs the linking stage of the vertex and pixel shader,
// resulting in a final program object to use. Checks for
// errors and prints them and returns true on success, else false
//----------------------------------------------------------
bool LinkProgram(GLuint p_uiProg)
{
    GLint iStatus;
    
    glLinkProgram(p_uiProg);
    
#if defined(DEBUG)
    GLint iLogLength;
    glGetProgramiv(p_uiProg, GL_INFO_LOG_LENGTH, &iLogLength);
    if (iLogLength > 0)
    {
        GLchar* pLog = (GLchar *)malloc(iLogLength);
        glGetProgramInfoLog(p_uiProg, iLogLength, &iLogLength, pLog);
        printf("Program link log:\n%s\n", pLog);
        free(pLog);
    }
#endif
    
    glGetProgramiv(p_uiProg, GL_LINK_STATUS, &iStatus);
    if (iStatus == 0)
        return false;
    
    return true;
}
//----------------------------------------------------------
// Loads a vertex and pixel shader from the given files, and
// compiles and links them into a program. Will return 0 on
// failure (and print errors to stdout), else the created
// program object
//----------------------------------------------------------
GLuint LoadShaders(const std::string& p_strVSFile, const std::string& p_strPSFile)
{
    GLuint uiVS, uiPS;
    
    // 1. Create and compile vertex shader.
    if( !CompileShader(&uiVS, GL_VERTEX_SHADER, p_strVSFile))
	{
        printf("Failed to compile vertex shader\n");
        return 0;
    }
    
    // 2. Create and compile fragment shader.
    if( !CompileShader(&uiPS, GL_FRAGMENT_SHADER, p_strPSFile))
	{
        printf("Failed to compile pixel shader\n");
		glDeleteShader(uiVS);
        return 0;
    }

    // 3. Create shader program that we'll (hopefully) eventually return
    GLuint uiProgram = glCreateProgram();
    
    // 4. Attach vertex shader and pixel shader to program.
    glAttachShader(uiProgram, uiVS);
    glAttachShader(uiProgram, uiPS);
    
    // 5. Link program.
    if( !LinkProgram(uiProgram) )
    {
        printf("Failed to link program: %d\n", uiProgram);
        
        if( uiVS )
        {
            glDeleteShader(uiVS);
        }
        if( uiPS )
        {
            glDeleteShader(uiPS);
        }
        if( uiProgram )
        {
            glDeleteProgram(uiProgram);
        }
        
        return 0;
    }
    
    // Release vertex and fragment shaders.
    if( uiVS )
        glDeleteShader(uiVS);
    if( uiPS )
        glDeleteShader(uiPS);
    
    return uiProgram;
}

bool LoadTGA(const std::string& p_strFile, unsigned int* p_pWidth, unsigned int* p_pHeight, unsigned char** p_ppData)
{
	GLFWimage img;
	if( GL_FALSE == glfwReadImage( p_strFile.c_str(), &img, 0 ) )
		return false;

	*p_pWidth = img.Width;
	*p_pHeight = img.Height;
	*p_ppData = img.Data;
	return true;
}

enum
{
	DDSF_MAX_MIPMAPS = 16,
	DDSF_MAX_TEXTURES = 16,

	DDSF_CAPS			= 0x00000001,
	DDSF_HEIGHT         = 0x00000002,
	DDSF_WIDTH          = 0x00000004,
	DDSF_PITCH          = 0x00000008,
	DDSF_PIXELFORMAT    = 0x00001000,
	DDSF_MIPMAPCOUNT    = 0x00020000,
	DDSF_LINEARSIZE     = 0x00080000,
	DDSF_DEPTH			= 0x00800000,

	// pixel format flags
	DDSF_ALPHAPIXELS	= 0x00000001,
	DDSF_FOURCC			= 0x00000004,
	DDSF_RGB			= 0x00000040,
	DDSF_RGBA			= 0x00000041,

	// dwCaps1 flags
	DDSF_COMPLEX			= 0x00000008,
	DDSF_TEXTURE			= 0x00001000,
	DDSF_MIPMAP				= 0x00400000,

	// dwCaps2 flags
	DDSF_CUBEMAP			= 0x00000200l,
	DDSF_CUBEMAP_POSITIVEX  = 0x00000400l,
	DDSF_CUBEMAP_NEGATIVEX  = 0x00000800l,
	DDSF_CUBEMAP_POSITIVEY  = 0x00001000l,
	DDSF_CUBEMAP_NEGATIVEY  = 0x00002000l,
	DDSF_CUBEMAP_POSITIVEZ  = 0x00004000l,
	DDSF_CUBEMAP_NEGATIVEZ  = 0x00008000l,
	DDSF_CUBEMAP_ALL_FACES  = 0x0000FC00l,
	DDSF_VOLUME				= 0x00200000l,

	// compressed texture types
	FOURCC_DXT1				= 0x31545844,
	FOURCC_DXT3				= 0x33545844,
	FOURCC_DXT5				= 0x35545844,
};

struct DXTColBlock
{
	unsigned short col0;
	unsigned short col1;
	unsigned char row[4];
};

struct DXT3AlphaBlock
{
	unsigned short row[4];
};

struct DXT5AlphaBlock
{
	unsigned char alpha0;
	unsigned char alpha1;
	unsigned char row[6];
};

struct DDS_PIXELFORMAT
{
	unsigned int dwSize;
	unsigned int dwFlags;
	unsigned int dwFourCC;
	unsigned int dwRGBBitCount;
	unsigned int dwRBitMask;
	unsigned int dwGBitMask;
	unsigned int dwBBitMask;
	unsigned int dwABitMask;
};

struct DDS_HEADER
{
	unsigned int dwSize;
	unsigned int dwFlags;
	unsigned int dwHeight;
	unsigned int dwWidth;
	unsigned int dwPitchOrLinearSize;
	unsigned int dwDepth;
	unsigned int dwMipMapCount;
	unsigned int dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	unsigned int dwCaps1;
	unsigned int dwCaps2;
	unsigned int dwReserved2[3];
};


enum TextureType
{
	TextureNone,
	TextureFlat,    // 1D, 2D, and rectangle textures
	Texture3D,
	TextureCubemap
};

struct DDS_IMAGE
{
	unsigned char *pixels[DDSF_MAX_MIPMAPS];		//the mip map images 
};

struct DDS_TEXTURE
{
	unsigned char		*buffer;	//pointer to loaded dds file
	unsigned int			format;		//compression used or pixel format
	unsigned int			components; //number of channels 
	unsigned int			width;		//width of base image in pixels
	unsigned int			height;		//height of based image in pixels
	unsigned int			mips;		//number of mip levels
	unsigned int			surfaces;	//number of surfaces ( 1 = a texture 6 = a cube map)
	DDS_IMAGE image[6];
};

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

bool ImageSpec(DDS_HEADER *ddsh, unsigned int *format,unsigned int *components)
{
	assert(format);
	assert(components);

	if (ddsh->ddspf.dwFlags & DDSF_FOURCC)	//its a compressed texture
	{
		switch(ddsh->ddspf.dwFourCC)
		{
			case FOURCC_DXT1:
				*format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				*components = 3;
				break;
			case FOURCC_DXT3:
				*format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				*components = 4;
				break;
			case FOURCC_DXT5:
				*format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				*components = 4;
				break;
			default:
				printf("ERROR: Uses a compressed texture of unsupported type\n");
				return false;
		}
	}
	else if (ddsh->ddspf.dwFlags == DDSF_RGBA && ddsh->ddspf.dwRGBBitCount == 32)
	{
		*format = GL_BGRA; 
		*components = 4;
	}
	else if (ddsh->ddspf.dwFlags == DDSF_RGB  && ddsh->ddspf.dwRGBBitCount == 32)
	{
		*format = GL_BGRA; 
		*components = 4;
	}
	else if (ddsh->ddspf.dwFlags == DDSF_RGB  && ddsh->ddspf.dwRGBBitCount == 24)
	{
		*format = GL_BGR; 
		*components = 3;
	}
	else 
	{
		printf("ERROR: Uses a texture of unsupported type");
		return false;
	}

	return true;
}

int DDSImageSize(unsigned int w,
		unsigned int h,
		unsigned int components,
		unsigned int format)
{
	switch(format)
	{
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			return ((w+3)/4)* ((h+3)/4)* 8;   
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			return ((w+3)/4)*((h+3)/4)* 16;   
		default:
			printf("ERROR: unable to determine image size\n");
			return 0;
	}
}

GLuint CreateTextureFromDDS(const std::string& p_strFile, unsigned int* p_pWidth, unsigned int* p_pHeight, bool* p_pHasMips)
{
	DDS_TEXTURE dds;
	memset(&dds, 0, sizeof(dds));

	unsigned char* pBuff = (unsigned char*) LoadWholeFile(p_strFile);
	unsigned char* pStart = pBuff;

	// read in file marker, make sure its a DDS file
	if(strncmp((char*)pBuff,"DDS ",4)!=0)
	{
		printf("%s is not a dds file", p_strFile.c_str());
		return 0;
	}
	pBuff+=4;  //skip over header 

	//read the dds header data
	DDS_HEADER* ddsh;

	ddsh=(DDS_HEADER*)pBuff;
	pBuff+=sizeof(DDS_HEADER);

	TextureType type = TextureFlat;
	if (ddsh->dwCaps2 & DDSF_CUBEMAP)	type = TextureCubemap;

	// check if image is a volume texture
	if ((ddsh->dwCaps2 & DDSF_VOLUME) && (ddsh->dwDepth > 0))
	{
		printf("ERROR: %s is a volume texture ", p_strFile.c_str());
		return 0;
	}
	        
	// get the format of the image
	unsigned int	format;
	unsigned int	components;

	//get the texture format and number of color channels
	ImageSpec(ddsh,&format,&components);

	unsigned int width, height;
	width = ddsh->dwWidth;
	height = ddsh->dwHeight;

	dds.buffer		= (unsigned char*)pStart;
	dds.format		= format;
	dds.components	= components;
	dds.height		= height;
	dds.width		= width;

	if(ddsh->dwMipMapCount==0) ddsh->dwMipMapCount++;

	dds.mips		= ddsh->dwMipMapCount;
	dds.surfaces	= 1;

    GLuint uiTex = 0;
    glGenTextures(1,&uiTex);
    glBindTexture(GL_TEXTURE_2D,uiTex);
	GL_CHECK_ERROR();

	int iWidth = dds.width;
	int iHeight = dds.height;
	unsigned char* pData = pBuff;

	for( unsigned int iMipLevel = 0; iMipLevel < dds.mips; iMipLevel++ )
	{
		if( iWidth == 0 )
			iWidth = 1;
		if( iHeight == 0 )
			iHeight = 1;

		unsigned int size = DDSImageSize(iWidth,iHeight,components,format);

		//Start fill our texture
		glCompressedTexImage2D(
				GL_TEXTURE_2D,
				iMipLevel, 
				format,
				iWidth,
				iHeight,
				0,
				size,
				pData);
		GL_CHECK_ERROR();

        iWidth /= 2;
		iHeight /= 2;
        
		pData += size;
	}

	if( dds.mips > 1 )
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL_CHECK_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL_CHECK_ERROR();
	
	if( p_pWidth )
		*p_pWidth = dds.width;
	if( p_pHeight )
		*p_pHeight = dds.height;
	if( p_pHasMips )
		*p_pHasMips = dds.mips > 1;

    return uiTex;
}
}
