#define TRGOVINA_MAX            32
#define TRGOVINA_SVIJET         3000
#define TRGOVINA_INT            17
#define TRGOVINA_DOMET          2.0
#define TRGOVINA_KASA_DOMET     2.5
#define TRGOVINA_SKIN           217
#define TRGOVINA_PLJACKA_S      3600
#define TRGOVINA_PREDAJA_MS     30000
#define TRGOVINA_RUKE_MS        60000
#define TRGOVINA_PLIJEN_MIN     300
#define TRGOVINA_PLIJEN_RASPON  700
#define TRGOVINA_BANDA          0
#define TRGOVINA_PULT           0.9
#define TRGOVINA_KUPAC          1.8
#define SRECKA_POLJA            9

#assert TRGOVINA_MAX <= FIRMA_MAX

enum E_TRGOVINA { bool:tgIma, tgDb, Float:tgX, Float:tgY, Float:tgZ, Float:tgA, bool:tgImaAktor, Float:tgAx, Float:tgAy, Float:tgAz, Float:tgAa, tgAktor, tgPljackano,
    STREAMER_TAG_PICKUP:tgPickup, STREAMER_TAG_PICKUP:tgPickupU, STREAMER_TAG_3D_TEXT_LABEL:tgTekst, STREAMER_TAG_3D_TEXT_LABEL:tgTekstU, STREAMER_TAG_3D_TEXT_LABEL:tgTekstA }

static gTrg[TRGOVINA_MAX][E_TRGOVINA];
static gApTrgovina[MAX_PLAYERS], gTrgPoruka[MAX_PLAYERS];
enum { TGTD_LIJEVO, TGTD_DESNO, TGTD_INFO, TGTD_KUPI, TGTD_IZADI, TGTD_BROJ }
static PlayerText:gTrgTd[MAX_PLAYERS][TGTD_BROJ], gTrgU[MAX_PLAYERS] = { -1, ... }, gTrgIdx[MAX_PLAYERS], gTrgObjekt[MAX_PLAYERS] = { INVALID_OBJECT_ID, ... };

forward PlayerText:SalonGumb(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, bool:gumb = true, Float:slovo = 0.4, Float:visina = 1.6, Float:kutija = 16.0);
static const Float:gTrgovinaUlaz[3] = { -25.884, -185.869, 1003.547 };
static const gTrgovinaRoba[][3] = { { ITEM_CRIJEVO, 25, 1 }, { ITEM_KANISTER, 25, 1 }, { ITEM_MOBITEL, 300, 1 }, { ITEM_CIGARETE, 8, 20 }, { ITEM_SRECKA, 5, 1 } };
static const gSreckaIznosi[] = { 5, 10, 25, 100, 1000 };
static gSreckaPolje[MAX_PLAYERS][SRECKA_POLJA], gSreckaOtvoreno[MAX_PLAYERS], bool:gSrecka[MAX_PLAYERS], PlayerText:gSreckaTd[MAX_PLAYERS][SRECKA_POLJA + 1];

InitTrgovina()
{
    SreckaProvjera();
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
    gTrg[s][tgTekst] = CreateDynamic3DTextLabel("24/7", COLOR_ORG, gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gTrg[s][tgTekst], TRGOVINA_DOMET, SAVJET_ULAZ);
    gTrg[s][tgPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gTrgovinaUlaz[0], gTrgovinaUlaz[1], gTrgovinaUlaz[2], svijet, TRGOVINA_INT);
    gTrg[s][tgTekstU] = CreateDynamic3DTextLabel("Izlaz", COLOR_ORG, gTrgovinaUlaz[0], gTrgovinaUlaz[1], gTrgovinaUlaz[2] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, TRGOVINA_INT);
    InterakcijaOznaka(gTrg[s][tgTekstU], TRGOVINA_DOMET, SAVJET_IZLAZ);
    FirmaUcitaj(FIRMA_TRGOVINA, s, gTrg[s][tgDb]);
}

