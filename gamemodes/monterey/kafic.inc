#define KAFIC_MAX           16
#define KAFIC_PC_MAX        64
#define KAFIC_SKIN          217
#define KAFIC_DOMET         2.5
#define KAFIC_PC_DOMET      2.0
#define KAFIC_VEZA          60.0
#define KAFIC_MAPA_INT      15
#define KAFIC_MAPA_X        1405.19
#define KAFIC_MAPA_Y        1.25
#define KAFIC_MAPA_Z        997.6

enum E_KAFIC { bool:kfIma, kfDb, Float:kfX, Float:kfY, Float:kfZ, Float:kfA, kfInt, kfSvijet, kfAktor, STREAMER_TAG_3D_TEXT_LABEL:kfTekst }
enum E_KAFIC_PC { bool:kpIma, kpDb, kpKafic, Float:kpX, Float:kpY, Float:kpZ, Float:kpA, kpInt, kpSvijet, STREAMER_TAG_OBJECT:kpObjekt }
static const gKaficMinuta[] = { 10, 20, 30, 60 };
static const gKaficCijena[] = { 20, 35, 50, 90 };
static gKaficUredi[MAX_PLAYERS] = { -1, ... }, gKafic[KAFIC_MAX][E_KAFIC], gKaficPc[KAFIC_PC_MAX][E_KAFIC_PC], gKaficIgrac[MAX_PLAYERS], gKaficDo[MAX_PLAYERS], gApKafic[MAX_PLAYERS][KAFIC_MAX + KAFIC_PC_MAX];

InitKafic()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS cafe_cashiers (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS cafe_pcs (id INTEGER PRIMARY KEY AUTOINCREMENT, cashier INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM cafe_cashiers LIMIT %d", KAFIC_MAX);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++, DB_SelectNextRow(res))
    {
        gKafic[k][kfDb] = DB_GetFieldInt(res, 0);
        gKafic[k][kfX] = DB_GetFieldFloat(res, 1);
        gKafic[k][kfY] = DB_GetFieldFloat(res, 2);
        gKafic[k][kfZ] = DB_GetFieldFloat(res, 3);
        gKafic[k][kfA] = DB_GetFieldFloat(res, 4);
        gKafic[k][kfInt] = DB_GetFieldInt(res, 5);
        gKafic[k][kfSvijet] = DB_GetFieldInt(res, 6);
        KaficStvori(k);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, cashier, x, y, z, a, interior, world FROM cafe_pcs LIMIT %d", KAFIC_PC_MAX);
    for (new p = 0, n = DB_GetRowCount(res); p < n; p++, DB_SelectNextRow(res))
    {
        gKaficPc[p][kpDb] = DB_GetFieldInt(res, 0);
        gKaficPc[p][kpKafic] = DB_GetFieldInt(res, 1);
        gKaficPc[p][kpX] = DB_GetFieldFloat(res, 2);
        gKaficPc[p][kpY] = DB_GetFieldFloat(res, 3);
        gKaficPc[p][kpZ] = DB_GetFieldFloat(res, 4);
        gKaficPc[p][kpA] = DB_GetFieldFloat(res, 5);
        gKaficPc[p][kpInt] = DB_GetFieldInt(res, 6);
        gKaficPc[p][kpSvijet] = DB_GetFieldInt(res, 7);
        KaficPcStvori(p);
    }
    DB_FreeResultSet(res);
}

static KaficStvori(k)
{
    gKafic[k][kfIma] = true;
    gKafic[k][kfAktor] = CreateActor(KAFIC_SKIN, gKafic[k][kfX], gKafic[k][kfY], gKafic[k][kfZ], gKafic[k][kfA]);
    SetActorVirtualWorld(gKafic[k][kfAktor], gKafic[k][kfSvijet]);
    SetActorInvulnerable(gKafic[k][kfAktor], true);
    gKafic[k][kfTekst] = CreateDynamic3DTextLabel("Internet kafic\n[Y]", COLOR_ORG, gKafic[k][kfX], gKafic[k][kfY], gKafic[k][kfZ] + 1.1, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gKafic[k][kfSvijet], gKafic[k][kfInt]);
}

static KaficPcStvori(p)
{
    gKaficPc[p][kpIma] = true;
    gKaficPc[p][kpObjekt] = CreateDynamicObject(RACUNALO_MODEL, gKaficPc[p][kpX], gKaficPc[p][kpY], gKaficPc[p][kpZ], 0.0, 0.0, gKaficPc[p][kpA], gKaficPc[p][kpSvijet], gKaficPc[p][kpInt]);
}

static KaficBlizu(playerid, Float:domet)
{
    for (new k = 0; k < KAFIC_MAX; k++)
        if (gKafic[k][kfIma] && GetPlayerVirtualWorld(playerid) == gKafic[k][kfSvijet] && GetPlayerInterior(playerid) == gKafic[k][kfInt] && IsPlayerInRangeOfPoint(playerid, domet, gKafic[k][kfX], gKafic[k][kfY], gKafic[k][kfZ])) return k;
    return -1;
}

KaficRacunalo(playerid, bool:samoPlaceno = true)
{
    for (new p = 0; p < KAFIC_PC_MAX; p++)
    {
        if (!gKaficPc[p][kpIma] || GetPlayerVirtualWorld(playerid) != gKaficPc[p][kpSvijet] || GetPlayerInterior(playerid) != gKaficPc[p][kpInt]) continue;
        if (!IsPlayerInRangeOfPoint(playerid, KAFIC_PC_DOMET, gKaficPc[p][kpX], gKaficPc[p][kpY], gKaficPc[p][kpZ])) continue;
        if (!samoPlaceno) return 1;
        return gKaficIgrac[playerid] && gKaficIgrac[playerid] == gKaficPc[p][kpKafic] && gKaficDo[playerid] > gettime() ? 1 : 0;
    }
    return 0;
}

KaficKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new k = KaficBlizu(playerid, KAFIC_DOMET);
    if (k == -1) return 0;
    new list[160], red[48], len = 0;
    for (new i = 0; i < sizeof gKaficMinuta; i++)
    {
        format(red, sizeof red, T(playerid, "%d minuta - $%d"), gKaficMinuta[i], gKaficCijena[i]);
        DodajStupac(list, len, red);
    }
    AktorKaze(playerid, gKafic[k][kfAktor], "Blagajnik", "Dobar dan, koliko vremena zelite?");
    PrikaziDialog(playerid, DLG_KAFIC, DIALOG_STYLE_LIST, T(playerid, "Internet kafic"), list, T(playerid, "Plati"), T(playerid, "Zatvori"));
    return 1;
}

DLG:kafic(playerid, response, listitem)
{
    new k = KaficBlizu(playerid, KAFIC_DOMET);
    if (!response || k == -1 || listitem < 0 || listitem >= sizeof gKaficMinuta) return 1;
    if (!DajNovac(playerid, -gKaficCijena[listitem])) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    BlagajnaDodaj(gKaficCijena[listitem]);
    new sad = gettime();
    gKaficDo[playerid] = (gKaficIgrac[playerid] == gKafic[k][kfDb] && gKaficDo[playerid] > sad ? gKaficDo[playerid] : sad) + gKaficMinuta[listitem] * 60;
    gKaficIgrac[playerid] = gKafic[k][kfDb];
    AktorKaze(playerid, gKafic[k][kfAktor], "Blagajnik", "Izvolite, slobodno sjednite za bilo koje racunalo.");
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste $%d. Racunalo mozete koristiti jos %d minuta (/racunalo)."), gKaficCijena[listitem], (gKaficDo[playerid] - sad + 59) / 60);
}

KaficTick()
{
    new sad = gettime();
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gKaficDo[i] || gKaficDo[i] > sad) continue;
        gKaficDo[i] = 0;
        if (!IsPlayerConnected(i) || !RacunaloKaficIstek(i)) continue;
        for (new k = 0; k < KAFIC_MAX; k++)
            if (gKafic[k][kfIma] && gKafic[k][kfDb] == gKaficIgrac[i]) AktorKaze(i, gKafic[k][kfAktor], "Blagajnik", "Vase vrijeme je isteklo. Racunalo je ugaseno.");
    }
}

