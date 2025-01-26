#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <utility>

// ���������� ������ � ����� ������ �� ������.
// from - ��������� ������� ������ (������, �������).
// to - �������� ������� ������ (������, �������).
// piecePositions - ������ ������� ������� �����, ������� ����� �������.

void movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to,
    std::vector<std::pair<int, int>>& piecePositions);

#endif // UTILS_H
