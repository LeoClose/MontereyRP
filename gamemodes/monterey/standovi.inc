#define STAND_MAX            32
#define STAND_CIJENA         5000
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
#define STAND_TRAKA_MS       40
#define STAND_TRAKA_ZELENO   18.0
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
#define STAND_NAPOJNICA_POSTO 30
#define STAND_RADNIK_GRESKA  20
#define KUPAC_BORBA_S        20
#define KUPAC_BORBA_DOMET    30.0
#define KUPAC_RANJEN_S       240
#define KUPAC_IZBJEGNI_MS    200
#define KUPAC_IZBJEGNI_S     2
#define KUPAC_DOLAZAK        3.0
#define KUPAC_MIRNO_S        3
#define KUPAC_POKUSAJA       6
#define KUPAC_KRENI_MS       700

static const gStandModeli[] = { 1340, 1341, 1342 };
static const gStandVrsta[][] = { "Hot dog", "Sladoled", "Rezanci" };
static const gStandMeni[][][] = {
    { "Hot dog", "Chili dog", "Cola" },
    { "Sladoled od vanilije", "Cokoladni sladoled", "Sladoled od jagode" },
    { "Rezanci s piletinom", "Rezanci s govedinom", "Proljetna rolica" }
};
static const gStandCijene[][3] = { { 4, 5, 2 }, { 3, 3, 3 }, { 8, 8, 3 } };
static const gStandHranaModel[][3] = { { 19346, 19346, 2647 }, { 19568, 19568, 19568 }, { 19811, 19811, 19811 } };
static const Float:gStandZdravlje[][3] = { { 15.0, 18.0, 5.0 }, { 8.0, 8.0, 8.0 }, { 25.0, 25.0, 10.0 } };
static gStandJede[MAX_PLAYERS];
enum { SI_NEMA, SI_TRAKA, SI_TIPKE, SI_KUSUR }
static const KEY:gStTipke[] = { KEY_YES, KEY_NO, KEY_WALK, KEY_SPRINT };
static const gStTipkeIme[][] = { "Y", "N", "ALT", "SPACE" };
static const gStNovcanice[] = { 5, 10, 20, 50 };
static gStIgra[MAX_PLAYERS], gStIgraS[MAX_PLAYERS], gStIgraK[MAX_PLAYERS], gStIgraStvar[MAX_PLAYERS], gStIgraCijena[MAX_PLAYERS], gStIgraOdg[MAX_PLAYERS], gStIgraTipke[MAX_PLAYERS][4], gStIgraKorak[MAX_PLAYERS], gStIgraTimer[MAX_PLAYERS];
static Float:gStIgraTraka[MAX_PLAYERS], Float:gStIgraSmjer[MAX_PLAYERS], Float:gStIgraZel[MAX_PLAYERS], PlayerText:gStIgraTd[MAX_PLAYERS][2];
static const gKupacSkinovi[] = { 7, 14, 15, 20, 22, 23, 24, 26, 29, 35, 37, 44, 46, 47, 48, 56, 59, 60, 72, 73, 93, 98, 101, 141, 150, 151, 170, 184, 185, 188, 190, 192, 193, 211, 214, 216, 217, 223, 225, 226, 227, 233, 240, 250, 261, 289, 290, 291 };

enum E_STAND { bool:snIma, snDb, snVrsta, Float:snX, Float:snY, Float:snZ, Float:snRz, Float:snKx, Float:snKy, Float:snKz, snVlasnik, snVlasnikIme[MAX_PLAYER_NAME + 1], snIme[25],
    snZaliha, snKasa, bool:snRadnik, snRadnikDo, snRadnikPauzaOd, bool:snRadnikCeka, snAktor, STREAMER_TAG_OBJECT:snObjekt, STREAMER_TAG_3D_TEXT_LABEL:snTekst, snKupac, snKupacOd }
static gStand[STAND_MAX][E_STAND];
static gStandRadi[MAX_PLAYERS], gStandMeniS[MAX_PLAYERS], gStandOpcija[MAX_PLAYERS][8], gStandOpcijaN[MAX_PLAYERS], gStandUredi[MAX_PLAYERS][2], bool:gStandBio[STAND_MAX], gApStand[MAX_PLAYERS], gApStandLista[MAX_PLAYERS][STAND_MAX];

enum { KU_NEMA, KU_SPAWN, KU_HODA, KU_CEKA, KU_SLUZI, KU_BORBA, KU_BJEZI, KU_RANJEN, KU_IZBJEGAVA }
enum E_KUPAC { kuStanje, kuNpc, kuPut, kuCilj, kuPosjeta, kuOd, Float:kuX, Float:kuY, Float:kuZ, Float:kuPx, Float:kuPy, kuMirno, kuPokusaj, kuNovacDo, Float:kuTx, Float:kuTy, Float:kuTz, kuNapadac, kuNapadacRacun, kuStanjeOd }
static gKupac[KUPAC_MAX][E_KUPAC], gNpcKupac[MAX_PLAYERS], gKupacRuta[STAZA_RUTA], gKupacZadnji;

