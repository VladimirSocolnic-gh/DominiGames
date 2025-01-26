#ifndef PIECE_RENDERER_H
#define PIECE_RENDERER_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include <utility>

// ����� ��� ��������� ��������� �����
class PieceRenderer {
public:
    // ����������� ��������� ���� � �������� � ��������� ���������
    PieceRenderer(const std::string& texturePath, GLuint shaderProgram);

    // ������������ ������ �� ��������� �������� � ������ ��������� � ������� ������
    void renderPieces(const std::vector<std::pair<int, int>>& positions, float startX, float startY, float cellSize);

private:
    GLuint textureID;      // ID �������� ������
    GLuint shaderProgram;  // ��������� ��������� ��� ������������
};

#endif // PIECE_RENDERER_H
