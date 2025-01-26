#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include <utility>
#include <iostream>
#include "Helpers.h"
#include "AIHandler.h"

// Инициализация начальных позиций белых фигур
std::vector<std::pair<int, int>> initializeWhitePieces() {
    return {
        {5, 0}, {5, 1}, {5, 2},
        {6, 0}, {6, 1}, {6, 2},
        {7, 0}, {7, 1}, {7, 2}
    };
}

// Инициализация начальных позиций черных фигур
std::vector<std::pair<int, int>> initializeBlackPieces() {
    return {
        {0, 7}, {0, 6}, {0, 5},
        {1, 7}, {1, 6}, {1, 5},
        {2, 7}, {2, 6}, {2, 5}
    };
}

// Инициализация приоритетных групп для AI
std::vector<std::vector<std::pair<int, int>>> initializePriorityGroups() {
    return {
        {{7, 0}},                                   // Приоритет 0
        {{7, 1}, {6, 0}, {6, 1}},                   // Приоритет 1
        {{5, 0}, {5, 1}, {5, 2}, {6, 2}, {7, 2}}    // Приоритет 2
    };
}

// Проверяет, все ли фигуры находятся на целевых клетках
bool checkVictory(const std::vector<std::pair<int, int>>& pieces, const std::vector<std::pair<int, int>>& targets) {
    return checkAllPiecesOnTargets(pieces, targets);
}

// Выполнение хода AI
void executeAIMove(
    AIHandler& aiHandler,
    std::vector<std::pair<int, int>>& blackPiecePositions,
    std::vector<std::pair<int, int>>& whitePiecePositions,
    bool& whiteTurn,
    bool& gameOver
) {
    // Расчет хода AI
    auto [from, to] = aiHandler.calculateMove();

    if (from.first != -1 && to.first != -1) { // Если ход допустим
        movePiece(from, to, blackPiecePositions);
        whiteTurn = true; // Передача хода белым

        // Проверка победы черных после хода
        if (checkVictory(blackPiecePositions, BLACK_TARGET_CELLS)) {
            std::cout << "BLACK WON\n";
            gameOver = true;
        }

        // Проверка победы белых после хода (если это критично)
        if (checkVictory(whitePiecePositions, WHITE_TARGET_CELLS)) {
            std::cout << "WHITE WON\n";
            gameOver = true;
        }
    }
}

#endif // GAMELOGIC_H
