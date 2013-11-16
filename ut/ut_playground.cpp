/******************************************************************************/
/**
    This file is just a temporary playground area for testing testocore
    framework.

    Copyright (C) 2013 Pekka Mäkinen
    makinpek [ at ] gmail

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
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <string>
#include <sstream>

#define DEFINE_MEMPOOL_MANAGER_GLOBAL
#include "mem_pool.h"
#include "list.h"
#include "gl_renderable.h"
#include "gl_renderer.h"
#include "timer.h"
#include "ut.h"

// Vertices for a cube
static GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

class TestCase : public TestCaseBase {
    public:
    TestCase( const char* name ) : TestCaseBase( name ) {}
    ~TestCase() { }
    void runTest();
};

int main( void ) {

    TestCase TC( "ut_mem_pool" );

    TC.execute();

    return 0;
}

/* -----------------------------------------------------------------------------
 * Define test script here.
 */
void TestCase::runTest( void ) {

/* ------------------------------ */

    UT_START_STEP( 1 );

    GLFWwindow* window;

    if( !glfwInit() ) {
        UT_COMMENT( "Failed to initialize GLFW\n" );
        return;
    }

    glfwWindowHint( GLFW_SAMPLES, 4 ); // 4x antialiasing
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); //OpenGL 3.3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // Open window and create openGL context
    window = glfwCreateWindow( 1024, 768, "testing...", NULL, NULL );
    if( !window ) {
        UT_COMMENT( "Failed to open GLFW window\n" );
        UT_CHECK_OUTPUT( !window );
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if( glewInit() != GLEW_OK ) {
        UT_COMMENT( "Failed to initialize GLEW\n" );
        UT_CHECK_OUTPUT( glewInit() != GLEW_OK );
        return;
    }

    GLRenderer Renderer;
    if( !Renderer.loadShaders(
        "../../sw/shaders/vertex_shader.glsl",
        "../../sw/shaders/fragment_shader.glsl" ) ) {
            UT_COMMENT( "Failed to load shaders\n." );
            UT_CHECK_OUTPUT( false );
        return;
    }
    Renderer.init();

    // Dark blue background
    Renderer.setClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Make array of GLVertex and copy the cube's vertex coordinates
    // and set color data
    GLVertex vertices[ 36 ];
    for( int i = 0; i < 36; i++ ){
        int j = i * 3;
        vertices[ i ].x = g_vertex_buffer_data[ j ];
        vertices[ i ].y = g_vertex_buffer_data[ j + 1 ];
        vertices[ i ].z = g_vertex_buffer_data[ j + 2 ];
        vertices[ i ].r = 1;
        vertices[ i ].g = 1;
        vertices[ i ].b = 0;
    }
    GLRenderable* model[ 100 ];
    for( int i= 0; i < 100; i++ ) {
        model[ i ] = new GLRenderable();
	GLVertexDataStr& vertex_data = model[ i ]->getVertexDataRef();
	vertex_data.buffer_ptr = vertices[ 0 ].pos;
	vertex_data.buffer_size = sizeof( vertices );
	vertex_data.vertex_count = 12 * 3;
	model[ i ]->translate( (float)i / 100.0, 0, 0 );
	Renderer.addRenderable( model[ i ] );
    }


    // Rotate 30 degrees on x and y axes


    // Projection
    // 45 degree FOV, 4:3 ratio, display range: 0.1 unit <->100 units
    glm::mat4 projection_matrix =
        glm::perspective( 45.0f, 4.0f / 3.0f, 0.1f, 100.0f );

    // Camera matrix
    glm::mat4 view_matrix = glm::lookAt(
        glm::vec3( 0, 0, 10 ), // Camera is at (0,0,10), in world space
        glm::vec3( 0, 0, 0 ), // and looks at the origin
        glm::vec3( 0, 1, 0 ) ); // Head is up

    Renderer.setProjectionMatrix( projection_matrix );
    Renderer.setViewMatrix( view_matrix );

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    Timer timer = Timer();
    int frame_count = 0;
    // Main loop
    while (!glfwWindowShouldClose(window)) {
      //      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Renderer.draw();
	
	glfwSwapBuffers( window );
        glfwPollEvents();

	for( int i= 0; i < 100; i++ ) {	
	  model[ i ]->rotate( 1, 1, 0 ,0 );
	}

	// Calculate time to draw one frame
	++frame_count;
	if ( timer.getElapsed() > 1000 ) {
	  printf( "%f ms/frame (%d fps)\n", 1000.0/double(frame_count), frame_count );
	  frame_count = 0;
	  timer.reset();
	}
    }

    glfwTerminate();

    // Cleanup VBO
    Renderer.cleanup();
    for( int i= 0; i < 100; i++ ) {	
        delete model[ i ];
    }

    UT_END_STEP;

/* ------------------------------ */

    return;
}
