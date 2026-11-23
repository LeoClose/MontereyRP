#define STAND_MAX            32
#define STAND_CIJENA         5000
#define STAND_KOMBI          3
#define STAND_KOMBI_CIJENA   15000
#define STAND_KOMBI_MODEL    588
#define STAND_KOMBI_RADNIK   1.8
#define STAND_KOMBI_KUPAC    3.4
#define STAND_KOMBI_DOMET    4.5
#define STAND_KOMBI_UNUTRA   0.5
#define STAND_KOMBI_VISINA   0.4
#define STAND_KOMBI_IZLAZ    4.0
#define STAND_CP_POMAK       5.0
#define STAND_PREUZMI_DOMET  6.0
#define STAND_KUTIJA_SLOT    2
#define STAND_KUTIJA_MODEL   1220
#define STAND_ZALIHA_CIJENA  1
#define STAND_ZALIHA_MAX     300
#define STAND_RADNIK_SAT     60
#define STAND_RADNIK_SKIN    168
#define STAND_DOSTAVA        6
#define STAND_DOMET          3.0
#define STAND_RADI_DOMET     4.0
#define STAND_RADNIK_RAZMAK  1.7
#define KUPAC_BLIZU_MIN      15.0
#define KUPAC_BLIZU_MAX      45.0
#define KUPAC_NA_OTVARANJE   2
#define STAND_IGRA_MS        12000
#define STAND_HRANA_SLOT     4
#define STAND_JEDENJE_MS     4500
#define STAND_RADNIK_MS      3000
#define STAND_BOJA           0xE67E22FF
#define STAND_SMS_BROJ       "558"
#define KUPAC_MAX            40
#define KUPAC_PO_STANDU      6
#define KUPAC_SPAWN_MIN      60.0
#define KUPAC_SPAWN_MAX      220.0
#define KUPAC_SKRIVENO       40.0
#define KUPAC_ZAGLAVIO_S     150
#define KUPAC_CEKA_S         60
#define KUPAC_SPAWN_S        2
#define KUPAC_RANJEN_HP      25.0
#define KUPAC_NOVAC_MIN      1
#define KUPAC_NOVAC_MAX      8
#define KUPAC_NOVAC_PAUZA_S  900
#define STAND_RADNIK_GRESKA  20
#define KUPAC_RANJEN_S       240
#define STAND_VIDLJIV        250.0
#define STAND_VIRT_S         18
#define KUPAC_DOLAZAK        3.0
#define KUPAC_KRENI_MS       700

static const gStandModeli[] = { 1340, 1341, 1342, STAND_KOMBI_MODEL };
static const gStandVrsta[][] = { "Hot dog", "Sladoled", "Rezanci", "Kombi s hranom" };
static const gStandMeni[][][] = {
    { "Hot dog", "Chili dog", "Cola" },
    { "Sladoled od vanilije", "Cokoladni sladoled", "Sladoled od jagode" },
    { "Rezanci s piletinom", "Rezanci s govedinom", "Proljetna rolica" },
    { "Hot dog", "Hamburger", "Cola" }
};
static const gStandCijene[][3] = { { 4, 5, 2 }, { 3, 3, 3 }, { 8, 8, 3 }, { 5, 7, 2 } };
static const gStandHranaModel[][3] = { { 19346, 19346, 2647 }, { 19568, 19568, 19568 }, { 19811, 19811, 19811 }, { 19346, 2703, 2647 } };
static const Float:gStandZdravlje[][3] = { { 15.0, 18.0, 5.0 }, { 8.0, 8.0, 8.0 }, { 25.0, 25.0, 10.0 }, { 15.0, 22.0, 5.0 } };
static gStandJede[MAX_PLAYERS], gStandIgraKupac[MAX_PLAYERS];
enum { SN_NEMA, SN_TRGOVINA, SN_NOSI, SN_GEPEK }
static gStandNar[MAX_PLAYERS], gStandNarS[MAX_PLAYERS], gStandNarKolicina[MAX_PLAYERS], gStandNarTrgovina[MAX_PLAYERS], gStandNarV[MAX_PLAYERS], Float:gStandNarCp[MAX_PLAYERS][3];

enum E_STAND { bool:snIma, snDb, snVrsta, Float:snX, Float:snY, Float:snZ, Float:snRz, Float:snKx, Float:snKy, Float:snKz, snVlasnik, snVlasnikIme[MAX_PLAYER_NAME + 1], snIme[25],
    snZaliha, snKasa, bool:snRadnik, snRadnikDo, snRadnikPauzaOd, bool:snRadnikCeka, snAktor, STREAMER_TAG_OBJECT:snObjekt, STREAMER_TAG_3D_TEXT_LABEL:snTekst, snKupac, snKupacOd, snVozilo, bool:snParkiran }
static gStand[STAND_MAX][E_STAND];
static gStandRadi[MAX_PLAYERS], gStandMeniS[MAX_PLAYERS], gStandOpcija[MAX_PLAYERS][8], gStandOpcijaN[MAX_PLAYERS], gStandUredi[MAX_PLAYERS][2], bool:gStandBio[STAND_MAX], bool:gStandGledan[STAND_MAX], gApStand[MAX_PLAYERS], gApStandLista[MAX_PLAYERS][STAND_MAX];

enum { KU_NEMA, KU_SPAWN, KU_HODA, KU_CEKA, KU_SLUZI, KU_REAGIRA, KU_RANJEN }
enum E_KUPAC { kuStanje, kuNpc, kuCilj, kuPosjeta, kuOd, kuNovacDo, Float:kuTx, Float:kuTy, Float:kuTz, kuNapadac, kuNapadacRacun, kuStanjeOd }
static gKupac[KUPAC_MAX][E_KUPAC], gKupacZadnji;

enum { SO_IME, SO_KASA, SO_ROBA, SO_RADNIK, SO_RADI }

InitStandovi()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS food_stands (id INTEGER PRIMARY KEY AUTOINCREMENT, kind INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, rz REAL NOT NULL, kx REAL NOT NULL, ky REAL NOT NULL, kz REAL NOT NULL, owner INTEGER NOT NULL DEFAULT 0, owner_name TEXT NOT NULL DEFAULT '', name TEXT NOT NULL DEFAULT '', stock INTEGER NOT NULL DEFAULT 0, till INTEGER NOT NULL DEFAULT 0, worker INTEGER NOT NULL DEFAULT 0, worker_until INTEGER NOT NULL DEFAULT 0)"));
    for (new k = 0; k < KUPAC_MAX; k++) gKupac[k][kuNpc] = INVALID_LIK;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, kind, x, y, z, rz, kx, ky, kz, owner, owner_name, name, stock, till, worker, worker_until FROM food_stands LIMIT %d", STAND_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gStand[s][snDb] = DB_GetFieldInt(res, 0);
        gStand[s][snVrsta] = DB_GetFieldInt(res, 1);
        gStand[s][snX] = DB_GetFieldFloat(res, 2);
        gStand[s][snY] = DB_GetFieldFloat(res, 3);
        gStand[s][snZ] = DB_GetFieldFloat(res, 4);
        gStand[s][snRz] = DB_GetFieldFloat(res, 5);
        gStand[s][snKx] = DB_GetFieldFloat(res, 6);
        gStand[s][snKy] = DB_GetFieldFloat(res, 7);
        gStand[s][snKz] = DB_GetFieldFloat(res, 8);
        gStand[s][snVlasnik] = DB_GetFieldInt(res, 9);
        DB_GetFieldString(res, 10, gStand[s][snVlasnikIme], MAX_PLAYER_NAME + 1);
        DB_GetFieldString(res, 11, gStand[s][snIme], 25);
        gStand[s][snZaliha] = DB_GetFieldInt(res, 12);
        gStand[s][snKasa] = DB_GetFieldInt(res, 13);
        gStand[s][snRadnik] = DB_GetFieldInt(res, 14) != 0;
        gStand[s][snRadnikDo] = DB_GetFieldInt(res, 15);
        StandStvori(s);
    }
    DB_FreeResultSet(res);
}

static StandSpremi(s)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE food_stands SET x = %f, y = %f, z = %f, rz = %f, owner = %d, owner_name = '%q', name = '%q', stock = %d, till = %d, worker = %d, worker_until = %d WHERE id = %d",
        gStand[s][snX], gStand[s][snY], gStand[s][snZ], gStand[s][snRz], gStand[s][snVlasnik], gStand[s][snVlasnikIme], gStand[s][snIme], gStand[s][snZaliha], gStand[s][snKasa], gStand[s][snRadnik] ? 1 : 0, gStand[s][snRadnikDo], gStand[s][snDb]));
}

