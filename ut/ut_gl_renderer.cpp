/******************************************************************************/
/**
    Performance testing (kind of..) for GLRenderer class.
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
// OpenGL, GLFW and GLM headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// UT specific headers
#include "ut_includes.h"

// Testocore SW headers
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
    TestCase( const char* name ) : TestCaseBase( name ), kModelCount( 1000 ) {}
    ~TestCase() { }
    void runTest();

private:
    const uint32_t kModelCount;
    GLFWwindow* create_glfw_window();
    void load_cubes( GLVertex* vertices, GLRenderable** model );
};

int main( void ) {

    TestCase TC( "ut_gl_renderer" );

    TC.execute();

    return 0;
}

/* -----------------------------------------------------------------------------
 * Define test script here.
 */
void TestCase::runTest( void ) {

/* ------------------------------ */

    UT_START_STEP( 1 );

    UT_COMMENT( "Starting test with " << kModelCount
        <<" preloaded cubes..\n\n" );

    GLFWwindow* window = create_glfw_window();

    GLVertex vertices[ 36 ];
    GLRenderable* model[ kModelCount ];
    load_cubes( vertices, model );

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

    // Load all models into GPU memory
    for( uint32_t i= 0; i < kModelCount; i++ ) {
        Renderer.addRenderable( model[ i ] );
    }

    // 45 degree FOV, 4:3 ratio, display range: 0.1 unit <->100 units
    Renderer.setProjectionMatrix(
        glm::perspective( 45.0f, 4.0f / 3.0f, 0.1f, 100.0f ) );

    Renderer.setViewMatrix( glm::lookAt(
        glm::vec3( 0, 10, 25 ), // Camera is at (0,0,10), in world space
        glm::vec3( 0, 0, 0 ), // and looks at the origin
        glm::vec3( 0, 1, 0 ) ) // Head is up
        );

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    Timer timer = Timer();
    Timer test_duration_timer = Timer();
    int frame_count = 0;
    int total_frame_count = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
      //      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Renderer.draw();

        glfwSwapBuffers( window );
        glfwPollEvents();

        // Rotate cubes
        int rotation_axis;
        for( uint32_t i= 0; i < kModelCount; i++ ) {
            if( rotation_axis == 0 )
                model[ i ]->rotate( (float)i / 100 + 1, 1, 0, 0 );
            else if( rotation_axis == 1 )
                model[ i ]->rotate( (float)i / 100 + 1, 0, 1, 0 );
            else
                model[ i ]->rotate( (float)i / 100 + 1, 0, 0, 1 );
        }

        // Calculate time to draw one frame
        ++frame_count;
        if ( timer.getElapsed() > 1000 ) {
            printf( "%f ms/frame (%d fps)\n", 1000.0/double(frame_count),
                frame_count );

            frame_count = 0;
            timer.reset();
            rotation_axis = rand() % 3; // change rotation axis every second
        }
        ++total_frame_count;
        // End this step when 6 seconds have passed
        if( test_duration_timer.getElapsed() > 6000 ) break;
    }
    UT_CHECK_OUTPUT( test_duration_timer.getElapsed() > 6000 );
    UT_COMMENT( "Average fps: " << ( double )total_frame_count / 6 << "\n" );

    glfwTerminate();

    // Cleanup VBO
    Renderer.cleanup();
    for( uint32_t i= 0; i < kModelCount; i++ ) {
        delete model[ i ];
    }

    UT_END_STEP;

