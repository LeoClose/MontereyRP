#define AVION_SLOTOVA       2
#define AVION_SKIN          61
#define AVION_START_MS      90000
#define AVION_MIN_MS        180000
#define AVION_MAX_MS        420000

enum E_AVION_RUTA { arRuta, arModel, Float:arX, Float:arY, Float:arZ, Float:arA }
static const gAvionRuta[][E_AVION_RUTA] = {
    { 0, 577, 2135.000, -2447.000, 13.650, 90.00 },
    { 0, 592, 2135.000, -2447.000, 14.850, 90.00 },
    { 0, 519, 2135.000, -2447.000, 14.750, 90.00 },
    { 0, 593, 2135.000, -2447.000, 14.250, 90.00 },
    { 0, 511, 2135.000, -2447.000, 15.150, 90.00 },
    { 1, 577, 2135.000, -2447.000, 13.650, 90.00 },
    { 1, 592, 2135.000, -2447.000, 14.850, 90.00 },
    { 1, 519, 2135.000, -2447.000, 14.750, 90.00 },
    { 1, 593, 2135.000, -2447.000, 14.250, 90.00 },
    { 1, 511, 2135.000, -2447.000, 15.150, 90.00 },
    { 2, 577, 2135.000, -2447.000, 13.650, 90.00 },
    { 2, 592, 2135.000, -2447.000, 14.850, 90.00 },
    { 2, 519, 2135.000, -2447.000, 14.750, 90.00 },
    { 2, 593, 2135.000, -2447.000, 14.250, 90.00 },
    { 2, 511, 2135.000, -2447.000, 15.150, 90.00 },
    { 3, 577, -1350.000, -250.000, 361.100, 196.93 },
    { 3, 592, -1350.000, -250.000, 362.300, 196.93 },
    { 3, 519, -1350.000, -250.000, 262.200, 196.93 },
    { 3, 593, -1350.000, -250.000, 261.700, 196.93 },
    { 3, 511, -1350.000, -250.000, 262.600, 196.93 },
    { 4, 577, 1600.000, 1200.000, 361.100, 132.71 },
    { 4, 592, 1600.000, 1200.000, 362.300, 132.71 },
    { 4, 519, 1600.000, 1200.000, 262.200, 132.71 },
    { 4, 593, 1600.000, 1200.000, 261.700, 132.71 },
    { 4, 511, 1600.000, 1200.000, 262.600, 132.71 },
    { 5, 577, 900.000, -4300.000, 361.100, 60.26 },
    { 5, 592, 900.000, -4300.000, 362.300, 60.26 },
    { 5, 519, 900.000, -4300.000, 262.200, 60.26 },
    { 5, 593, 900.000, -4300.000, 261.700, 60.26 },
    { 5, 511, 900.000, -4300.000, 262.600, 60.26 },
    { 6, 519, 300.000, -3600.000, 222.200, 335.22 },
    { 6, 593, 300.000, -3600.000, 221.700, 335.22 },
    { 6, 511, 300.000, -3600.000, 222.600, 335.22 },
    { 7, 519, 1700.000, 1100.000, 242.200, 203.43 },
    { 7, 593, 1700.000, 1100.000, 241.700, 203.43 },
    { 7, 511, 1700.000, 1100.000, 242.600, 203.43 }
};

static gAvionNpc[AVION_SLOTOVA], gAvionVozilo[AVION_SLOTOVA], gAvionTrenutna[AVION_SLOTOVA], gNpcAvion[MAX_PLAYERS];

InitAvioni()
{
    for (new i = 0; i < AVION_SLOTOVA; i++) SetTimerEx("AvionStvori", AVION_START_MS + i * AVION_MIN_MS, false, "i", i);
}

static AvionZakazi(i)
{
    SetTimerEx("AvionStvori", AVION_MIN_MS + random(AVION_MAX_MS - AVION_MIN_MS), false, "i", i);
}

static bool:AvionGrupaZauzeta(i, r)
{
    for (new j = 0; j < AVION_SLOTOVA; j++)
        if (j != i && gAvionVozilo[j] && gAvionRuta[gAvionTrenutna[j]][arRuta] / 3 == gAvionRuta[r][arRuta] / 3) return true;
    return false;
}

forward AvionStvori(i);
public AvionStvori(i)
{
    new r = random(sizeof gAvionRuta);
    if (AvionGrupaZauzeta(i, r))
    {
        AvionZakazi(i);
        return;
    }
    new name[MAX_PLAYER_NAME + 1];
    format(name, sizeof name, "Kapetan_%d", i);
    new npc = NPC_Create(name);
    if (npc == INVALID_PLAYER_ID)
    {
        AvionZakazi(i);
        return;
    }
    new v = CreateVehicle(gAvionRuta[r][arModel], gAvionRuta[r][arX], gAvionRuta[r][arY], gAvionRuta[r][arZ], gAvionRuta[r][arA], -1, -1, -1);
    if (v == INVALID_VEHICLE_ID)
    {
        NPC_Destroy(npc);
        AvionZakazi(i);
        return;
    }
    gAvionNpc[i] = npc;
    gAvionVozilo[i] = v;
    gAvionTrenutna[i] = r;
    gNpcAvion[npc] = i + 1;
    NPC_Spawn(npc);
}

AvionSpawn(npcid)
{
    new i = gNpcAvion[npcid] - 1;
    if (i < 0) return;
    NPC_SetSkin(npcid, AVION_SKIN);
    NPC_SetInvulnerable(npcid, true);
    SetVehicleParamsEx(gAvionVozilo[i], 1, 1, 0, 0, 0, 0, 0);
    NPC_PutInVehicle(npcid, gAvionVozilo[i], 0);
    SetTimerEx("AvionLeti", 1000, false, "i", i);
}

forward AvionLeti(i);
public AvionLeti(i)
{
    if (!gAvionVozilo[i]) return;
    new ime[16];
    format(ime, sizeof ime, "av_%d_%d", gAvionRuta[gAvionTrenutna[i]][arRuta], gAvionRuta[gAvionTrenutna[i]][arModel]);
    NPC_StartPlayback(gAvionNpc[i], ime);
}

static AvionKraj(i)
{
    gNpcAvion[gAvionNpc[i]] = 0;
    NPC_Destroy(gAvionNpc[i]);
    DestroyVehicle(gAvionVozilo[i]);
    gAvionVozilo[i] = 0;
    AvionZakazi(i);
}

AvionPlaybackEnd(npcid)
{
    new i = gNpcAvion[npcid] - 1;
    if (i < 0) return 0;
    AvionKraj(i);
    return 1;
}

AvionMrtav(v)
{
    for (new i = 0; i < AVION_SLOTOVA; i++)
    {
        if (!gAvionVozilo[i] || gAvionVozilo[i] != v) continue;
        AvionKraj(i);
        return 1;
    }
    return 0;
}

AvionEnter(playerid, vehicleid)
{
    if (IsPlayerNPC(playerid)) return 0;
    for (new i = 0; i < AVION_SLOTOVA; i++)
    {
        if (!gAvionVozilo[i] || gAvionVozilo[i] != vehicleid) continue;
        ClearAnimations(playerid);
        return 1;
    }
    return 0;
}