static StandImeIli(s, ime[], len)
{
    ime[0] = EOS;
    if (gStand[s][snIme][0]) strcat(ime, gStand[s][snIme], len);
    else strcat(ime, gStandVrsta[gStand[s][snVrsta]], len);
}

static StandOsvjeziTekst(s)
{
    new tekst[160], ime[25], vlasnik[MAX_PLAYER_NAME + 1];
    StandImeIli(s, ime, sizeof ime);
    if (gStand[s][snVlasnik])
    {
        strcopy(vlasnik, gStand[s][snVlasnikIme]);
        for (new i = 0; vlasnik[i]; i++) if (vlasnik[i] == '_') vlasnik[i] = ' ';
        format(tekst, sizeof tekst, "%s\n{FFFFFF}%s", ime, vlasnik);
        if (gStand[s][snZaliha] <= 0) strcat(tekst, "\n{E74C3C}Zatvoreno - nema robe");
    }
    else format(tekst, sizeof tekst, "%s\n{FFFFFF}$%d", ime, StandCijenaVrste(s));
    UpdateDynamic3DTextLabelText(gStand[s][snTekst], STAND_BOJA, tekst);
}

static StandCijenaVrste(s)
{
    return gStand[s][snVrsta] == STAND_KOMBI ? STAND_KOMBI_CIJENA : STAND_CIJENA;
}

static StandRadnikMjesto(s, &Float:x, &Float:y, &Float:a)
{
    new Float:dx = gStand[s][snX] - gStand[s][snKx], Float:dy = gStand[s][snY] - gStand[s][snKy], Float:d = VectorSize(dx, dy, 0.0);
    if (d < 0.1) d = 1.0;
    if (gStand[s][snVrsta] == STAND_KOMBI)
    {
        x = gStand[s][snX] - dx / d * STAND_KOMBI_UNUTRA;
        y = gStand[s][snY] - dy / d * STAND_KOMBI_UNUTRA;
    }
    else
    {
        x = gStand[s][snX] + dx / d * STAND_RADNIK_RAZMAK;
        y = gStand[s][snY] + dy / d * STAND_RADNIK_RAZMAK;
    }
    a = atan2(-dy, -dx) - 90.0;
}

static Float:StandRadnikZ(s)
{
    return gStand[s][snVrsta] == STAND_KOMBI ? gStand[s][snZ] + STAND_KOMBI_VISINA : gStand[s][snKz];
}

static StandNarCilj(s, &Float:x, &Float:y, &Float:z)
{
    if (gStand[s][snVrsta] == STAND_KOMBI && !gStand[s][snParkiran])
    {
        GetVehiclePos(gStand[s][snVozilo], x, y, z);
        return;
    }
    x = gStand[s][snKx];
    y = gStand[s][snKy];
    z = gStand[s][snKz];
}

static StandNarCp(playerid, bool:uvijek)
{
    new Float:x, Float:y, Float:z;
    StandNarCilj(gStandNarS[playerid], x, y, z);
    if (!uvijek && VectorSize(x - gStandNarCp[playerid][0], y - gStandNarCp[playerid][1], 0.0) < STAND_CP_POMAK) return;
    gStandNarCp[playerid][0] = x;
    gStandNarCp[playerid][1] = y;
    gStandNarCp[playerid][2] = z;
    SetPlayerCheckpoint(playerid, x, y, z, 3.0);
}

static StandKutijaUzmi(playerid)
{
    SetPlayerAttachedObject(playerid, STAND_KUTIJA_SLOT, STAND_KUTIJA_MODEL, 5, 0.1, 0.1, 0.15, 0.0, 90.0, 90.0, 0.6, 0.6, 0.6);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
}

StandKutijaGepek(playerid, v, bool:stavi)
{
    if (stavi)
    {
        RemovePlayerAttachedObject(playerid, STAND_KUTIJA_SLOT);
        if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        if (gStandNar[playerid] == SN_NOSI)
        {
            gStandNar[playerid] = SN_GEPEK;
            gStandNarV[playerid] = v;
        }
        return;
    }
    if (gStandNar[playerid] == SN_GEPEK || gStandNar[playerid] == SN_NOSI) gStandNar[playerid] = SN_NOSI;
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) StandKutijaUzmi(playerid);
}

StandGepekAlt(playerid, v)
{
    if (gInv[playerid][ITEM_KUTIJA] && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY && ImaKljuc(playerid, v))
    {
        if (GepekPrebaci(playerid, v, ITEM_KUTIJA, 1, true) != GEPEK_OK) return 0;
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s stavlja kutiju u prtljaznik.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (gStandNar[playerid] == SN_GEPEK && gStandNarV[playerid] == v && GepekImaItem(v, ITEM_KUTIJA))
    {
        if (GepekPrebaci(playerid, v, ITEM_KUTIJA, 1, false) != GEPEK_OK) return 0;
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s vadi kutiju iz prtljaznika.", GetRPIme(playerid), "", 1);
        return 1;
    }
    return 0;
}

static StandKombiPozicija(s)
{
    new v = gStand[s][snVozilo], Float:a;
    GetVehiclePos(v, gStand[s][snX], gStand[s][snY], gStand[s][snZ]);
    GetVehicleZAngle(v, a);
    gStand[s][snRz] = a;
    gStand[s][snKx] = gStand[s][snX] + STAND_KOMBI_KUPAC * floatcos(a, degrees);
    gStand[s][snKy] = gStand[s][snY] + STAND_KOMBI_KUPAC * floatsin(a, degrees);
    gStand[s][snKz] = HodTlo(gStand[s][snKx], gStand[s][snKy], gStand[s][snZ]);
}

static bool:StandKombiStoji(s)
{
    new v = gStand[s][snVozilo], Float:vx, Float:vy, Float:vz;
    if (GetVehicleModel(v) != STAND_KOMBI_MODEL || GetVehicleVirtualWorld(v) || VoziloGet(v, VP_MOTOR) == 1) return false;
    GetVehicleVelocity(v, vx, vy, vz);
    return vx * vx + vy * vy + vz * vz < 0.0001;
}

StandKombiVlasnik(v)
{
    for (new s = 0; s < STAND_MAX; s++) if (gStand[s][snIma] && gStand[s][snVrsta] == STAND_KOMBI && gStand[s][snVozilo] == v) return gStand[s][snVlasnik];
    return -1;
}

StandKombiSpawn(v)
{
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (!gStand[s][snIma] || gStand[s][snVrsta] != STAND_KOMBI || gStand[s][snVozilo] != v) continue;
        SetVehiclePos(v, gStand[s][snX], gStand[s][snY], gStand[s][snZ]);
        SetVehicleZAngle(v, gStand[s][snRz]);
        return;
    }
}

static StandAktor(s)
{
    if (gStand[s][snAktor] != INVALID_ACTOR_ID)
    {
        DestroyActor(gStand[s][snAktor]);
        gStand[s][snAktor] = INVALID_ACTOR_ID;
    }
    if (!gStand[s][snRadnik] || gStand[s][snRadnikCeka] || gStand[s][snRadnikPauzaOd] || (gStand[s][snVrsta] == STAND_KOMBI && !gStand[s][snParkiran])) return;
    new Float:x, Float:y, Float:a;
    StandRadnikMjesto(s, x, y, a);
    gStand[s][snAktor] = CreateActor(STAND_RADNIK_SKIN, x, y, StandRadnikZ(s), a);
    SetActorInvulnerable(gStand[s][snAktor], true);
}

static StandStvori(s)
{
    gStand[s][snIma] = true;
    gStand[s][snAktor] = INVALID_ACTOR_ID;
    gStand[s][snKupac] = -1;
    if (gStand[s][snVrsta] == STAND_KOMBI)
    {
        gStand[s][snVozilo] = CreateVehicle(STAND_KOMBI_MODEL, gStand[s][snX], gStand[s][snY], gStand[s][snZ], gStand[s][snRz], 1, 1, -1);
        gStand[s][snParkiran] = true;
        StandKombiPozicija(s);
        gStand[s][snTekst] = CreateDynamic3DTextLabel(" ", STAND_BOJA, 0.0, 0.0, 1.8, 12.0, INVALID_PLAYER_ID, gStand[s][snVozilo], 0, 0, 0);
    }
    else
    {
        gStand[s][snObjekt] = CreateDynamicObject(gStandModeli[gStand[s][snVrsta]], gStand[s][snX], gStand[s][snY], gStand[s][snZ], 0.0, 0.0, gStand[s][snRz]);
        gStand[s][snTekst] = CreateDynamic3DTextLabel(" ", STAND_BOJA, gStand[s][snX], gStand[s][snY], gStand[s][snZ] + 1.6, 12.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
        InterakcijaOznaka(gStand[s][snTekst], 3.0);
    }
    StandOsvjeziTekst(s);
    StandAktor(s);
}

static StandMakni(s)
{
    for (new k = 0; k < KUPAC_MAX; k++) if (gKupac[k][kuStanje] != KU_NEMA && gKupac[k][kuCilj] == s) KupacMakni(k);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gStandRadi[i] == s + 1) gStandRadi[i] = 0;
    if (gStand[s][snVrsta] == STAND_KOMBI) DestroyVehicle(gStand[s][snVozilo]);
    else DestroyDynamicObject(gStand[s][snObjekt]);
    DestroyDynamic3DTextLabel(gStand[s][snTekst]);
    if (gStand[s][snAktor] != INVALID_ACTOR_ID) DestroyActor(gStand[s][snAktor]);
    gStand[s][snIma] = false;
}

