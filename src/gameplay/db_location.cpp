#include "headers.h"

#include "shared/ccor.h"
#include "engine/configreader.h"
#include "database.h"
#include "casting.h"
#include "version.h"
#include "gameplay.h"

using namespace ccor;
using namespace database;
using namespace std;

/**
 * sub-database : lists of local assets
 */

static LocationInfo::AssetInfo dropzoneAssets[] = 
{
    { "CrowdMale01", "./res/x/crowdmale01.x" },
    { "DropzoneCutscene", "./res/dropzone/cutscene.x" },
    { "RoadTraffic", "./res/dropzone/roadtraffic.x" },
    { "Airplane01", "./res/dropzone/airplane01.x" },
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo ostankinoAssets[] = 
{
    { "CrowdMale01", "./res/x/crowdmale01.x" },
    { "TrafficTrain01", "./res/ostankino/traffic_train.x" },
    { "TrafficBus01", "./res/ostankino/traffic_bus.x" },
    { "TrafficCars01", "./res/ostankino/traffic_cars.x" },
    { "OstankinoCutscene", "./res/ostankino/cutscene.x" },
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo caveAssets[] = 
{
    { "Bush01", "./res/cave/bush01.x" },
    { "Bush02", "./res/cave/bush02.x" },
    { "Tree01", "./res/cave/tree01.x" },
    { "Tree02", "./res/cave/tree02.x" },
    { "Tree03", "./res/cave/tree03.x" },
    { "Tree04", "./res/cave/tree04.x" },
    { "Tree05", "./res/cave/tree05.x" },
    { "Tree06", "./res/cave/tree06.x" },
    { "Birds", "./res/cave/birds.x" },
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "CaveCutscene", "./res/cave/cutscene.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo trollveggenAssets[] = 
{   
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "TrafficCars01", "./res/trollveggen/traffic_car.x" },
    { "TrafficTrain01", "./res/trollveggen/traffic_train.x" },
    { "TrollveggenCutscene", "./res/trollveggen/cutscene.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo royalGorgeBridgeAssets[] = 
{
    { "CrowdMale01", "./res/x/crowdmale01.x" },
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "RoyalGorgeBridgeCutscene", "./res/royalgorge/cutscene.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo kvlyAssets[] = 
{
    { "KVLYCutscene", "./res/dropzone/cutscene.x" },
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo kjeragAssets[] = 
{   
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "KjeragCutscene", "./res/kjerag/cutscene.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo angelFallsAssets[] = 
{   
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "AngelFallsCutscene", "./res/angelfalls/cutscene.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};

static LocationInfo::AssetInfo elCapitanAssets[] = 
{   
    { "Helicopter01", "./res/x/helicopter01.x" },
    { "AngelFallsCutscene", "./res/angelfalls/cutscene.x" },
    { "Airplane02", "./res/x/cessna_static.x" },
};


/**
 * sub-database : lists of local textures
 */

static const char* dropzoneTextures[] =
{
    "./res/x/textures/crowdmale01.dds",
    "./res/x/textures/crowdmale02.dds",
    "./res/x/textures/crowdmale03.dds",
    "./res/x/textures/crowdmale04.dds",
    "./res/x/textures/crowdmale05.dds",
    "./res/x/textures/crowdmale06.dds"
};

static const char* ostankinoTextures[] =
{
    "./res/x/textures/crowdmale01.dds",
    "./res/x/textures/crowdmale02.dds",
    "./res/x/textures/crowdmale03.dds",
    "./res/x/textures/crowdmale04.dds",
    "./res/x/textures/crowdmale05.dds",
    "./res/x/textures/crowdmale06.dds"
};

static const char* royalGorgeBridgeTextures[] =
{
    "./res/x/textures/crowdmale01.dds",
    "./res/x/textures/crowdmale02.dds",
    "./res/x/textures/crowdmale03.dds",
    "./res/x/textures/crowdmale04.dds",
    "./res/x/textures/crowdmale05.dds",
    "./res/x/textures/crowdmale06.dds"
};

/**
 * sub-database : lists of exit points
 */


static LocationInfo::ExitPoint dropzoneExitPoints[] = 
{
    { 279, "Enclosure01", 1.0f },
};

static LocationInfo::ExitPoint ostankinoExitPoints[] = 
{
    { 122, "Enclosure01", 6.0f },
    { 123, "Enclosure02", 7.0f },
    { 124, "Enclosure03", 8.0f },
    { 238, "Enclosure04", 1.0f },
};

static LocationInfo::ExitPoint caveExitPoints[] = 
{
    { 126, "Enclosure01", 6.0f }, 
};

static LocationInfo::ExitPoint trollveggenExitPoints[] = 
{
    { 235, "Enclosure01", 15.0f },
    { 233, "Enclosure02", 15.0f },
    { 269, "Enclosure03", 15.0f },
    { 272, "Enclosure04", 10.0f },
    { 275, "Enclosure05", 10.0f }, 
    { 276, "Enclosure06", 10.0f }, 
};

static LocationInfo::ExitPoint royalGorgeBridgeExitPoints[] = 
{
    { 229, "Enclosure01", 4.0f },
};

static LocationInfo::ExitPoint kvlyExitPoints[] = 
{
    { 769, "Enclosure01", 8.0f },
};

static LocationInfo::ExitPoint kjeragExitPoints[] = 
{
    { 785, "Enclosure01", 8.0f },
    { 790, "Enclosure02", 8.0f },
    { 793, "Enclosure04", 8.0f },
    { 796, "Enclosure03", 8.0f },
};

static LocationInfo::ExitPoint angelFallsExitPoints[] = 
{
    { 826, "Enclosure01", 8.0f },
    { 827, "Enclosure02", 8.0f },
};

static LocationInfo::ExitPoint elCapitanExitPoints[] = 
{
    { 846, "Enclosure01", 8.0f },
};


/**
 * sub-database : lists of grass schemes
 */

static engine::GrassSpecie caveSpecies[] = 
{
    { "Salvia Divinorum", { Vector2f(0.25f,0.25f), Vector2f(0.25f,0.0f), Vector2f(0.0f,0.0f), Vector2f(0.0f,0.25f) }, Vector2f(25.0f,25.0f), Vector2f(5.0f,5.0f), 0.2f },
    { "Calea Zakatetichi", { Vector2f(0.5f,0.25f), Vector2f(0.5f,0.0f), Vector2f(0.25f,0.0f), Vector2f(0.25f,0.25f) }, Vector2f(35.0f,35.0f), Vector2f(7.5f,7.5f), 0.2f },
    { "Actaea Spicata", { Vector2f(0.75f,0.25f), Vector2f(0.75f,0.0f), Vector2f(0.5f,0.0f), Vector2f(0.5f,0.25f) }, Vector2f(25.0f,25.0f), Vector2f(2.5f,2.5f), 5.0f },
    { "Ranunculus Acris", { Vector2f(0.125f,0.75f), Vector2f(0.125f,0.25f), Vector2f(0.0f,0.25f), Vector2f(0.0f,0.75f) }, Vector2f(25.0f,95.0f), Vector2f(10.0f,25.0f), 0.1f },
    { "Calea Zinkwichi", { Vector2f(0.375f,0.75f), Vector2f(0.375f,0.25f), Vector2f(0.125f,0.25f), Vector2f(0.125f,0.75f) }, Vector2f(50.0f,75.0f), Vector2f(5.0f,7.0f), 0.1f },
    { "Cannabis Ruderalis", { Vector2f(0.625f,0.75f), Vector2f(0.625f,0.25f), Vector2f(0.375f,0.25f), Vector2f(0.375f,0.75f) }, Vector2f(50.0f,75.0f), Vector2f(5.0f,7.0f), 0.4f },
    { "Caltha Palustris", { Vector2f(0.875f,0.75f), Vector2f(0.875f,0.25f), Vector2f(0.625f,0.25f), Vector2f(0.625f,0.75f) }, Vector2f(20.0f,40.0f), Vector2f(8.0f,16.0f), 1.0f },
    { "Anemone Nemorosa", { Vector2f(0.25f,1.0f), Vector2f(0.25f,0.75f), Vector2f(0.0f,0.75f), Vector2f(0.0f,1.0f) }, Vector2f(40.0f,20.0f), Vector2f(2.0f,2.0f), 0.3f },
    { NULL, { Vector2f(0,0), Vector2f(0,0), Vector2f(0,0), Vector2f(0,0) }, Vector2f(0,0), Vector2f(0,0), 0 }
};

static engine::GrassScheme caveGrass =
{
    1000.0f,
    Vector4f( 1.0f, 1.0f, 1.0f, 1.0f ),
    caveSpecies
};

static engine::GrassSpecie kvlySpecies[] = 
{
    { "Zea mays", { Vector2f(1.0f,1.0f), Vector2f(1.0f,0.0f), Vector2f(0.0f,0.0f), Vector2f(0.0f,1.0f) }, Vector2f(75.0f,150.0f), Vector2f(12.5f,25.0f), 1.0f },
    { NULL, { Vector2f(0,0), Vector2f(0,0), Vector2f(0,0), Vector2f(0,0) }, Vector2f(0,0), Vector2f(0,0), 0 }
};

static engine::GrassScheme kvlyGrass =
{
    2000.0f,
    Vector4f( 0.75f, 0.75f, 0.75f, 1.0f ),
    kvlySpecies
};

/**
 * weather database
 */

static LocationInfo::Weather dropzoneWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.00000175f, Vector4f( 0.84f,0.86f,0.88f,1.0f ), 1.0f,  1.0f,  { "./res/dropzone/clearday.ba", 1000, 2500000 } },
    { ::wtVariable,  engine::fogExp2, 0.0000025f,  Vector4f( 0.84f,0.85f,0.88f,1.0f ), 0.95f, 0.95f, { "./res/dropzone/cloudyday.ba", 1000, 2500000 } },
    { ::wtCloudy,    engine::fogExp,  0.000005f,   Vector4f( 0.84f,0.85f,0.88f,1.0f ), 0.85f, 0.9f,  { "./res/dropzone/cloudyday.ba", 1000, 2500000 } },
    { ::wtLightRain, engine::fogExp2, 0.00001f,   Vector4f( 0.65f,0.66f,0.68f,1.0f ), 0.75f, 0.85f, { "./res/dropzone/rainyday.ba", 1000, 2500000 } },
    { ::wtHardRain,  engine::fogExp2, 0.000025f,   Vector4f( 0.65f,0.66f,0.68f,1.0f ), 0.75f, 0.85f, { "./res/dropzone/rainyday.ba", 1000, 2500000 } },
    { ::wtThunder,   engine::fogExp2, 0.00005f,    Vector4f( 0.65f,0.66f,0.68f,1.0f ), 0.65f, 0.80f, { "./res/dropzone/rainyday.ba", 1000, 2500000 } },
};

static LocationInfo::Weather ostankinoWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.0000035f, Vector4f( 0.84f,0.86f,0.88f,1.0f ), 1.0f,  1.0f,  { "./res/ostankino/clearday.ba", 1000, 1500000 } },
    { ::wtVariable,  engine::fogExp2, 0.000005f,  Vector4f( 0.65f,0.66f,0.67f,1.0f ), 0.95f, 0.95f, { "./res/ostankino/cloudyday.ba", 1000, 1500000 } },
    { ::wtCloudy,    engine::fogExp,  0.00001f,   Vector4f( 0.65f,0.66f,0.67f,1.0f ), 0.85f, 0.9f,  { "./res/ostankino/cloudyday.ba", 1000, 1500000 } },
    { ::wtLightRain, engine::fogExp2, 0.00002f,   Vector4f( 0.65f,0.66f,0.68f,1.0f ), 0.75f, 0.85f, { "./res/ostankino/rainyday.ba", 1000, 1500000 } },
    { ::wtHardRain,  engine::fogExp2, 0.00005f,   Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/ostankino/stormday.ba", 1000, 1500000 } },
    { ::wtThunder,   engine::fogExp2, 0.0001f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/ostankino/stormday.ba", 1000, 1500000 } },
};

static LocationInfo::Weather trollveggenWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.000001f,   Vector4f( 0.84f,0.86f,0.88f,1.0f ), 1.0f,  1.0f,  { "./res/trollveggen/clearday.ba", 1000, 2000000 } },
    { ::wtVariable,  engine::fogExp2, 0.0000015f,  Vector4f( 0.72f,0.75f,0.78f,1.0f ), 0.95f, 0.95f, { "./res/trollveggen/cloudyday.ba", 1000, 2000000 } },
    { ::wtCloudy,    engine::fogExp,  0.00000225f, Vector4f( 0.72f,0.75f,0.78f,1.0f ), 0.85f, 0.9f,  { "./res/trollveggen/cloudyday.ba", 1000, 2000000 } },
    { ::wtLightRain, engine::fogExp2, 0.0000035f,  Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/trollveggen/rainyday.ba", 1000, 2000000 } },
    { ::wtHardRain,  engine::fogExp2, 0.000005f,   Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/trollveggen/rainyday.ba", 1000, 2000000 } },
    { ::wtThunder,   engine::fogExp2, 0.0000075f,  Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/trollveggen/rainyday.ba", 1000, 2000000 } },
};

static LocationInfo::Weather caveWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.0000025f, Vector4f( 0.51f,0.59f,0.65f,1.0f ), 1.0f,  1.0f,  { "./res/cave/clearday.ba", 1000, 2000000 } },
    { ::wtVariable,  engine::fogExp2, 0.0000045f, Vector4f( 0.51f,0.59f,0.65f,1.0f ), 0.95f, 0.95f, { "./res/cave/cloudyday.ba", 1000, 2000000 } },
    { ::wtCloudy,    engine::fogExp,  0.00001f,   Vector4f( 0.51f,0.59f,0.65f,1.0f ), 0.85f, 0.9f,  { "./res/cave/cloudyday.ba", 1000, 2000000 } },
    { ::wtLightRain, engine::fogExp2, 0.0001f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/cave/rainyday.ba", 1000, 2000000 } },
    { ::wtHardRain,  engine::fogExp2, 0.0002f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/cave/rainyday.ba", 1000, 2000000 } },
    { ::wtThunder,   engine::fogExp2, 0.0002f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/cave/rainyday.ba", 1000, 2000000 } },
};

static LocationInfo::Weather bridgeWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.000002f,  Vector4f( 0.84f,0.86f,0.88f,1.0f ), 1.0f,  1.0f,  { "./res/royalgorge/clearday.ba", 1000, 2000000 } },
    { ::wtVariable,  engine::fogExp2, 0.0000025f, Vector4f( 0.65f,0.66f,0.67f,1.0f ), 0.95f, 0.95f, { "./res/royalgorge/cloudyday.ba", 1000, 2000000 } },
    { ::wtCloudy,    engine::fogExp2, 0.000003f,  Vector4f( 0.65f,0.66f,0.67f,1.0f ), 0.85f, 0.9f,  { "./res/royalgorge/cloudyday.ba", 1000, 2000000 } },
    { ::wtLightRain, engine::fogExp2, 0.000005f,  Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/royalgorge/rainyday.ba", 1000, 2000000 } },
    { ::wtHardRain,  engine::fogExp2, 0.0000075f, Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/royalgorge/rainyday.ba", 1000, 2000000 } },
    { ::wtThunder,   engine::fogExp2, 0.00001f,   Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/royalgorge/rainyday.ba", 1000, 2000000 } },
};

static LocationInfo::Weather kvlyWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.000002f,  Vector4f( 0.84f,0.86f,0.88f,1.0f ), 1.0f,  1.0f,  { "./res/royalgorge/clearday.ba", 1000, 2000000 } },
    { ::wtVariable,  engine::fogExp2, 0.0000025f, Vector4f( 0.65f,0.66f,0.67f,1.0f ), 0.95f, 0.95f, { "./res/royalgorge/cloudyday.ba", 1000, 2000000 } },
    { ::wtCloudy,    engine::fogExp2, 0.000003f,  Vector4f( 0.65f,0.66f,0.67f,1.0f ), 0.85f, 0.9f,  { "./res/royalgorge/cloudyday.ba", 1000, 2000000 } },
    { ::wtLightRain, engine::fogExp2, 0.000005f,  Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/royalgorge/rainyday.ba", 1000, 2000000 } },
    { ::wtHardRain,  engine::fogExp2, 0.0000075f, Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/royalgorge/rainyday.ba", 1000, 2000000 } },
    { ::wtThunder,   engine::fogExp2, 0.00001f,   Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/royalgorge/rainyday.ba", 1000, 2000000 } },
};

static LocationInfo::Weather kjeragWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.000001f,   Vector4f( 0.84f,0.86f,0.88f,1.0f ), 1.0f,  1.0f,  { "./res/trollveggen/clearday.ba", 1000, 2000000 } },
    { ::wtVariable,  engine::fogExp2, 0.0000015f,  Vector4f( 0.72f,0.75f,0.78f,1.0f ), 0.95f, 0.95f, { "./res/trollveggen/cloudyday.ba", 1000, 2000000 } },
    { ::wtCloudy,    engine::fogExp,  0.00000225f, Vector4f( 0.72f,0.75f,0.78f,1.0f ), 0.85f, 0.9f,  { "./res/trollveggen/cloudyday.ba", 1000, 2000000 } },
    { ::wtLightRain, engine::fogExp2, 0.0000035f,  Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/trollveggen/rainyday.ba", 1000, 2000000 } },
    { ::wtHardRain,  engine::fogExp2, 0.000005f,   Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/trollveggen/rainyday.ba", 1000, 2000000 } },
    { ::wtThunder,   engine::fogExp2, 0.0000075f,  Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/trollveggen/rainyday.ba", 1000, 2000000 } },
};

static LocationInfo::Weather angelFallsWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.00000035f, Vector4f( 0.51f,0.59f,0.65f,1.0f ), 1.0f,  1.0f,  { "./res/angelfalls/clearday.ba", 1000, 3000000 } },
    { ::wtVariable,  engine::fogExp2, 0.00000075f, Vector4f( 0.51f,0.59f,0.65f,1.0f ), 0.95f, 0.95f, { "./res/angelfalls/variableday.ba", 1000, 3000000 } },
    { ::wtCloudy,    engine::fogExp2, 0.00000125f,   Vector4f( 0.51f,0.59f,0.65f,1.0f ), 0.85f, 0.9f,  { "./res/angelfalls/cloudyday.ba", 1000, 3000000 } },
    { ::wtLightRain, engine::fogExp,  0.0000125f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/angelfalls/rainyday.ba", 1000, 3000000 } },
    { ::wtHardRain,  engine::fogExp,  0.000025f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/angelfalls/rainyday.ba", 1000, 3000000 } },
    { ::wtThunder,   engine::fogExp,  0.00005f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/angelfalls/rainyday.ba", 1000, 3000000 } },
};

static LocationInfo::Weather elCapitanWeatherOptions[] = 
{
    { ::wtSunny,     engine::fogExp2, 0.00000035f, Vector4f( 0.51f,0.59f,0.65f,1.0f ), 1.0f,  1.0f,  { "./res/angelfalls/clearday.ba", 1000, 3000000 } },
    { ::wtVariable,  engine::fogExp2, 0.00000075f, Vector4f( 0.51f,0.59f,0.65f,1.0f ), 0.95f, 0.95f, { "./res/angelfalls/variableday.ba", 1000, 3000000 } },
    { ::wtCloudy,    engine::fogExp2, 0.00000125f,   Vector4f( 0.51f,0.59f,0.65f,1.0f ), 0.85f, 0.9f,  { "./res/angelfalls/cloudyday.ba", 1000, 3000000 } },
    { ::wtLightRain, engine::fogExp,  0.0000125f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/angelfalls/rainyday.ba", 1000, 3000000 } },
    { ::wtHardRain,  engine::fogExp,  0.000025f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.75f, 0.85f, { "./res/angelfalls/rainyday.ba", 1000, 3000000 } },
    { ::wtThunder,   engine::fogExp,  0.00005f,    Vector4f( 0.37f,0.40f,0.45f,1.0f ), 0.65f, 0.80f, { "./res/angelfalls/rainyday.ba", 1000, 3000000 } },
};


/**
 * reverberation datatbase
 */
/*
LocationInfo::Reverberation dropzoneReverberation = { 1.0f, 0.33f, 0.125f, 0.25f };
LocationInfo::Reverberation moscowReverberation = { 1.0f, 0.5f, 0.33f, 0.66f };
LocationInfo::Reverberation caveReverberation = { 1.0f, 0.9f, 1.5f, 0.75f };
LocationInfo::Reverberation trollveggenReverberation = { 1.0f, 0.419f, 1.740f, 0.158f };
LocationInfo::Reverberation bridgeReverberation = { 1.0f, 0.5f, 1.0f, 0.9f };
LocationInfo::Reverberation kvlyReverberation = { 1.0f, 0.5f, 1.0f, 0.9f };
*/

/**
 * main location database
 */
extern TournamentInfo dropzoneTournaments[];
extern TournamentInfo ostankinoTournaments[];

static LocationInfo gStaticLocations[] = 
{
    // 0 - home dropzone
    { 
        "N88", 0, 0, "LGD00", "./res/thumbnails/000.dds", true,
        0.0f, // no stay-in-location fee
        0, // no boogies
        0, // no festivals
        0, // no climbings
        639, // smokeball event
        824, // community event
        { 1000,1000,1000,1000,1000 },        
        { "./res/dropzone/dropzone.ba", 10, 1100000 },
        { "./res/dropzone/gamedata.ba", 0, 0 },
        std::vector<LocationInfo::AssetInfo>(dropzoneAssets, dropzoneAssets + 6),
        std::vector<string>(dropzoneTextures, dropzoneTextures + 6),
        { 0.03f, 0.73f },
        { NULL, "", "", "", "", 0.0f, 0.0f },
        std::vector<LocationInfo::ExitPoint>(dropzoneExitPoints, dropzoneExitPoints + 1),
        casting::castingCallbackDropzone,
        { 100.0f, 20000.0f, 1.0f, 1.25f, 0.9f, 1.0f, "./res/sounds/footsteps/metal/walk.ogg", "./res/sounds/footsteps/metal/turn.ogg" },
        std::vector<LocationInfo::Weather>(dropzoneWeatherOptions, dropzoneWeatherOptions + 6),
        { 1.0f, 0.33f, 0.125f, 0.25f },
        std::vector<TournamentInfo>()
    },
    // 1 - ostankino tv tower
    { 
        "N89", 1243, 179, "LGD01", "./res/thumbnails/100.dds", true,
        20.0f,
        0, // no boogies
        522, // festival event name
        0, // no climbings
        0, // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },        
        { "./res/ostankino/ostankino.ba", 10, 1000000 },
        { "./res/ostankino/gamedata.ba", 0, 0 },
        std::vector<LocationInfo::AssetInfo>(ostankinoAssets, ostankinoAssets + 7),
        std::vector<string>(ostankinoTextures, ostankinoTextures + 6),
        { 0.03f, 0.73f },
        { NULL, "", "", "", "", 0.0f, 0.0f },
        std::vector<LocationInfo::ExitPoint>(ostankinoExitPoints, ostankinoExitPoints + 4),
        casting::castingCallbackOstankino,
        { 100.0f, 20000.0f, 1.0f, 1.25f, 0.9f, 1.0f, "./res/sounds/footsteps/metal/walk.ogg", "./res/sounds/footsteps/metal/turn.ogg" },
        std::vector<LocationInfo::Weather>(ostankinoWeatherOptions, ostankinoWeatherOptions + 6),
        { 1.0f, 0.5f, 0.33f, 0.66f },
        std::vector<TournamentInfo>()
    },
    // 2 - cave of swallows
    { 
        "N90", 460, 394, "LGD02", "./res/thumbnails/400.dds", false,
        15.0f,
        254, // boogie event
        0, // no festivals
        0, // no climbings
        0, // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },
        { "./res/cave/caveofswallows.ba", 10, 1000000 },
        { "./res/cave/gamedata.ba", 0, 0 },
        std::vector<LocationInfo::AssetInfo>(caveAssets, caveAssets + 12),
        std::vector<string>(),
        { 0.025f, 0.65f },
        { &caveGrass, "mexicanvegetation", "./res/particles/mexicanvegetation.dds", "./usr/cache/caveofswallows.grass", "GrassSurface", 2000, 2500 },
        std::vector<LocationInfo::ExitPoint>(caveExitPoints, caveExitPoints + 1),
        casting::castingCallbackCaveOfSwallows,
        { 100.0f, 20000.0f, 1.0f, 1.1f, 0.9f, 1.5f, "./res/sounds/footsteps/rock/walk.ogg", "./res/sounds/footsteps/rock/turn.ogg" },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::Weather, caveWeatherOptions),
        { 1.0f, 0.9f, 1.5f, 0.75f },
        std::vector<TournamentInfo>()
    },
    // 3 - trollveggen
    { 
        "N91", 1047, 153, "LGD03", "./res/thumbnails/300.dds", true,
        30.0f,
        253, // boogie event
        0,   // no festivals
        567, // climbing event
        0,   // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },
        { "./res/trollveggen/trollveggen.ba", 10, 1000000 },
        { "./res/trollveggen/gamedata.ba", 0, 0 },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::AssetInfo, trollveggenAssets),
        std::vector<string>(),
        { 0.03f, 0.63f },
        { NULL, "", "", "", "", 0.0f, 0.0f },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::ExitPoint, trollveggenExitPoints),
        casting::castingCallbackTrollveggen,
        { 100.0f, 20000.0f, 1.0f, 1.1f, 0.9f, 1.5f, "./res/sounds/footsteps/rock/walk.ogg", "./res/sounds/footsteps/rock/turn.ogg" },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::Weather, trollveggenWeatherOptions),
        { 1.0f, 0.419f, 1.740f, 0.158f },
        std::vector<TournamentInfo>()
    },
    // 4 - royal gorge bridge
    { 
        "N92", 398, 303, "LGD04", "./res/thumbnails/500.dds", false,
        25.0f,
        0,   // no boogies
        596, // festival event
        0,   // no climbings
        0,   // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },
        { "./res/royalgorge/royalgorgebridge.ba", 10, 1000000 },
        { "./res/royalgorge/gamedata.ba", 0, 0 },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::AssetInfo, royalGorgeBridgeAssets),
        INIT_STD_VECTOR_WITH_ARRAY(string, royalGorgeBridgeTextures),
        { 0.0275f, 0.8f },
        { NULL, "", "", "", "", 0.0f, 0.0f },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::ExitPoint, royalGorgeBridgeExitPoints),
        casting::castingCallbackRoyalGorgeBridge,
        { 100.0f, 20000.0f, 1.0f, 1.25f, 0.9f, 1.0f, "./res/sounds/footsteps/metal/walk.ogg", "./res/sounds/footsteps/metal/turn.ogg" },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::Weather, bridgeWeatherOptions),
        { 1.0f, 0.5f, 1.0f, 0.9f },
        std::vector<TournamentInfo>()
    },
    // 5 - kvly-tv mast
    { 
        "N768", 464, 256, "LGD05", "./res/thumbnails/600.dds", true,
        25.0f,
        773, // boogie event
        0, // no festival
        0, // no climbings
        0, // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },
        { "./res/kvly/kvly.ba", 10, 1000000 },
        { "./res/kvly/gamedata.ba", 0, 0 },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::AssetInfo, kvlyAssets),
        std::vector<string>(), // no extra textures
        { 0.0275f, 0.8f },
        { &kvlyGrass, "corn", "./res/particles/corn.dds", "./usr/cache/kvly.grass", "GrassSurface", 7500, 10000 },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::ExitPoint, kvlyExitPoints),
        casting::castingCallbackKVLY,
        { 100.0f, 20000.0f, 1.0f, 1.25f, 0.9f, 1.0f, "./res/sounds/footsteps/metal/walk.ogg", "./res/sounds/footsteps/metal/turn.ogg" },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::Weather, kvlyWeatherOptions),
        { 1.0f, 0.5f, 1.0f, 0.9f },
        std::vector<TournamentInfo>()
    },
    // 6 - kjerag
    { 
        "N784", 1094, 185, "LGD06", "./res/thumbnails/700.dds", true,
        25.0f,
        799, // boogie event
        0, // no festivals
        0, // no climbings
        0, // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },
        { "./res/kjerag/kjerag.ba", 10, 1000000 },
        { "./res/kjerag/gamedata.ba", 0, 0 },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::AssetInfo, kjeragAssets),
        std::vector<string>(),
        { 0.02825f, 0.675f },
        { NULL, "", "", "", "", 0.0f, 0.0f },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::ExitPoint, kjeragExitPoints),
        casting::castingCallbackKjerag,
        { 100.0f, 20000.0f, 1.0f, 1.1f, 0.9f, 1.5f, "./res/sounds/footsteps/rock/walk.ogg", "./res/sounds/footsteps/rock/turn.ogg" },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::Weather, kjeragWeatherOptions),
        { 1.0f, 0.419f, 1.740f, 0.158f },
        std::vector<TournamentInfo>()
    },
    // 7 - angel falls
    { 
        "N832", 328*2, 241*2, "LGD07", "./res/thumbnails/800.dds", true,
        25.0f,
        842, // angel falls boogie
        0, // no festivals
        0, // no climbings
        0, // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },
        { "./res/angelfalls/angelfalls.ba", 10, 1500000 },
        { "./res/angelfalls/gamedata.ba", 0, 0 },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::AssetInfo, angelFallsAssets),
        std::vector<string>(),
        { 0.025f, 0.5f },
        { NULL, "", "", "", "", 0.0f, 0.0f },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::ExitPoint, angelFallsExitPoints),
        casting::castingCallbackAngelFalls,
        { 100.0f, 20000.0f, 1.0f, 1.1f, 0.9f, 1.5f, "./res/sounds/footsteps/rock/walk.ogg", "./res/sounds/footsteps/rock/turn.ogg" },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::Weather, angelFallsWeatherOptions),
        { 1.0f, 0.419f, 1.740f, 0.158f },
        std::vector<TournamentInfo>()
    },
    // 8 - el capitan
    { 
        "N846", 340, 301, "LGD08", "./res/thumbnails/900.dds", true,
        25.0f,
        0, // el capitan boogie
        0, // no festivals
        0, // no climbings
        0, // no smokeballs
        824, // community event
        { 1000,1000,1000,1000,1000 },
        { "./res/elcapitan/elcapitan.ba", 10, 1500000 },
        { "./res/elcapitan/gamedata.ba", 0, 0 },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::AssetInfo, elCapitanAssets),
        std::vector<string>(),
        { 0.025f, 0.5f },
        { NULL, "", "", "", "", 0.0f, 0.0f },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::ExitPoint, elCapitanExitPoints),
        casting::castingCallbackElCapitan,
        { 100.0f, 20000.0f, 1.0f, 1.1f, 0.9f, 1.5f, "./res/sounds/footsteps/rock/walk.ogg", "./res/sounds/footsteps/rock/turn.ogg" },
        INIT_STD_VECTOR_WITH_ARRAY(LocationInfo::Weather, elCapitanWeatherOptions),
        { 1.0f, 0.419f, 1.740f, 0.158f },
        std::vector<TournamentInfo>()
    },
    { "", 0, 0 }
};

