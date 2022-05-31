#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

class cContactEvent
{
public:
    std::string cell1;
    std::string cell2;
    int start;
    int end;
    cContactEvent(const std::string &line);
    bool operator==(const cContactEvent &other) const
    {
        return cell1 == other.cell1 && cell2 == other.cell2;
    }
};
class cContacts
{
public:
    std::vector<cContactEvent> myContact;
    std::vector<cContactEvent> myUnique;
    int mymaxframe;

    void read(const std::string &fname);
    void indexContacts();
    void textContactIndex();
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

                // draw the dot - here I just output the co=-ordinates
                std::cout << "dot at " << frame << "," << yPosition << "\n";
            }

            if (frame == contact.end)
                incontact = false;
        }
    }
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        myContacts.read("test.txt");
        myContacts.indexContacts();
        myContacts.textContactIndex();
        myContacts.placeDots();

        show();
        run();
    }

private:
    wex::label &lb;
    cContacts myContacts;
};

main()
{
    cGUI theGUI;
    return 0;
}
