
#include "headers.h"
#include "database.h"

using namespace ccor;
using namespace database;
using namespace std;

#define MODELID_VECTOR_VELCRO 174
#define MODELID_VECTOR_PIN    175
#define MODELID_HARPY         370
#define MODELID_FB_BASE       749
#define MODELID_FB_SKYDIVING  750

#define DESCRIPTIONID_VECTOR_VELCRO 341
#define DESCRIPTIONID_VECTOR_PIN    342
#define DESCRIPTIONID_HARPY         371
#define DESCRIPTIONID_FB_BASE       751
#define DESCRIPTIONID_FB_SKYDIVING  752

#define COST_VECTOR_VELCRO 325.0f
#define COST_VECTOR_PIN    255.0f
#define COST_HARPY         450.0f
#define COST_FB_BASE       0.0f
#define COST_FB_SKYDIVING  0.0f

#define PROPS_VECTOR_VELCRO 0.0f, 3.0f, 0.0f, 12.0f, 1.0f/60.0f
#define PROPS_VECTOR_PIN    0.0f, 3.0f, 0.0f, 12.0f, 1.0f/120.0f
#define PROPS_HARPY         0.0f, 3.0f, 0.0f, 12.0f, 1.0f/60.0f
#define PROPS_FB_BASE       0.0f, 3.0f, 0.0f, 12.0f, 1.0f/120.0f
#define PROPS_FB_SKYDIVING  0.0f, 3.0f, 0.0f, 12.0f, 1.0f/60.0f

static std::vector<Rig> rigs;

#if 0
{
    /* 000 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_DARK_BLUE, MFRID_D3, 0, PROPS_VECTOR_VELCRO },
    /* 001 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_PURPLE, MFRID_D3, 1, PROPS_VECTOR_VELCRO },
    /* 002 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_KHAKI, MFRID_D3, 2, PROPS_VECTOR_VELCRO },
    /* 003 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_LILAC, MFRID_D3, 3, PROPS_VECTOR_VELCRO },
    /* 004 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_YELLOW, MFRID_D3, 4, PROPS_VECTOR_VELCRO },
    /* 005 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_MAGENTA, MFRID_D3, 5, PROPS_VECTOR_VELCRO },
    /* 006 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_LIGHT_BLUE, MFRID_D3, 6, PROPS_VECTOR_VELCRO },
    /* 007 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_LIGHT_GREEN, MFRID_D3, 7, PROPS_VECTOR_VELCRO },
    /* 008 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_ORANGE, MFRID_D3, 8, PROPS_VECTOR_VELCRO },
    /* 009 */ { true, COST_VECTOR_VELCRO, false, MODELID_VECTOR_VELCRO, DESCRIPTIONID_VECTOR_VELCRO, CLID_WHITE, MFRID_D3, 9, PROPS_VECTOR_VELCRO },
    /* 010 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_YELLOW, MFRID_D3, 10, PROPS_VECTOR_PIN },
    /* 011 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_CRIMSON, MFRID_D3, 11, PROPS_VECTOR_PIN },
    /* 012 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_LIGHT_VIOLET, MFRID_D3, 12, PROPS_VECTOR_PIN },
    /* 013 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_VIOLET, MFRID_D3, 13, PROPS_VECTOR_PIN },
    /* 014 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_DARK_BLUE, MFRID_D3, 14, PROPS_VECTOR_PIN },
    /* 015 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_DARK_GREEN, MFRID_D3, 15, PROPS_VECTOR_PIN },
    /* 016 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_WHITE, MFRID_D3, 16, PROPS_VECTOR_PIN },
    /* 017 */ { true, COST_HARPY, true, MODELID_HARPY, DESCRIPTIONID_HARPY, CLID_WHITE, MFRID_D3, 45, PROPS_HARPY },
    /* 018 */ { true, COST_HARPY, true, MODELID_HARPY, DESCRIPTIONID_HARPY, CLID_MAGENTA, MFRID_D3, 46, PROPS_HARPY },
    /* 019 */ { true, COST_HARPY, true, MODELID_HARPY, DESCRIPTIONID_HARPY, CLID_YELLOW, MFRID_D3, 47, PROPS_HARPY },
    /* 020 */ { true, COST_HARPY, true, MODELID_HARPY, DESCRIPTIONID_HARPY, CLID_DARK_GREEN, MFRID_D3, 48, PROPS_HARPY },
    /* 021 */ { true, COST_VECTOR_PIN, false, MODELID_VECTOR_PIN, DESCRIPTIONID_VECTOR_PIN, CLID_BLACK, MFRID_D3, 64, PROPS_VECTOR_PIN },
    /* 022 */ { false, COST_FB_BASE, false, MODELID_FB_BASE, DESCRIPTIONID_FB_BASE, CLID_WHITE, MFRID_D3, 70, PROPS_FB_BASE },
    /* 023 */ { false, COST_FB_SKYDIVING, true, MODELID_FB_SKYDIVING, DESCRIPTIONID_FB_SKYDIVING, CLID_WHITE, MFRID_D3, 70, PROPS_FB_SKYDIVING },
    { 0.0f, 0,0,0,0 }
};
#endif


