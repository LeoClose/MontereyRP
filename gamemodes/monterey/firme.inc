#define FIRMA_MAX               32
#define FIRMA_TRGOVINA_CIJENA   25000
#define FIRMA_FRIZER_CIJENA     15000
#define FIRMA_RESTORAN_CIJENA   30000
#define FIRMA_ZALIHA_MAX        50
#define FIRMA_NABAVA            400
#define FIRMA_RADNI_DOMET       3.0
#define FIRMA_GOST_RAZMAK_MIN   8
#define FIRMA_GOST_RAZMAK_MAX   20
#define FIRMA_GOST_HODA_S       25
#define FIRMA_GOST_CEKA_S       60
#define FIRMA_IGRA_S            15
#define FIRMA_GOST_KRENI_MS     700
#define FIRMA_SISANJE_MIN       30
#define FIRMA_SISANJE_RASPON    31
#define POSAO_NAPOJNICA_POSTO   30
#define POSAO_TRAKA_MS          40
#define POSAO_TRAKA_ZELENO      12.0
#define POSAO_SKARE_REZOVA      3
#define POSAO_BOJA              0xE67E22FF

enum { FIRMA_TRGOVINA, FIRMA_FRIZER, FIRMA_PIZZA, FIRMA_BURGER, FIRMA_CLUCKIN, FIRMA_VRSTA }
enum { IZVOR_STAND, IZVOR_FIRMA }
enum { SI_NEMA, SI_TRAKA, SI_TIPKE, SI_KUSUR, SI_SKARE }
enum { FG_NEMA, FG_DOLAZI, FG_CEKA, FG_ODLAZI }
enum { FO_KASA, FO_RADI, FO_ZALIHA }

new const gKupacSkinovi[] = { 7, 14, 15, 20, 22, 23, 24, 26, 29, 35, 37, 44, 46, 47, 48, 56, 59, 60, 72, 73, 93, 98, 101, 141, 150, 151, 170, 184, 185, 188, 190, 192, 193, 211, 214, 216, 217, 223, 225, 226, 227, 233, 240, 250, 261, 289, 290, 291 };

static const KEY:gStTipke[] = { KEY_YES, KEY_NO, KEY_WALK, KEY_SPRINT };
static const gStTipkeIme[][] = { "Y", "N", "ALT", "SPACE" };
static const gStNovcanice[] = { 5, 10, 20, 50, 100 };
static gStIgra[MAX_PLAYERS], gStIgraIzvor[MAX_PLAYERS], gStIgraId[MAX_PLAYERS], gStIgraCijena[MAX_PLAYERS], gStIgraOdg[MAX_PLAYERS], gStIgraTipke[MAX_PLAYERS][4], gStIgraKorak[MAX_PLAYERS], gStIgraTimer[MAX_PLAYERS], gStIgraZelja[MAX_PLAYERS][32];
static Float:gStIgraTraka[MAX_PLAYERS], Float:gStIgraSmjer[MAX_PLAYERS], Float:gStIgraZel[MAX_PLAYERS];

enum E_FIRMA { bool:fIma, fDb, fVlasnik, fVlasnikIme[MAX_PLAYER_NAME + 1], fKasa, fZaliha, fRadnik, fGost, fGostFaza, fGostOd, fSljedeci, fCijena, fRoba[32] }
static gFirma[FIRMA_VRSTA][FIRMA_MAX][E_FIRMA];
static gFirmaMeni[MAX_PLAYERS][2], gFirmaOpcija[MAX_PLAYERS][3], gFirmaOpcijaN[MAX_PLAYERS];
static const gFirmaIme[FIRMA_VRSTA][] = { "Trgovina 24/7", "Frizerski salon", "Well Stacked Pizza", "Burger Shot", "Cluckin' Bell" };
static const gFirmaCijena[FIRMA_VRSTA] = { FIRMA_TRGOVINA_CIJENA, FIRMA_FRIZER_CIJENA, FIRMA_RESTORAN_CIJENA, FIRMA_RESTORAN_CIJENA, FIRMA_RESTORAN_CIJENA };

