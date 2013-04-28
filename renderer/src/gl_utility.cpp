#include "pch.h"
#include "gl_utility.h"
#include "emem.h"
#include "elog.h"

///////////////////////////////////////////////////////////////////////////////
// convert OpenGL internal format enum to string
///////////////////////////////////////////////////////////////////////////////
const char* convertInternalFormatToString(GLenum format)
{
    char* formatName;

    switch(format)
    {
    case GL_STENCIL_INDEX:
        formatName = "GL_STENCIL_INDEX";
        break;
    case GL_DEPTH_COMPONENT:
        formatName = "GL_DEPTH_COMPONENT";
        break;
    case GL_ALPHA:
        formatName = "GL_ALPHA";
        break;
    case GL_RGB:
        formatName = "GL_RGB";
        break;
    case GL_RGBA:
        formatName = "GL_RGBA";
        break;
#ifndef __APPLE__
    case GL_LUMINANCE:
        formatName = "GL_LUMINANCE";
        break;
    case GL_LUMINANCE_ALPHA:
        formatName = "GL_LUMINANCE_ALPHA";
        break;
    case GL_ALPHA4:
        formatName = "GL_ALPHA4";
        break;
    case GL_ALPHA8:
        formatName = "GL_ALPHA8";
        break;
    case GL_ALPHA12:
        formatName = "GL_ALPHA12";
        break;
    case GL_ALPHA16:
        formatName = "GL_ALPHA16";
        break;
    case GL_LUMINANCE4:
        formatName = "GL_LUMINANCE4";
        break;
    case GL_LUMINANCE8:
        formatName = "GL_LUMINANCE8";
        break;
    case GL_LUMINANCE12:
        formatName = "GL_LUMINANCE12";
        break;
    case GL_LUMINANCE16:
        formatName = "GL_LUMINANCE16";
        break;
    case GL_LUMINANCE4_ALPHA4:
        formatName = "GL_LUMINANCE4_ALPHA4";
        break;
    case GL_LUMINANCE6_ALPHA2:
        formatName = "GL_LUMINANCE6_ALPHA2";
        break;
    case GL_LUMINANCE8_ALPHA8:
        formatName = "GL_LUMINANCE8_ALPHA8";
        break;
    case GL_LUMINANCE12_ALPHA4:
        formatName = "GL_LUMINANCE12_ALPHA4";
        break;
    case GL_LUMINANCE12_ALPHA12:
        formatName = "GL_LUMINANCE12_ALPHA12";
        break;
    case GL_LUMINANCE16_ALPHA16:
        formatName = "GL_LUMINANCE16_ALPHA16";
        break;
    case GL_INTENSITY:
        formatName = "GL_INTENSITY";
        break;
    case GL_INTENSITY4:
        formatName = "GL_INTENSITY4";
        break;
    case GL_INTENSITY8:
        formatName = "GL_INTENSITY8";
        break;
    case GL_INTENSITY12:
        formatName = "GL_INTENSITY12";
        break;
    case GL_INTENSITY16:
        formatName = "GL_INTENSITY16";
        break;
#endif
    case GL_R3_G3_B2:
        formatName = "GL_R3_G3_B2";
        break;
    case GL_RGB4:
        formatName = "GL_RGB4";
        break;
    case GL_RGB5:
        formatName = "GL_RGB4";
        break;
    case GL_RGB8:
        formatName = "GL_RGB8";
        break;
    case GL_RGB10:
        formatName = "GL_RGB10";
        break;
    case GL_RGB12:
        formatName = "GL_RGB12";
        break;
    case GL_RGB16:
        formatName = "GL_RGB16";
        break;
    case GL_RGBA2:
        formatName = "GL_RGBA2";
        break;
    case GL_RGBA4:
        formatName = "GL_RGBA4";
        break;
    case GL_RGB5_A1:
        formatName = "GL_RGB5_A1";
        break;
    case GL_RGBA8:
        formatName = "GL_RGBA8";
        break;
    case GL_RGB10_A2:
        formatName = "GL_RGB10_A2";
        break;
    case GL_RGBA12:
        formatName = "GL_RGBA12";
        break;
    case GL_RGBA16:
        formatName = "GL_RGBA16";
        break;
    case GL_DEPTH_COMPONENT16:
        formatName = "GL_DEPTH_COMPONENT16";
        break;
    case GL_DEPTH_COMPONENT24:
        formatName = "GL_DEPTH_COMPONENT24";
        break;
    case GL_DEPTH_COMPONENT32:
        formatName = "GL_DEPTH_COMPONENT32";
        break;
    default:
        formatName = "Unknown Format";
        printf("format %x\n", format);
    }

    uint len = 0;
    while (formatName[len])
    {
        len++;
    }
    char* ret = (char*)SMalloc( (len + 1) * sizeof(char) );
    memcpy(ret, formatName, (len + 1) * sizeof(char));
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// return renderbuffer parameters as string using glGetRenderbufferParameterivEXT
///////////////////////////////////////////////////////////////////////////////
const char* getRenderbufferParameters(GLuint id)
{
    if(glIsRenderbuffer(id) == GL_FALSE)
    {
        char* str = "Not Renderbuffer object";
        uint len = 0;
        while (str[len])
        {
            len++;
        }
        char* ret = (char*)SMalloc( (len + 1) * sizeof(char) );
        memcpy( ret, str,(len + 1) * sizeof(char) );
        return ret;
    }

    int width, height, format;

    char* formatName;
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);    // get renderbuffer width
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);  // get renderbuffer height
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); // get renderbuffer internal format
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    formatName = (char*)convertInternalFormatToString(format);

    char sbuf[256];
    snprintf(sbuf, 255, "%dx%d, %s", width, height, formatName);
    uint len = 0;
    while (sbuf[len])
    {
        len++;
    }
    char* ret = (char*)SMalloc( (len + 1) * sizeof(char) );
    memcpy( ret, sbuf, (len + 1) * sizeof(char) );

    Mfree(formatName);
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// return texture parameters as string using glGetTexLevelParameteriv()
///////////////////////////////////////////////////////////////////////////////
const char* getTextureParameters(GLuint id)
{
    if(glIsTexture(id) == GL_FALSE)
    {
        char* str = "Not texture object";
        uint len = 0;
        while (str[len])
        {
            len++;
        }
        char* ret = (char*)SMalloc( (len + 1) * sizeof(char) );
        memcpy( ret, str,(len + 1) * sizeof(char) );
        return ret;
    }


    int width, height, format;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
    glBindTexture(GL_TEXTURE_2D, 0);

    const char* formatName = convertInternalFormatToString(format);

    char sbuf[256];
    snprintf(sbuf, 255, "%dx%d, %s", width, height, formatName);
    uint len = 0;
    while (sbuf[len])
    {
        len++;
    }
    char* ret = (char*)SMalloc( (len + 1) * sizeof(char) );
    memcpy( ret, sbuf, (len + 1) * sizeof(char) );

    Mfree((vptr)formatName);
    return ret;
}
///////////////////////////////////////////////////////////////////////////////
// print out the FBO infos
///////////////////////////////////////////////////////////////////////////////
void printFramebufferInfo()
{
    printf("\n***** FBO STATUS *****\n");

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    printf("Max Number of Color Buffer Attachment Points: %d\n", colorBufferCount);
    ERROR_PROC;

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    int i = 0;
    for(; i < colorBufferCount; ++i)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0+i,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                              &objectType);

        ERROR_PROC;

        if(objectType != GL_NONE)
        {
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0+i,
                    GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                    &objectId);

            ERROR_PROC;

            printf("Color Attachment %d: ", i);

            if(objectType == GL_TEXTURE)
            {
                ERROR_PROC;
                const char* pam = getTextureParameters(objectId);
                printf("GL_TEXTURE, %s\n", pam);
                Mfree((vptr)pam);
                ERROR_PROC;
            }
            else if(objectType == GL_RENDERBUFFER)
            {
                ERROR_PROC;
                const char* pam = getRenderbufferParameters(objectId);
                printf("GL_RENDERBUFFER_EXT, %s\n", pam);
                Mfree((vptr)pam);
                ERROR_PROC;
            }
        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);

    ERROR_PROC;
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_DEPTH_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

        ERROR_PROC;

        printf("Depth Attachment: ");
        const char* pam = NULL;
        switch(objectType)
        {
        case GL_TEXTURE:
            pam = getTextureParameters(objectId);
            printf("GL_TEXTURE, %s\n", pam);
            break;
        case GL_RENDERBUFFER:
            pam = getRenderbufferParameters(objectId);
            printf("GL_RENDERBUFFER, %s", pam);
            break;
        }
        if (pam)
            Mfree((vptr)pam);

        ERROR_PROC;
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_STENCIL_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);

    ERROR_PROC;
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_STENCIL_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

        ERROR_PROC;

        printf("Stencil Attachment: ");
        switch(objectType)
        {
        case GL_TEXTURE:
            printf("GL_TEXTURE, %s", getTextureParameters(objectId));
            break;
        case GL_RENDERBUFFER:
            printf("GL_RENDERBUFFER, %s\n", getRenderbufferParameters(objectId));
            break;
        }
    }
    printf("\n");
}