std::vector<LocationInfo*> gLocations;


unsigned int LocationInfo::getNumRecords(void)
{
    return gLocations.size();
}

LocationInfo* LocationInfo::getRecord(unsigned int id)
{
    assert( id >= 0 && id < getNumRecords() );
    return gLocations[id];
}


void LocationInfo::loadLocations(const char* fileName)
{
    int i = 0;
    while (!gStaticLocations[i].nameId.empty()) {
        gLocations.push_back(&gStaticLocations[i]);
        i++;
    }

    ConfigReader cfg(fileName);
    if (!cfg.IsFileOpen()) {
        return;
    }

    while (cfg.PeekKey()) {
        if (string("Location") == cfg.PeekKey()) {
            string locationFile = cfg.ReadString("Location");
            locationFile.insert(0, "./res/");
            string missionsFile(locationFile);
            locationFile.append("/location.cfg");
            missionsFile.append("/missions.cfg");

            LocationInfo* location = loadLocation(locationFile.c_str());
            if (location != 0) {
                gLocations.push_back(location);
                database::TournamentInfo::loadMissions(location, missionsFile.c_str());
            }
        } else {
            getCore()->logMessage("Error: location file %s is malformed. Keyword: %s", fileName, cfg.PeekKey());
            break;
        }
    }
}


