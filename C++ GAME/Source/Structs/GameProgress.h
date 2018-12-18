#include <string>
using namespace std;

/*
 * Structure to store game progression data.
 */

struct GameProgress
{
public:
    /* Map position */
    int getCurrentTile() { return current_tile_index; }
    void setCurrentTile(int index) { current_tile_index = index; }

    /* Position memory */
    void cacheMovement() { previous_tile_index = current_tile_index; }
    void revertMovement() { current_tile_index = previous_tile_index; }

    /* Score */
    int getScore() { return score; }
    void increaseScore(int by) { score += by; };
    void decreaseScore(int by) { score -= by; };

    /* Candle life */
    int getCandleLife() { return candle_life; }
    void decreaseCandleLife() { candle_life -= 1; }

private:
    enum rooms { STARTING_ROOM = 57 };

    int current_tile_index = rooms::STARTING_ROOM;
    int previous_tile_index = rooms::STARTING_ROOM;

    int score = 0;
    int candle_life = 60;
};