///////////////////////////////////////////////////////////////////////////////
// check FBO completeness
///////////////////////////////////////////////////////////////////////////////
bool checkFramebufferStatus()
{
    // check FBO status
    ERROR_PROC;
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ERROR_PROC;
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        printf("Framebuffer complete.\n");
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        printf("[ERROR] Framebuffer incomplete: Attachment is NOT complete.\n");
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        printf("[ERROR] Framebuffer incomplete: No image is attached to FBO.\n");
        return false;
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        printf("[ERROR] Framebuffer incomplete: Attached images have different dimensions.\n");
        return false;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        printf("[ERROR] Framebuffer incomplete: Color attached images have different internal formats.\n");
        return false;
#endif
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        printf("[ERROR] Framebuffer incomplete: Draw buffer.\n");
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        printf("[ERROR] Framebuffer incomplete: Read buffer.\n");
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        printf("[ERROR] Unsupported by FBO implementation.\n");
        return false;

    default:
        printf("[ERROR] Unknow error.\n");
        return false;
    }
}

void _error_proc(const char* _file, uint _line)
{
	/**
    GLenum error_info = glGetError();
    if (error_info != GL_NO_ERROR)
    {
        switch (error_info)
        {
        case GL_INVALID_ENUM:
            elog("Error: %s %d: invalid enum", _file, _line);
            break;
        case GL_INVALID_VALUE:
            elog("Error: %s %d: invalid value", _file, _line);
            break;
        case GL_INVALID_OPERATION:
            elog("Error: %s %d: invalid operation", _file, _line);
            break;
        case GL_STACK_OVERFLOW:
            elog("Error: %s %d: stack overflow", _file, _line);
            break;
        case GL_STACK_UNDERFLOW:
            elog("Error: %s %d: stack underflow", _file, _line);
            break;
        case GL_OUT_OF_MEMORY:
            elog("Error: %s %d: out of memory", _file, _line);
            break;
        default:
            elog("Error: %s %d: unknown error", _file, _line);
            break;
        }
        ///exit(3);
    }
	**/
}

