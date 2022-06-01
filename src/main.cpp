#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

/// a contact event
class cContactEvent
{
public:
    std::string cell1;
    std::string cell2;
    int start;
    int end;

    /// construct from line of input
    cContactEvent(const std::string &line);

    /// two contact events are equivalent if they involve same two cells
    bool operator==(const cContactEvent &other) const
    {
        return cell1 == other.cell1 && cell2 == other.cell2;
    }
};

/** all the contact events
 *
 * Implements https://stackoverflow.com/a/72451534/16582
 */
class cContacts
{
public:

    /// process input
    void process(const std::string &fname);

    /// draw dots
    void draw(wex::shapes &S);

private:
    /// Vector of contact events
    std::vector<cContactEvent> myContact;

    /// vector of first contact event between cell pair
    std::vector<cContactEvent> myUnique;

    /// vector of dot locations
    std::vector<std::pair<int, int>> myDotLocs;

    /// maximum time
    int mymaxframe;

    /// read input file
    void read(const std::string &fname);

    /// index unique cell pair contacts
    void indexContacts();

    /// display unique cell pair contacts as text on console
    void textContactIndex();

    /// display dot locations as text on console
    void textDotLocs();

    /// calculate dot locations
    void placeDots();
};

cContactEvent::cContactEvent(const std::string &line)
{
    std::istringstream iss(line);
    iss >> cell1;
    iss >> cell2;
    iss >> start;
    iss >> end;
}

void cContacts::process(const std::string &fname)
{
    read("test.txt");
    indexContacts();
    textContactIndex();
    placeDots();
    textDotLocs();
}

void cContacts::read(const std::string &fname)
{
    mymaxframe = 0;
    std::ifstream f(fname);
    if (!f.is_open())
        throw std::runtime_error(
            "Cannot open input file");
    std::string line;
    while (std::getline(f, line))
    {
        myContact.push_back(cContactEvent(line));
        if (myContact.back().end > mymaxframe)
            mymaxframe = myContact.back().end;
    }
}
void cContacts::indexContacts()
{
    for (auto &contact : myContact)
    {
        if (std::find(
                myUnique.begin(),
                myUnique.end(),
                contact) == myUnique.end())
            myUnique.push_back(contact);
    }
}
void cContacts::textContactIndex()
{
    int k = 1;
    for (auto &contact : myUnique)
    {
        std::cout << k++ << " " << contact.cell1 << " " << contact.cell2 << "\n";
    }
}
void cContacts::textDotLocs()
{
    for (auto &loc : myDotLocs)
    {
        std::cout << "dot at "
                  << loc.first << "," << loc.second
                  << "\n";
    }
}

void cContacts::placeDots()
{
    // loop over contacts
    for (auto &contact : myContact)
    {
        // track whether we are in contact
        bool incontact = false;

        // loop over frames
        for (int frame = 1; frame <= mymaxframe; frame++)
        {
            if (frame == contact.start)
                incontact = true;

            if (incontact)
            {
                // in contact - work out y position of dot from unique index
                int yPosition = std::find(
                                    myUnique.begin(),
                                    myUnique.end(),
                                    contact) -
                                myUnique.begin() + 1;
                myDotLocs.push_back(std::make_pair(frame, yPosition));
            }

            if (frame == contact.end)
                incontact = false;
        }
    }
}
void cContacts::draw(wex::shapes &S)
{
    S.fill();
    S.color(0x0000FF);
    for (auto &loc : myDotLocs)
    {
        S.circle(
            loc.first * 20,
            100 - loc.second * 20,
            10);
    }
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500})
    {
        myContacts.process( "test.txt" );

        fm.events().draw(
            [&](PAINTSTRUCT &ps)
            {
                wex::shapes S(ps);
                myContacts.draw(S);
            });

        show();
        run();
    }

private:
    cContacts myContacts;
};

main()
{
    cGUI theGUI;
    return 0;
}
