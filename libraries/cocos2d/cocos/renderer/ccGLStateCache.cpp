/****************************************************************************
Copyright (c) 2011      Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2017 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "renderer/ccGLStateCache.h"

#include "renderer/CCGLProgram.h"
#include "renderer/CCRenderState.h"
#include "base/CCDirector.h"
#include "base/ccConfig.h"
#include "base/CCConfiguration.h"

NS_CC_BEGIN

static const int MAX_ATTRIBUTES = 16;
static const int MAX_ACTIVE_TEXTURE = 16;

namespace
{
    static GLuint s_currentProjectionMatrix = -1;
    static uint32_t s_attributeFlags = 0;  // 32 attributes max

#if CC_ENABLE_GL_STATE_CACHE

    static GLuint    s_currentShaderProgram = -1;
    static GLuint    s_currentBoundTexture[MAX_ACTIVE_TEXTURE] =  {(GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, };
    static GLenum    s_blendingSource = -1;
    static GLenum    s_blendingDest = -1;
    static int       s_GLServerState = 0;
    static GLuint    s_VAO = 0;
    static GLenum    s_activeTexture = -1;

#endif // CC_ENABLE_GL_STATE_CACHE
}

// GL State Cache functions

namespace GL {

void invalidateStateCache( void )
{
    Director::getInstance()->resetMatrixStack();
    s_currentProjectionMatrix = -1;
    s_attributeFlags = 0;

#if CC_ENABLE_GL_STATE_CACHE
    s_currentShaderProgram = -1;
    for( int i=0; i < MAX_ACTIVE_TEXTURE; i++ )
    {
        s_currentBoundTexture[i] = -1;
    }

    s_blendingSource = -1;
    s_blendingDest = -1;
    s_GLServerState = 0;
    s_VAO = 0;
    
#endif // CC_ENABLE_GL_STATE_CACHE
}

void deleteProgram( GLuint program )
{
#if CC_ENABLE_GL_STATE_CACHE
    if(program == s_currentShaderProgram)
    {
        s_currentShaderProgram = -1;
    }
#endif // CC_ENABLE_GL_STATE_CACHE

    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

    f->glDeleteProgram( program );
}

void useProgram( GLuint program )
{
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

#if CC_ENABLE_GL_STATE_CACHE
    if( program != s_currentShaderProgram ) {
        s_currentShaderProgram = program;
        f->glUseProgram(program);
    }
#else
    f->glUseProgram(program);
#endif // CC_ENABLE_GL_STATE_CACHE
}

static void SetBlending(GLenum sfactor, GLenum dfactor)
{
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

	if (sfactor == GL_ONE && dfactor == GL_ZERO)
    {
        f->glDisable(GL_BLEND);
        RenderState::StateBlock::_defaultState->setBlend(false);
	}
    else
    {
        f->glEnable(GL_BLEND);
        f->glBlendFunc(sfactor, dfactor);

        RenderState::StateBlock::_defaultState->setBlend(true);
        RenderState::StateBlock::_defaultState->setBlendSrc((RenderState::Blend)sfactor);
        RenderState::StateBlock::_defaultState->setBlendDst((RenderState::Blend)dfactor);
    }
}

void blendFunc(GLenum sfactor, GLenum dfactor)
{
#if CC_ENABLE_GL_STATE_CACHE
    if (sfactor != s_blendingSource || dfactor != s_blendingDest)
    {
        s_blendingSource = sfactor;
        s_blendingDest = dfactor;
        SetBlending(sfactor, dfactor);
    }
#else
    SetBlending( sfactor, dfactor );
#endif // CC_ENABLE_GL_STATE_CACHE
}

void blendResetToCache(void)
{
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

    f->glBlendEquation(GL_FUNC_ADD);
#if CC_ENABLE_GL_STATE_CACHE
	SetBlending(s_blendingSource, s_blendingDest);
#else
	SetBlending(CC_BLEND_SRC, CC_BLEND_DST);
#endif // CC_ENABLE_GL_STATE_CACHE
}

void bindTexture2D(GLuint textureId)
{
    GL::bindTexture2DN(0, textureId);
}

void bindTexture2D(Texture2D* texture)
{
    GL::bindTexture2DN(0, texture->getName());
    auto alphaTexID = texture->getAlphaTextureName();
    if (alphaTexID > 0) {
        GL::bindTexture2DN(1, alphaTexID);
    }
}

void bindTexture2DN(GLuint textureUnit, GLuint textureId)
{
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

#if CC_ENABLE_GL_STATE_CACHE
	CCASSERT(textureUnit < MAX_ACTIVE_TEXTURE, "textureUnit is too big");
	if (s_currentBoundTexture[textureUnit] != textureId)
	{
		s_currentBoundTexture[textureUnit] = textureId;
		activeTexture(GL_TEXTURE0 + textureUnit);
        f->glBindTexture(GL_TEXTURE_2D, textureId);
	}
#else
    f->glActiveTexture(GL_TEXTURE0 + textureUnit);
    f->glBindTexture(GL_TEXTURE_2D, textureId);
#endif
}

void bindTextureN(GLuint textureUnit, GLuint textureId, GLuint textureType/* = GL_TEXTURE_2D*/)
{
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

#if CC_ENABLE_GL_STATE_CACHE
    CCASSERT(textureUnit < MAX_ACTIVE_TEXTURE, "textureUnit is too big");
    if (s_currentBoundTexture[textureUnit] != textureId)
    {
        s_currentBoundTexture[textureUnit] = textureId;
        activeTexture(GL_TEXTURE0 + textureUnit);
        f->glBindTexture(textureType, textureId);
    }
#else
    f->glActiveTexture(GL_TEXTURE0 + textureUnit);
    f->glBindTexture(textureType, textureId);
#endif
}