enum { SO_IME, SO_KASA, SO_ROBA, SO_RADNIK, SO_RADI }

InitStandovi()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS food_stands (id INTEGER PRIMARY KEY AUTOINCREMENT, kind INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, rz REAL NOT NULL, kx REAL NOT NULL, ky REAL NOT NULL, kz REAL NOT NULL, owner INTEGER NOT NULL DEFAULT 0, owner_name TEXT NOT NULL DEFAULT '', name TEXT NOT NULL DEFAULT '', stock INTEGER NOT NULL DEFAULT 0, till INTEGER NOT NULL DEFAULT 0, worker INTEGER NOT NULL DEFAULT 0, worker_until INTEGER NOT NULL DEFAULT 0)"));
    for (new k = 0; k < KUPAC_MAX; k++)
    {
        gKupac[k][kuNpc] = INVALID_PLAYER_ID;
        gKupac[k][kuPut] = -1;
    }
    SetTimer("KupacIzbjegavaj", KUPAC_IZBJEGNI_MS, true);
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
    else format(tekst, sizeof tekst, "%s\n{FFFFFF}$%d", ime, STAND_CIJENA);
    UpdateDynamic3DTextLabelText(gStand[s][snTekst], STAND_BOJA, tekst);
}

static StandRadnikMjesto(s, &Float:x, &Float:y, &Float:a)
{
    new Float:dx = gStand[s][snX] - gStand[s][snKx], Float:dy = gStand[s][snY] - gStand[s][snKy], Float:d = VectorSize(dx, dy, 0.0);
    if (d < 0.1) d = 1.0;
    x = gStand[s][snX] + dx / d * STAND_RADNIK_RAZMAK;
    y = gStand[s][snY] + dy / d * STAND_RADNIK_RAZMAK;
    a = atan2(-dy, -dx) - 90.0;
}

static StandAktor(s)
{
    if (gStand[s][snAktor] != INVALID_ACTOR_ID)
    {
        DestroyActor(gStand[s][snAktor]);
        gStand[s][snAktor] = INVALID_ACTOR_ID;
    }
    if (!gStand[s][snRadnik] || gStand[s][snRadnikCeka] || gStand[s][snRadnikPauzaOd]) return;
    new Float:x, Float:y, Float:a;
    StandRadnikMjesto(s, x, y, a);
    gStand[s][snAktor] = CreateActor(STAND_RADNIK_SKIN, x, y, gStand[s][snKz], a);
    SetActorInvulnerable(gStand[s][snAktor], true);
}

static StandStvori(s)
{
    gStand[s][snIma] = true;
    gStand[s][snAktor] = INVALID_ACTOR_ID;
    gStand[s][snKupac] = -1;
    gStand[s][snObjekt] = CreateDynamicObject(gStandModeli[gStand[s][snVrsta]], gStand[s][snX], gStand[s][snY], gStand[s][snZ], 0.0, 0.0, gStand[s][snRz]);
    gStand[s][snTekst] = CreateDynamic3DTextLabel(" ", STAND_BOJA, gStand[s][snX], gStand[s][snY], gStand[s][snZ] + 1.6, 12.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
    InterakcijaOznaka(gStand[s][snTekst], 3.0);
    StandOsvjeziTekst(s);
    StandAktor(s);
}

static StandMakni(s)
{
    for (new k = 0; k < KUPAC_MAX; k++) if (gKupac[k][kuStanje] != KU_NEMA && gKupac[k][kuCilj] == s) KupacMakni(k);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gStandRadi[i] == s + 1) gStandRadi[i] = 0;
    DestroyDynamicObject(gStand[s][snObjekt]);
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
    return gStand[s][snIma] && gStand[s][snVlasnik] && gStand[s][snZaliha] > 0 && (StandRadnikRadi(s) || StandIgracRadi(s) != INVALID_PLAYER_ID);
}

StandBlizu(playerid)
{
    for (new s = 0; s < STAND_MAX; s++)
        if (gStand[s][snIma] && (IsPlayerInRangeOfPoint(playerid, STAND_DOMET, gStand[s][snKx], gStand[s][snKy], gStand[s][snKz]) || IsPlayerInRangeOfPoint(playerid, STAND_DOMET, gStand[s][snX], gStand[s][snY], gStand[s][snZ]))) return s;
    return -1;
}