static bool:StandRadnikRadi(s)
{
    return gStand[s][snRadnik] && !gStand[s][snRadnikCeka] && !gStand[s][snRadnikPauzaOd];
}

static StandIgracRadi(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gStandRadi[i] == s + 1) return i;
    return INVALID_PLAYER_ID;
}

static bool:StandOtvoren(s)
{
    return gStand[s][snIma] && gStand[s][snVlasnik] && gStand[s][snZaliha] > 0 && (gStand[s][snVrsta] != STAND_KOMBI || gStand[s][snParkiran]) && (StandRadnikRadi(s) || StandIgracRadi(s) != INVALID_PLAYER_ID);
}

StandBlizu(playerid)
{
    new Float:x, Float:y, Float:z;
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (!gStand[s][snIma]) continue;
        if (gStand[s][snVrsta] == STAND_KOMBI)
        {
            GetVehiclePos(gStand[s][snVozilo], x, y, z);
            if (IsPlayerInRangeOfPoint(playerid, STAND_KOMBI_DOMET, x, y, z) || (gStand[s][snParkiran] && IsPlayerInRangeOfPoint(playerid, STAND_DOMET, gStand[s][snKx], gStand[s][snKy], gStand[s][snKz]))) return s;
        }
        else if (IsPlayerInRangeOfPoint(playerid, STAND_DOMET, gStand[s][snKx], gStand[s][snKy], gStand[s][snKz]) || IsPlayerInRangeOfPoint(playerid, STAND_DOMET, gStand[s][snX], gStand[s][snY], gStand[s][snZ])) return s;
    }
    return -1;
}

static StandSms(racun, const tekst[], const ime[], iznos = 0)
{
    new poruka[144], i = IgracRacuna(racun);
    new bool:dva = BrojSpecifikatora(tekst) >= 2;
    if (i != INVALID_PLAYER_ID)
    {
        if (dva) format(poruka, sizeof poruka, T(i, tekst), ime, iznos);
        else format(poruka, sizeof poruka, T(i, tekst), ime);
    }
    else if (dva) format(poruka, sizeof poruka, tekst, ime, iznos);
    else format(poruka, sizeof poruka, tekst, ime);
    MobitelPrimiPoruku(racun, STAND_SMS_BROJ, poruka);
}

static StandPlaca(s)
{
    if (!gStand[s][snRadnik] || !gStand[s][snVlasnik] || gStand[s][snRadnikPauzaOd] || gettime() < gStand[s][snRadnikDo]) return;
    new ime[25];
    StandImeIli(s, ime, sizeof ime);
    if (BankaUplati(gStand[s][snVlasnik], -STAND_RADNIK_SAT, "Placa radnika"))
    {
        gStand[s][snRadnikDo] = gettime() + 3600;
        if (gStand[s][snRadnikCeka])
        {
            gStand[s][snRadnikCeka] = false;
            StandAktor(s);
            StandSms(gStand[s][snVlasnik], "Radnik (%s): Hvala na placi, vracam se na posao.", ime);
        }
    }
    else if (!gStand[s][snRadnikCeka])
    {
        gStand[s][snRadnikCeka] = true;
        StandAktor(s);
        StandSms(gStand[s][snVlasnik], "Radnik (%s): Ne radim dok mi ne platite. Na vasem racunu nema $%d za placu.", ime, STAND_RADNIK_SAT);
    }
    StandSpremi(s);
}

static bool:StandRadnikPauza(s, bool:stanka)
{
    if (!gStand[s][snRadnik] || gStand[s][snRadnikCeka])
        return false;

    if (stanka)
    {
        if (gStand[s][snRadnikPauzaOd])
            return false;

        gStand[s][snRadnikPauzaOd] = gettime();
    }
    else
    {
        if (!gStand[s][snRadnikPauzaOd])
            return false;

        gStand[s][snRadnikDo] +=
            gettime() - gStand[s][snRadnikPauzaOd];

        gStand[s][snRadnikPauzaOd] = 0;
    }

    StandAktor(s);
    StandSpremi(s);
    return true;
}

StandMeni(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPlayerVirtualWorld(playerid) || GetPlayerInterior(playerid)) return 0;
    new s = StandBlizu(playerid);
    if (s == -1) return 0;
    gStandMeniS[playerid] = s;
    if (gStandNar[playerid] == SN_NOSI && gStandNarS[playerid] == s)
    {
        StandPredaj(playerid);
        return 1;
    }
    if (!gStand[s][snVlasnik])
    {
        new body[160];
        if (gStand[s][snVrsta] == STAND_KOMBI) format(body, sizeof body, T(playerid, "Kupiti kombi s hranom za $%d?"), STAND_KOMBI_CIJENA);
        else format(body, sizeof body, T(playerid, "Kupiti %s stand za $%d?"), T(playerid, gStandVrsta[gStand[s][snVrsta]]), STAND_CIJENA);
        PrikaziDialog(playerid, DLG_STAND_KUPI, DIALOG_STYLE_MSGBOX, T(playerid, "Stand s hranom"), body, T(playerid, "Kupi"), T(playerid, "Zatvori"));
        return 1;
    }
    if (gStand[s][snVlasnik] != gAccountId[playerid])
    {
        if (!StandOtvoren(s))
        {
            ObavijestGreska(playerid, T(playerid, "Stand je trenutno zatvoren."));
            return 1;
        }
        if (gStand[s][snAktor] != INVALID_ACTOR_ID) AktorKaze(playerid, gStand[s][snAktor], "Prodavac", "Dobar dan! Izvolite, sve je svjeze.");
        new lista[256], stavka[64], duljina = 0, v = gStand[s][snVrsta], naslov[25];
        for (new i = 0; i < 3; i++)
        {
            format(stavka, sizeof stavka, "%s - $%d", T(playerid, gStandMeni[v][i]), gStandCijene[v][i]);
            DodajStupac(lista, duljina, stavka);
        }
        StandImeIli(s, naslov, sizeof naslov);
        PrikaziDialog(playerid, DLG_STAND_HRANA, DIALOG_STYLE_LIST, naslov, lista, T(playerid, "Kupi"), T(playerid, "Zatvori"));
        return 1;
    }
    new list[512], red[96], len = 0, n = 0, ime[25];
    StandImeIli(s, ime, sizeof ime);
    gStandOpcija[playerid][n++] = SO_IME;
    DodajStupac(list, len, T(playerid, "Promijeni ime"));
    gStandOpcija[playerid][n++] = SO_KASA;
    format(red, sizeof red, T(playerid, "Uzmi zaradu ($%d)"), gStand[s][snKasa]);
    DodajStupac(list, len, red);
    gStandOpcija[playerid][n++] = SO_ROBA;
    if (gStandNar[playerid] != SN_NEMA) format(red, sizeof red, T(playerid, "Otkazi narudzbu robe"));
    else format(red, sizeof red, T(playerid, "Naruci robu (zaliha %d/%d, $%d po komadu)"), gStand[s][snZaliha], STAND_ZALIHA_MAX, STAND_ZALIHA_CIJENA);
    DodajStupac(list, len, red);
    gStandOpcija[playerid][n++] = SO_RADNIK;
    if (gStand[s][snRadnik]) format(red, sizeof red, gStand[s][snRadnikCeka] ? T(playerid, "Otpusti radnika (ceka placu)") : T(playerid, "Otpusti radnika"));
    else format(red, sizeof red, T(playerid, "Zaposli radnika ($%d/h iz banke)"), STAND_RADNIK_SAT);
    DodajStupac(list, len, red);
    gStandOpcija[playerid][n++] = SO_RADI;
    DodajStupac(list, len, gStandRadi[playerid] == s + 1 ? T(playerid, "Prestani raditi") : T(playerid, "Radi na standu"));
    gStandOpcijaN[playerid] = n;
    PrikaziDialog(playerid, DLG_STAND, DIALOG_STYLE_LIST, ime, list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
    return 1;
}

