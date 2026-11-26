#define MAPA_KAMION_DEPO_RESPAWN    300

static const gMapaKamionDepoModel[] = { 435, 591, 435, 591, 591, 435, 584, 435, 591, 403, 515, 514, 403, 450, 435, 584, 591, 403, 514, 515 };
static const gMapaKamionDepoBoja[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 3, 79, 1, 1, 1, 1, 53, 1, 6 };
static const Float:gMapaKamionDepoPoz[][4] = {
    { 2187.467, -2224.599, 13.55, 45.0 }, { 2194.538, -2217.528, 13.55, 45.0 }, { 2201.821, -2210.245, 13.55, 45.0 }, { 2209.246, -2202.820, 13.55, 45.0 },
    { 2204.633, -2274.414, 13.55, 225.0 }, { 2212.094, -2267.070, 13.55, 225.0 }, { 2219.422, -2259.523, 13.55, 225.0 }, { 2226.969, -2252.141, 13.55, 225.0 }, { 2234.391, -2244.828, 13.55, 225.0 },
    { 2218.888, -2215.307, 13.55, 225.0 }, { 2215.352, -2218.843, 13.55, 225.0 }, { 2211.817, -2222.378, 13.55, 225.0 }, { 2208.281, -2225.914, 13.55, 225.0 },
    { 2083.523, -2159.617, 13.60, 270.0 },
    { 2104.490, -2082.991, 13.55, 312.96 }, { 2107.898, -2086.650, 13.55, 312.96 }, { 2111.305, -2090.310, 13.55, 312.96 },
    { 2114.712, -2093.969, 13.55, 312.96 }, { 2118.120, -2097.628, 13.55, 312.96 }, { 2121.527, -2101.287, 13.55, 312.96 }
};
static gMapaKamionDepoVozilo[sizeof gMapaKamionDepoModel];

MapaKamionDepoUkloni(playerid)
{
    RemoveBuildingForPlayer(playerid, 5244, 2198.852, -2213.922, 14.883, 0.25);
    RemoveBuildingForPlayer(playerid, 5305, 2198.852, -2213.922, 14.883, 0.25);
    RemoveBuildingForPlayer(playerid, 3569, 2219.422, -2259.523, 14.938, 22.0);
    RemoveBuildingForPlayer(playerid, 3747, 2219.422, -2259.523, 14.938, 22.0);
    RemoveBuildingForPlayer(playerid, 3567, 2083.523, -2159.617, 13.258, 0.25);
}

MapaKamionDepo()
{
    for (new i = 0; i < sizeof gMapaKamionDepoModel; i++)
    {
        gMapaKamionDepoVozilo[i] = CreateVehicle(gMapaKamionDepoModel[i], gMapaKamionDepoPoz[i][0], gMapaKamionDepoPoz[i][1], gMapaKamionDepoPoz[i][2], gMapaKamionDepoPoz[i][3], gMapaKamionDepoBoja[i], 1, MAPA_KAMION_DEPO_RESPAWN);
        MapaKamionDepoSpawn(gMapaKamionDepoVozilo[i]);
    }
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
}
