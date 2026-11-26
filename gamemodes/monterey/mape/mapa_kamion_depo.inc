#define MAPA_KAMION_DEPO_RESPAWN    300
#define MAPA_KAMION_LIK_ZONA        4100
#define MAPA_KAMION_VRATA_DIZANJE   3.9
#define MAPA_KAMION_SPUST           -1.55

static const gMapaKamionDepoModel[] = { 435, 591, 435, 591, 591, 435, 584, 435, 591, 450, 435, 584, 591, 435 };
static const Float:gMapaKamionDepoPoz[][4] = {
    { 2187.467, -2224.599, 13.55, 225.0 }, { 2194.538, -2217.528, 13.55, 225.0 }, { 2201.821, -2210.245, 13.55, 225.0 }, { 2209.246, -2202.820, 13.55, 225.0 },
    { 2204.633, -2274.414, 13.55, 45.0 }, { 2212.094, -2267.070, 13.55, 45.0 }, { 2219.422, -2259.523, 13.55, 45.0 }, { 2226.969, -2252.141, 13.55, 45.0 }, { 2234.391, -2244.828, 13.55, 45.0 },
    { 2083.523, -2159.617, 13.60, 90.0 },
    { 2137.638, -2093.028, 13.55, 315.0 }, { 2134.385, -2089.775, 13.55, 315.0 }, { 2131.133, -2086.523, 13.55, 315.0 }, { 2127.880, -2083.270, 13.55, 315.0 }
};
static const Float:gMapaKamionDepoRampa[][5] = {
    { 2176.554, -2251.836, 12.170, 0.0, 225.0 }, { 2172.134, -2247.417, 12.170, 0.0, 225.0 }, { 2167.715, -2242.997, 12.170, 0.0, 225.0 },
    { 2163.296, -2238.578, 12.170, 0.0, 225.0 }, { 2158.876, -2234.159, 12.170, 0.0, 225.0 }, { 2157.869, -2233.151, 12.160, 0.0, 225.0 }
};
static const Float:gMapaKamionVrataPoz[3] = { 2178.074, -2254.240, 15.12 };
static const Float:gMapaKamionUredPoz[4] = { 2133.225, -2278.825, 18.12, 135.0 };
static const Float:gMapaKamionHangar[][5] = {
    { 5171.0, 2124.9453, -2275.4531, 20.1406, 45.00 },
    { 5131.0, 2151.7344, -2273.2969, 18.6562, 315.00 },
    { 3577.0, 2131.3281, -2274.6484, 16.0156, 225.00 },
    { 3577.0, 2131.3438, -2274.6641, 14.4688, 45.00 },
    { 3577.0, 2126.8516, -2270.4453, 14.4688, 45.00 },
    { 3577.0, 2126.8359, -2270.4297, 16.0156, 225.00 },
    { 5129.0, 2148.7812, -2269.6406, 17.3281, 315.00 },
    { 5130.0, 2139.8594, -2271.7812, 16.6797, 0.00 },
    { 3631.0, 2140.0391, -2271.5391, 14.1016, 315.00 },
    { 5268.0, 2143.3047, -2267.4531, 20.2969, 0.00 },
    { 3631.0, 2149.1406, -2266.9062, 12.8750, 315.00 },
    { 5260.0, 2161.3438, -2264.9141, 14.0156, 45.00 },
    { 3631.0, 2161.8516, -2264.0938, 16.3516, 315.00 },
    { 3631.0, 2163.3750, -2262.6875, 16.3516, 315.00 },
    { 3633.0, 2142.9141, -2256.3359, 13.9297, 315.00 },
    { 3632.0, 2144.2969, -2258.1484, 13.9297, 45.00 },
    { 3631.0, 2142.3047, -2255.8984, 12.8750, 315.00 },
    { 5262.0, 2152.7109, -2256.7812, 15.2109, 45.00 },
    { 3633.0, 2158.0078, -2257.2656, 16.2188, 315.00 },
    { 3633.0, 2167.6641, -2256.7812, 12.7500, 315.00 },
    { 3633.0, 2167.6641, -2256.7812, 13.7109, 315.00 },
    { 3633.0, 2167.6641, -2256.7812, 14.6719, 315.00 },
    { 3632.0, 2167.8047, -2257.3516, 16.3828, 45.00 },
    { 3632.0, 2167.1719, -2257.1250, 16.4062, 45.00 },
    { 3577.0, 2170.0781, -2257.6641, 16.0391, 45.00 },
    { 3632.0, 2169.3516, -2258.0703, 17.2422, 45.00 },
    { 3632.0, 2168.8281, -2257.5234, 17.2500, 45.00 },
    { 3633.0, 2140.3828, -2254.1016, 13.9297, 315.00 },
    { 3632.0, 2150.6641, -2251.5547, 12.7656, 45.00 },
    { 3632.0, 2150.2812, -2250.8516, 12.7656, 45.00 },
    { 3633.0, 2150.6953, -2252.9141, 16.2344, 315.00 },
    { 3632.0, 2149.8125, -2253.3672, 16.2344, 45.00 },
    { 5261.0, 2152.2578, -2239.4609, 14.5000, 45.00 },
    { 5269.0, 2146.3750, -2248.7969, 14.6172, 45.00 },
    { 3633.0, 2153.7734, -2253.0859, 14.2031, 315.00 },
    { 3633.0, 2154.5078, -2254.4766, 14.2109, 315.00 },
    { 3632.0, 2158.5703, -2251.0156, 15.8125, 45.00 },
    { 3632.0, 2158.0469, -2250.5078, 15.8125, 45.00 },
    { 5132.0, 2163.2891, -2251.6094, 14.1406, 45.00 }
};
static const Float:gMapaKamionDepoMakni[][5] = {
    { 5244.0, 2198.852, -2213.922, 14.883, 0.25 }, { 5305.0, 2198.852, -2213.922, 14.883, 0.25 },
    { 3569.0, 2219.422, -2259.523, 14.938, 22.0 }, { 3747.0, 2219.422, -2259.523, 14.938, 22.0 },
    { 3567.0, 2083.523, -2159.617, 13.258, 0.25 }, { 5215.0, 2151.734, -2273.297, 18.656, 0.25 },
    { 5259.0, 2168.844, -2246.781, 13.938, 0.5 }, { 3577.0, 2160.562, -2234.805, 14.562, 0.5 }, { 3577.0, 2160.578, -2234.820, 13.023, 0.5 }
};
static gMapaKamionDepoVozilo[sizeof gMapaKamionDepoModel];
static STREAMER_TAG_OBJECT:gMapaKamionVrata;