InitFirme()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS business_owners (kind INTEGER NOT NULL, place INTEGER NOT NULL, owner INTEGER NOT NULL DEFAULT 0, owner_name TEXT NOT NULL DEFAULT '', till INTEGER NOT NULL DEFAULT 0, stock INTEGER NOT NULL DEFAULT 0, PRIMARY KEY (kind, place))"));
    new zaliha = 2, kasa = 0;
    FirmaPotrosi(zaliha, kasa);
    assert(zaliha == 1 && kasa == 0);
    FirmaPotrosi(zaliha, kasa);
    assert(zaliha == FIRMA_ZALIHA_MAX && kasa == -FIRMA_NABAVA);
}

static FirmaPotrosi(&zaliha, &kasa)
{
    if (--zaliha > 0) return;
    zaliha = FIRMA_ZALIHA_MAX;
    kasa -= FIRMA_NABAVA;
}

FirmaUcitaj(k, s, db)
{
    gFirma[k][s][fIma] = true;
    gFirma[k][s][fDb] = db;
    gFirma[k][s][fVlasnik] = 0;
    gFirma[k][s][fVlasnikIme][0] = EOS;
    gFirma[k][s][fKasa] = 0;
    gFirma[k][s][fZaliha] = FIRMA_ZALIHA_MAX;
    gFirma[k][s][fRadnik] = INVALID_PLAYER_ID;
    gFirma[k][s][fGost] = INVALID_LIK;
    gFirma[k][s][fGostFaza] = FG_NEMA;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT owner, owner_name, till, stock FROM business_owners WHERE kind = %d AND place = %d", k, db);
    if (DB_GetRowCount(res))
    {
        gFirma[k][s][fVlasnik] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, gFirma[k][s][fVlasnikIme], MAX_PLAYER_NAME + 1);
        gFirma[k][s][fKasa] = DB_GetFieldInt(res, 2);
        gFirma[k][s][fZaliha] = DB_GetFieldInt(res, 3);
    }
    DB_FreeResultSet(res);
}

static FirmaSpremi(k, s)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO business_owners (kind, place, owner, owner_name, till, stock) VALUES (%d, %d, %d, '%q', %d, %d)",
        k, gFirma[k][s][fDb], gFirma[k][s][fVlasnik], gFirma[k][s][fVlasnikIme], gFirma[k][s][fKasa], gFirma[k][s][fZaliha]));
}

FirmaMakni(k, s)
{
    if (!gFirma[k][s][fIma]) return;
    FirmaPrestani(k, s);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM business_owners WHERE kind = %d AND place = %d", k, gFirma[k][s][fDb]));
    gFirma[k][s][fIma] = false;
}

FirmaProdaja(k, s, iznos, bool:roba)
{
    if (s < 0 || s >= FIRMA_MAX || !gFirma[k][s][fIma] || !gFirma[k][s][fVlasnik]) return;
    gFirma[k][s][fKasa] += iznos;
    if (roba && k == FIRMA_TRGOVINA) FirmaPotrosi(gFirma[k][s][fZaliha], gFirma[k][s][fKasa]);
    FirmaSpremi(k, s);
}

bool:FirmaImaVlasnika(k, s)
{
    return s >= 0 && s < FIRMA_MAX && gFirma[k][s][fIma] && gFirma[k][s][fVlasnik] != 0;
}

FirmaGorivo(db, iznos)
{
    if (db) FirmaProdaja(FIRMA_TRGOVINA, TrgovinaPoDb(db), iznos, false);
}

FirmaStolicaZauzeta(s)
{
    return gFirma[FIRMA_FRIZER][s][fIma] && (gFirma[FIRMA_FRIZER][s][fGostFaza] == FG_DOLAZI || gFirma[FIRMA_FRIZER][s][fGostFaza] == FG_CEKA);
}