void _shader_log(const char* _file, uint _line, uint32 _id)
{
    int log_length = 0xffffffff;
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &log_length);
    ///char log_buffer[log_length];
	char* log_buffer = (char*)SMalloc(log_length);
    glGetShaderInfoLog(_id, log_length, &log_length, log_buffer);
    elog("SHADER LOG BEGIN:%s %d\n", _file, _line);
    elog("%s", log_buffer);
    elog("%s", "SHADER LOG END:");
	Mfree(log_buffer);
}

void _program_log(const char* _file, uint _line, uint32 _id)
{
    int log_length = 0xffffffff;
    glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_length);
    ///char log_buffer[log_length];
	char* log_buffer = (char*)SMalloc(log_length);
    glGetProgramInfoLog(_id, log_length, &log_length, log_buffer);
    elog("PROGRAM LOG BEGIN:%s %d\n", _file, _line);
    elog("%s", log_buffer);
    elog("%s", "PROGRAM LOG END:");
	Mfree(log_buffer);
}

//dds ¸ñÊ½
/**
typedef struct _DDS_IMAGE_DATA
{
    GLsizei  width;
    GLsizei  height;
    GLint    components;
    GLenum   format;
    int      numMipMaps;
    GLubyte *pixels;
} DDS_IMAGE_DATA;

//-----------------------------------------------------------------------------
// Name: loadDDSTextureFile()
// Desc:
//-----------------------------------------------------------------------------
DDS_IMAGE_DATA* loadDDSTextureFile( const char *filename )
{
    DDS_IMAGE_DATA *pDDSImageData;
    DDSURFACEDESC2 ddsd;
    char filecode[4];
    FILE *pFile;
    int factor;
    int bufferSize;

    // Open the file
    pFile = fopen( filename, "rb" );

    if( pFile == NULL )
    {
        char str[255];
        sprintf( str, "loadDDSTextureFile couldn't find, or failed to load \"%s\"", filename );
        MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return NULL;
    }

    // Verify the file is a true .dds file
    fread( filecode, 1, 4, pFile );

    EAssert( strncmp( filecode, "DDS ", 4 ) == 0, "The file \"%s\" doesn't appear to be a valid .dds file!", filename );
    {
        ///char str[255];
        ///sprintf( str, "The file \"%s\" doesn't appear to be a valid .dds file!", filename );
        ///MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        ///fclose( pFile );
        ///return NULL;
    }

    // Get the surface descriptor
    fread( &ddsd, sizeof(ddsd), 1, pFile );

    pDDSImageData = (DDS_IMAGE_DATA*) malloc(sizeof(DDS_IMAGE_DATA));

    memset( pDDSImageData, 0, sizeof(DDS_IMAGE_DATA) );

    //
    // This .dds loader supports the loading of compressed formats DXT1, DXT3
    // and DXT5.
    //

    switch( ddsd.ddpfPixelFormat.dwFourCC )
    {
        case FOURCC_DXT1:
            // DXT1's compression ratio is 8:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            factor = 2;
            break;

        case FOURCC_DXT3:
            // DXT3's compression ratio is 4:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            factor = 4;
            break;

        case FOURCC_DXT5:
            // DXT5's compression ratio is 4:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            factor = 4;
            break;

        default:

            {
                EAssert(0, "The file \"%s\" doesn't appear to be compressed "
                "using DXT1, DXT3, or DXT5!", filename);
            }
            return NULL;
    }

    //
    // How big will the buffer need to be to load all of the pixel data
    // including mip-maps?
    //

    EAssert(ddsd.dwLinearSize != 0, "%s", "dwLinearSize is 0!");

    if( ddsd.dwMipMapCount > 1 )
        bufferSize = ddsd.dwLinearSize * factor;
    else
        bufferSize = ddsd.dwLinearSize;

    pDDSImageData->pixels = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));

    fread( pDDSImageData->pixels, 1, bufferSize, pFile );

    // Close the file
    fclose( pFile );

    pDDSImageData->width      = ddsd.dwWidth;
    pDDSImageData->height     = ddsd.dwHeight;
    pDDSImageData->numMipMaps = ddsd.dwMipMapCount;

    if( ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1 )
        pDDSImageData->components = 3;
    else
        pDDSImageData->components = 4;

    return pDDSImageData;
}

//-----------------------------------------------------------------------------
// Name: loadCompressedTexture()
// Desc:
//-----------------------------------------------------------------------------
void loadCompressedTexture( void )
{
    // NOTE: Unlike "lena.bmp", "lena.dds" actually contains its own mip-map
    // levels, which are also compressed.
    DDS_IMAGE_DATA *pDDSImageData = loadDDSTextureFile( "lena.dds" );

    if( pDDSImageData != NULL )
    {
        int nHeight     = pDDSImageData->height;
        int nWidth      = pDDSImageData->width;
        int nNumMipMaps = pDDSImageData->numMipMaps;

        int nBlockSize;

        if( pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
            nBlockSize = 8;
        else
            nBlockSize = 16;

        glGenTextures( 1, &g_compressedTextureID );
        glBindTexture( GL_TEXTURE_2D, g_compressedTextureID );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        int nSize;
        int nOffset = 0;

        // Load the mip-map levels

        for( int i = 0; i < nNumMipMaps; ++i )
        {
            if( nWidth  == 0 ) nWidth  = 1;
            if( nHeight == 0 ) nHeight = 1;

            nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;

            glCompressedTexImage2DARB( GL_TEXTURE_2D,
                                       i,
                                       pDDSImageData->format,
                                       nWidth,
                                       nHeight,
                                       0,
                                       nSize,
                                       pDDSImageData->pixels + nOffset );

            nOffset += nSize;

            // Half the image size for the next mip-map level...
            nWidth  = (nWidth  / 2);
            nHeight = (nHeight / 2);
        }
    }

    if( pDDSImageData != NULL )
    {
        if( pDDSImageData->pixels != NULL )
            free( pDDSImageData->pixels );

        free( pDDSImageData );
    }
}

**/

