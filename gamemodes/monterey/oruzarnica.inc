#define ORUZARNICA_MAX          16
#define ORUZARNICA_SVIJET       3500
#define ORUZARNICA_DOMET        2.0
#define ORUZARNICA_KASA_DOMET   2.5
#define ORUZARNICA_SKIN         179
#define ORUZARNICA_TD           40
#define ORUZARNICA_STUPACA      4
#define ORUZARNICA_ROBA         12
#define ORUZARNICA_KATALOG      0

enum E_ORUZARNICA { bool:orIma, orDb, orInt, Float:orX, Float:orY, Float:orZ, Float:orA, bool:orImaAktor, Float:orAx, Float:orAy, Float:orAz, Float:orAa, orAktor,
    STREAMER_TAG_PICKUP:orPickup, STREAMER_TAG_PICKUP:orPickupU, STREAMER_TAG_3D_TEXT_LABEL:orTekst, STREAMER_TAG_3D_TEXT_LABEL:orTekstU, STREAMER_TAG_3D_TEXT_LABEL:orTekstA }

enum E_ORUZ_INT { oiIme[16], oiInt, Float:oiX, Float:oiY, Float:oiZ }
static const gOruzarnicaInt[][E_ORUZ_INT] = {
    { "Ammu-Nation 1", 1, 286.148, -40.644, 1001.515 },
    { "Ammu-Nation 2", 4, 286.800, -82.547, 1001.515 },
    { "Ammu-Nation 3", 6, 296.919, -108.071, 1001.515 },
    { "Ammu-Nation 4", 7, 314.820, -141.431, 999.601 },
    { "Ammu-Nation 5", 6, 316.524, -167.706, 999.593 }
};

static const gOruzarnicaZadano[][3] = {
    { ITEM_COLT, 1, 800 }, { ITEM_DEAGLE, 1, 1500 }, { ITEM_SACMARICA, 1, 2000 }, { ITEM_UZI, 1, 2500 },
    { ITEM_MP5, 1, 3000 }, { ITEM_AK47, 1, 4500 }, { ITEM_M4, 1, 5000 }, { ITEM_NOZ, 1, 150 },
    { ITEM_MUN_PISTOLJ, 50, 100 }, { ITEM_MUN_SACMARICA, 20, 120 }, { ITEM_MUN_SMG, 50, 150 }, { ITEM_MUN_PUSKA, 50, 200 }
};

#assert ORUZARNICA_ROBA <= ORUZARNICA_STUPACA * 3 && ORUZARNICA_TD >= 3 + 2 * ORUZARNICA_ROBA && sizeof gOruzarnicaZadano <= ORUZARNICA_ROBA

static gOruz[ORUZARNICA_MAX][E_ORUZARNICA];
static gApOruzarnica[MAX_PLAYERS];
static gOruzRoba[ORUZARNICA_MAX][ORUZARNICA_ROBA][3], gOruzRobaDb[ORUZARNICA_MAX][ORUZARNICA_ROBA], gOruzRobaBroj[ORUZARNICA_MAX];
static PlayerText:gOruzarnicaTd[MAX_PLAYERS][ORUZARNICA_TD], gOruzarnicaBroj[MAX_PLAYERS], bool:gOruzarnicaKatalog[MAX_PLAYERS];
static PlayerText:gOruzarnicaPlocica[MAX_PLAYERS][ORUZARNICA_ROBA], PlayerText:gOruzarnicaIzlaz[MAX_PLAYERS];

