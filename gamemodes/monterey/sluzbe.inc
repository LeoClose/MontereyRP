#define SLUCAJ_MAX          10
#define SLUCAJ_OBJEKT        18688
#define SLUCAJ_TRAJANJE_S    180
#define SLUCAJ_POZIV_S       30
#define SLUCAJ_GASENJE_S     8
#define SLUCAJ_ODLAZAK_S     120
#define SLUCAJ_DOMET_APARAT  6.0
#define SLUCAJ_DOMET_CIJEV   25.0
#define SLUCAJ_RAZMAK        15.0
#define SLUCAJ_POVRATAK_S   90
#define SLUCAJ_IZLAZAK_MS    2500
#define SLUCAJ_DOMET_NPC     3.0
#define SLUCAJ_MUNICIJA      500
#define KAMION_MODEL        407
#define KAMION_SKIN         277
#define KAMION_BOJA         3
#define KAMION_MAX          24
#define MEHANICAR_IGRAC_DOMET 6.0
#define MEHANICAR_MODEL     525
#define MEHANICAR_SKIN      50
#define MEHANICAR_BOJA      1
#define MEHANICAR_DOMET     3.5
#define MEHANICAR_STETA     900.0
#define MEHANICAR_CEKA_S    60
#define MEHANICAR_GORIVO    50
#define MEHANICAR_POPRAVAK  150
#define MEHANICAR_GUME      100
#define BOLNICA_SPAWN_CIJENA 500
#define MEHANICAR_ODVOZ     2500
#define MEHANICAR_ODVOZ_MS  20000
#define MEHANICAR_ODVOZ_SVIJET 9900
#define MEHANICAR_ODVOZ_VOZILA 8

forward bool:VlasnikNaParkingu(v);
forward bool:GarazaIzvana(db, &Float:x, &Float:y);
#define MEHANICAR_SLEPANJE  250
#define RADIONICA_X         1207.18
#define RADIONICA_Y         -981.57
#define RADIONICA_Z         43.0
#define RADIONICA_A         0.0
#define MEHANICAR_ODGOVOR_MS 2000
#define MEHANICAR_RAD_MS    6000
#define KAMION_PO_TIPU      8
#define AMBULANTA_MODEL     416
#define AMBULANTA_SKIN      274
#define AMBULANTA_BOJA      1
#define RANJEN_TRAJANJE_S   300
#define RANJEN_CEKANJE_MS   10000
#define RANJEN_POGODAKA     5
#define RANJEN_CIJENA       100
#define RANJEN_DOMET_NPC    0.8
#define RANJEN_PRAG         20.0
#define RANJEN_IZLAZ_MS     1500
#define RANJEN_NPC          -1
#define RANJEN_MEDIK_DOMET  5.0
#define SLUCAJ_BRZE         1.3
#define SLUCAJ_RUB_DOMET    30.0
#define SLUCAJ_RUB_SIRINA   5.0
#define SLUCAJ_RUB_MS       6000
#define RANJEN_GRUPA        40.0
#define RANJEN_POMAK        1.0
#define RANJEN_POPRAVAK_MS  1000
#define KAMION_RESPAWN      120
#define KAMION_PARKING      15.0
#define SLUCAJ_CEKA_MS      2500
#define SLUCAJ_PROGURAJ_MS  8000
#define SLUCAJ_USPUT        40.0
#define SLUCAJ_PJESKE_DOMET 40.0

enum { SL_NEMA, SL_POZIV, SL_IGRAC, SL_NPC, SL_IZLAZI, SL_PJESKE, SL_PRISTANAK, SL_RADI, SL_GASI, SL_ODLAZI, SL_POVRATAK, SL_ULAZI, SL_KRAJ }
enum E_SLUCAJ { slStanje, slTip, slPacijent, slPoziv, STREAMER_TAG_OBJECT:slObjekt, slRok, slGasenje, slIgrac, slNpc, slVozilo, slIdx, slBroj, slKamion, slCeka, slProguraj, slSporo, bool:slObilazi, bool:slPjeske, slNastavi, bool:slNastavak, Float:slX, Float:slY, Float:slZ, slPosao }

static gSlucaj[SLUCAJ_MAX][E_SLUCAJ];
static Float:gSlucajTocke[SLUCAJ_MAX][CESTA_MAX_TOCAKA][4];
static gSlucajPoziv[MAX_PLAYERS], gNpcSlucaj[MAX_PLAYERS], gMehanicar[MAX_PLAYERS], gMehanicarVozilo[MAX_PLAYERS];

enum E_KAMION { bool:kIma, kTip, kDb, kVozilo, kSlucaj, Float:kX, Float:kY, Float:kZ, Float:kA }

static gKamion[KAMION_MAX][E_KAMION];
static gApKamionDb[MAX_PLAYERS], gApKamionTip[MAX_PLAYERS];

enum { RN_NE, RN_CEKA, RN_DOLJE }

static const Float:gBolnice[][4] = { { 1172.08, -1323.35, 15.40, 270.0 }, { 2034.20, -1401.67, 17.30, 180.0 } };
static bool:gRanjenBolnica[MAX_PLAYERS];
static gRanjenPonuda[MAX_PLAYERS], gRanjen[MAX_PLAYERS], gRanjenOd[MAX_PLAYERS], gRanjenPogoci[MAX_PLAYERS], gRanjenInt[MAX_PLAYERS], gRanjenSvijet[MAX_PLAYERS], gRanjenAnimId[MAX_PLAYERS], gRanjenPopravak[MAX_PLAYERS], Float:gRanjenPos[MAX_PLAYERS][4];

static PromijeniScore(org, delta)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO org_score (org, score) VALUES (%d, 0)", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE org_score SET score = score + %d WHERE org = %d", delta, org));
}

static GetOrgScore(org)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT score FROM org_score WHERE org = %d", org);
    new score = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return score;
}

InitSluzbe()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_score (org INTEGER PRIMARY KEY, score INTEGER NOT NULL)"));
    PromijeniScore(-1, 1);
    PromijeniScore(-1, -2);
    assert(GetOrgScore(-1) == -1);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_score WHERE org = -1"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS fire_trucks (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ambulances (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS tow_trucks (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    new k = 0;
    KamionUcitaj(ORG_VATROGASCI, k);
    KamionUcitaj(ORG_MEDICINSKA, k);
    KamionUcitaj(ORG_MEHANICAR, k);
}

static KamionUcitaj(tip, &k)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM %s ORDER BY id LIMIT %d", tip == ORG_VATROGASCI ? ("fire_trucks") : tip == ORG_MEDICINSKA ? ("ambulances") : ("tow_trucks"), KAMION_PO_TIPU);
    for (new i = DB_GetRowCount(res); i > 0; i--)
    {
        gKamion[k][kDb] = DB_GetFieldInt(res, 0);
        if (KamionStvori(k, tip, DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4))) k++;
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static KamionStvori(k, tip, Float:x, Float:y, Float:z, Float:a)
{
    new boja = tip == ORG_VATROGASCI ? KAMION_BOJA : tip == ORG_MEDICINSKA ? AMBULANTA_BOJA : MEHANICAR_BOJA;
    new v = CreateVehicle(tip == ORG_VATROGASCI ? KAMION_MODEL : tip == ORG_MEDICINSKA ? AMBULANTA_MODEL : MEHANICAR_MODEL, x, y, z, a, boja, boja, KAMION_RESPAWN);
    if (v == INVALID_VEHICLE_ID) return 0;
    gKamion[k][kIma] = true;
    gKamion[k][kTip] = tip;
    gKamion[k][kVozilo] = v;
    gKamion[k][kSlucaj] = 0;
    gKamion[k][kX] = x;
    gKamion[k][kY] = y;
    gKamion[k][kZ] = z;
    gKamion[k][kA] = a;
    return 1;
}

static KamionBroj(tip)
{
    new n = 0;
    for (new k = 0; k < KAMION_MAX; k++) if (gKamion[k][kIma] && gKamion[k][kTip] == tip) n++;
    return n;
}

ShowKamione(playerid, tip)
{
    new list[512], row[64], title[64], len = 0;
    gApKamionTip[playerid] = tip;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Novo vozilo na mojoj poziciji"));
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (!gKamion[k][kIma] || gKamion[k][kTip] != tip) continue;
        format(row, sizeof row, "[%d] %s", gKamion[k][kDb], CestaZona(gKamion[k][kX], gKamion[k][kY]));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, tip == ORG_VATROGASCI ? T(playerid, "Vatrogasni kamioni (%d/%d)") : tip == ORG_MEDICINSKA ? T(playerid, "Ambulante (%d/%d)") : T(playerid, "Mehanicarska vozila (%d/%d)"), KamionBroj(tip), KAMION_PO_TIPU);
    return PrikaziDialog(playerid, DLG_APANEL_KAMION, DIALOG_STYLE_LIST, title, list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static KamionNovi(playerid, tip)
{
    new Float:x, Float:y, Float:z, Float:a, Float:d, k = 0;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    CestaNajbliza(x, y, z, d);
    while (k < KAMION_MAX && gKamion[k][kIma]) k++;
    if (k == KAMION_MAX || KamionBroj(tip) >= KAMION_PO_TIPU || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || d > TAKSI_MAX_OD_CESTE || !KamionStvori(k, tip, x, y, z + 0.5, a))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo se ovdje ne moze stvoriti."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO %s (x, y, z, a) VALUES (%f, %f, %f, %f)", tip == ORG_VATROGASCI ? ("fire_trucks") : tip == ORG_MEDICINSKA ? ("ambulances") : ("tow_trucks"), x, y, z + 0.5, a));
    gKamion[k][kDb] = ZadnjiId();
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo dodano: %s."), CestaZona(x, y));
}

