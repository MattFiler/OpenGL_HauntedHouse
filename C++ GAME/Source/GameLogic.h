#include <string>
#include <time.h>

#include "GameDataUtilities.h"
#include "Structs/ParsedInputData.h"

using namespace std;

/*
 * Define logic related to game actions and progression.
 */

class GameLogic : public GameDataUtilities
{
public:
    GameLogic();
    ~GameLogic() = default;

    /* Public input logic trigger */
    void handleInput(string input);

private:
    /* Input logic */
    void parseInput(string input);
    string validateSubject(string input_subject, string command_subject);
    void performAction();

    /* Preset output logic */
    string parseResponse(string response);

    /* Scripted Actions */
    void move(string direction);
    void equip(string item);
    void unequip(string item);
    void open(string object);
    void examine(string object);
    void swing();
    void light(bool on);
    void score();
    void read(string object);
    void say(string word);
    void unlock();
    void use(string item);
    void climb(string object);
    void dig();
    void spray();

    /* Input data */
    ParsedInputData action_data;
};