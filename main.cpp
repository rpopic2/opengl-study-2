#include <fstream>
#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

std::string read_source(const std::string &filename) {
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

    auto *window =glfwCreateWindow(800, 400, "Hello World", NULL, NULL);
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

    auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    source = read_source("shader.frag");
    psource = source.c_str();
    glShaderSource(frag_shader, 1, &psource, NULL);
    glCompileShader(frag_shader);

    auto shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glViewport(0, 0, 800, 400);

    GLfloat positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // order matters: 1st
    glBindVertexArray(0); // 2nd

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(0, &vbo);

    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();
}

