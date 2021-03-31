/*
  "Hello World GLFW3". Just the basics.
  Dependencies:
  GLFW3
  - add ifdef/endif APPLE
  - add opengl callback (reshape)
  - ATTENTION: change glew by gl3w for opengl
  by Luiz Gonzaga Jr @ Unisinos University
*/
#include <iostream>
#include <GL/gl3w.h> // here: we need compile gl3w.c - utils dir
#include <GLFW/glfw3.h> 

static int width = 800, height = 600;

static void reshape(GLFWwindow *window, int w, int h)
{
  width = w > 1 ? w : 1;
  height = h > 1 ? h : 1;
  glViewport(0, 0, width, height);
  std::cout << "(" << width << "," << height << ")" << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  std::cout << key << std::endl;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(void)
{
  GLFWwindow* window;
  std::cout << "Starting APP..."<<std::endl;
  if (!glfwInit())
    return -1;
#ifdef APPLE
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  window = glfwCreateWindow(width, height, "Hello Window and GL3W", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, reshape);

  if (gl3wInit()) {
    std::cout << "failed to initialize OpenGL\n" << std::endl;
    return -1;
  }

  if (!gl3wIsSupported(3, 2)) {
    std::cout << "OpenGL 3.2 not supported\n" << std::endl;
    return -1;
  }
  std::cout << "OpenGL: " <<  glGetString(GL_VERSION);
  std::cout <<  "  GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  /* OpenGL */

  /*r,g,b,a*/
  glClearColor(0.9f, 0.3f, 0.3f, 1.0f); /*cor de fundo */

  while (!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT);
      glfwSwapBuffers(window); /*front buffer and back buffer */
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
