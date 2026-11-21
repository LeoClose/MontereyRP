#define FPS_MODEL           19300
#define FPS_X               0.0
#define FPS_Y               0.15
#define FPS_Z               0.68

static gFpsObj[MAX_PLAYERS] = { INVALID_OBJECT_ID, ... }, bool:gFps[MAX_PLAYERS], bool:gFpsPrisilno[MAX_PLAYERS], bool:gFpsPrije[MAX_PLAYERS];

static FpsPrimijeni(playerid)
{
    if (gFpsObj[playerid] != INVALID_OBJECT_ID)
    {
        DestroyPlayerObject(playerid, gFpsObj[playerid]);
        gFpsObj[playerid] = INVALID_OBJECT_ID;
    }
    if (!gFps[playerid])
    {
        SetCameraBehindPlayer(playerid);
        return;
    }
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    gFpsObj[playerid] = CreatePlayerObject(playerid, FPS_MODEL, x, y, z, 0.0, 0.0, 0.0);
    AttachPlayerObjectToPlayer(playerid, gFpsObj[playerid], playerid, FPS_X, FPS_Y, FPS_Z, 0.0, 0.0, 0.0);
    AttachCameraToPlayerObject(playerid, gFpsObj[playerid]);
}

FpsPrisili(playerid, bool:ukljuci)
{
    if (ukljuci == gFpsPrisilno[playerid]) return;
    gFpsPrisilno[playerid] = ukljuci;
    if (ukljuci) gFpsPrije[playerid] = gFps[playerid];
    gFps[playerid] = ukljuci ? true : gFpsPrije[playerid];
    FpsPrimijeni(playerid);
}

FpsSpawn(playerid)
{
    if (gFps[playerid]) FpsPrimijeni(playerid);
}

FpsDisconnect(playerid)
{
    gFpsObj[playerid] = INVALID_OBJECT_ID;
    gFps[playerid] = false;
    gFpsPrisilno[playerid] = false;
    gFpsPrije[playerid] = false;
}

CMD:fps(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (gFpsPrisilno[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dok radite u kombiju kamera je u prvom licu."));
    gFps[playerid] = !gFps[playerid];
    FpsPrimijeni(playerid);
    return SendClientMessage(playerid, COLOR_INFO, gFps[playerid] ? T(playerid, "Kamera u prvom licu je ukljucena (/fps za iskljucivanje).") : T(playerid, "Kamera u prvom licu je iskljucena."));
}