static TrgovinaAktor(s)
{
    new svijet = TRGOVINA_SVIJET + s;
    gTrg[s][tgImaAktor] = true;
    gTrg[s][tgAktor] = CreateActor(TRGOVINA_SKIN, gTrg[s][tgAx], gTrg[s][tgAy], gTrg[s][tgAz], gTrg[s][tgAa]);
    SetActorVirtualWorld(gTrg[s][tgAktor], svijet);
    gTrg[s][tgTekstA] = CreateDynamic3DTextLabel(" ", COLOR_ORG, gTrg[s][tgAx], gTrg[s][tgAy], gTrg[s][tgAz] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, TRGOVINA_INT);
    InterakcijaOznaka(gTrg[s][tgTekstA]);
}

static TrgovinaMakniAktor(s)
{
    if (!gTrg[s][tgImaAktor]) return;
    DestroyActor(gTrg[s][tgAktor]);
    DestroyDynamic3DTextLabel(gTrg[s][tgTekstA]);
    gTrg[s][tgImaAktor] = false;
}

TrgovinaIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - TRGOVINA_SVIJET;
    return s >= 0 && s < TRGOVINA_MAX && gTrg[s][tgIma] && GetPlayerInterior(playerid) == TRGOVINA_INT ? s : -1;
}

static TrgovinaVani(playerid, s)
{
    TrgovinaZatvori(playerid);
    PrebaciIgraca(playerid, 0, 0, gTrg[s][tgX], gTrg[s][tgY], gTrg[s][tgZ], gTrg[s][tgA] + 180.0);
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
        PrebaciIgraca(playerid, TRGOVINA_INT, TRGOVINA_SVIJET + s, gTrgovinaUlaz[0], gTrgovinaUlaz[1], gTrgovinaUlaz[2]);
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
    if (gTrgU[playerid] != -1) return 1;
    AktorKaze(playerid, gTrg[s][tgAktor], "Prodavac", "Izvolite, sto ce biti?");
    TrgovinaOtvori(playerid, s);
    return 1;
}

static TrgovinaPult(s, &Float:x, &Float:y)
{
    new Float:a = gTrg[s][tgAa];
    x = gTrg[s][tgAx] + TRGOVINA_PULT * floatsin(-a, degrees);
    y = gTrg[s][tgAy] + TRGOVINA_PULT * floatcos(-a, degrees);
}

