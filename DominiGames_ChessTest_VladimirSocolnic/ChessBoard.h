#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <glad/glad.h>

// Класс для управления шахматной доской
class ChessBoard {
public:
    ChessBoard(int size, GLuint shaderProgram); // Инициализация доски
    ~ChessBoard();                              // Освобождение ресурсов

    void draw() const; // Отрисовка доски

private:
    int boardSize;
    GLuint shaderProgram, VAO, VBO, EBO;

    void initializeQuad(); // Настройка OpenGL-буферов
    void drawQuad(float x, float y, float size, float r, float g, float b) const; // Рисование одной клетки
};

#endif // CHESSBOARD_H
