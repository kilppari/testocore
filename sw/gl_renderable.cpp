/******************************************************************************/
/**
    Implementation for GLRenderable. See gl_rendrable.h for details.

    Copyright (C) 2013 Pekka Mäkinen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/******************************************************************************/
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl_renderable.h"

/*
 * Initialized the object
 */
GLRenderable::GLRenderable() : m_Id( 0 ) {
    m_ModelMatrix = glm::mat4( 1.0f );
    m_VertexData.buffer_ptr = NULL;
}

/*
 * Performs translation on the model matrix.
 */
void GLRenderable::translate( GLfloat x, GLfloat y, GLfloat z ) {
    // Get translated matrix
    m_ModelMatrix = glm::translate( m_ModelMatrix, glm::vec3( x, y, z ) );
}

/*
 * Performs rotation on the model matrix.
 */
void GLRenderable::rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z ) {
    // Get rotated matrix
    m_ModelMatrix = glm::rotate( m_ModelMatrix, angle, glm::vec3( x, y, z ) );
}

/*
 * Performs scaling on the model matrix.
 */
void GLRenderable::scale( GLfloat x, GLfloat y, GLfloat z ) {
    // Get scaled matrix
    m_ModelMatrix = glm::translate( m_ModelMatrix, glm::vec3( x, y, z ) );
}