static StandSms(racun, const tekst[], const ime[], iznos = 0)
{
    new poruka[144], i = IgracRacuna(racun);
    if (i != INVALID_PLAYER_ID) format(poruka, sizeof poruka, T(i, tekst), ime, iznos);
    else format(poruka, sizeof poruka, tekst, ime, iznos);
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
    if (!gStand[s][snVlasnik])
    {
        new body[160];
        format(body, sizeof body, T(playerid, "Kupiti %s stand za $%d?"), T(playerid, gStandVrsta[gStand[s][snVrsta]]), STAND_CIJENA);
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
    format(red, sizeof red, T(playerid, "Naruci robu (zaliha %d/%d, $%d po komadu)"), gStand[s][snZaliha], STAND_ZALIHA_MAX, STAND_ZALIHA_CIJENA);
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
    for (new k = 0; k < STAND_MAX; k++) if (gStand[k][snIma] && gStand[k][snVlasnik] == gAccountId[playerid]) return ObavijestGreska(playerid, T(playerid, "Vec imate stand s hranom."));
    if (!DajNovac(playerid, -STAND_CIJENA)) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
    gStand[s][snVlasnik] = gAccountId[playerid];
    GetPlayerName(playerid, gStand[s][snVlasnikIme], MAX_PLAYER_NAME + 1);
    gStand[s][snIme][0] = EOS;
    gStand[s][snKasa] = 0;
    StandSpremi(s);
    StandOsvjeziTekst(s);
    return ObavijestInfo(playerid, T(playerid, "Stand s hranom"), T(playerid, "Kupili ste stand. Narucite robu i zaposlite radnika ili radite sami (ALT kod standa)."));
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
                if (gStIgra[playerid])
                    StandIgraKraj(playerid, false);

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

            new Float:x, Float:y, Float:a;
            StandRadnikMjesto(s, x, y, a);

            SetPlayerPos(playerid, x, y, gStand[s][snKz] + 0.1);
            SetPlayerFacingAngle(playerid, a);

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
    if (!DajNovac(playerid, -n * STAND_ZALIHA_CIJENA)) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
    gStand[s][snZaliha] += n;
    StandSpremi(s);
    StandOsvjeziTekst(s);
    new tekst[96];
    format(tekst, sizeof tekst, T(playerid, "Narucili ste %d komada za $%d."), n, n * STAND_ZALIHA_CIJENA);
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

static StandIgraPanel(playerid)
{
    new tekst[256], red[128];
    format(tekst, sizeof tekst, T(playerid, "Kupac zeli: %s ($%d)"), T(playerid, gStandMeni[gStand[gStIgraS[playerid]][snVrsta]][gStIgraStvar[playerid]]), gStIgraCijena[playerid]);
    strcat(tekst, "~n~");
    switch (gStIgra[playerid])
    {
        case SI_TRAKA: strcat(tekst, T(playerid, "Pritisnite ~y~SPACE~w~ kad je traka zelena."));
        case SI_TIPKE:
        {
            strcat(tekst, T(playerid, "Pritisnite redom:"));
            for (new i = 0; i < 4; i++)
            {
                format(red, sizeof red, " %s%s", i < gStIgraKorak[playerid] ? ("~g~") : ("~y~"), gStTipkeIme[gStIgraTipke[playerid][i]]);
                strcat(tekst, red);
            }
        }
        case SI_KUSUR:
        {
            format(red, sizeof red, T(playerid, "Kupac daje $%d. Upisite ostatak u chat."), gStIgraCijena[playerid] + gStIgraOdg[playerid]);
            strcat(tekst, red);
        }
    }
    IgraObavijestPanel(playerid, T(playerid, "Kupac"), STAND_BOJA, tekst);
}

static StandIgraPocni(playerid, s, k)
{
    new v = gStand[s][snVrsta], stvar = random(3);
    gStIgraS[playerid] = s;
    gStIgraK[playerid] = k;
    gStIgraStvar[playerid] = stvar;
    gStIgraCijena[playerid] = gStandCijene[v][stvar];
    gStIgraKorak[playerid] = 0;
    gStIgra[playerid] = 1 + random(3);
    KupacKaze(k, "", gStandMeni[v][stvar]);
    switch (gStIgra[playerid])
    {
        case SI_TRAKA:
        {
            gStIgraTraka[playerid] = 0.0;
            gStIgraSmjer[playerid] = 3.5;
            gStIgraZel[playerid] = float(25 + random(55));
            SnagaBarStvori(playerid, gStIgraTd[playerid][0], gStIgraTd[playerid][1]);
            gStIgraTimer[playerid] = SetTimerEx("StandTrakaKorak", STAND_TRAKA_MS, true, "i", playerid);
        }
        case SI_TIPKE: for (new i = 0; i < 4; i++) gStIgraTipke[playerid][i] = random(sizeof gStTipke);
        case SI_KUSUR:
        {
            new opcije[sizeof gStNovcanice], n = 0;
            for (new i = 0; i < sizeof gStNovcanice; i++) if (gStNovcanice[i] > gStIgraCijena[playerid]) opcije[n++] = gStNovcanice[i];
            gStIgraOdg[playerid] = opcije[random(n)] - gStIgraCijena[playerid];
        }
    }
    StandIgraPanel(playerid);
}

forward StandTrakaKorak(playerid);
public StandTrakaKorak(playerid)
{
    if (gStIgra[playerid] != SI_TRAKA) return;
    gStIgraTraka[playerid] += gStIgraSmjer[playerid];
    if (gStIgraTraka[playerid] >= 100.0 || gStIgraTraka[playerid] <= 0.0) gStIgraSmjer[playerid] = -gStIgraSmjer[playerid];
    SnagaBarPostavi(playerid, gStIgraTd[playerid][1], gStIgraTraka[playerid], gStIgraZel[playerid], gStIgraZel[playerid] + STAND_TRAKA_ZELENO);
}

static StandIgraKraj(playerid, bool:uspjeh)
{
    new s = gStIgraS[playerid], igra = gStIgra[playerid];
    gStIgra[playerid] = SI_NEMA;
    if (igra == SI_TRAKA)
    {
        if (gStIgraTimer[playerid]) KillTimer(gStIgraTimer[playerid]);
        gStIgraTimer[playerid] = 0;
        PlayerTextDrawDestroy(playerid, gStIgraTd[playerid][0]);
        PlayerTextDrawDestroy(playerid, gStIgraTd[playerid][1]);
    }
    IgraObavijestUkloni(playerid);
    if (!gStand[s][snIma] || gStand[s][snKupac] != gStIgraK[playerid]) return;
    if (uspjeh && gStand[s][snZaliha] > 0)
    {
        StandProdaja(s);
        new napojnica = random(100) < STAND_NAPOJNICA_POSTO ? 1 + random(5) : 0, tekst[96];
        DajNovac(playerid, gStIgraCijena[playerid] + napojnica);
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
        if (napojnica) format(tekst, sizeof tekst, T(playerid, "Kupac je platio $%d i ostavio napojnicu od $%d."), gStIgraCijena[playerid], napojnica);
        else format(tekst, sizeof tekst, T(playerid, "Kupac je platio $%d."), gStIgraCijena[playerid]);
        ObavijestInfo(playerid, T(playerid, "Stand s hranom"), tekst);
    }
    else ObavijestGreska(playerid, T(playerid, "Kupac je otisao nezadovoljan."));
    StandKupacGotov(s);
}

StandKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (gStIgra[playerid] == SI_TRAKA)
    {
        if (!(newkeys & KEY_SPRINT) || (oldkeys & KEY_SPRINT)) return 0;
        StandIgraKraj(playerid, gStIgraTraka[playerid] >= gStIgraZel[playerid] && gStIgraTraka[playerid] <= gStIgraZel[playerid] + STAND_TRAKA_ZELENO);
        return 1;
    }
    if (gStIgra[playerid] != SI_TIPKE) return 0;
    new pritisnuta = -1;
    for (new i = 0; i < sizeof gStTipke; i++) if ((newkeys & gStTipke[i]) && !(oldkeys & gStTipke[i])) pritisnuta = i;
    if (pritisnuta == -1) return 0;
    if (pritisnuta != gStIgraTipke[playerid][gStIgraKorak[playerid]])
    {
        StandIgraKraj(playerid, false);
        return 1;
    }
    if (++gStIgraKorak[playerid] >= 4) StandIgraKraj(playerid, true);
    else StandIgraPanel(playerid);
    return 1;
}

StandChat(playerid, const text[])
{
    if (gStIgra[playerid] != SI_KUSUR || !ProvjeriBroj(text) || strlen(text) > 4) return 0;
    StandIgraKraj(playerid, strval(text) == gStIgraOdg[playerid]);
    return 1;
}

static KupacKaze(k, const tekst[], const a[] = "")
{
    new npc = gKupac[k][kuNpc], Float:x, Float:y, Float:z;
    if (npc == INVALID_PLAYER_ID) return;
    NPC_GetPos(npc, x, y, z);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUlogiran[i] && IsPlayerInRangeOfPoint(i, 15.0, x, y, z)) SendClientMessage(i, COLOR_CHAT, T(i, "Kupac kaze: %s"), a[0] ? T(i, a) : T(i, tekst));
}

