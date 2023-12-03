#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH=800, HEIGHT=600;

GLuint VAO, VBO, shader;

/**
 * vec3: three floating pointer vector
 * vec4: four floating pointer vector
 *
 * layout: input position
 * out: used in shader output variable
 * */
/* vertex shader */
static const char* vShader =
        "#version 330\n"                        /* GLSL version 3.3 */
        "layout(location = 0) in vec3 pos;\n"   /* vertex position, vec3 attribute */
        "void main() {\n"                       /* void main(), each vertex will enter this function, assign the value to gl_Position */
        "   gl_Position = vec4(0.3 * pos.x, 0.4 * pos.y, pos.z, 1.0);\n"
        "}";

/* Fragment shader */
static const char* fShader =
        "#version 330\n"        /* GLSL version 3.3 */
        "out vec4 colour;\n"    /* Shader fragment */
        "void main() {\n"       /* set fragment color */
        "   colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}";

/**
 * In OpenGL,
 * +------------------------+
 * |                        |
 * |                        |
 * |         (0, 1)         |
 * | (-1, 0) (0, 0) (1, 0)  |
 * |         (0,-1)         |
 * |                        |
 * |                        |
 * +------------------------+
 * */
void createTriangle() {
    GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
    };
    /* here create an set the VAO and VBO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* set the vector attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    /* bind buffer */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void addShader(GLuint shader, const GLchar *const * ptr_shader, GLenum flag){
    GLuint add_shader = glCreateShader(flag);
    glShaderSource(add_shader, 1, ptr_shader, nullptr);
    glCompileShader(add_shader);
    glAttachShader(shader, add_shader);
}

void compileShaders() {
    int rc;
    GLchar log[1024] = {0};

    shader = glCreateProgram();
    if (!shader) {
        printf("Error creating shader program\n");
        return;
    }

    addShader(shader, &vShader, GL_VERTEX_SHADER);
    addShader(shader, &fShader, GL_FRAGMENT_SHADER);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &rc);
    if(!rc) {
        glGetProgramInfoLog(shader, sizeof(log), nullptr, log);
        printf("Error when link the program, %s\n", log);
    }
}

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
    glViewport(0, 0, bufferWidth, bufferHeight);

    /* Create triangle */
    createTriangle();

    compileShaders();

    glUseProgram(shader);

    /* Loop until window closed */
    while(!glfwWindowShouldClose(mainWindow))
    {
        /* Get handle user input events */
        glfwPollEvents();

        /* clear window */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw the triangle */
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(mainWindow);
    }

    /* Release the resource */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);

    /* terminate */
    glfwTerminate();

    return 0;
}