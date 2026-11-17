new gMapaInt, bool:gMapaBezSitnih;

static const gMapaVeliki[] = { 8650, 8661, 14793, 18762, 18763, 18764, 18765, 18766, 18980, 18981, 19325, 19357, 19358, 19376, 19377, 19378, 19379, 19445, 19446, 19447, 19448, 19449, 19450, 19451, 19452, 19453, 19454, 19455, 19456, 19457, 19458, 19459, 19460, 19461, 19462, 19463, 19464, 19466, 19531, 19545, 19548, 19550 };

#define MAPA_DALEKO         1500.0

static const gMapaDaleko[] = { 620, 694, 716, 729, 732, 1260, 3509, 3636, 3661, 3783, 3997, 4005, 4028, 4182, 4199, 4565, 4590, 4681, 4810, 4811, 4812, 4813, 4823, 4851, 4852, 4875, 5474, 5859, 6035, 6038, 6039, 6057, 6058, 6113, 6127, 6199, 6203, 6966, 7245, 7315, 7416, 7417, 7418, 7533, 7534, 7586, 7616, 7942, 8408, 8409, 8578, 8640, 8652, 8661, 8673, 9070, 9132, 10975, 11008, 11095, 11504, 16335, 17533, 17549, 18268, 18850, 18980, 18981, 19129, 19532, 19536, 19540, 19542, 19550, 19913 };

static const gMapaSitni[] = { 3509, 8661, 8673 };

static bool:MapaDaleko(model)
{
    if (gMapaBezSitnih) for (new i = 0; i < sizeof gMapaSitni; i++) if (gMapaSitni[i] == model) return false;
    new lo = 0, hi = sizeof gMapaDaleko - 1;
    while (lo <= hi)
    {
        new mid = (lo + hi) / 2;
        if (gMapaDaleko[mid] == model) return true;
        if (gMapaDaleko[mid] < model) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

static MapaPrioritet(model)
{
    for (new i = 0; i < sizeof gMapaVeliki; i++) if (gMapaVeliki[i] == model) return 1;
    return 0;
}

STREAMER_TAG_OBJECT:MapaObjekt(model, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:stream = 300.0, Float:draw = 300.0)
{
    Kolizija_Dodaj(model, x, y, z, rx, ry, rz);
    if (!gMapaInt && MapaDaleko(model)) return CreateDynamicObject(model, x, y, z, rx, ry, rz, -1, 0, -1, MAPA_DALEKO, MAPA_DALEKO, STREAMER_TAG_AREA:-1, 1);
    return CreateDynamicObject(model, x, y, z, rx, ry, rz, -1, gMapaInt, -1, stream, draw, STREAMER_TAG_AREA:-1, MapaPrioritet(model));
}

MapaUkloni(playerid, model, Float:x, Float:y, Float:z, Float:radius)
{
    if (playerid == INVALID_PLAYER_ID) return Kolizija_Ukloni(model, x, y, z, radius);
    return RemoveBuildingForPlayer(playerid, model, x, y, z, radius);
}

#define CreateDynamicObjectEx MapaObjekt
#define RemoveBuildingForPlayer MapaUkloni