WeatherType WeatherTypeFromString(const char* str) {
    string name(str);
    if (name == "Sunny") { return wtSunny; }
    else if (name == "Variable") { return wtVariable; }
    else if (name == "Cloudy") { return wtCloudy; }
    else if (name == "LightRain") { return wtLightRain; }
    else if (name == "HardRain") { return wtHardRain; }
    else if (name == "Thunder") { return wtThunder; }
    else return wtSunny;
}


CastingCallback CastingCallbackFromString(const char* str) {
    string name(str);

    if (name == "castingCallbackTestLevel") { return casting::castingCallbackTestLevel; }
    else if (name == "castingCallbackDropzone") { return casting::castingCallbackDropzone; }
    else if (name == "castingCallbackOstankino") { return casting::castingCallbackOstankino; }
    else if (name == "castingCallbackCaveOfSwallows") { return casting::castingCallbackCaveOfSwallows; }
    else if (name == "castingCallbackTrollveggen") { return casting::castingCallbackTrollveggen; }
    else if (name == "castingCallbackRoyalGorgeBridge") { return casting::castingCallbackRoyalGorgeBridge; }
    else if (name == "castingCallbackKVLY") { return casting::castingCallbackKVLY; }
    else if (name == "castingCallbackKjerag") { return casting::castingCallbackKjerag; }
    else if (name == "castingCallbackAngelFalls") { return casting::castingCallbackAngelFalls; }
    else if (name == "castingCallbackElCapitan") { return casting::castingCallbackElCapitan; }
    else return casting::castingCallbackTestLevel;
}