static StandSluzi(s, k)
{
    gStand[s][snKupac] = k;
    gStand[s][snKupacOd] = GetTickCount();
    gKupac[k][kuStanje] = KU_SLUZI;
    NPC_SetFacingAngle(gKupac[k][kuNpc], atan2(gStand[s][snY] - gStand[s][snKy], gStand[s][snX] - gStand[s][snKx]) - 90.0);
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
    new npc = gKupac[k][kuNpc];
    if (gKupac[k][kuPut] != -1) NPC_DestroyPath(gKupac[k][kuPut]);
    gKupac[k][kuPut] = -1;
    if (npc != INVALID_PLAYER_ID)
    {
        gNpcKupac[npc] = 0;
        NPC_Destroy(npc);
    }
    gKupac[k][kuNpc] = INVALID_PLAYER_ID;
    for (new s = 0; s < STAND_MAX; s++) if (gStand[s][snIma] && gStand[s][snKupac] == k) gStand[s][snKupac] = -1;
    gKupac[k][kuStanje] = KU_NEMA;
}

static bool:KupacIdi(k, Float:tx, Float:ty, Float:tz)
{
    new npc = gKupac[k][kuNpc], Float:x, Float:y, Float:z;
    NPC_GetPos(npc, x, y, z);
    new a = StazaNajbliza(x, y, z), b = StazaNajbliza(tx, ty, tz), n = StazaPut(a, b, gKupacRuta, sizeof gKupacRuta);
    if (!n) return false;
    if (gKupac[k][kuPut] != -1) NPC_DestroyPath(gKupac[k][kuPut]);
    new put = NPC_CreatePath();
    if (!NPC_IsValidPath(put)) return false;
    for (new i = 0; i < n; i++)
    {
        StazaTocka(gKupacRuta[i], x, y, z);
        NPC_AddPointToPath(put, x, y, HodTlo(x, y, z), 0.6);
    }
    NPC_AddPointToPath(put, tx + float(random(21) - 10) / 10.0, ty + float(random(21) - 10) / 10.0, HodTlo(tx, ty, tz), 1.0);
    gKupac[k][kuPut] = put;
    gKupac[k][kuStanje] = KU_HODA;
    gKupac[k][kuOd] = gettime();
    gKupac[k][kuMirno] = 0;
    gKupac[k][kuTx] = tx;
    gKupac[k][kuTy] = ty;
    gKupac[k][kuTz] = tz;
    NPC_MoveByPath(npc, put, NPC_MOVE_TYPE_WALK);
    return true;
}

