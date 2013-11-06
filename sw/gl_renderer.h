/******************************************************************************/
/**
    Rendering manager for Testocore framework. Works as a wrapper for openGL
    calls and handles the loading and rendering of given models (GLRenderables).

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
#ifndef GL_RENDERER_H_
#define GL_RENDERER_H_

class GLRenderer {
private:
    // Linked list of Renderables to draw. (Rendering list)
    List< GLRenderable* > m_Renderables;
    // Running ID counter for new renderables.
    uint64_t m_RunningId;
    // ID of the compiled shader program.
    GLuint m_ShaderProgramId;
    // Location ID of the MVP matrix in the vertex shader.
    GLuint m_ShaderMVPLocation;
    // ID of the vertex array object.
    GLuint m_VertexArrayId;
    // Projection and View -matrices. These stay constant during a frame.
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

public:
    ~GLRenderer() { cleanup(); }
    // Initializes vertex array object.
    void init();

    // Deletes all GL buffers and clears rendering list.
    void cleanup();

    // Wrapper for glClearColor
    void setClearColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a ) {
        glClearColor( r, g, b, a );
    }

    // Sets projection matrix.
    void setProjectionMatrix( const glm::mat4& p_matrix ) {
        m_ProjectionMatrix = p_matrix;
    }

    // Sets view matrix.
    void setViewMatrix( const glm::mat4& v_matrix ) {
        m_ViewMatrix = v_matrix;
    }

    // Adds a new GLRenderable into the rendering list.
    void addRenderable( GLRenderable* renderable_ptr );

    // Removes GLRenderable from the rendering list.
    void removeRenderable( uint64_t id );

    // Loads and compiles shaders.
    bool loadShaders( const char* v_shader, const char* f_shader );

    // Renders all the objects in the rendering list (m_Renderables).
    void draw();

// -----------------------------------------------------------------------------
// Private methods:
private:
    // Loads vertex data into GPU memory.
    void loadVertexData( GLVertexDataStr& data );
};

#endif /* #ifndef GL_RENDERER_H_ */
