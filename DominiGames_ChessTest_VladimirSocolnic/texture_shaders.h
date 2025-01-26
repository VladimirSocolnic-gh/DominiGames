#ifndef TEXTURE_SHADERS_H
#define TEXTURE_SHADERS_H

#include <glad/glad.h>
#include <string>
#include <iostream>

// Вершинный шейдер для работы с текстурами
const char* vertexShaderSourceTexture = R"(
#version 330 core
layout (location = 0) in vec3 aPos;      // Координаты вершины
layout (location = 1) in vec2 aTexCoord; // Текстурные координаты

out vec2 TexCoord; // Передача текстурных координат в фрагментный шейдер

void main()
{
    gl_Position = vec4(aPos, 1.0); // Позиция вершины
    TexCoord = aTexCoord;          // Проброс текстурных координат
}
)";

// Фрагментный шейдер для отображения текстур
const char* fragmentShaderSourceTexture = R"(
#version 330 core
out vec4 FragColor; // Итоговый цвет фрагмента

in vec2 TexCoord; // Получение текстурных координат из вершинного шейдера

uniform sampler2D texture1; // Текстура, используемая для отрисовки

void main()
{
    FragColor = texture(texture1, TexCoord); // Вычисление цвета пикселя из текстуры
}
)";

// Проверка ошибок компиляции и линковки шейдеров
void checkShaderCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "Ошибка компиляции шейдера (" << type << "): " << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "Ошибка линковки программы: " << infoLog << std::endl;
        }
    }
}

// Создание шейдерной программы для работы с текстурами
GLuint createShaderProgramTexture(const char* vertexSource, const char* fragmentSource) {
    // Создание и компиляция вершинного шейдера
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader, "VERTEX");

    // Создание и компиляция фрагментного шейдера
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    // Создание и линковка шейдерной программы
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompileErrors(shaderProgram, "PROGRAM");

    // Удаление шейдеров после линковки, так как они больше не нужны
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

#endif // TEXTURE_SHADERS_H
