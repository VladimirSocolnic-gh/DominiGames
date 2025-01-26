#ifndef AI_HANDLER_H
#define AI_HANDLER_H

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "Utils.h"
#include <queue>
#include <climits>

///////////////////////////////////////////////////////////////////
// Структуры BFSResult и PotentialMove — как и раньше
///////////////////////////////////////////////////////////////////
struct BFSResult {
    std::vector<std::pair<int, int>> path;
    int length;
};

struct PotentialMove {
    std::pair<int, int> from;
    std::pair<int, int> to;
    int pathLength;   // Для BFS
    int improvement;  // Для fallback
};

///////////////////////////////////////////////////////////////////
// Функция для определения приоритета клетки
///////////////////////////////////////////////////////////////////
int getCellPriority(const std::pair<int, int>& cell,
    const std::vector<std::vector<std::pair<int, int>>>& priorityGroups)
{
    for (int prio = 0; prio < (int)priorityGroups.size(); ++prio) {
        const auto& group = priorityGroups[prio];
        if (std::find(group.begin(), group.end(), cell) != group.end()) {
            return prio;
        }
    }
    return -1; // не найдено в списке
}

//-----------------------------------------------------------------
// Класс AIHandler
//-----------------------------------------------------------------
class AIHandler {
private:
    std::vector<std::pair<int, int>>& blackPiecePositions;
    std::vector<std::pair<int, int>>& whitePiecePositions;

    // Вместо простого targetCells — теперь у нас группа приоритетных клеток
    std::vector<std::vector<std::pair<int, int>>> priorityGroups;

    int boardSize;

public:
    // Конструктор
    AIHandler(std::vector<std::pair<int, int>>& blackPieces,
        std::vector<std::pair<int, int>>& whitePieces,
        std::vector<std::vector<std::pair<int, int>>> priorityGroups,
        int size)
        : blackPiecePositions(blackPieces)
        , whitePiecePositions(whitePieces)
        , priorityGroups(priorityGroups)
        , boardSize(size)
    {
    }

    //------------------------------------------------------------------
    // Главная функция
    //------------------------------------------------------------------
    std::pair<std::pair<int, int>, std::pair<int, int>> calculateMove() {
        // Собираем все «кандидатные» ходы
        std::vector<PotentialMove> allMoves;

        for (auto& piece : blackPiecePositions) {
            // 1) Узнаём, какой приоритет у клетки, на которой стоит пешка
            int currentPrio = getCellPriority(piece, priorityGroups);

            // 2) Если currentPrio == 0, уже на лучшем месте — стоим на месте
            if (currentPrio == 0) {
                // Никаких ходов не предлагаем, эта пешка не двигается
                continue;
            }

            // 3) Ищем путь BFS к более высокому приоритету (т.е. prio < currentPrio).
            //    Если currentPrio == -1, значит пешка вообще не в списке,
            //    тогда ищем от prio=0 вплоть до prio=priorityGroups.size()-1.
            bool foundBFS = false;
            PotentialMove bestBFS;
            bestBFS.from = piece;
            bestBFS.to = { -1,-1 };
            bestBFS.pathLength = INT_MAX;
            bestBFS.improvement = -1;

            // Здесь определяем, какие приоритеты мы будем пытаться:
            // если currentPrio == -1, то пытаемся все приоритеты (0..N-1)
            // иначе — 0..(currentPrio - 1)
            int maxPrioToCheck = (currentPrio == -1)
                ? (int)priorityGroups.size() - 1
                : currentPrio - 1;

            for (int prioToGo = 0; prioToGo <= maxPrioToCheck; ++prioToGo) {
                BFSResult r = findBFSForGroup(piece, priorityGroups[prioToGo]);
                if (!r.path.empty() && r.length < bestBFS.pathLength) {
                    foundBFS = true;
                    bestBFS.to = r.path[1]; // первый шаг
                    bestBFS.pathLength = r.length;
                }
            }

            if (foundBFS) {
                // Добавляем в allMoves
                bestBFS.from = piece;
                allMoves.push_back(bestBFS);
            }
            else {
                // BFS не найден — fallback (локальный шаг в сторону)
                PotentialMove fb = getFallbackMove(piece);
                if (fb.to.first != -1) {
                    allMoves.push_back(fb);
                }
            }
        }

        // Если нет ходов
        if (allMoves.empty()) {
            return { {-1,-1},{-1,-1} };
        }

        // Смотрим, есть ли BFS-ходы
        bool haveBFS = false;
        int minLen = INT_MAX;
        PotentialMove bestBFSmove;

        int bestImprovement = -999999;
        PotentialMove bestFallback;

        for (auto& pm : allMoves) {
            if (pm.pathLength != INT_MAX) {
                // BFS-ход
                haveBFS = true;
                if (pm.pathLength < minLen) {
                    minLen = pm.pathLength;
                    bestBFSmove = pm;
                }
            }
            else {
                // fallback
                if (pm.improvement > bestImprovement) {
                    bestImprovement = pm.improvement;
                    bestFallback = pm;
                }
            }
        }

        PotentialMove chosen;
        if (haveBFS) {
            chosen = bestBFSmove;
        }
        else {
            chosen = bestFallback;
        }

        if (chosen.from.first == -1) {
            return { {-1,-1},{-1,-1} };
        }

        // Совершаем ход
        auto it = std::find(blackPiecePositions.begin(), blackPiecePositions.end(), chosen.from);
        if (it != blackPiecePositions.end()) {
            *it = chosen.to;
        }
        return { chosen.from, chosen.to };
    }

private:

    //------------------------------------------------------------------
    // Ищем BFS-путь от start к ЛЮБОЙ клетке из group.
    // Возвращаем кратчайший найденный путь. Если нет пути — возвращаем path.empty().
    //------------------------------------------------------------------
    BFSResult findBFSForGroup(const std::pair<int, int>& start,
        const std::vector<std::pair<int, int>>& group) const
    {
        BFSResult best;
        best.length = INT_MAX;

        for (auto& goal : group) {
            // Если старт == goal, пропускаем, чтобы пешка могла двигаться дальше
            if (start == goal) {
                continue;
            }
            BFSResult r = findPathBFS(start, goal);
            if (!r.path.empty() && r.length < best.length) {
                best = r;
            }
        }

        if (best.length == INT_MAX) {
            best.path.clear();
        }
        return best;
    }

    //------------------------------------------------------------------
    // Поиск пути BFS (как раньше)
    //------------------------------------------------------------------
    BFSResult findPathBFS(const std::pair<int, int>& start,
        const std::pair<int, int>& goal) const
    {
        // ...
        // (используйте ваш уже написанный BFS)
        static const std::vector<std::pair<int, int>> directions = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        };

        std::vector<std::vector<bool>> visited(boardSize, std::vector<bool>(boardSize, false));
        std::vector<std::vector<std::pair<int, int>>> parent(boardSize, std::vector<std::pair<int, int>>(boardSize, { -1,-1 }));

        std::queue<std::pair<int, int>> q;
        q.push(start);
        visited[start.first][start.second] = true;

        bool found = false;
        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();
            if (std::make_pair(r, c) == goal) {
                found = true;
                break;
            }
            for (auto& d : directions) {
                int nr = r + d.first;
                int nc = c + d.second;
                if (nr >= 0 && nr < boardSize && nc >= 0 && nc < boardSize) {
                    if (!visited[nr][nc] && isCellFree(nr, nc, start)) {
                        visited[nr][nc] = true;
                        parent[nr][nc] = { r, c };
                        q.push({ nr, nc });
                    }
                }
            }
        }

        // Восстанавливаем путь
        if (!found) {
            return BFSResult{ {}, 0 };
        }

        std::vector<std::pair<int, int>> path;
        int rr = goal.first, cc = goal.second;
        while (!(rr == -1 && cc == -1)) {
            path.push_back({ rr, cc });
            auto p = parent[rr][cc];
            rr = p.first;
            cc = p.second;
        }
        std::reverse(path.begin(), path.end());
        BFSResult res;
        res.path = path;
        res.length = (int)path.size() - 1;
        return res;
    }

    //------------------------------------------------------------------
    // Проверка, свободна ли клетка (r, c), не считая стартовую (startCell)
    //------------------------------------------------------------------
    bool isCellFree(int r, int c, const std::pair<int, int>& startCell) const {
        if (r < 0 || r >= boardSize || c < 0 || c >= boardSize) {
            return false;
        }
        // Белые
        if (std::find(whitePiecePositions.begin(), whitePiecePositions.end(), std::make_pair(r, c))
            != whitePiecePositions.end())
        {
            return false;
        }
        // Чёрные (кроме той, которую двигаем)
        auto it = std::find(blackPiecePositions.begin(), blackPiecePositions.end(), std::make_pair(r, c));
        if (it != blackPiecePositions.end() && std::make_pair(r, c) != startCell) {
            return false;
        }
        return true;
    }

    //------------------------------------------------------------------
    // Fallback-ход по манхэттенской эвристике, если BFS путь не найден
    //------------------------------------------------------------------
    PotentialMove getFallbackMove(const std::pair<int, int>& piece) const {
        PotentialMove pm;
        pm.from = piece;
        pm.to = { -1,-1 };
        pm.pathLength = INT_MAX; // означает "не BFS"
        pm.improvement = -999999;

        static const std::vector<std::pair<int, int>> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        auto manhattan = [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return std::abs(a.first - b.first) + std::abs(a.second - b.second);
            };

        // Соберём все возможные одноклеточные ходы
        std::vector<std::pair<int, int>> possibleMoves;
        for (auto& d : directions) {
            int nr = piece.first + d.first;
            int nc = piece.second + d.second;
            if (isCellFree(nr, nc, piece)) {
                possibleMoves.push_back({ nr, nc });
            }
        }

        if (possibleMoves.empty()) {
            return pm; // (-1,-1), improvement = -999999
        }

        // Выбираем ход с максимальным "улучшением" относительно 
        // всех приоритетных групп. Обычно достаточно смотреть самые глубокие.
        // Но можно по очереди смотреть все cells в priorityGroups.
        // Здесь, для простоты, смотрим "самый глубокий" = 0, или сразу все.
        for (auto& mv : possibleMoves) {
            int bestLocalImp = -999999;
            // перебираем все группы, все клетки
            for (int g = 0; g < (int)priorityGroups.size(); ++g) {
                for (auto& goal : priorityGroups[g]) {
                    int before = manhattan(piece, goal);
                    int after = manhattan(mv, goal);
                    int imp = before - after;
                    if (imp > bestLocalImp) {
                        bestLocalImp = imp;
                    }
                }
            }
            if (bestLocalImp > pm.improvement) {
                pm.improvement = bestLocalImp;
                pm.to = mv;
            }
        }
        return pm;
    }

};

#endif // AI_HANDLER_H