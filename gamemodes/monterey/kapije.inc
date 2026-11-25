#define KAPIJA_MAX          128
#define KAPIJA_DOMET        8.0
#define KAPIJA_OTVORENA_MS  8000
#define KAPIJA_SEKUNDI      2.5

enum { KT_KLIZI = 1, KT_RAMPA, KT_RAMPA_X, KT_OKRET, KT_DIZI }
enum E_KAPIJA { kpDb, kpOrg, kpModel, kpTip, Float:kpVrijednost, STREAMER_TAG_OBJECT:kpObjekt, Float:kpX, Float:kpY, Float:kpZ, Float:kpRx, Float:kpRy, Float:kpRz, bool:kpOtvorena, kpToken }
static gKapija[KAPIJA_MAX][E_KAPIJA], gKapijaBroj;

STREAMER_TAG_OBJECT:ObjektBlizu(Float:x, Float:y, Float:z, Float:r, model = -1)
{
    new STREAMER_TAG_OBJECT:objekti[128], n = Streamer_GetNearbyItems(x, y, z, STREAMER_TYPE_OBJECT, objekti, sizeof objekti, r), STREAMER_TAG_OBJECT:naj = STREAMER_TAG_OBJECT:0, Float:nd = r, Float:ox, Float:oy, Float:oz;
    for (new i = 0; i < n; i++)
    {
        if (model != -1 && Streamer_GetIntData(STREAMER_TYPE_OBJECT, objekti[i], E_STREAMER_MODEL_ID) != model) continue;
        GetDynamicObjectPos(objekti[i], ox, oy, oz);
        new Float:d = VectorSize(ox - x, oy - y, oz - z);
        if (d >= nd) continue;
        nd = d;
        naj = objekti[i];
    }
    return naj;
}

ObjektPomakni(STREAMER_TAG_OBJECT:o, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:sekundi)
{
    new Float:cx, Float:cy, Float:cz;
    GetDynamicObjectPos(o, cx, cy, cz);
    new Float:d = VectorSize(x - cx, y - cy, z - cz);
    if (d < 0.001)
    {
        z += cz > z ? -0.01 : 0.01;
        d = 0.01;
    }
    MoveDynamicObject(o, x, y, z, d / sekundi, rx, ry, rz);
}

InitKapije()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS gates (id INTEGER PRIMARY KEY AUTOINCREMENT, org INTEGER NOT NULL, model INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, tip INTEGER NOT NULL, vrijednost REAL NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, org, model, x, y, z, tip, vrijednost FROM gates ORDER BY id LIMIT %d", KAPIJA_MAX);
    if (DB_GetRowCount(res)) do
    {
        new k = gKapijaBroj, STREAMER_TAG_OBJECT:o = ObjektBlizu(DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), DB_GetFieldFloat(res, 5), 0.5, DB_GetFieldInt(res, 2));
        if (!o) continue;
        gKapija[k][kpDb] = DB_GetFieldInt(res, 0);
        gKapija[k][kpOrg] = DB_GetFieldInt(res, 1);
        gKapija[k][kpModel] = DB_GetFieldInt(res, 2);
        gKapija[k][kpTip] = DB_GetFieldInt(res, 6);
        gKapija[k][kpVrijednost] = DB_GetFieldFloat(res, 7);
        KapijaPostavi(k, o);
        gKapijaBroj++;
    }
    while (gKapijaBroj < KAPIJA_MAX && DB_SelectNextRow(res));
    DB_FreeResultSet(res);
}

static KapijaPostavi(k, STREAMER_TAG_OBJECT:o)
{
    gKapija[k][kpObjekt] = o;
    gKapija[k][kpOtvorena] = false;
    GetDynamicObjectPos(o, gKapija[k][kpX], gKapija[k][kpY], gKapija[k][kpZ]);
    GetDynamicObjectRot(o, gKapija[k][kpRx], gKapija[k][kpRy], gKapija[k][kpRz]);
}

static KapijaPomakni(k, bool:otvori)
{
    new Float:x = gKapija[k][kpX], Float:y = gKapija[k][kpY], Float:z = gKapija[k][kpZ], Float:rx = gKapija[k][kpRx], Float:ry = gKapija[k][kpRy], Float:rz = gKapija[k][kpRz], Float:v = gKapija[k][kpVrijednost];
    gKapija[k][kpOtvorena] = otvori;
    if (otvori) switch (gKapija[k][kpTip])
    {
        case KT_KLIZI:
        {
            x += floatcos(rz, degrees) * v;
            y += floatsin(rz, degrees) * v;
        }
        case KT_RAMPA: ry += v;
        case KT_RAMPA_X: rx += v;
        case KT_OKRET: rz += v;
        case KT_DIZI: z += v;
    }
    ObjektPomakni(gKapija[k][kpObjekt], x, y, z, rx, ry, rz, KAPIJA_SEKUNDI);
}

