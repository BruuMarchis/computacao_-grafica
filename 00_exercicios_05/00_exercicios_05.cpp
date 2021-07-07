#define STB_IMAGE_IMPLEMENTATION
#include <GL/gl3w.h> // here: we need compile gl3w.c - utils dir
#include <GLFW/glfw3.h>
#include "gl_utils.h" // parser for shader source files

#include <filesystem.h>
#include <shader_m.h>
#include <camera.h>
#include <model.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include<ImGui/imgui.h>
#include<ImGui/imgui_impl_glfw.h>
#include<ImGui/imgui_impl_opengl3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseCan = false;

//posicionamento
float rotateX = 0.0f;
float rotateY = 0.0f;
float rotateZ = 0.0f;
float moveX = 0.5f;
float moveY = -0.8f;
float moveZ = 0.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int frameCount = 0;
float previousTime = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Loading model...", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    if (gl3wInit()) {
        std::cout << "failed to initialize OpenGL\n" << std::endl;
        return -1;
    }

    if (!gl3wIsSupported(3, 2)) {
        std::cout << "OpenGL 3.2 not supported\n" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
    // 


    double currentTime = glfwGetTime();


    static const char* items[]{"Bueiro", "Quadro", "Tronco"};
    static int selectedItem = 0;
    int lastItem = 0;
    Model ourModels[]{ (FileSystem::getPath("data/bueiro/bueiro.obj")),
                        (FileSystem::getPath("data/quadro/quadro.obj")),
                        (FileSystem::getPath("data/tronco/tronco.obj"))
    };
    // 
    //Model ourModel(FileSystem::getPath("data/cyborg/cyborg.obj"));
    //ourModels[0] = new Model(FileSystem::getPath("data/nanosuit/nanosuit.obj"));
    //Model ourModel(FileSystem::getPath("data/planet/planet.obj"));
    //Model ourModel(FileSystem::getPath("data/sponza/sponza.obj"));


    int newTime;
    newTime = glfwGetTime() - currentTime;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");



    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        if (lastItem != selectedItem) {

            rotateX = 0.0f;
            rotateY = 0.0f;
            rotateZ = 0.0f;
            moveX = 0.5f;
            moveY = -0.8f;
            moveZ = 0.0f;

            lastItem = selectedItem;

        }

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        currentTime = currentFrame;
        frameCount++;
        // If a second has passed.
        if (currentTime - previousTime >= 1.0)
        {
            // Display the frame count here any way you want.
            system("cls");
            std::cout << "Tempo para carregar os modelos: " << newTime << " segundos." << std::endl;
            //std::cout << "Frames por segundo: " << frameCount<< std::endl;

            frameCount = 0;
            previousTime = currentTime;
        }



        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(moveX, moveY, moveZ)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1.0f, 0.0, 0.0));
        model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0.0, 1.0f, 0.0));
        model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0.0, 0.0, 1.0f));
        ourShader.setMat4("model", model);
        ourModels[selectedItem].Draw(ourShader);


        ImGui::Begin("Trabalho GC");

        ImGui::Text(" ");
        ImGui::ListBox("Modelos", &selectedItem, items, IM_ARRAYSIZE(items));
        ImGui::Checkbox("Liberar Mouse", &mouseCan);
        ImGui::SliderFloat("Rotacionar X", &rotateX, 0.0f, 360.0f);
        ImGui::SliderFloat("Rotacionar Y", &rotateY, 0.0f, 360.0f);
        ImGui::SliderFloat("Rotacionar Z", &rotateZ, 0.0f, 360.0f);
        ImGui::SliderFloat("Mover em X", &moveX, -2.0f, 2.0f);
        ImGui::SliderFloat("Mover em Y", &moveY, -2.0f, 2.0f);
        ImGui::SliderFloat("Mover em Z", &moveZ, -2.0f, 2.0f);

        ImGui::End();

        ImGui::Render(); 
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    static bool gWireframe = 0;     
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)   
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);


    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        rotateY -=0.1;
        if(rotateY < 0){
            rotateY = 360.0f - rotateY;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        rotateY += 0.1;
        if (rotateY > 360) {
            rotateY = rotateY - 360.0f;
        }
    }


    //

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseCan) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
