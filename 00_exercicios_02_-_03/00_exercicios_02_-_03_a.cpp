#include <iostream>

#include <GL/gl3w.h> 
#include <GLFW/glfw3.h>
#include "gl_utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

static int width = 800, height = 600;
float dx = 1.0f, dy=1.0f,dz=5.0f;
glm::mat4 view = glm::mat4(1.0);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    static bool gWireframe = 0;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        gWireframe = !gWireframe;
        if (gWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
            dx -= 1.0;
    }

    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
            dx += 1.0;
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        dy += 1.0;
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        dy -= 1.0;
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        dz += 1.0;
    }
    else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        dz -= 1.0;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        dx = dy = 0.0f;
        dz = 5.0;
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        std::cout << "dx,dy,dz = (" << dx << "," << dy << "," << dz << ")" << std::endl;
        std::cout << "view matrix " << glm::to_string(view) << std::endl;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height)
{
    glViewport(0, 0, _width, _height);
}


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

  g_window = glfwCreateWindow(width,height , "3D Transform", NULL, NULL);
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
  glEnable(GL_DEPTH_TEST);
  

  char vertex_shader[1024 * 256];
  char fragment_shader[1024 * 256];
  parse_file_into_str( "transf_vs.glsl", vertex_shader, 1024 * 256 );
  parse_file_into_str( "transf_fs.glsl", fragment_shader, 1024 * 256 );

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


  float vertices[] = {     
      1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   
      0.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f, 
      1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
      0.5f, 2.0f, 0.5f,   0.0f, 0.0f, 0.0f


  };
  unsigned int index[] = {
      3, 1, 0,
      1, 3, 2,
      5, 7, 4,
      7, 5, 6,
      5, 0, 1,
      0, 5, 4,
      6, 1, 2,
      1, 6, 5,
      6, 3, 7,
      3, 6, 2,
      8, 3, 0,
      8, 7, 4,
      8, 4, 0,
      8, 7, 3
  };

  GLuint VBO, VAO, ibo;
  glGenVertexArrays( 1, &VAO );
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &ibo);

  glBindVertexArray(VAO);

  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) 0 );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
  glEnableVertexAttribArray( 1 );

  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindVertexArray(0);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glUseProgram(shaderProgram);

  glm::mat4 model(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
  model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0, 0.5, 3.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
 
  glm::mat4 projection(1.0);

  projection = glm::perspective(glm::radians(45.0f), 
                                (float)screenWidth / (float)screenHeight, 
                                0.4f, 
                                200.0f);                     
  unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  

  std::cout << "Camera position" << std::endl;
  std::cout << "dx,dy,dz = (" << dx << "," << dy << "," << dz << ")" << std::endl;

  while ( !glfwWindowShouldClose( g_window ) )
    {
      view = glm::mat4(1.0f);
      view = glm::lookAt(glm::vec3(dx, dy, dz), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

      glfwPollEvents( );
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glBindVertexArray( VAO );
      glDrawElements(GL_TRIANGLES, sizeof(index), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
      glfwSwapBuffers( g_window );
    }

  glDeleteVertexArrays( 1, &VAO );
  glDeleteBuffers( 1, &VBO );
  glDeleteBuffers(1, &ibo);

  glfwTerminate( );

  return EXIT_SUCCESS;
}