InitOruzarnica()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS gun_shops (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL DEFAULT 0, Aktor INTEGER NOT NULL DEFAULT 0, ax REAL NOT NULL DEFAULT 0, ay REAL NOT NULL DEFAULT 0, az REAL NOT NULL DEFAULT 0, aa REAL NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS store_items (id INTEGER PRIMARY KEY AUTOINCREMENT, store_type INTEGER NOT NULL, store INTEGER NOT NULL, kind INTEGER NOT NULL DEFAULT 0, model INTEGER NOT NULL, amount INTEGER NOT NULL DEFAULT 1, price INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, Aktor, ax, ay, az, aa FROM gun_shops ORDER BY id LIMIT %d", ORUZARNICA_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++)
    {
        gOruz[s][orDb] = DB_GetFieldInt(res, 0);
        gOruz[s][orX] = DB_GetFieldFloat(res, 1);
        gOruz[s][orY] = DB_GetFieldFloat(res, 2);
        gOruz[s][orZ] = DB_GetFieldFloat(res, 3);
        gOruz[s][orA] = DB_GetFieldFloat(res, 4);
        gOruz[s][orInt] = clamp(DB_GetFieldInt(res, 5), 0, sizeof gOruzarnicaInt - 1);
        OruzarnicaStvori(s);
        OruzarnicaUcitajRobu(s);
        if (DB_GetFieldInt(res, 6))
        {
            gOruz[s][orAx] = DB_GetFieldFloat(res, 7);
            gOruz[s][orAy] = DB_GetFieldFloat(res, 8);
            gOruz[s][orAz] = DB_GetFieldFloat(res, 9);
            gOruz[s][orAa] = DB_GetFieldFloat(res, 10);
            OruzarnicaAktor(s);
        }
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static OruzarnicaUcitajRobu(s, bool:zadano = true)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, model, amount, price FROM store_items WHERE store_type = %d AND store = %d ORDER BY id LIMIT %d", ORUZARNICA_KATALOG, gOruz[s][orDb], ORUZARNICA_ROBA);
    gOruzRobaBroj[s] = 0;
    for (new n = DB_GetRowCount(res); gOruzRobaBroj[s] < n; DB_SelectNextRow(res))
    {
        new r = gOruzRobaBroj[s]++;
        gOruzRobaDb[s][r] = DB_GetFieldInt(res, 0);
        gOruzRoba[s][r][0] = DB_GetFieldInt(res, 1);
        gOruzRoba[s][r][1] = DB_GetFieldInt(res, 2);
        gOruzRoba[s][r][2] = DB_GetFieldInt(res, 3);
    }
    DB_FreeResultSet(res);
    if (gOruzRobaBroj[s] || !zadano) return;
    for (new i = 0; i < sizeof gOruzarnicaZadano; i++)
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO store_items (store_type, store, model, amount, price) VALUES (%d, %d, %d, %d, %d)", ORUZARNICA_KATALOG, gOruz[s][orDb], gOruzarnicaZadano[i][0], gOruzarnicaZadano[i][1], gOruzarnicaZadano[i][2]));
    OruzarnicaUcitajRobu(s, false);
}

static OruzarnicaStvori(s)
{
    new svijet = ORUZARNICA_SVIJET + s, i = gOruz[s][orInt];
    gOruz[s][orIma] = true;
    gOruz[s][orPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gOruz[s][orX], gOruz[s][orY], gOruz[s][orZ], 0, 0);
    gOruz[s][orTekst] = CreateDynamic3DTextLabel("Oruzarnica\n[Y]", COLOR_ORG, gOruz[s][orX], gOruz[s][orY], gOruz[s][orZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gOruz[s][orPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gOruzarnicaInt[i][oiX], gOruzarnicaInt[i][oiY], gOruzarnicaInt[i][oiZ], svijet, gOruzarnicaInt[i][oiInt]);
    gOruz[s][orTekstU] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gOruzarnicaInt[i][oiX], gOruzarnicaInt[i][oiY], gOruzarnicaInt[i][oiZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gOruzarnicaInt[i][oiInt]);
}

static OruzarnicaAktor(s)
{
    new svijet = ORUZARNICA_SVIJET + s;
    gOruz[s][orImaAktor] = true;
    gOruz[s][orAktor] = CreateActor(ORUZARNICA_SKIN, gOruz[s][orAx], gOruz[s][orAy], gOruz[s][orAz], gOruz[s][orAa]);
    SetActorVirtualWorld(gOruz[s][orAktor], svijet);
    gOruz[s][orTekstA] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gOruz[s][orAx], gOruz[s][orAy], gOruz[s][orAz] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gOruzarnicaInt[gOruz[s][orInt]][oiInt]);
}

static OruzarnicaMakniAktor(s)
{
    if (!gOruz[s][orImaAktor]) return;
    DestroyActor(gOruz[s][orAktor]);
    DestroyDynamic3DTextLabel(gOruz[s][orTekstA]);
    gOruz[s][orImaAktor] = false;
}

static OruzarnicaIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - ORUZARNICA_SVIJET;
    return s >= 0 && s < ORUZARNICA_MAX && gOruz[s][orIma] && GetPlayerInterior(playerid) == gOruzarnicaInt[gOruz[s][orInt]][oiInt] ? s : -1;
}

static OruzarnicaVani(playerid, s)
{
    OruzarnicaZatvori(playerid);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gOruz[s][orX], gOruz[s][orY], gOruz[s][orZ]);
    SetPlayerFacingAngle(playerid, gOruz[s][orA] + 180.0);
    SetCameraBehindPlayer(playerid);
}

OruzarnicaUlaz(playerid)
{
    new s = OruzarnicaIgraca(playerid);
    if (s != -1)
    {
        new i = gOruz[s][orInt];
        if (!IsPlayerInRangeOfPoint(playerid, ORUZARNICA_DOMET, gOruzarnicaInt[i][oiX], gOruzarnicaInt[i][oiY], gOruzarnicaInt[i][oiZ])) return 0;
        OruzarnicaVani(playerid, s);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < ORUZARNICA_MAX; s++)
    {
        if (!gOruz[s][orIma] || !IsPlayerInRangeOfPoint(playerid, ORUZARNICA_DOMET, gOruz[s][orX], gOruz[s][orY], gOruz[s][orZ])) continue;
        new i = gOruz[s][orInt];
        SetPlayerInterior(playerid, gOruzarnicaInt[i][oiInt]);
        SetPlayerVirtualWorld(playerid, ORUZARNICA_SVIJET + s);
        SetPlayerPos(playerid, gOruzarnicaInt[i][oiX], gOruzarnicaInt[i][oiY], gOruzarnicaInt[i][oiZ]);
        SetCameraBehindPlayer(playerid);
        if (gOruz[s][orImaAktor]) AktorKaze(playerid, gOruz[s][orAktor], "Prodavac", "Dobrodosli. Oruzje i municiju prodajem na pultu.");
        return 1;
    }
    return 0;
}

static PlayerText:OruzarnicaTekst(playerid, Float:x, Float:y, const tekst[], Float:sirina, Float:visina, boja = 0xFFFFFFFF)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetOutline(playerid, td, 1);
    gOruzarnicaTd[playerid][gOruzarnicaBroj[playerid]++] = td;
    return td;
}

