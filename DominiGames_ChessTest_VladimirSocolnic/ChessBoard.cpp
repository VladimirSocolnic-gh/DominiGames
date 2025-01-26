#include "ChessBoard.h"
#include <glad/glad.h>

// Конструктор: инициализация доски
ChessBoard::ChessBoard(int size, GLuint shaderProgram) : boardSize(size), shaderProgram(shaderProgram) {
    initializeQuad();
}

// Деструктор: освобождение OpenGL ресурсов
ChessBoard::~ChessBoard() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Инициализация OpenGL буферов для клетки доски
void ChessBoard::initializeQuad() {
    float vertices[8] = { 0 };
    GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// Рисование клетки заданного цвета
void ChessBoard::drawQuad(float x, float y, float size, float r, float g, float b) const {
    float vertices[] = {
        x, y,
        x + size, y,
        x + size, y + size,
        x, y + size
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), r, g, b);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Рисование всей доски
void ChessBoard::draw() const {
    float cellSize = 1.6f / boardSize; // Размер клетки
    float startX = -0.8f;              // Левый верхний угол доски
    float startY = -0.8f;

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            float x = startX + col * cellSize;
            float y = startY + row * cellSize;

            // Чередование цветов клеток
            if ((row + col) % 2 == 0) {
                drawQuad(x, y, cellSize, 0.0f, 0.0f, 0.0f); // Черная клетка
            }
            else {
                drawQuad(x, y, cellSize, 1.0f, 1.0f, 1.0f); // Белая клетка
            }
        }
    }
}