static FirmaTocke(k, s, &interijer, &svijet, Float:ulaz[3], Float:cilj[4], Float:radnik[3])
{
    if (k >= FIRMA_PIZZA) return 0;
    return k == FIRMA_TRGOVINA ? TrgovinaTocke(s, interijer, svijet, ulaz, cilj, radnik) : OdjecaTocke(s, interijer, svijet, ulaz, cilj, radnik);
}

static FirmaUnutra(playerid, k, s)
{
    return (k == FIRMA_TRGOVINA ? TrgovinaIgraca(playerid) : k >= FIRMA_PIZZA ? RestoranIgraca(playerid) : OdjecaFrizerIgraca(playerid)) == s;
}

static FirmaGdje(playerid, &k, &s)
{
    s = TrgovinaIgraca(playerid);
    k = FIRMA_TRGOVINA;
    if (s == -1)
    {
        s = OdjecaFrizerIgraca(playerid);
        k = FIRMA_FRIZER;
    }
    if (s == -1)
    {
        s = RestoranIgraca(playerid);
        k = FIRMA_PIZZA + RestoranVrsta(s);
    }
    return s != -1 && gFirma[k][s][fIma];
}

static FirmaUPultu(playerid, k, s)
{
    new interijer, svijet, Float:u[3], Float:c[4], Float:r[3];
    return FirmaTocke(k, s, interijer, svijet, u, c, r) && IsPlayerInRangeOfPoint(playerid, FIRMA_RADNI_DOMET, r[0], r[1], r[2]);
}