KaficUredi(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rz)
{
    new p = gKaficUredi[playerid];
    if (p == -1 || response == _:EDIT_RESPONSE_UPDATE) return 1;
    gKaficUredi[playerid] = -1;
    if (!gKaficPc[p][kpIma] || gKaficPc[p][kpObjekt] != objectid) return 1;
    if (response == _:EDIT_RESPONSE_FINAL && gAdmin[playerid] >= ALVL_ORG)
    {
        gKaficPc[p][kpX] = x;
        gKaficPc[p][kpY] = y;
        gKaficPc[p][kpZ] = z;
        gKaficPc[p][kpA] = rz;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE cafe_pcs SET x = %f, y = %f, z = %f, a = %f WHERE id = %d", x, y, z, rz, gKaficPc[p][kpDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pozicija racunala je spremljena."));
    }
    SetDynamicObjectPos(objectid, gKaficPc[p][kpX], gKaficPc[p][kpY], gKaficPc[p][kpZ]);
    SetDynamicObjectRot(objectid, 0.0, 0.0, gKaficPc[p][kpA]);
    return 1;
}

CMD:kaficint(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    PrebaciIgraca(playerid, KAFIC_MAPA_INT, 0, KAFIC_MAPA_X, KAFIC_MAPA_Y, KAFIC_MAPA_Z);
    return 1;
}

KaficDisconnect(playerid)
{
    gKaficUredi[playerid] = -1;
    gKaficDo[playerid] = 0;
    gKaficIgrac[playerid] = 0;
}

