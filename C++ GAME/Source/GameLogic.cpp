#include "GameLogic.h"

/* PUBLIC */

GameLogic::GameLogic()
{
    srand(time(NULL));
}

/* Handle the user's input */
void GameLogic::handleInput(string input)
{
    parseInput(input);
    if (action_data.getSubjectValidity()) {
        performAction();
    }
    else
    {
        refreshText("YOU DON'T MAKE SENSE");
    }
}


/* PRIVATE */


/* Input logic */

/* Interpret the action and subject from input */
void GameLogic::parseInput(string input)
{
    action_data.clearAll();
    string this_action = "";
    string return_action = "";
    int return_action_index = 0;
    for (int i=0; i<sizeOfCategory("inputs");i++)
    {
        this_action = getCategory("inputs")[i]["command"];
        if (input.substr(0,this_action.length()) == this_action &&
            return_action.length() < this_action.length())
        {
            if (this_action == "N" ||
                this_action == "E" ||
                this_action == "S" ||
                this_action == "W" ||
                this_action == "U" ||
                this_action == "D")
            {
                if (input == this_action)
                {
                    //Make the single letter commands stricter
                    return_action = this_action;
                    return_action_index = i;
                    action_data.setSubject("");
                    action_data.setSubjectValidity(true);
                }
            }
            else
            {
                auto thisSubject = getCategory("inputs")[i]["subject"];
                if (thisSubject.size() == 0)
                {
                    //No subject required
                    action_data.setSubject("");
                    action_data.setSubjectValidity(true);
                }
                else
                {
                    if (input.length() > this_action.length()+1)
                    {
                        //Subject provided, check it is valid
                        for (int x=0; x<thisSubject.size(); x++)
                        {
                            action_data.setSubject(validateSubject(input.substr(this_action.length()+1), thisSubject[x]));
                            if (action_data.getSubject() == "")
                            {
                                action_data.setSubjectValidity(false);
                            }
                            else
                            {
                                action_data.setSubjectValidity(true);
                                break;
                            }
                        }
                    }
                    else
                    {
                        //No subject provided, but it is required
                        action_data.setSubject("");
                        action_data.setSubjectValidity(false);
                    }
                }
                return_action = this_action;
                return_action_index = i;
            }
        }
    }
    action_data.setActionID(return_action_index);
}

/* Validate input subject against the command's required subjects */
string GameLogic::validateSubject(string input_subject, string command_subject)
{
    if (command_subject.substr(0,1) == "{")
    {
        command_subject = command_subject.substr(1,command_subject.length()-2);
        auto subject_array = getCategory(command_subject);
        for (int i=0; i<sizeOfCategory(command_subject); i++)
        {
            //Check for a subject match within the referenced data structure
            if (subject_array[i]["name"] == input_subject)
            {
                return input_subject;
            }
        }
        //No match against subject's specified structure
        return "";
    }
    else if (command_subject.find(':') != string::npos)
    {
        //Subject is an exact reference in a data structure - parse the reference
        string subject_array = "";
        string subject_index = "";
        bool updating_index = false;
        for (int i=0; i<command_subject.length(); i++)
        {
            if (!updating_index)
            {
                if (command_subject[i] == ':')
                {
                    updating_index = true;
                    continue;
                }
                else
                {
                    subject_array += command_subject[i];
                }
            }
            else
            {
                subject_index += command_subject[i];
            }
        }
        //Find the actual reference
        string final_subject = getCategory(subject_array)[stoi(subject_index)]["name"];
        //Check the reference against our input
        if (final_subject == input_subject)
        {
            return input_subject;
        }
        else
        {
            return "";
        }
    }
    else if (command_subject == "*")
    {
        //Input can have ANY subject, but must be valid against our data (E.G. not "GO ASDF")
        string validated_subject = "";
        for (int i=0; i<sizeOfCategory("items"); i++)
        {
            if (getCategory("items")[i]["name"] == input_subject)
            {
                validated_subject = input_subject;
                break;
            }
        }
        if (validated_subject == "")
        {
            for (int i=0; i<sizeOfCategory("specials"); i++)
            {
                if (getCategory("specials")[i]["name"] == input_subject)
                {
                    validated_subject = input_subject;
                    break;
                }
            }
        }
        return validated_subject;
    }
    else
    {
        //Subject is just plain text, check it
        if (command_subject == input_subject)
        {
            return input_subject;
        }
        else
        {
            return "";
        }
    }
}

