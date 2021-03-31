
#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h> // sfml, glut,...


int main()
{
    std::cout << "****OpenGL Detection****" << std::endl;
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL detection!", nullptr, nullptr); 
    glfwMakeContextCurrent(window);
  
     // OpenGL 3.2+ is ideal...attention for Graphics Card
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
  /*
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();
    */

}
