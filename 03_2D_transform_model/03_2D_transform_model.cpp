/*
"2D Transform". Just the basics about model matrix
Dependencies:
GLM, GL3W and GLFW3 to start GL.
Based on LearnOpenGL
*/

#include <iostream>

#include <GL/gl3w.h> // here: we need compile gl3w.c - utils dir
#include <GLFW/glfw3.h>
#include "gl_utils.h" // parser for shader source files

#include <glm/glm.hpp> // GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static int width = 800, height = 600;
float dx = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    static bool gWireframe = 0;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        gWireframe = !gWireframe;
        if (gWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        if (dx >=-1.0f)
            dx -= 0.1;
    }

    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        if (dx <= 1.0f)
            dx += 0.1;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
            dx = 0.0;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height)
{
    glViewport(0, 0, _width, _height);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main( )
{
  GLFWwindow* g_window;

  if (!glfwInit())
    return -1;
#ifdef APPLE
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  g_window = glfwCreateWindow(width,height , "2D Transform", NULL, NULL);
  if (!g_window)
    {
      glfwTerminate();
      return -1;
    }

  glfwMakeContextCurrent( g_window );
  int screenWidth, screenHeight;
  glfwGetFramebufferSize(g_window, &screenWidth, &screenHeight);
  glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
  glfwSetKeyCallback(g_window, key_callback);

  if (gl3wInit()) {
      std::cout << "failed to initialize OpenGL\n" << std::endl;
      return -1;
  }

  if (!gl3wIsSupported(3, 2)) {
      std::cout << "OpenGL 3.2 not supported\n" << std::endl;
      return -1;
  }


  glViewport( 0, 0, screenWidth, screenHeight );


  char vertex_shader[1024 * 256];
  char fragment_shader[1024 * 256];
  parse_file_into_str( "core_vs.glsl", vertex_shader, 1024 * 256 );
  parse_file_into_str( "core_frag.glsl", fragment_shader, 1024 * 256 );

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *p = (const GLchar *)vertex_shader;
  glShaderSource(vs, 1, &p, NULL);
  glCompileShader(vs);

  GLint result;
  GLchar infoLog[512];
  glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
  if (!result)
    {
      glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
      std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
    }

  GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
  p = (const GLchar *)fragment_shader;
  glShaderSource( fs, 1, &p, NULL );
  glCompileShader(fs);

  glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
  if (!result)
    {
      glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
      std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
    }

  GLint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vs);
  glAttachShader(shaderProgram, fs);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
  if (!result)
    {
      glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
      std::cout << "Error! Shader program linker failure. " << infoLog  << std::endl;
    }
  glDeleteShader(vs);
  glDeleteShader(fs);


  GLfloat vertices[] =
    {
      // Positions         // Colors
      0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
      -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
      0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
    };
  GLuint VBO, VAO;
  glGenVertexArrays( 1, &VAO );
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(VAO);

  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

  // Position attribute
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) 0 );
  glEnableVertexAttribArray( 0 );
  // Color attribute
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
  glEnableVertexAttribArray( 1 );
  glBindVertexArray( 0 ); // Unbind VAO
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glUseProgram(shaderProgram);

  // create transformations
  glm::mat4 model_cpu(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");

  // rendering loop
  while ( !glfwWindowShouldClose( g_window ) )
    {
      // pass them to the shaders (3 different ways)
      model_cpu = glm::translate(glm::mat4(1.0f), glm::vec3(dx, 0.0f, 0.0f));
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_cpu));

      glfwPollEvents( );
      glClear( GL_COLOR_BUFFER_BIT );
      glBindVertexArray( VAO );
      glDrawArrays( GL_TRIANGLES, 0, 3 );
      glBindVertexArray(0);
      // Swap the screen buffers
      glfwSwapBuffers( g_window );
    }

  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays( 1, &VAO );
  glDeleteBuffers( 1, &VBO );

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate( );

  return EXIT_SUCCESS;
}
