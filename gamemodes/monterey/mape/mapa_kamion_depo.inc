#define MAPA_KAMION_DEPO_RESPAWN    300
#define MAPA_KAMION_VRATA_DIZANJE   3.9
#define MAPA_KAMION_UR_DOMET        2.2

static const gMapaKamionDepoModel[] = { 435, 591, 435, 591, 591, 435, 584, 435, 591, 450, 435, 584, 591, 435 };
static const Float:gMapaKamionDepoPoz[][4] = {
    { 2187.467, -2224.599, 13.55, 225.0 }, { 2194.538, -2217.528, 13.55, 225.0 }, { 2201.821, -2210.245, 13.55, 225.0 }, { 2209.246, -2202.820, 13.55, 225.0 },
    { 2204.633, -2274.414, 13.55, 45.0 }, { 2212.094, -2267.070, 13.55, 45.0 }, { 2219.422, -2259.523, 13.55, 45.0 }, { 2226.969, -2252.141, 13.55, 45.0 }, { 2234.391, -2244.828, 13.55, 45.0 },
    { 2083.523, -2159.617, 13.60, 90.0 },
    { 2137.638, -2093.028, 13.55, 315.0 }, { 2134.385, -2089.775, 13.55, 315.0 }, { 2131.133, -2086.523, 13.55, 315.0 }, { 2127.880, -2083.270, 13.55, 315.0 }
};
static const Float:gMapaKamionDepoRampa[][5] = {
    { 2171.385, -2247.551, 12.627, 6.754, 225.0 }, { 2175.774, -2251.940, 13.362, 6.754, 225.0 },
    { 2182.995, -2259.161, 13.417, -5.739, 225.0 }, { 2187.392, -2263.558, 12.792, -5.739, 225.0 }
};
static const Float:gMapaKamionVrataPoz[3] = { 2178.074, -2254.240, 16.67 };
static const Float:gMapaKamionUredPoz[4] = { 2133.225, -2278.825, 19.67, 135.0 };
static gMapaKamionDepoVozilo[sizeof gMapaKamionDepoModel];
static STREAMER_TAG_OBJECT:gMapaKamionVrata, STREAMER_TAG_OBJECT:gMapaKamionUred, bool:gMapaKamionUredOtvoren;

MapaKamionDepoUkloni(playerid)
{
    RemoveBuildingForPlayer(playerid, 5244, 2198.852, -2213.922, 14.883, 0.25);
    RemoveBuildingForPlayer(playerid, 5305, 2198.852, -2213.922, 14.883, 0.25);
    RemoveBuildingForPlayer(playerid, 3569, 2219.422, -2259.523, 14.938, 22.0);
    RemoveBuildingForPlayer(playerid, 3747, 2219.422, -2259.523, 14.938, 22.0);
    RemoveBuildingForPlayer(playerid, 3567, 2083.523, -2159.617, 13.258, 0.25);
    RemoveBuildingForPlayer(playerid, 3631, 2149.140, -2266.910, 12.880, 0.6);
    RemoveBuildingForPlayer(playerid, 3631, 2142.300, -2255.900, 12.880, 0.6);
    RemoveBuildingForPlayer(playerid, 3633, 2142.910, -2256.340, 13.930, 0.6);
    RemoveBuildingForPlayer(playerid, 3633, 2140.380, -2254.100, 13.930, 0.6);
    RemoveBuildingForPlayer(playerid, 3632, 2144.300, -2258.150, 13.930, 0.6);
}

MapaKamionDepo()
{
    for (new i = 0; i < sizeof gMapaKamionDepoModel; i++)
    {
        gMapaKamionDepoVozilo[i] = CreateVehicle(gMapaKamionDepoModel[i], gMapaKamionDepoPoz[i][0], gMapaKamionDepoPoz[i][1], gMapaKamionDepoPoz[i][2], gMapaKamionDepoPoz[i][3], 1, 1, MAPA_KAMION_DEPO_RESPAWN);
        MapaKamionDepoSpawn(gMapaKamionDepoVozilo[i]);
    }
    for (new i = 0; i < sizeof gMapaKamionDepoRampa; i++)
    {
        new STREAMER_TAG_OBJECT:o = CreateDynamicObject(19741, gMapaKamionDepoRampa[i][0], gMapaKamionDepoRampa[i][1], gMapaKamionDepoRampa[i][2], gMapaKamionDepoRampa[i][3], 0.0, gMapaKamionDepoRampa[i][4], 0, 0);
        SetDynamicObjectMaterial(o, 0, 5131, "imrancomp_las2", "ws_carpark2", 0);
    }
    gMapaKamionVrata = CreateDynamicObject(8957, gMapaKamionVrataPoz[0], gMapaKamionVrataPoz[1], gMapaKamionVrataPoz[2], 0.0, 0.0, 315.0, 0, 0);
    SetDynamicObjectMaterial(gMapaKamionVrata, 0, 5131, "imrancomp_las2", "alleydoor8", 0);
    gMapaKamionUred = CreateDynamicObject(1498, gMapaKamionUredPoz[0], gMapaKamionUredPoz[1], gMapaKamionUredPoz[2], 0.0, 0.0, gMapaKamionUredPoz[3], 0, 0);
}

MapaKamionDepoVrata(bool:otvori, bool:brzo = false)
{
    MoveDynamicObject(gMapaKamionVrata, gMapaKamionVrataPoz[0], gMapaKamionVrataPoz[1], gMapaKamionVrataPoz[2] + (otvori ? MAPA_KAMION_VRATA_DIZANJE : 0.0), otvori ? 1.6 : (brzo ? 6.0 : 2.5));
}

MapaKamionDepoSpawn(v)
{
    for (new i = 0; i < sizeof gMapaKamionDepoVozilo; i++)
    {
        if (gMapaKamionDepoVozilo[i] != v || v == INVALID_VEHICLE_ID) continue;
        VoziloZakljucaj(v, true);
        return;
    }
}

MapaKamionDepoTick()
{
    for (new i = 0; i < sizeof gMapaKamionDepoVozilo; i++)
    {
        new v = gMapaKamionDepoVozilo[i], c = GetVehicleCab(v);
        if (!c || c == INVALID_VEHICLE_ID) continue;
        DetachTrailerFromVehicle(c);
        SetVehicleToRespawn(v);
    }
    new bool:blizu = false;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji && !blizu; i++)
        blizu = IsPlayerConnected(i) && !GetPlayerVirtualWorld(i) && IsPlayerInRangeOfPoint(i, MAPA_KAMION_UR_DOMET, gMapaKamionUredPoz[0], gMapaKamionUredPoz[1], gMapaKamionUredPoz[2] + 1.0);
    if (blizu == gMapaKamionUredOtvoren) return;
    gMapaKamionUredOtvoren = blizu;
    MoveDynamicObject(gMapaKamionUred, gMapaKamionUredPoz[0], gMapaKamionUredPoz[1], gMapaKamionUredPoz[2] + (blizu ? 0.01 : 0.0), 0.016, 0.0, 0.0, gMapaKamionUredPoz[3] + (blizu ? 90.0 : 0.0));
}
