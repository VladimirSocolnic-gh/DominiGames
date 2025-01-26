#ifndef BOARD_SHADERS_H
#define BOARD_SHADERS_H

#include <glad/glad.h>
#include <iostream>

// ��������� ������ ��� ��������� �����
const char* vertexShaderSourceBoard = R"(
#version 330 core
layout(location = 0) in vec2 aPos;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

// ����������� ������ ��� ������� ����� ������ �����
const char* fragmentShaderSourceBoard = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
}
)";

// �������� ���������� �������
void checkShaderCompileBoard(GLuint shader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "������ ���������� �������:\n" << infoLog << std::endl;
    }
}

// �������� � �������� ��������� ��������� ��� �����
GLuint createBoardShaderProgram() {
    // ���������� ���������� �������
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceBoard, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileBoard(vertexShader);

    // ���������� ������������ �������
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceBoard, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileBoard(fragmentShader);

    // �������� ��������� ���������
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // �������� �������� ��������� ���������
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "������ �������� ��������� ���������:\n" << infoLog << std::endl;
    }

    // ������� ��������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

#endif // BOARD_SHADERS_H
