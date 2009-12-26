
#include "headers.h"
#include "engine/configreader.h"
#include "database.h"
#include "casting.h"
#include "arbitrary.h"
#include "dropzone.h"
#include "ostankino.h"
#include "trollveggen.h"
#include "cave.h"
#include "royalgorge.h"
#include "kvly.h"
#include "kjerag.h"
#include "angelfalls.h"
#include "elcapitan.h"
#include "version.h"

using namespace database;
using namespace std;

/**
 * mission groups
 */

static MissionInfo demoMissions[] =
{
    { 0, 524, 525, 0, 1,  60.0f, "./res/thumbnails/121.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_Opening, equipCallback_OpenAir_CommonMission, NULL },
    { 0, 534, 535, 0, 1,  60.0f, "./res/thumbnails/122.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_LandingAccuracy, equipCallback_OpenAir_CommonMission, NULL },
    { 0, 536, 537, 0, 1,  60.0f, "./res/thumbnails/123.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_TrackingPerformance, equipCallback_OpenAir_WingsuitMission, NULL },
    { 0, 543, 544, 0, 0,  90.0f, "./res/thumbnails/124.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_SpiralFreefall, equipCallback_OpenAir_WingsuitMission, NULL },
    { 0, 545, 546, 0, 2, 180.0f, "./res/thumbnails/125.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_PikeOfThrills, equipCallback_OpenAir_CommonMission, NULL },
    { 0, 547, 548, 0, 1,  60.0f, "./res/thumbnails/126.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_HardLanding, equipCallback_OpenAir_HardLanding, NULL },
    { 0, 550, 551, 0, 0,  90.0f, "./res/thumbnails/127.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_Cameraman, equipCallback_OpenAir_CommonMission, NULL },
    { 0, 559, 560, 0, 0,  90.0f, "./res/thumbnails/128.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_6way, equipCallback_OpenAir_CommonMission, NULL }
};

static MissionInfo vffMissions[] =
{
    { 0, 146, 147, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/011.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_TheBeginning, NULL, NULL },
    { 0, 203, 204, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/012.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_TheManeuvering01, NULL, NULL },
    { 0, 208, 209, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/013.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_TheManeuvering02, NULL, NULL },
    { 0, 398, 399, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/014.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_HopAndPop, NULL, NULL },
    { 0, 402, 403, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/015.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_Freefall, NULL, NULL },
    { 0, 415, 416, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/016.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_Tracking, NULL, NULL },
    { 0, 428, 429, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/017.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_Gadgets, NULL, NULL },
    { 0, 440, 441, mfForcedEquipment, AIRPLANE_EXIT, 2*60, "./res/thumbnails/018.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_VFF_Linetwists, NULL, NULL }
};

static MissionInfo baseVffMissions[] =
{
    { 0, 465, 466, mfForcedEquipment, 0, 2*60, "./res/thumbnails/021.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_BASEVFF_PCA, NULL, NULL },
    { 0, 472, 473, mfForcedEquipment, 0, 2*60, "./res/thumbnails/022.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_BASEVFF_Freefall, NULL, NULL },
    { 0, 475, 476, mfForcedEquipment, 0, 2*60, "./res/thumbnails/023.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_BASEVFF_LandingAccuracy, NULL, NULL },
    { 0, 507, 508, mfForcedEquipment, 0, 2*60, "./res/thumbnails/024.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_BASEVFF_Lineovers, NULL, NULL }
};

static MissionInfo freeJumpingMissions[] = 
{
    { 0, 487, 488, 0, 0, 60.0f, "./res/thumbnails/031.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_Freejump_1000, equipCallback_Freejump, NULL },
    { 1, 491, 492, 0, 0, 65.0f, "./res/thumbnails/032.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_Freejump_1500, equipCallback_Freejump, NULL },
    { 2, 495, 496, 0, 0, 70.0f, "./res/thumbnails/033.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_Freejump_2000, equipCallback_Freejump, NULL },
    { 3, 497, 498, 0, 0, 75.0f, "./res/thumbnails/034.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_Freejump_2500, equipCallback_Freejump, NULL },
    { 4, 499, 500, 0, 0, 80.0f, "./res/thumbnails/035.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_Freejump_3000, equipCallback_Freejump, NULL }
};

static MissionInfo skybaseMissions[] = 
{
    { 1, 503, 504, 0, 0, 60.0f, "./res/thumbnails/041.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_Skybase_250, equipCallback_Skybase_250, NULL },
    { 1, 505, 506, 0, 0, 90.0f, "./res/thumbnails/042.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_Skybase_500, equipCallback_Skybase_500, NULL }
};

static MissionInfo cloudyRodeoMissions[] = 
{
    { 1, 641, 642, 0, AIRPLANE_EXIT, 2*60, "./res/thumbnails/051.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_CloudyRodeo_RGB, equipCallback_FreejumpCommonSuit, NULL },
    { 1, 644, 645, 0, AIRPLANE_EXIT, 2*60, "./res/thumbnails/052.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_CloudyRodeo_SlalomRGB, equipCallback_FreejumpWingsuit, NULL },
    { 3, 646, 647, 0, AIRPLANE_EXIT, 2*60, "./res/thumbnails/053.dds", weatherClearanceDropzone, windClearanceDropzone, castingCallback_CloudyRodeo_ExtremeRGB, equipCallback_CloudyRodeo_ExtremeRGB, NULL }
};

static MissionInfo moscowUndergroundMissions[] = 
{
    { 0, 516, 517, 0, 3, 60, "./res/thumbnails/111.dds", weatherClearanceAU, windClearanceAU, castingCallback_AU_PCA, equipCallback_AU_PCA, NULL }
};

static MissionInfo openAirMissions[] = 
{
    { 2, 524, 525, 0, 1,  60.0f, "./res/thumbnails/121.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_Opening, equipCallback_OpenAir_CommonMission, NULL },
    { 1, 534, 535, 0, 1,  60.0f, "./res/thumbnails/122.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_LandingAccuracy, equipCallback_OpenAir_CommonMission, NULL },
    { 2, 536, 537, 0, 1,  60.0f, "./res/thumbnails/123.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_TrackingPerformance, equipCallback_OpenAir_WingsuitMission, NULL },
    { 3, 543, 544, 0, 0,  90.0f, "./res/thumbnails/124.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_SpiralFreefall, equipCallback_OpenAir_WingsuitMission, NULL },
    { 2, 545, 546, 0, 2, 180.0f, "./res/thumbnails/125.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_PikeOfThrills, equipCallback_OpenAir_CommonMission, NULL },
    { 1, 547, 548, 0, 1,  60.0f, "./res/thumbnails/126.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_HardLanding, equipCallback_OpenAir_HardLanding, NULL },
    { 1, 550, 551, 0, 0,  90.0f, "./res/thumbnails/127.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_Cameraman, equipCallback_OpenAir_CommonMission, NULL },
    { 1, 559, 560, 0, 0,  90.0f, "./res/thumbnails/128.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_6way, equipCallback_OpenAir_CommonMission, NULL },
    { 1, 685, 688, 0, 1,  60.0f, "./res/thumbnails/129.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_FlipCount, equipCallback_OpenAir_FlipCount, NULL }
};

static MissionInfo openAirSmokeballMissions[] = 
{
    { 2, 652, 653, 0, 2, 180.0f, "./res/thumbnails/131.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_RGB_Zigzag, equipCallback_OpenAir_WingsuitMission, NULL },
    { 2, 654, 655, 0, 2, 180.0f, "./res/thumbnails/132.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_RGB_Spiral, equipCallback_OpenAir_WingsuitMission, NULL },
    { 2, 646, 656, 0, 2, 180.0f, "./res/thumbnails/133.dds", weatherClearanceOpenAir, windClearanceOpenAir, castingCallback_OpenAir_RGB_Extreme, equipCallback_OpenAir_WingsuitMission, NULL }
};

static MissionInfo trollFreejumpingMissions[] =
{
    { 0, 625, 626, 0, 0, 5*60.0f, "./res/thumbnails/331.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollryggenFreejumping, equipCallback_TrollBoogie, NULL },
    { 1, 627, 628, 0, 1, 6*60.0f, "./res/thumbnails/332.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollveggenFreejumping, equipCallback_TrollBoogie, NULL },
    { 2, 629, 630, 0, 2, 7*60.0f, "./res/thumbnails/333.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollFreejumping, equipCallback_TrollBoogie, NULL }
};

static MissionInfo trollFieldTrainingMissions[] = 
{
    { 1, 663, 664, 0, 0, 5*60.0f, "./res/thumbnails/351.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollField_JumpFromRun, equipCallback_TrollField, NULL }
};

static MissionInfo trollBoogieMissions[] =
{
    { 1, 592, 593, 0, 0, 5*60.0f, "./res/thumbnails/316.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollBoogie_TrollryggenIntervalJumping, equipCallback_TrollBoogie, NULL },
    { 2, 563, 564, 0, 0, 5*60.0f, "./res/thumbnails/311.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollBoogie_Trollryggen3way, equipCallback_TrollBoogie, NULL },
    { 2, 565, 566, 0, 1, 6*60.0f, "./res/thumbnails/312.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollBoogie_Trollveggen3way, equipCallback_TrollBoogie, NULL },
    { 3, 580, 581, 0, 0, 5*60.0f, "./res/thumbnails/314.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollBoogie_TrollryggenProximity, equipCallback_TrollBoogie, NULL },
    { 3, 576, 577, 0, 2, 7*60.0f, "./res/thumbnails/313.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollBoogie_TrollProximity, equipCallback_TrollBoogie, NULL },
    { 1, 582, 583, 0, AIRPLANE_EXIT, 2*60.0f, "./res/thumbnails/315.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollBoogie_HeliBASE, equipCallback_TrollBoogie, NULL }
};

static MissionInfo trollClimbingMissions[] =
{
    { 5, 569, 570, 0, 3, 7*60.0f, "./res/thumbnails/321.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollClimbing_TrollveggenRoute, equipCallback_TrollClimbing, NULL },
    { 5, 571, 572, 0, 4, 8*60.0f, "./res/thumbnails/322.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollClimbing_TrollryggenRoute, equipCallback_TrollClimbing, NULL },
    { 5, 573, 574, 0, 5, 9*60.0f, "./res/thumbnails/323.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollClimbing_TrollRoute, equipCallback_TrollClimbing, NULL }
};

static MissionInfo trollSmokeballMissions[] = 
{
    { 3, 652, 657, 0, 0, 5*60.0f, "./res/thumbnails/341.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollSmokeball_RGB_Tracking, equipCallback_TrollClimbing, NULL },
    { 3, 644, 658, 0, 5, 9*60.0f, "./res/thumbnails/342.dds", weatherClearanceTrollveggen, windClearanceTrollveggen, castingCallback_TrollSmokeball_RGB_Slalom, equipCallback_TrollClimbing, NULL }
};

static MissionInfo caveFreejumpMissions[] =
{
    { 0, 621, 623, 0, 0, 4*60.0f, "./res/thumbnails/421.dds", weatherClearanceCave, windClearanceCave, castingCallback_CaveBase_Freejumping, equipCallback_CaveBase, NULL }
};

static MissionInfo caveBaseMissions[] =
{
    { 2, 589, 590, 0, 0, 2*60.0f, "./res/thumbnails/413.dds", weatherClearanceCave, windClearanceCave, castingCallback_CaveBase_IntervalJumping, equipCallback_CaveBase, NULL },
    { 3, 585, 586, 0, 0, 2*60.0f, "./res/thumbnails/411.dds", weatherClearanceCave, windClearanceCave, castingCallback_CaveBase_2way, equipCallback_CaveBase, NULL },
    { 3, 587, 588, 0, 0, 2*60.0f, "./res/thumbnails/412.dds", weatherClearanceCave, windClearanceCave, castingCallback_CaveBase_3way, equipCallback_CaveBase, NULL }
};

static MissionInfo royalGorgeUndergroundMissions[] = 
{
    { 0, 612, 613, 0, 0, 60.0f, "./res/thumbnails/521.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_RoyalGorgeUnderground, equipCallback_RoyalGorge, NULL }
};

static MissionInfo goFallFieldTrainingMissions[] = 
{
    { 1, 676, 677, 0, 0, 4*60.0f, "./res/thumbnails/541.dds", weatherClearanceAU, windClearanceAU, castingCallback_GoFallField_Frontflip, equipCallback_RoyalGorge_FieldTraining, NULL },
    { 2, 670, 671, 0, 0, 4*60.0f, "./res/thumbnails/542.dds", weatherClearanceCave, windClearanceCave, castingCallback_GoFallField_Backflip, equipCallback_RoyalGorge_FieldTraining, NULL },
    { 3, 680, 681, 0, 0, 4*60.0f, "./res/thumbnails/543.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFallField_BackflipBackforward, equipCallback_RoyalGorge_FieldTraining, NULL }
};

static MissionInfo goFallMissions[] =
{
    { 2, 524, 615, 0, AIRPLANE_EXIT, 2*60.0f, "./res/thumbnails/514.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_Opening, equipCallback_RoyalGorge, NULL },
    { 1, 589, 598, 0, 0, 60.0f, "./res/thumbnails/511.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_IntervalJumping, equipCallback_RoyalGorge, NULL },
    { 2, 605, 606, 0, 0, 60.0f, "./res/thumbnails/513.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_Leader4way, equipCallback_GoFall_Leader4way, NULL },
    { 1, 609, 610, 0, 0, 60.0f, "./res/thumbnails/513.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_LastOne4way, equipCallback_GoFall_LastOne4way, NULL },
    { 1, 685, 686, 0, 0, 60.0f, "./res/thumbnails/516.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_FlipCompetition, equipCallback_RoyalGorge_FieldTraining, NULL },
    { 3, 599, 600, 0, AIRPLANE_EXIT, 2*60.0f, "./res/thumbnails/512.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_BridgeProxy, equipCallback_GoFall_BridgeProxy, NULL },
    { 3, 619, 620, 0, AIRPLANE_EXIT, 2*60.0f, "./res/thumbnails/515.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_ExtremeTracking, equipCallback_RoyalGorge, NULL }
};

static MissionInfo goFallSmokeballMissions[] =
{
    { 2, 641, 659, 0, AIRPLANE_EXIT, 2*60.0f, "./res/thumbnails/531.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_RGB_Tracking, equipCallback_GoFall_BridgeProxy, NULL },
    { 2, 646, 660, 0, AIRPLANE_EXIT, 2*60.0f, "./res/thumbnails/532.dds", weatherClearanceRoyalGorge, windClearanceRoyalGorge, castingCallback_GoFall_RGB_Extreme, equipCallback_GoFall_BridgeProxy, NULL }
};

static MissionInfo kvlyUndergroundMissions[] =
{
    { 0, 612, 772, 0, 0, 4*60.0f, "./res/thumbnails/611.dds", weatherClearanceKVLY, windClearanceKVLY, castingCallback_KVLY_Underground, equipCallback_KVLY, NULL }
};

static MissionInfo tvBoogieMissions[] =
{
    { 1, 589, 775, 0, 0, 4*60.0f, "./res/thumbnails/621.dds", weatherClearanceKVLY, windClearanceKVLY, castingCallback_KVLY_IntervalJumps, equipCallback_KVLY, NULL },
    { 2, 776, 777, 0, 0, 4*60.0f, "./res/thumbnails/622.dds", weatherClearanceKVLY, windClearanceKVLY, castingCallback_KVLY_3way, equipCallback_KVLY, NULL },
    { 3, 778, 779, 0, 0, 4*60.0f, "./res/thumbnails/623.dds", weatherClearanceKVLY, windClearanceKVLY, castingCallback_KVLY_6way, equipCallback_KVLY, NULL },
    { 3, 780, 781, 0, 0, 4*60.0f, "./res/thumbnails/624.dds", weatherClearanceKVLY, windClearanceKVLY, castingCallback_KVLY_6wayWings, equipCallback_KVLY_Wings, NULL }
};

static MissionInfo kjeragFreejumpingMissions[] = 
{
    { 0, 788, 789, 0, 0, 2.0f*60.0f, "./res/thumbnails/711.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_Kjerag_Freejump, equipCallback_Kjerag, NULL },
    { 0, 791, 792, 0, 1, 2.5f*60.0f, "./res/thumbnails/712.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_Kjerag_Freejump, equipCallback_Kjerag_Wings, NULL },
    { 0, 794, 795, 0, 2, 3.0f*60.0f, "./res/thumbnails/713.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_Kjerag_Freejump, equipCallback_Kjerag, NULL },
    { 0, 797, 798, 0, 3, 3.5f*60.0f, "./res/thumbnails/714.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_Kjerag_Freejump, equipCallback_Kjerag_Wings, NULL }
};

static MissionInfo kjeragBoogieMissions[] =
{
    { 1, 801, 802, 0, 2, 3.0f*60.0f, "./res/thumbnails/721.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_IntervalJumps, equipCallback_Kjerag, NULL },
    { 1, 803, 804, 0, 2, 3.0f*60.0f, "./res/thumbnails/722.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_6wayFromEP2, equipCallback_Kjerag, NULL },
    { 2, 805, 806, 0, 0, 2.0f*60.0f, "./res/thumbnails/723.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_6wayFromTW, equipCallback_Kjerag, NULL },
    { 2, 807, 808, 0, 1, 2.5f*60.0f, "./res/thumbnails/724.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_3wingsFromEP1, equipCallback_Kjerag_Wings, NULL },
    { 2, 809, 810, 0, 3, 3.5f*60.0f, "./res/thumbnails/725.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_3wingsFromEP3, equipCallback_Kjerag_Wings, NULL },
    { 3, 811, 812, 0, 1, 2.5f*60.0f, "./res/thumbnails/726.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_Proxi, equipCallback_Kjerag_Wings, NULL },
    { 3, 813, 814, 0, 3, 3.5f*60.0f, "./res/thumbnails/727.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_Proxi, equipCallback_Kjerag_Wings, NULL },
    { 3, 817, 818, 0, AIRPLANE_EXIT, 1.5f*60.0f, "./res/thumbnails/729.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_HeliBASE, equipCallback_Kjerag, NULL },
    { 4, 815, 816, 0, AIRPLANE_EXIT, 1.5f*60.0f, "./res/thumbnails/728.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_HeliBASE2, equipCallback_Kjerag_Wings, NULL },
    { 4, 819, 820, 0, AIRPLANE_EXIT, 1.5f*60.0f, "./res/thumbnails/72a.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragBoogie_HeliBASE3, equipCallback_Kjerag, NULL }
};

static MissionInfo kjeragSmokeballMissions[] = 
{
    { 1, 641, 821, 0, 2, 3.0f*60.0f, "./res/thumbnails/731.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragSmokeball_RGBTracking, equipCallback_Kjerag, NULL },
    { 2, 644, 822, 0, 0, 2.0f*60.0f, "./res/thumbnails/732.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragSmokeball_RGBSlalom01, equipCallback_Kjerag_Wings, NULL },
    { 3, 652, 823, AIRPLANE_EXIT, 0, 1.5f*60.0f, "./res/thumbnails/733.dds", weatherClearanceKjerag, windClearanceKjerag, castingCallback_KjeragSmokeball_RGBZigzag, equipCallback_Kjerag_Wings, NULL }
};

static MissionInfo angelFallsFreeJumps[] = 
{
    { 0, 830, 831, 0, 0, 3.0f*60.0f, "./res/thumbnails/811.dds", weatherClearanceAngelFalls, windClearanceAngelFalls, castingCallback_AngelFallsFreeJump, equipCallback_AngelFalls, NULL },
    { 0, 833, 834, 0, 1, 2.5f*60.0f, "./res/thumbnails/812.dds", weatherClearanceAngelFalls, windClearanceAngelFalls, castingCallback_AngelFallsFreeJump, equipCallback_AngelFalls, NULL }
};

static MissionInfo angelFallsBoogie[] = 
{
    { 1, 589, 835, 0, 0, 3.0f*60.0f, "./res/thumbnails/821.dds", weatherClearanceAngelFalls, windClearanceAngelFalls, castingCallback_AngelFallsIntervalJump, equipCallback_AngelFalls, NULL },
    { 2, 838, 839, 0, 1, 2.5f*60.0f, "./res/thumbnails/822.dds", weatherClearanceAngelFalls, windClearanceAngelFalls, castingCallback_AngelFallsCloseAndCloser, equipCallback_AngelFalls_Wings, NULL },
    { 2, 840, 841, 0, 1, 2.5f*60.0f, "./res/thumbnails/823.dds", weatherClearanceAngelFalls, windClearanceAngelFalls, castingCallback_AngelFalls6way, equipCallback_AngelFalls, NULL },
    { 3, 836, 837, 0, 0, 3.0f*60.0f, "./res/thumbnails/824.dds", weatherClearanceAngelFalls, windClearanceAngelFalls, castingCallback_AngelFallsNoWings, equipCallback_AngelFalls, NULL }
};

static MissionInfo elCapitanFreeJumps[] =
{
    { 0, 849, 850, 0, 0, 4.0f*60.0f, "./res/thumbnails/911.dds", weatherClearanceElCapitan, windClearanceElCapitan, castingCallback_ElCapitanFreeJump, equipCallback_ElCapitan, NULL }
};

/**
 * tournament database
 */
/*
static TournamentInfo tournaments[] =
{
    { 649, 650, 1, tfArbitrary, NULL,  "./res/thumbnails/120.dds", demoMissions },
    { 125, 145, 0, tfArbitrary, "T01", "./res/thumbnails/010.dds", vffMissions },
    { 463, 464, 0, tfArbitrary, "T02", "./res/thumbnails/020.dds", baseVffMissions },
    { 485, 486, 0, tfArbitrary, "T03", "./res/thumbnails/030.dds", freeJumpingMissions },
    { 501, 502, 0, tfArbitrary, "T04", "./res/thumbnails/040.dds", skybaseMissions },
    { 639, 640, 0, tfSmokeball, "T05", "./res/thumbnails/050.dds", cloudyRodeoMissions },
    { 514, 515, 1, tfArbitrary, "T06", "./res/thumbnails/110.dds", moscowUndergroundMissions },
    { 522, 523, 1, tfFestival,  "T07", "./res/thumbnails/120.dds", openAirMissions },
    { 637, 638, 1, tfFestival,  "T08", "./res/thumbnails/130.dds", openAirSmokeballMissions },
    {  91, 624, 3, tfArbitrary, "T09", "./res/thumbnails/330.dds", trollFreejumpingMissions },
    { 661, 662, 3, tfBoogie,    "T10", "./res/thumbnails/350.dds", trollFieldTrainingMissions }, 
    { 253, 562, 3, tfBoogie,    "T11", "./res/thumbnails/310.dds", trollBoogieMissions },
    { 567, 568, 3, tfClimbing,  "T12", "./res/thumbnails/320.dds", trollClimbingMissions },
    { 637, 638, 3, tfBoogie,    "T13", "./res/thumbnails/340.dds", trollSmokeballMissions },
    {  90, 622, 2, tfArbitrary, "T14", "./res/thumbnails/420.dds", caveFreejumpMissions },
    { 254, 584, 2, tfBoogie,    "T15", "./res/thumbnails/410.dds", caveBaseMissions },
    {  92, 611, 4, tfArbitrary, "T16", "./res/thumbnails/520.dds", royalGorgeUndergroundMissions },
    { 661, 662, 4, tfFestival,  "T17", "./res/thumbnails/540.dds", goFallFieldTrainingMissions },
    { 596, 597, 4, tfFestival,  "T18", "./res/thumbnails/510.dds", goFallMissions },
    { 637, 638, 4, tfFestival,  "T19", "./res/thumbnails/530.dds", goFallSmokeballMissions },
    { 770, 771, 5, tfArbitrary, "T20", "./res/thumbnails/610.dds", kvlyUndergroundMissions },
    { 773, 774, 5, tfBoogie,    "T21", "./res/thumbnails/620.dds", tvBoogieMissions },
    { 786, 787, 6, tfArbitrary, "T22", "./res/thumbnails/710.dds", kjeragFreejumpingMissions },
    { 799, 800, 6, tfBoogie,    "T23", "./res/thumbnails/720.dds", kjeragBoogieMissions },
    { 637, 638, 6, tfBoogie,    "T24", "./res/thumbnails/730.dds", kjeragSmokeballMissions },
    { 828, 829, 7, tfArbitrary, "T25", "./res/thumbnails/810.dds", angelFallsFreeJumps },
    { 842, 829, 7, tfBoogie,    "T26", "./res/thumbnails/820.dds", angelFallsBoogie },
    { 847, 848, 8, tfArbitrary, "T27", "./res/thumbnails/910.dds", elCapitanFreeJumps },
    { 770, 771, 9, tfArbitrary, "T28", "./res/thumbnails/610.dds", kvlyUndergroundMissions },
    { 0, 0, 0, tfArbitrary, NULL, NULL, NULL }
};
*/

/**
 * database access
 */

unsigned int TournamentInfo::getNumMissions(void)
{
    return missions.size();
}

unsigned int TournamentInfo::getMinimalRank(void)
{
    unsigned int minimalRank = 10;
    unsigned int numMissions = getNumMissions();
    for( unsigned int i=0; i<numMissions; i++ )
    {
        if( missions[i].rank < minimalRank )
        {
            minimalRank = missions[i].rank;
        }
    }
    return minimalRank;
}


unsigned int TournamentInfo::getDemoTournament(void)
{
    return 0;
}


TournamentType TournamentTypeFromString(const char* str)
{
    string name(str);
    if (name == "Arbitrary") { return tfArbitrary; }
    else if (name == "Boogie") { return tfBoogie; }
    else if (name == "Festival") { return tfFestival; }
    else if (name == "Climbing") { return tfClimbing; }
    else if (name == "Smokeball") { return tfSmokeball; }
    else if (name == "User") { return tfUser; }
    
    getCore()->logMessage("Error: bad tournament type: %s, defaulting to Arbitrary", str);
    return tfArbitrary;
}


unsigned int MissionFlagFromString(const char* str)
{
    string name(str);
    if (name == "ForcedEquipment") {
        return (unsigned int)mfForcedEquipment;
    }

    getCore()->logMessage("Error: bad mission flag: %s", str);

    return 0;
}


WeatherClearance WeatherClearanceFromString(const char* str)
{
    string name(str);
    if (name == "weatherClearanceOpenAir") { return weatherClearanceOpenAir; }
    else if (name == "weatherClearanceDropzone") { return weatherClearanceDropzone; }
    else if (name == "weatherClearanceAU") { return weatherClearanceAU; }
    else if (name == "weatherClearanceOpenAir") { return weatherClearanceOpenAir; }
    else if (name == "weatherClearanceTrollveggen") { return weatherClearanceTrollveggen; }
    else if (name == "weatherClearanceCave") { return weatherClearanceCave; }
    else if (name == "weatherClearanceRoyalGorge") { return weatherClearanceRoyalGorge; }
    else if (name == "weatherClearanceKVLY") { return weatherClearanceKVLY; }
    else if (name == "weatherClearanceKjerag") { return weatherClearanceKjerag; }
    else if (name == "weatherClearanceAngelFalls") { return weatherClearanceAngelFalls; }
    else if (name == "weatherClearanceElCapitan") { return weatherClearanceElCapitan; }

    getCore()->logMessage("Error: bad weather clearance: %s, defaulting to weatherClearanceDropzone", str);
    return weatherClearanceDropzone;
}


WindClearance WindClearanceFromString(const char* str)
{
    string name(str);
    if (name == "windClearanceOpenAir") { return windClearanceOpenAir; }
    else if (name == "windClearanceDropzone") { return windClearanceDropzone; }
    else if (name == "windClearanceAU") { return windClearanceAU; }
    else if (name == "windClearanceOpenAir") { return windClearanceOpenAir; }
    else if (name == "windClearanceTrollveggen") { return windClearanceTrollveggen; }
    else if (name == "windClearanceCave") { return windClearanceCave; }
    else if (name == "windClearanceRoyalGorge") { return windClearanceRoyalGorge; }
    else if (name == "windClearanceKVLY") { return windClearanceKVLY; }
    else if (name == "windClearanceKjerag") { return windClearanceKjerag; }
    else if (name == "windClearanceAngelFalls") { return windClearanceAngelFalls; }
    else if (name == "windClearanceElCapitan") { return windClearanceElCapitan; }

    getCore()->logMessage("Error: bad wind clearance: %s, defaulting to windClearanceDropzone", str);
    return windClearanceDropzone;
}


CastingCallback MissionCastingCallbackFromString(const char* str)
{
    string name(str);
    if (name == "castingCallback_OpenAir_Opening") { return castingCallback_OpenAir_Opening; }

    else if (name == "castingCallback_OpenAir_LandingAccuracy") { return castingCallback_OpenAir_LandingAccuracy; }
    else if (name == "castingCallback_OpenAir_TrackingPerformance") { return castingCallback_OpenAir_TrackingPerformance; }
    else if (name == "castingCallback_OpenAir_SpiralFreefall") { return castingCallback_OpenAir_SpiralFreefall; }
    else if (name == "castingCallback_OpenAir_PikeOfThrills") { return castingCallback_OpenAir_PikeOfThrills; }
    else if (name == "castingCallback_OpenAir_HardLanding") { return castingCallback_OpenAir_HardLanding; }
    else if (name == "castingCallback_OpenAir_Cameraman") { return castingCallback_OpenAir_Cameraman; }
    else if (name == "castingCallback_OpenAir_6way") { return castingCallback_OpenAir_6way; }

    else if (name == "castingCallback_VFF_TheBeginning") { return castingCallback_VFF_TheBeginning; }
    else if (name == "castingCallback_VFF_TheManeuvering01") { return castingCallback_VFF_TheManeuvering01; }
    else if (name == "castingCallback_VFF_TheManeuvering02") { return castingCallback_VFF_TheManeuvering02; }
    else if (name == "castingCallback_VFF_HopAndPop") { return castingCallback_VFF_HopAndPop; }
    else if (name == "castingCallback_VFF_Freefall") { return castingCallback_VFF_Freefall; }
    else if (name == "castingCallback_VFF_Tracking") { return castingCallback_VFF_Tracking; }
    else if (name == "castingCallback_VFF_Gadgets") { return castingCallback_VFF_Gadgets; }
    else if (name == "castingCallback_VFF_Linetwists") { return castingCallback_VFF_Linetwists; }

    else if (name == "castingCallback_BASEVFF_PCA") { return castingCallback_BASEVFF_PCA; }
    else if (name == "castingCallback_BASEVFF_Freefall") { return castingCallback_BASEVFF_Freefall; }
    else if (name == "castingCallback_BASEVFF_LandingAccuracy") { return castingCallback_BASEVFF_LandingAccuracy; }
    else if (name == "castingCallback_BASEVFF_Lineovers") { return castingCallback_BASEVFF_Lineovers; }

    else if (name == "castingCallback_Freejump_1000") { return castingCallback_Freejump_1000; }
    else if (name == "castingCallback_Freejump_1500") { return castingCallback_Freejump_1500; }
    else if (name == "castingCallback_Freejump_2000") { return castingCallback_Freejump_2000; }
    else if (name == "castingCallback_Freejump_2500") { return castingCallback_Freejump_2500; }
    else if (name == "castingCallback_Freejump_3000") { return castingCallback_Freejump_3000; }

    else if (name == "castingCallback_Skybase_250") { return castingCallback_Skybase_250; }
    else if (name == "castingCallback_Skybase_500") { return castingCallback_Skybase_500; }

    else if (name == "castingCallback_CloudyRodeo_RGB") { return castingCallback_CloudyRodeo_RGB; }
    else if (name == "castingCallback_CloudyRodeo_SlalomRGB") { return castingCallback_CloudyRodeo_SlalomRGB; }
    else if (name == "castingCallback_CloudyRodeo_ExtremeRGB") { return castingCallback_CloudyRodeo_ExtremeRGB; }

    else if (name == "castingCallback_AU_PCA") { return castingCallback_AU_PCA; }

    else if (name == "castingCallback_OpenAir_Opening") { return castingCallback_OpenAir_Opening; }
    else if (name == "castingCallback_OpenAir_LandingAccuracy") { return castingCallback_OpenAir_LandingAccuracy; }
    else if (name == "castingCallback_OpenAir_TrackingPerformance") { return castingCallback_OpenAir_TrackingPerformance; }
    else if (name == "castingCallback_OpenAir_SpiralFreefall") { return castingCallback_OpenAir_SpiralFreefall; }
    else if (name == "castingCallback_OpenAir_PikeOfThrills") { return castingCallback_OpenAir_PikeOfThrills; }
    else if (name == "castingCallback_OpenAir_HardLanding") { return castingCallback_OpenAir_HardLanding; }
    else if (name == "castingCallback_OpenAir_Cameraman") { return castingCallback_OpenAir_Cameraman; }
    else if (name == "castingCallback_OpenAir_6way") { return castingCallback_OpenAir_6way; }
    else if (name == "castingCallback_OpenAir_FlipCount") { return castingCallback_OpenAir_FlipCount; }

    else if (name == "castingCallback_OpenAir_RGB_Zigzag") { return castingCallback_OpenAir_RGB_Zigzag; }
    else if (name == "castingCallback_OpenAir_RGB_Spiral") { return castingCallback_OpenAir_RGB_Spiral; }
    else if (name == "castingCallback_OpenAir_RGB_Extreme") { return castingCallback_OpenAir_RGB_Extreme; }

    else if (name == "castingCallback_TrollryggenFreejumping") { return castingCallback_TrollryggenFreejumping; }
    else if (name == "castingCallback_TrollFreejumping") { return castingCallback_TrollFreejumping; }

    else if (name == "castingCallback_TrollField_JumpFromRun") { return castingCallback_TrollField_JumpFromRun; }

    else if (name == "castingCallback_TrollBoogie_TrollryggenIntervalJumping") { return castingCallback_TrollBoogie_TrollryggenIntervalJumping; }
    else if (name == "castingCallback_TrollBoogie_Trollryggen3way") { return castingCallback_TrollBoogie_Trollryggen3way; }
    else if (name == "castingCallback_TrollBoogie_Trollveggen3way") { return castingCallback_TrollBoogie_Trollveggen3way; }
    else if (name == "castingCallback_TrollBoogie_TrollryggenProximity") { return castingCallback_TrollBoogie_TrollryggenProximity; }
    else if (name == "castingCallback_TrollBoogie_TrollProximity") { return castingCallback_TrollBoogie_TrollProximity; }
    else if (name == "castingCallback_TrollBoogie_HeliBASE") { return castingCallback_TrollBoogie_HeliBASE; }

    else if (name == "castingCallback_TrollClimbing_TrollveggenRoute") { return castingCallback_TrollClimbing_TrollveggenRoute; }
    else if (name == "castingCallback_TrollClimbing_TrollryggenRoute") { return castingCallback_TrollClimbing_TrollryggenRoute; }
    else if (name == "castingCallback_TrollClimbing_TrollRoute") { return castingCallback_TrollClimbing_TrollRoute; }

    else if (name == "castingCallback_TrollSmokeball_RGB_Tracking") { return castingCallback_TrollSmokeball_RGB_Tracking; }
    else if (name == "castingCallback_TrollSmokeball_RGB_Slalom") { return castingCallback_TrollSmokeball_RGB_Slalom; }

    else if (name == "castingCallback_CaveBase_Freejumping") { return castingCallback_CaveBase_Freejumping; }

    else if (name == "castingCallback_CaveBase_IntervalJumping") { return castingCallback_CaveBase_IntervalJumping; }
    else if (name == "castingCallback_CaveBase_2way") { return castingCallback_CaveBase_2way; }
    else if (name == "castingCallback_CaveBase_3way") { return castingCallback_CaveBase_3way; }

    else if (name == "castingCallback_RoyalGorgeUnderground") { return castingCallback_RoyalGorgeUnderground; }

    else if (name == "castingCallback_GoFallField_Frontflip") { return castingCallback_GoFallField_Frontflip; }
    else if (name == "castingCallback_GoFallField_Backflip") { return castingCallback_GoFallField_Backflip; }
    else if (name == "castingCallback_GoFallField_BackflipBackforward") { return castingCallback_GoFallField_BackflipBackforward; }

    else if (name == "castingCallback_GoFall_Opening") { return castingCallback_GoFall_Opening; }
    else if (name == "castingCallback_GoFall_IntervalJumping") { return castingCallback_GoFall_IntervalJumping; }
    else if (name == "castingCallback_GoFall_Leader4way") { return castingCallback_GoFall_Leader4way; }
    else if (name == "castingCallback_GoFall_LastOne4way") { return castingCallback_GoFall_LastOne4way; }
    else if (name == "castingCallback_GoFall_FlipCompetition") { return castingCallback_GoFall_FlipCompetition; }
    else if (name == "castingCallback_GoFall_BridgeProxy") { return castingCallback_GoFall_BridgeProxy; }
    else if (name == "castingCallback_GoFall_ExtremeTracking") { return castingCallback_GoFall_ExtremeTracking; }

    else if (name == "castingCallback_GoFall_RGB_Tracking") { return castingCallback_GoFall_RGB_Tracking; }
    else if (name == "castingCallback_GoFall_RGB_Extreme") { return castingCallback_GoFall_RGB_Extreme; }

    else if (name == "castingCallback_KVLY_Underground") { return castingCallback_KVLY_Underground; }

    else if (name == "castingCallback_KVLY_IntervalJumps") { return castingCallback_KVLY_IntervalJumps; }
    else if (name == "castingCallback_KVLY_3way") { return castingCallback_KVLY_3way; }
    else if (name == "castingCallback_KVLY_6way") { return castingCallback_KVLY_6way; }
    else if (name == "castingCallback_KVLY_6wayWings") { return castingCallback_KVLY_6wayWings; }

    else if (name == "castingCallback_Kjerag_Freejump") { return castingCallback_Kjerag_Freejump; }

    else if (name == "castingCallback_KjeragBoogie_IntervalJumps") { return castingCallback_KjeragBoogie_IntervalJumps; }
    else if (name == "castingCallback_KjeragBoogie_6wayFromEP2") { return castingCallback_KjeragBoogie_6wayFromEP2; }
    else if (name == "castingCallback_KjeragBoogie_6wayFromTW") { return castingCallback_KjeragBoogie_6wayFromTW; }
    else if (name == "castingCallback_KjeragBoogie_3wingsFromEP1") { return castingCallback_KjeragBoogie_3wingsFromEP1; }
    else if (name == "castingCallback_KjeragBoogie_3wingsFromEP3") { return castingCallback_KjeragBoogie_3wingsFromEP3; }
    else if (name == "castingCallback_KjeragBoogie_Proxi") { return castingCallback_KjeragBoogie_Proxi; }
    else if (name == "castingCallback_KjeragBoogie_HeliBASE") { return castingCallback_KjeragBoogie_HeliBASE; }
    else if (name == "castingCallback_KjeragBoogie_HeliBASE2") { return castingCallback_KjeragBoogie_HeliBASE2; }
    else if (name == "castingCallback_KjeragBoogie_HeliBASE3") { return castingCallback_KjeragBoogie_HeliBASE3; }

    else if (name == "castingCallback_KjeragSmokeball_RGBTracking") { return castingCallback_KjeragSmokeball_RGBTracking; }
    else if (name == "castingCallback_KjeragSmokeball_RGBSlalom01") { return castingCallback_KjeragSmokeball_RGBSlalom01; }
    else if (name == "castingCallback_KjeragSmokeball_RGBZigzag") { return castingCallback_KjeragSmokeball_RGBZigzag; }

    else if (name == "castingCallback_AngelFallsFreeJump") { return castingCallback_AngelFallsFreeJump; }

    else if (name == "castingCallback_AngelFallsIntervalJump") { return castingCallback_AngelFallsIntervalJump; }
    else if (name == "castingCallback_AngelFallsCloseAndCloser") { return castingCallback_AngelFallsCloseAndCloser; }
    else if (name == "castingCallback_AngelFalls6way") { return castingCallback_AngelFalls6way; }
    else if (name == "castingCallback_AngelFallsNoWings") { return castingCallback_AngelFallsNoWings; }

    else if (name == "castingCallback_ElCapitanFreeJump") { return castingCallback_ElCapitanFreeJump; }

    getCore()->logMessage("Error: bad casting callback: %s, defaulting to castingCallback_KVLY_Underground", str);
    return castingCallback_KVLY_Underground;
}


EquipCallback EquipCallbackFromString(const char* str)
{
    string name(str);
    if (name == "equipCallback_OpenAir_CommonMission") { return equipCallback_OpenAir_CommonMission; }
    else if (name == "equipCallback_OpenAir_WingsuitMission") { return equipCallback_OpenAir_WingsuitMission; }
    else if (name == "equipCallback_OpenAir_HardLanding") { return equipCallback_OpenAir_HardLanding; }

    else if (name == "equipCallback_Freejump") { return equipCallback_Freejump; }

    else if (name == "equipCallback_Skybase_250") { return equipCallback_Skybase_250; }
    else if (name == "equipCallback_Skybase_500") { return equipCallback_Skybase_500; }

    else if (name == "equipCallback_FreejumpCommonSuit") { return equipCallback_FreejumpCommonSuit; }
    else if (name == "equipCallback_FreejumpWingsuit") { return equipCallback_FreejumpWingsuit; }

    else if (name == "equipCallback_CloudyRodeo_ExtremeRGB") { return equipCallback_CloudyRodeo_ExtremeRGB; }

    else if (name == "equipCallback_AU_PCA") { return equipCallback_AU_PCA; }

    else if (name == "equipCallback_OpenAir_CommonMission") { return equipCallback_OpenAir_CommonMission; }
    else if (name == "equipCallback_OpenAir_WingsuitMission") { return equipCallback_OpenAir_WingsuitMission; }
    else if (name == "equipCallback_OpenAir_HardLanding") { return equipCallback_OpenAir_HardLanding; }
    else if (name == "equipCallback_OpenAir_FlipCount") { return equipCallback_OpenAir_FlipCount; }

    else if (name == "equipCallback_TrollBoogie") { return equipCallback_TrollBoogie; }
    else if (name == "equipCallback_TrollField") { return equipCallback_TrollField; }
    else if (name == "equipCallback_TrollClimbing") { return equipCallback_TrollClimbing; }

    else if (name == "equipCallback_CaveBase") { return equipCallback_CaveBase; }

    else if (name == "equipCallback_RoyalGorge") { return equipCallback_RoyalGorge; }
    else if (name == "equipCallback_RoyalGorge_FieldTraining") { return equipCallback_RoyalGorge_FieldTraining; }
    else if (name == "equipCallback_GoFall_Leader4way") { return equipCallback_GoFall_Leader4way; }
    else if (name == "equipCallback_GoFall_LastOne4way") { return equipCallback_GoFall_LastOne4way; }
    else if (name == "equipCallback_GoFall_BridgeProxy") { return equipCallback_GoFall_BridgeProxy; }

    else if (name == "equipCallback_KVLY") { return equipCallback_KVLY; }

    else if (name == "equipCallback_KVLY_Wings") { return equipCallback_KVLY_Wings; }

    else if (name == "equipCallback_Kjerag") { return equipCallback_Kjerag; }
    else if (name == "equipCallback_Kjerag_Wings") { return equipCallback_Kjerag_Wings; }

    else if (name == "equipCallback_AngelFalls") { return equipCallback_AngelFalls; }
    else if (name == "equipCallback_AngelFalls_Wings") { return equipCallback_AngelFalls_Wings; }

    else if (name == "equipCallback_ElCapitan") { return equipCallback_ElCapitan; }

    getCore()->logMessage("Error: bad equipment callback: %s, defaulting to equipCallback_Freejump", str);
    return equipCallback_Freejump;
}



void TournamentInfo::loadMissions(LocationInfo* location, const char* fileName)
{
    assert(location);

    ConfigReader cfg(fileName);
    if (!cfg.IsFileOpen()) {
        getCore()->logMessage("Error: mission file %s not found", fileName);
        return;
    }

    getCore()->logMessage("Info: loading missions: %s", fileName);

    while (cfg.PeekKey()) {
        string key = cfg.PeekKey();
        if (key == "Tournament") {
            TournamentInfo tournamentInfo;

            cfg.BeginGroup("Tournament");
            while (cfg.PeekKey()) {
                string key = cfg.PeekKey();
                if (key == "NameId") { tournamentInfo.nameId = cfg.ReadUInt32("NameId"); }
                else if (key == "BriefingId") { tournamentInfo.briefId = cfg.ReadUInt32("BriefingId"); }
                else if (key == "Type") { tournamentInfo.type = TournamentTypeFromString(cfg.ReadString("Type")); }
                else if (key == "Thumbnail") { tournamentInfo.thumbnail = cfg.ReadString("Thumbnail"); }
                else if (key == "Mission") {
                    MissionInfo missionInfo;
                    if (loadMission(cfg, &missionInfo)) {
                        tournamentInfo.missions.push_back(missionInfo);
                    }
                else if (key == "}") { break; }
                } else {
                    getCore()->logMessage("Error: mission file %s is malformed. Keyword: %s", fileName, key.c_str());
                    break;
                }
            }

            cfg.EndGroup();

            tournamentInfo.gameData = location->gameData;
            tournamentInfo.gameData.append("Tournament");

            location->tournaments.push_back(tournamentInfo);
        } else {
            getCore()->logMessage("Error: mission file %s is malformed. Keyword: %s", fileName, key.c_str());
            break;
        }
    }
}


bool TournamentInfo::loadMission(ConfigReader& cfg, MissionInfo* mission)
{
    mission->missionNode=0;

    cfg.BeginGroup("Mission");
    while (cfg.PeekKey()) {
        string key = cfg.PeekKey();
        if (key == "Rank") { mission->rank = cfg.ReadUInt32("Rank"); }

        else if (key == "NameId") { mission->nameId = cfg.ReadUInt32("NameId"); }
        else if (key == "BriefingId") { mission->briefId = cfg.ReadUInt32("BriefingId"); }

        else if (key == "Flag") { mission->flags |= MissionFlagFromString(cfg.ReadString("Flag")); }

        else if (key == "ExitPointId") { mission->exitPointId = cfg.ReadUInt32("ExitPointId"); }
        else if (key == "MissionTime") { mission->missionTime = cfg.ReadFloat("MissionTime"); }
        else if (key == "Thumbnail") { mission->thumbnail = cfg.ReadString("Thumbnail"); }

        else if (key == "WeatherClearance") { mission->weatherClearance = WeatherClearanceFromString(cfg.ReadString("WeatherClearance")); }
        else if (key == "WindClearance") { mission->windClearance = WindClearanceFromString(cfg.ReadString("WindClearance")); }
        else if (key == "CastingCallback") { mission->castingCallback = MissionCastingCallbackFromString(cfg.ReadString("CastingCallback")); }
        else if (key == "EquipCallback") { mission->equipCallback = EquipCallbackFromString(cfg.ReadString("EquipCallback")); }

        else if (key == "}") { break; }

        else {
            getCore()->logMessage("Error: mission file is malformed. Keyword: %s", key.c_str());
            return false;
        }
    }

    cfg.EndGroup();

    return true;
}
