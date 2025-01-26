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
// ��������� BFSResult � PotentialMove � ��� � ������
///////////////////////////////////////////////////////////////////
struct BFSResult {
    std::vector<std::pair<int, int>> path;
    int length;
};

struct PotentialMove {
    std::pair<int, int> from;
    std::pair<int, int> to;
    int pathLength;   // ��� BFS
    int improvement;  // ��� fallback
};

///////////////////////////////////////////////////////////////////
// ������� ��� ����������� ���������� ������
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
    return -1; // �� ������� � ������
}

//-----------------------------------------------------------------
// ����� AIHandler
//-----------------------------------------------------------------
class AIHandler {
private:
    std::vector<std::pair<int, int>>& blackPiecePositions;
    std::vector<std::pair<int, int>>& whitePiecePositions;

    // ������ �������� targetCells � ������ � ��� ������ ������������ ������
    std::vector<std::vector<std::pair<int, int>>> priorityGroups;

    int boardSize;

public:
    // �����������
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
    // ������� �������
    //------------------------------------------------------------------
    std::pair<std::pair<int, int>, std::pair<int, int>> calculateMove() {
        // �������� ��� ������������ ����
        std::vector<PotentialMove> allMoves;

        for (auto& piece : blackPiecePositions) {
            // 1) �����, ����� ��������� � ������, �� ������� ����� �����
            int currentPrio = getCellPriority(piece, priorityGroups);

            // 2) ���� currentPrio == 0, ��� �� ������ ����� � ����� �� �����
            if (currentPrio == 0) {
                // ������� ����� �� ����������, ��� ����� �� ���������
                continue;
            }

            // 3) ���� ���� BFS � ����� �������� ���������� (�.�. prio < currentPrio).
            //    ���� currentPrio == -1, ������ ����� ������ �� � ������,
            //    ����� ���� �� prio=0 ������ �� prio=priorityGroups.size()-1.
            bool foundBFS = false;
            PotentialMove bestBFS;
            bestBFS.from = piece;
            bestBFS.to = { -1,-1 };
            bestBFS.pathLength = INT_MAX;
            bestBFS.improvement = -1;

            // ����� ����������, ����� ���������� �� ����� ��������:
            // ���� currentPrio == -1, �� �������� ��� ���������� (0..N-1)
            // ����� � 0..(currentPrio - 1)
            int maxPrioToCheck = (currentPrio == -1)
                ? (int)priorityGroups.size() - 1
                : currentPrio - 1;

            for (int prioToGo = 0; prioToGo <= maxPrioToCheck; ++prioToGo) {
                BFSResult r = findBFSForGroup(piece, priorityGroups[prioToGo]);
                if (!r.path.empty() && r.length < bestBFS.pathLength) {
                    foundBFS = true;
                    bestBFS.to = r.path[1]; // ������ ���
                    bestBFS.pathLength = r.length;
                }
            }

            if (foundBFS) {
                // ��������� � allMoves
                bestBFS.from = piece;
                allMoves.push_back(bestBFS);
            }
            else {
                // BFS �� ������ � fallback (��������� ��� � �������)
                PotentialMove fb = getFallbackMove(piece);
                if (fb.to.first != -1) {
                    allMoves.push_back(fb);
                }
            }
        }

        // ���� ��� �����
        if (allMoves.empty()) {
            return { {-1,-1},{-1,-1} };
        }

        // �������, ���� �� BFS-����
        bool haveBFS = false;
        int minLen = INT_MAX;
        PotentialMove bestBFSmove;

        int bestImprovement = -999999;
        PotentialMove bestFallback;

        for (auto& pm : allMoves) {
            if (pm.pathLength != INT_MAX) {
                // BFS-���
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

        // ��������� ���
        auto it = std::find(blackPiecePositions.begin(), blackPiecePositions.end(), chosen.from);
        if (it != blackPiecePositions.end()) {
            *it = chosen.to;
        }
        return { chosen.from, chosen.to };
    }

private:

    //------------------------------------------------------------------
    // ���� BFS-���� �� start � ����� ������ �� group.
    // ���������� ���������� ��������� ����. ���� ��� ���� � ���������� path.empty().
    //------------------------------------------------------------------
    BFSResult findBFSForGroup(const std::pair<int, int>& start,
        const std::vector<std::pair<int, int>>& group) const
    {
        BFSResult best;
        best.length = INT_MAX;

        for (auto& goal : group) {
            // ���� ����� == goal, ����������, ����� ����� ����� ��������� ������
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
    // ����� ���� BFS (��� ������)
    //------------------------------------------------------------------
    BFSResult findPathBFS(const std::pair<int, int>& start,
        const std::pair<int, int>& goal) const
    {
        // ...
        // (����������� ��� ��� ���������� BFS)
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

        // ��������������� ����
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
    // ��������, �������� �� ������ (r, c), �� ������ ��������� (startCell)
    //------------------------------------------------------------------
    bool isCellFree(int r, int c, const std::pair<int, int>& startCell) const {
        if (r < 0 || r >= boardSize || c < 0 || c >= boardSize) {
            return false;
        }
        // �����
        if (std::find(whitePiecePositions.begin(), whitePiecePositions.end(), std::make_pair(r, c))
            != whitePiecePositions.end())
        {
            return false;
        }
        // ׸���� (����� ���, ������� �������)
        auto it = std::find(blackPiecePositions.begin(), blackPiecePositions.end(), std::make_pair(r, c));
        if (it != blackPiecePositions.end() && std::make_pair(r, c) != startCell) {
            return false;
        }
        return true;
    }

    //------------------------------------------------------------------
    // Fallback-��� �� ������������� ���������, ���� BFS ���� �� ������
    //------------------------------------------------------------------
    PotentialMove getFallbackMove(const std::pair<int, int>& piece) const {
        PotentialMove pm;
        pm.from = piece;
        pm.to = { -1,-1 };
        pm.pathLength = INT_MAX; // �������� "�� BFS"
        pm.improvement = -999999;

        static const std::vector<std::pair<int, int>> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        auto manhattan = [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return std::abs(a.first - b.first) + std::abs(a.second - b.second);
            };

        // ������ ��� ��������� ������������� ����
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

        // �������� ��� � ������������ "����������" ������������ 
        // ���� ������������ �����. ������ ���������� �������� ����� ��������.
        // �� ����� �� ������� �������� ��� cells � priorityGroups.
        // �����, ��� ��������, ������� "����� ��������" = 0, ��� ����� ���.
        for (auto& mv : possibleMoves) {
            int bestLocalImp = -999999;
            // ���������� ��� ������, ��� ������
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