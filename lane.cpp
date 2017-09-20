#include "lane.h"

Lane::Lane(Point lineP1, Point lineP2, Rect toTrack, String name)
{
    this->name = name;
    this->lineP1 = lineP1;
    this->lineP2 = lineP2;
    this->toTrack = toTrack;
    contacted = false;
    objCounter = 0;
    lineContactHeat = vector <int>( lineP2.x - lineP1.x );
    fill(lineContactHeat.begin(), lineContactHeat.end(), 0);
}

bool Lane::isContacted(const int xc, const int sensibility, const int hotness)
{
    bool contact = false;
    for(int i = -sensibility; i < sensibility; ++i) {
        // If there is still a hot contact, return true
        if ( lineContactHeat[ xc - lineP1.x - i ] != 0 )
            contact = true;

        // But also make it hot anyways
        lineContactHeat[ xc - lineP1.x - i ] = hotness;
    }

    coolContacts();

    return contact;
}

bool Lane::coolContacts() {
    for (int i = 0; i < lineContactHeat.size(); ++i)
        if( lineContactHeat[i] > 0) --lineContactHeat[i];
}
