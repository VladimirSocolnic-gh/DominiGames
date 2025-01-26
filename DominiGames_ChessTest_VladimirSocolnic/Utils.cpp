#include "Utils.h"

// ���������� ������ � ����� ������ �� ������
void movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to, std::vector<std::pair<int, int>>& piecePositions) {
    for (auto& position : piecePositions) {
        if (position == from) {
            position = to; // ���������� ������� ������
            break;
        }
    }
}