/* Perform the action now we know it's valid */
void GameLogic::performAction()
{
    //Handle candle life per action
    if (flagIsActiveOnItem("is_lit", "CANDLE") &&
        progression().getCandleLife() != 0)
    {
        progression().decreaseCandleLife();
    }
    else if (progression().getCandleLife() == 0)
    {
        disableFlagOnItem("is_lit", "CANDLE");
    }

    //Perform action
    auto command = getCategory("inputs")[action_data.getActionID()];
    if (command["response"].is_string())
    {
        refreshText(parseResponse(command["response"]));
    }
    else if (command["action"].is_string())
    {
        string action = command["action"];
        if (action == "move")
        {
            move(action_data.getSubject());
        }
        else if (action.substr(0,action.length()-1) == "move_")
        {
            move(action.substr(action.length()-1, 1));
        }
        else if (action == "equip")
        {
            equip(action_data.getSubject());
        }
        else if (action == "unequip")
        {
            unequip(action_data.getSubject());
        }
        else if (action == "examine")
        {
            examine(action_data.getSubject());
        }
        else if (action == "open")
        {
            open(action_data.getSubject());
        }
        else if (action == "swing_axe")
        {
            swing();
        }
        else if (action == "light_candle")
        {
            light(true);
        }
        else if (action == "unlight_candle")
        {
            light(false);
        }
        else if (action == "show_score")
        {
            score();
        }
        else if (action == "read")
        {
            read(action_data.getSubject());
        }
        else if (action == "say")
        {
            say(action_data.getSubject());
        }
        else if (action == "open_door")
        {
            unlock();
        }
        else if (action == "use_item")
        {
            use(action_data.getSubject());
        }
        else if (action == "climb_tree")
        {
            climb(action_data.getSubject());
        }
        else if (action == "dig")
        {
            dig();
        }
        else if (action == "spray")
        {
            spray();
        }
    }
    else
    {
        refreshText("THAT'S SILLY!");
    }

    //Output any candle issues
    if (progression().getCandleLife() == 10)
    {
        refreshText("YOUR CANDLE IS WANING!");
    }
    else if (progression().getCandleLife() == 1)
    {
        refreshText("YOUR CANDLE IS OUT!");
    }

    //Check for item completeness
    if ((progression().getScore() == sizeOfCategory("items") - 1) &&
        !paramIsOnTile("DOUBLE_SCORE") &&
        command["action"] != "show_score" &&
        !itemIsInInventory("BOAT"))
    {
        refreshText("YOU HAVE EVERYTHING\nRETURN TO THE GATE FOR FINAL SCORE");
    }
}


/* Output logic */

/* Parse a response output for any special actions */
string GameLogic::parseResponse(string response)
{
    //Check for formatted data in response
    int special_start_index = 0;
    int special_end_index = 0;
    int modifier_start_index = 0;
    int modifier_end_index = 0;
    bool found_special = false;
    bool found_modifier = false;
    for (int i=0; i<response.length(); i++)
    {
        if (response[i] == '{')
        {
            found_special = true;
            special_start_index = i;
        }
        else if (response[i] == '}')
        {
            special_end_index = i;
        }
        else if (response[i] == '[')
        {
            found_modifier = true;
            modifier_start_index = i;
        }
        else if (response[i] == ']')
        {
            modifier_end_index = i;
        }
    }

    //If formatted, parse it
    if (found_special)
    {
        string category_name = response.substr(special_start_index + 1, special_end_index - special_start_index - 1);
        string category_modifier = response.substr(modifier_start_index + 1, modifier_end_index - modifier_start_index - 1);
        string category_output = response.substr(0, special_start_index) + "\n";
        int result_count = 0;
        auto category_data = getCategory(category_name);
        for (int i=0; i<sizeOfCategory(category_name); i++)
        {
            if (category_data[i]["name"].is_string())
            {
                if (found_modifier &&
                    category_modifier == "inventory")
                {
                    if (category_data[i]["equipped"] == "true")
                    {
                        //Return inventory
                        category_output += (string)category_data[i]["name"] + ",";
                        result_count++;
                    }
                }
                else
                {
                    //Return whole category names
                    category_output += (string)category_data[i]["name"] + ",";
                    result_count++;
                }
            }
            else if (category_data[i]["command"].is_string())
            {
                //Return whole category commands
                category_output += (string)category_data[i]["command"] + ",";
                result_count++;
            }
            if (result_count % 7 == 0 &&
                result_count >= 7)
            {
                //Every 7 items break a line
                category_output += "\n";
            }
        }
        return category_output;
    }
    //No parsing needed
    else
    {
        return response;
    }
}


