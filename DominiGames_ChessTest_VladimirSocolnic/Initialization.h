#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// ��������� ���������� OpenGL
void setupOpenGL(int width, int height) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, width, height);
}

// ������������� GLFW
bool initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "�� ������� ���������������� GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    return true;
}

// �������� ���� GLFW
GLFWwindow* createWindow(int width, int height, const char* title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "�� ������� ������� ���� GLFW" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    return window;
}

// ������������� GLAD
bool initializeGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "�� ������� ���������������� GLAD" << std::endl;
        return false;
    }
    return true;
}

#endif // INITIALIZATION_H
