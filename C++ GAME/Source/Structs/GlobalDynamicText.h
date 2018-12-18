#include <string>
using namespace std;

/*
 * Structure to store text to output across the game.
 */

struct GlobalDynamicText
{
public:
    /* Set global text */
    void setLocationName(string name) { location_text = name; }
    void setLocationExits(string exits) { location_exits_text = exits; }
    void setLocationItems(string items) { location_item_text = items; }
    void setSystemResponse(string response) { system_response_text = response; }

    /* Return formatted text */
    string getScreenText();

private:
    static string location_text;
    static string location_exits_text;
    static string location_item_text;
    static string system_response_text;
};