/* Actions */

/* Move to another block */
void GameLogic::move(string direction)
{
    char identifier = toupper(direction[0]);
    if (movementIsValidHere(identifier)) {
        //Pre-check for things that get you stuck
        if (paramIsOnTile("BOAT_REQUIRED") &&
            !itemIsInInventory("BOAT"))
        {
            refreshText("YOU'RE STUCK!");
        }
        else if (paramIsOnTile("dark") &&
                 !flagIsActiveOnItem("is_lit", "CANDLE"))
        {
            refreshText("TOO DARK TO MOVE");
        }
        else if (paramIsOnTile("MAGICAL_BARRIER") &&
                 itemIsInInventory("PAINTING") &&
                 !flagIsActiveOnItem("barrier_defeated", "MAGIC SPELLS"))
        {
            refreshText("A MAGICAL BARRIER TO THE WEST");
        }
        else if (paramIsOnTile("SPOOKY_GHOSTS") &&
                 !flagIsActiveOnItem("activated", "VACUUM") &&
                 itemIsInInventory("PAINTING"))
        {
            refreshText("GHOSTS WILL NOT LET YOU MOVE");
        }
        else if (paramIsOnTile("BATS") &&
                 !flagIsActiveOnItem("activated", "AEROSOL") &&
                 (rand() % 5 < 3))
        {
            refreshText("BATS ATTACKING!");
        }
        else if (flagIsActiveOnItem("climbed", "ROPE"))
        {
            makeItemVisible("ROPE");
            disableFlagOnItem("climbed", "ROPE");
            refreshText("CRASH! YOU FELL OUT OF THE TREE!");
        }
        else
        {
            progression().cacheMovement();

            //Perform valid movement
            if (identifier == 'N')
            {
                progression().setCurrentTile(progression().getCurrentTile() - mapTilesPerRow());
            }
            else if (identifier == 'E')
            {
                progression().setCurrentTile(progression().getCurrentTile() + 1);
            }
            else if (identifier == 'S')
            {
                progression().setCurrentTile(progression().getCurrentTile() + mapTilesPerRow());
            }
            else if (identifier == 'W')
            {
                progression().setCurrentTile(progression().getCurrentTile() - 1);
            }

            //Check for movement issues
            if (!paramIsOnTile("BOAT_ALLOWED") &&
                itemIsInInventory("BOAT"))
            {
                progression().revertMovement();
                refreshText("YOU CAN'T CARRY A BOAT!");
            }
            else if (paramIsOnTile("dark") &&
                     !flagIsActiveOnItem("is_lit", "CANDLE"))
            {
                progression().revertMovement();
                refreshText("YOU NEED A LIGHT");
            }
            else
            {
                //Tile specific movement responses
                if (flagIsActiveOnTile("DOOR_SLAM"))
                {
                    disableFlagOnTile("DOOR_SLAM");
                    refreshText("THE DOOR SLAMS SHUT!");
                }
                else
                {
                    //Didn't get any issues or responses
                    refreshText("OK");
                }
            }
        }
    }
    else
    {
        //Impossible to move this direction
        refreshText("CAN'T GO THAT WAY!");
    }
}

/* Add item to our inventory */
void GameLogic::equip(string item)
{
    if (!itemIsInInventory(item))
    {
        if (itemIsOnTile(item))
        {
            if (itemIsVisible(item))
            {
                equipItem(item);
                refreshText("YOU HAVE THE " + item);
                progression().increaseScore(1);
            }
            else
            {
                refreshText("WHAT "+item+"?");
            }
        }
        else
        {
            refreshText("IT ISN'T HERE");
        }
    }
    else
    {
        refreshText("YOU ALREADY HAVE IT");
    }
}

/* Remove an item from our inventory */
void GameLogic::unequip(string item)
{
    if (itemIsInInventory(item))
    {
        getCategory("items")[getItemID(item)]["tile"] = to_string(progression().getCurrentTile());
        unequipItem(item);
        refreshText("DONE");
        progression().decreaseScore(1);
    }
    else
    {
        refreshText("YOU DON'T HAVE '"+item+"'");
    }
}

