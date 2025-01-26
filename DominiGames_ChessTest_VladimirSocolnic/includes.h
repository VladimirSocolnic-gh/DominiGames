#ifndef INCLUDES_H
#define INCLUDES_H

// ����������� ����������� ���������, ����������� ��� ������ ���������
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// ����������� OpenGL ��� ������ � ��������
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ���������� stb_image ��� �������� �������
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ���������������� ������������ �����
// ����� ������������ ��� ������, ������� � ���������� ��� �������
#include "ChessBoard.h"				 // ������ ��������� �����
#include "shaders_and_text_render.h" // ������ � ������� � ���������
#include "board_shaders.h"			 // ������� ��� ����������� �����
#include "texture_shaders.h"	     // ������� ��� ������ � ����������
#include "PieceRenderer.h"		     // ��������� �����
#include "MouseHandler.h"			 // ��������� ����� ����
#include "AIHandler.h"			     // ������ ��� AI
#include "Utils.h"					 // ��������������� �������
#include "Initialization.h"		     // ������������� �����������
#include "Helpers.h"			     // ������� ��� ������ � ������ ���������
#include "Constants.h"			     // ���������, ������������ � �������
#include "GameLogic.h"			     // �������� ������� ������
#include "RenderHelpers.h"		     // ��������� ��� ���������

#endif // INCLUDES_H