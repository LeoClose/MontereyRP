#define VLAK_BROJ           2
#define VLAK_MODEL          538
#define VLAK_VAGON          570
#define VLAK_SKIN           7
#define VLAK_START_MS       5000
#define VLAK_RAZMAK_MS      300000
#define VLAK_PAUZA_MS       600000
#define VLAK_IZVAN_LS_MS    60000
#define VLAK_LS_MIN_X       44.6
#define VLAK_LS_MAX_Y       -768.0

// Vlak krece s Unity stanice, prolazi Market i odlazi prema SF-u gdje nestaje
static const Float:gVlakStart[4] = { 1700.7551, -1953.6531, 14.8756, 190.0 };
static const gVlakZapis[][] = { "TRAIN_UNITY_TO_MARKET", "TRAIN_MARKET_TO_CRANBERRY" };

static gVlakNpc[VLAK_BROJ], gVlakVozilo[VLAK_BROJ], gVlakDio[VLAK_BROJ], gVlakIzvan[VLAK_BROJ], gNpcVlak[MAX_PLAYERS];

InitVlakovi()
{
    for (new i = 0; i < VLAK_BROJ; i++) SetTimerEx("VlakStvori", VLAK_START_MS + i * VLAK_RAZMAK_MS, false, "i", i);
}

forward VlakStvori(i);
public VlakStvori(i)
{
    new name[MAX_PLAYER_NAME + 1];
    format(name, sizeof name, "Strojovoda_%d", i);
    new npc = NPC_Create(name);
    if (npc == INVALID_PLAYER_ID) return;
    new v = CreateVehicle(VLAK_MODEL, gVlakStart[0], gVlakStart[1], gVlakStart[2], gVlakStart[3], -1, -1, -1);
    if (v == INVALID_VEHICLE_ID)
    {
        NPC_Destroy(npc);
        return;
    }
    gVlakNpc[i] = npc;
    gVlakVozilo[i] = v;
    gVlakDio[i] = 0;
    gVlakIzvan[i] = 0;
    gNpcVlak[npc] = i + 1;
    NPC_Spawn(npc);
}

VlakSpawn(npcid)
{
    new i = gNpcVlak[npcid] - 1;
    if (i < 0) return;
    NPC_SetSkin(npcid, VLAK_SKIN);
    NPC_SetInvulnerable(npcid, true);
    NPC_PutInVehicle(npcid, gVlakVozilo[i], 0);
    SetTimerEx("VlakVozi", 1000, false, "i", i);
}

forward VlakVozi(i);
public VlakVozi(i)
{
    if (gVlakVozilo[i]) NPC_StartPlayback(gVlakNpc[i], gVlakZapis[gVlakDio[i]]);
}

static VlakNestani(i)
{
    gNpcVlak[gVlakNpc[i]] = 0;
    NPC_Destroy(gVlakNpc[i]);
    DestroyVehicle(gVlakVozilo[i]);
    gVlakVozilo[i] = 0;
    SetTimerEx("VlakStvori", VLAK_PAUZA_MS, false, "i", i);
}

VlakPlaybackEnd(npcid)
{
    new i = gNpcVlak[npcid] - 1;
    if (i < 0) return 0;
    if (++gVlakDio[i] < sizeof gVlakZapis) VlakVozi(i);
    else VlakNestani(i);
    return 1;
}

VlakTick()
{
    new Float:x, Float:y, Float:z, sad = GetTickCount();
    for (new i = 0; i < VLAK_BROJ; i++)
    {
        if (!gVlakVozilo[i]) continue;
        NPC_GetPos(gVlakNpc[i], x, y, z);
        if (x > VLAK_LS_MIN_X && y < VLAK_LS_MAX_Y) gVlakIzvan[i] = 0;
        else if (!gVlakIzvan[i]) gVlakIzvan[i] = sad;
        else if (sad - gVlakIzvan[i] >= VLAK_IZVAN_LS_MS) VlakNestani(i);
    }
}

static bool:JeVlak(vehicleid)
{
    new m = GetVehicleModel(vehicleid);
    return m == VLAK_MODEL || m == VLAK_VAGON;
}

// Igraci ne smiju u vlak ni kao vozac ni kao putnik
VlakEnter(playerid, vehicleid)
{
    if (IsPlayerNPC(playerid) || !JeVlak(vehicleid)) return 0;
    ClearAnimations(playerid);
    return 1;
}

VlakStateChange(playerid, PLAYER_STATE:newstate)
{
    if (IsPlayerNPC(playerid) || (newstate != PLAYER_STATE_DRIVER && newstate != PLAYER_STATE_PASSENGER)) return;
    if (JeVlak(GetPlayerVehicleID(playerid))) RemovePlayerFromVehicle(playerid);
}