static OruzarnicaOtvori(playerid, s)
{
    gOruzarnicaKatalog[playerid] = true;
    gOruzarnicaBroj[playerid] = 0;
    new PlayerText:td = CreatePlayerTextDraw(playerid, 160.0, 102.0, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, 320.0, 285.0);
    PlayerTextDrawColour(playerid, td, 0x101418E6);
    gOruzarnicaTd[playerid][gOruzarnicaBroj[playerid]++] = td;
    OruzarnicaTekst(playerid, 320.0, 108.0, T(playerid, "ORUZARNICA"), 0.45, 1.8, 0xD4A017FF);
    for (new r = 0; r < gOruzRobaBroj[s]; r++)
    {
        new item = gOruzRoba[s][r][0], Float:x = 172.0 + float(r % ORUZARNICA_STUPACA) * 76.0, Float:y = 134.0 + float(r / ORUZARNICA_STUPACA) * 76.0, natpis[64];
        td = CreatePlayerTextDraw(playerid, x, y, "_");
        PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_MODEL_PREVIEW);
        PlayerTextDrawTextSize(playerid, td, 70.0, 56.0);
        PlayerTextDrawBackgroundColour(playerid, td, 0x2A3038FF);
        PlayerTextDrawSetPreviewModel(playerid, td, gItems[item][ITEM_MODEL]);
        PlayerTextDrawSetPreviewRot(playerid, td, -10.0, 0.0, -30.0, 1.0);
        PlayerTextDrawSetSelectable(playerid, td, true);
        gOruzarnicaTd[playerid][gOruzarnicaBroj[playerid]++] = td;
        gOruzarnicaPlocica[playerid][r] = td;
        if (gOruzRoba[s][r][1] > 1) format(natpis, sizeof natpis, "%s x%d~n~~g~$%d", T(playerid, gItems[item][ITEM_NAME]), gOruzRoba[s][r][1], gOruzRoba[s][r][2]);
        else format(natpis, sizeof natpis, "%s~n~~g~$%d", T(playerid, gItems[item][ITEM_NAME]), gOruzRoba[s][r][2]);
        OruzarnicaTekst(playerid, x + 35.0, y + 57.0, natpis, 0.16, 0.8);
    }
    gOruzarnicaIzlaz[playerid] = OruzarnicaTekst(playerid, 320.0, 364.0, T(playerid, "IZLAZ"), 0.3, 1.3);
    PlayerTextDrawUseBox(playerid, gOruzarnicaIzlaz[playerid], true);
    PlayerTextDrawBoxColour(playerid, gOruzarnicaIzlaz[playerid], 0x8B1A1ACC);
    PlayerTextDrawTextSize(playerid, gOruzarnicaIzlaz[playerid], 14.0, 70.0);
    PlayerTextDrawSetSelectable(playerid, gOruzarnicaIzlaz[playerid], true);
    for (new i = 0; i < gOruzarnicaBroj[playerid]; i++) PlayerTextDrawShow(playerid, gOruzarnicaTd[playerid][i]);
    SelectTextDraw(playerid, 0xD4A017FF);
}