DLG:apanel_kamion(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new tip = gApKamionTip[playerid];
    if (listitem == 0)
    {
        KamionNovi(playerid, tip);
        return ShowKamione(playerid, tip);
    }
    new n = listitem;
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (!gKamion[k][kIma] || gKamion[k][kTip] != tip || --n) continue;
        new body[96];
        gApKamionDb[playerid] = gKamion[k][kDb];
        format(body, sizeof body, T(playerid, "Obrisati vozilo: %s?"), CestaZona(gKamion[k][kX], gKamion[k][kY]));
        return PrikaziDialog(playerid, DLG_APANEL_KAMION_BRISI, DIALOG_STYLE_MSGBOX, tip == ORG_VATROGASCI ? T(playerid, "Vatrogasni kamioni") : tip == ORG_MEDICINSKA ? T(playerid, "Ambulante") : T(playerid, "Mehanicarska vozila"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowKamione(playerid, tip);
}

DLG:apanel_kamion_brisi(playerid, response)
{
    new tip = gApKamionTip[playerid];
    if (!response) return ShowKamione(playerid, tip);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new k = 0;
    while (k < KAMION_MAX && (!gKamion[k][kIma] || gKamion[k][kTip] != tip || gKamion[k][kDb] != gApKamionDb[playerid])) k++;
    if (k == KAMION_MAX) return ShowKamione(playerid, tip);
    if (gKamion[k][kSlucaj])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo je trenutno na intervenciji, pokusajte kasnije."));
        return ShowKamione(playerid, tip);
    }
    DestroyVehicle(gKamion[k][kVozilo]);
    gKamion[k][kIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM %s WHERE id = %d", tip == ORG_VATROGASCI ? ("fire_trucks") : tip == ORG_MEDICINSKA ? ("ambulances") : ("tow_trucks"), gApKamionDb[playerid]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo obrisano."));
    return ShowKamione(playerid, tip);
}

SlucajNaslov(playerid, title[], size)
{
    new tip = GetOrgTip(gOrg[playerid]);
    if (tip != ORG_VATROGASCI && tip != ORG_MEDICINSKA) return;
    new row[32];
    format(row, sizeof row, T(playerid, " | Rezultat: %d"), GetOrgScore(gOrg[playerid]));
    strcat(title, row, size);
}

static bool:JeUSluzbi(playerid, tip)
{
    if (!gUlogiran[playerid] || !gOrg[playerid] || !NaDuznosti(playerid)) return false;
    new org = GetOrgTip(gOrg[playerid]);
    if (tip == ORG_MEHANICAR) return org == ORG_MEHANICAR;
    return (tip == ORG_VATROGASCI || tip == ORG_MEDICINSKA) && org == ORG_VATROGASCI;
}

static bool:SlucajImaIgraca(tip, osim)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (i != osim && JeUSluzbi(i, tip)) return true;
    return false;
}

static SlucajOcistiPozive(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gSlucajPoziv[i] == s + 1) gSlucajPoziv[i] = 0;
}

static bool:SlucajImaPoziva(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gSlucajPoziv[i] == s + 1) return true;
    return false;
}

static SlucajMakniKamion(s, bool:vrati = true)
{
    new npc = gSlucaj[s][slNpc], k = gSlucaj[s][slKamion] - 1;
    if (npc != INVALID_PLAYER_ID)
    {
        gNpcSlucaj[npc] = 0;
        NPC_Destroy(npc);
        gSlucaj[s][slNpc] = INVALID_PLAYER_ID;
    }
    if (k >= 0)
    {
        gKamion[k][kSlucaj] = 0;
        if (vrati) SetVehicleToRespawn(gKamion[k][kVozilo]);
    }
    gSlucaj[s][slKamion] = 0;
    gSlucaj[s][slVozilo] = 0;
}

static SlucajKorak(s)
{
    new i = gSlucaj[s][slIdx];
    NPC_Move(gSlucaj[s][slNpc], gSlucajTocke[s][i][0], gSlucajTocke[s][i][1], gSlucajTocke[s][i][2], NPC_MOVE_TYPE_DRIVE, gSlucajTocke[s][i][3] * float(gSlucaj[s][slSporo]) / 100.0 * (gSlucaj[s][slStanje] == SL_NPC ? SLUCAJ_BRZE : 1.0));
}

static SlucajVozi(s)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(gSlucaj[s][slVozilo], e, l, a, d, bn, bt, o);
    SetVehicleParamsEx(gSlucaj[s][slVozilo], 1, 1, a, d, bn, bt, o);
    NPC_StopMove(gSlucaj[s][slNpc]);
    NPC_UseVehicleSiren(gSlucaj[s][slNpc], gSlucaj[s][slStanje] == SL_NPC);
    gSlucaj[s][slCeka] = 0;
    gSlucaj[s][slProguraj] = 0;
    gSlucaj[s][slObilazi] = false;
    gSlucaj[s][slSporo] = 100;
    gSlucaj[s][slIdx] = 1;
    SlucajKorak(s);
}

static bool:KamionSlobodan(k)
{
    new Float:x, Float:y, Float:z;
    if (!gKamion[k][kIma] || gKamion[k][kSlucaj]) return false;
    GetVehiclePos(gKamion[k][kVozilo], x, y, z);
    if (VectorSize(x - gKamion[k][kX], y - gKamion[k][kY], z - gKamion[k][kZ]) > KAMION_PARKING) return false;
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerInVehicle(i, gKamion[k][kVozilo]) && GetPlayerState(i) == PLAYER_STATE_DRIVER) return false;
    return true;
}

static SlucajNajbliziKamion(s)
{
    new best = -1, Float:bd = 1.0e12;
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (gKamion[k][kTip] != gSlucaj[s][slTip] || !KamionSlobodan(k)) continue;
        new Float:d = VectorSize(gKamion[k][kX] - gSlucaj[s][slX], gKamion[k][kY] - gSlucaj[s][slY], 0.0);
        if (d < bd) { bd = d; best = k; }
    }
    return best;
}

static SlucajPutOdKamiona(s, v, cilj)
{
    new sekundi, Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    return CestaPut(CestaPocetak(x, y, z, a), cilj, gSlucajTocke[s], CESTA_MAX_TOCAKA, gSlucaj[s][slBroj], sekundi, true, x, y, z, a);
}

static SlucajGrupa(s)
{
    if (gSlucaj[s][slTip] != ORG_MEDICINSKA) return 0;
    for (new i = 0; i < SLUCAJ_MAX; i++)
    {
        new st = gSlucaj[i][slStanje];
        if (i != s && st >= SL_NPC && st < SL_ODLAZI && gSlucaj[i][slTip] == ORG_MEDICINSKA && gSlucaj[i][slVozilo] && VectorSize(gSlucaj[i][slX] - gSlucaj[s][slX], gSlucaj[i][slY] - gSlucaj[s][slY], 0.0) <= RANJEN_GRUPA) return 1;
    }
    return 0;
}

static SlucajPredaj(s, n, ostaje)
{
    new npc = gSlucaj[s][slNpc], k = gSlucaj[s][slKamion];
    SlucajOcistiPozive(n);
    gSlucaj[n][slNpc] = npc;
    gSlucaj[n][slVozilo] = gSlucaj[s][slVozilo];
    gSlucaj[n][slKamion] = k;
    gSlucaj[n][slPjeske] = gSlucaj[s][slPjeske];
    gNpcSlucaj[npc] = n + 1;
    gKamion[k - 1][kSlucaj] = n + 1;
    gSlucaj[s][slNpc] = INVALID_PLAYER_ID;
    gSlucaj[s][slVozilo] = 0;
    gSlucaj[s][slKamion] = 0;
    gSlucaj[s][slPjeske] = false;
    gSlucaj[s][slStanje] = ostaje;
}

static bool:SlucajPutDo(s, v)
{
    return SlucajPutOdKamiona(s, v, CestaNajbliza(gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ])) >= 0;
}

static SlucajNaPut(s)
{
    NPC_SetKeys(gSlucaj[s][slNpc], 0, 0, 0);
    gSlucaj[s][slStanje] = SL_NPC;
    SlucajVozi(s);
}

static bool:SlucajUsput(s)
{
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gSlucaj[s][slVozilo], x, y, z);
    new Float:doCilja = VectorSize(gSlucaj[s][slX] - x, gSlucaj[s][slY] - y, 0.0);
    for (new n = 0; n < SLUCAJ_MAX; n++)
    {
        if (n == s || gSlucaj[n][slStanje] != SL_POZIV || gSlucaj[n][slVozilo] || gSlucaj[n][slTip] != gSlucaj[s][slTip]) continue;
        new Float:d = VectorSize(gSlucaj[n][slX] - x, gSlucaj[n][slY] - y, 0.0);
        if (d > SLUCAJ_USPUT || d >= doCilja || !SlucajPutDo(n, gSlucaj[s][slVozilo])) continue;
        SlucajPredaj(s, n, SL_POZIV);
        gSlucaj[n][slNastavi] = s + 1;
        SlucajNaPut(n);
        return true;
    }
    return false;
}

