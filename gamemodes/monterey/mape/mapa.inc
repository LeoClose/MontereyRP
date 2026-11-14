new gMapaInt;

static const gMapaVeliki[] = { 8650, 8661, 14793, 18762, 18763, 18764, 18765, 18766, 18980, 18981, 19325, 19357, 19358, 19376, 19377, 19378, 19379, 19445, 19446, 19447, 19448, 19449, 19450, 19451, 19452, 19453, 19454, 19455, 19456, 19457, 19458, 19459, 19460, 19461, 19462, 19463, 19464, 19466, 19531, 19545, 19548, 19550 };

static MapaPrioritet(model)
{
    for (new i = 0; i < sizeof gMapaVeliki; i++) if (gMapaVeliki[i] == model) return 1;
    return 0;
}

STREAMER_TAG_OBJECT:MapaObjekt(model, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:stream = 300.0, Float:draw = 300.0)
{
    Kolizija_Dodaj(model, x, y, z, rx, ry, rz);
    return CreateDynamicObject(model, x, y, z, rx, ry, rz, -1, gMapaInt, -1, stream, draw, STREAMER_TAG_AREA:-1, MapaPrioritet(model));
}

MapaUkloni(playerid, model, Float:x, Float:y, Float:z, Float:radius)
{
    if (playerid == INVALID_PLAYER_ID) return Kolizija_Ukloni(model, x, y, z, radius);
    return RemoveBuildingForPlayer(playerid, model, x, y, z, radius);
}

#define CreateDynamicObjectEx MapaObjekt
#define RemoveBuildingForPlayer MapaUkloni