OruzarnicaZatvori(playerid)
{
    if (!gOruzarnicaKatalog[playerid]) return;
    gOruzarnicaKatalog[playerid] = false;
    for (new i = 0; i < gOruzarnicaBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gOruzarnicaTd[playerid][i]);
    gOruzarnicaBroj[playerid] = 0;
    CancelSelectTextDraw(playerid);
}

OruzarnicaKlik(playerid, PlayerText:td)
{
    if (!gOruzarnicaKatalog[playerid]) return 0;
    new s = OruzarnicaIgraca(playerid);
    if (td == gOruzarnicaIzlaz[playerid] || s == -1)
    {
        OruzarnicaZatvori(playerid);
        return 1;
    }
    for (new r = 0; r < gOruzRobaBroj[s]; r++)
    {
        if (td != gOruzarnicaPlocica[playerid][r]) continue;
        new item = gOruzRoba[s][r][0], kolicina = gOruzRoba[s][r][1], cijena = gOruzRoba[s][r][2];
        if (gNovac[playerid] < cijena) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        else if (!GiveItem(playerid, item, kolicina)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        else
        {
            DajNovac(playerid, -cijena);
            PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
            SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d)."), T(playerid, gItems[item][ITEM_NAME]), cijena);
        }
        return 1;
    }
    return 1;
}

OruzarnicaKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid) || gOruzarnicaKatalog[playerid]) return 0;
    new s = OruzarnicaIgraca(playerid);
    if (s == -1 || !gOruz[s][orImaAktor] || !IsPlayerInRangeOfPoint(playerid, ORUZARNICA_KASA_DOMET, gOruz[s][orAx], gOruz[s][orAy], gOruz[s][orAz])) return 0;
    AktorKaze(playerid, gOruz[s][orAktor], "Prodavac", "Izvolite, pogledajte ponudu.");
    OruzarnicaOtvori(playerid, s);
    return 1;
}

OruzarnicaDisconnect(playerid)
{
    gOruzarnicaKatalog[playerid] = false;
    gOruzarnicaBroj[playerid] = 0;
}