static StandMoj(playerid)
{
    new s = gStandMeniS[playerid];
    return s >= 0 && s < STAND_MAX && gStand[s][snIma] && gStand[s][snVlasnik] == gAccountId[playerid] && StandBlizu(playerid) == s ? s : -1;
}

DLG:stand_kupi(playerid, response)
{
    new s = gStandMeniS[playerid];
    if (!response || s < 0 || !gStand[s][snIma] || gStand[s][snVlasnik] || StandBlizu(playerid) != s) return 1;
    new bool:kombi = gStand[s][snVrsta] == STAND_KOMBI;
    for (new k = 0; k < STAND_MAX; k++)
        if (gStand[k][snIma] && gStand[k][snVlasnik] == gAccountId[playerid] && (gStand[k][snVrsta] == STAND_KOMBI) == kombi)
            return ObavijestGreska(playerid, kombi ? T(playerid, "Vec imate kombi s hranom.") : T(playerid, "Vec imate stand s hranom."));
    if (!DajNovac(playerid, -StandCijenaVrste(s))) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
    gStand[s][snVlasnik] = gAccountId[playerid];
    GetPlayerName(playerid, gStand[s][snVlasnikIme], MAX_PLAYER_NAME + 1);
    gStand[s][snIme][0] = EOS;
    gStand[s][snKasa] = 0;
    StandSpremi(s);
    StandOsvjeziTekst(s);
    if (kombi) return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Kupili ste kombi. Prodaja radi kad je parkiran s ugasenim motorom. Narucite robu i zaposlite radnika ili radite sami (ALT kod kombija)."));
    return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Kupili ste stand. Narucite robu i zaposlite radnika ili radite sami (ALT kod standa)."));
}

static StandNarudzbaKraj(playerid)
{
    if (gStandNar[playerid] == SN_NEMA) return;
    if (gStandNar[playerid] == SN_GEPEK && GepekImaItem(gStandNarV[playerid], ITEM_KUTIJA)) GepekMakniItem(gStandNarV[playerid], ITEM_KUTIJA);
    if (gInv[playerid][ITEM_KUTIJA]) GiveItem(playerid, ITEM_KUTIJA, -gInv[playerid][ITEM_KUTIJA]);
    if (gStandNar[playerid] == SN_NOSI && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY)
    {
        RemovePlayerAttachedObject(playerid, STAND_KUTIJA_SLOT);
        if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    }
    gStandNar[playerid] = SN_NEMA;
    DisablePlayerCheckpoint(playerid);
}

static StandPredaj(playerid)
{
    new s = gStandNarS[playerid], n = gStandNarKolicina[playerid], tekst[96];
    StandNarudzbaKraj(playerid);
    if (!gStand[s][snIma] || gStand[s][snVlasnik] != gAccountId[playerid]) return ObavijestGreska(playerid, T(playerid, "Ovaj stand vise nije vas."));
    n = min(n, STAND_ZALIHA_MAX - gStand[s][snZaliha]);
    gStand[s][snZaliha] += n;
    StandSpremi(s);
    StandOsvjeziTekst(s);
    format(tekst, sizeof tekst, T(playerid, "Dostavili ste %d komada robe."), n);
    return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), tekst);
}

static StandNajblizaTrgovina(playerid)
{
    new t = -1, r, Float:najblize = 1.0e9, Float:x, Float:y, Float:z, Float:a;
    for (new i = 0; (r = TrgovinaLokacija(i, x, y, z, a)) != -1; i++)
    {
        if (!r) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, x, y, z);
        if (d >= najblize) continue;
        najblize = d;
        t = i;
    }
    return t;
}

StandCheckpoint(playerid)
{
    if (gStandNar[playerid] == SN_NEMA) return 0;
    new s = gStandNarS[playerid];
    if (gStandNar[playerid] == SN_GEPEK)
    {
        if (StandBlizu(playerid) != s) return 0;
        ObavijestGreska(playerid, T(playerid, "Izvadite kutiju iz prtljaznika (ALT kod prtljaznika)."));
        return 1;
    }
    if (gStandNar[playerid] == SN_NOSI)
    {
        if (StandBlizu(playerid) != s) return 0;
        if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) StandPredaj(playerid);
        else ObavijestGreska(playerid, T(playerid, "Izadite iz vozila i predajte robu (ALT kod standa)."));
        return 1;
    }
    new Float:x, Float:y, Float:z, Float:a, t = gStandNarTrgovina[playerid], iznos = gStandNarKolicina[playerid] * STAND_ZALIHA_CIJENA;
    if (TrgovinaLokacija(t, x, y, z, a) != 1 || !IsPlayerInRangeOfPoint(playerid, STAND_PREUZMI_DOMET, x, y, z)) return 0;
    if (!DajNovac(playerid, -iznos))
    {
        ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
        return 1;
    }
    FirmaProdaja(FIRMA_TRGOVINA, t, iznos, false);
    gStandNar[playerid] = SN_NOSI;
    GiveItem(playerid, ITEM_KUTIJA, 1);
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) StandKutijaUzmi(playerid);
    StandNarCp(playerid, true);
    return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Preuzeli ste robu. Odnesite je do standa."));
}

DLG:stand(playerid, response, listitem)
{
    new s = StandMoj(playerid);
    if (!response || s == -1 || listitem < 0 || listitem >= gStandOpcijaN[playerid]) return 1;
    switch (gStandOpcija[playerid][listitem])
    {
        case SO_IME: return PrikaziDialog(playerid, DLG_STAND_IME, DIALOG_STYLE_INPUT, T(playerid, "Promijeni ime"), T(playerid, "Upisite novo ime standa (1-24 znaka):"), T(playerid, "Spremi"), T(playerid, "Natrag"));
        case SO_KASA:
        {
            new iznos = gStand[s][snKasa];
            if (!iznos) return ObavijestGreska(playerid, T(playerid, "U kasi nema novca."));
            if (!DajNovac(playerid, iznos)) return ObavijestGreska(playerid, T(playerid, "Ne mozete primiti toliko novca."));
            gStand[s][snKasa] = 0;
            StandSpremi(s);
            new tekst[96];
            format(tekst, sizeof tekst, T(playerid, "Uzeli ste $%d iz kase."), iznos);
            return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), tekst);
        }
        case SO_ROBA:
        {
            if (gStandNar[playerid] != SN_NEMA)
            {
                StandNarudzbaKraj(playerid);
                return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Otkazali ste narudzbu robe."));
            }
            new body[128];
            format(body, sizeof body, T(playerid, "Koliko komada narucujete? ($%d po komadu, najvise jos %d)"), STAND_ZALIHA_CIJENA, STAND_ZALIHA_MAX - gStand[s][snZaliha]);
            return PrikaziDialog(playerid, DLG_STAND_ROBA, DIALOG_STYLE_INPUT, T(playerid, "Naruci robu"), body, T(playerid, "Naruci"), T(playerid, "Natrag"));
        }
        case SO_RADNIK:
        {
            if (gStand[s][snRadnik])
            {
                gStand[s][snRadnik] = false;
                gStand[s][snRadnikCeka] = false;
                gStand[s][snRadnikPauzaOd] = 0;
                StandAktor(s);
                StandSpremi(s);
                return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Otpustili ste radnika."));
            }
            gStand[s][snRadnik] = true;
            gStand[s][snRadnikCeka] = false;
            gStand[s][snRadnikDo] = 0;
            gStand[s][snRadnikPauzaOd] = 0;
            StandPlaca(s);
            if (gStand[s][snRadnikCeka]) return 1;
            StandAktor(s);
            return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Zaposlili ste radnika. Placa se svaki sat skida s vaseg bankovnog racuna."));
        }
        case SO_RADI:
        {
            if (gStandRadi[playerid] == s + 1)
            {
                if (PosaoIgraZa(playerid, IZVOR_STAND, s))
                    PosaoIgraKraj(playerid, false);

                gStandRadi[playerid] = 0;

                StandRadnikPauza(s, false);
                StandIgracOdmakni(playerid, s);

                return ObavijestInfo(playerid,
                    T(playerid, "Stand s hranom"),
                    T(playerid, "Prestali ste raditi."));
            }

            if (StandIgracRadi(s) != INVALID_PLAYER_ID)
                return ObavijestGreska(playerid,
                    T(playerid, "Netko vec radi na ovom standu."));

            if (gStand[s][snVrsta] == STAND_KOMBI && !gStand[s][snParkiran])
                return ObavijestGreska(playerid,
                    T(playerid, "Parkirajte kombi i ugasite motor."));

            new Float:x, Float:y, Float:a;
            StandRadnikMjesto(s, x, y, a);

            SetPlayerPos(playerid, x, y, StandRadnikZ(s) + 0.1);
            SetPlayerFacingAngle(playerid, a);
            if (gStand[s][snVrsta] == STAND_KOMBI)
            {
                TogglePlayerControllable(playerid, false);
                FpsPrisili(playerid, true);
            }

            gStandRadi[playerid] = s + 1;

            StandRadnikPauza(s, true);

            return ObavijestInfo(playerid,
                T(playerid, "Stand s hranom"),
                T(playerid, "Radite na standu. Kupci ce dolaziti, posluzite ih pravom narudzbom."));
        }
    }
    return 1;
}

