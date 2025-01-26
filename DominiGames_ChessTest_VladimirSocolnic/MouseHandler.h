#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include <vector>
#include <utility>
#include <iostream>
#include <cmath>

// ����� ��� ��������� ����� ����
class MouseHandler {
public:
    // ����������� �������������� ��������� ����� � ������ �� ������� �����
    MouseHandler(float startX, float startY, float cellSize, int boardSize,
        std::vector<std::pair<int, int>>& whitePieces,
        std::vector<std::pair<int, int>>& blackPieces, bool& whiteTurn)
        : startX(startX), startY(startY), cellSize(cellSize), boardSize(boardSize),
        whitePiecePositions(whitePieces), blackPiecePositions(blackPieces),
        whiteTurn(whiteTurn), selectedCell(-1, -1), pieceSelected(false) {
    }

    // ������������ ���� ����
    void handleMouseClick(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            std::pair<int, int> clickedCell = getCellFromMouse(mouseX, mouseY);

            if (clickedCell.first != -1 && clickedCell.second != -1) {
                if (!pieceSelected) {
                    // ����� ������, ���� ��� ���������
                    if (whiteTurn && isPieceAtCell(clickedCell, whitePiecePositions)) {
                        selectedCell = clickedCell;
                        pieceSelected = true;
                        std::cout << "White Pawn from: (" << selectedCell.first << ", " << selectedCell.second << ")" << std::endl;
                    }
                    else if (!whiteTurn && isPieceAtCell(clickedCell, blackPiecePositions)) {
                        selectedCell = clickedCell;
                        pieceSelected = true;
                        std::cout << "Black Pawn from: (" << selectedCell.first << ", " << selectedCell.second << ")" << std::endl;
                    }
                }
                else {
                    // ������� ����������� ������
                    int colDiff = std::abs(clickedCell.first - selectedCell.first);
                    int rowDiff = std::abs(clickedCell.second - selectedCell.second);

                    if ((colDiff + rowDiff == 1) && !isPieceAtCell(clickedCell, whitePiecePositions) && !isPieceAtCell(clickedCell, blackPiecePositions)) {
                        // �������� �����������
                        if (whiteTurn) {
                            movePiece(selectedCell, clickedCell, whitePiecePositions);
                            std::cout << "White Pawn To: (" << clickedCell.first << ", " << clickedCell.second << ")" << std::endl;
                        }
                        else {
                            movePiece(selectedCell, clickedCell, blackPiecePositions);
                            std::cout << "Black Pawn To: (" << clickedCell.first << ", " << clickedCell.second << ")" << std::endl;
                        }
                        whiteTurn = !whiteTurn; // ����� ����
                        selectedCell = { -1, -1 };
                        pieceSelected = false;
                    }
                    else {
                        // �������� ���, ����� ������
                        std::cout << "Invalid Move!" << std::endl;
                        selectedCell = { -1, -1 };
                        pieceSelected = false;
                    }
                }
            }
        }
    }

private:
    float startX, startY, cellSize;
    int boardSize;
    std::vector<std::pair<int, int>>& whitePiecePositions;
    std::vector<std::pair<int, int>>& blackPiecePositions;
    bool& whiteTurn;

    std::pair<int, int> selectedCell; // ������� ��������� ������
    bool pieceSelected; // ���� ������ ������

    // ��������� ������� ������ �� ��������� ������
    bool isPieceAtCell(const std::pair<int, int>& cell, const std::vector<std::pair<int, int>>& piecePositions) const {
        for (const auto& position : piecePositions) {
            if (position == cell) {
                return true;
            }
        }
        return false;
    }

    // ���������� ������ � ����� ������ �� ������
    void movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to, std::vector<std::pair<int, int>>& piecePositions) {
        for (auto& position : piecePositions) {
            if (position == from) {
                position = to;
                break;
            }
        }
    }

    // ����������� ���������� ���� � ������ �����
    std::pair<int, int> getCellFromMouse(double mouseX, double mouseY) const {
        float normX = (mouseX / 400.0f) - 1.0f;
        float normY = 1.0f - (mouseY / 400.0f);

        int col = static_cast<int>((normX - startX) / cellSize);
        int row = static_cast<int>((normY - startY) / cellSize);

        if (col >= 0 && col < boardSize && row >= 0 && row < boardSize) {
            return { col, row };
        }
        return { -1, -1 };
    }
};

#endif // MOUSE_HANDLER_H