void deleteTexture(GLuint textureId)
{
#if CC_ENABLE_GL_STATE_CACHE
    for (size_t i = 0; i < MAX_ACTIVE_TEXTURE; ++i)
    {
        if (s_currentBoundTexture[i] == textureId)
        {
            s_currentBoundTexture[i] = -1;
        }
    }
#endif // CC_ENABLE_GL_STATE_CACHE
    
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

    f->glDeleteTextures(1, &textureId);
}

void deleteTextureN(GLuint /*textureUnit*/, GLuint textureId)
{
    deleteTexture(textureId);
}

void activeTexture(GLenum texture)
{
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

#if CC_ENABLE_GL_STATE_CACHE
    if(s_activeTexture != texture) {
        s_activeTexture = texture;
        f->glActiveTexture(s_activeTexture);
    }
#else
    f->glActiveTexture(texture);
#endif
}

void bindVAO(GLuint vaoId)
{    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
        Q_ASSERT(context == QOpenGLContext::currentContext());
        auto f = context->extraFunctions();

#if CC_ENABLE_GL_STATE_CACHE
        if (s_VAO != vaoId)
        {
            s_VAO = vaoId;
            f->glBindVertexArray(vaoId);
        }
#else
        f->glBindVertexArray(vaoId);
#endif // CC_ENABLE_GL_STATE_CACHE
    
    }
}

// GL Vertex Attrib functions

void enableVertexAttribs(uint32_t flags)
{
    auto context = cocos2d::Director::getInstance()->getOpenGLView()->getOpenGLContext();
    Q_ASSERT(context == QOpenGLContext::currentContext());
    auto f = context->functions();

    bindVAO(0);

    // hardcoded!
    for(int i=0; i < MAX_ATTRIBUTES; i++) {
        unsigned int bit = 1 << i;
        //FIXME:Cache is disabled, try to enable cache as before
        bool enabled = (flags & bit) != 0;
        bool enabledBefore = (s_attributeFlags & bit) != 0;
        if(enabled != enabledBefore) 
        {
            if( enabled )
                f->glEnableVertexAttribArray(i);
            else
                f->glDisableVertexAttribArray(i);
        }
    }
    s_attributeFlags = flags;
}

// GL Uniforms functions

void setProjectionMatrixDirty( void )
{
    s_currentProjectionMatrix = -1;
}

} // Namespace GL

NS_CC_END