static StandIgracOdmakni(playerid, s)
{
    if (gStand[s][snVrsta] == STAND_KOMBI)
    {
        new Float:v[3], Float:r;
        GetVehiclePos(gStand[s][snVozilo], v[0], v[1], v[2]);
        GetVehicleZAngle(gStand[s][snVozilo], r);
        v[0] += STAND_KOMBI_IZLAZ * floatsin(r, degrees);
        v[1] -= STAND_KOMBI_IZLAZ * floatcos(r, degrees);
        TogglePlayerControllable(playerid, true);
        FpsPrisili(playerid, false);
        SetPlayerPos(playerid, v[0], v[1], HodTlo(v[0], v[1], v[2]));
        SetPlayerFacingAngle(playerid, r);
        return;
    }
    new Float:x, Float:y, Float:a;
    StandRadnikMjesto(s, x, y, a);

    new Float:dx = x - gStand[s][snX];
    new Float:dy = y - gStand[s][snY];
    new Float:d = VectorSize(dx, dy, 0.0);

    if (d < 0.1) return;

    x += dx / d * 1.5;
    y += dy / d * 1.5;

    SetPlayerPos(playerid, x, y, gStand[s][snKz] + 0.1);
    SetPlayerFacingAngle(playerid, a);
}

DLG:stand_ime(playerid, response, const inputtext[])
{
    new s = StandMoj(playerid), len = strlen(inputtext);
    if (!response || s == -1) return 1;
    if (len < 1 || len > 24 || strfind(inputtext, "~") != -1 || strfind(inputtext, "{") != -1) return ObavijestGreska(playerid, T(playerid, "Ime mora imati 1-24 znaka, bez znakova ~ i {."));
    strcopy(gStand[s][snIme], inputtext, 25);
    StandSpremi(s);
    StandOsvjeziTekst(s);
    return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Ime standa je promijenjeno."));
}

DLG:stand_roba(playerid, response, const inputtext[])
{
    new s = StandMoj(playerid);
    if (!response || s == -1) return 1;
    new n = strval(inputtext);
    if (!ProvjeriBroj(inputtext) || n < 1 || n > STAND_ZALIHA_MAX - gStand[s][snZaliha]) return ObavijestGreska(playerid, T(playerid, "Neispravna kolicina."));
    new t = StandNajblizaTrgovina(playerid), Float:x, Float:y, Float:z, Float:a, tekst[160];
    if (t == -1) return ObavijestGreska(playerid, T(playerid, "Nema trgovine 24/7 za preuzimanje robe."));
    TrgovinaLokacija(t, x, y, z, a);
    gStandNar[playerid] = SN_TRGOVINA;
    gStandNarS[playerid] = s;
    gStandNarKolicina[playerid] = n;
    gStandNarTrgovina[playerid] = t;
    SetPlayerCheckpoint(playerid, x, y, z, 3.0);
    format(tekst, sizeof tekst, T(playerid, "Preuzmite %d komada robe u najblizoj trgovini 24/7 (%s). Placate $%d pri preuzimanju."), n, CestaZona(x, y), n * STAND_ZALIHA_CIJENA);
    return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), tekst);
}

DLG:stand_hrana(playerid, response, listitem)
{
    new s = gStandMeniS[playerid];
    if (!response || s < 0 || s >= STAND_MAX || listitem < 0 || listitem > 2 || StandBlizu(playerid) != s || !StandOtvoren(s)) return 1;
    if (gStandJede[playerid]) return ObavijestGreska(playerid, T(playerid, "Vec jedete."));
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 1;
    new v = gStand[s][snVrsta], cijena = gStandCijene[v][listitem];
    if (!DajNovac(playerid, -cijena)) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
    gStand[s][snKasa] += cijena;
    StandProdaja(s);
    new Float:hp;
    GetPlayerHealth(playerid, hp);
    hp += gStandZdravlje[v][listitem];
    SetPlayerHealth(playerid, hp > 100.0 ? 100.0 : hp);
    SetPlayerAttachedObject(playerid, STAND_HRANA_SLOT, gStandHranaModel[v][listitem], 6, 0.06, 0.03, 0.02, 0.0, 0.0, 0.0);
    if (gStandHranaModel[v][listitem] == 2647) ApplyAnimation(playerid, "VENDING", "VEND_Drink2_P", 4.1, false, false, false, false, 0, SYNC_ALL);
    else ApplyAnimation(playerid, "FOOD", "EAT_Burger", 4.1, false, false, false, false, 0, SYNC_ALL);
    gStandJede[playerid] = 1;
    SetTimerEx("StandPojeo", STAND_JEDENJE_MS, false, "ii", playerid, gAccountId[playerid]);
    new tekst[96];
    format(tekst, sizeof tekst, T(playerid, "Kupili ste %s za $%d."), T(playerid, gStandMeni[v][listitem]), cijena);
    return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), tekst);
}

forward StandPojeo(playerid, racun);
public StandPojeo(playerid, racun)
{
    gStandJede[playerid] = 0;
    if (IsPlayerConnected(playerid) && gAccountId[playerid] == racun) RemovePlayerAttachedObject(playerid, STAND_HRANA_SLOT);
}

static StandProdaja(s)
{
    if (gStand[s][snZaliha] > 0) gStand[s][snZaliha]--;
    StandSpremi(s);
    if (gStand[s][snZaliha] > 0) return;
    StandOsvjeziTekst(s);
    new ime[25];
    StandImeIli(s, ime, sizeof ime);
    if (StandRadnikRadi(s)) StandSms(gStand[s][snVlasnik], "Radnik (%s): Nestalo nam je robe. Narucite novu da mogu nastaviti raditi.", ime);
    new p = StandIgracRadi(s);
    if (p != INVALID_PLAYER_ID) ObavijestGreska(p, T(p, "Nestalo je robe. Narucite novu kako bi stand ponovno radio."));
}

static StandIgraPocni(playerid, s, k)
{
    new v = gStand[s][snVrsta], stvar = random(3);
    gStandIgraKupac[playerid] = k;
    KupacKaze(k, "", gStandMeni[v][stvar]);
    PosaoIgraPocni(playerid, IZVOR_STAND, s, SI_TRAKA + random(3), gStandMeni[v][stvar], gStandCijene[v][stvar]);
}

StandIgraGotova(playerid, s, bool:uspjeh, cijena)
{
    if (!gStand[s][snIma] || gStand[s][snKupac] != gStandIgraKupac[playerid]) return;
    if (uspjeh && gStand[s][snZaliha] > 0)
    {
        StandProdaja(s);
        new napojnica = random(100) < POSAO_NAPOJNICA_POSTO ? 1 + random(5) : 0, tekst[96];
        DajNovac(playerid, cijena + napojnica);
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
        if (napojnica) format(tekst, sizeof tekst, T(playerid, "Kupac je platio $%d i ostavio napojnicu od $%d."), cijena, napojnica);
        else format(tekst, sizeof tekst, T(playerid, "Kupac je platio $%d."), cijena);
        ObavijestInfo(playerid, T(playerid, "Stand s hranom"), tekst);
    }
    else ObavijestGreska(playerid, T(playerid, "Kupac je otisao nezadovoljan."));
    StandKupacGotov(s);
}

static KupacKaze(k, const tekst[], const a[] = "")
{
    new lik = gKupac[k][kuNpc], Float:x, Float:y, Float:z;
    if (lik == INVALID_LIK) return;
    Lik_Pozicija(lik, x, y, z);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUlogiran[i] && IsPlayerInRangeOfPoint(i, 15.0, x, y, z)) SendClientMessage(i, COLOR_CHAT, T(i, "Kupac kaze: %s"), a[0] ? T(i, a) : T(i, tekst));
}

