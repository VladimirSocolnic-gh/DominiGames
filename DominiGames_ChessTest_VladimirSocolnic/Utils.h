#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <utility>

// Перемещает фигуру с одной клетки на другую.
// from - начальная позиция фигуры (строка, столбец).
// to - конечная позиция фигуры (строка, столбец).
// piecePositions - список текущих позиций фигур, который будет обновлён.

void movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to,
    std::vector<std::pair<int, int>>& piecePositions);

#endif // UTILS_H
