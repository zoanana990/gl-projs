#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH=800, HEIGHT=600;

int main() {
    /* Initialize GLFW */
    if (!glfwInit()) {
        printf("GLFW initialization failed\n");
        glfwTerminate();
        return 1;
    }

    /* Setup GLFW window properties */
    /* OpenGL version 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Core profile = no backwards compatibility */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Allow forward compatibility */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* create the window */
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "First Window", nullptr, nullptr);
    if (mainWindow == nullptr)
    {
        printf("GLFW window creation failed\n");
        glfwTerminate();
        return 1;
    }

    /* Get the buffer size information */
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    /* Set context for GLEW to use */
    glfwMakeContextCurrent(mainWindow);

    /* Allow modern extension features */
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    /* Setup viewport size */
    glViewport(0, 0, bufferHeight, bufferHeight);

    /* Loop until window closed */
    while(!glfwWindowShouldClose(mainWindow))
    {
        /* Get handle user input events */
        glfwPollEvents();

        /* clear window */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}