static SlucajNajbliziPovratak(s, &Float:najblize)
{
    new best = -1, Float:x, Float:y, Float:z;
    najblize = 1.0e12;
    for (new i = 0; i < SLUCAJ_MAX; i++)
    {
        if (i == s || gSlucaj[i][slStanje] != SL_ODLAZI || gSlucaj[i][slTip] != gSlucaj[s][slTip] || !gSlucaj[i][slVozilo] || gSlucaj[i][slPjeske]) continue;
        GetVehiclePos(gSlucaj[i][slVozilo], x, y, z);
        new Float:d = VectorSize(x - gSlucaj[s][slX], y - gSlucaj[s][slY], 0.0);
        if (d < najblize)
        {
            najblize = d;
            best = i;
        }
    }
    return best;
}

static SlucajPosaljiKamion(s)
{
    SlucajOcistiPozive(s);
    for (new i = 0; i < SLUCAJ_MAX; i++) if (gSlucaj[i][slNastavi] == s + 1 && gSlucaj[i][slVozilo]) return;
    if (SlucajGrupa(s)) return;
    new k = SlucajNajbliziKamion(s), Float:dPovratak, r = SlucajNajbliziPovratak(s, dPovratak);
    if (r != -1 && (k == -1 || dPovratak < VectorSize(gKamion[k][kX] - gSlucaj[s][slX], gKamion[k][kY] - gSlucaj[s][slY], 0.0)) && SlucajPutDo(s, gSlucaj[r][slVozilo]))
    {
        SlucajPredaj(r, s, SL_NEMA);
        SlucajNaPut(s);
        return;
    }
    if (k == -1) return;
    gSlucaj[s][slStanje] = SL_NPC;
    new v = gKamion[k][kVozilo];
    if (SlucajPutOdKamiona(s, v, CestaNajbliza(gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ])) < 0) return;
    new name[MAX_PLAYER_NAME + 1];
    format(name, sizeof name, gSlucaj[s][slTip] == ORG_VATROGASCI ? ("Vatrogasac_%d") : gSlucaj[s][slTip] == ORG_MEDICINSKA ? ("Bolnicar_%d") : ("Mehanicar_%d"), s);
    new npc = NPC_Create(name);
    if (npc == INVALID_PLAYER_ID) return;
    gSlucaj[s][slNpc] = npc;
    gSlucaj[s][slVozilo] = v;
    gSlucaj[s][slIdx] = 0;
    gSlucaj[s][slKamion] = k + 1;
    gKamion[k][kSlucaj] = s + 1;
    gNpcSlucaj[npc] = s + 1;
    NPC_Spawn(npc);
}

SlucajSpawn(npcid)
{
    new s = gNpcSlucaj[npcid] - 1;
    if (s < 0) return;
    NPC_SetSkin(npcid, gSlucaj[s][slTip] == ORG_VATROGASCI ? KAMION_SKIN : gSlucaj[s][slTip] == ORG_MEDICINSKA ? AMBULANTA_SKIN : MEHANICAR_SKIN);
    NPC_SetInvulnerable(npcid, true);
    NPC_PutInVehicle(npcid, gSlucaj[s][slVozilo], 0);
    SetTimerEx("SlucajKreni", 1000, false, "i", s);
}

forward SlucajKreni(s);
public SlucajKreni(s)
{
    if (gSlucaj[s][slStanje] == SL_NPC && gSlucaj[s][slVozilo]) SlucajVozi(s);
}

SlucajNpcStigao(npcid)
{
    new s = gNpcSlucaj[npcid] - 1;
    if (s < 0) return;
    new st = gSlucaj[s][slStanje], bool:vatra = gSlucaj[s][slTip] == ORG_VATROGASCI;
    if (st == SL_RADI)
    {
        MehanicarAnim(s);
        return;
    }
    if (st == SL_PJESKE)
    {
        if (gSlucaj[s][slTip] == ORG_MEHANICAR)
        {
            gSlucaj[s][slStanje] = SL_PRISTANAK;
            gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
            MehanicarPonudi(gSlucaj[s][slPacijent], s);
            return;
        }
        if (!vatra)
        {
            gSlucaj[s][slStanje] = SL_PRISTANAK;
            RanjenPonudi(gSlucaj[s][slPacijent], RANJEN_NPC);
            return;
        }
        gSlucaj[s][slStanje] = SL_GASI;
        NPC_AimAt(npcid, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ] - CESTA_Z_VOZILA, true);
        return;
    }
    if (st == SL_POVRATAK)
    {
        gSlucaj[s][slStanje] = SL_ULAZI;
        NPC_EnterVehicle(npcid, gSlucaj[s][slVozilo], 0, NPC_MOVE_TYPE_WALK);
        return;
    }
    if ((st != SL_NPC && st != SL_ODLAZI) || gSlucaj[s][slCeka]) return;
    if (gSlucaj[s][slObilazi])
    {
        gSlucaj[s][slObilazi] = false;
        gSlucaj[s][slProguraj] = 0;
        CestaNastavi(gSlucaj[s][slVozilo], gSlucajTocke[s], gSlucaj[s][slBroj], gSlucaj[s][slIdx]);
        SlucajKorak(s);
        return;
    }
    if (++gSlucaj[s][slIdx] < gSlucaj[s][slBroj])
    {
        SlucajKorak(s);
        return;
    }
    if (st == SL_ODLAZI)
    {
        gSlucaj[s][slStanje] = SL_KRAJ;
        return;
    }
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gSlucaj[s][slVozilo], x, y, z);
    if (vatra && VectorSize(gSlucaj[s][slX] - x, gSlucaj[s][slY] - y, 0.0) <= SLUCAJ_DOMET_CIJEV)
    {
        gSlucaj[s][slStanje] = SL_GASI;
        NPC_SetKeys(npcid, 0, 0, _:KEY_FIRE);
        return;
    }
    gSlucaj[s][slStanje] = SL_IZLAZI;
    gSlucaj[s][slPjeske] = true;
    new e, l, al, d, bn, bt, o;
    GetVehicleParamsEx(gSlucaj[s][slVozilo], e, l, al, d, bn, bt, o);
    SetVehicleParamsEx(gSlucaj[s][slVozilo], e, 0, al, d, bn, bt, o);
    NPC_ExitVehicle(npcid);
    SetTimerEx("SlucajPjeske", SLUCAJ_IZLAZAK_MS, false, "i", s);
}

static SlucajMakniTaksije(v, Float:x, Float:y, Float:z)
{
    new Float:a;
    GetVehicleZAngle(v, a);
    new Float:fx = -floatsin(a, degrees), Float:fy = floatcos(a, degrees);
    for (new i = 0; i < gVozilaCount; i++)
    {
        new w = gVozila[i], t = TaksiUVoznji(w), Float:vx, Float:vy, Float:vz, Float:wa;
        if (!t || w == v) continue;
        GetVehiclePos(w, vx, vy, vz);
        GetVehicleZAngle(w, wa);
        vx -= x;
        vy -= y;
        new Float:uz = vx * fx + vy * fy;
        if (uz <= 0.0 || uz > SLUCAJ_RUB_DOMET || floatabs(vx * fy - vy * fx) > SLUCAJ_RUB_SIRINA || floatabs(vz - z) > 4.0 || -floatsin(wa, degrees) * fx + floatcos(wa, degrees) * fy < 0.3) continue;
        TaksiNaRub(t - 1, SLUCAJ_RUB_MS);
    }
}

SlucajPrepreke(sad, bool:snimljeno)
{
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        new st = gSlucaj[s][slStanje], v = gSlucaj[s][slVozilo], bool:kupac, Float:x, Float:y, Float:z, Float:ox, Float:oy;
        if ((st != SL_NPC && st != SL_ODLAZI) || !v || !gSlucaj[s][slIdx] || sad - gSlucaj[s][slProguraj] < 0) continue;
        if (gSlucaj[s][slObilazi])
        {
            gSlucaj[s][slObilazi] = false;
            CestaNastavi(v, gSlucajTocke[s], gSlucaj[s][slBroj], gSlucaj[s][slIdx]);
            SlucajKorak(s);
        }
        if (!snimljeno)
        {
            TaksiSnimi();
            snimljeno = true;
        }
        GetVehiclePos(v, x, y, z);
        if (st == SL_NPC) SlucajMakniTaksije(v, x, y, z);
        new Float:d = CestaPrepreka(v, gSlucaj[s][slPacijent], x, y, z, kupac, ox, oy);
        gSlucaj[s][slSporo] = d >= TAKSI_USPORI ? 100 : clamp(floatround(d / TAKSI_USPORI * 100.0), 30, 100);
        if (d >= TAKSI_STOP)
        {
            if (gSlucaj[s][slCeka])
            {
                gSlucaj[s][slCeka] = 0;
                SlucajKorak(s);
            }
            continue;
        }
        if (st == SL_NPC && (kupac || VectorSize(gSlucaj[s][slX] - x, gSlucaj[s][slY] - y, 0.0) <= SLUCAJ_PJESKE_DOMET))
        {
            NPC_StopMove(gSlucaj[s][slNpc]);
            gSlucaj[s][slCeka] = 0;
            gSlucaj[s][slIdx] = gSlucaj[s][slBroj] - 1;
            SlucajNpcStigao(gSlucaj[s][slNpc]);
        }
        else if (!gSlucaj[s][slCeka])
        {
            gSlucaj[s][slCeka] = sad;
            NPC_StopMove(gSlucaj[s][slNpc]);
        }
        else if (sad - gSlucaj[s][slCeka] >= SLUCAJ_CEKA_MS)
        {
            gSlucaj[s][slCeka] = 0;
            gSlucaj[s][slProguraj] = sad + SLUCAJ_PROGURAJ_MS;
            gSlucaj[s][slSporo] = 100;
            if (CestaObidi(gSlucaj[s][slNpc], v, ox, oy, gSlucajTocke[s][gSlucaj[s][slIdx]][3])) gSlucaj[s][slObilazi] = true;
            else
            {
                gSlucaj[s][slCeka] = sad;
                gSlucaj[s][slProguraj] = 0;
            }
        }
    }
}

