new gMapaInt;

STREAMER_TAG_OBJECT:MapaObjekt(model, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:stream = 300.0, Float:draw = 300.0)
{
    Kolizija_Dodaj(model, x, y, z, rx, ry, rz);
    return CreateDynamicObject(model, x, y, z, rx, ry, rz, -1, gMapaInt, -1, stream, draw);
}

MapaUkloni(playerid, model, Float:x, Float:y, Float:z, Float:radius)
{
    if (playerid == INVALID_PLAYER_ID) return Kolizija_Ukloni(model, x, y, z, radius);
    return RemoveBuildingForPlayer(playerid, model, x, y, z, radius);
}

#define CreateDynamicObjectEx MapaObjekt
#define RemoveBuildingForPlayer MapaUkloni