ShowOruzarnice(playerid)
{
    new list[1024], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova oruzarnica na mojoj poziciji"));
    for (new s = 0; s < ORUZARNICA_MAX; s++)
    {
        if (!gOruz[s][orIma]) continue;
        format(row, sizeof row, "[%d] %s (%s)%s", gOruz[s][orDb], CestaZona(gOruz[s][orX], gOruz[s][orY]), gOruzarnicaInt[gOruz[s][orInt]][oiIme], gOruz[s][orImaAktor] ? ("") : T(playerid, " - bez blagajnika"));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_ORUZARNICA, DIALOG_STYLE_LIST, T(playerid, "Oruzarnice"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static OruzarnicaNova(playerid, i)
{
    new s = 0;
    while (s < ORUZARNICA_MAX && gOruz[s][orIma]) s++;
    if (s == ORUZARNICA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Oruzarnica se postavlja samo izvana."));
    GetPlayerPos(playerid, gOruz[s][orX], gOruz[s][orY], gOruz[s][orZ]);
    GetPlayerFacingAngle(playerid, gOruz[s][orA]);
    gOruz[s][orInt] = i;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO gun_shops (x, y, z, a, interior) VALUES (%f, %f, %f, %f, %d)", gOruz[s][orX], gOruz[s][orY], gOruz[s][orZ], gOruz[s][orA], i));
    gOruz[s][orDb] = ZadnjiId();
    OruzarnicaStvori(s);
    OruzarnicaUcitajRobu(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Oruzarnica dodana: %s. Udite i postavite blagajnika."), CestaZona(gOruz[s][orX], gOruz[s][orY]));
}

static OruzarnicaBlagajnik(playerid, s)
{
    if (OruzarnicaIgraca(playerid) != s) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te oruzarnice."));
    OruzarnicaMakniAktor(s);
    GetPlayerPos(playerid, gOruz[s][orAx], gOruz[s][orAy], gOruz[s][orAz]);
    GetPlayerFacingAngle(playerid, gOruz[s][orAa]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE gun_shops SET Aktor = 1, ax = %f, ay = %f, az = %f, aa = %f WHERE id = %d", gOruz[s][orAx], gOruz[s][orAy], gOruz[s][orAz], gOruz[s][orAa], gOruz[s][orDb]));
    OruzarnicaAktor(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Blagajnik je postavljen."));
}

static OruzarnicaObrisi(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && OruzarnicaIgraca(i) == s) OruzarnicaVani(i, s);
    OruzarnicaMakniAktor(s);
    DestroyDynamicPickup(gOruz[s][orPickup]);
    DestroyDynamicPickup(gOruz[s][orPickupU]);
    DestroyDynamic3DTextLabel(gOruz[s][orTekst]);
    DestroyDynamic3DTextLabel(gOruz[s][orTekstU]);
    gOruz[s][orIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM gun_shops WHERE id = %d", gOruz[s][orDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM store_items WHERE store_type = %d AND store = %d", ORUZARNICA_KATALOG, gOruz[s][orDb]));
}

DLG:apanel_oruzarnica(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new list[256], len = 0;
        for (new i = 0; i < sizeof gOruzarnicaInt; i++) DodajStupac(list, len, gOruzarnicaInt[i][oiIme]);
        return PrikaziDialog(playerid, DLG_APANEL_ORUZARNICA_INT, DIALOG_STYLE_LIST, T(playerid, "Odaberite interijer"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    new n = listitem;
    for (new s = 0; s < ORUZARNICA_MAX; s++)
    {
        if (!gOruz[s][orIma] || --n) continue;
        gApOruzarnica[playerid] = s;
        return PrikaziDialog(playerid, DLG_APANEL_ORUZARNICA_MENI, DIALOG_STYLE_LIST, T(playerid, "Oruzarnice"), T(playerid, "Postavi blagajnika na mojoj poziciji\nKatalog\nObrisi oruzarnicu"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    return ShowOruzarnice(playerid);
}

DLG:apanel_oruzarnica_int(playerid, response, listitem)
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (response && listitem >= 0 && listitem < sizeof gOruzarnicaInt) OruzarnicaNova(playerid, listitem);
    return ShowOruzarnice(playerid);
}

DLG:apanel_oruzarnica_meni(playerid, response, listitem)
{
    new s = gApOruzarnica[playerid];
    if (!response || !gOruz[s][orIma]) return ShowOruzarnice(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        OruzarnicaBlagajnik(playerid, s);
        return ShowOruzarnice(playerid);
    }
    if (listitem == 1) return ShowOruzarnicaKatalog(playerid);
    OruzarnicaObrisi(s);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Oruzarnica obrisana."));
    return ShowOruzarnice(playerid);
}

static ShowOruzarnicaKatalog(playerid)
{
    new s = gApOruzarnica[playerid], list[1024], row[80], len = 0;
    if (!gOruz[s][orIma]) return ShowOruzarnice(playerid);
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Dodaj artikl"));
    for (new r = 0; r < gOruzRobaBroj[s]; r++)
    {
        format(row, sizeof row, "%s x%d - $%d", T(playerid, gItems[gOruzRoba[s][r][0]][ITEM_NAME]), gOruzRoba[s][r][1], gOruzRoba[s][r][2]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_ORUZARNICA_KATALOG, DIALOG_STYLE_LIST, T(playerid, "Katalog"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_oruzarnica_katalog(playerid, response, listitem)
{
    new s = gApOruzarnica[playerid];
    if (!response || !gOruz[s][orIma]) return ShowOruzarnice(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new body[1024], row[64], len = 0;
        for (new i = 0; i < BROJ_ITEMA; i++)
        {
            format(row, sizeof row, T(playerid, "  %d - %s (najvise %d %s)"), i, T(playerid, gItems[i][ITEM_NAME]), gItems[i][ITEM_MAX], T(playerid, gItems[i][ITEM_UNIT]));
            DodajStupac(body, len, row);
        }
        strcat(body, T(playerid, "Unesite: [predmet] [kolicina] [cijena]"));
        return PrikaziDialog(playerid, DLG_APANEL_ORUZARNICA_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Katalog"), body, T(playerid, "Dodaj"), T(playerid, "Natrag"));
    }
    new r = listitem - 1;
    if (r >= 0 && r < gOruzRobaBroj[s])
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM store_items WHERE id = %d", gOruzRobaDb[s][r]));
        OruzarnicaUcitajRobu(s, false);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Obrisano."));
    }
    return ShowOruzarnicaKatalog(playerid);
}

DLG:apanel_oruzarnica_unos(playerid, response, const inputtext[])
{
    new s = gApOruzarnica[playerid];
    if (!response || !gOruz[s][orIma]) return ShowOruzarnicaKatalog(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new idx = 0, p1[12], p2[12], p3[12];
    if (!SljedeciParam(inputtext, idx, p1) || !SljedeciParam(inputtext, idx, p2) || !SljedeciParam(inputtext, idx, p3) || !ProvjeriBroj(p1) || !ProvjeriBroj(p2) || !ProvjeriBroj(p3))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [predmet] [kolicina] [cijena]"));
        return ShowOruzarnicaKatalog(playerid);
    }
    new item = strval(p1), kolicina = strval(p2), cijena = strval(p3);
    if (item >= BROJ_ITEMA || kolicina < 1 || kolicina > gItems[item][ITEM_MAX] || cijena > 1000000 || gOruzRobaBroj[s] >= ORUZARNICA_ROBA)
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravan artikl."));
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO store_items (store_type, store, model, amount, price) VALUES (%d, %d, %d, %d, %d)", ORUZARNICA_KATALOG, gOruz[s][orDb], item, kolicina, cijena));
        OruzarnicaUcitajRobu(s, false);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Artikl dodan."));
    }
    return ShowOruzarnicaKatalog(playerid);
}