LocationInfo* LocationInfo::loadLocation(const char* fileName)
{
    LocationInfo* locationInfo = new LocationInfo();
    memset(locationInfo, 0, sizeof(LocationInfo));

    ConfigReader cfg(fileName);
    if (!cfg.IsFileOpen()) {
        delete locationInfo;
        return 0;
    }

    while (cfg.PeekKey()) {
        string key = cfg.PeekKey();
        if (key == "NameId") { locationInfo->nameId = Gameplay::iLanguage->parseTranslationString(cfg.ReadString("NameId")); }
        else if (key == "WorldX") { locationInfo->worldX = cfg.ReadInt32("WorldX"); }
        else if (key == "WorldY") { locationInfo->worldY = cfg.ReadInt32("WorldY"); }
        else if (key == "GameData") { locationInfo->gameData = cfg.ReadString("GameData"); }
        else if (key == "Thumbnail") { locationInfo->thumbnail = cfg.ReadString("Thumbnail"); }
        else if (key == "Wind") { locationInfo->wind = cfg.ReadBool("Wind"); }
        else if (key == "StayFee") { locationInfo->stayFee = cfg.ReadFloat("StayFee"); }

        else if (key == "BoogieId") { locationInfo->boogieId = cfg.ReadInt32("BoogieId"); }
        else if (key == "FestivalId") { locationInfo->festivalId = cfg.ReadInt32("FestivalId"); }
        else if (key == "ClimbindId") { locationInfo->climbingId = cfg.ReadInt32("ClimbindId"); }
        else if (key == "SmokeballId") { locationInfo->smokeballId = cfg.ReadInt32("SmokeballId"); }
        else if (key == "CommunityId") { locationInfo->communityId = cfg.ReadInt32("CommunityId"); }

        else if (key == "MaxActors") { locationInfo->physicsLimits.numActors = cfg.ReadUInt32("MaxActors"); }
        else if (key == "MaxBodies") { locationInfo->physicsLimits.numBodies = cfg.ReadUInt32("MaxBodies"); }
        else if (key == "MaxStaticShapes") { locationInfo->physicsLimits.numStaticShapes = cfg.ReadUInt32("MaxStaticShapes"); }
        else if (key == "MaxDynamicShapes") { locationInfo->physicsLimits.numDynamicShapes = cfg.ReadUInt32("MaxDynamicShapes"); }
        else if (key == "MaxJoints") { locationInfo->physicsLimits.numJoints = cfg.ReadUInt32("MaxJoints"); }

        else if (key == "LevelFile") { locationInfo->stage.resource = cfg.ReadString("LevelFile"); }
        else if (key == "GameDataFile") { locationInfo->extras.resource = cfg.ReadString("GameDataFile"); }
        else if (key == "ZNear") { locationInfo->stage.zNear = cfg.ReadFloat("ZNear"); }
        else if (key == "ZFar") { locationInfo->stage.zFar = cfg.ReadFloat("ZFar"); }

        else if (key == "Asset") { 
            LocationInfo::AssetInfo asset;

            cfg.BeginGroup("Asset");
            asset.name = cfg.ReadString("Name");
            asset.resource = cfg.ReadString("File");
            cfg.EndGroup();

            locationInfo->localAssets.push_back(asset);
        }

        else if (key == "Texture") { locationInfo->localTextures.push_back(cfg.ReadString("Texture")); }

        else if (key == "BrightnessPass") { locationInfo->afterFx.brightPass = cfg.ReadFloat("BrightnessPass"); }
        else if (key == "Bloom") { locationInfo->afterFx.bloom = cfg.ReadFloat("Bloom"); }

// TODO
//;{ &kvlyGrass, "corn", "./res/particles/corn.dds", "./usr/cache/kvly.grass", "GrassSurface", 7500, 10000 },
//;Grass {
//;	Scheme
//;	TextureName
//;	TextureFile
//;	Cache
//;	Templ
//;	FadeStart
//;	FadeEnd
//;}

        else if (key == "ExitPoint") { 
            LocationInfo::ExitPoint exit;

            cfg.BeginGroup("ExitPoint");
            exit.nameId = cfg.ReadUInt32("NameId");
            exit.extras = cfg.ReadString("ClumpName");
            exit.delay = cfg.ReadFloat("Delay");
            cfg.EndGroup();

            locationInfo->exitPoints.push_back(exit);
        }

        else if (key == "CastingCallback") { locationInfo->castingCallback = CastingCallbackFromString(cfg.ReadString("CastingCallback")); }

        else if (key == "Footsteps") { 
            cfg.BeginGroup("Footsteps");
            locationInfo->footsteps.refdist = cfg.ReadFloat("RefDist");
            locationInfo->footsteps.maxdist = cfg.ReadFloat("MaxDist");
            locationInfo->footsteps.rolloff = cfg.ReadFloat("RollOff");
            locationInfo->footsteps.walkPitch = cfg.ReadFloat("WalkPitch");
            locationInfo->footsteps.backPitch = cfg.ReadFloat("BackPitch");
            locationInfo->footsteps.turnPitch = cfg.ReadFloat("TurnPitch");
            locationInfo->footsteps.walkSound = cfg.ReadString("WalkSound");
            locationInfo->footsteps.turnSound = cfg.ReadString("TurnSound");
            cfg.EndGroup();
        }
        
        else if (key == "Weather") { 
            LocationInfo::Weather weather;

            cfg.BeginGroup("Weather");
            weather.weather = WeatherTypeFromString(cfg.ReadString("WeatherType"));
            weather.fogType = string(cfg.ReadString("FogType")) == "Exp" ? engine::fogExp : engine::fogExp2;
            weather.fogDensity = cfg.ReadFloat("FogDensity");
            float r = cfg.ReadFloat("FogColorRed");
            float g = cfg.ReadFloat("FogColorGreen");
            float b = cfg.ReadFloat("FogColorBlue");
            weather.fogColor.set(r, g, b, 1.0f);
            weather.sunMute = cfg.ReadFloat("SunMute");
            weather.ambientMute = cfg.ReadFloat("AmbientMute");
            weather.panorama.resource = cfg.ReadString("SkyboxFile");
            weather.panorama.zNear = cfg.ReadFloat("ZNear");
            weather.panorama.zFar = cfg.ReadFloat("ZFar");
            cfg.EndGroup();

            locationInfo->weathers.push_back(weather);
        }
        
        else if (key == "Reverberation") { 
            cfg.BeginGroup("Reverberation");
            locationInfo->reverberation.inGain = cfg.ReadFloat("InGain");
            locationInfo->reverberation.reverbMixDB = cfg.ReadFloat("ReverbMixDB");
            locationInfo->reverberation.reverbTime = cfg.ReadFloat("ReverbTime");
            locationInfo->reverberation.hfTimeRatio = cfg.ReadFloat("HfTimeRatio");
            cfg.EndGroup();
        }
        
        else {
            getCore()->logMessage("Error: location file %s is malformed. Keyword: %s", fileName, key.c_str());
            break;
        }
    }

    return locationInfo;
}


void LocationInfo::deleteLocations()
{
    int staticLocationsCount = 0;
    while (!gStaticLocations[staticLocationsCount].nameId.empty()) {
        staticLocationsCount++;
    }

    int i = 0;
    for (i = 0; i < (int)gLocations.size(); ++i) {
        if (i >= staticLocationsCount) {
            delete gLocations[i];
        }
    }

    gLocations.clear();
}