static StandSluzi(s, k)
{
    gStand[s][snKupac] = k;
    gStand[s][snKupacOd] = GetTickCount();
    gKupac[k][kuStanje] = KU_SLUZI;
    Lik_Kut(gKupac[k][kuNpc], atan2(gStand[s][snY] - gStand[s][snKy], gStand[s][snX] - gStand[s][snKx]) - 90.0);
    new p = StandIgracRadi(s);
    if (p != INVALID_PLAYER_ID)
    {
        StandIgraPocni(p, s, k);
        return;
    }
    if (!StandRadnikRadi(s))
    {
        StandKupacGotov(s);
        return;
    }
    KupacKaze(k, "", gStandMeni[gStand[s][snVrsta]][random(3)]);
    if (gStand[s][snAktor] != INVALID_ACTOR_ID) ApplyActorAnimation(gStand[s][snAktor], "PED", "IDLE_chat", 4.1, false, false, false, false, STAND_RADNIK_MS);
}

static StandKupacGotov(s)
{
    new k = gStand[s][snKupac];
    gStand[s][snKupac] = -1;
    if (k >= 0 && gKupac[k][kuStanje] == KU_SLUZI) KupacDalje(k);
}

DostavaStandBroj()
{
    return STAND_MAX;
}

DostavaStand(s, &Float:x, &Float:y, &Float:z)
{
    if (s < 0 || s >= STAND_MAX || !StandOtvoren(s)) return 0;
    x = gStand[s][snKx];
    y = gStand[s][snKy];
    z = gStand[s][snKz];
    return 1;
}

DostavaStandPreuzmi(s)
{
    if (s < 0 || s >= STAND_MAX || !gStand[s][snIma] || gStand[s][snZaliha] <= 0) return 0;
    gStand[s][snZaliha]--;
    gStand[s][snKasa] += STAND_DOSTAVA;
    StandSpremi(s);
    return 1;
}

static KupacMakni(k)
{
    if (gKupac[k][kuNpc] != INVALID_LIK) Lik_Ukloni(gKupac[k][kuNpc]);
    gKupac[k][kuNpc] = INVALID_LIK;
    for (new s = 0; s < STAND_MAX; s++) if (gStand[s][snIma] && gStand[s][snKupac] == k) gStand[s][snKupac] = -1;
    gKupac[k][kuStanje] = KU_NEMA;
}

static bool:KupacIdi(k, Float:tx, Float:ty, Float:tz)
{
    new lik = gKupac[k][kuNpc];
    Lik_OcistiAnimaciju(lik);
    if (!Lik_IdiDo(lik, tx + float(random(21) - 10) / 10.0, ty + float(random(21) - 10) / 10.0, tz, LIK_HOD)) return false;
    gKupac[k][kuStanje] = KU_HODA;
    gKupac[k][kuOd] = gettime();
    gKupac[k][kuTx] = tx;
    gKupac[k][kuTy] = ty;
    gKupac[k][kuTz] = tz;
    return true;
}

static KupacOtvoreniStand(osim)
{
    new popis[STAND_MAX], n = 0;
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (s == osim || !StandOtvoren(s) || !gStandGledan[s]) continue;
        new broj = 0;
        for (new k = 0; k < KUPAC_MAX; k++) if (gKupac[k][kuStanje] != KU_NEMA && gKupac[k][kuCilj] == s) broj++;
        if (broj < KUPAC_PO_STANDU) popis[n++] = s;
    }
    return n ? popis[random(n)] : -1;
}

static KupacDalje(k)
{
    new s = gKupac[k][kuCilj];
    gKupac[k][kuPosjeta]++;
    new novi = gKupac[k][kuPosjeta] < 3 && random(3) ? KupacOtvoreniStand(s) : -1;
    if (novi != -1 && KupacIdi(k, gStand[novi][snKx], gStand[novi][snKy], gStand[novi][snKz]))
    {
        gKupac[k][kuCilj] = novi;
        return;
    }
    new Float:x, Float:y, Float:z;
    if (RestoranBroj() && RestoranPozicija(random(RestoranBroj()), x, y, z) && KupacIdi(k, x, y, z))
    {
        gKupac[k][kuCilj] = -1;
        return;
    }
    KupacMakni(k);
}

static Float:gIgPoz[MAX_PLAYERS][3], gIgPozN, gIgPozOd = -1000000;

static IgraciPozicije()
{
    new sad = GetTickCount();
    if (sad - gIgPozOd < 500) return;
    gIgPozOd = sad;
    gIgPozN = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i] || IsPlayerNPC(i) || GetPlayerVirtualWorld(i)) continue;
        GetPlayerPos(i, gIgPoz[gIgPozN][0], gIgPoz[gIgPozN][1], gIgPoz[gIgPozN][2]);
        gIgPozN++;
    }
}

static bool:KupacSkriven(Float:x, Float:y, Float:z, Float:r = KUPAC_SKRIVENO)
{
    IgraciPozicije();
    new Float:r2 = r * r;
    for (new i = 0; i < gIgPozN; i++)
    {
        new Float:dx = gIgPoz[i][0] - x, Float:dy = gIgPoz[i][1] - y, Float:dz = gIgPoz[i][2] - z;
        if (dx * dx + dy * dy + dz * dz <= r2) return false;
    }
    return true;
}

static StandVirtualnaProdaja(s)
{
    if (!StandOtvoren(s) || gStandGledan[s] || StandIgracRadi(s) != INVALID_PLAYER_ID || !StandRadnikRadi(s) || random(STAND_VIRT_S)) return;
    if (random(100) < STAND_RADNIK_GRESKA) return;
    gStand[s][snKasa] += gStandCijene[gStand[s][snVrsta]][random(3)] + (random(100) < POSAO_NAPOJNICA_POSTO ? 1 + random(5) : 0);
    StandProdaja(s);
}

static KupacNovi(cilj = -1)
{
    new k = -1;
    for (new i = 0; i < KUPAC_MAX && k == -1; i++) if (gKupac[i][kuStanje] == KU_NEMA) k = i;
    if (k == -1) return;
    new s = cilj == -1 ? KupacOtvoreniStand(-1) : cilj;
    if (s == -1 || !gStand[s][snIma]) return;
    new Float:x, Float:y, Float:z, bool:ima = false;
    for (new pokusaj = 0; pokusaj < 8 && !ima; pokusaj++)
    {
        if (cilj != -1) ima = Lik_TockaStaze(gStand[s][snKx], gStand[s][snKy], KUPAC_BLIZU_MIN, KUPAC_BLIZU_MAX, x, y, z);
        else ima = Lik_TockaStaze(gStand[s][snKx], gStand[s][snKy], KUPAC_SPAWN_MIN, KUPAC_SPAWN_MAX, x, y, z) && KupacSkriven(x, y, z);
    }
    if (!ima) return;
    new lik = Lik_Stvori(gKupacSkinovi[random(sizeof gKupacSkinovi)], x, y, z, float(random(360)));
    if (lik == INVALID_LIK) return;
    Lik_Ponasanje(lik, LIK_REAGIRA | LIK_IZBJEGAVA | LIK_RANJIV, KUPAC_RANJEN_HP);
    gKupac[k][kuNpc] = lik;
    gKupac[k][kuCilj] = s;
    gKupac[k][kuPosjeta] = 0;
    gKupac[k][kuNovacDo] = 0;
    gKupac[k][kuStanje] = KU_SPAWN;
    gKupac[k][kuOd] = gettime();
    gKupac[k][kuNapadac] = INVALID_PLAYER_ID;
    gKupac[k][kuNapadacRacun] = 0;
    gKupac[k][kuTx] = gStand[s][snKx];
    gKupac[k][kuTy] = gStand[s][snKy];
    gKupac[k][kuTz] = gStand[s][snKz];
    SetTimerEx("KupacKreni", KUPAC_KRENI_MS, false, "ii", k, lik);
}

static KupacLika(lik)
{
    if (lik == INVALID_LIK) return -1;
    for (new k = 0; k < KUPAC_MAX; k++) if (gKupac[k][kuStanje] != KU_NEMA && gKupac[k][kuNpc] == lik) return k;
    return -1;
}

forward KupacKreni(k, lik);
public KupacKreni(k, lik)
{
    if (gKupac[k][kuNpc] != lik || gKupac[k][kuStanje] != KU_SPAWN) return;
    if (!KupacIdi(k, gKupac[k][kuTx], gKupac[k][kuTy], gKupac[k][kuTz])) KupacMakni(k);
}

static KupacNaCilju(k)
{
    Lik_Stani(gKupac[k][kuNpc]);
    new s = gKupac[k][kuCilj];
    if (s < 0 || !gStand[s][snIma])
    {
        KupacMakni(k);
        return;
    }
    gKupac[k][kuStanje] = KU_CEKA;
    gKupac[k][kuOd] = gettime();
    if (gStand[s][snKupac] == -1 && StandOtvoren(s)) StandSluzi(s, k);
}