MapaKamionDepoUkloni(playerid)
{
    for (new i = 0; i < sizeof gMapaKamionDepoMakni; i++)
        RemoveBuildingForPlayer(playerid, floatround(gMapaKamionDepoMakni[i][0]), gMapaKamionDepoMakni[i][1], gMapaKamionDepoMakni[i][2], gMapaKamionDepoMakni[i][3], gMapaKamionDepoMakni[i][4]);
    for (new i = 0; i < sizeof gMapaKamionHangar; i++)
        RemoveBuildingForPlayer(playerid, floatround(gMapaKamionHangar[i][0]), gMapaKamionHangar[i][1], gMapaKamionHangar[i][2], gMapaKamionHangar[i][3], 0.25);
}

MapaKamionDepo()
{
    for (new i = 0; i < sizeof gMapaKamionDepoModel; i++)
    {
        gMapaKamionDepoVozilo[i] = CreateVehicle(gMapaKamionDepoModel[i], gMapaKamionDepoPoz[i][0], gMapaKamionDepoPoz[i][1], gMapaKamionDepoPoz[i][2], gMapaKamionDepoPoz[i][3], 1, 1, MAPA_KAMION_DEPO_RESPAWN);
        MapaKamionDepoSpawn(gMapaKamionDepoVozilo[i]);
    }
    for (new i = 0; i < sizeof gMapaKamionDepoMakni; i++)
        Kolizija_Ukloni(floatround(gMapaKamionDepoMakni[i][0]), gMapaKamionDepoMakni[i][1], gMapaKamionDepoMakni[i][2], gMapaKamionDepoMakni[i][3], gMapaKamionDepoMakni[i][4]);
    for (new i = 0; i < sizeof gMapaKamionHangar; i++)
    {
        new m = floatround(gMapaKamionHangar[i][0]), Float:z = gMapaKamionHangar[i][3] + MAPA_KAMION_SPUST;
        Kolizija_Ukloni(m, gMapaKamionHangar[i][1], gMapaKamionHangar[i][2], gMapaKamionHangar[i][3], 0.25);
        CreateDynamicObject(m, gMapaKamionHangar[i][1], gMapaKamionHangar[i][2], z, 0.0, 0.0, gMapaKamionHangar[i][4], 0, 0, -1, 300.0, 300.0);
        Kolizija_Dodaj(m, gMapaKamionHangar[i][1], gMapaKamionHangar[i][2], z, 0.0, 0.0, gMapaKamionHangar[i][4]);
    }
    for (new i = 0; i < sizeof gMapaKamionDepoRampa; i++)
    {
        new STREAMER_TAG_OBJECT:o = CreateDynamicObject(19741, gMapaKamionDepoRampa[i][0], gMapaKamionDepoRampa[i][1], gMapaKamionDepoRampa[i][2], gMapaKamionDepoRampa[i][3], 0.0, gMapaKamionDepoRampa[i][4], 0, 0);
        SetDynamicObjectMaterial(o, 0, 5131, "imrancomp_las2", "ws_carpark2", 0);
        Kolizija_Dodaj(19741, gMapaKamionDepoRampa[i][0], gMapaKamionDepoRampa[i][1], gMapaKamionDepoRampa[i][2], gMapaKamionDepoRampa[i][3], 0.0, gMapaKamionDepoRampa[i][4]);
    }
    gMapaKamionVrata = CreateDynamicObject(8957, gMapaKamionVrataPoz[0], gMapaKamionVrataPoz[1], gMapaKamionVrataPoz[2], 0.0, 0.0, 315.0, 0, 0);
    SetDynamicObjectMaterial(gMapaKamionVrata, 0, 5131, "imrancomp_las2", "alleydoor8", 0);
    Lik_Zona(MAPA_KAMION_LIK_ZONA, 2110.0, -2300.0, 2245.0, -2195.0, 0.0);
    Lik_Zona(MAPA_KAMION_LIK_ZONA + 1, 2100.0, -2105.0, 2150.0, -2060.0, 0.0);
    CreateDynamicObject(1498, gMapaKamionUredPoz[0], gMapaKamionUredPoz[1], gMapaKamionUredPoz[2], 0.0, 0.0, gMapaKamionUredPoz[3], 0, 0);
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
}
