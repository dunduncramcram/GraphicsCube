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
xPos = 0,
yPos = 0,
zPos = -2,
xTheta = 1,
yTheta = 1,
scale = 0.1;

void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
        yPos += 0.05;
    if (key == GLFW_KEY_S && action == GLFW_REPEAT)
        yPos -= 0.05;
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
        xPos += 0.05;
    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
        xPos -= 0.05;
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
        xTheta += 1;
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
        xTheta -= 1;
    if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
        yTheta += 1;
    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
        yTheta -= 1;
    if (key == GLFW_KEY_E && action == GLFW_REPEAT)
        scale += 0.01;
    if (key == GLFW_KEY_Q && action == GLFW_REPEAT)
        scale -= 0.01;
    if (key == GLFW_KEY_Z && action == GLFW_REPEAT)
        zPos += 0.03;
    if (key == GLFW_KEY_X && action == GLFW_REPEAT)
        zPos -= 0.03;

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
    // View Matrix End


    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;

    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample1.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    std::fstream skyboxVertSrc("Shaders/Skybox.vert");
    std::stringstream skyboxVertBuff;

    skyboxVertBuff << skyboxVertSrc.rdbuf();

    std::string skyboxVertS = skyboxVertBuff.str();
    const char* sky_v = skyboxVertS.c_str();

    std::fstream skyboxfragSrc("Shaders/Skybox.frag");
    std::stringstream skyboxfragBuff;

    skyboxfragBuff << skyboxfragSrc.rdbuf();

    std::string skyboxfragS = skyboxfragBuff.str();
    const char* sky_f = skyboxfragS.c_str();

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

    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes =
        stbi_load("3D/partenza.jpg", &img_width, &img_height, &colorChannels, 0);

    //texture
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_bytes);

    glViewport(0, 0, window_width, window_height); // MIn x, y, width , height


    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint skyboxvertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyboxvertexShader, 1, &sky_v, NULL);
    glCompileShader(skyboxvertexShader);

    GLuint skyboxfragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyboxfragShader, 1, &sky_f, NULL);
    glCompileShader(skyboxfragShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

        std::cout << &errorLog[0] << std::endl;
    }

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);

        std::cout << &errorLog[0] << std::endl;
    }

    glGetShaderiv(skyboxvertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(skyboxvertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(skyboxvertexShader, maxLength, &maxLength, &errorLog[0]);

        std::cout << &errorLog[0] << std::endl;
    }

    glGetShaderiv(skyboxfragShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(skyboxfragShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(skyboxfragShader, maxLength, &maxLength, &errorLog[0]);

        std::cout << &errorLog[0] << std::endl;
    }

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);


    GLuint skyboxShaderProg = glCreateProgram();
    glAttachShader(skyboxShaderProg, skyboxvertexShader);
    glAttachShader(skyboxShaderProg, skyboxfragShader);

    glLinkProgram(skyboxShaderProg);


    std::string path = "3D/djSword.obj";
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

    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]
        );

        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]
        );

        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );

    }

    /*
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }
    */

    unsigned int skyboxVAO, SkyboxVBO, SkyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &SkyboxVBO);
    glGenBuffers(1, &SkyboxEBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, SkyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, // 0 pos, 2 texture
        3, //  3 components
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SkyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png"
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    //avoid skybox pixelating
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //Texture stretcehs to edge
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyChannel, 0);

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );

            stbi_image_free(data);

        }
    }

    stbi_set_flip_vertically_on_load(true);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Currently editing VAO = null
    glBindVertexArray(VAO);
    //Currently editing  VAO = VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Currently editing VBO = VBO
    //VAO <- VBO

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0, // 0 pos, 2 texture
        3, //  3 components
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    GLintptr normPtr = 3 * sizeof(float);

    glVertexAttribPointer(
        1, //  1 normals
        3, //  3 components
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)normPtr
    );

    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);

    glVertexAttribPointer(
        2, // 0 pos, 2 texture
        2, //  2 components
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)uvPtr
    );

    glEnableVertexAttribArray(2);

    /*
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );
    */

    /*
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), &UV[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(2);
    */

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glm::mat4 identity_matrix = glm::mat4(1.f);

    glfwSetKeyCallback(window, Key_Callback);

    glm::mat4 projection = glm::perspective(glm::radians(60.f), window_height / window_width, near, far);

    glm::vec3 lightPos = glm::vec3(0, 0, 0);
    glm::vec3 lightColor = glm::vec3(1,1,1);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16;

    float brightness = 10;

    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShaderProg);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProg, "view");
        glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShaderProg, "projection");
        glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProg);

        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(xPos, yPos, zPos));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale, scale, scale));
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


        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));


        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));


        GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

        GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);


        GLuint brightnessAddress = glGetUniformLocation(shaderProg, "brightness");
        glUniform1f(brightnessAddress, brightness);

        glBindVertexArray(VAO);

        //glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
