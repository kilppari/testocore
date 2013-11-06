/******************************************************************************/
/**
    This class defines the base properties for all the models to be drawn
    by GLRenderer.

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
#ifndef GL_RENDERABLE_H_
#define GL_RENDERABLE_H_

struct GLVertex {
    union {
        GLfloat pos[ 3 ];
        struct {
            GLfloat x, y ,z;
        };
    };
    union {
        GLfloat color[ 4 ];
        struct {
            GLfloat r, g, b, a;
        };
    };
};

/**
 * Holds vertex information for a single GLRenderable.
 */
struct GLVertexDataStr {
    // points to the array containing actual vertices
    GLfloat* buffer_ptr;

    // ID for the vertex buffer given by openGL
    GLuint buffer_id;

    // byte size of the vertex buffer
    GLuint buffer_size;

    // number of vertices in vertex buffer
    GLuint vertex_count;
};

/**
 * Specifies common renderable object for GLRenderer to draw.
 */
class GLRenderable {
private:
    uint64_t m_Id;

    // Vertex data for this object
    GLVertexDataStr m_VertexData;

    // Model matrix for this object
    glm::mat4 m_ModelMatrix;

public:
    GLRenderable();

    // Settter and ggert for object's ID
    void setId( uint64_t id ) { m_Id = id; }
    uint64_t getId( void ) { return m_Id; }
/*
    void setVertices( GLfloat* vertices ) {
        m_VertexData.buffer_ptr = vertices; }
*/
    // Returns the model matrix
    const glm::mat4& getModelMatrix( void ) { return m_ModelMatrix; }
/*
    void copyVertexData( const GLVertexDataStr* data_ptr ) {
        memcpy( &m_VertexData, data_ptr, sizeof( GLVertexDataStr ) ); }
*/
    // Returns reference to the vertex data
    GLVertexDataStr& getVertexDataRef( void ) { return m_VertexData; }

    // Performs translation on model matrix
    void translate( GLfloat x, GLfloat y, GLfloat z );
    // Performs rotation on model matrix
    void rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
    // PErforms scaling for model matrix
    void scale( GLfloat x, GLfloat y, GLfloat z );
};

#endif /* #ifndef GL_RENDERABLE_H_ */
