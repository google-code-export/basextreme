
#ifndef GEAR_DESCRIPTOR_INCLUDED
#define GEAR_DESCRIPTOR_INCLUDED

#include "../shared/vector.h"
#include "../shared/gui.h"

/**
 * gear types
 */

enum GearType
{
    gtUnequipped,
    gtHelmet,
    gtSuit,
    gtRig,
    gtCanopy
};

/**
 * gear descriptor
 */

struct Gear
{
public:
    GearType     type;  // type of gear
    int          id;    // database identifier
    char         name[64];  // named database identifier
    float        state; // state factor (0..1)
    unsigned int age;   // age (number of jumps)
public:
    Gear() : type(gtUnequipped), id(0), state(0), age(0) { name[0] = 0; }    
    Gear(GearType t, unsigned int i);
public:
    void updateNameFromId();
    void updateIdFromName();
    float getCost(void);
    const wchar_t* getName(void);
    const wchar_t* getDescription(void);
    const wchar_t* getStateText(void);
    const wchar_t* getStateDescription(void);
    Vector4f getStateColor(void);
    Vector4f getGearColor(void);
    gui::Rect getGearPreview(void);
    bool isTradeable(void);
};

#endif