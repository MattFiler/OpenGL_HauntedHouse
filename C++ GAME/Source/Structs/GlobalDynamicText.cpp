#include "GlobalDynamicText.h"

/* Compile output to display on screen */
string GlobalDynamicText::getScreenText()
{
    //Pretty-ify exits text
    string exits_to_display = "";
    for (int i=0; i<location_exits_text.length(); i++)
    {
        exits_to_display += location_exits_text[i];
        exits_to_display += ",";
    }

    //Begin output string
    string output = "HAUNTED HOUSE\n"
                    "---------------\n"
                    "YOUR LOCATION\n"+location_text+"\n"
                    "EXITS: "+exits_to_display+"\n";

    //Include items in location if they exist
    if (location_item_text != "")
    {
        output += location_item_text;
    }

    //Finish output string
    output += "====================\n"
              +system_response_text+"\n"
              "WHAT WILL YOU DO NOW: ";

    return output;
}

/* Initial static declarations */
std::string GlobalDynamicText::location_text = "";
std::string GlobalDynamicText::location_exits_text = "";
std::string GlobalDynamicText::location_item_text = "";
std::string GlobalDynamicText::system_response_text = "";