static KupacOtvoreniStand(osim)
{
    new popis[STAND_MAX], n = 0;
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (s == osim || !StandOtvoren(s)) continue;
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

static bool:KupacSkriven(Float:x, Float:y, Float:z)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUlogiran[i] && !IsPlayerNPC(i) && IsPlayerInRangeOfPoint(i, KUPAC_SKRIVENO, x, y, z)) return false;
    return true;
}

static KupacNovi(cilj = -1)
{
    new k = -1;
    for (new i = 0; i < KUPAC_MAX && k == -1; i++) if (gKupac[i][kuStanje] == KU_NEMA) k = i;
    if (k == -1) return;
    new s = cilj == -1 ? KupacOtvoreniStand(-1) : cilj;
    if (s == -1) return;
    new cvor = -1, Float:x, Float:y, Float:z;
    for (new pokusaj = 0; pokusaj < (cilj != -1 ? 400 : 40) && cvor == -1; pokusaj++)
    {
        new c = random(StazaBrojTocaka());
        StazaTocka(c, x, y, z);
        new Float:d = VectorSize(x - gStand[s][snKx], y - gStand[s][snKy], 0.0);
        if (cilj != -1 ? (d >= KUPAC_BLIZU_MIN && d <= KUPAC_BLIZU_MAX) : (d >= KUPAC_SPAWN_MIN && d <= KUPAC_SPAWN_MAX && KupacSkriven(x, y, z))) cvor = c;
    }
    if (cvor == -1) return;
    new ime[MAX_PLAYER_NAME + 1];
    format(ime, sizeof ime, "Kupac_%d", k);
    new npc = NPC_Create(ime);
    if (npc == INVALID_PLAYER_ID) return;
    gKupac[k][kuNpc] = npc;
    gKupac[k][kuCilj] = s;
    gKupac[k][kuPosjeta] = 0;
    gKupac[k][kuNovacDo] = 0;
    gKupac[k][kuStanje] = KU_SPAWN;
    gKupac[k][kuOd] = gettime();
    gKupac[k][kuPut] = -1;
    gKupac[k][kuX] = x;
    gKupac[k][kuY] = y;
    gKupac[k][kuZ] = HodTlo(x, y, z);
    gNpcKupac[npc] = k + 1;
    NPC_Spawn(npc);
}

KupacSpawn(npcid)
{
    new k = gNpcKupac[npcid] - 1;
    if (k < 0) return;
    new s = gKupac[k][kuCilj];
    NPC_SetSkin(npcid, gKupacSkinovi[random(sizeof gKupacSkinovi)]);
    NPC_SetInvulnerable(npcid, false);
    NPC_SetHealth(npcid, 100.0);
    NPC_SetWeapon(npcid, WEAPON_FIST);
    gKupac[k][kuNapadac] = INVALID_PLAYER_ID;
    gKupac[k][kuNapadacRacun] = 0;
    NPC_SetPos(npcid, gKupac[k][kuX], gKupac[k][kuY], gKupac[k][kuZ]);
    gKupac[k][kuPokusaj] = 0;
    if (s < 0 || !gStand[s][snIma])
    {
        KupacMakni(k);
        return;
    }
    gKupac[k][kuTx] = gStand[s][snKx];
    gKupac[k][kuTy] = gStand[s][snKy];
    gKupac[k][kuTz] = gStand[s][snKz];
    SetTimerEx("KupacKreni", KUPAC_KRENI_MS, false, "ii", k, npcid);
}

forward KupacKreni(k, npcid);
public KupacKreni(k, npcid)
{
    if (gKupac[k][kuNpc] != npcid || gKupac[k][kuStanje] != KU_SPAWN) return;
    if (!KupacIdi(k, gKupac[k][kuTx], gKupac[k][kuTy], gKupac[k][kuTz])) KupacMakni(k);
}

