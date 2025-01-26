#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include <utility>
#include <iostream>
#include "Helpers.h"
#include "AIHandler.h"

// ������������� ��������� ������� ����� �����
std::vector<std::pair<int, int>> initializeWhitePieces() {
    return {
        {5, 0}, {5, 1}, {5, 2},
        {6, 0}, {6, 1}, {6, 2},
        {7, 0}, {7, 1}, {7, 2}
    };
}

// ������������� ��������� ������� ������ �����
std::vector<std::pair<int, int>> initializeBlackPieces() {
    return {
        {0, 7}, {0, 6}, {0, 5},
        {1, 7}, {1, 6}, {1, 5},
        {2, 7}, {2, 6}, {2, 5}
    };
}

// ������������� ������������ ����� ��� AI
std::vector<std::vector<std::pair<int, int>>> initializePriorityGroups() {
    return {
        {{7, 0}},                                   // ��������� 0
        {{7, 1}, {6, 0}, {6, 1}},                   // ��������� 1
        {{5, 0}, {5, 1}, {5, 2}, {6, 2}, {7, 2}}    // ��������� 2
    };
}

// ���������, ��� �� ������ ��������� �� ������� �������
bool checkVictory(const std::vector<std::pair<int, int>>& pieces, const std::vector<std::pair<int, int>>& targets) {
    return checkAllPiecesOnTargets(pieces, targets);
}

// ���������� ���� AI
void executeAIMove(
    AIHandler& aiHandler,
    std::vector<std::pair<int, int>>& blackPiecePositions,
    std::vector<std::pair<int, int>>& whitePiecePositions,
    bool& whiteTurn,
    bool& gameOver
) {
    // ������ ���� AI
    auto [from, to] = aiHandler.calculateMove();

    if (from.first != -1 && to.first != -1) { // ���� ��� ��������
        movePiece(from, to, blackPiecePositions);
        whiteTurn = true; // �������� ���� �����

        // �������� ������ ������ ����� ����
        if (checkVictory(blackPiecePositions, BLACK_TARGET_CELLS)) {
            std::cout << "BLACK WON\n";
            gameOver = true;
        }

        // �������� ������ ����� ����� ���� (���� ��� ��������)
        if (checkVictory(whitePiecePositions, WHITE_TARGET_CELLS)) {
            std::cout << "WHITE WON\n";
            gameOver = true;
        }
    }
}

#endif // GAMELOGIC_H
