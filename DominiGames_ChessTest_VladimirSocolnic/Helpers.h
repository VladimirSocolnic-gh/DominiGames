#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include <algorithm>

// Проверяет, находятся ли все "targetCells" среди "piecePositions"
bool checkAllPiecesOnTargets(
    const std::vector<std::pair<int, int>>& piecePositions,
    const std::vector<std::pair<int, int>>& targetCells
) {
    for (auto& cell : targetCells) {
        if (std::find(piecePositions.begin(), piecePositions.end(), cell) == piecePositions.end()) {
            return false;
        }
    }
    return true;
}

#endif // HELPERS_H
