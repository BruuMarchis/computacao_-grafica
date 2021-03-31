/*
 "Hello World GLFW3". Just the basics.
 Dependencies:
    GLFW3
 by Luiz Gonzaga Jr @ Unisinos University
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1024, 768, "First Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	} 

	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	
	while (!glfwWindowShouldClose(window))
	{	
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
