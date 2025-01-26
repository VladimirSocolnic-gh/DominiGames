#ifndef PIECE_RENDERER_H
#define PIECE_RENDERER_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include <utility>

// Класс для отрисовки шахматных фигур
class PieceRenderer {
public:
    // Конструктор принимает путь к текстуре и шейдерную программу
    PieceRenderer(const std::string& texturePath, GLuint shaderProgram);

    // Отрисовывает фигуры на указанных позициях с учетом координат и размера клеток
    void renderPieces(const std::vector<std::pair<int, int>>& positions, float startX, float startY, float cellSize);

private:
    GLuint textureID;      // ID текстуры фигуры
    GLuint shaderProgram;  // Шейдерная программа для визуализации
};

#endif // PIECE_RENDERER_H
