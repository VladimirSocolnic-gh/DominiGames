#ifndef SHADERS_AND_TEXT_RENDER_H
#define SHADERS_AND_TEXT_RENDER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

// Вершинный шейдер для рендеринга текста
const char* vertexShaderSourceText = R"(
#version 330 core
layout(location = 0) in vec4 vertex; // Позиция и текстурные координаты

uniform mat4 projection; // Матрица проекции
out vec2 TexCoords; // Текстурные координаты для передачи во фрагментный шейдер

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0); // Позиция в экранных координатах
    TexCoords = vertex.zw; // Текстурные координаты
}
)";

// Фрагментный шейдер для рендеринга текста
const char* fragmentShaderSourceText = R"(
#version 330 core
in vec2 TexCoords; // Текстурные координаты из вершинного шейдера
out vec4 color; // Финальный цвет пикселя

uniform sampler2D text; // Текстура символов
uniform vec3 textColor; // Цвет текста

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r); // Альфа из текстуры
    color = vec4(textColor, 1.0) * sampled; // Цвет текста с учетом альфа-значения
}
)";

// Проверка компиляции и линковки шейдеров
void checkShaderCompileText(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Ошибка компиляции шейдера " << type << ":\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Ошибка линковки программы:\n" << infoLog << std::endl;
        }
    }
}

// Создание шейдерной программы для текста
GLuint createShaderProgramText(const char* vertexSource, const char* fragmentSource) {
    // Компиляция вершинного шейдера
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileText(vertexShader, "VERTEX");

    // Компиляция фрагментного шейдера
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileText(fragmentShader, "FRAGMENT");

    // Линковка шейдерной программы
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompileText(shaderProgram, "PROGRAM");

    // Очистка ресурсов
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Класс для рендеринга текста
class TextRenderer {
public:
    // Конструктор класса
    TextRenderer(const char* fontPath, int width, int height) {
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "Ошибка: Не удалось инициализировать FreeType" << std::endl;
            exit(-1);
        }

        if (FT_New_Face(ft, fontPath, 0, &face)) {
            std::cerr << "Ошибка: Не удалось загрузить шрифт из пути: " << fontPath << std::endl;
            exit(-1);
        }

        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &texture);

        // Настройка ортографической проекции
        projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Деструктор класса
    ~TextRenderer() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // Метод рендеринга текста
    void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color) {
        glUseProgram(shaderProgram);
        glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        for (const char& c : text) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "Ошибка: Не удалось загрузить символ " << c << std::endl;
                continue;
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            float xpos = x + face->glyph->bitmap_left * scale;
            float ypos = y - (face->glyph->bitmap.rows - face->glyph->bitmap_top) * scale;

            float w = face->glyph->bitmap.width * scale;
            float h = face->glyph->bitmap.rows * scale;

            float vertices[6][4] = {
                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos, ypos, 0.0f, 1.0f },
                { xpos + w, ypos, 1.0f, 1.0f },

                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos + w, ypos, 1.0f, 1.0f },
                { xpos + w, ypos + h, 1.0f, 0.0f }
            };

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (face->glyph->advance.x >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Установка шейдерной программы
    void setShaderProgram(GLuint program) {
        shaderProgram = program;
    }

private:
    FT_Library ft;
    FT_Face face;
    GLuint texture;
    GLuint VAO, VBO;
    glm::mat4 projection;
    GLuint shaderProgram;
};

#endif // SHADERS_AND_TEXT_RENDER_H
