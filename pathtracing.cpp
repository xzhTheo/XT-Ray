#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene/scene.h"
#include "shader/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "scene/scenecreator.h"
#include <iostream>
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
 
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int SAMPLES = 1;
int CHANNELS = 4;
 
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 viewPos = glm::vec3(0.0f, 2.0f, 4.0f);
glm::vec3 viewFront = glm::vec3(0, 0, -1);
glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
 
 
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My_RayTracing", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader("shader/vertex", "shader/fragment");
    // 设置光线、平面、球体的参数，并把它们加入场景
    
    // ------------------------------------------------------------------
    Scene* scene = SceneCreator::GetInstance()->CreateBasicScene();

    float vertices[] = {
        // positions           // texture coords
         1.0f,  1.0f, 0.0f,    1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,    1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = new unsigned char[SCR_WIDTH * SCR_HEIGHT * 4];

    model = glm::mat4(1.0f);
    view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
    projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

    // 初始化观察右向量
    glm::vec3 viewRight = glm::normalize(glm::cross(viewFront, viewUp));
    for(unsigned int i = 0; i < SAMPLES; i++)
    {
        std::cout << "Rendering: " << i + 1 << "/" << SAMPLES << std::endl;
        for (unsigned int j = 0; j < SCR_HEIGHT; j++)
        {
            for (unsigned int i = 0; i < SCR_WIDTH; i++)
            {
                // 将像素坐标分量映射到[0, 1]
                glm::vec3 pos(float(i) * 2 / SCR_WIDTH - 1.0f, float(j) * 2 / SCR_HEIGHT - 1.0f, 0.0f);    
                // 计算像素在世界坐标中的位置
                glm::vec3 globalPos = viewPos + viewFront + pos.x * viewRight * (float(SCR_WIDTH) / SCR_HEIGHT) + pos.y * viewUp;
    
                // 计算出光线并进行光线追踪
                Ray ray(viewPos, globalPos);
                glm::vec3 color = scene->traceRay(ray,4);
                // auto r = double(i) / (SCR_WIDTH-1);
                // auto g = double(j) / (SCR_HEIGHT-1);
                // auto b = 0.0;
                data[j * SCR_WIDTH * 4 + i * 4 + 0] = int(255.99 * color.r);
                data[j * SCR_WIDTH * 4 + i * 4 + 1] = int(255.99 * color.g);
                data[j * SCR_WIDTH * 4 + i * 4 + 2] = int(255.99 * color.b);
                data[j * SCR_WIDTH * 4 + i * 4 + 3] = 255;
    
            }
        }
    }
    // 加载并生成纹理
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    while (!glfwWindowShouldClose(window))
    {
        shader.use();
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}