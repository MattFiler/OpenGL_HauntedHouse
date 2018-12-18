#include "../Libs/nlohmann/json.hpp"
#include <string>
#include <sstream>
#include <Engine/FileIO.h>

#include "Structs/GlobalDynamicText.h"
#include "Structs/GameProgress.h"

using namespace std;
using json = nlohmann::json;

/*
 * Define utilities for accessing and manipulating our game data.
 */

class GameDataUtilities
{
public:
    GameDataUtilities();
    ~GameDataUtilities() = default;

protected:
    /* Object interaction */
    GameProgress& progression();
    int sizeOfCategory(string category);
    json& getCategory(string category);

    /* Map definitions */
    int totalMapTiles();
    int mapTilesPerRow();

    /* Map tile utilities */
    void enableFlagOnTile(string flag);
    void disableFlagOnTile(string flag);
    bool flagIsActiveOnTile(string flag);
    bool itemIsOnTile(string item);
    bool paramIsOnTile(string param);
    bool movementIsValidHere(char identifier);
    void setCurrentTileName(string name);
    void setCurrentTileExits(string exits);

    /* Item utilities */
    void enableFlagOnItem(string flag, string item);
    void disableFlagOnItem(string flag, string item);
    bool flagIsActiveOnItem(string flag, string item);
    bool itemIsVisible(string item);
    void makeItemVisible(string item);
    void makeItemInvisible(string item);
    bool itemIsInInventory(string item);
    void equipItem(string item);
    void unequipItem(string item);
    int getItemID(string item);

    /* Game utilities */
    void refreshText(string response_text, bool game_over = false);

private:
    GameProgress game_progress;
    GlobalDynamicText output_data;
    json game_data;
};