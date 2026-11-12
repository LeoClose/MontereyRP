#define TRGOVINA_MAX            32
#define TRGOVINA_SVIJET         3000
#define TRGOVINA_INT            17
#define TRGOVINA_DOMET          2.0
#define TRGOVINA_KASA_DOMET     2.5
#define TRGOVINA_SKIN           217
#define TRGOVINA_PLJACKA_S      3600
#define TRGOVINA_PREDAJA_MS     4000
#define TRGOVINA_RUKE_MS        60000
#define TRGOVINA_PLIJEN_MIN     300
#define TRGOVINA_PLIJEN_RASPON  700

enum E_TRGOVINA { bool:tgIma, tgDb, Float:tgX, Float:tgY, Float:tgZ, Float:tgA, bool:tgImaAktor, Float:tgAx, Float:tgAy, Float:tgAz, Float:tgAa, tgAktor, tgPljackano,
    STREAMER_TAG_PICKUP:tgPickup, STREAMER_TAG_PICKUP:tgPickupU, STREAMER_TAG_3D_TEXT_LABEL:tgTekst, STREAMER_TAG_3D_TEXT_LABEL:tgTekstU, STREAMER_TAG_3D_TEXT_LABEL:tgTekstA }

static gTrg[TRGOVINA_MAX][E_TRGOVINA];
static gApTrgovina[MAX_PLAYERS];
static const Float:gTrgovinaUlaz[3] = { -25.884, -185.869, 1003.547 };
static const gTrgovinaRoba[][2] = { { ITEM_CRIJEVO, 150 }, { ITEM_KANISTER, 50 }, { ITEM_MOBITEL, 200 } };

InitTrgovina()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS shops (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, Aktor INTEGER NOT NULL DEFAULT 0, ax REAL NOT NULL DEFAULT 0, ay REAL NOT NULL DEFAULT 0, az REAL NOT NULL DEFAULT 0, aa REAL NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, Aktor, ax, ay, az, aa FROM shops ORDER BY id LIMIT %d", TRGOVINA_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++)
    {
        gTrg[s][tgDb] = DB_GetFieldInt(res, 0);
        gTrg[s][tgX] = DB_GetFieldFloat(res, 1);
        gTrg[s][tgY] = DB_GetFieldFloat(res, 2);
        gTrg[s][tgZ] = DB_GetFieldFloat(res, 3);
        gTrg[s][tgA] = DB_GetFieldFloat(res, 4);
        TrgovinaStvori(s);
        if (DB_GetFieldInt(res, 5))
        {
            gTrg[s][tgAx] = DB_GetFieldFloat(res, 6);
            gTrg[s][tgAy] = DB_GetFieldFloat(res, 7);
            gTrg[s][tgAz] = DB_GetFieldFloat(res, 8);
            gTrg[s][tgAa] = DB_GetFieldFloat(res, 9);
            TrgovinaAktor(s);
        }
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static TrgovinaStvori(s)
{
    new svijet = TRGOVINA_SVIJET + s;
    gTrg[s][tgIma] = true;
    gTrg[s][tgPljackano] = 0;
    gTrg[s][tgPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ], 0, 0);
    gTrg[s][tgTekst] = CreateDynamic3DTextLabel("24/7\n[Y]", COLOR_ORG, gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gTrg[s][tgPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gTrgovinaUlaz[0], gTrgovinaUlaz[1], gTrgovinaUlaz[2], svijet, TRGOVINA_INT);
    gTrg[s][tgTekstU] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gTrgovinaUlaz[0], gTrgovinaUlaz[1], gTrgovinaUlaz[2] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, TRGOVINA_INT);
}

static TrgovinaAktor(s)
{
    new svijet = TRGOVINA_SVIJET + s;
    gTrg[s][tgImaAktor] = true;
    gTrg[s][tgAktor] = CreateActor(TRGOVINA_SKIN, gTrg[s][tgAx], gTrg[s][tgAy], gTrg[s][tgAz], gTrg[s][tgAa]);
    SetActorVirtualWorld(gTrg[s][tgAktor], svijet);
    gTrg[s][tgTekstA] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gTrg[s][tgAx], gTrg[s][tgAy], gTrg[s][tgAz] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, TRGOVINA_INT);
}

static TrgovinaMakniAktor(s)
{
    if (!gTrg[s][tgImaAktor]) return;
    DestroyActor(gTrg[s][tgAktor]);
    DestroyDynamic3DTextLabel(gTrg[s][tgTekstA]);
    gTrg[s][tgImaAktor] = false;
}

static TrgovinaIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - TRGOVINA_SVIJET;
    return s >= 0 && s < TRGOVINA_MAX && gTrg[s][tgIma] && GetPlayerInterior(playerid) == TRGOVINA_INT ? s : -1;
}

static TrgovinaVani(playerid, s)
{
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ]);
    SetPlayerFacingAngle(playerid, gTrg[s][tgA] + 180.0);
    SetCameraBehindPlayer(playerid);
}

TrgovinaUlaz(playerid)
{
    new s = TrgovinaIgraca(playerid);
    if (s != -1)
    {
        if (!IsPlayerInRangeOfPoint(playerid, TRGOVINA_DOMET, gTrgovinaUlaz[0], gTrgovinaUlaz[1], gTrgovinaUlaz[2])) return 0;
        TrgovinaVani(playerid, s);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < TRGOVINA_MAX; s++)
    {
        if (!gTrg[s][tgIma] || !IsPlayerInRangeOfPoint(playerid, TRGOVINA_DOMET, gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ])) continue;
        SetPlayerInterior(playerid, TRGOVINA_INT);
        SetPlayerVirtualWorld(playerid, TRGOVINA_SVIJET + s);
        SetPlayerPos(playerid, gTrgovinaUlaz[0], gTrgovinaUlaz[1], gTrgovinaUlaz[2]);
        SetCameraBehindPlayer(playerid);
        if (gTrg[s][tgImaAktor]) AktorKaze(playerid, gTrg[s][tgAktor], "Prodavac", "Dobar dan! Ako trebate nesto, tu sam na kasi.");
        return 1;
    }
    return 0;
}

TrgovinaKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new s = TrgovinaIgraca(playerid);
    if (s == -1 || !gTrg[s][tgImaAktor] || !IsPlayerInRangeOfPoint(playerid, TRGOVINA_KASA_DOMET, gTrg[s][tgAx], gTrg[s][tgAy], gTrg[s][tgAz])) return 0;
    new list[512], row[64];
    format(list, sizeof list, "%s\t%s\n", T(playerid, "Predmet"), T(playerid, "Cijena"));
    new len = strlen(list);
    for (new i = 0; i < sizeof gTrgovinaRoba; i++)
    {
        format(row, sizeof row, "%s\t$%d", T(playerid, gItems[gTrgovinaRoba[i][0]][ITEM_NAME]), gTrgovinaRoba[i][1]);
        DodajStupac(list, len, row);
    }
    AktorKaze(playerid, gTrg[s][tgAktor], "Prodavac", "Izvolite, sto ce biti?");
    PrikaziDialog(playerid, DLG_TRGOVINA, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Trgovina"), list, T(playerid, "Kupi"), T(playerid, "Zatvori"));
    return 1;
}

DLG:trgovina(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gTrgovinaRoba || TrgovinaIgraca(playerid) == -1) return 1;
    new item = gTrgovinaRoba[listitem][0], cijena = gTrgovinaRoba[listitem][1];
    if (gNovac[playerid] < cijena) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    if (!GiveItem(playerid, item, 1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
    DajNovac(playerid, -cijena);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d)."), T(playerid, gItems[item][ITEM_NAME]), cijena);
}

TrgovinaUpdate(playerid)
{
    new s = TrgovinaIgraca(playerid);
    if (s == -1 || !gTrg[s][tgImaAktor] || GetPlayerTargetActor(playerid) != gTrg[s][tgAktor]) return;
    new WEAPON:w = GetPlayerWeapon(playerid);
    if (w < WEAPON_COLT45 || w > WEAPON_MINIGUN || gettime() - gTrg[s][tgPljackano] < TRGOVINA_PLJACKA_S) return;
    gTrg[s][tgPljackano] = gettime();
    ApplyActorAnimation(gTrg[s][tgAktor], "SHOP", "SHP_Rob_GiveCash", 4.1, false, false, false, true, 0);
    SetTimerEx("TrgovinaPlijen", TRGOVINA_PREDAJA_MS, false, "iii", playerid, gAccountId[playerid], s);
}

