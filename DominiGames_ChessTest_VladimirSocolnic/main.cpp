#include "includes.h"

// Глобальный указатель для обработки кликов мыши
MouseHandler* g_mouseHandler = nullptr;

// Колбэк обработки кликов мыши
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (g_mouseHandler) {
        g_mouseHandler->handleMouseClick(window, button, action, mods);
    }
}

int main() {
    // Инициализация GLFW и GLAD
    if (!initializeGLFW()) {
        return -1;
    }

    GLFWwindow* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chess Board");
    if (!window) {
        return -1;
    }

    if (!initializeGLAD()) {
        return -1;
    }

    // Настройка OpenGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Создание шейдерных программ
    GLuint boardShaderProgram = createBoardShaderProgram();
    GLuint textShaderProgram = createShaderProgramText(vertexShaderSourceText, fragmentShaderSourceText);
    GLuint textureShaderProgram = createShaderProgramTexture(vertexShaderSourceTexture, fragmentShaderSourceTexture);

    // Инициализация объектов игры
    ChessBoard chessBoard(BOARD_SIZE, boardShaderProgram);
    TextRenderer textRenderer("fonts/Arial.ttf", WINDOW_WIDTH, WINDOW_HEIGHT);
    textRenderer.setShaderProgram(textShaderProgram);

    PieceRenderer whitePawnRenderer("assets/images/whitePawn.png", textureShaderProgram);   
    PieceRenderer blackPawnRenderer("assets/images/blackPawn.png", textureShaderProgram);

    /* This code is released as public domain or under a permissive license.
    * You are free to use, modify, and distribute it without restrictions. * No copyright claimed.*/
    
    // Расстановка фигур
    auto whitePiecePositions = initializeWhitePieces();
    auto blackPiecePositions = initializeBlackPieces();
    auto priorityGroups = initializePriorityGroups();

    // Настройка обработчиков 
    bool whiteTurn = true;
    MouseHandler mouseHandler(-0.8f, -0.8f, 1.6f / BOARD_SIZE, BOARD_SIZE,
        whitePiecePositions, blackPiecePositions, whiteTurn);
    g_mouseHandler = &mouseHandler;
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    AIHandler aiHandler(blackPiecePositions, whitePiecePositions, priorityGroups, BOARD_SIZE);

    // Игровой цикл
    bool gameOver = false;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка элементов Доски
        renderBoard(chessBoard, textRenderer, whitePiecePositions, blackPiecePositions,
            whitePawnRenderer, blackPawnRenderer);

        // Обработка хода AI & Определение Победы
        if (!gameOver && !whiteTurn) {
            executeAIMove(aiHandler, blackPiecePositions, whitePiecePositions, whiteTurn, gameOver);
        }

        glfwSwapBuffers(window);
    }

    // Очистка ресурсов
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
