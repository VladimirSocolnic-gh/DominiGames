#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <utility>

// Размеры окна
const int WINDOW_WIDTH = 800;    // Ширина окна
const int WINDOW_HEIGHT = 800;   // Высота окна

// Размер шахматной доски
const int BOARD_SIZE = 8;        // Количество клеток на стороне доски

// Параметры для рендера текста (буквы и цифры на полях доски)
const float FIRST_LETTER_X = 102.0f;          // Начальная X-координата для букв
const float FIRST_LETTER_Y_BOTTOM = 740.0f;   // Y-координата для нижней части доски
const float FIRST_LETTER_Y_TOP = 35.0f;       // Y-координата для верхней части доски
const float LETTER_SPACING = 81.0f;           // Расстояние между буквами

const float FIRST_NUMBER_Y = 100.0f;          // Начальная Y-координата для цифр
const float FIRST_NUMBER_X_LEFT = 35.0f;      // X-координата для левой стороны
const float FIRST_NUMBER_X_RIGHT = 750.0f;    // X-координата для правой стороны
const float NUMBER_SPACING = 81.0f;           // Расстояние между цифрами

// Начальные позиции фигур
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

// Целевые клетки для победы
const std::vector<std::pair<int, int>> WHITE_TARGET_CELLS = INITIAL_BLACK_POSITIONS; // Белые фигуры должны попасть на стартовые клетки черных
const std::vector<std::pair<int, int>> BLACK_TARGET_CELLS = INITIAL_WHITE_POSITIONS; // Черные фигуры должны попасть на стартовые клетки белых

// Приоритетные группы для AI
const std::vector<std::pair<int, int>> PRIORITY_0 = { {7, 0} };         // Приоритет 0: h1
const std::vector<std::pair<int, int>> PRIORITY_1 = { {7, 1}, {6, 0}, {6, 1} }; // Приоритет 1: h2, g1, g2
const std::vector<std::pair<int, int>> PRIORITY_2 = {
    {5, 0}, {5, 1}, {5, 2},       // f1, f2, f3
    {6, 2}, {7, 2}                // g3, h3
};

#endif // CONSTANTS_H