static TrgovinaPrikazi(playerid)
{
    new s = gTrgU[playerid], r = gTrgIdx[playerid], item = gTrgovinaRoba[r][0], info[96], Float:x, Float:y;
    if (gTrgovinaRoba[r][2] > 1) format(info, sizeof info, "%s x%d~n~~g~$%d~n~~w~%d/%d", T(playerid, gItems[item][ITEM_NAME]), gTrgovinaRoba[r][2], gTrgovinaRoba[r][1], r + 1, sizeof gTrgovinaRoba);
    else format(info, sizeof info, "%s~n~~g~$%d~n~~w~%d/%d", T(playerid, gItems[item][ITEM_NAME]), gTrgovinaRoba[r][1], r + 1, sizeof gTrgovinaRoba);
    PlayerTextDrawSetString(playerid, gTrgTd[playerid][TGTD_INFO], info);
    if (gTrgObjekt[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gTrgObjekt[playerid]);
    TrgovinaPult(s, x, y);
    gTrgObjekt[playerid] = CreatePlayerObject(playerid, gItems[item][ITEM_MODEL], x, y, gTrg[s][tgAz] + 0.05, 90.0, 0.0, gTrg[s][tgAa] + 90.0);
}

static TrgovinaOtvori(playerid, s)
{
    new Float:a = gTrg[s][tgAa], Float:x, Float:y;
    TrgovinaPult(s, x, y);
    gTrgU[playerid] = s;
    gTrgIdx[playerid] = 0;
    SetPlayerCameraPos(playerid, x + 1.1 * floatsin(-a, degrees), y + 1.1 * floatcos(-a, degrees), gTrg[s][tgAz] + 0.7);
    SetPlayerCameraLookAt(playerid, x, y, gTrg[s][tgAz] + 0.05);
    KupovinaGumbi(playerid, gTrgTd[playerid]);
    TrgovinaPrikazi(playerid);
}

TrgovinaZatvori(playerid)
{
    if (gTrgU[playerid] == -1) return;
    gTrgU[playerid] = -1;
    if (gTrgObjekt[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gTrgObjekt[playerid]);
    gTrgObjekt[playerid] = INVALID_OBJECT_ID;
    KupovinaZatvori(playerid, gTrgTd[playerid]);
}

TrgovinaKlik(playerid, PlayerText:td)
{
    if (gTrgU[playerid] == -1) return 0;
    if (td == gTrgTd[playerid][TGTD_IZADI] || TrgovinaIgraca(playerid) != gTrgU[playerid])
    {
        TrgovinaZatvori(playerid);
        return 1;
    }
    if (td == gTrgTd[playerid][TGTD_LIJEVO] || td == gTrgTd[playerid][TGTD_DESNO])
    {
        gTrgIdx[playerid] = (gTrgIdx[playerid] + (td == gTrgTd[playerid][TGTD_DESNO] ? 1 : sizeof gTrgovinaRoba - 1)) % sizeof gTrgovinaRoba;
        TrgovinaPrikazi(playerid);
        return 1;
    }
    if (td != gTrgTd[playerid][TGTD_KUPI]) return 1;
    new item = gTrgovinaRoba[gTrgIdx[playerid]][0], cijena = gTrgovinaRoba[gTrgIdx[playerid]][1];
    if (gNovac[playerid] < cijena) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    else if (!GiveItem(playerid, item, gTrgovinaRoba[gTrgIdx[playerid]][2])) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
    else
    {
        DajNovac(playerid, -cijena);
        FirmaProdaja(FIRMA_TRGOVINA, gTrgU[playerid], cijena, true);
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d)."), T(playerid, gItems[item][ITEM_NAME]), cijena);
    }
    return 1;
}

TrgovinaDisconnect(playerid)
{
    if (gSrecka[playerid]) SreckaKraj(playerid);
    gTrgU[playerid] = -1;
    gTrgObjekt[playerid] = INVALID_OBJECT_ID;
}

TrgovinaUpdate(playerid)
{
    new s = TrgovinaIgraca(playerid);
    if (s == -1 || !gTrg[s][tgImaAktor] || GetPlayerTargetActor(playerid) != gTrg[s][tgAktor]) return;
    new WEAPON:w = GetPlayerWeapon(playerid), sad = gettime();
    if (w < WEAPON_COLT45 || w > WEAPON_MINIGUN || !OruzjeLegit(playerid, w) || sad - gTrg[s][tgPljackano] < TRGOVINA_PLJACKA_S || sad - gTrgPoruka[playerid] < 10) return;
    if (!gOrg[playerid] || GetOrgTip(gOrg[playerid]) != TRGOVINA_BANDA)
    {
        gTrgPoruka[playerid] = sad;
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Samo bande mogu pljackati trgovine."));
        return;
    }
    gTrg[s][tgPljackano] = sad;
    ApplyActorAnimation(gTrg[s][tgAktor], "SHOP", "SHP_Rob_HandsUp", 4.1, false, false, false, true, 0);
    DodajWanted(playerid);
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pljacka je pocela! Ostanite u trgovini %d s dok blagajnik puni vrecu."), TRGOVINA_PREDAJA_MS / 1000);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (PolicajacNaDuznosti(i)) SendClientMessage(i, COLOR_ERROR, T(i, "Dojava: pljacka trgovine u tijeku, %s."), CestaZona(gTrg[s][tgX], gTrg[s][tgY]));
    SetTimerEx("TrgovinaPlijen", TRGOVINA_PREDAJA_MS, false, "iii", playerid, gAccountId[playerid], s);
}

forward TrgovinaPlijen(playerid, racun, s);
public TrgovinaPlijen(playerid, racun, s)
{
    if (!gTrg[s][tgImaAktor]) return;
    ApplyActorAnimation(gTrg[s][tgAktor], "SHOP", "SHP_Rob_GiveCash", 4.1, false, false, false, true, 0);
    SetTimerEx("TrgovinaRuke", TRGOVINA_RUKE_MS, false, "ii", s, gTrg[s][tgAktor]);
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    if (TrgovinaIgraca(playerid) != s || IsRanjen(playerid))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pljacka trgovine je propala."));
        return;
    }
    new plijen = TRGOVINA_PLIJEN_MIN + random(TRGOVINA_PLIJEN_RASPON);
    DajNovac(playerid, plijen);
    OrgDnevnik(gOrg[playerid], playerid, DNEVNIK_TRGOVINA, plijen, -1, CestaZona(gTrg[s][tgX], gTrg[s][tgY]));
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
    FirmaMakni(FIRMA_TRGOVINA, s);
    PumpeOdspoji(gTrg[s][tgDb]);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && TrgovinaIgraca(i) == s) TrgovinaVani(i, s);
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
        new list[384], len = 0;
        DodajStupac(list, len, T(playerid, "Postavi blagajnika na mojoj poziciji"));
        DodajStupac(list, len, T(playerid, "Obrisi trgovinu"));
        DodajStupac(list, len, T(playerid, "Nova pumpa za benzin spojena s ovom trgovinom (na mojoj poziciji)"));
        DodajStupac(list, len, T(playerid, "Nova pumpa za dizel spojena s ovom trgovinom (na mojoj poziciji)"));
        return PrikaziDialog(playerid, DLG_APANEL_TRGOVINA_MENI, DIALOG_STYLE_LIST, T(playerid, "Trgovine"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
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
    if (listitem == 2 || listitem == 3)
    {
        PumpaNova(playerid, listitem - 2, gTrg[s][tgDb]);
        return ShowTrgovine(playerid);
    }
    TrgovinaObrisi(s);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Trgovina obrisana."));
    return ShowTrgovine(playerid);
}

TrgovinaLokacija(i, &Float:x, &Float:y, &Float:z, &Float:a)
{
    if (i >= TRGOVINA_MAX) return -1;
    if (!gTrg[i][tgIma]) return 0;
    x = gTrg[i][tgX];
    y = gTrg[i][tgY];
    z = gTrg[i][tgZ];
    a = gTrg[i][tgA];
    return 1;
}

TrgovinaPoDb(db)
{
    for (new s = 0; s < TRGOVINA_MAX; s++) if (gTrg[s][tgIma] && gTrg[s][tgDb] == db) return s;
    return -1;
}

TrgovinaTocke(s, &interijer, &svijet, Float:ulaz[3], Float:cilj[4], Float:radnik[3])
{
    if (s < 0 || s >= TRGOVINA_MAX || !gTrg[s][tgIma] || !gTrg[s][tgImaAktor]) return 0;
    new Float:a = gTrg[s][tgAa];
    interijer = TRGOVINA_INT;
    svijet = TRGOVINA_SVIJET + s;
    for (new i = 0; i < 3; i++) ulaz[i] = gTrgovinaUlaz[i];
    cilj[0] = gTrg[s][tgAx] - TRGOVINA_KUPAC * floatsin(a, degrees);
    cilj[1] = gTrg[s][tgAy] + TRGOVINA_KUPAC * floatcos(a, degrees);
    cilj[2] = gTrg[s][tgAz];
    cilj[3] = a + 180.0;
    radnik[0] = gTrg[s][tgAx];
    radnik[1] = gTrg[s][tgAy];
    radnik[2] = gTrg[s][tgAz];
    return 1;
}

TrgovinaNasumicnaRoba(&item, &cijena)
{
    new r = random(sizeof gTrgovinaRoba);
    item = gTrgovinaRoba[r][0];
    cijena = gTrgovinaRoba[r][1];
}

static SreckaDobitak()
{
    new r = random(10000);
    return r < 5 ? 1000 : r < 65 ? 100 : r < 365 ? 25 : r < 1165 ? 10 : r < 3165 ? 5 : 0;
}

static SreckaPopuni(polja[SRECKA_POLJA], dobitak)
{
    new broj[sizeof gSreckaIznosi], n = 0;
    for (new v = 0; v < sizeof gSreckaIznosi; v++)
    {
        if (gSreckaIznosi[v] != dobitak) continue;
        broj[v] = 3;
        for (new i = 0; i < 3; i++) polja[n++] = dobitak;
    }
    while (n < SRECKA_POLJA)
    {
        new v = random(sizeof gSreckaIznosi);
        if (broj[v] >= 2) continue;
        broj[v]++;
        polja[n++] = gSreckaIznosi[v];
    }
    for (new i = SRECKA_POLJA - 1; i > 0; i--)
    {
        new j = random(i + 1), t = polja[i];
        polja[i] = polja[j];
        polja[j] = t;
    }
}

static SreckaPobjeda(const polja[SRECKA_POLJA])
{
    for (new v = 0; v < sizeof gSreckaIznosi; v++)
    {
        new n = 0;
        for (new i = 0; i < SRECKA_POLJA; i++) if (polja[i] == gSreckaIznosi[v]) n++;
        if (n >= 3) return gSreckaIznosi[v];
    }
    return 0;
}

static SreckaProvjera()
{
    new polja[SRECKA_POLJA];
    for (new k = 0; k < 50; k++)
    {
        new d = k % 2 ? 0 : gSreckaIznosi[k / 2 % sizeof gSreckaIznosi];
        SreckaPopuni(polja, d);
        assert(SreckaPobjeda(polja) == d);
    }
}

SreckaOgrebi(playerid)
{
    if (gSrecka[playerid] || gTrgU[playerid] != -1) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti pjeske."));
    if (!GiveItem(playerid, ITEM_SRECKA, -1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
    gSrecka[playerid] = true;
    gSreckaOtvoreno[playerid] = 0;
    SreckaPopuni(gSreckaPolje[playerid], SreckaDobitak());
    gSreckaTd[playerid][SRECKA_POLJA] = SalonGumb(playerid, 320.0, 140.0, T(playerid, "SRECKA~n~Pronadite 3 ista iznosa"), 0x1B5E20DD, 190.0, false);
    for (new i = 0; i < SRECKA_POLJA; i++) gSreckaTd[playerid][i] = SalonGumb(playerid, 260.0 + float(i % 3) * 60.0, 190.0 + float(i / 3) * 36.0, "?", 0xC9A227EE, 50.0);
    SelectTextDraw(playerid, 0xFFD700FF);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s grebe srecku.", GetRPIme(playerid), "", 1);
    return 1;
}

SreckaKlik(playerid, PlayerText:td)
{
    if (!gSrecka[playerid]) return 0;
    for (new i = 0; i < SRECKA_POLJA; i++)
    {
        if (td != gSreckaTd[playerid][i] || gSreckaOtvoreno[playerid] & (1 << i)) continue;
        new tekst[16];
        format(tekst, sizeof tekst, "$%d", gSreckaPolje[playerid][i]);
        gSreckaOtvoreno[playerid] |= 1 << i;
        PlayerTextDrawSetString(playerid, td, tekst);
        PlayerTextDrawBoxColour(playerid, td, 0x333333EE);
        PlayerTextDrawShow(playerid, td);
        PlayerPlaySound(playerid, 1053, 0.0, 0.0, 0.0);
        if (gSreckaOtvoreno[playerid] == (1 << SRECKA_POLJA) - 1) SreckaKraj(playerid);
        break;
    }
    return 1;
}

SreckaKraj(playerid)
{
    if (!gSrecka[playerid]) return;
    gSrecka[playerid] = false;
    for (new i = 0; i <= SRECKA_POLJA; i++) PlayerTextDrawDestroy(playerid, gSreckaTd[playerid][i]);
    CancelSelectTextDraw(playerid);
    new dobitak = SreckaPobjeda(gSreckaPolje[playerid]);
    if (!dobitak)
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Srecka: ovaj put nista. Vise srece drugi put!"));
        return;
    }
    DajNovac(playerid, dobitak);
    PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Srecka: osvojili ste $%d!"), dobitak);
}