ShowKafici(playerid)
{
    new list[2048], red[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "+ Novi blagajnik na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "+ Novo racunalo ispred mene"));
    for (new k = 0; k < KAFIC_MAX; k++)
    {
        if (!gKafic[k][kfIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Blagajnik (%s) - obrisi"), gKafic[k][kfDb], CestaZona(gKafic[k][kfX], gKafic[k][kfY]));
        DodajStupac(list, len, red);
        gApKafic[playerid][n++] = k;
    }
    for (new p = 0; p < KAFIC_PC_MAX; p++)
    {
        if (!gKaficPc[p][kpIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Racunalo, blagajnik %d - obrisi"), gKaficPc[p][kpDb], gKaficPc[p][kpKafic]);
        DodajStupac(list, len, red);
        gApKafic[playerid][n++] = KAFIC_MAX + p;
    }
    return PrikaziDialog(playerid, DLG_APANEL_KAFIC, DIALOG_STYLE_LIST, T(playerid, "Internet kafici"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_kafic(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new Float:x, Float:y, Float:z, Float:a, unutra = GetPlayerInterior(playerid), svijet = GetPlayerVirtualWorld(playerid);
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    if (listitem == 0)
    {
        new k = 0;
        while (k < KAFIC_MAX && gKafic[k][kfIma]) k++;
        if (k == KAFIC_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj blagajnika.")), ShowKafici(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO cafe_cashiers (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", x, y, z, a, unutra, svijet));
        gKafic[k][kfDb] = ZadnjiId();
        gKafic[k][kfX] = x;
        gKafic[k][kfY] = y;
        gKafic[k][kfZ] = z;
        gKafic[k][kfA] = a;
        gKafic[k][kfInt] = unutra;
        gKafic[k][kfSvijet] = svijet;
        KaficStvori(k);
        SetPlayerPos(playerid, x + 1.5 * floatsin(-a, degrees), y + 1.5 * floatcos(-a, degrees), z);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Blagajnik internet kafica je postavljen."));
        return ShowKafici(playerid);
    }
    if (listitem == 1)
    {
        new p = 0, k = -1, Float:najblize = KAFIC_VEZA;
        while (p < KAFIC_PC_MAX && gKaficPc[p][kpIma]) p++;
        for (new i = 0; i < KAFIC_MAX; i++)
        {
            if (!gKafic[i][kfIma] || gKafic[i][kfInt] != unutra || gKafic[i][kfSvijet] != svijet) continue;
            new Float:d = VectorSize(gKafic[i][kfX] - x, gKafic[i][kfY] - y, gKafic[i][kfZ] - z);
            if (d < najblize) najblize = d, k = i;
        }
        if (p == KAFIC_PC_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj racunala.")), ShowKafici(playerid);
        if (k == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Prvo postavite blagajnika u blizini (%.0f m)."), KAFIC_VEZA), ShowKafici(playerid);
        x += 0.7 * floatsin(-a, degrees);
        y += 0.7 * floatcos(-a, degrees);
        z -= 0.2;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO cafe_pcs (cashier, x, y, z, a, interior, world) VALUES (%d, %f, %f, %f, %f, %d, %d)", gKafic[k][kfDb], x, y, z, a + 180.0, unutra, svijet));
        gKaficPc[p][kpDb] = ZadnjiId();
        gKaficPc[p][kpKafic] = gKafic[k][kfDb];
        gKaficPc[p][kpX] = x;
        gKaficPc[p][kpY] = y;
        gKaficPc[p][kpZ] = z;
        gKaficPc[p][kpA] = a + 180.0;
        gKaficPc[p][kpInt] = unutra;
        gKaficPc[p][kpSvijet] = svijet;
        KaficPcStvori(p);
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
        gKaficUredi[playerid] = p;
        EditDynamicObject(playerid, gKaficPc[p][kpObjekt]);
        return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Racunalo je postavljeno. Pomaknite ga i kliknite na disketu za spremanje."));
    }
    new i = listitem - 2;
    if (i < 0 || i >= KAFIC_MAX + KAFIC_PC_MAX) return 1;
    new s = gApKafic[playerid][i];
    if (s < KAFIC_MAX)
    {
        if (!gKafic[s][kfIma]) return ShowKafici(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM cafe_cashiers WHERE id = %d", gKafic[s][kfDb]));
        DestroyActor(gKafic[s][kfAktor]);
        DestroyDynamic3DTextLabel(gKafic[s][kfTekst]);
        gKafic[s][kfIma] = false;
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Blagajnik je obrisan."));
        return ShowKafici(playerid);
    }
    s -= KAFIC_MAX;
    if (!gKaficPc[s][kpIma]) return ShowKafici(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM cafe_pcs WHERE id = %d", gKaficPc[s][kpDb]));
    DestroyDynamicObject(gKaficPc[s][kpObjekt]);
    gKaficPc[s][kpIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Racunalo je obrisano."));
    return ShowKafici(playerid);
}
