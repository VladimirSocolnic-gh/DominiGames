#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <utility>

// ������� ����
const int WINDOW_WIDTH = 800;    // ������ ����
const int WINDOW_HEIGHT = 800;   // ������ ����

// ������ ��������� �����
const int BOARD_SIZE = 8;        // ���������� ������ �� ������� �����

// ��������� ��� ������� ������ (����� � ����� �� ����� �����)
const float FIRST_LETTER_X = 102.0f;          // ��������� X-���������� ��� ����
const float FIRST_LETTER_Y_BOTTOM = 740.0f;   // Y-���������� ��� ������ ����� �����
const float FIRST_LETTER_Y_TOP = 35.0f;       // Y-���������� ��� ������� ����� �����
const float LETTER_SPACING = 81.0f;           // ���������� ����� �������

const float FIRST_NUMBER_Y = 100.0f;          // ��������� Y-���������� ��� ����
const float FIRST_NUMBER_X_LEFT = 35.0f;      // X-���������� ��� ����� �������
const float FIRST_NUMBER_X_RIGHT = 750.0f;    // X-���������� ��� ������ �������
const float NUMBER_SPACING = 81.0f;           // ���������� ����� �������

// ��������� ������� �����
const std::vector<std::pair<int, int>> INITIAL_WHITE_POSITIONS = {
    {5, 0}, {5, 1}, {5, 2},
    {6, 0}, {6, 1}, {6, 2},
    {7, 0}, {7, 1}, {7, 2}
};

const std::vector<std::pair<int, int>> INITIAL_BLACK_POSITIONS = {
    {0, 7}, {0, 6}, {0, 5},
    {1, 7}, {1, 6}, {1, 5},
    {2, 7}, {2, 6}, {2, 5}
};

// ������� ������ ��� ������
const std::vector<std::pair<int, int>> WHITE_TARGET_CELLS = INITIAL_BLACK_POSITIONS; // ����� ������ ������ ������� �� ��������� ������ ������
const std::vector<std::pair<int, int>> BLACK_TARGET_CELLS = INITIAL_WHITE_POSITIONS; // ������ ������ ������ ������� �� ��������� ������ �����

// ������������ ������ ��� AI
const std::vector<std::pair<int, int>> PRIORITY_0 = { {7, 0} };         // ��������� 0: h1
const std::vector<std::pair<int, int>> PRIORITY_1 = { {7, 1}, {6, 0}, {6, 1} }; // ��������� 1: h2, g1, g2
const std::vector<std::pair<int, int>> PRIORITY_2 = {
    {5, 0}, {5, 1}, {5, 2},       // f1, f2, f3
    {6, 2}, {7, 2}                // g3, h3
};

#endif // CONSTANTS_H