forward SlucajPjeske(s);
public SlucajPjeske(s)
{
    if (gSlucaj[s][slStanje] != SL_IZLAZI) return;
    new npc = gSlucaj[s][slNpc], bool:vatra = gSlucaj[s][slTip] == ORG_VATROGASCI;
    if (NPC_GetVehicle(npc) == gSlucaj[s][slVozilo]) NPC_RemoveFromVehicle(npc);
    if (vatra)
    {
        NPC_SetAmmo(npc, SLUCAJ_MUNICIJA);
        NPC_SetWeapon(npc, WEAPON_FIREEXTINGUISHER);
    }
    gSlucaj[s][slStanje] = SL_PJESKE;
    NPCHodaj(npc, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ], NPC_MOVE_TYPE_JOG, NPC_MOVE_SPEED_AUTO, vatra ? SLUCAJ_DOMET_NPC : gSlucaj[s][slTip] == ORG_MEHANICAR ? MEHANICAR_DOMET : RANJEN_DOMET_NPC);
}

static SlucajUKamion(s)
{
    new npc = gSlucaj[s][slNpc], Float:x, Float:y, Float:z;
    NPC_StopMove(npc);
    NPC_StopAim(npc);
    NPC_ClearAnimations(npc);
    NPC_SetWeapon(npc, WEAPON_FIST);
    GetVehiclePos(gSlucaj[s][slVozilo], x, y, z);
    NPCHodaj(npc, x, y, z, NPC_MOVE_TYPE_JOG, NPC_MOVE_SPEED_AUTO, 4.0);
    gSlucaj[s][slStanje] = SL_POVRATAK;
    gSlucaj[s][slRok] = SLUCAJ_POVRATAK_S;
}

static SlucajVratiKamion(s)
{
    new k = gSlucaj[s][slKamion] - 1;
    NPC_SetKeys(gSlucaj[s][slNpc], 0, 0, 0);
    if (SlucajPutOdKamiona(s, gSlucaj[s][slVozilo], CestaNajbliza(gKamion[k][kX], gKamion[k][kY], gKamion[k][kZ])) < 0)
    {
        gSlucaj[s][slStanje] = SL_KRAJ;
        return;
    }
    gSlucaj[s][slStanje] = SL_ODLAZI;
    gSlucaj[s][slRok] = SLUCAJ_ODLAZAK_S;
    SlucajVozi(s);
}

static bool:SlucajSljedeci(s)
{
    new n = gSlucaj[s][slNastavi] - 1;
    gSlucaj[s][slNastavi] = 0;
    new bool:nastavak = n >= 0 && gSlucaj[n][slStanje] == SL_POZIV && !gSlucaj[n][slVozilo];
    if (!nastavak)
    {
        if (gSlucaj[s][slTip] != ORG_MEDICINSKA) return false;
        n = -1;
        new Float:bd = RANJEN_GRUPA;
        for (new i = 0; i < SLUCAJ_MAX; i++)
        {
            if (i == s || gSlucaj[i][slStanje] != SL_POZIV || gSlucaj[i][slTip] != ORG_MEDICINSKA || gSlucaj[i][slVozilo]) continue;
            new Float:d = VectorSize(gSlucaj[i][slX] - gSlucaj[s][slX], gSlucaj[i][slY] - gSlucaj[s][slY], 0.0);
            if (d <= bd) { bd = d; n = i; }
        }
        if (n == -1) return false;
    }
    new st = gSlucaj[s][slStanje], npc = gSlucaj[s][slNpc], bool:pjeske = gSlucaj[s][slPjeske];
    if (!pjeske && !SlucajPutDo(n, gSlucaj[s][slVozilo])) return false;
    SlucajPredaj(s, n, SL_NEMA);
    if (!pjeske)
    {
        SlucajNaPut(n);
        return true;
    }
    if (nastavak)
    {
        gSlucaj[n][slNastavak] = true;
        SlucajUKamion(n);
        return true;
    }
    if (st == SL_IZLAZI)
    {
        gSlucaj[n][slStanje] = SL_IZLAZI;
        SetTimerEx("SlucajPjeske", SLUCAJ_IZLAZAK_MS, false, "i", n);
        return true;
    }
    NPC_StopMove(npc);
    NPC_ClearAnimations(npc);
    gSlucaj[n][slStanje] = SL_PJESKE;
    NPCHodaj(npc, gSlucaj[n][slX], gSlucaj[n][slY], gSlucaj[n][slZ], NPC_MOVE_TYPE_JOG, NPC_MOVE_SPEED_AUTO, RANJEN_DOMET_NPC);
    return true;
}

static SlucajZatvori(s)
{
    SlucajOcistiPozive(s);
    if (gSlucaj[s][slIgrac] != INVALID_PLAYER_ID) DisablePlayerCheckpoint(gSlucaj[s][slIgrac]);
    gSlucaj[s][slIgrac] = INVALID_PLAYER_ID;
    gSlucaj[s][slPacijent] = INVALID_PLAYER_ID;
    if (!gSlucaj[s][slVozilo]) gSlucaj[s][slStanje] = SL_NEMA;
    else if (SlucajSljedeci(s)) return;
    else if (gSlucaj[s][slPjeske]) SlucajUKamion(s);
    else SlucajVratiKamion(s);
}

static SlucajKraj(s, bool:ok, ko = INVALID_PLAYER_ID)
{
    new tip = gSlucaj[s][slTip], p = gSlucaj[s][slPacijent];
    if (tip == ORG_MEHANICAR)
    {
        if (p != INVALID_PLAYER_ID)
        {
            if (gMehanicar[p] == s + 1) gMehanicar[p] = 0;
            if (!ok) SendClientMessage(p, COLOR_ERROR, T(p, "Mehanicar je otisao."));
        }
        SlucajZatvori(s);
        return;
    }
    if (tip == ORG_VATROGASCI) DestroyDynamicObject(gSlucaj[s][slObjekt]);
    else RanjenPrimijeni(p, ok);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM orgs WHERE type = %d", tip);
    if (DB_GetRowCount(res)) do PromijeniScore(DB_GetFieldInt(res, 0), ok ? 1 : -1); while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!JeUSluzbi(i, tip)) continue;
        new score = GetOrgScore(gOrg[i]);
        if (tip == ORG_VATROGASCI)
        {
            if (!ok) SendClientMessage(i, COLOR_ORG, T(i, "Pozar nije ugasen na vrijeme. Rezultat vatrogasaca: %d."), score);
            else if (ko == INVALID_PLAYER_ID) SendClientMessage(i, COLOR_ORG, T(i, "NPC vatrogasci su ugasili pozar. Rezultat vatrogasaca: %d."), score);
            else SendClientMessage(i, COLOR_ORG, T(i, "%s je ugasio pozar. Rezultat vatrogasaca: %d."), GetRPIme(ko), score);
        }
        else if (!ok) SendClientMessage(i, COLOR_ORG, T(i, "%s nije spasen. Rezultat hitne: %d."), GetRPIme(p), score);
        else if (ko == INVALID_PLAYER_ID) SendClientMessage(i, COLOR_ORG, T(i, "NPC bolnicari su spasili %s. Rezultat hitne: %d."), GetRPIme(p), score);
        else SendClientMessage(i, COLOR_ORG, T(i, "%s je spasio %s. Rezultat hitne: %d."), GetRPIme(ko), GetRPIme(p), score);
    }
    SlucajZatvori(s);
}

static bool:SlucajPrska(i, s)
{
    new KEY:k, ud, lr, Float:domet = SLUCAJ_DOMET_APARAT;
    GetPlayerKeys(i, k, ud, lr);
    if (GetPlayerState(i) == PLAYER_STATE_DRIVER)
    {
        if (!(k & (KEY_FIRE | KEY_ACTION)) || GetVehicleModel(GetPlayerVehicleID(i)) != KAMION_MODEL) return false;
        domet = SLUCAJ_DOMET_CIJEV;
    }
    else if (GetPlayerState(i) != PLAYER_STATE_ONFOOT || !(k & KEY_FIRE) || GetPlayerWeapon(i) != WEAPON_FIREEXTINGUISHER) return false;
    return IsPlayerInRangeOfPoint(i, domet, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ]) && JeUSluzbi(i, ORG_VATROGASCI);
}

