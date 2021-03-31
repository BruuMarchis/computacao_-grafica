#include <GLFW/glfw3.h>
int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "First Window (GLFW)", nullptr, nullptr); // janela
    glfwMakeContextCurrent(window);
      
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();
}
