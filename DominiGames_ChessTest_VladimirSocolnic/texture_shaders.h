#ifndef TEXTURE_SHADERS_H
#define TEXTURE_SHADERS_H

#include <glad/glad.h>
#include <string>
#include <iostream>

// ��������� ������ ��� ������ � ����������
const char* vertexShaderSourceTexture = R"(
#version 330 core
layout (location = 0) in vec3 aPos;      // ���������� �������
layout (location = 1) in vec2 aTexCoord; // ���������� ����������

out vec2 TexCoord; // �������� ���������� ��������� � ����������� ������

void main()
{
    gl_Position = vec4(aPos, 1.0); // ������� �������
    TexCoord = aTexCoord;          // ������� ���������� ���������
}
)";

// ����������� ������ ��� ����������� �������
const char* fragmentShaderSourceTexture = R"(
#version 330 core
out vec4 FragColor; // �������� ���� ���������

in vec2 TexCoord; // ��������� ���������� ��������� �� ���������� �������

uniform sampler2D texture1; // ��������, ������������ ��� ���������

void main()
{
    FragColor = texture(texture1, TexCoord); // ���������� ����� ������� �� ��������
}
)";

// �������� ������ ���������� � �������� ��������
void checkShaderCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "������ ���������� ������� (" << type << "): " << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "������ �������� ���������: " << infoLog << std::endl;
        }
    }
}

// �������� ��������� ��������� ��� ������ � ����������
GLuint createShaderProgramTexture(const char* vertexSource, const char* fragmentSource) {
    // �������� � ���������� ���������� �������
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader, "VERTEX");

    // �������� � ���������� ������������ �������
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    // �������� � �������� ��������� ���������
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompileErrors(shaderProgram, "PROGRAM");

    // �������� �������� ����� ��������, ��� ��� ��� ������ �� �����
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

#endif // TEXTURE_SHADERS_H