/* Open a door or drawer */
void GameLogic::open(string object)
{
    if (object == "DRAWER" &&
        paramIsOnTile("EXPLORE_DRAWER"))
    {
        makeItemVisible("CANDLE");
        refreshText("DRAWER OPEN");
    }
    else if (object == "DOOR" &&
             paramIsOnTile("BIG_DOOR"))
    {
        if (!flagIsActiveOnItem("was_used", "KEY"))
        {
            refreshText("IT'S LOCKED");
        }
        else
        {
            refreshText("IT'S ALREADY OPEN!");
        }
    }
    else if (object == "COFFIN" &&
             paramIsOnTile("EXPLORE_COFFIN"))
    {
        makeItemVisible("RING");
        refreshText("THAT'S CREEPY!");
    }
    else
    {
        refreshText("THAT'S SILLY!");
    }
}

/* Examine an object */
void GameLogic::examine(string object)
{
    if (paramIsOnTile("EXPLORE_"+object))
    {
        if (object == "RUBBISH")
        {
            refreshText("THAT'S DISGUSTING!");
        }
        else if (object == "DESK" || object == "DRAWER")
        {
            refreshText("THERE IS A DRAWER");
        }
        else if (object == "COAT")
        {
            makeItemVisible("KEY");
            refreshText("SOMETHING HERE!");
        }
        else if (object == "WALL")
        {
            refreshText("THERE IS SOMETHING BEYOND...");
        }
        else if (object == "COFFIN")
        {
            makeItemVisible("RING");
            refreshText("THAT'S CREEPY!");
        }
        else
        {
            refreshText("THAT'S SILLY!");
        }
    }
    else
    {
        if (object == "SCROLL" &&
            itemIsInInventory("SCROLL"))
        {
            refreshText("THE SCRIPT IS AN ALIEN TONGUE");
        }
        else
        {
            refreshText("IT ISN'T HERE");
        }
    }
}

/* Swing axe (can only be accessed with axe in current json config) */
void GameLogic::swing()
{
    if (itemIsInInventory("AXE"))
    {
        if (paramIsOnTile("breakable_wall"))
        {
            setCurrentTileName("STUDY WITH SECRET ROOM");
            setCurrentTileExits("NW");
            refreshText("YOU BROKE THE THIN WALL");
        }
        else
        {
            refreshText("WHOOSH!");
        }
    }
    else
    {
        refreshText("YOU DON'T HAVE 'AXE'");
    }
}

/* Light candle */
void GameLogic::light(bool on)
{
    if (itemIsInInventory("CANDLE"))
    {
        if (on)
        {
            if (itemIsInInventory("MATCHES"))
            {
                if (itemIsInInventory("CANDLESTICK"))
                {
                    if (progression().getCandleLife() == 0)
                    {
                        refreshText("IT HAS ALREADY BURNED OUT");
                    }
                    else
                    {
                        enableFlagOnItem("is_lit", "CANDLE");
                        refreshText("IT CASTS A FLICKERING LIGHT");
                    }
                }
                else
                {
                    refreshText("IT WILL BURN YOUR HANDS");
                }
            }
            else
            {
                refreshText("NOTHING TO LIGHT IT WITH");
            }
        }
        else
        {
            if (flagIsActiveOnItem("is_lit", "CANDLE"))
            {
                disableFlagOnItem("is_lit", "CANDLE");
                refreshText("EXTINGUISHED");
            }
            else
            {
                refreshText("CANDLE IS NOT LIT.");
            }
        }
    }
    else
    {
        refreshText("YOU DON'T HAVE 'CANDLE'");
    }
}

/* Show score */
void GameLogic::score()
{
    if ((progression().getScore() == sizeOfCategory("items") - 1) &&
        paramIsOnTile("DOUBLE_SCORE"))
    {
        progression().increaseScore(progression().getScore());
        refreshText("DOUBLE SCORE FOR REACHING HERE!\nSCORE="+to_string(progression().getScore()), true);
    }
    else
    {
        refreshText("YOUR SCORE="+to_string(progression().getScore()));
    }
}

