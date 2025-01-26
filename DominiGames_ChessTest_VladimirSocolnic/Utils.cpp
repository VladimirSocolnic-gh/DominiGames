#include "Utils.h"

// Перемещает фигуру с одной клетки на другую
void movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to, std::vector<std::pair<int, int>>& piecePositions) {
    for (auto& position : piecePositions) {
        if (position == from) {
            position = to; // Обновление позиции фигуры
            break;
        }
    }
}
