#include "../inc/console_view.hpp"

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace game {

namespace {

const char* kAnsiReset = "\033[0m";
const char* kAnsiPlayer = "\033[32m";
const char* kAnsiEnemy = "\033[31m";
const char* kAnsiBonus = "\033[33m";
const char* kAnsiOverlap = "\033[36m";
const char* kAnsiEvent = "\033[35m";

const char* colorForSymbol(char symbol)
{
    switch(symbol)
    {
        case 'P':
            return kAnsiPlayer;
        case 'E':
            return kAnsiEnemy;
        case 'B':
            return kAnsiBonus;
        case '*':
            return kAnsiOverlap;
        default:
            return kAnsiReset;
    }
}

std::vector<EntitySnapshot> collectActiveSnapshots(const TurnResult& result)
{
    std::vector<EntitySnapshot> activeSnapshots;
    for(const EntitySnapshot& snapshot: result.snapshots)
        if(snapshot.active)
            activeSnapshots.push_back(snapshot);

    return activeSnapshots;
}

}  // namespace

std::string renderTurnResult(const TurnResult& result)
{
    std::ostringstream output;
    printTurnResult(output, result);
    return output.str();
}

void printTurnResult(std::ostream& os, const TurnResult& result)
{
    std::vector<EntitySnapshot> activeSnapshots = collectActiveSnapshots(result);

    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;

    if(!activeSnapshots.empty())
    {
        minX = activeSnapshots.front().x;
        maxX = activeSnapshots.front().x;
        minY = activeSnapshots.front().y;
        maxY = activeSnapshots.front().y;

        for(const EntitySnapshot& snapshot: activeSnapshots)
        {
            minX = std::min(minX, snapshot.x);
            maxX = std::max(maxX, snapshot.x);
            minY = std::min(minY, snapshot.y);
            maxY = std::max(maxY, snapshot.y);
        }
    }

    minX -= 1;
    maxX += 1;
    minY -= 1;
    maxY += 1;

    const int width = maxX - minX + 1;
    const int height = maxY - minY + 1;

    std::vector<std::string> board(height, std::string(width, '.'));
    for(const EntitySnapshot& snapshot: activeSnapshots)
    {
        const int row = maxY - snapshot.y;
        const int column = snapshot.x - minX;
        char& cell = board[row][column];
        if(cell == '.')
            cell = snapshot.symbol;
        else
            cell = '*';
    }

    os << "Turn " << result.turnNumber << '\n';
    os << '+' << std::string(width, '-') << "+\n";

    for(const std::string& row: board)
    {
        os << '|';
        for(char cell: row)
        {
            if(cell == '.')
                os << cell;
            else
                os << colorForSymbol(cell) << cell << kAnsiReset;
        }
        os << "|\n";
    }

    os << '+' << std::string(width, '-') << "+\n";
    os << kAnsiEvent << "Event: " << result.collision.message << kAnsiReset
       << '\n';
}

}  // namespace game