/* Read an object */
void GameLogic::read(string object)
{
    if (object == "BOOKS" &&
        paramIsOnTile("READ_BOOKS"))
    {
        refreshText("THEY ARE DEMONIC WORKS");
    }
    else if ((object == "MAGIC SPELLS" ||
             object == "SPELLS") &&
             itemIsInInventory("MAGIC SPELLS"))
    {
        refreshText("USE THIS WORD WITH CARE 'XZANFAR'");
    }
    else if (object == "SCROLL" &&
             itemIsInInventory("SCROLL"))
    {
        refreshText("THE SCRIPT IS AN ALIEN TONGUE");
    }
    else
    {
        refreshText("YOU DON'T HAVE '"+object+"'");
    }
}

/* Say a word / do MAGIC! */
void GameLogic::say(string word)
{
    if (word == "XZANFAR" &&
        itemIsInInventory("MAGIC SPELLS"))
    {
        if (paramIsOnTile("MAGICAL_BARRIER") &&
            itemIsInInventory("PAINTING") &&
            !flagIsActiveOnItem("barrier_defeated", "MAGIC SPELLS"))
        {
            enableFlagOnItem("barrier_defeated", "MAGIC SPELLS");
        }
        else
        {
            progression().setCurrentTile(rand() % totalMapTiles());
        }
        refreshText("*MAGIC OCCURS*");
    }
    else
    {
        refreshText("OK '"+word+"'");
    }
}

/* Unlock door */
void GameLogic::unlock()
{
    if (paramIsOnTile("BIG_DOOR") &&
        itemIsInInventory("KEY"))
    {
        setCurrentTileName("HUGE OPEN DOOR");
        setCurrentTileExits("SEW");
        enableFlagOnItem("was_used", "KEY");
        refreshText("THE KEY TURNS!");
    }
    else
    {
        if (paramIsOnTile("BIG_DOOR"))
        {
            refreshText("WITH WHAT?"); //No key
        }
        else
        {
            refreshText("WHAT DOOR?"); //Wrong tile
        }
    }
}

/* Use an item if requested */
void GameLogic::use(string item)
{
    if (item == "VACUUM" &&
        itemIsInInventory("VACUUM"))
    {
        if (itemIsInInventory("BATTERIES"))
        {
            enableFlagOnItem("activated", "VACUUM");
            if (paramIsOnTile("SPOOKY_GHOSTS") &&
                itemIsInInventory("PAINTING"))
            {
                refreshText("WHIZZ- VACUUMED THE GHOSTS UP!");
            }
            else
            {
                refreshText("SWITCHED ON");
            }
        }
        else
        {
            refreshText("NOTHING TO POWER IT WITH!");
        }
    }
    else
    {
        refreshText("THAT'S SILLY!");
    }
}

/* Climb something */
void GameLogic::climb(string object)
{
    if (itemIsInInventory("ROPE"))
    {
        refreshText("IT ISN'T ATTACHED TO ANYTHING!");
    }
    else
    {
        if (paramIsOnTile("CAN_CLIMB"))
        {
            if (flagIsActiveOnItem("climbed", "ROPE"))
            {
                makeItemVisible("ROPE");
                disableFlagOnItem("climbed", "ROPE");
                refreshText("GOING DOWN!");
            }
            else
            {
                makeItemInvisible("ROPE");
                enableFlagOnItem("climbed", "ROPE");
                refreshText("YOU SEE THICK FOREST AND CLIFF SOUTH");
            }
        }
        else
        {
            refreshText("NOTHING TO CLIMB!");
        }
    }
}

/* Dig a hole or dig out bars in supported location(s) */
void GameLogic::dig()
{
    if (itemIsInInventory("SHOVEL"))
    {
        if (paramIsOnTile("DIG_BARS"))
        {
            setCurrentTileName("HOLE IN WALL");
            setCurrentTileExits("NSE");
            refreshText("DUG THE BARS OUT");
        }
        else
        {
            refreshText("YOU MADE A HOLE");
        }
    }
    else
    {
        refreshText("YOU WILL GET YOUR HANDS DIRTY!");
    }
}

/* Spray aerosol can */
void GameLogic::spray()
{
    if (itemIsInInventory("AEROSOL"))
    {
        if (paramIsOnTile("BATS"))
        {
            enableFlagOnItem("activated", "AEROSOL");
            refreshText("PFFT! GOT THEM");
        }
        else
        {
            refreshText("HISSSS");
        }
    }
    else
    {
        refreshText("YOU DON'T HAVE 'AEROSOL'");
    }
}