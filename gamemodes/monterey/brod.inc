#define BROD_ODMAK          650.0
#define BROD_DALEKO         1200.0

static const Float:gBrodDio[][5] = {
    { 5156.0, 2838.039, -2423.883, 10.961, -90.0 },
    { 5160.0, 2829.953, -2479.570, 5.266, -90.0 },
    { 5166.0, 2829.953, -2479.570, 5.266, -90.0 },
    { 5167.0, 2838.031, -2371.953, 7.297, -90.0 },
    { 5157.0, 2838.039, -2532.773, 17.023, -90.0 },
    { 5154.0, 2838.141, -2447.844, 15.750, -90.0 },
    { 3724.0, 2838.195, -2488.664, 29.312, 90.0 },
    { 5155.0, 2838.023, -2358.477, 21.312, -90.0 },
    { 3724.0, 2838.195, -2407.141, 29.312, -90.0 },
    { 5158.0, 2837.773, -2334.477, 11.992, 0.0 }
};
static const Float:gBrodFaze[][2] = {
    { 180.0, 9.0 },
    { 50.0, 5.0 },
    { 8.0, 2.0 },
    { 0.0, 0.7 }
};

static STREAMER_TAG_OBJECT:gBrod[sizeof gBrodDio], gBrodFaza = -1, bool:gBrodOdlazi;

BrodUkloni(playerid)
{
    for (new d = 0; d < sizeof gBrodDio; d++) RemoveBuildingForPlayer(playerid, floatround(gBrodDio[d][0]), gBrodDio[d][1], gBrodDio[d][2], gBrodDio[d][3], 0.5);
    RemoveBuildingForPlayer(playerid, 5159, 2838.03, -2371.95, 7.3, 0.5);
    RemoveBuildingForPlayer(playerid, 5161, 2838.02, -2358.48, 21.31, 0.5);
    RemoveBuildingForPlayer(playerid, 5162, 2838.04, -2423.88, 10.96, 0.5);
    RemoveBuildingForPlayer(playerid, 5163, 2838.04, -2532.77, 17.02, 0.5);
    RemoveBuildingForPlayer(playerid, 5164, 2838.14, -2447.84, 15.73, 0.5);
    RemoveBuildingForPlayer(playerid, 5165, 2838.03, -2520.19, 18.41, 0.5);
    RemoveBuildingForPlayer(playerid, 5335, 2829.95, -2479.57, 5.27, 0.5);
    RemoveBuildingForPlayer(playerid, 5336, 2829.95, -2479.57, 5.27, 0.5);
    RemoveBuildingForPlayer(playerid, 5352, 2838.2, -2488.66, 29.31, 0.5);
}

static BrodPomakni(Float:odmak, Float:brzina)
{
    for (new d = 0; d < sizeof gBrodDio; d++) MoveDynamicObject(gBrod[d], gBrodDio[d][1], gBrodDio[d][2] - odmak, gBrodDio[d][3], brzina);
}

#define BROD_CIKLUS_S       900
#define BROD_PRISTAN_S      300

static gBrodSlot, gBrodOdlazak, bool:gBrodPristao;

BrodSljedeciDolazak()
{
    new sad = gettime(), slot = (sad / BROD_CIKLUS_S + 1) * BROD_CIKLUS_S;
    if (gBrodFaza >= 0 && gBrodSlot == slot - BROD_CIKLUS_S && !gBrodPristao) return slot - BROD_CIKLUS_S;
    return slot;
}

static BrodDolazi(slot)
{
    gBrodSlot = slot;
    gBrodOdlazi = false;
    gBrodPristao = false;
    gBrodFaza = 0;
    for (new d = 0; d < sizeof gBrodDio; d++)
        gBrod[d] = CreateDynamicObject(floatround(gBrodDio[d][0]), gBrodDio[d][1], gBrodDio[d][2] - BROD_ODMAK, gBrodDio[d][3], 0.0, 0.0, gBrodDio[d][4], 0, 0, -1, BROD_DALEKO, BROD_DALEKO, STREAMER_TAG_AREA:-1, 1);
    BrodPomakni(gBrodFaze[0][0], gBrodFaze[0][1]);
}

BrodTick()
{
    new sad = gettime(), slot = sad / BROD_CIKLUS_S * BROD_CIKLUS_S;
    if (gBrodFaza < 0 && slot != gBrodSlot && sad - slot < BROD_PRISTAN_S) BrodDolazi(slot);
    else if (gBrodPristao && !gBrodOdlazi && sad >= gBrodOdlazak)
    {
        gBrodOdlazi = true;
        gBrodPristao = false;
        gBrodFaza = 0;
        BrodPomakni(BROD_ODMAK, 4.0);
    }
}

public OnDynamicObjectMoved(STREAMER_TAG_OBJECT:objectid)
{
    if (gBrodFaza < 0 || objectid != gBrod[0]) return 0;
    if (gBrodOdlazi)
    {
        for (new d = 0; d < sizeof gBrodDio; d++) DestroyDynamicObject(gBrod[d]);
        gBrodFaza = -1;
        return 1;
    }
    if (++gBrodFaza < sizeof gBrodFaze) BrodPomakni(gBrodFaze[gBrodFaza][0], gBrodFaze[gBrodFaza][1]);
    else
    {
        gBrodPristao = true;
        gBrodOdlazak = gettime() + BROD_PRISTAN_S;
        LukaPristao(gBrodSlot);
    }
    return 1;
}