KupacStigao(lik)
{
    new k = KupacLika(lik);
    if (k == -1) return 0;
    if (gKupac[k][kuStanje] == KU_HODA)
    {
        if (gKupac[k][kuCilj] >= 0) KupacNaCilju(k);
        else KupacMakni(k);
    }
    return 1;
}

KupacSmrt(lik, killerid)
{
    new k = KupacLika(lik);
    if (k == -1) return 0;
    if (killerid != INVALID_PLAYER_ID && IsPlayerConnected(killerid) && !IsPlayerNPC(killerid))
    {
        DodajWanted(killerid);
        DodajWanted(killerid);
    }
    HitnaNpcUmro(LIK_PACIJENT + lik);
    KupacMakni(k);
    return 1;
}

static KupacPrekiniPosao(k)
{
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (!gStand[s][snIma] || gStand[s][snKupac] != k) continue;
        new p = StandIgracRadi(s);
        if (p != INVALID_PLAYER_ID && PosaoIgraZa(p, IZVOR_STAND, s)) PosaoIgraKraj(p, false);
        else gStand[s][snKupac] = -1;
    }
}

KupacSmiren(lik)
{
    new k = KupacLika(lik);
    if (k == -1) return 0;
    if (gKupac[k][kuStanje] == KU_REAGIRA && !KupacIdi(k, gKupac[k][kuTx], gKupac[k][kuTy], gKupac[k][kuTz])) KupacMakni(k);
    return 1;
}

KupacRanjen(lik)
{
    new k = KupacLika(lik);
    if (k == -1) return 0;
    new Float:x, Float:y, Float:z;
    KupacPrekiniPosao(k);
    gKupac[k][kuStanje] = KU_RANJEN;
    gKupac[k][kuStanjeOd] = gettime();
    Lik_Pozicija(lik, x, y, z);
    if (gettime() >= gKupac[k][kuNovacDo])
    {
        gKupac[k][kuNovacDo] = gettime() + KUPAC_NOVAC_PAUZA_S;
        BaciNovac(x + float(random(11) - 5) / 10.0, y + float(random(11) - 5) / 10.0, z, 0, 0, KUPAC_NOVAC_MIN + random(KUPAC_NOVAC_MAX - KUPAC_NOVAC_MIN + 1));
    }
    HitnaZaNpc(LIK_PACIJENT + lik, x, y, z);
    return 1;
}

KupacJeRanjen(pacijent)
{
    if (pacijent < LIK_PACIJENT) return 0;
    new k = KupacLika(pacijent - LIK_PACIJENT);
    return k >= 0 && gKupac[k][kuStanje] == KU_RANJEN ? 1 : 0;
}

KupacNajbliziRanjen(playerid, Float:domet)
{
    new best = INVALID_PLAYER_ID, Float:bd = domet, Float:x, Float:y, Float:z;
    for (new k = 0; k < KUPAC_MAX; k++)
    {
        if (gKupac[k][kuStanje] != KU_RANJEN) continue;
        Lik_Pozicija(gKupac[k][kuNpc], x, y, z);
        new Float:d = GetPlayerDistanceFromPoint(playerid, x, y, z);
        if (d < bd)
        {
            bd = d;
            best = LIK_PACIJENT + gKupac[k][kuNpc];
        }
    }
    return best;
}

KupacOzivi(pacijent, bool:spasen)
{
    if (pacijent < LIK_PACIJENT) return;
    new lik = pacijent - LIK_PACIJENT, k = KupacLika(lik);
    if (k < 0 || gKupac[k][kuStanje] != KU_RANJEN) return;
    if (!spasen)
    {
        KupacMakni(k);
        return;
    }
    Lik_Pridigni(lik);
    gKupac[k][kuCilj] = -1;
    new Float:x, Float:y, Float:z;
    if (RestoranBroj() && RestoranPozicija(random(RestoranBroj()), x, y, z) && KupacIdi(k, x, y, z)) return;
    KupacMakni(k);
}

KupacPogoden(lik, issuerid, Float:amount, WEAPON:weaponid)
{
    #pragma unused amount, weaponid
    new k = KupacLika(lik);
    if (k < 0) return -1;
    if (gKupac[k][kuStanje] == KU_RANJEN) return 1;
    if (issuerid != INVALID_PLAYER_ID && IsPlayerConnected(issuerid) && !IsPlayerNPC(issuerid))
    {
        if (gKupac[k][kuNapadacRacun] != gAccountId[issuerid]) DodajWanted(issuerid);
        gKupac[k][kuNapadac] = issuerid;
        gKupac[k][kuNapadacRacun] = gAccountId[issuerid];
    }
    if (gKupac[k][kuStanje] != KU_REAGIRA)
    {
        KupacPrekiniPosao(k);
        gKupac[k][kuStanje] = KU_REAGIRA;
        gKupac[k][kuStanjeOd] = gettime();
    }
    return 1;
}

StandTick()
{
    new sad = gettime();
    for (new s = 0; s < STAND_MAX; s++) gStandGledan[s] = gStand[s][snIma] && !KupacSkriven(gStand[s][snKx], gStand[s][snKy], gStand[s][snKz], STAND_VIDLJIV);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if ((gStandNar[i] == SN_NOSI || gStandNar[i] == SN_GEPEK) && gStand[gStandNarS[i]][snVrsta] == STAND_KOMBI) StandNarCp(i, false);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        new s = gStandRadi[i] - 1;
        if (s < 0) continue;
        new Float:x, Float:y, Float:a;
        StandRadnikMjesto(s, x, y, a);
        if (!gStand[s][snIma] || gStand[s][snVlasnik] != gAccountId[i] || (gStand[s][snVrsta] == STAND_KOMBI && !gStand[s][snParkiran]) || !IsPlayerInRangeOfPoint(i, STAND_RADI_DOMET, x, y, StandRadnikZ(s)))
        {
            if (PosaoIgraZa(i, IZVOR_STAND, s)) PosaoIgraKraj(i, false);
            if (gStand[s][snIma] && gStand[s][snVrsta] == STAND_KOMBI) StandIgracOdmakni(i, s);

            gStandRadi[i] = 0;
            StandRadnikPauza(s, false);

            ObavijestInfo(i, T(i, "Stand s hranom"),
                T(i, "Udaljili ste se od standa i prestali raditi."));
        }
    }
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (!gStand[s][snIma]) continue;
        if (sad % 60 == s % 60) StandPlaca(s);
        StandVirtualnaProdaja(s);
        if (gStand[s][snVrsta] == STAND_KOMBI && StandKombiStoji(s) != gStand[s][snParkiran])
        {
            gStand[s][snParkiran] = !gStand[s][snParkiran];
            if (gStand[s][snParkiran])
            {
                StandKombiPozicija(s);
                StandSpremi(s);
            }
            StandAktor(s);
        }
        new k = gStand[s][snKupac];
        if (k >= 0)
        {
            new ms = GetTickCount() - gStand[s][snKupacOd];
            if (StandIgracRadi(s) == INVALID_PLAYER_ID && ms >= STAND_RADNIK_MS)
            {
                if (StandOtvoren(s) && StandRadnikRadi(s) && random(100) >= STAND_RADNIK_GRESKA)
                {
                    gStand[s][snKasa] += gStandCijene[gStand[s][snVrsta]][random(3)] + (random(100) < POSAO_NAPOJNICA_POSTO ? 1 + random(5) : 0);
                    StandProdaja(s);
                }
                StandKupacGotov(s);
            }
            else if (ms >= STAND_IGRA_MS)
            {
                new p = StandIgracRadi(s);
                if (p != INVALID_PLAYER_ID && PosaoIgraZa(p, IZVOR_STAND, s)) PosaoIgraKraj(p, false);
                else StandKupacGotov(s);
            }
        }
    }
    new otvoreno = 0, aktivni = 0;
    for (new s = 0; s < STAND_MAX; s++)
    {
        new bool:otvoren = StandOtvoren(s) && gStandGledan[s];
        if (otvoren) otvoreno++;
        if (otvoren && !gStandBio[s]) for (new k = 0; k < KUPAC_NA_OTVARANJE; k++) KupacNovi(s);
        gStandBio[s] = otvoren;
    }
    new ukupno = 0;
    for (new k = 0; k < KUPAC_MAX; k++)
    {
        if (gKupac[k][kuStanje] == KU_NEMA) continue;
        if (gKupac[k][kuStanje] < KU_REAGIRA && gKupac[k][kuStanje] != KU_SPAWN)
        {
            new Float:kx, Float:ky, Float:kz, c = gKupac[k][kuCilj];
            Lik_Pozicija(gKupac[k][kuNpc], kx, ky, kz);
            if ((c < 0 || !gStandGledan[c]) && KupacSkriven(kx, ky, kz, STAND_VIDLJIV))
            {
                KupacMakni(k);
                continue;
            }
        }
        ukupno++;
        if (gKupac[k][kuCilj] >= 0) aktivni++;
        new s = gKupac[k][kuCilj];
        if (gKupac[k][kuStanje] == KU_CEKA)
        {
            if (s >= 0 && gStand[s][snIma] && gStand[s][snKupac] == -1 && StandOtvoren(s)) StandSluzi(s, k);
            else if (sad - gKupac[k][kuOd] >= KUPAC_CEKA_S || s < 0 || !StandOtvoren(s)) KupacDalje(k);
        }
        else if (gKupac[k][kuStanje] == KU_HODA)
        {
            new Float:x, Float:y, Float:z;
            Lik_Pozicija(gKupac[k][kuNpc], x, y, z);
            if (s >= 0 && gStand[s][snIma] && VectorSize(x - gStand[s][snKx], y - gStand[s][snKy], 0.0) <= KUPAC_DOLAZAK) KupacNaCilju(k);
            else if (sad - gKupac[k][kuOd] >= KUPAC_ZAGLAVIO_S) KupacMakni(k);
        }
        else if (gKupac[k][kuStanje] == KU_SPAWN && sad - gKupac[k][kuOd] >= 10) KupacMakni(k);
        else if (gKupac[k][kuStanje] == KU_RANJEN && sad - gKupac[k][kuStanjeOd] >= KUPAC_RANJEN_S) KupacMakni(k);
        else if (gKupac[k][kuStanje] == KU_REAGIRA && sad - gKupac[k][kuStanjeOd] >= 60) KupacMakni(k);
    }
    if (sad - gKupacZadnji >= KUPAC_SPAWN_S && ukupno < KUPAC_MAX && aktivni < otvoreno * KUPAC_PO_STANDU)
    {
        gKupacZadnji = sad;
        KupacNovi();
    }
}

