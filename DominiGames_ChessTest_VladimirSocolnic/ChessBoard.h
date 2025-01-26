#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <glad/glad.h>

// ����� ��� ���������� ��������� ������
class ChessBoard {
public:
    ChessBoard(int size, GLuint shaderProgram); // ������������� �����
    ~ChessBoard();                              // ������������ ��������

    void draw() const; // ��������� �����

private:
    int boardSize;
    GLuint shaderProgram, VAO, VBO, EBO;

    void initializeQuad(); // ��������� OpenGL-�������
    void drawQuad(float x, float y, float size, float r, float g, float b) const; // ��������� ����� ������
};

#endif // CHESSBOARD_H