static SlucajKoPrska(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && SlucajPrska(i, s)) return i;
    return INVALID_PLAYER_ID;
}

SlucajTick()
{
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        new st = gSlucaj[s][slStanje];
        if (st == SL_NEMA) continue;
        if (st == SL_KRAJ)
        {
            SlucajMakniKamion(s);
            gSlucaj[s][slStanje] = SL_NEMA;
            continue;
        }
        if (st == SL_POVRATAK || st == SL_ULAZI)
        {
            new npc = gSlucaj[s][slNpc], v = gSlucaj[s][slVozilo];
            if (NPC_GetVehicle(npc) == v && !NPC_IsEnteringVehicle(npc))
            {
                gSlucaj[s][slPjeske] = false;
                if (!gSlucaj[s][slNastavak]) SlucajVratiKamion(s);
                else
                {
                    gSlucaj[s][slNastavak] = false;
                    if (SlucajPutDo(s, v)) SlucajNaPut(s);
                    else
                    {
                        SlucajMakniKamion(s);
                        gSlucaj[s][slStanje] = SL_POZIV;
                    }
                }
            }
            else if (--gSlucaj[s][slRok] <= 0)
            {
                NPC_PutInVehicle(npc, v, 0);
                gSlucaj[s][slRok] = SLUCAJ_POVRATAK_S;
            }
            continue;
        }
        if (--gSlucaj[s][slRok] <= 0)
        {
            if (st == SL_ODLAZI) gSlucaj[s][slStanje] = SL_KRAJ;
            else SlucajKraj(s, false);
            continue;
        }
        if (st == SL_ODLAZI) continue;
        if (st == SL_IGRAC && gSlucaj[s][slTip] == ORG_MEHANICAR)
        {
            MehanicarIgracStigao(s);
            continue;
        }
        if (st == SL_NPC && SlucajUsput(s)) continue;
        if (st == SL_POZIV && (--gSlucaj[s][slPoziv] <= 0 || !SlucajImaPoziva(s))) SlucajPosaljiKamion(s);
        new ko = gSlucaj[s][slTip] == ORG_VATROGASCI ? SlucajKoPrska(s) : INVALID_PLAYER_ID;
        if ((st == SL_GASI || ko != INVALID_PLAYER_ID) && ++gSlucaj[s][slGasenje] >= SLUCAJ_GASENJE_S) SlucajKraj(s, true, ko);
    }
}

static SlucajNovi(tip, Float:x, Float:y, Float:z, pacijent = INVALID_PLAYER_ID)
{
    new s = -1, bool:vatra = tip == ORG_VATROGASCI;
    for (new i = SLUCAJ_MAX - 1; i >= 0; i--)
    {
        new st = gSlucaj[i][slStanje];
        if (st == SL_NEMA) s = i;
        else if (vatra && gSlucaj[i][slTip] == tip && st < SL_ODLAZI && VectorSize(gSlucaj[i][slX] - x, gSlucaj[i][slY] - y, 0.0) < SLUCAJ_RAZMAK) return -1;
    }
    if (s == -1) return -1;
    gSlucaj[s][slStanje] = SL_POZIV;
    gSlucaj[s][slTip] = tip;
    gSlucaj[s][slPacijent] = pacijent;
    gSlucaj[s][slPoziv] = SLUCAJ_POZIV_S;
    if (vatra) gSlucaj[s][slObjekt] = CreateDynamicObject(SLUCAJ_OBJEKT, x, y, z - CESTA_Z_VOZILA, 0.0, 0.0, 0.0, 0);
    gSlucaj[s][slRok] = vatra ? SLUCAJ_TRAJANJE_S : RANJEN_TRAJANJE_S;
    gSlucaj[s][slGasenje] = 0;
    gSlucaj[s][slIgrac] = INVALID_PLAYER_ID;
    gSlucaj[s][slNpc] = INVALID_PLAYER_ID;
    gSlucaj[s][slVozilo] = 0;
    gSlucaj[s][slKamion] = 0;
    gSlucaj[s][slPjeske] = false;
    gSlucaj[s][slNastavi] = 0;
    gSlucaj[s][slNastavak] = false;
    gSlucaj[s][slPosao] = 0;
    gSlucaj[s][slX] = x;
    gSlucaj[s][slY] = y;
    gSlucaj[s][slZ] = z;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!JeUSluzbi(i, tip) || i == pacijent) continue;
        new body[128], Float:px, Float:py, Float:pz, d;
        GetPlayerPos(i, px, py, pz);
        d = floatround(VectorSize(px - x, py - y, 0.0));
        format(body, sizeof body, vatra ? T(i, "Gori vozilo %d m od vas. Prihvacate li poziv?") : tip == ORG_MEHANICAR ? T(i, "Vozilo u kvaru %d m od vas. Prihvacate li poziv?") : T(i, "Ranjena osoba %d m od vas. Prihvacate li poziv?"), d);
        gSlucajPoziv[i] = s + 1;
        PrikaziDialog(i, DLG_SLUCAJ, DIALOG_STYLE_MSGBOX, vatra ? T(i, "Poziv vatrogascima") : tip == ORG_MEHANICAR ? T(i, "Poziv mehanicaru") : T(i, "Poziv hitnoj"), body, T(i, "Prihvati"), T(i, "Odbij"));
    }
    return s;
}

SlucajEksplozija(v)
{
    new Float:x, Float:y, Float:z, bool:ima = GetVehiclePos(v, x, y, z) && !GetVehicleVirtualWorld(v) && z > 1.0;
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        if (!gSlucaj[s][slStanje] || gSlucaj[s][slVozilo] != v) continue;
        SlucajMakniKamion(s, false);
        gSlucaj[s][slPjeske] = false;
        gSlucaj[s][slStanje] = gSlucaj[s][slStanje] >= SL_ODLAZI ? SL_NEMA : SL_POZIV;
    }
    if (ima) SlucajNovi(ORG_VATROGASCI, x, y, z);
}

JeKamion(v)
{
    for (new k = 0; k < KAMION_MAX; k++) if (gKamion[k][kIma] && gKamion[k][kVozilo] == v) return 1;
    return 0;
}

SlucajStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_DRIVER || IsPlayerNPC(playerid)) return;
    new v = GetPlayerVehicleID(playerid);
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (!gKamion[k][kIma] || gKamion[k][kVozilo] != v) continue;
        if (JeUSluzbi(playerid, gKamion[k][kTip])) return;
        RemovePlayerFromVehicle(playerid);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Samo sluzba moze voziti ovo vozilo."));
        return;
    }
}

SlucajDisconnect(playerid)
{
    gSlucajPoziv[playerid] = 0;
    gMehanicar[playerid] = 0;
    gRanjen[playerid] = RN_NE;
    gRanjenPonuda[playerid] = 0;
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        if (gSlucaj[s][slStanje] && gSlucaj[s][slStanje] < SL_ODLAZI && (gSlucaj[s][slTip] == ORG_MEDICINSKA || gSlucaj[s][slTip] == ORG_MEHANICAR) && gSlucaj[s][slPacijent] == playerid)
        {
            SlucajZatvori(s);
            continue;
        }
        if (gSlucaj[s][slStanje] != SL_IGRAC || gSlucaj[s][slIgrac] != playerid) continue;
        gSlucaj[s][slIgrac] = INVALID_PLAYER_ID;
        gSlucaj[s][slStanje] = SL_POZIV;
    }
}

ShowMehanicare(playerid)
{
    return ShowKamione(playerid, ORG_MEHANICAR);
}

static gOdvozVozila[MAX_PLAYERS][MEHANICAR_ODVOZ_VOZILA], gOdvozBroj[MAX_PLAYERS], bool:gOdvoz[MAX_VEHICLES];

static MehanicarOdvozPoziv(playerid)
{
    new vozila[MEHANICAR_ODVOZ_VOZILA];
    if (!VlasnistvaIgraca(playerid, vozila)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate vlastitih vozila."));
    MobitelPoziv(playerid, T(playerid, "Mehanicar"));
    SetTimerEx("MehanicarOdvozPitaj", MEHANICAR_ODGOVOR_MS, false, "i", playerid);
    return 1;
}

static MehanicarOdvozGreska(playerid, const tekst[])
{
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, tekst));
    TaksiSpustiMobitel(playerid);
    return 1;
}

forward MehanicarOdvozPitaj(playerid);
public MehanicarOdvozPitaj(playerid)
{
    if (!IsPlayerConnected(playerid) || !gUlogiran[playerid]) return;
    SendClientMessage(playerid, COLOR_CHAT, T(playerid, "Dispecer (telefon): Mehanicarski servis, izvolite. Koje vozilo trebate odslepati i odakle?"));
    MehanicarOdvozIzbor(playerid);
}

