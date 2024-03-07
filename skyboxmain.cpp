#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

//GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES
#include <cmath>

GLFWwindow* window;

// variables to edit
float window_width = 600,
window_height = 600,
near = 0.1f,
far = 100.f,
x = 0,
y = 0,
xTheta = 1,
yTheta = 1,
s = 1,
z = -2;

void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
        y += 0.01;
    if (key == GLFW_KEY_S && action == GLFW_REPEAT)
        y -= 0.01;
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
        x += 0.01;
    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
        x -= 0.01;
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
        xTheta += 1;
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
        xTheta -= 1;
    if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
        yTheta += 1;
    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
        yTheta -= 1;
    if (key == GLFW_KEY_E && action == GLFW_REPEAT)
        s += 0.01;
    if (key == GLFW_KEY_Q && action == GLFW_REPEAT)
        s -= 0.01;
    if (key == GLFW_KEY_Z && action == GLFW_REPEAT)
        z += 0.01;
    if (key == GLFW_KEY_X && action == GLFW_REPEAT)
        z -= 0.01;

}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Dun Gerald C. Baniqued", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // View Matrix Start
    glm::vec3 cameraPos = glm::vec3(0, 0, 10.f);

    glm::mat4 cameraPositionMatrix =
        glm::translate(glm::mat4(1.0f),
            cameraPos * -1.0f);

    glm::vec3 WorldUp = glm::vec3(0, 1.0f, 0);

    glm::vec3 Center = glm::vec3(0, 3.f, 0);

    glm::vec3 F = glm::normalize(glm::vec3(Center - cameraPos));

    glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));

    glm::vec3 U = glm::normalize(glm::cross(R, F));

    glm::mat4 cameraOrientation = glm::mat4(1.f);

    glm::mat4 viewMatrix = glm::lookAt(cameraPos, Center, WorldUp);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    // View Matrix End

    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample1.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes =
        stbi_load("3D/ayaya.png", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_bytes);

    glViewport(0, 0, window_width, window_height); // MIn x, y, width , height

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    std::string path = "3D/myCube.obj";
    std::vector <tinyobj::shape_t> shapes;
    std::vector <tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO_UV);

    //Currently editing VAO = null
    glBindVertexArray(VAO);
    //Currently editing  VAO = VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Currently editing VBO = VBO
    //VAO <- VBO

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        attributes.vertices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, // 0 pos, 2 texture
        3, //  3 components
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    // VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // VBO = EBO

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), &UV[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_matrix = glm::mat4(1.f);

    glfwSetKeyCallback(window, Key_Callback);

    glm::mat4 projection = glm::perspective(glm::radians(60.f), window_height / window_width, near, far);


    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProg);

        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(s, s, s));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(yTheta), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(xTheta), glm::normalize(glm::vec3(0.0f, 1.0f, 0.f)));
        
        
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");

        glBindTexture(GL_TEXTURE_2D, texture);

        glUniform1i(tex0Address, 0);

        
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
