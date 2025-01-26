#ifndef RENDERHELPERS_H
#define RENDERHELPERS_H

#include "ChessBoard.h"
#include "shaders_and_text_render.h"
#include "PieceRenderer.h"
#include "Constants.h" // Для использования BOARD_SIZE, FIRST_LETTER_X и других констант

// Функция для отрисовки букв (A-H) на шахматной доске
void renderLetters(TextRenderer& textRenderer, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        float textX = FIRST_LETTER_X + i * LETTER_SPACING;
        textRenderer.renderText(
            std::string(1, 'A' + i),
            textX,
            FIRST_LETTER_Y_BOTTOM,
            0.8f,
            { 1.0f, 1.0f, 1.0f }
        );
        textRenderer.renderText(
            std::string(1, 'A' + i),
            textX,
            FIRST_LETTER_Y_TOP,
            0.8f,
            { 1.0f, 1.0f, 1.0f }
        );
    }
}

// Функция для отрисовки цифр (1-8) на шахматной доске
void renderNumbers(TextRenderer& textRenderer, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        float textY = FIRST_NUMBER_Y + i * NUMBER_SPACING;
        textRenderer.renderText(
            std::to_string(i + 1),
            FIRST_NUMBER_X_LEFT,
            textY,
            0.8f,
            { 1.0f, 1.0f, 1.0f }
        );
        textRenderer.renderText(
            std::to_string(i + 1),
            FIRST_NUMBER_X_RIGHT,
            textY,
            0.8f,
            { 1.0f, 1.0f, 1.0f }
        );
    }
}

// Функция для отрисовки всех фигур на доске
void renderPieces(
    const std::vector<std::pair<int, int>>& whitePieces,
    const std::vector<std::pair<int, int>>& blackPieces,
    PieceRenderer& whitePawnRenderer,
    PieceRenderer& blackPawnRenderer,
    int boardSize
) {
    // Отрисовка белых фигур
    for (const auto& piece : whitePieces) {
        whitePawnRenderer.renderPieces({ piece }, -0.8f, -0.8f, 1.6f / boardSize);
    }

    // Отрисовка чёрных фигур
    for (const auto& piece : blackPieces) {
        blackPawnRenderer.renderPieces({ piece }, -0.8f, -0.8f, 1.6f / boardSize);
    }
}

// Основная функция для отрисовки всей сцены
void renderBoard(
    ChessBoard& chessBoard,
    TextRenderer& textRenderer,
    const std::vector<std::pair<int, int>>& whitePieces,
    const std::vector<std::pair<int, int>>& blackPieces,
    PieceRenderer& whitePawnRenderer,
    PieceRenderer& blackPawnRenderer
) {
    // Отрисовка шахматной доски
    chessBoard.draw();

    // Отрисовка координат (буквы и цифры)
    renderLetters(textRenderer, BOARD_SIZE);
    renderNumbers(textRenderer, BOARD_SIZE);

    // Отрисовка фигур
    renderPieces(whitePieces, blackPieces, whitePawnRenderer, blackPawnRenderer, BOARD_SIZE);
}

#endif // RENDERHELPERS_H
