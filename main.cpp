#include <fstream>
#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static std::string read_source(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate);
    if (!file.is_open()) {
        std::cout << "Failed to open " << filename << std::endl;
    }
    auto size = file.tellg();
    std::string source(size, '\0');
    file.seekg(0);
    file.read(&source[0], size);
    return source;
}

int main() {
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cout << "Failed to start GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto *window =glfwCreateWindow(400, 400, "Hello World", NULL, NULL);
    if (window == nullptr) {
        glfwTerminate();
        std::cout << "Failed to create glfw window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
    std::string source = read_source("shader.vert");
    const char *psource = source.c_str();
    glShaderSource(vert_shader, 1, &psource, NULL);
    glCompileShader(vert_shader);

    char buf[1024];
    GLsizei len;


    auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    source = read_source("shader.frag");
    psource = source.c_str();
    glShaderSource(frag_shader, 1, &psource, NULL);
    glCompileShader(frag_shader);

    glGetShaderInfoLog(frag_shader, 1024, &len, buf);

    auto shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);


    // glGetProgramInfoLog(shader_program, 1024, &len, buf);
    std::cout << buf << std::endl;

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glViewport(0, 0, 400, 400);

    GLfloat verticies[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };

    GLint indicies[] = {
        0, 1, 3,
        3, 1, 2,
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // unbind. order matters
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //

    // GLint uniID = glGetUniformLocation(shader_program, "scale");

    stbi_set_flip_vertically_on_load(true);
    int width_img, height_img, num_col_ch;
    unsigned char *tex_buf = stbi_load("ring.png", &width_img, &height_img, &num_col_ch, 0);

    GLuint tex;
    glGenTextures(1, &tex);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_img, height_img, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_buf);
    glGenerateMipmap(GL_TEXTURE_2D);
    GLint tex0_loc = glGetUniformLocation(shader_program, "tex0");

    glUseProgram(shader_program);


    stbi_image_free(tex_buf);

    glUniform1i(tex0_loc, 0);

    glClearColor(1, 1, 1, 1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // glUniform1f(uniID, 1.5f);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &tex);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();
}

