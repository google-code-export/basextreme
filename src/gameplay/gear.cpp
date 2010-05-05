
#include "headers.h"
#include "gear.h"
#include "database.h"
#include "gameplay.h"

const wchar_t* Gear::getName(void)
{
    switch( type )
    {
    case gtHelmet:
            return database::Helmet::getRecord(id)->wname.c_str();
    case gtSuit: {
            return database::Suit::getRecord( id )->wname.c_str();
    }
    case gtRig:
            return database::Rig::getRecord(id)->wname.c_str();
    case gtCanopy:
            return database::Canopy::getRecord( id )->wname.c_str();
    }
    return L"";
}

const wchar_t* Gear::getDescription(void)
{
    switch( type )
    {
    case gtHelmet:
        return Gameplay::iLanguage->getUnicodeString(database::Helmet::getRecord( id )->descriptionId);
    case gtSuit:
        return Gameplay::iLanguage->getUnicodeString(database::Suit::getRecord( id )->descriptionId);
    case gtRig:
        return Gameplay::iLanguage->getUnicodeString(database::Rig::getRecord( id )->descriptionId);
    case gtCanopy:
        return Gameplay::iLanguage->getUnicodeString(database::Canopy::getRecord( id )->descriptionId);
    }
    return L"";
}

float Gear::getCost(void)
{
    switch( type )
    {
    case gtHelmet:
        return float( unsigned int( database::Helmet::getRecord( id )->cost * state ) );
    case gtSuit:
        return float( unsigned int( database::Suit::getRecord( id )->cost * state ) );
    case gtRig:
        return float( unsigned int( database::Rig::getRecord( id )->cost * state ) );
    case gtCanopy:
        return float( unsigned int( database::Canopy::getRecord( id )->cost * state ) );
    }
    return 0.0f;
}

const wchar_t* Gear::getStateText(void)
{
    if( state > 0.9f ) return Gameplay::iLanguage->getUnicodeString(194);
    if( state > 0.75f ) return Gameplay::iLanguage->getUnicodeString(195);
    if( state > 0.5f ) return Gameplay::iLanguage->getUnicodeString(196);
    if( state > 0.25f ) return Gameplay::iLanguage->getUnicodeString(197);
    return Gameplay::iLanguage->getUnicodeString(198);
}

const wchar_t* Gear::getStateDescription(void)
{
    if( state > 0.9f ) return Gameplay::iLanguage->getUnicodeString(334);
    if( state > 0.75f ) return Gameplay::iLanguage->getUnicodeString(335);
    if( state > 0.5f ) return Gameplay::iLanguage->getUnicodeString(336);
    if( state > 0.25f ) return Gameplay::iLanguage->getUnicodeString(337);
    return Gameplay::iLanguage->getUnicodeString(338);
}

Vector4f Gear::getStateColor(void)
{
    if( state > 0.9f ) return Vector4f(0,0.5f,0,1);
    if( state > 0.75f ) return Vector4f(0,0.5f,0,1);
    if( state > 0.5f ) return Vector4f(0.5f,0.5f,0.25f,1);
    if( state > 0.25f ) return Vector4f(0.5f,0.5f,0.25f,1);
    return Vector4f(0.5f,0,0,1);
}

Vector4f Gear::getGearColor(void)
{
    // canopiest have a texture preview
    if( type == ::gtCanopy ) return Vector4f( 1,1,1,1 );

    unsigned int colorId = 0;
    switch( type )
    {
    case gtHelmet:
        colorId = database::Helmet::getRecord( id )->colorId;
        break;
    case gtSuit:
        colorId = database::Suit::getRecord( id )->colorId;
        break;
    case gtRig:
        colorId = database::Rig::getRecord( id )->colorId;
        break;
    }
    
    Vector4f result( 0,0,0,0 );
    switch( colorId )
    {
    case 156: result.set( 57,49,85,255 ); break; // "Темно-синий",
    case 157: result.set( 82,49,82,255 ); break; // "Пурпурный",
    case 158: result.set( 74,89,49,255 ); break; // "Хаки",
    case 159: result.set( 49,73,90,255 ); break; // "Сиреневый",
    case 160: result.set( 173,170,0,255 ); break; // "Желтый",
    case 161: result.set( 236,82,71,255 ); break; // "Ярко-красный",
    case 162: result.set( 80,115,233,255 ); break; // "Светло-синий",
    case 163: result.set( 88,158,55,255 ); break; // "Светло-зеленый",
    case 164: result.set( 203,105,0,255 ); break; // "Оранжевый",
    case 165: result.set( 208,206,208,255 ); break; // "Белый",
    case 166: result.set( 181,47,16,255 ); break; // "Малиновый",
    case 167: result.set( 197,16,99,255 ); break; // "Светло-фиолетовый",
    case 168: result.set( 148,61,151,255 ); break; // "Фиолетовый",
    case 169: result.set( 16,97,25,255 ); break; // "Темно-зеленый",
    case 230: result.set( 8,8,8,255 ); break; // "Черный",
    }
    result *= ( 1.0f / 255.0f );
    return result;
}

gui::Rect Gear::getGearPreview(void)
{
    if( type == ::gtCanopy )
    {
        return gui::Rect( 255, 255, 255, 255 );
    }
    else
    {
        return gui::Rect( 0,0,15,31 );
    }
}

bool Gear::isTradeable(void)
{
    switch( type )
    {
    case gtHelmet:
        return true;
    case gtSuit:
        return database::Suit::getRecord( id )->trade;
    case gtRig:
        return database::Rig::getRecord( id )->trade;
    case gtCanopy:
        return database::Canopy::getRecord( id )->trade;
    }
    return 0.0f;
}


void Gear::updateNameFromId()
{
        const char* getName = "";
        switch (type) {
                case gtSuit:       getName = database::Suit::getRecord(id)->name.c_str(); break;
                case gtCanopy:     getName = database::Canopy::getRecord(id)->name.c_str(); break;
                case gtRig:        getName = database::Rig::getRecord(id)->name.c_str(); break;
                case gtHelmet:     getName = database::Helmet::getRecord(id)->name.c_str(); break;
        }
        strncpy(name, getName, 64);
        name[63] = 0;
}


void Gear::updateIdFromName()
{
        switch (type) {
                case gtSuit:       id = database::Suit::getRecordId(name); break;
                case gtCanopy:     id = database::Canopy::getRecordId(name); break;
                case gtRig:        id = database::Rig::getRecordId(name); break;
                case gtHelmet:     id = database::Helmet::getRecordId(name); break;
                case gtUnequipped: id = -1; break;
        }

        if (id == -1) {
                type = gtUnequipped;
        }
}


Gear::Gear(GearType t, unsigned int i) 
: type(t), id(i), state(1), age(0)
{
        updateNameFromId();
}
