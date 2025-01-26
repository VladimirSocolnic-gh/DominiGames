#include "PieceRenderer.h"
#include <stb_image.h>
#include <iostream>

// �����������: �������� �������� � ��������� OpenGL
PieceRenderer::PieceRenderer(const std::string& texturePath, GLuint shaderProgram)
    : shaderProgram(shaderProgram) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // �������� �������� � �������� ��������
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "�� ������� ��������� ��������: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}

// ��������� ����� �� �����
void PieceRenderer::renderPieces(const std::vector<std::pair<int, int>>& positions, float startX, float startY, float cellSize) {
    for (const auto& position : positions) {
        int col = position.first;
        int row = position.second;

        // ���������� ������ ������ ��� ���������� ������
        float pieceCenterX = startX + col * cellSize + cellSize / 2.0f;
        float pieceCenterY = startY + row * cellSize + cellSize / 2.0f;

        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // ������� ��� ��������� ������
        float vertices[] = {
            pieceCenterX - 0.05f, pieceCenterY - 0.05f, 0.0f, 0.0f, 1.0f, // ����� ������ ����
            pieceCenterX + 0.05f, pieceCenterY - 0.05f, 0.0f, 1.0f, 1.0f, // ������ ������ ����
            pieceCenterX + 0.05f, pieceCenterY + 0.05f, 0.0f, 1.0f, 0.0f, // ������ ������� ����
            pieceCenterX - 0.05f, pieceCenterY + 0.05f, 0.0f, 0.0f, 0.0f  // ����� ������� ����
        };

        // ������� ��� ��������� �������������
        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        // �������� VAO, VBO � EBO ��� ������� ������
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // ��������� ��������� ������
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // ��������� ������
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // ������� �������� ���������
        glBindVertexArray(0);

        // �������� VAO, VBO � EBO ��� �������������� ������ ������
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}