unsigned int Rig::oldRigIdToNewId(unsigned int oldId)
{
        string type;

        if (oldId >= 0 && oldId <= 9) {
                type = "Vector Velcro";
        } else if (oldId >= 10 && oldId <= 16) {
                type = "Vector Pin";
        } else if (oldId >= 17 && oldId <= 20) {
                type = "Harpy";
        } else if (oldId >= 21 && oldId <= 21) {
                type = "Vector Pin";
        } else if (oldId >= 22 && oldId <= 22) {
                type = "Vector Pin";
        } else if (oldId >= 23 && oldId <= 23) {
                type = "Harpy";
        } else {
                assert("Shouldn't happen");
                type = "Vector Pin"; // Safe choise :-D
        }

        int i;
        int count = 0;
        // count
        for (i = 0; i < rigs.size(); ++i) {
                if (rigs[i].type == type) {
                        ++count;
                }
        }
        // select random
        int choose = (rand() >> 4) % count;
        count = 0;
        // find it
        for (i = 0; i < rigs.size(); ++i) {
                if (rigs[i].type == type) {
                        if (count != choose) {
                                ++count;
                        } else {
                                return i;
                        }
                }
        }

        assert("Shouldn't happen");
        return 0;
}


unsigned int Rig::getNumRecords(void)
{
        return rigs.size();
}

Rig* Rig::getRecord(unsigned int id)
{
    assert( id >= 0 && id < getNumRecords() );
    return &rigs[id];
}



int Rig::getRecordId(char* name)
{
        string s(name);

        int i;
        for (i = 0; i < (int)rigs.size(); ++i) {
                if (rigs[i].name == s) {
                        return i;
                }
        }

        return -1;
}


static bool getdir (string dir, vector<string>* files)
{
     WIN32_FIND_DATA find_data;
     HANDLE hnd;

     find_data.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
     if((hnd = FindFirstFile(dir.c_str(), &find_data)) == INVALID_HANDLE_VALUE) {
          return true; //no flies or error
     }
     
     do {
             files->push_back(find_data.cFileName);
     } while(FindNextFile(hnd, &find_data));
     
     FindClose(hnd);

     return true;
}


void Rig::initRigs()
{
        getCore()->logMessage("Info: Loading rigs.");

        Rig prototypes[] = {
                { true, COST_VECTOR_VELCRO, false, "AFF Container", "AFF Container", L"AFF Container", DESCRIPTIONID_VECTOR_VELCRO, CLID_WHITE, MFRID_D3, "./res/Gear/Special/AFF Suit.dds", PROPS_VECTOR_VELCRO },
                { true, COST_VECTOR_VELCRO, false, "Vector Velcro", "Vector Velcro", L"Vector Velcro", DESCRIPTIONID_VECTOR_VELCRO, CLID_WHITE, MFRID_D3, "", PROPS_VECTOR_VELCRO },
                { true, COST_VECTOR_PIN,    false, "Vector Pin", "Vector Pin", L"Vector Pin",       DESCRIPTIONID_VECTOR_PIN,    CLID_WHITE, MFRID_D3, "", PROPS_VECTOR_PIN },
                { true, COST_HARPY,         true,  "Harpy", "Harpy", L"Harpy",                 DESCRIPTIONID_HARPY,         CLID_WHITE, MFRID_D3, "", PROPS_HARPY }
        };

//    /* 022 */ { false, COST_FB_BASE, false, MODELID_FB_BASE, DESCRIPTIONID_FB_BASE, CLID_WHITE, MFRID_D3, 70, PROPS_FB_BASE },
//    /* 023 */ { false, COST_FB_SKYDIVING, true, MODELID_FB_SKYDIVING, DESCRIPTIONID_FB_SKYDIVING, CLID_WHITE, MFRID_D3, 70, PROPS_FB_SKYDIVING },


        // Add rig 0 - AFF rig
        rigs.push_back(prototypes[0]);

        loadRigs(prototypes[1], "./res/Gear/Containers/Vector Velcro/", "./res/Gear/Containers/Vector Velcro/*.dds");
        loadRigs(prototypes[2], "./res/Gear/Containers/Vector Pin/", "./res/Gear/Containers/Vector Pin/*.dds");
        loadRigs(prototypes[3], "./res/Gear/Containers/Harpy/", "./res/Gear/Containers/Harpy/*.dds");
 
        getCore()->logMessage("Info: Rigs loaded.");
}


void Rig::loadRigs(Rig& rigPrototype, string textureBase, const char* dir)
{
        std::vector<string> files;
        
        if (getdir(dir, &files)) {
                string baseName = rigPrototype.name;
                wstring baseWName = rigPrototype.wname;

                int i;
                for (i = 0; i < (int)files.size(); ++i) {
                        rigPrototype.texture = textureBase + files[i];

                        string s(files[i]);
                        s = s.substr(0, s.find_last_of('.'));
                        wstring name(L" ", s.length());
                        copy(s.begin(), s.end(), name.begin());

                        rigPrototype.name = baseName + " " + s;
                        rigPrototype.wname = baseWName + L" " + name;
                        rigs.push_back(rigPrototype);
                }
        } else {
                getCore()->logMessage("Can't open directory: %s", "");
        }
}