static MehanicarOdvozIzbor(playerid)
{
    gOdvozBroj[playerid] = VlasnistvaIgraca(playerid, gOdvozVozila[playerid]);
    if (!gOdvozBroj[playerid]) return MehanicarOdvozGreska(playerid, "Nemate vlastitih vozila.");
    new list[512], row[96], naslov[64], len = 0, Float:x, Float:y, Float:z;
    for (new i = 0; i < gOdvozBroj[playerid]; i++)
    {
        new v = gOdvozVozila[playerid][i];
        GetVehiclePos(v, x, y, z);
        format(row, sizeof row, "%s - %s", gImenaVozila[GetVehicleModel(v) - 400], CestaZona(x, y));
        DodajStupac(list, len, row);
    }
    format(naslov, sizeof naslov, T(playerid, "Odvoz vozila na parking ($%d)"), MEHANICAR_ODVOZ);
    return PrikaziDialog(playerid, DLG_MEHANICAR_ODVOZ, DIALOG_STYLE_LIST, naslov, list, T(playerid, "Odaberi"), T(playerid, "Odustani"));
}

DLG:mehanicar_odvoz(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= gOdvozBroj[playerid])
    {
        TaksiSpustiMobitel(playerid);
        return 1;
    }
    new v = gOdvozVozila[playerid][listitem], bool:zauzeto = false;
    if (!GetVehicleModel(v) || VlasnikVozila(v) != gAccountId[playerid]) return MehanicarOdvozGreska(playerid, "Vozilo vise nije vase.");
    if (gOdvoz[v]) return MehanicarOdvozGreska(playerid, "To vozilo se vec odvozi.");
    for (new i = 0; i < MAX_PLAYERS && !zauzeto; i++) if (IsPlayerInVehicle(i, v)) zauzeto = true;
    if (zauzeto && !gPremium[playerid]) return MehanicarOdvozGreska(playerid, "Netko je u vozilu, ne mozemo ga odslepati.");
    new Float:x, Float:y, Float:z;
    GetVehiclePos(v, x, y, z);
    if (VlasnikNaParkingu(v)) return MehanicarOdvozGreska(playerid, "Vozilo je vec na parkingu.");
    if (!DajNovac(playerid, -MEHANICAR_ODVOZ)) return MehanicarOdvozGreska(playerid, "Nemate dovoljno novca.");
    new model[32], odakle[28], dokle[28];
    strcopy(model, gImenaVozila[GetVehicleModel(v) - 400]);
    odakle = CestaZona(x, y);
    if (GetVehicleVirtualWorld(v) && GarazaIzvana(VlasnikGarazaDb(v), x, y)) odakle = CestaZona(x, y);
    VlasnikParkingZona(v, dokle);
    SetTimerEx("TaksiSpustiMobitel", TAKSI_MOBITEL_MS, false, "i", playerid);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_CHAT, "%s kaze (telefon): Dobar dan, odslepajte mi %s iz %s do parkinga u %s.", GetRPIme(playerid), model, 1, odakle, dokle);
    SetTimerEx("MehanicarOdvozOdgovor", MEHANICAR_ODGOVOR_MS, false, "i", playerid);
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerInVehicle(i, v)) RemovePlayerFromVehicle(i);
    gOdvoz[v] = true;
    SetVehicleVirtualWorld(v, MEHANICAR_ODVOZ_SVIJET);
    SetVehiclePos(v, 1477.0 + float(v % 10) * 6.0, 1350.0 + float((v / 10) % 10) * 8.0, 11.0);
    SetTimerEx("MehanicarOdvozGotov", MEHANICAR_ODVOZ_MS, false, "ii", v, VlasnikDb(v));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste $%d."), MEHANICAR_ODVOZ);
    return 1;
}

forward MehanicarOdvozOdgovor(playerid);
public MehanicarOdvozOdgovor(playerid)
{
    if (IsPlayerConnected(playerid)) SendClientMessage(playerid, COLOR_CHAT, T(playerid, "Dispecer (telefon): U redu, slep je krenuo, vozilo ce biti na parkingu za par minuta."));
}

forward MehanicarOdvozGotov(v, db);
public MehanicarOdvozGotov(v, db)
{
    if (!gOdvoz[v]) return;
    gOdvoz[v] = false;
    if (!GetVehicleModel(v) || VlasnikDb(v) != db) return;
    VlasnikNaParking(v);
    MobitelMehanicarSms(VlasnikVozila(v), "Vase vozilo %s je dostavljeno na parking.", gImenaVozila[GetVehicleModel(v) - 400]);
}

MehanicarPozovi(playerid)
{
    if (!gUlogiran[playerid]) return 1;
    new v = GetPlayerVehicleID(playerid), Float:zdravlje, Float:x, Float:y, Float:z, Float:d;
    if (!v) return MehanicarOdvozPoziv(playerid);
    GetVehicleHealth(v, zdravlje);
    if (VoziloImaGoriva(v) && zdravlje >= MEHANICAR_STETA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vase vozilo ne treba mehanicara."));
    for (new s = 0; s < SLUCAJ_MAX; s++)
        if (gSlucaj[s][slStanje] && gSlucaj[s][slStanje] < SL_ODLAZI && gSlucaj[s][slTip] == ORG_MEHANICAR && gSlucaj[s][slPacijent] == playerid)
            return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mehanicar je vec na putu."));
    GetVehiclePos(v, x, y, z);
    CestaNajbliza(x, y, z, d);
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || d > TAKSI_MAX_OD_CESTE) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mehanicar dolazi samo na ulice Los Santosa."));
    if ((!KamionBroj(ORG_MEHANICAR) && !SlucajImaIgraca(ORG_MEHANICAR, playerid)) || SlucajNovi(ORG_MEHANICAR, x, y, z, playerid) == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Svi mehanicari su zauzeti, pokusajte kasnije."));
    gMehanicarVozilo[playerid] = v;
    MobitelPoziv(playerid, T(playerid, "Mehanicar"));
    SetTimerEx("TaksiSpustiMobitel", TAKSI_MOBITEL_MS, false, "i", playerid);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_CHAT, "%s kaze (telefon): Dobar dan, trebao bih mehanicara. Nalazim se u %s.", GetRPIme(playerid), CestaZona(x, y), 1);
    SetTimerEx("MehanicarOdgovor", MEHANICAR_ODGOVOR_MS, false, "i", playerid);
    return 1;
}

forward MehanicarOdgovor(playerid);
public MehanicarOdgovor(playerid)
{
    if (IsPlayerConnected(playerid)) SendClientMessage(playerid, COLOR_CHAT, T(playerid, "Dispecer (telefon): U redu, mehanicar je na putu."));
}

static MehanicarPonudi(playerid, s)
{
    if (playerid == INVALID_PLAYER_ID || !IsPlayerConnected(playerid)) return;
    new body[256];
    gMehanicar[playerid] = s + 1;
    if (gSlucaj[s][slIgrac] != INVALID_PLAYER_ID) format(body, sizeof body, T(playerid, "Napuni gorivo ($%d)\nPopravi motor ($%d)\nSlepaj vozilo u radionicu ($%d)\nZamijeni gume ($%d)"), MEHANICAR_GORIVO, MEHANICAR_POPRAVAK, MEHANICAR_SLEPANJE, MEHANICAR_GUME);
    else format(body, sizeof body, T(playerid, "Napuni gorivo ($%d)\nPopravi motor ($%d)\nZamijeni gume ($%d)"), MEHANICAR_GORIVO, MEHANICAR_POPRAVAK, MEHANICAR_GUME);
    PrikaziDialog(playerid, DLG_MEHANICAR, DIALOG_STYLE_LIST, T(playerid, "Mehanicar"), body, T(playerid, "Odaberi"), T(playerid, "Gotovo"));
}

DLG:mehanicar(playerid, response, listitem)
{
    static const cijene[] = { MEHANICAR_GORIVO, MEHANICAR_POPRAVAK, MEHANICAR_SLEPANJE, MEHANICAR_GUME };
    new s = gMehanicar[playerid] - 1, v = gMehanicarVozilo[playerid];
    if (s < 0 || gSlucaj[s][slStanje] != SL_PRISTANAK || gSlucaj[s][slPacijent] != playerid)
    {
        gMehanicar[playerid] = 0;
        return 1;
    }
    if (!response || !GetVehicleModel(v))
    {
        gMehanicar[playerid] = 0;
        SlucajKraj(s, true);
        return 1;
    }
    new igrac = gSlucaj[s][slIgrac];
    if (igrac == INVALID_PLAYER_ID && listitem >= 2) listitem++;
    if (listitem < 0 || listitem >= sizeof cijene) return 1;
    if (listitem == 3)
    {
        new VEHICLE_PANEL_STATUS:panels, VEHICLE_DOOR_STATUS:doors, VEHICLE_LIGHT_STATUS:lights, VEHICLE_TIRE_STATUS:tires;
        GetVehicleDamageStatus(v, panels, doors, lights, tires);
        if (!tires)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Gume su ispravne."));
            MehanicarPonudi(playerid, s);
            return 1;
        }
    }
    if (igrac != INVALID_PLAYER_ID ? !TransferNovac(playerid, igrac, cijene[listitem]) : !DajNovac(playerid, -cijene[listitem]))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        MehanicarPonudi(playerid, s);
        return 1;
    }
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste $%d."), cijene[listitem]);
    if (listitem == 2)
    {
        for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerInVehicle(i, v)) RemovePlayerFromVehicle(i);
        new kamion = GetPlayerVehicleID(igrac);
        if (kamion && GetVehicleModel(kamion) == MEHANICAR_MODEL) AttachTrailerToVehicle(v, kamion);
        SendClientMessage(igrac, COLOR_ORG, T(igrac, "Musterija je platila slepanje. Odslepajte vozilo do radionice."));
        gMehanicar[playerid] = 0;
        SlucajKraj(s, true);
        return 1;
    }
    MehanicarRadi(s, v, listitem);
    return 1;
}