CMD:firma(playerid, params[])
{
    new k, s;
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    if (!FirmaGdje(playerid, k, s)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti u trgovini 24/7, frizerskom salonu ili restoranu."));
    gFirmaMeni[playerid][0] = k;
    gFirmaMeni[playerid][1] = s;
    if (!gFirma[k][s][fVlasnik])
    {
        new body[128];
        format(body, sizeof body, T(playerid, "Kupiti ovu firmu (%s) za $%d?"), T(playerid, gFirmaIme[k]), gFirmaCijena[k]);
        return PrikaziDialog(playerid, DLG_FIRMA_KUPI, DIALOG_STYLE_MSGBOX, T(playerid, gFirmaIme[k]), body, T(playerid, "Kupi"), T(playerid, "Zatvori"));
    }
    if (gFirma[k][s][fVlasnik] != gAccountId[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ova firma vec ima vlasnika."));
    return FirmaMeni(playerid, k, s);
}

CMD:business(playerid, params[]) return cmd_firma(playerid, params);

static FirmaMeni(playerid, k, s)
{
    new list[384], red[112], len = 0, n = 0;
    gFirmaOpcija[playerid][n++] = FO_KASA;
    if (gFirma[k][s][fKasa] < 0) format(red, sizeof red, T(playerid, "Dug za nabavu robe: $%d"), -gFirma[k][s][fKasa]);
    else format(red, sizeof red, T(playerid, "Uzmi zaradu ($%d)"), gFirma[k][s][fKasa]);
    DodajStupac(list, len, red);
    if (k < FIRMA_PIZZA)
    {
        gFirmaOpcija[playerid][n++] = FO_RADI;
        DodajStupac(list, len, gFirma[k][s][fRadnik] == playerid ? T(playerid, "Prestani raditi") : T(playerid, "Pocni raditi"));
    }
    if (k == FIRMA_TRGOVINA)
    {
        gFirmaOpcija[playerid][n++] = FO_ZALIHA;
        format(red, sizeof red, T(playerid, "Zaliha: %d/%d (automatska nabava $%d)"), gFirma[k][s][fZaliha], FIRMA_ZALIHA_MAX, FIRMA_NABAVA);
        DodajStupac(list, len, red);
    }
    gFirmaOpcijaN[playerid] = n;
    return PrikaziDialog(playerid, DLG_FIRMA, DIALOG_STYLE_LIST, T(playerid, gFirmaIme[k]), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static FirmaMoja(playerid)
{
    new k, s;
    if (!FirmaGdje(playerid, k, s) || k != gFirmaMeni[playerid][0] || s != gFirmaMeni[playerid][1]) return -1;
    return k * FIRMA_MAX + s;
}

DLG:firma_kupi(playerid, response)
{
    new id = FirmaMoja(playerid);
    if (!response || id == -1) return 1;
    new k = id / FIRMA_MAX, s = id % FIRMA_MAX;
    if (gFirma[k][s][fVlasnik]) return ObavijestGreska(playerid, T(playerid, "Ova firma vec ima vlasnika."));
    for (new i = 0; i < FIRMA_MAX; i++)
        if (gFirma[k][i][fIma] && gFirma[k][i][fVlasnik] == gAccountId[playerid]) return ObavijestGreska(playerid, T(playerid, "Vec imate firmu ove vrste."));
    if (!DajNovac(playerid, -gFirmaCijena[k])) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
    gFirma[k][s][fVlasnik] = gAccountId[playerid];
    GetPlayerName(playerid, gFirma[k][s][fVlasnikIme], MAX_PLAYER_NAME + 1);
    gFirma[k][s][fKasa] = 0;
    gFirma[k][s][fZaliha] = FIRMA_ZALIHA_MAX;
    FirmaSpremi(k, s);
    return ObavijestInfo(playerid, T(playerid, gFirmaIme[k]), T(playerid, "Kupili ste firmu. Upravljajte njome naredbom /firma."));
}

DLG:firma(playerid, response, listitem)
{
    new id = FirmaMoja(playerid);
    if (!response || id == -1 || listitem < 0 || listitem >= gFirmaOpcijaN[playerid]) return 1;
    new k = id / FIRMA_MAX, s = id % FIRMA_MAX, tekst[128];
    if (gFirma[k][s][fVlasnik] != gAccountId[playerid]) return 1;
    switch (gFirmaOpcija[playerid][listitem])
    {
        case FO_KASA:
        {
            new iznos = gFirma[k][s][fKasa];
            if (iznos < 0)
            {
                format(tekst, sizeof tekst, T(playerid, "Firma duguje $%d za nabavu robe. Dug se pokriva iz iduce zarade."), -iznos);
                return ObavijestGreska(playerid, tekst);
            }
            if (!iznos) return ObavijestGreska(playerid, T(playerid, "U kasi nema novca."));
            if (!DajNovac(playerid, iznos)) return ObavijestGreska(playerid, T(playerid, "Ne mozete primiti toliko novca."));
            gFirma[k][s][fKasa] = 0;
            FirmaSpremi(k, s);
            format(tekst, sizeof tekst, T(playerid, "Uzeli ste $%d iz kase."), iznos);
            return ObavijestInfo(playerid, T(playerid, gFirmaIme[k]), tekst);
        }
        case FO_RADI:
        {
            if (gFirma[k][s][fRadnik] == playerid)
            {
                FirmaPrestani(k, s);
                return ObavijestInfo(playerid, T(playerid, gFirmaIme[k]), T(playerid, "Prestali ste raditi."));
            }
            if (!FirmaUPultu(playerid, k, s)) return ObavijestGreska(playerid, k == FIRMA_FRIZER ? T(playerid, "Stanite kraj frizerske stolice.") : T(playerid, "Stanite za pult kraj blagajnika."));
            gFirma[k][s][fRadnik] = playerid;
            gFirma[k][s][fSljedeci] = gettime() + 3;
            return ObavijestInfo(playerid, T(playerid, gFirmaIme[k]), T(playerid, "Radite. Musterije dolaze jedna po jedna. Ako izadete van, prestajete raditi."));
        }
        case FO_ZALIHA:
        {
            format(tekst, sizeof tekst, T(playerid, "Kad zaliha padne na 0, roba se sama naruci, a $%d se skida iz kase."), FIRMA_NABAVA);
            return ObavijestInfo(playerid, T(playerid, gFirmaIme[k]), tekst);
        }
    }
    return 1;
}

static FirmaGostKaze(k, s, const tekst[])
{
    new lik = gFirma[k][s][fGost], Float:x, Float:y, Float:z, interijer, svijet, Float:u[3], Float:c[4], Float:r[3];
    if (lik == INVALID_LIK || !FirmaTocke(k, s, interijer, svijet, u, c, r)) return;
    Lik_Pozicija(lik, x, y, z);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUlogiran[i] && GetPlayerVirtualWorld(i) == svijet && IsPlayerInRangeOfPoint(i, 15.0, x, y, z)) SendClientMessage(i, COLOR_CHAT, T(i, "Kupac kaze: %s"), T(i, tekst));
}

static FirmaGostMakni(k, s)
{
    if (gFirma[k][s][fGost] != INVALID_LIK) Lik_Ukloni(gFirma[k][s][fGost]);
    gFirma[k][s][fGost] = INVALID_LIK;
    gFirma[k][s][fGostFaza] = FG_NEMA;
    gFirma[k][s][fSljedeci] = gettime() + FIRMA_GOST_RAZMAK_MIN + random(FIRMA_GOST_RAZMAK_MAX - FIRMA_GOST_RAZMAK_MIN + 1);
}

static FirmaGostOdlazi(k, s)
{
    new lik = gFirma[k][s][fGost], interijer, svijet, Float:u[3], Float:c[4], Float:r[3];
    if (lik == INVALID_LIK || !FirmaTocke(k, s, interijer, svijet, u, c, r))
    {
        FirmaGostMakni(k, s);
        return;
    }
    gFirma[k][s][fGostFaza] = FG_ODLAZI;
    gFirma[k][s][fGostOd] = gettime();
    Lik_OcistiAnimaciju(lik);
    Lik_Idi(lik, u[0], u[1], u[2], LIK_HOD);
}

static FirmaGostNovi(k, s)
{
    new interijer, svijet, Float:u[3], Float:c[4], Float:r[3];
    gFirma[k][s][fSljedeci] = gettime() + 5;
    if (!FirmaTocke(k, s, interijer, svijet, u, c, r) || (k == FIRMA_FRIZER && OdjecaStolicaZauzeta(s))) return;
    new lik = Lik_Stvori(gKupacSkinovi[random(sizeof gKupacSkinovi)], u[0], u[1], u[2], 0.0, svijet, interijer);
    if (lik == INVALID_LIK) return;
    CivilPostavi(lik, interijer != 0 || svijet != 0);
    gFirma[k][s][fGost] = lik;
    gFirma[k][s][fGostFaza] = FG_DOLAZI;
    gFirma[k][s][fGostOd] = gettime();
    SetTimerEx("FirmaGostKreni", FIRMA_GOST_KRENI_MS, false, "ii", k * FIRMA_MAX + s, lik);
}

forward FirmaGostKreni(id, lik);
public FirmaGostKreni(id, lik)
{
    new k = id / FIRMA_MAX, s = id % FIRMA_MAX, interijer, svijet, Float:u[3], Float:c[4], Float:r[3];
    if (gFirma[k][s][fGost] != lik || gFirma[k][s][fGostFaza] != FG_DOLAZI) return;
    if (!FirmaTocke(k, s, interijer, svijet, u, c, r)) FirmaGostMakni(k, s);
    else Lik_Idi(lik, c[0], c[1], c[2], LIK_HOD);
}

static FirmaPosluzi(k, s)
{
    new p = gFirma[k][s][fRadnik];
    if (p == INVALID_PLAYER_ID || gStIgra[p] || !FirmaUPultu(p, k, s)) return;
    new igra = k == FIRMA_FRIZER ? SI_SKARE : random(2) ? SI_TIPKE : SI_KUSUR;
    if (k == FIRMA_FRIZER) ApplyAnimation(p, "HAIRCUTS", "BRB_Cut", 4.1, true, false, false, false, 0, SYNC_ALL);
    gFirma[k][s][fGostOd] = gettime();
    FirmaGostKaze(k, s, gFirma[k][s][fRoba]);
    PosaoIgraPocni(p, IZVOR_FIRMA, k * FIRMA_MAX + s, igra, gFirma[k][s][fRoba], gFirma[k][s][fCijena]);
}

static FirmaGostNaCilju(k, s)
{
    new lik = gFirma[k][s][fGost], interijer, svijet, Float:u[3], Float:c[4], Float:r[3];
    if (!FirmaTocke(k, s, interijer, svijet, u, c, r))
    {
        FirmaGostMakni(k, s);
        return;
    }
    Lik_Stani(lik);
    Lik_Postavi(lik, c[0], c[1], c[2]);
    Lik_Kut(lik, c[3]);
    if (k == FIRMA_FRIZER)
    {
        Lik_Animacija(lik, "HAIRCUTS", "BRB_Sit_Loop", 4.1, 1, 0, 0, 1, 0);
        strcopy(gFirma[k][s][fRoba], "Sisanje", 32);
        gFirma[k][s][fCijena] = FIRMA_SISANJE_MIN + random(FIRMA_SISANJE_RASPON);
    }
    else
    {
        new item, cijena;
        TrgovinaNasumicnaRoba(item, cijena);
        strcopy(gFirma[k][s][fRoba], gItems[item][ITEM_NAME], 32);
        gFirma[k][s][fCijena] = cijena;
    }
    gFirma[k][s][fGostFaza] = FG_CEKA;
    gFirma[k][s][fGostOd] = gettime();
    FirmaPosluzi(k, s);
}

FirmaStigao(lik)
{
    for (new k = 0; k < sizeof gFirma; k++) for (new s = 0; s < FIRMA_MAX; s++)
    {
        if (gFirma[k][s][fGost] != lik) continue;
        if (gFirma[k][s][fGostFaza] == FG_DOLAZI) FirmaGostNaCilju(k, s);
        else if (gFirma[k][s][fGostFaza] == FG_ODLAZI) FirmaGostMakni(k, s);
        return 1;
    }
    return 0;
}

FirmaIgraGotova(playerid, id, bool:uspjeh, cijena)
{
    new k = id / FIRMA_MAX, s = id % FIRMA_MAX;
    if (k == FIRMA_FRIZER) ClearAnimations(playerid, SYNC_ALL);
    if (!gFirma[k][s][fIma] || gFirma[k][s][fGostFaza] != FG_CEKA) return;
    if (uspjeh)
    {
        new napojnica = random(100) < POSAO_NAPOJNICA_POSTO ? 1 + random(5) : 0, tekst[96];
        DajNovac(playerid, cijena + napojnica);
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
        if (k == FIRMA_TRGOVINA) FirmaProdaja(k, s, 0, true);
        if (napojnica) format(tekst, sizeof tekst, T(playerid, "Kupac je platio $%d i ostavio napojnicu od $%d."), cijena, napojnica);
        else format(tekst, sizeof tekst, T(playerid, "Kupac je platio $%d."), cijena);
        ObavijestInfo(playerid, T(playerid, gFirmaIme[k]), tekst);
        FirmaGostKaze(k, s, "Hvala, dovidenja!");
    }
    else ObavijestGreska(playerid, T(playerid, "Kupac je otisao nezadovoljan."));
    FirmaGostOdlazi(k, s);
}

static FirmaPrestani(k, s)
{
    new p = gFirma[k][s][fRadnik];
    if (p != INVALID_PLAYER_ID && gStIgra[p] && gStIgraIzvor[p] == IZVOR_FIRMA && gStIgraId[p] == k * FIRMA_MAX + s) PosaoIgraKraj(p, false);
    gFirma[k][s][fRadnik] = INVALID_PLAYER_ID;
    if (gFirma[k][s][fGost] != INVALID_LIK) FirmaGostMakni(k, s);
}

FirmaTick()
{
    new sad = gettime();
    for (new k = 0; k < FIRMA_VRSTA; k++) for (new s = 0; s < FIRMA_MAX; s++)
    {
        new p = gFirma[k][s][fRadnik];
        if (!gFirma[k][s][fIma] || p == INVALID_PLAYER_ID) continue;
        if (!IsPlayerConnected(p) || !FirmaUnutra(p, k, s))
        {
            FirmaPrestani(k, s);
            if (IsPlayerConnected(p)) ObavijestInfo(p, T(p, gFirmaIme[k]), T(p, "Izasli ste iz firme i prestali raditi."));
            continue;
        }
        switch (gFirma[k][s][fGostFaza])
        {
            case FG_NEMA: if (sad >= gFirma[k][s][fSljedeci]) FirmaGostNovi(k, s);
            case FG_DOLAZI: if (sad - gFirma[k][s][fGostOd] >= FIRMA_GOST_HODA_S) FirmaGostNaCilju(k, s);
            case FG_CEKA:
            {
                if (gStIgra[p]) { if (sad - gFirma[k][s][fGostOd] >= FIRMA_IGRA_S) PosaoIgraKraj(p, false); }
                else if (sad - gFirma[k][s][fGostOd] >= FIRMA_GOST_CEKA_S) FirmaGostOdlazi(k, s);
                else FirmaPosluzi(k, s);
            }
            case FG_ODLAZI: if (sad - gFirma[k][s][fGostOd] >= FIRMA_GOST_HODA_S) FirmaGostMakni(k, s);
        }
    }
}

FirmaDisconnect(playerid)
{
    for (new k = 0; k < FIRMA_VRSTA; k++) for (new s = 0; s < FIRMA_MAX; s++)
        if (gFirma[k][s][fIma] && gFirma[k][s][fRadnik] == playerid) FirmaPrestani(k, s);
    if (gStIgraTimer[playerid]) KillTimer(gStIgraTimer[playerid]);
    gStIgraTimer[playerid] = 0;
    gStIgra[playerid] = SI_NEMA;
}

static PosaoIgraPanel(playerid)
{
    new naslov[96], tekst[192], red[128];
    format(naslov, sizeof naslov, T(playerid, "Kupac zeli: %s ($%d)"), T(playerid, gStIgraZelja[playerid]), gStIgraCijena[playerid]);
    switch (gStIgra[playerid])
    {
        case SI_TRAKA: strcat(tekst, T(playerid, "Pritisnite ~y~SPACE~w~ kad je oznaka u zelenom."));
        case SI_SKARE: format(tekst, sizeof tekst, T(playerid, "Sisajte: ~y~SPACE~w~ kad je oznaka u zelenom (%d/%d)."), gStIgraKorak[playerid] + 1, POSAO_SKARE_REZOVA);
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
    IgraPanel(playerid, naslov, 60.0, POSAO_BOJA);
    if (gStIgra[playerid] != SI_TRAKA && gStIgra[playerid] != SI_SKARE)
    {
        IgraTekst(playerid, tekst);
        return;
    }
    IgraTraka(playerid, gStIgraZel[playerid], gStIgraZel[playerid] + POSAO_TRAKA_ZELENO, 0x3A3A3AFF, 0x27AE60FF, 0x3A3A3AFF, tekst);
    IgraOznakaPomakni(playerid, gStIgraTraka[playerid]);
}

PosaoIgraPocni(playerid, izvor, id, igra, const zelja[], cijena)
{
    new opcije[sizeof gStNovcanice], n = 0;
    for (new i = 0; i < sizeof gStNovcanice; i++) if (gStNovcanice[i] > cijena) opcije[n++] = gStNovcanice[i];
    if (igra == SI_KUSUR && !n) igra = SI_TIPKE;
    gStIgraIzvor[playerid] = izvor;
    gStIgraId[playerid] = id;
    gStIgraCijena[playerid] = cijena;
    gStIgraKorak[playerid] = 0;
    gStIgra[playerid] = igra;
    strcopy(gStIgraZelja[playerid], zelja, sizeof gStIgraZelja[]);
    switch (igra)
    {
        case SI_TRAKA, SI_SKARE:
        {
            gStIgraTraka[playerid] = 0.0;
            gStIgraSmjer[playerid] = 3.5;
            gStIgraZel[playerid] = float(25 + random(55));
            gStIgraTimer[playerid] = SetTimerEx("PosaoTrakaKorak", POSAO_TRAKA_MS, true, "i", playerid);
        }
        case SI_TIPKE: for (new i = 0; i < 4; i++) gStIgraTipke[playerid][i] = random(sizeof gStTipke);
        case SI_KUSUR: gStIgraOdg[playerid] = opcije[random(n)] - cijena;
    }
    PosaoIgraPanel(playerid);
}

forward PosaoTrakaKorak(playerid);
public PosaoTrakaKorak(playerid)
{
    if (gStIgra[playerid] != SI_TRAKA && gStIgra[playerid] != SI_SKARE) return;
    gStIgraTraka[playerid] += gStIgraSmjer[playerid];
    if (gStIgraTraka[playerid] >= 100.0 || gStIgraTraka[playerid] <= 0.0) gStIgraSmjer[playerid] = -gStIgraSmjer[playerid];
    IgraOznakaPomakni(playerid, gStIgraTraka[playerid]);
}

bool:PosaoIgraZa(playerid, izvor, id)
{
    return gStIgra[playerid] != SI_NEMA && gStIgraIzvor[playerid] == izvor && gStIgraId[playerid] == id;
}

PosaoIgraKraj(playerid, bool:uspjeh)
{
    new igra = gStIgra[playerid];
    if (!igra) return;
    gStIgra[playerid] = SI_NEMA;
    if (igra == SI_TRAKA || igra == SI_SKARE)
    {
        if (gStIgraTimer[playerid]) KillTimer(gStIgraTimer[playerid]);
        gStIgraTimer[playerid] = 0;
    }
    IgraTdMakni(playerid);
    if (gStIgraIzvor[playerid] == IZVOR_STAND) StandIgraGotova(playerid, gStIgraId[playerid], uspjeh, gStIgraCijena[playerid]);
    else FirmaIgraGotova(playerid, gStIgraId[playerid], uspjeh, gStIgraCijena[playerid]);
}

PosaoIgraKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (gStIgra[playerid] == SI_TRAKA || gStIgra[playerid] == SI_SKARE)
    {
        if (!(newkeys & KEY_SPRINT) || (oldkeys & KEY_SPRINT)) return 0;
        new bool:pogodak = gStIgraTraka[playerid] >= gStIgraZel[playerid] && gStIgraTraka[playerid] <= gStIgraZel[playerid] + POSAO_TRAKA_ZELENO;
        if (gStIgra[playerid] == SI_SKARE && pogodak && ++gStIgraKorak[playerid] < POSAO_SKARE_REZOVA)
        {
            gStIgraSmjer[playerid] *= 1.3;
            gStIgraZel[playerid] = float(10 + random(70));
            PosaoIgraPanel(playerid);
            return 1;
        }
        PosaoIgraKraj(playerid, pogodak);
        return 1;
    }
    if (gStIgra[playerid] != SI_TIPKE) return 0;
    new pritisnuta = -1;
    for (new i = 0; i < sizeof gStTipke; i++) if ((newkeys & gStTipke[i]) && !(oldkeys & gStTipke[i])) pritisnuta = i;
    if (pritisnuta == -1) return 0;
    if (pritisnuta != gStIgraTipke[playerid][gStIgraKorak[playerid]])
    {
        PosaoIgraKraj(playerid, false);
        return 1;
    }
    if (++gStIgraKorak[playerid] >= 4) PosaoIgraKraj(playerid, true);
    else PosaoIgraPanel(playerid);
    return 1;
}

PosaoIgraChat(playerid, const text[])
{
    if (gStIgra[playerid] != SI_KUSUR || !ProvjeriBroj(text) || strlen(text) > 4) return 0;
    PosaoIgraKraj(playerid, strval(text) == gStIgraOdg[playerid]);
    return 1;
}