/* ------------------------------ */

    UT_START_STEP( 2 );

    UT_COMMENT( "Starting test with " << kModelCount <<
        " cubes which are loaded and removed\n" <<
        "from GPU memory during each frame..\n\n" );

    GLFWwindow* window = create_glfw_window();

    GLVertex vertices[ 36 ];
    GLRenderable* model[ kModelCount ];
    load_cubes( vertices, model );

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

    // 45 degree FOV, 4:3 ratio, display range: 0.1 unit <->100 units
    Renderer.setProjectionMatrix(
        glm::perspective( 45.0f, 4.0f / 3.0f, 0.1f, 100.0f ) );

    Renderer.setViewMatrix( glm::lookAt(
        glm::vec3( 0, 10, 25 ), // Camera is at (0,0,10), in world space
        glm::vec3( 0, 0, 0 ), // and looks at the origin
        glm::vec3( 0, 1, 0 ) ) // Head is up
        );

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    Timer timer = Timer();
    Timer test_duration_timer = Timer();
    int frame_count = 0;
    int total_frame_count = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Load each model into GPU memory
        for( uint32_t i= 0; i < kModelCount; i++ ) {
            Renderer.addRenderable( model[ i ] );
        }

        Renderer.draw();

        glfwSwapBuffers( window );
        glfwPollEvents();

        // Rotate cubes
        int rotation_axis;
        for( uint32_t i= 0; i < kModelCount; i++ ) {
            if( rotation_axis == 0 )
                model[ i ]->rotate( (float)i / 100 + 1, 1, 0, 0 );
            else if( rotation_axis == 1 )
                model[ i ]->rotate( (float)i / 100 + 1, 0, 1, 0 );
            else
                model[ i ]->rotate( (float)i / 100 + 1, 0, 0, 1 );
        }

        // Calculate time to draw one frame
        ++frame_count;
        if ( timer.getElapsed() > 1000 ) {
            printf( "%f ms/frame (%d fps)\n", 1000.0/double(frame_count),
                frame_count );
            frame_count = 0;
            timer.reset();
            rotation_axis = rand() % 3; // change rotation axis every second
        }

        // Remove each model from GPU memory
        for( uint32_t i= 0; i < kModelCount; i++ ) {
            Renderer.removeRenderable( model[ i ]->getId() );
        }
        ++total_frame_count;
        // End this step when 6 seconds have passed
        if( test_duration_timer.getElapsed() > 6000 ) break;
    }
    UT_CHECK_OUTPUT( test_duration_timer.getElapsed() > 6000 );
    UT_COMMENT( "Average fps: " << ( double )total_frame_count / 6 << "\n" );

    glfwTerminate();

    // Cleanup VBO
    Renderer.cleanup();
    for( uint32_t i= 0; i < kModelCount; i++ ) {
        delete model[ i ];
    }

    UT_END_STEP;