static KupacNaCilju(k)
{
    if (gKupac[k][kuPut] != -1)
    {
        NPC_StopMove(gKupac[k][kuNpc]);
        NPC_DestroyPath(gKupac[k][kuPut]);
        gKupac[k][kuPut] = -1;
    }
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

public OnNPCFinishMovePath(npcid, pathid)
{
    if (PlaninaPutGotov(npcid, pathid)) return 1;
    new k = gNpcKupac[npcid] - 1;
    if (k < 0 || gKupac[k][kuPut] != pathid || gKupac[k][kuStanje] != KU_HODA) return 1;
    KupacNaCilju(k);
    return 1;
}

KupacSmrt(npcid, killerid)
{
    new k = gNpcKupac[npcid] - 1;
    if (k < 0) return;
    if (killerid != INVALID_PLAYER_ID && IsPlayerConnected(killerid) && !IsPlayerNPC(killerid))
    {
        DodajWanted(killerid);
        DodajWanted(killerid);
    }
    KupacMakni(k);
}

static KupacPrekiniPosao(k)
{
    if (gKupac[k][kuPut] != -1)
    {
        NPC_StopMove(gKupac[k][kuNpc]);
        NPC_DestroyPath(gKupac[k][kuPut]);
        gKupac[k][kuPut] = -1;
    }
    for (new s = 0; s < STAND_MAX; s++)
    {
        if (!gStand[s][snIma] || gStand[s][snKupac] != k) continue;
        new p = StandIgracRadi(s);
        if (p != INVALID_PLAYER_ID && gStIgra[p] && gStIgraS[p] == s) StandIgraKraj(p, false);
        else gStand[s][snKupac] = -1;
    }
}

static KupacNastavi(k)
{
    gKupac[k][kuStanje] = KU_HODA;
    NPC_StopMeleeAttack(gKupac[k][kuNpc]);
    NPC_ClearAnimations(gKupac[k][kuNpc]);
    if (!KupacIdi(k, gKupac[k][kuTx], gKupac[k][kuTy], gKupac[k][kuTz])) KupacMakni(k);
}

static KupacRanjen(k)
{
    new npc = gKupac[k][kuNpc], Float:x, Float:y, Float:z;
    KupacPrekiniPosao(k);
    NPC_StopMeleeAttack(npc);
    NPC_SetHealth(npc, KUPAC_RANJEN_HP);
    NPC_SetInvulnerable(npc, true);
    NPC_StopMove(npc);
    NPC_GetPos(npc, x, y, z);
    new Float:hx, Float:hy, Float:hz;
    if (Kolizija_Zraka(x, y, z + 1.0, x, y, z - 50.0, hx, hy, hz)) NPC_SetPos(npc, x, y, hz + 1.0);
    NPC_ApplyAnimation(npc, "CRACK", "crckidle1", 4.1, true, false, false, true, 0);
    gKupac[k][kuStanje] = KU_RANJEN;
    gKupac[k][kuStanjeOd] = gettime();
    NPC_GetPos(npc, x, y, z);
    if (gettime() >= gKupac[k][kuNovacDo])
    {
        gKupac[k][kuNovacDo] = gettime() + KUPAC_NOVAC_PAUZA_S;
        BaciNovac(x + float(random(11) - 5) / 10.0, y + float(random(11) - 5) / 10.0, z, 0, 0, KUPAC_NOVAC_MIN + random(KUPAC_NOVAC_MAX - KUPAC_NOVAC_MIN + 1));
    }
    HitnaZaNpc(npc, x, y, z);
}

KupacJeRanjen(npcid)
{
    if (npcid < 0 || npcid >= MAX_PLAYERS) return 0;
    new k = gNpcKupac[npcid] - 1;
    return k >= 0 && gKupac[k][kuStanje] == KU_RANJEN ? 1 : 0;
}

KupacNajbliziRanjen(playerid, Float:domet)
{
    new best = INVALID_PLAYER_ID, Float:bd = domet, Float:x, Float:y, Float:z;
    for (new k = 0; k < KUPAC_MAX; k++)
    {
        if (gKupac[k][kuStanje] != KU_RANJEN) continue;
        NPC_GetPos(gKupac[k][kuNpc], x, y, z);
        new Float:d = GetPlayerDistanceFromPoint(playerid, x, y, z);
        if (d < bd)
        {
            bd = d;
            best = gKupac[k][kuNpc];
        }
    }
    return best;
}

KupacOzivi(npcid, bool:spasen)
{
    new k = gNpcKupac[npcid] - 1;
    if (k < 0 || gKupac[k][kuStanje] != KU_RANJEN) return;
    if (!spasen)
    {
        KupacMakni(k);
        return;
    }
    NPC_ClearAnimations(npcid);
    NPC_SetInvulnerable(npcid, false);
    NPC_SetHealth(npcid, 100.0);
    gKupac[k][kuCilj] = -1;
    new Float:x, Float:y, Float:z;
    if (RestoranBroj() && RestoranPozicija(random(RestoranBroj()), x, y, z) && KupacIdi(k, x, y, z)) return;
    KupacMakni(k);
}

public OnNPCTakeDamage(npcid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
    new k = gNpcKupac[npcid] - 1;
    if (k < 0) return 1;
    if (gKupac[k][kuStanje] == KU_RANJEN) return 0;
    if (issuerid != INVALID_PLAYER_ID && IsPlayerConnected(issuerid) && !IsPlayerNPC(issuerid))
    {
        if (gKupac[k][kuNapadacRacun] != gAccountId[issuerid]) DodajWanted(issuerid);
        gKupac[k][kuNapadac] = issuerid;
        gKupac[k][kuNapadacRacun] = gAccountId[issuerid];
    }
    if (NPC_GetHealth(npcid) - amount <= KUPAC_RANJEN_HP)
    {
        KupacRanjen(k);
        return 0;
    }
    if (gKupac[k][kuNapadac] == INVALID_PLAYER_ID || gKupac[k][kuStanje] == KU_BORBA || gKupac[k][kuStanje] == KU_BJEZI) return 1;
    KupacPrekiniPosao(k);
    gKupac[k][kuStanje] = _:weaponid > _:WEAPON_SPRAYCAN || (_:weaponid >= 22 && _:weaponid <= 38) ? KU_BJEZI : KU_BORBA;
    gKupac[k][kuStanjeOd] = gettime();
    return 1;
}

forward KupacIzbjegavaj();
public KupacIzbjegavaj()
{
    for (new k = 0; k < KUPAC_MAX; k++)
    {
        new st = gKupac[k][kuStanje];
        if (st != KU_HODA && st != KU_CEKA) continue;
        new npc = gKupac[k][kuNpc], Float:x, Float:y, Float:z;
        NPC_GetPos(npc, x, y, z);
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        {
            if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue;
            new v = GetPlayerVehicleID(i), Float:vx, Float:vy, Float:vz, Float:px, Float:py, Float:pz;
            GetVehiclePos(v, px, py, pz);
            new Float:dx = x - px, Float:dy = y - py;
            if (dx * dx + dy * dy > 400.0 || floatabs(z - pz) > 3.0) continue;
            GetVehicleVelocity(v, vx, vy, vz);
            new Float:brzina2 = vx * vx + vy * vy;
            if (brzina2 < 0.02) continue;
            new Float:t = (dx * vx + dy * vy) / brzina2;
            if (t <= 0.0 || t > 25.0) continue;
            new Float:bx = dx - vx * t, Float:by = dy - vy * t;
            if (bx * bx + by * by > 6.25) continue;
            new Float:b = VectorSize(bx, by, 0.0);
            if (b < 0.1)
            {
                b = floatsqroot(brzina2);
                bx = -vy / b;
                by = vx / b;
            }
            else
            {
                bx /= b;
                by /= b;
            }
            KupacPrekiniPosao(k);
            gKupac[k][kuStanje] = KU_IZBJEGAVA;
            gKupac[k][kuStanjeOd] = gettime();
            NPC_ApplyAnimation(npc, "PED", "EV_dive", 4.1, false, true, true, false, 0);
            NPC_Move(npc, x + bx * 3.0, y + by * 3.0, z, NPC_MOVE_TYPE_SPRINT);
            break;
        }
    }
}

static KupacBorbaTick(k, sad)
{
    new npc = gKupac[k][kuNpc], st = gKupac[k][kuStanje];
    if (st == KU_RANJEN)
    {
        if (sad - gKupac[k][kuStanjeOd] >= KUPAC_RANJEN_S) KupacMakni(k);
        return;
    }
    if (st == KU_IZBJEGAVA)
    {
        if (sad - gKupac[k][kuStanjeOd] >= KUPAC_IZBJEGNI_S) KupacNastavi(k);
        return;
    }
    new p = gKupac[k][kuNapadac], Float:x, Float:y, Float:z, Float:px, Float:py, Float:pz;
    if (p == INVALID_PLAYER_ID || !IsPlayerConnected(p) || gAccountId[p] != gKupac[k][kuNapadacRacun] || sad - gKupac[k][kuStanjeOd] >= KUPAC_BORBA_S)
    {
        KupacNastavi(k);
        return;
    }
    NPC_GetPos(npc, x, y, z);
    GetPlayerPos(p, px, py, pz);
    new Float:d = VectorSize(px - x, py - y, 0.0);
    if (d > KUPAC_BORBA_DOMET || IsRanjen(p))
    {
        KupacNastavi(k);
        return;
    }
    if (st == KU_BJEZI)
    {
        NPC_Move(npc, x + (x - px) / d * 15.0, y + (y - py) / d * 15.0, z, NPC_MOVE_TYPE_SPRINT);
        return;
    }
    if (d > 1.2 || IsPlayerInAnyVehicle(p))
    {
        NPC_StopMeleeAttack(npc);
        NPC_MoveToPlayer(npc, p, NPC_MOVE_TYPE_JOG, NPC_MOVE_SPEED_AUTO, 0.8, 250, true);
        return;
    }
    NPC_StopMove(npc);
    NPC_SetFacingAngle(npc, atan2(py - y, px - x) - 90.0);
    NPC_MeleeAttack(npc, 1000);
}

StandTick()
{
    new sad = gettime();
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        new s = gStandRadi[i] - 1;
        if (s < 0) continue;
        new Float:x, Float:y, Float:a;
        StandRadnikMjesto(s, x, y, a);
        if (!gStand[s][snIma] || gStand[s][snVlasnik] != gAccountId[i] || !IsPlayerInRangeOfPoint(i, STAND_RADI_DOMET, x, y, gStand[s][snKz]))
        {
            if (gStIgra[i]) StandIgraKraj(i, false);

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
        new k = gStand[s][snKupac];
        if (k >= 0)
        {
            new ms = GetTickCount() - gStand[s][snKupacOd];
            if (StandIgracRadi(s) == INVALID_PLAYER_ID && ms >= STAND_RADNIK_MS)
            {
                if (StandRadnikRadi(s) && gStand[s][snZaliha] > 0 && random(100) >= STAND_RADNIK_GRESKA)
                {
                    gStand[s][snKasa] += gStandCijene[gStand[s][snVrsta]][random(3)] + (random(100) < STAND_NAPOJNICA_POSTO ? 1 + random(5) : 0);
                    StandProdaja(s);
                }
                StandKupacGotov(s);
            }
            else if (ms >= STAND_IGRA_MS)
            {
                new p = StandIgracRadi(s);
                if (p != INVALID_PLAYER_ID && gStIgra[p] && gStIgraS[p] == s) StandIgraKraj(p, false);
                else StandKupacGotov(s);
            }
        }
    }
    new otvoreno = 0, aktivni = 0;
    for (new s = 0; s < STAND_MAX; s++)
    {
        new bool:otvoren = StandOtvoren(s);
        if (otvoren) otvoreno++;
        if (otvoren && !gStandBio[s]) for (new k = 0; k < KUPAC_NA_OTVARANJE; k++) KupacNovi(s);
        gStandBio[s] = otvoren;
    }
    new ukupno = 0;
    for (new k = 0; k < KUPAC_MAX; k++)
    {
        if (gKupac[k][kuStanje] == KU_NEMA) continue;
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
            NPC_GetPos(gKupac[k][kuNpc], x, y, z);
            if (s >= 0 && gStand[s][snIma] && VectorSize(x - gStand[s][snKx], y - gStand[s][snKy], 0.0) <= KUPAC_DOLAZAK)
            {
                KupacNaCilju(k);
                continue;
            }
            if (VectorSize(x - gKupac[k][kuPx], y - gKupac[k][kuPy], 0.0) < 0.3) gKupac[k][kuMirno]++;
            else
            {
                gKupac[k][kuMirno] = 0;
                gKupac[k][kuPokusaj] = 0;
            }
            gKupac[k][kuPx] = x;
            gKupac[k][kuPy] = y;
            if (sad - gKupac[k][kuOd] >= KUPAC_ZAGLAVIO_S)
            {
                KupacMakni(k);
                continue;
            }
            if (gKupac[k][kuMirno] < KUPAC_MIRNO_S) continue;
            if (++gKupac[k][kuPokusaj] > KUPAC_POKUSAJA || (gKupac[k][kuPokusaj] > 2 && KupacSkriven(x, y, z)))
            {
                KupacMakni(k);
                continue;
            }
            new pokusaj = gKupac[k][kuPokusaj], Float:tx = gKupac[k][kuTx], Float:ty = gKupac[k][kuTy], Float:tz = gKupac[k][kuTz];
            NPC_StopMove(gKupac[k][kuNpc]);
            NPC_ClearAnimations(gKupac[k][kuNpc]);
            if (pokusaj > 1)
            {
                new c = StazaNajbliza(x, y, z), Float:cx, Float:cy, Float:cz;
                StazaTocka(c, cx, cy, cz);
                NPC_SetPos(gKupac[k][kuNpc], cx, cy, HodTlo(cx, cy, cz));
            }
            if (!KupacIdi(k, tx, ty, tz)) KupacMakni(k);
            else gKupac[k][kuPokusaj] = pokusaj;
        }
        else if (gKupac[k][kuStanje] == KU_SPAWN && sad - gKupac[k][kuOd] >= 10) KupacMakni(k);
        else if (gKupac[k][kuStanje] >= KU_BORBA) KupacBorbaTick(k, sad);
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
    if (gStIgra[playerid] && gStIgraTimer[playerid]) KillTimer(gStIgraTimer[playerid]);
    gStIgra[playerid] = SI_NEMA;
    gStIgraTimer[playerid] = 0;
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
    gStand[s][snX] = gStand[s][snKx] - 1.6 * floatsin(a, degrees);
    gStand[s][snY] = gStand[s][snKy] + 1.6 * floatcos(a, degrees);
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