forward KapijaZatvori(k, token);
public KapijaZatvori(k, token)
{
    if (token == gKapija[k][kpToken] && gKapija[k][kpOtvorena]) KapijaPomakni(k, false);
}

KapijaKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gUlogiran[playerid] || !gOrg[playerid]) return 0;
    new PLAYER_STATE:st = GetPlayerState(playerid), KEY:tipka = st == PLAYER_STATE_ONFOOT ? KEY_WALK : KEY_FIRE;
    if ((st != PLAYER_STATE_ONFOOT && st != PLAYER_STATE_DRIVER) || !(newkeys & tipka) || (oldkeys & tipka)) return 0;
    for (new k = 0; k < gKapijaBroj; k++)
    {
        if (gKapija[k][kpOrg] != gOrg[playerid] || !IsPlayerInRangeOfPoint(playerid, KAPIJA_DOMET, gKapija[k][kpX], gKapija[k][kpY], gKapija[k][kpZ])) continue;
        KapijaPomakni(k, !gKapija[k][kpOtvorena]);
        if (gKapija[k][kpOtvorena]) SetTimerEx("KapijaZatvori", KAPIJA_OTVORENA_MS, false, "ii", k, ++gKapija[k][kpToken]);
        return 1;
    }
    return 0;
}

static gKapijaSavjet[MAX_PLAYERS] = { -1, ... };

KapijaSavjet(playerid)
{
    new naj = -1;
    if (gOrg[playerid])
        for (new k = 0; k < gKapijaBroj && naj == -1; k++)
            if (gKapija[k][kpOrg] == gOrg[playerid] && IsPlayerInRangeOfPoint(playerid, KAPIJA_DOMET, gKapija[k][kpX], gKapija[k][kpY], gKapija[k][kpZ])) naj = k;
    if (naj != -1 && naj != gKapijaSavjet[playerid]) Savjet(playerid, T(playerid, "~y~ALT~w~ - kapija"), 4000);
    gKapijaSavjet[playerid] = naj;
}

KapijeTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i]) KapijaSavjet(i);
}

CMD:kapija(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new idx = 0, p1[12], p2[12], p3[16];
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !SljedeciParam(params, idx, p3) || !ProvjeriBroj(p1) || !ProvjeriBroj(p2) || strval(p2) < KT_KLIZI || strval(p2) > KT_DIZI)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /kapija [id organizacije] [1 klizi, 2 rampa, 3 rampa X, 4 okretna, 5 dize se] [metara ili stupnjeva]"));
    if (gKapijaBroj >= KAPIJA_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj kapija."));
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    new STREAMER_TAG_OBJECT:o = ObjektBlizu(x - floatsin(a, degrees) * 2.0, y + floatcos(a, degrees) * 2.0, z, 6.0);
    if (!o) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ispred vas nema objekta."));
    new k = gKapijaBroj++;
    gKapija[k][kpOrg] = strval(p1);
    gKapija[k][kpModel] = Streamer_GetIntData(STREAMER_TYPE_OBJECT, o, E_STREAMER_MODEL_ID);
    gKapija[k][kpTip] = strval(p2);
    gKapija[k][kpVrijednost] = floatstr(p3);
    KapijaPostavi(k, o);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO gates (org, model, x, y, z, tip, vrijednost) VALUES (%d, %d, %f, %f, %f, %d, %f)", gKapija[k][kpOrg], gKapija[k][kpModel], gKapija[k][kpX], gKapija[k][kpY], gKapija[k][kpZ], gKapija[k][kpTip], gKapija[k][kpVrijednost]));
    gKapija[k][kpDb] = ZadnjiId();
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kapija postavljena: objekt %d, organizacija %d."), gKapija[k][kpModel], gKapija[k][kpOrg]);
}

CMD:kapijabrisi(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    for (new k = 0; k < gKapijaBroj; k++)
    {
        if (!IsPlayerInRangeOfPoint(playerid, KAPIJA_DOMET, gKapija[k][kpX], gKapija[k][kpY], gKapija[k][kpZ])) continue;
        if (gKapija[k][kpOtvorena]) KapijaPomakni(k, false);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM gates WHERE id = %d", gKapija[k][kpDb]));
        gKapija[k] = gKapija[--gKapijaBroj];
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kapija uklonjena."));
    }
    return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema kapije u blizini."));
}
