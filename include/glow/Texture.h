#pragma once

#include <glow/Object.h>

namespace glow {

class Texture : public Object
{
public:
	Texture(GLenum  target = GL_TEXTURE_2D);
	~Texture();

	void setParameter(GLenum name,  GLint value);
	void setParameter(GLenum name,  GLfloat value);

	void bind();
	void unbind();

	static void unbind(GLenum target);

	void image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);
	void storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);

	void bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
protected:
	GLenum  _target;
};

} // namespace glow