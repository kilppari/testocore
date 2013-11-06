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
#include <iostream>
#include <stdint.h>
#include <cstring>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mem_pool.h"
#include "list.h"
#include "gl_renderable.h"
#include "gl_renderer.h"

/* -------------------------------------------------------------------------- */
// Public methods
/* -------------------------------------------------------------------------- */

/*
 * Initializes vertex array object.
 */
void GLRenderer::init() {
    // Initialize Vertex array object
    glGenVertexArrays( 1, &m_VertexArrayId );
    glBindVertexArray( m_VertexArrayId );
}

/*
 * Deletes all GL buffers and clears rendering list.
 */
void GLRenderer::cleanup() {

    Node< GLRenderable* >* node_ptr = m_Renderables.begin();

    // Delete buffers of all the renderables
    while( node_ptr != NULL ) {
        GLuint id = node_ptr->item()->getVertexDataRef().buffer_id;
        glDeleteBuffers( 1, &id );
        node_ptr = node_ptr->next();
    }
    // Delete VAO
    glDeleteVertexArrays( 1, &m_VertexArrayId );

    // Remove all renderables
    m_Renderables.clear();
}

/*
 * Adds a new GLRenderable into the rendering list and load
 * it into GPU memory.
 */
void GLRenderer::addRenderable( GLRenderable* renderable_ptr ) {
    renderable_ptr->setId( m_RunningId++ );
    m_Renderables.pushBack( renderable_ptr );

    loadVertexData( renderable_ptr->getVertexDataRef() );
}

/*
 * Removes GLRenderable from the rendering list and deletes
 * corresponding GL vertexbuffer.
 */
void GLRenderer::removeRenderable( uint64_t id ) {

    Node< GLRenderable* >* node_ptr = m_Renderables.begin();

    while( node_ptr != NULL ) {
        if( node_ptr->item()->getId() == id ) {
            GLuint id = node_ptr->item()->getVertexDataRef().buffer_id;
            glDeleteBuffers( 1, &id );
            m_Renderables.remove( node_ptr );
            break;
        }
        node_ptr = node_ptr->next();
    }
}

/*
 * Loads and compiles shaders.
 */
