#include "GameDataUtilities.h"

/* PUBLIC */

/* Load the game logic */
GameDataUtilities::GameDataUtilities()
{
    auto file = ASGE::FILEIO::File();
    file.open("/data/logic.json");
    auto buffer = file.read();
    stringstream(string(buffer.as_char(), buffer.length)) >> game_data;

    refreshText("OK");
}


/* PROTECTED */


/* Object interaction */

/* Return game progress data for access/modification */
GameProgress& GameDataUtilities::progression()
{
    return game_progress;
}

/* Return the size of a json category */
int GameDataUtilities::sizeOfCategory(string category)
{
    return game_data[category].size();
}

/* Return the category for access/modification */
json& GameDataUtilities::getCategory(string category)
{
    return game_data[category];
}


/* Map definitions */

/* Return the total number of map tiles */
int GameDataUtilities::totalMapTiles()
{
    return game_data["tiles"].size();
}

/* Return the tiles per row on map */
int GameDataUtilities::mapTilesPerRow()
{
    return sqrt(totalMapTiles());
}


/* Map tile utilities */

/* Enable specified flag on current tile */
void GameDataUtilities::enableFlagOnTile(string flag)
{
    json &flags = game_data["tiles"][game_progress.getCurrentTile()]["flags"];
    for (int i=0; i<flags.size(); i++)
    {
        if (flags[i]["name"] == flag)
        {
            flags[i]["value"] = "true";
        }
    }
}

/* Disable specified flag on current tile */
void GameDataUtilities::disableFlagOnTile(string flag)
{
    json &flags = game_data["tiles"][game_progress.getCurrentTile()]["flags"];
    for (int i=0; i<flags.size(); i++)
    {
        if (flags[i]["name"] == flag)
        {
            flags[i]["value"] = "false";
        }
    }
}

/* Check for flag value on current tile */
bool GameDataUtilities::flagIsActiveOnTile(string flag)
{
    auto tile_flags = game_data["tiles"][game_progress.getCurrentTile()]["flags"];
    if (tile_flags.size() > 0)
    {
        for (int i=0; i<tile_flags.size(); i++)
        {
            if (tile_flags[i]["name"] == flag &&
                tile_flags[i]["value"] == "true")
            {
                return true;
            }
        }
    }
    return false;
}

/* Check requested item is on current game tile */
bool GameDataUtilities::itemIsOnTile(string item)
{
    return (game_data["items"][getItemID(item)]["tile"] == to_string(game_progress.getCurrentTile()));
}

/* Check if tile has requested parameter */
bool GameDataUtilities::paramIsOnTile(string param)
{
    auto tile_params = game_data["tiles"][game_progress.getCurrentTile()]["params"];
    if (tile_params.size() > 0)
    {
        for (int i=0; i<tile_params.size(); i++)
        {
            if (param == tile_params[i])
            {
                return true;
            }
        }
    }
    return false;
}

/* Check movement is valid in the current block */
bool GameDataUtilities::movementIsValidHere(char identifier)
{
    string location_exits = game_data["tiles"][game_progress.getCurrentTile()]["exits"];
    for (int i=0; i<location_exits.length(); i++)
    {
        if (location_exits[i] == identifier)
        {
            return true;
        }
    }
    return false;
}

/* Change tile title */
void GameDataUtilities::setCurrentTileName(string name)
{
    game_data["tiles"][game_progress.getCurrentTile()]["name"] = name;
}

/* Change tile exits */
void GameDataUtilities::setCurrentTileExits(string exits)
{
    game_data["tiles"][game_progress.getCurrentTile()]["exits"] = exits;
}


/* Item utilities */

/* Enable flag on specified item */
void GameDataUtilities::enableFlagOnItem(string flag, string item)
{
    json &flags = game_data["items"][getItemID(item)]["flags"];
    for (int i=0; i<flags.size(); i++)
    {
        if (flags[i]["name"] == flag)
        {
            flags[i]["value"] = "true";
        }
    }
}

/* Disable flag on specified item */
void GameDataUtilities::disableFlagOnItem(string flag, string item)
{
    json &flags = game_data["items"][getItemID(item)]["flags"];
    for (int i=0; i<flags.size(); i++)
    {
        if (flags[i]["name"] == flag)
        {
            flags[i]["value"] = "false";
        }
    }
}

/* Check for flag value on specified item */
bool GameDataUtilities::flagIsActiveOnItem(string flag, string item)
{
    auto item_flags = game_data["items"][getItemID(item)]["flags"];
    if (item_flags.size() > 0)
    {
        for (int i=0; i<item_flags.size(); i++)
        {
            if (item_flags[i]["name"] == flag &&
                item_flags[i]["value"] == "true")
            {
                return true;
            }
        }
    }
    return false;
}

/* Check if requested item is visible */
bool GameDataUtilities::itemIsVisible(string item)
{
    return (game_data["items"][getItemID(item)]["visible"] == "true");
}

/* Make item visible */
void GameDataUtilities::makeItemVisible(string item)
{
    game_data["items"][getItemID(item)]["visible"] = "true";
}

/* Make item invisible */
void GameDataUtilities::makeItemInvisible(string item)
{
    game_data["items"][getItemID(item)]["visible"] = "false";
}

/* Check if requested item is in inventory */
bool GameDataUtilities::itemIsInInventory(string item)
{
    return (game_data["items"][getItemID(item)]["equipped"] == "true");
}

/* Set item's equip flag to equipped */
void GameDataUtilities::equipItem(string item)
{
    game_data["items"][getItemID(item)]["equipped"] = "true";
}

/* Set item's equip flag to unequipped */
void GameDataUtilities::unequipItem(string item)
{
    game_data["items"][getItemID(item)]["equipped"] = "false";
}

/* Get the ID of the item by name */
int GameDataUtilities::getItemID(string item)
{
    for (int i=0; i<game_data["items"].size(); i++)
    {
        if (game_data["items"][i]["name"] == item)
        {
            return i;
        }
    }
    return 99; //we should never reach here due to prior logic where this is called
}


/* Game utilities */

/* Refresh the text shown on screen */
void GameDataUtilities::refreshText(string response_text, bool game_over)
{
    //Work out items in location to display
    string items = "";
    for (int i=0; i<game_data["items"].size(); i++)
    {
        auto item = game_data["items"][i];
        if (item["tile"] == to_string(game_progress.getCurrentTile()) &&
            item["equipped"] == "false" &&
            item["visible"] != "false")
        {
            items += "YOU CAN SEE "+(string)item["name"]+" HERE\n";
        }
    }

    //If game is over, append message to response
    if (game_over)
    {
        response_text += "\nWELL DONE! YOU FINISHED THE GAME";
    }

    //Update text
    output_data.setLocationName(game_data["tiles"][game_progress.getCurrentTile()]["name"]);
    output_data.setLocationExits(game_data["tiles"][game_progress.getCurrentTile()]["exits"]);
    output_data.setSystemResponse(response_text);
    output_data.setLocationItems(items);
}