static MehanicarHauba(v, bool:otvorena)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    SetVehicleParamsEx(v, e, l, a, d, otvorena ? 1 : 0, bt, o);
}

static MehanicarIgracStigao(s)
{
    new i = gSlucaj[s][slIgrac], p = gSlucaj[s][slPacijent], Float:x, Float:y, Float:z;
    if (i == INVALID_PLAYER_ID || p == INVALID_PLAYER_ID || GetPlayerState(i) != PLAYER_STATE_ONFOOT || !GetVehiclePos(gMehanicarVozilo[p], x, y, z) || !IsPlayerInRangeOfPoint(i, MEHANICAR_IGRAC_DOMET, x, y, z)) return;
    DisablePlayerCheckpoint(i);
    gSlucaj[s][slStanje] = SL_PRISTANAK;
    gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
    MehanicarPonudi(p, s);
}

static MehanicarRadi(s, v, posao)
{
    new igrac = gSlucaj[s][slIgrac];
    if (igrac != INVALID_PLAYER_ID)
    {
        if (posao == 1) MehanicarHauba(v, true);
        if (posao == 1) ApplyAnimation(igrac, "CAR", "Fixn_Car_Loop", 4.1, true, false, false, false, 0, SYNC_ALL);
        else if (posao == 3) ApplyAnimation(igrac, "BOMBER", "BOM_Plant_Loop", 4.1, true, false, false, false, 0, SYNC_ALL);
        else ApplyAnimation(igrac, "BAR", "Barserve_bottle", 4.1, true, false, false, false, 0, SYNC_ALL);
        gSlucaj[s][slStanje] = SL_RADI;
        gSlucaj[s][slPosao] = posao;
        gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
        SetTimerEx("MehanicarGotov", MEHANICAR_RAD_MS, false, "i", s);
        return;
    }
    new Float:x, Float:y, Float:z, Float:a, Float:sx, Float:sy, Float:sz;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    new Float:fx = -floatsin(a, degrees), Float:fy = floatcos(a, degrees), Float:px, Float:py;
    if (posao == 1)
    {
        px = x + fx * (sy / 2.0 + 0.7);
        py = y + fy * (sy / 2.0 + 0.7);
        MehanicarHauba(v, true);
    }
    else
    {
        px = x - fx * (sy / 2.0 - 0.6) + fy * (sx / 2.0 + 0.6);
        py = y - fy * (sy / 2.0 - 0.6) - fx * (sx / 2.0 + 0.6);
    }
    gSlucaj[s][slStanje] = SL_RADI;
    gSlucaj[s][slPosao] = posao;
    gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
    NPCHodaj(gSlucaj[s][slNpc], px, py, HodTlo(px, py, z), NPC_MOVE_TYPE_WALK, NPC_MOVE_SPEED_AUTO, 0.3);
}

static MehanicarAnim(s)
{
    new npc = gSlucaj[s][slNpc], p = gSlucaj[s][slPacijent], Float:x, Float:y, Float:z;
    if (p != INVALID_PLAYER_ID && GetVehiclePos(gMehanicarVozilo[p], x, y, z)) NPC_SetAngleToPos(npc, x, y, z);
    if (gSlucaj[s][slPosao] == 1)
    {
        new Float:kut;
        NPC_GetFacingAngle(npc, kut);
        NPC_SetFacingAngle(npc, kut + 180.0);
        NPC_ApplyAnimation(npc, "CAR", "Fixn_Car_Loop", 4.1, true, false, false, false, 0);
    }
    else if (gSlucaj[s][slPosao] == 3) NPC_ApplyAnimation(npc, "BOMBER", "BOM_Plant_Loop", 4.1, true, false, false, false, 0);
    else NPC_ApplyAnimation(npc, "BAR", "Barserve_bottle", 4.1, true, false, false, false, 0);
    SetTimerEx("MehanicarGotov", MEHANICAR_RAD_MS, false, "i", s);
}

forward MehanicarGotov(s);
public MehanicarGotov(s)
{
    if (gSlucaj[s][slStanje] != SL_RADI) return;
    new p = gSlucaj[s][slPacijent];
    if (gSlucaj[s][slNpc] != INVALID_PLAYER_ID) NPC_ClearAnimations(gSlucaj[s][slNpc]);
    else if (gSlucaj[s][slIgrac] != INVALID_PLAYER_ID) ClearAnimations(gSlucaj[s][slIgrac]);
    gSlucaj[s][slStanje] = SL_PRISTANAK;
    gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
    if (p == INVALID_PLAYER_ID) return;
    new v = gMehanicarVozilo[p];
    if (GetVehicleModel(v))
    {
        if (gSlucaj[s][slPosao] == 1)
        {
            SetVehicleHealth(v, 1000.0);
            MehanicarHauba(v, false);
        }
        else if (gSlucaj[s][slPosao] == 3)
        {
            new VEHICLE_PANEL_STATUS:panels, VEHICLE_DOOR_STATUS:doors, VEHICLE_LIGHT_STATUS:lights, VEHICLE_TIRE_STATUS:tires;
            GetVehicleDamageStatus(v, panels, doors, lights, tires);
            UpdateVehicleDamageStatus(v, panels, doors, lights, VEHICLE_TIRE_STATUS:0);
        }
        else VoziloNapuni(v);
    }
    MehanicarPonudi(p, s);
}

CMD:radionica(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, RADIONICA_X, RADIONICA_Y, RADIONICA_Z);
    return 1;
}

DLG:slucaj(playerid, response)
{
    new s = gSlucajPoziv[playerid] - 1;
    gSlucajPoziv[playerid] = 0;
    if (!response) return 1;
    if (s < 0 || gSlucaj[s][slStanje] != SL_POZIV) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Netko je vec prihvatio poziv."));
    SlucajOcistiPozive(s);
    gSlucaj[s][slStanje] = SL_IGRAC;
    gSlucaj[s][slIgrac] = playerid;
    SetPlayerCheckpoint(playerid, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ], 5.0);
    if (gSlucaj[s][slTip] == ORG_VATROGASCI) return SendClientMessage(playerid, COLOR_ORG, T(playerid, "Pozar je oznacen na karti. Gasi ga vatrogasnim kamionom ili aparatom (/aparat)."));
    if (gSlucaj[s][slTip] == ORG_MEHANICAR) return SendClientMessage(playerid, COLOR_ORG, T(playerid, "Vozilo u kvaru je oznaceno na karti. Kad stignete, vlasnik bira uslugu."));
    return SendClientMessage(playerid, COLOR_ORG, T(playerid, "Ranjenik je oznacen na karti. Ozivi ga s /heal %d (treba /duty)."), gSlucaj[s][slPacijent]);
}

IsRanjen(playerid)
{
    return gRanjen[playerid];
}

static SlucajPacijenta(playerid)
{
    for (new s = 0; s < SLUCAJ_MAX; s++)
        if (gSlucaj[s][slStanje] && gSlucaj[s][slStanje] < SL_ODLAZI && gSlucaj[s][slTip] == ORG_MEDICINSKA && gSlucaj[s][slPacijent] == playerid) return s;
    return -1;
}

RanjenPadni(playerid)
{
    if (!gUlogiran[playerid] || ZatvorProvjera(playerid)) return 0;
    if (gRanjen[playerid] == RN_DOLJE)
    {
        gRanjen[playerid] = RN_CEKA;
        return 1;
    }
    GetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
    GetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    gRanjenInt[playerid] = GetPlayerInterior(playerid);
    gRanjenSvijet[playerid] = GetPlayerVirtualWorld(playerid);
    gRanjen[playerid] = RN_CEKA;
    SpremiSvoOruzje(playerid);
    return 1;
}

RanjenSpawn(playerid)
{
    if (gRanjen[playerid] != RN_CEKA) return;
    new bool:prvi = SlucajPacijenta(playerid) == -1;
    gRanjen[playerid] = RN_DOLJE;
    gRanjenOd[playerid] = GetTickCount();
    gRanjenPogoci[playerid] = 0;
    SetPlayerInterior(playerid, gRanjenInt[playerid]);
    SetPlayerVirtualWorld(playerid, gRanjenSvijet[playerid]);
    new Float:hx, Float:hy, Float:hz;
    if (!gRanjenInt[playerid] && !gRanjenSvijet[playerid] && Kolizija_Zraka(gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2] + 1.0, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2] - 300.0, hx, hy, hz))
        gRanjenPos[playerid][2] = hz + 1.0;
    SetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
    SetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    SetPlayerHealth(playerid, FLOAT_INFINITY);
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "CRACK", "crckidle1", 4.1, true, false, false, true, 0, SYNC_ALL);
    gRanjenAnimId[playerid] = -2;
    SetTimerEx("RanjenAnim", 200, false, "ii", playerid, gAccountId[playerid]);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Ranjeni ste. Za 10 s mozete /odustani ili cekajte hitnu."));
    if (prvi && !gRanjenInt[playerid] && !gRanjenSvijet[playerid]) SlucajNovi(ORG_MEDICINSKA, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2], playerid);
}

