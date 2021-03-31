#define GLEW_STATIC
#include <iostream>
#include "GL/glew.h" // gl3w,glee
#include <GLFW/glfw3.h> // sfml, glut,...

int main()
{
    std::cout << "****OpenGL Detection****" << std::endl;
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL detection!", nullptr, nullptr); 
    glfwMakeContextCurrent(window);
      
     // IMPORTANT: remove GLEW comments if OpenGL 1.x, 2.x has been detected 
     // OpenGL 3.2+ is ideal...attention for Graphics Card

    
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();
}