StandUgasi()
{
    for (new s = 0; s < STAND_MAX; s++) if (gStand[s][snIma]) StandRadnikPauza(s, false);
}

StandDisconnect(playerid)
{
    StandNarudzbaKraj(playerid);
    new s = gStandRadi[playerid] - 1;
    if (s >= 0 && s < STAND_MAX && gStand[s][snIma])
    StandRadnikPauza(s, false);
    gStandRadi[playerid] = 0;
    gStandJede[playerid] = 0;
    gStandUredi[playerid][0] = 0;
}

ShowStandoveAdmin(playerid)
{
    new list[2048], red[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "+ Novi stand (stand se pojavljuje ispred vas, kupci stoje gdje vi stojite)"));
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (!gStand[s][snIma]) continue;
        new ime[25];
        StandImeIli(s, ime, sizeof ime);
        format(red, sizeof red, "[%d] %s (%s)", gStand[s][snDb], ime, CestaZona(gStand[s][snX], gStand[s][snY]));
        DodajStupac(list, len, red);
        gApStandLista[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_STAND, DIALOG_STYLE_LIST, T(playerid, "Standovi s hranom"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_stand(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new list[128], len = 0;
        for (new v = 0; v < sizeof gStandVrsta; v++) DodajStupac(list, len, T(playerid, gStandVrsta[v]));
        return PrikaziDialog(playerid, DLG_APANEL_STAND_MODEL, DIALOG_STYLE_LIST, T(playerid, "Vrsta standa"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    new n = 0;
    for (new s = 0; s < STAND_MAX; s++) if (gStand[s][snIma]) n++;
    if (listitem - 1 >= n) return ShowStandoveAdmin(playerid);
    gApStand[playerid] = gApStandLista[playerid][listitem - 1];
    return PrikaziDialog(playerid, DLG_APANEL_STAND_MENI, DIALOG_STYLE_LIST, T(playerid, "Standovi s hranom"), T(playerid, "Pomakni stand\nObrisi stand"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_stand_model(playerid, response, listitem)
{
    if (!response) return ShowStandoveAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG) || listitem < 0 || listitem >= sizeof gStandModeli) return 1;
    new s = 0;
    while (s < STAND_MAX && gStand[s][snIma]) s++;
    if (s == STAND_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Stand se ovdje ne moze postaviti."));
    new Float:a;
    GetPlayerPos(playerid, gStand[s][snKx], gStand[s][snKy], gStand[s][snKz]);
    GetPlayerFacingAngle(playerid, a);
    gStand[s][snVrsta] = listitem;
    new Float:razmak = listitem == STAND_KOMBI ? 4.0 : 1.6;
    gStand[s][snX] = gStand[s][snKx] - razmak * floatsin(a, degrees);
    gStand[s][snY] = gStand[s][snKy] + razmak * floatcos(a, degrees);
    gStand[s][snZ] = gStand[s][snKz];
    gStand[s][snRz] = a;
    gStand[s][snVlasnik] = 0;
    gStand[s][snVlasnikIme][0] = EOS;
    gStand[s][snIme][0] = EOS;
    gStand[s][snZaliha] = 0;
    gStand[s][snKasa] = 0;
    gStand[s][snRadnik] = false;
    gStand[s][snRadnikCeka] = false;
    gStand[s][snRadnikDo] = 0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO food_stands (kind, x, y, z, rz, kx, ky, kz) VALUES (%d, %f, %f, %f, %f, %f, %f, %f)", listitem, gStand[s][snX], gStand[s][snY], gStand[s][snZ], a, gStand[s][snKx], gStand[s][snKy], gStand[s][snKz]));
    gStand[s][snDb] = ZadnjiId();
    StandStvori(s);
    if (listitem == STAND_KOMBI)
    {
        StandSpremi(s);
        return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kombi s hranom je postavljen i ceka kupca."));
    }
    gStandUredi[playerid][0] = 1;
    gStandUredi[playerid][1] = s;
    EditDynamicObject(playerid, gStand[s][snObjekt]);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Stand je postavljen. Namjestite ga i spremite (disketa)."));
}

DLG:apanel_stand_meni(playerid, response, listitem)
{
    new s = gApStand[playerid];
    if (!response) return ShowStandoveAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG) || s < 0 || s >= STAND_MAX || !gStand[s][snIma]) return 1;
    switch (listitem)
    {
        case 0:
        {
            if (gStand[s][snVrsta] == STAND_KOMBI)
            {
                new Float:x, Float:y, Float:z, Float:a;
                if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Stand se ovdje ne moze postaviti."));
                GetPlayerPos(playerid, x, y, z);
                GetPlayerFacingAngle(playerid, a);
                SetVehiclePos(gStand[s][snVozilo], x - 4.0 * floatsin(a, degrees), y + 4.0 * floatcos(a, degrees), z);
                SetVehicleZAngle(gStand[s][snVozilo], a);
                StandKombiPozicija(s);
                StandSpremi(s);
                StandAktor(s);
                SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kombi je premjesten ispred vas."));
                return ShowStandoveAdmin(playerid);
            }
            gStandUredi[playerid][0] = 1;
            gStandUredi[playerid][1] = s;
            EditDynamicObject(playerid, gStand[s][snObjekt]);
            return 1;
        }
        case 1:
        {
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM food_stands WHERE id = %d", gStand[s][snDb]));
            StandMakni(s);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Stand je obrisan."));
        }
    }
    return ShowStandoveAdmin(playerid);
}

StandUredi(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rz)
{
    new tip = gStandUredi[playerid][0], i = gStandUredi[playerid][1];
    if (!tip) return 0;
    if (tip == 1 && (i < 0 || i >= STAND_MAX || !gStand[i][snIma] || gStand[i][snObjekt] != objectid)) return 0;
    if (response == _:EDIT_RESPONSE_UPDATE) return 1;
    gStandUredi[playerid][0] = 0;
    if (tip == 1)
    {
        if (response == _:EDIT_RESPONSE_FINAL)
        {
            gStand[i][snX] = x;
            gStand[i][snY] = y;
            gStand[i][snZ] = z;
            gStand[i][snRz] = rz;
            StandSpremi(i);
        }
        SetDynamicObjectPos(objectid, gStand[i][snX], gStand[i][snY], gStand[i][snZ]);
        SetDynamicObjectRot(objectid, 0.0, 0.0, gStand[i][snRz]);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, gStand[i][snTekst], E_STREAMER_X, gStand[i][snX]);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, gStand[i][snTekst], E_STREAMER_Y, gStand[i][snY]);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, gStand[i][snTekst], E_STREAMER_Z, gStand[i][snZ] + 1.6);
        StandAktor(i);
        return 1;
    }
    return 1;
}