RanjenProvjeri(playerid, Float:zivot)
{
    if (zivot > RANJEN_PRAG) return false;
    RanjenOdmah(playerid);
    return true;
}

RanjenOdmah(playerid)
{
    TaksiPlayerLeft(playerid);
    KurvaDisconnect(playerid);
    MaricaReset(playerid);
    PolicijaDisconnect(playerid, false);
    RanjenPadni(playerid);
    TeritorijDeath(playerid);
    if (gRanjen[playerid] != RN_CEKA || !IsPlayerInAnyVehicle(playerid))
    {
        RanjenSpawn(playerid);
        return;
    }
    SetPlayerHealth(playerid, FLOAT_INFINITY);
    RemovePlayerFromVehicle(playerid);
    SetTimerEx("RanjenIzVozila", RANJEN_IZLAZ_MS, false, "ii", playerid, gAccountId[playerid]);
}

forward RanjenIzVozila(playerid, racun);
public RanjenIzVozila(playerid, racun)
{
    if (gAccountId[playerid] != racun || gRanjen[playerid] != RN_CEKA) return;
    GetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
    GetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    RanjenSpawn(playerid);
}

forward RanjenAnim(playerid, racun);
public RanjenAnim(playerid, racun)
{
    if (gAccountId[playerid] != racun || gRanjen[playerid] != RN_DOLJE) return;
    ApplyAnimation(playerid, "CRACK", "crckidle1", 4.1, true, false, false, true, 0);
    TogglePlayerControllable(playerid, true);
    gRanjenAnimId[playerid] = -1;
    gRanjenPopravak[playerid] = GetTickCount();
}

RanjenUpdate(playerid)
{
    if (gRanjen[playerid] != RN_DOLJE || gRanjenAnimId[playerid] == -2) return;
    new idx = GetPlayerAnimationIndex(playerid);
    if (idx != gRanjenAnimId[playerid])
    {
        new lib[32], ime[32];
        GetAnimationName(idx, lib, sizeof lib, ime, sizeof ime);
        if (!strcmp(ime, "crckidle1", true)) gRanjenAnimId[playerid] = idx;
    }
    if (idx == gRanjenAnimId[playerid] && GetPlayerDistanceFromPoint(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]) <= RANJEN_POMAK) return;
    new sad = GetTickCount();
    if (sad - gRanjenPopravak[playerid] < RANJEN_POPRAVAK_MS) return;
    gRanjenPopravak[playerid] = sad;
    SetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
    SetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    ApplyAnimation(playerid, "CRACK", "crckidle1", 4.1, true, false, false, true, 0);
}

static RanjenCijena(playerid)
{
    return gNovac[playerid] < RANJEN_CIJENA ? gNovac[playerid] : RANJEN_CIJENA;
}

RanjenPonudi(playerid, medic)
{
    new body[128];
    gRanjenPonuda[playerid] = medic == RANJEN_NPC ? RANJEN_NPC : medic + 1;
    format(body, sizeof body, T(playerid, "Bolnicar vam zeli pomoci. Cijena: $%d. Prihvacate li?"), RanjenCijena(playerid));
    PrikaziDialog(playerid, DLG_RANJEN, DIALOG_STYLE_MSGBOX, T(playerid, "Hitna pomoc"), body, T(playerid, "Prihvati"), T(playerid, "Odbij"));
}

DLG:ranjen(playerid, response)
{
    new n = gRanjenPonuda[playerid], s = SlucajPacijenta(playerid);
    gRanjenPonuda[playerid] = 0;
    if (!n || gRanjen[playerid] != RN_DOLJE) return 1;
    new bool:npcCeka = s != -1 && gSlucaj[s][slStanje] == SL_PRISTANAK;
    if (n > 0)
    {
        new medic = n - 1, bool:tu = IsPlayerConnected(medic) && gUlogiran[medic];
        if (response && tu && GetPlayerDistanceFromPoint(medic, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]) <= RANJEN_MEDIK_DOMET)
        {
            PosaljiBlizu(medic, CHAT_RANGE, COLOR_ME, "* %s ozivljava %s.", GetRPIme(medic), GetRPIme(playerid), 1);
            RanjenZavrsi(playerid, true, medic);
            return 1;
        }
        if (!response && tu) SendClientMessage(medic, COLOR_ERROR, T(medic, "Ranjenik je odbio pomoc."));
        if (npcCeka) RanjenPonudi(playerid, RANJEN_NPC);
        return 1;
    }
    if (!npcCeka) return 1;
    if (!response)
    {
        SlucajZatvori(s);
        return 1;
    }
    gSlucaj[s][slStanje] = SL_GASI;
    NPC_ApplyAnimation(gSlucaj[s][slNpc], "BOMBER", "BOM_Plant_Loop", 4.1, true, false, false, false, 0);
    return 1;
}

RanjenPrimijeni(playerid, bool:spasen)
{
    gRanjen[playerid] = RN_NE;
    gRanjenPonuda[playerid] = 0;
    SetPlayerHealth(playerid, 100.0);
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid);
    if (spasen)
    {
        new cijena = RanjenCijena(playerid);
        if (cijena) DajNovac(playerid, -cijena);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Spaseni ste. Placate $%d."), cijena);
        return;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT oruzje_dozvola FROM accounts WHERE id = %d", gAccountId[playerid]), bool:dozvola = DB_GetRowCount(res) && DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    SpremiSvoOruzje(playerid);
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gInv[playerid][i]) continue;
        new bool:droga = i == ITEM_MARIHUANA || i == ITEM_KOKAIN || i == ITEM_SJEME, bool:oruzje = gItems[i][ITEM_WEAPON] > 0 || (ITEM_MUN_PISTOLJ <= i <= ITEM_MUN_PUSKA);
        if (droga || (oruzje && !dozvola)) GiveItem(playerid, i, -gInv[playerid][i]);
    }
    SendClientMessage(playerid, COLOR_INFO, dozvola ? T(playerid, "Izgubili ste drogu.") : T(playerid, "Izgubili ste drogu i oruzje (nemate dozvolu za oruzje)."));
    gRanjenBolnica[playerid] = true;
    SpawnPlayer(playerid);
}

RanjenBolnica(playerid)
{
    if (!gRanjenBolnica[playerid]) return;
    gRanjenBolnica[playerid] = false;
    new b = VectorSize(gRanjenPos[playerid][0] - gBolnice[0][0], gRanjenPos[playerid][1] - gBolnice[0][1], 0.0) <= VectorSize(gRanjenPos[playerid][0] - gBolnice[1][0], gRanjenPos[playerid][1] - gBolnice[1][1], 0.0) ? 0 : 1;
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gBolnice[b][0], gBolnice[b][1], gBolnice[b][2]);
    SetPlayerFacingAngle(playerid, gBolnice[b][3]);
    SetCameraBehindPlayer(playerid);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Probudili ste se u bolnici."));
    if (!gPremium[playerid]) return;
    new body[128];
    format(body, sizeof body, T(playerid, "Ostani u bolnici\nVrati me na moj spawn ($%d)"), BOLNICA_SPAWN_CIJENA);
    PrikaziDialog(playerid, DLG_BOLNICA, DIALOG_STYLE_LIST, T(playerid, "Bolnica"), body, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DLG:bolnica(playerid, response, listitem)
{
    if (!response || listitem != 1 || !gPremium[playerid] || IsRanjen(playerid)) return 1;
    if (!DajNovac(playerid, -BOLNICA_SPAWN_CIJENA)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, SPAWN_X, SPAWN_Y, SPAWN_Z);
    SetPlayerFacingAngle(playerid, SPAWN_A);
    SetCameraBehindPlayer(playerid);
    KucaSpawn(playerid);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste $%d."), BOLNICA_SPAWN_CIJENA);
}

RanjenZavrsi(playerid, bool:spasen, ko = INVALID_PLAYER_ID)
{
    new s = SlucajPacijenta(playerid);
    if (s != -1) SlucajKraj(s, spasen, ko);
    else RanjenPrimijeni(playerid, spasen);
}

RanjenPogodak(playerid, BULLET_HIT_TYPE:hittype, hitid)
{
    if (hittype != BULLET_HIT_TYPE_PLAYER || playerid == hitid || gRanjen[hitid] != RN_DOLJE || ++gRanjenPogoci[hitid] < RANJEN_POGODAKA) return;
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s dokrajcuje %s.", GetRPIme(playerid), GetRPIme(hitid), 1);
    RanjenZavrsi(hitid, false);
}

CMD:odustani(playerid, params[])
{
    if (gRanjen[playerid] != RN_DOLJE) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste ranjeni."));
    new ms = RANJEN_CEKANJE_MS - (GetTickCount() - gRanjenOd[playerid]);
    if (ms > 0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mozete odustati za %d s."), ms / 1000 + 1);
    RanjenZavrsi(playerid, false);
    return 1;
}

CMD:giveup(playerid, params[]) return cmd_odustani(playerid, params);