/* ------------------------------ */

    UT_START_STEP( 3 );

    UT_COMMENT( "Starting test with "<< kModelCount <<
        " cubes, using memory pool with " << "GLRenderer\n\n" );

    GLFWwindow* window = create_glfw_window();
    GLVertex vertices[ 36 ];
    GLRenderable* model[ kModelCount ];
    load_cubes( vertices, model );

    MemPoolManager* PoolMngr = new MemPoolManager();
    PoolMngr->addPool( new MemoryPool( 100, kModelCount ) );

    // Set global pointer for mempool allocations
    __kMEMPOOLMANAGER = PoolMngr;

    GLRenderer Renderer = GLRenderer( GLRenderer::ALLOC_TYPE_MEM_POOL );

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

    // 45 degree FOV, 4:3 ratio, display range: 0.1 unit <->100 units
    Renderer.setProjectionMatrix(
        glm::perspective( 45.0f, 4.0f / 3.0f, 0.1f, 100.0f ) );

    Renderer.setViewMatrix( glm::lookAt(
        glm::vec3( 0, 10, 25 ), // Camera is at (0,0,10), in world space
        glm::vec3( 0, 0, 0 ), // and looks at the origin
        glm::vec3( 0, 1, 0 ) ) // Head is up
        );

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    Timer timer = Timer();
    Timer test_duration_timer = Timer();
    int frame_count = 0;
    int total_frame_count = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Load each model into GPU memory
        for( uint32_t i= 0; i < kModelCount; i++ ) {
            Renderer.addRenderable( model[ i ] );
        }

        Renderer.draw();

        glfwSwapBuffers( window );
        glfwPollEvents();

        // Rotate cubes
        int rotation_axis;
        for( uint32_t i = 0; i < kModelCount; i++ ) {
            if( rotation_axis == 0 )
                model[ i ]->rotate( (float)i / 100 + 1, 1, 0, 0 );
            else if( rotation_axis == 1 )
                model[ i ]->rotate( (float)i / 100 + 1, 0, 1, 0 );
            else
                model[ i ]->rotate( (float)i / 100 + 1, 0, 0, 1 );
        }

        // Calculate time to draw one frame
        ++frame_count;
        if ( timer.getElapsed() > 1000 ) {
            printf( "%f ms/frame (%d fps)\n", 1000.0/double(frame_count),
                frame_count );
            frame_count = 0;
            timer.reset();
            rotation_axis = rand() % 3; // change rotation axis every second
        }

        // Remove each model from GPU memory
        for( uint32_t i = 0; i < kModelCount; i++ ) {
            Renderer.removeRenderable( model[ i ]->getId() );
        }
        ++total_frame_count;
        // End this step when 6 seconds have passed
        if( test_duration_timer.getElapsed() > 6000 ) break;
    }
    UT_CHECK_OUTPUT( test_duration_timer.getElapsed() > 6000 );
    UT_COMMENT( "Average fps: " << ( double )total_frame_count / 6 << "\n" );

    glfwTerminate();

    // Cleanup VBO
    Renderer.cleanup();
    for( uint32_t i= 0; i < kModelCount; i++ ) {
        delete model[ i ];
    }

    PoolMngr->clearAllPools();
    delete PoolMngr;

    UT_END_STEP;

/* ------------------------------ */

    return;
}

GLFWwindow* TestCase::create_glfw_window() {
    GLFWwindow* window;

    if( !glfwInit() ) {
        UT_COMMENT( "Failed to initialize GLFW\n" );
        return NULL;
    }

    glfwWindowHint( GLFW_SAMPLES, 4 ); // 4x antialiasing
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); //OpenGL 3.3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // Open window and create openGL context
    window = glfwCreateWindow( 1024, 768, "ut_gl_renderer", NULL, NULL );
    if( !window ) {
        UT_COMMENT( "Failed to open GLFW window\n" );
        UT_CHECK_OUTPUT( !window );
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if( glewInit() != GLEW_OK ) {
        UT_COMMENT( "Failed to initialize GLEW\n" );
        UT_CHECK_OUTPUT( glewInit() != GLEW_OK );
        return NULL;
    }
    return window;
}

// Allocates memory for kModelCount GLRenderables (cubes)
void TestCase::load_cubes( GLVertex* vertices, GLRenderable** model ) {
    // Make array of GLVertex and copy the cube's vertex coordinates
    // and set color data
    for( uint32_t i = 0; i < 36; i++ ){
        int j = i * 3;
        vertices[ i ].x = g_vertex_buffer_data[ j ];
        vertices[ i ].y = g_vertex_buffer_data[ j + 1 ];
        vertices[ i ].z = g_vertex_buffer_data[ j + 2 ];
        vertices[ i ].r = rand() % 2;
        vertices[ i ].g = rand() % 2;
        vertices[ i ].b = rand() % 2;
    }
    for( uint32_t i= 0; i < kModelCount; i++ ) {
        model[ i ] = new GLRenderable();
        GLVertexDataStr& vertex_data = model[ i ]->getVertexDataRef();
        vertex_data.buffer_ptr = vertices[ 0 ].pos;
        vertex_data.buffer_size = 36 * sizeof( GLVertex );
        vertex_data.vertex_count = 12 * 3;
        model[ i ]->translate(
            rand() % 20 - 10,
            rand() % 20 - 10,
            rand() % 20 - 10 );
    }
}