bool GLRenderer::loadShaders( const char* v_shader, const char* f_shader ) {

    char* v_buf_ptr = NULL;
    char* f_buf_ptr = NULL;
    FILE* fp = NULL;

    // Read vertex shader source into f_buf_ptr
    fp = fopen( v_shader, "r" );
    if( fp != NULL ) {
        fseek( fp, 0, SEEK_END );
        uint32_t size = ftell( fp );
        fseek( fp, 0, SEEK_SET );
        v_buf_ptr = new char[ size + 1 ];
        fread( v_buf_ptr, size, 1, fp );
        v_buf_ptr[ size ] = '\0';
        fclose( fp );
    }

    // Read fragment shader source into f_buf_ptr
    fp = fopen( f_shader, "r" );
    if( fp != NULL ) {
        fseek( fp, 0, SEEK_END );
        uint32_t size = ftell( fp );
        fseek( fp, 0, SEEK_SET );
        f_buf_ptr = new char[ size + 1 ];
        fread( f_buf_ptr, size, 1, fp );
        f_buf_ptr[ size ] = '\0';
        fclose( fp );
    }

    if( v_buf_ptr == NULL || f_buf_ptr == NULL ) {
        // Could not load shaders, return immediately.
        fprintf( stderr, "Failed to load shaders\n" );
        if( v_buf_ptr != NULL ) delete[] v_buf_ptr;
        if( f_buf_ptr != NULL ) delete[] f_buf_ptr;
        return false;
    }

    const char* v_src_ptr = v_buf_ptr;
    const char* f_src_ptr = f_buf_ptr;

    // Create shader IDs
    GLuint v_shader_id = glCreateShader( GL_VERTEX_SHADER );
    GLuint f_shader_id = glCreateShader( GL_FRAGMENT_SHADER );
    GLint compile_status = GL_FALSE;
    int info_log_len;

    // Compile Vertex Shader
    printf( "Compiling shader : %s\n", v_shader );
    glShaderSource( v_shader_id, 1, &v_src_ptr , NULL);
    glCompileShader( v_shader_id );

    // Check Vertex Shader
    glGetShaderiv( v_shader_id, GL_COMPILE_STATUS, &compile_status );
    glGetShaderiv( v_shader_id, GL_INFO_LOG_LENGTH, &info_log_len );
    char v_shader_err_msg[ info_log_len ];
    glGetShaderInfoLog( v_shader_id, info_log_len, NULL, v_shader_err_msg );
    fprintf( stdout, "%s\n", v_shader_err_msg );

    // Compile Fragment Shader
    printf( "Compiling shader : %s\n", f_shader );
    glShaderSource( f_shader_id, 1, &f_src_ptr , NULL);
    glCompileShader( f_shader_id );

    // Check Fragment Shader
    glGetShaderiv( f_shader_id, GL_COMPILE_STATUS, &compile_status );
    glGetShaderiv( f_shader_id, GL_INFO_LOG_LENGTH, &info_log_len );
    char f_shader_err_msg[ info_log_len ];
    glGetShaderInfoLog( f_shader_id, info_log_len, NULL, f_shader_err_msg );
    fprintf( stdout, "%s\n", f_shader_err_msg );

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint program_id = glCreateProgram();
    glAttachShader( program_id, v_shader_id );
    glAttachShader( program_id, f_shader_id );
    glLinkProgram( program_id );

    // Check the program
    glGetProgramiv( program_id, GL_LINK_STATUS, &compile_status );
    glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &info_log_len );
    int len = ( info_log_len > 1 ) ? info_log_len : 1;
    char program_err_msg[ len ];
    glGetProgramInfoLog(program_id, info_log_len, NULL, program_err_msg );
    fprintf(stdout, "%s\n", program_err_msg );

    // Store shader program id and the location id for MVP matrix
    m_ShaderProgramId = program_id;
    m_ShaderMVPLocation = glGetUniformLocation( program_id, "MVP" );

    glDeleteShader( v_shader_id );
    glDeleteShader( f_shader_id );
    delete[] f_buf_ptr;
    delete[] v_buf_ptr;

    return true;
}

/*
 * Renders all the objects in the rendering list (m_Renderables).
 */
void GLRenderer::draw( void ) {
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram( m_ShaderProgramId );

    // Draw each object in our Renderable list
    Node< GLRenderable* >* node_ptr = m_Renderables.begin();
    while( node_ptr != NULL ) {

        const glm::mat4& model_matrix = node_ptr->item()->getModelMatrix();

        glm::mat4 mvp = m_ProjectionMatrix * m_ViewMatrix * model_matrix;

        // Send transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv( m_ShaderMVPLocation, 1, GL_FALSE, &mvp[0][0] );

        GLVertexDataStr& vertex_data = node_ptr->item()->getVertexDataRef();
        glBindBuffer( GL_ARRAY_BUFFER, vertex_data.buffer_id );

        // 1st attribute buffer for vertex positions
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer(
            0, //attribute 0
            3, //size (x, y, z)
            GL_FLOAT, // type of the vertices
            GL_FALSE, // not normalized
            28, // stride ( size of GLVertex )
            ( void* )0 // Array buffer offset
        );

        // 2nd attribute buffer for vertex colors
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer(
            1, //attribute 1
            4, //size (r, g, b, a)
            GL_FLOAT, // type of the vertices
            GL_FALSE, // not normalized
            28, // stride
            ( void* )12 // Array buffer offset
        );

        // Draw the Renderable:
        glDrawArrays( GL_TRIANGLES, 0, vertex_data.vertex_count );
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );

        node_ptr = node_ptr->next();
    }
}

/* -------------------------------------------------------------------------- */
// Private methods
/* -------------------------------------------------------------------------- */

/*
 * Loads vertex data into GPU memory.
 */
void GLRenderer::loadVertexData( GLVertexDataStr& data ) {

    glGenBuffers( 1, &(data.buffer_id) );
    glBindBuffer( GL_ARRAY_BUFFER, data.buffer_id );
    glBufferData(
        GL_ARRAY_BUFFER,
        data.buffer_size,
        data.buffer_ptr,
        GL_STATIC_DRAW );
}


