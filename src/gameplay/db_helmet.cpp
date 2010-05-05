#include "headers.h"

#include "shared/ccor.h"
#include "database.h"


using namespace ccor;
using namespace database;
using namespace std;

#define MODELID_SYNDROME       0
#define DESCRIPTIONID_SYNDROME 333
#define COST_SYNDROME          50.0f
#define PROPS_SYNDROME         0.25f, 0.125f

#define MODELID_TENSOR       1
#define DESCRIPTIONID_TENSOR 366 
#define COST_TENSOR          250.0f
#define PROPS_TENSOR         0.33f, 0.25f

#define MODELID_SHOOTER       2
#define DESCRIPTIONID_SHOOTER 368
#define COST_SHOOTER          250.0f
#define PROPS_SHOOTER         0.5f, 0.5f

static std::vector<Helmet> helmets;

#if 0
{
    /* 000 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_DARK_BLUE, MFRID_D3, 0 },
    /* 001 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_PURPLE, MFRID_D3, 1 },
    /* 002 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_KHAKI, MFRID_D3, 2 }, 
    /* 003 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_LILAC, MFRID_D3, 3 }, 
    /* 004 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_YELLOW, MFRID_D3, 4 }, 
    /* 005 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_MAGENTA, MFRID_D3, 5 }, 
    /* 006 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_LIGHT_BLUE, MFRID_D3, 6 }, 
    /* 007 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_LIGHT_GREEN, MFRID_D3, 7 }, 
    /* 008 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_ORANGE, MFRID_D3, 8 },
    /* 009 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_WHITE, MFRID_D3, 9 }, 
    /* 010 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_CRIMSON, MFRID_D3, 11 }, 
    /* 011 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_LIGHT_VIOLET, MFRID_D3, 12 }, 
    /* 012 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_VIOLET, MFRID_D3, 13 }, 
    /* 013 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_DARK_GREEN, MFRID_D3, 15 },
    /* 014 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_WHITE, MFRID_D3, 18 },
    /* 015 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_LIGHT_BLUE, MFRID_D3, 19 },
    /* 016 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_DARK_BLUE, MFRID_D3, 20 },
    /* 017 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_VIOLET, MFRID_D3, 21 },
    /* 018 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_LIGHT_VIOLET, MFRID_D3, 22 },
    /* 019 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_MAGENTA, MFRID_D3, 23 },
    /* 020 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_DARK_GREEN, MFRID_D3, 24 },
    /* 021 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_LIGHT_GREEN, MFRID_D3, 25 },
    /* 022 */ { COST_TENSOR, PROPS_TENSOR, 1, MODELID_TENSOR, DESCRIPTIONID_TENSOR, CLID_YELLOW, MFRID_D3, 26 },
    /* 023 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_WHITE, MFRID_D3, 27 },
    /* 024 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_DARK_BLUE, MFRID_D3, 28 },
    /* 025 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_LIGHT_BLUE, MFRID_D3, 29 },
    /* 026 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_VIOLET, MFRID_D3, 30 },
    /* 027 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_LIGHT_VIOLET, MFRID_D3, 31 },
    /* 028 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_MAGENTA, MFRID_D3, 32 },
    /* 029 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_YELLOW, MFRID_D3, 33 },
    /* 030 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_LIGHT_GREEN, MFRID_D3, 34 },
    /* 031 */ { COST_SHOOTER, PROPS_SHOOTER, 2, MODELID_SHOOTER, DESCRIPTIONID_SHOOTER, CLID_DARK_GREEN, MFRID_D3, 35 },
    /* 032 */ { COST_SYNDROME, PROPS_SYNDROME, 0, MODELID_SYNDROME, DESCRIPTIONID_SYNDROME, CLID_BLACK, MFRID_D3, 64 },
    { 0.0f, 0, 0, 0, 0, 0 },
};
#endif


unsigned int Helmet::getNumRecords(void)
{
        return helmets.size();
}

Helmet* Helmet::getRecord(unsigned int id)
{
    assert( id >= 0 && id < getNumRecords() );
    return &helmets[id];
}

int Helmet::getRecordId(char* name)
{
        string s(name);

        int i;
        for (i = 0; i < (int)helmets.size(); ++i) {
                if (helmets[i].name == s) {
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


void Helmet::initHelmets()
{
        getCore()->logMessage("Info: Loading helmets.");

        Helmet prototypes[] = {
                { COST_SYNDROME, PROPS_SYNDROME, MODELID_SYNDROME, "Syndrome", L"Syndrome", DESCRIPTIONID_SYNDROME, CLID_WHITE, MFRID_D3, "" },
                { COST_TENSOR,   PROPS_TENSOR,   MODELID_TENSOR,   "Tensor", L"Tensor",     DESCRIPTIONID_TENSOR,   CLID_WHITE, MFRID_D3, "" },
                { COST_SHOOTER,  PROPS_SHOOTER,  MODELID_SHOOTER,  "Shooter", L"Shooter",   DESCRIPTIONID_SHOOTER,  CLID_WHITE, MFRID_D3, "" },
        };

        // Add helmet 0 - AFF helmet
        prototypes[0].texture = "./res/Gear/Special/AFF Suit.dds";
        prototypes[0].name = "AFF Helmet";
        prototypes[0].wname = L"AFF Helmet";
        helmets.push_back(prototypes[0]);

        loadHelmets(prototypes[0], "./res/Gear/Helmets/Syndrome/", "./res/Gear/Helmets/Syndrome/*.dds");
        loadHelmets(prototypes[1], "./res/Gear/Helmets/Tensor/", "./res/Gear/Helmets/Tensor/*.dds");
        loadHelmets(prototypes[2], "./res/Gear/Helmets/Shooter/", "./res/Gear/Helmets/Shooter/*.dds");
 
        getCore()->logMessage("Info: Helmets loaded.");
}


void Helmet::loadHelmets(Helmet& helmetPrototype, string textureBase, const char* dir)
{
        std::vector<string> files;
        
        if (getdir(dir, &files)) {
                int i;
                for (i = 0; i < (int)files.size(); ++i) {
                        helmetPrototype.texture = textureBase + files[i];

                        string s(files[i]);
                        s = s.substr(0, s.find_last_of('.'));
                        wstring name(L" ", s.length());
                        copy(s.begin(), s.end(), name.begin());

                        helmetPrototype.name = s;
                        helmetPrototype.wname = name;
                        helmets.push_back(helmetPrototype);
                }
        } else {
                getCore()->logMessage("Can't open directory: %s", "");
        }
}
