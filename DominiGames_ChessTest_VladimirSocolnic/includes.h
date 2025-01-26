#ifndef INCLUDES_H
#define INCLUDES_H

// Подключение стандартных библиотек, необходимых для работы программы
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// Подключение OpenGL для работы с графикой
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Библиотека stb_image для загрузки текстур
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Пользовательские заголовочные файлы
// Здесь подключаются все модули, которые я реализовал для проекта
#include "ChessBoard.h"				 // Логика шахматной доски
#include "shaders_and_text_render.h" // Работа с текстом и шейдерами
#include "board_shaders.h"			 // Шейдеры для отображения доски
#include "texture_shaders.h"	     // Шейдеры для работы с текстурами
#include "PieceRenderer.h"		     // Отрисовка фигур
#include "MouseHandler.h"			 // Обработка ввода мыши
#include "AIHandler.h"			     // Логика для AI
#include "Utils.h"					 // Вспомогательные функции
#include "Initialization.h"		     // Инициализация компонентов
#include "Helpers.h"			     // Утилиты для помощи в работе программы
#include "Constants.h"			     // Константы, используемые в проекте
#include "GameLogic.h"			     // Основная игровая логика
#include "RenderHelpers.h"		     // Помощники для отрисовки

#endif // INCLUDES_H