forward TrgovinaPlijen(playerid, racun, s);
public TrgovinaPlijen(playerid, racun, s)
{
    if (!gTrg[s][tgImaAktor]) return;
    ApplyActorAnimation(gTrg[s][tgAktor], "SHOP", "SHP_Rob_HandsUp", 4.1, false, false, false, true, 0);
    SetTimerEx("TrgovinaRuke", TRGOVINA_RUKE_MS, false, "ii", s, gTrg[s][tgAktor]);
    if (gAccountId[playerid] != racun || TrgovinaIgraca(playerid) != s) return;
    new plijen = TRGOVINA_PLIJEN_MIN + random(TRGOVINA_PLIJEN_RASPON);
    DajNovac(playerid, plijen);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s uzima novac od blagajnika.", GetRPIme(playerid), "", 1);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Opljackali ste trgovinu: $%d."), plijen);
}

forward TrgovinaRuke(s, Aktor);
public TrgovinaRuke(s, Aktor)
{
    if (gTrg[s][tgImaAktor] && gTrg[s][tgAktor] == Aktor) ClearActorAnimations(Aktor);
}

ShowTrgovine(playerid)
{
    new list[1024], row[80], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova trgovina na mojoj poziciji"));
    for (new s = 0; s < TRGOVINA_MAX; s++)
    {
        if (!gTrg[s][tgIma]) continue;
        format(row, sizeof row, "[%d] %s%s", gTrg[s][tgDb], CestaZona(gTrg[s][tgX], gTrg[s][tgY]), T(playerid, gTrg[s][tgImaAktor] ? ("") : (" - bez blagajnika")));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_TRGOVINA, DIALOG_STYLE_LIST, T(playerid, "Trgovine"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static TrgovinaNova(playerid)
{
    new s = 0;
    while (s < TRGOVINA_MAX && gTrg[s][tgIma]) s++;
    if (s == TRGOVINA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trgovina se postavlja samo izvana."));
    GetPlayerPos(playerid, gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ]);
    GetPlayerFacingAngle(playerid, gTrg[s][tgA]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO shops (x, y, z, a) VALUES (%f, %f, %f, %f)", gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ], gTrg[s][tgA]));
    gTrg[s][tgDb] = ZadnjiId();
    TrgovinaStvori(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Trgovina dodana: %s. Udite i postavite blagajnika."), CestaZona(gTrg[s][tgX], gTrg[s][tgY]));
}

static TrgovinaBlagajnik(playerid, s)
{
    if (TrgovinaIgraca(playerid) != s) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te trgovine."));
    TrgovinaMakniAktor(s);
    GetPlayerPos(playerid, gTrg[s][tgAx], gTrg[s][tgAy], gTrg[s][tgAz]);
    GetPlayerFacingAngle(playerid, gTrg[s][tgAa]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE shops SET Aktor = 1, ax = %f, ay = %f, az = %f, aa = %f WHERE id = %d", gTrg[s][tgAx], gTrg[s][tgAy], gTrg[s][tgAz], gTrg[s][tgAa], gTrg[s][tgDb]));
    TrgovinaAktor(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Blagajnik je postavljen."));
}

static TrgovinaObrisi(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && TrgovinaIgraca(i) == s) TrgovinaVani(i, s);
    TrgovinaMakniAktor(s);
    DestroyDynamicPickup(gTrg[s][tgPickup]);
    DestroyDynamicPickup(gTrg[s][tgPickupU]);
    DestroyDynamic3DTextLabel(gTrg[s][tgTekst]);
    DestroyDynamic3DTextLabel(gTrg[s][tgTekstU]);
    gTrg[s][tgIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM shops WHERE id = %d", gTrg[s][tgDb]));
}

DLG:apanel_trgovina(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        TrgovinaNova(playerid);
        return ShowTrgovine(playerid);
    }
    new n = listitem;
    for (new s = 0; s < TRGOVINA_MAX; s++)
    {
        if (!gTrg[s][tgIma] || --n) continue;
        gApTrgovina[playerid] = s;
        return PrikaziDialog(playerid, DLG_APANEL_TRGOVINA_MENI, DIALOG_STYLE_LIST, T(playerid, "Trgovine"), T(playerid, "Postavi blagajnika na mojoj poziciji\nObrisi trgovinu"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    return ShowTrgovine(playerid);
}

DLG:apanel_trgovina_meni(playerid, response, listitem)
{
    new s = gApTrgovina[playerid];
    if (!response || !gTrg[s][tgIma]) return ShowTrgovine(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        TrgovinaBlagajnik(playerid, s);
        return ShowTrgovine(playerid);
    }
    TrgovinaObrisi(s);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Trgovina obrisana."));
    return ShowTrgovine(playerid);
}
