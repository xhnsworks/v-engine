#include "gl_utility.h"
#include "recycle_bin.h"
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
    char* ret = Malloc( (len + 1) * sizeof(char) );
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
        char* ret = Malloc( (len + 1) * sizeof(char) );
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
    sprintf(sbuf, "%dx%d, %s", width, height, formatName);
    uint len = 0;
    while (sbuf[len])
    {
        len++;
    }
    char* ret = Malloc( (len + 1) * sizeof(char) );
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
        char* ret = Malloc( (len + 1) * sizeof(char) );
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
    sprintf(sbuf, "%dx%d, %s", width, height, formatName);
    uint len = 0;
    while (sbuf[len])
    {
        len++;
    }
    char* ret = Malloc( (len + 1) * sizeof(char) );
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
        if(objectType != GL_NONE)
        {
            glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0+i,
                    GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                    &objectId);

            printf("Color Attachment %d: ", i);

            if(objectType == GL_TEXTURE)
            {
                const char* pam = getTextureParameters(objectId);
                printf("GL_TEXTURE, %s\n", pam);
                Mfree((vptr)pam);
            }
            else if(objectType == GL_RENDERBUFFER)
            {
                const char* pam = getRenderbufferParameters(objectId);
                printf("GL_RENDERBUFFER_EXT, %s\n", pam);
                Mfree((vptr)pam);
            }
        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_DEPTH_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

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
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_STENCIL_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_STENCIL_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

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
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
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

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        printf("[ERROR] Framebuffer incomplete: Attached images have different dimensions.\n");
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        printf("[ERROR] Framebuffer incomplete: Color attached images have different internal formats.\n");
        return false;

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
