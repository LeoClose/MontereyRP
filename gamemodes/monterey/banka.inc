#define BANKA_MAX           16
#define BANKA_SVIJET        9600
#define BANKA_DOMET         2.0
#define BANKA_KASA_DOMET    4.0
#define BANKA_SALTERI       4
#define BANKA_SKIN          150
#define BANKA_OTVARANJE     100
#define BANKA_KARTICA       50
#define BANKA_KREDIT_MAX    50000
#define BANKA_KREDIT_SATI   10
#define BANKA_KAMATA        10
#define BANKA_RATA          10
#define BANKA_KAZNA         5
#define BANKA_DAN           86400
#define BANKA_STEDNJA_MAX   1000
#define BANKA_CUVARI        4
#define BANKA_CUVAR_SKIN    71
#define PLJ_EKSPLOZIVA      6
#define PLJ_SEFOVA          12
#define PLJ_PUT             16
#define PLJ_PODIZNA         4
#define PLJ_MENADZER_SKIN   147
#define PLJ_VRATA_S         4.0
#define PLJ_VRATA_KORACI    12
#define PLJ_DOMET           1.6
#define PLJ_VRECA_SLOT      0
#define PLJ_MAFIJA          1

enum E_BANKA { bool:bkIma, bkDb, bkInt, Float:bkX, Float:bkY, Float:bkZ, Float:bkA, Float:bkIx, Float:bkIy, Float:bkIz, bkPljacka, bkPljackaKraj, bkPljackano,
    STREAMER_TAG_PICKUP:bkPickup, STREAMER_TAG_PICKUP:bkPickupU, STREAMER_TAG_3D_TEXT_LABEL:bkTekst, STREAMER_TAG_3D_TEXT_LABEL:bkTekstU, bkIkona }

enum E_BANKA_INT { biIme[24], biInt, Float:biX, Float:biY, Float:biZ }
static const gBankaInt[][E_BANKA_INT] = {
    { "Palomino Creek banka", 0, 2306.380, -15.240, 26.750 },
    { "Gradski ured", 3, 384.808, 173.804, 1008.382 },
    { "Atrium", 18, 1727.285, -1642.945, 20.225 },
    { "Fleeca banka", 0, 2050.0, 2042.5, 1000.05 },
    { "Gradska banka", 0, 1397.334, -454.192, -76.242 }
};

enum { BA_OTVORI, BA_INFO, BA_UPLATI, BA_ISPLATI, BA_NA_STEDNJU, BA_SA_STEDNJE, BA_POSALJI, BA_KREDIT, BA_OTPLATI, BA_KARTICA }
static const gBankaAkcija[][] = { "Otvori racun ($%d)", "Informacije", "Uplati novac", "Podigni novac", "Prebaci na stednju", "Podigni sa stednje", "Posalji novac", "Podigni kredit", "Otplati kredit", "Nova kartica ($%d)" };

enum { SK_RACUN, SK_GOTOVINA, SK_STEDNJA, SK_KREDIT, SK_SVE }
static const gSalterIme[][] = { "Salter: racun i kartice", "Salter: gotovina", "Salter: stednja i transferi", "Salter: krediti", "Salter: sve usluge" };
static const gSalterOpis[][] = { "Otvaranje racuna, informacije, kartice", "Uplata i isplata novca", "Stednja i slanje novca", "Podizanje i otplata kredita", "Sve bankovne usluge" };
static const gBankaAkcijaKat[] = { SK_RACUN, SK_RACUN, SK_GOTOVINA, SK_GOTOVINA, SK_STEDNJA, SK_STEDNJA, SK_STEDNJA, SK_KREDIT, SK_KREDIT, SK_RACUN };
static const gSalterSkin[BANKA_SALTERI] = { 150, 141, 17, 76 };

enum { PP_CLANOVI, PP_PAUZA, PP_TRAJANJE, PP_SIFRA, PP_VRECA_MIN, PP_VRECA_MAX, PP_WANTED, PP_SIGURNO }
enum E_PLJ_POSTAVKA { ppKljuc[12], ppOpis[40], ppZadano, ppMin, ppMax }
static const gPljPostavke[][E_PLJ_POSTAVKA] = {
    { "clanovi", "Potrebno clanova mafije u banci", 3, 1, 20 },
    { "pauza", "Cekanje izmedu pljacki (min)", 60, 0, 1440 },
    { "trajanje", "Trajanje pljacke (min)", 15, 1, 120 },
    { "sifra", "Upisivanje sifre (s)", 10, 1, 120 },
    { "vrecamin", "Najmanje novca po vreci ($)", 3000, 0, 1000000 },
    { "vrecamax", "Najvise novca po vreci ($)", 6000, 0, 1000000 },
    { "wanted", "Zvjezdice potjernice", 3, 0, 6 },
    { "udaljenost", "Udaljenost za detonaciju (m)", 8, 1, 50 }
};
static gPljCfg[sizeof gPljPostavke], gPljUredi[MAX_PLAYERS];

static gBanka[BANKA_MAX][E_BANKA];
static gSalterKat[BANKA_MAX][BANKA_SALTERI] = { { -1, ... }, ... }, gSalterAktor[BANKA_MAX][BANKA_SALTERI], Float:gSalterPoz[BANKA_MAX][BANKA_SALTERI][4], STREAMER_TAG_3D_TEXT_LABEL:gSalterTekst[BANKA_MAX][BANKA_SALTERI];
static gBankaCuvar[BANKA_MAX][BANKA_CUVARI] = { { INVALID_LIK, ... }, ... }, gBankaCuvarDb[BANKA_MAX][BANKA_CUVARI], Float:gBankaCuvarPoz[BANKA_MAX][BANKA_CUVARI][4];
enum { PS_MIRNO, PS_TRCI, PS_CEKA, PS_TRCI2, PS_OTKLJUCAVA, PS_OTVORENO, PS_RAZNESENO }
enum E_PLJ { bool:pjIma, Float:pjMx, Float:pjMy, Float:pjMz, Float:pjMa, Float:pjVx, Float:pjVy, Float:pjVz, pjDModel, Float:pjDx, Float:pjDy, Float:pjDz, Float:pjDrx, Float:pjDry, Float:pjDrz, Float:pjDkut, STREAMER_TAG_OBJECT:pjVrata, pjMenadzer, pjStanje, pjKraj, pjFaza, bool:pjSarka, Float:pjSx, Float:pjSy, Float:pjVrataF, Float:pjVrataCilj, pjVrataToken, pjOtvoreniModel }
enum E_PLJ_POD { STREAMER_TAG_OBJECT:ppObjekt, Float:ppX, Float:ppY, Float:ppZ, Float:ppRx, Float:ppRy, Float:ppRz, Float:ppVis }
enum E_PLJ_EKS { Float:peX, Float:peY, Float:peZ, STREAMER_TAG_OBJECT:peObjekt }
enum E_PLJ_SEF { psModel, Float:psX, Float:psY, Float:psZ, STREAMER_TAG_OBJECT:psObjekt, bool:psUzet }
static gPlj[BANKA_MAX][E_PLJ], gPljEks[BANKA_MAX][PLJ_EKSPLOZIVA][E_PLJ_EKS], gPljEksBroj[BANKA_MAX], gPljSef[BANKA_MAX][PLJ_SEFOVA][E_PLJ_SEF], gPljSefBroj[BANKA_MAX], Float:gPljPut[BANKA_MAX][PLJ_PUT * 3 + 3], gPljPutBroj[BANKA_MAX], gPljPod[BANKA_MAX][PLJ_PODIZNA][E_PLJ_POD], gPljPodBroj[BANKA_MAX], gPljVreca[MAX_PLAYERS], gPljPoruka[MAX_PLAYERS];
static Float:gApBankaPoz[MAX_PLAYERS][4], gApBanka[MAX_PLAYERS], gBankaMeni[MAX_PLAYERS][sizeof gBankaAkcija], gBankaOdabir[MAX_PLAYERS], gBankaSat;

static BankaDug(iznos)
{
    return iznos + iznos * BANKA_KAMATA / 100;
}

static BankaRata(dug)
{
    return (dug + BANKA_RATA - 1) / BANKA_RATA;
}

static BankaKamata(stednja)
{
    new k = stednja / 200;
    return k > BANKA_STEDNJA_MAX ? BANKA_STEDNJA_MAX : k;
}

InitBanka()
{
    assert(BankaDug(1000) == 1100 && BankaRata(1100) == 110 && BankaRata(1101) == 111 && BankaKamata(100) == 0 && BankaKamata(1000000) == BANKA_STEDNJA_MAX);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS banks (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, ix REAL NOT NULL, iy REAL NOT NULL, iz REAL NOT NULL, Aktor INTEGER NOT NULL DEFAULT 0, ax REAL NOT NULL DEFAULT 0, ay REAL NOT NULL DEFAULT 0, az REAL NOT NULL DEFAULT 0, aa REAL NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_guards (id INTEGER PRIMARY KEY AUTOINCREMENT, bank INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_log (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, amount INTEGER NOT NULL, kind TEXT NOT NULL, other INTEGER NOT NULL DEFAULT 0, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS bank_log_account ON bank_log (account, id)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_accounts (account INTEGER PRIMARY KEY, balance INTEGER NOT NULL DEFAULT 0, savings INTEGER NOT NULL DEFAULT 0, savings_tick INTEGER NOT NULL DEFAULT 0, loan INTEGER NOT NULL DEFAULT 0, loan_rate INTEGER NOT NULL DEFAULT 0, loan_due INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE banks SET ix = 1397.3341, iy = -454.1920, iz = -76.2418 WHERE interior = 0 AND abs(ix - 1411.9) < 1.0 AND abs(iy + 453.0) < 1.0"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, ix, iy, iz, Aktor, ax, ay, az, aa FROM banks ORDER BY id LIMIT %d", BANKA_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++)
    {
        gBanka[s][bkDb] = DB_GetFieldInt(res, 0);
        gBanka[s][bkX] = DB_GetFieldFloat(res, 1);
        gBanka[s][bkY] = DB_GetFieldFloat(res, 2);
        gBanka[s][bkZ] = DB_GetFieldFloat(res, 3);
        gBanka[s][bkA] = DB_GetFieldFloat(res, 4);
        gBanka[s][bkInt] = DB_GetFieldInt(res, 5);
        gBanka[s][bkIx] = DB_GetFieldFloat(res, 6);
        gBanka[s][bkIy] = DB_GetFieldFloat(res, 7);
        gBanka[s][bkIz] = DB_GetFieldFloat(res, 8);
        BankaStvori(s);
        BankaUcitajCuvare(s);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
    SalteriUcitaj();
    PljUcitaj();
}

static BankaStvori(s)
{
    new svijet = BANKA_SVIJET + s;
    gBanka[s][bkIma] = true;
    gBanka[s][bkPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ], 0, 0);
    gBanka[s][bkIkona] = MapaIkona(gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ], 52);
    gBanka[s][bkTekst] = CreateDynamic3DTextLabel("Banka", COLOR_ORG, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gBanka[s][bkTekst], BANKA_DOMET, SAVJET_ULAZ);
    gBanka[s][bkPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz], svijet, gBanka[s][bkInt]);
    gBanka[s][bkTekstU] = CreateDynamic3DTextLabel("Izlaz", COLOR_ORG, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gBanka[s][bkInt]);
    InterakcijaOznaka(gBanka[s][bkTekstU], BANKA_DOMET, SAVJET_IZLAZ);
}

static SalterStvori(s, k)
{
    new svijet = BANKA_SVIJET + s;
    gSalterAktor[s][k] = CreateActor(gSalterSkin[k], gSalterPoz[s][k][0], gSalterPoz[s][k][1], gSalterPoz[s][k][2], gSalterPoz[s][k][3]);
    SetActorVirtualWorld(gSalterAktor[s][k], svijet);
    SetActorInvulnerable(gSalterAktor[s][k], true);
    new tekst[96];
    format(tekst, sizeof tekst, "%s\n%s", gSalterIme[gSalterKat[s][k]], gSalterOpis[gSalterKat[s][k]]);
    gSalterTekst[s][k] = CreateDynamic3DTextLabel(tekst, COLOR_ORG, gSalterPoz[s][k][0], gSalterPoz[s][k][1], gSalterPoz[s][k][2] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gBanka[s][bkInt]);
    InterakcijaOznaka(gSalterTekst[s][k], BANKA_KASA_DOMET);
    if (gBanka[s][bkPljacka]) ApplyActorAnimation(gSalterAktor[s][k], "PED", "cower", 4.1, true, false, false, true, 0);
}

static SalterMakni(s, k)
{
    if (gSalterKat[s][k] == -1) return;
    DestroyActor(gSalterAktor[s][k]);
    DestroyDynamic3DTextLabel(gSalterTekst[s][k]);
    gSalterKat[s][k] = -1;
}

static SalteriMakni(s)
{
    for (new k = 0; k < BANKA_SALTERI; k++) SalterMakni(s, k);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_tellers WHERE bank = %d", gBanka[s][bkDb]));
}

static BankaPrviSalter(s)
{
    for (new k = 0; k < BANKA_SALTERI; k++) if (gSalterKat[s][k] != -1) return k;
    return -1;
}

static SalteriSkloni(s, bool:skloni)
{
    for (new k = 0; k < BANKA_SALTERI; k++)
    {
        if (gSalterKat[s][k] == -1) continue;
        if (skloni) ApplyActorAnimation(gSalterAktor[s][k], "PED", "cower", 4.1, true, false, false, true, 0);
        else ClearActorAnimations(gSalterAktor[s][k]);
    }
}

static bool:SalterCilj(playerid, s)
{
    new a = GetPlayerTargetActor(playerid);
    if (a == INVALID_ACTOR_ID) return false;
    for (new k = 0; k < BANKA_SALTERI; k++) if (gSalterKat[s][k] != -1 && gSalterAktor[s][k] == a) return true;
    return false;
}

static SalteriUcitaj()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_tellers (bank INTEGER NOT NULL, slot INTEGER NOT NULL, kat INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, PRIMARY KEY (bank, slot))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO bank_tellers (bank, slot, kat, x, y, z, a) SELECT id, 0, %d, ax, ay, az, aa FROM banks WHERE Aktor = 1", SK_SVE));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE banks SET Aktor = 0 WHERE Aktor = 1"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT bank, slot, kat, x, y, z, a FROM bank_tellers");
    if (DB_GetRowCount(res)) do
    {
        new db = DB_GetFieldInt(res, 0), k = DB_GetFieldInt(res, 1), kat = DB_GetFieldInt(res, 2);
        if (k < 0 || k >= BANKA_SALTERI || kat < SK_RACUN || kat > SK_SVE) continue;
        for (new b = 0; b < BANKA_MAX; b++)
        {
            if (!gBanka[b][bkIma] || gBanka[b][bkDb] != db) continue;
            SalterMakni(b, k);
            gSalterKat[b][k] = kat;
            for (new i = 0; i < 4; i++) gSalterPoz[b][k][i] = DB_GetFieldFloat(res, 3 + i);
            SalterStvori(b, k);
            break;
        }
    }
    while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
}

static BankaIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - BANKA_SVIJET;
    return s >= 0 && s < BANKA_MAX && gBanka[s][bkIma] && GetPlayerInterior(playerid) == gBanka[s][bkInt] ? s : -1;
}

static BankaVani(playerid, s)
{
    PrebaciIgraca(playerid, 0, 0, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ], gBanka[s][bkA] + 180.0);
}

static BankaCuvarStvori(s, c)
{
    new lik = Lik_Stvori(BANKA_CUVAR_SKIN, gBankaCuvarPoz[s][c][0], gBankaCuvarPoz[s][c][1], gBankaCuvarPoz[s][c][2], gBankaCuvarPoz[s][c][3], BANKA_SVIJET + s, gBanka[s][bkInt]);
    gBankaCuvar[s][c] = lik;
    if (lik == INVALID_LIK) return;
    Lik_Oruzje(lik, _:WEAPON_COLT45, 500);
    Lik_Preciznost(lik, 0.5);
}

static BankaCuvarMakni(s, c)
{
    new lik = gBankaCuvar[s][c];
    gBankaCuvarDb[s][c] = 0;
    gBankaCuvar[s][c] = INVALID_LIK;
    if (lik != INVALID_LIK) Lik_Ukloni(lik);
}

static BankaUcitajCuvare(s)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM bank_guards WHERE bank = %d ORDER BY id LIMIT %d", gBanka[s][bkDb], BANKA_CUVARI);
    for (new c = 0, n = DB_GetRowCount(res); c < n; c++, DB_SelectNextRow(res))
    {
        gBankaCuvarDb[s][c] = DB_GetFieldInt(res, 0);
        for (new i = 0; i < 4; i++) gBankaCuvarPoz[s][c][i] = DB_GetFieldFloat(res, 1 + i);
        BankaCuvarStvori(s, c);
    }
    DB_FreeResultSet(res);
}


static bool:BankaMafija(playerid)
{
    return gOrg[playerid] && GetOrgTip(gOrg[playerid]) == PLJ_MAFIJA;
}

static BankaEkipa(playerid, s)
{
    if (!gOrg[playerid]) return 0;
    new n = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && gOrg[i] == gOrg[playerid] && BankaIgraca(i) == s && !IsRanjen(i)) n++;
    return n;
}

bool:BankaBlokira(playerid)
{
    if (IsPlayerNPC(playerid) || PolicajacNaDuznosti(playerid)) return false;
    new s = BankaIgraca(playerid);
    if (s == -1 || gBanka[s][bkPljacka]) return false;
    return !BankaMafija(playerid) || BankaEkipa(playerid, s) < gPljCfg[PP_CLANOVI];
}

static BankaPljackasi(s, igraci[])
{
    new n = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && gOrg[i] && gOrg[i] == gBanka[s][bkPljacka] && BankaIgraca(i) == s && !IsRanjen(i)) igraci[n++] = i;
    return n;
}

static BankaPljObavijest(s, const tekst[], greska = false)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i] || gOrg[i] != gBanka[s][bkPljacka] || BankaIgraca(i) != s) continue;
        if (greska) ObavijestGreska(i, T(i, tekst));
        else ObavijestInfo(i, T(i, "Pljacka banke"), T(i, tekst));
    }
}

static PljMenadzer(s)
{
    if (gPlj[s][pjMenadzer] != INVALID_LIK) Lik_Ukloni(gPlj[s][pjMenadzer]);
    gPlj[s][pjMenadzer] = Lik_Stvori(PLJ_MENADZER_SKIN, gPlj[s][pjMx], gPlj[s][pjMy], gPlj[s][pjMz], gPlj[s][pjMa], BANKA_SVIJET + s, gBanka[s][bkInt]);
    if (gPlj[s][pjMenadzer] != INVALID_LIK) Lik_Neranjiv(gPlj[s][pjMenadzer], true);
}

static PljPodizna(s, bool:otvori)
{
    for (new k = 0; k < gPljPodBroj[s]; k++) ObjektPomakni(gPljPod[s][k][ppObjekt], gPljPod[s][k][ppX], gPljPod[s][k][ppY], gPljPod[s][k][ppZ] + (otvori ? gPljPod[s][k][ppVis] : 0.0), gPljPod[s][k][ppRx], gPljPod[s][k][ppRy], gPljPod[s][k][ppRz], PLJ_VRATA_S);
}

forward PljVrataKorak(s, token);
public PljVrataKorak(s, token)
{
    if (token != gPlj[s][pjVrataToken] || !gPlj[s][pjVrata]) return;
    new Float:f = gPlj[s][pjVrataF], Float:cilj = gPlj[s][pjVrataCilj], Float:korak = 1.0 / PLJ_VRATA_KORACI;
    f = f < cilj ? (f + korak > cilj ? cilj : f + korak) : (f - korak < cilj ? cilj : f - korak);
    gPlj[s][pjVrataF] = f;
    new Float:kut = gPlj[s][pjDkut] * (1.0 - floatcos(f * 180.0, degrees)) / 2.0, Float:x = gPlj[s][pjDx], Float:y = gPlj[s][pjDy];
    if (gPlj[s][pjSarka])
    {
        new Float:dx = x - gPlj[s][pjSx], Float:dy = y - gPlj[s][pjSy], Float:c = floatcos(kut, degrees), Float:sn = floatsin(kut, degrees);
        x = gPlj[s][pjSx] + dx * c - dy * sn;
        y = gPlj[s][pjSy] + dx * sn + dy * c;
    }
    ObjektPomakni(gPlj[s][pjVrata], x, y, gPlj[s][pjDz], gPlj[s][pjDrx], gPlj[s][pjDry], gPlj[s][pjDrz] + kut, PLJ_VRATA_S / PLJ_VRATA_KORACI);
    if (f != cilj) SetTimerEx("PljVrataKorak", floatround(PLJ_VRATA_S * 1000.0 / PLJ_VRATA_KORACI), false, "ii", s, token);
}

static PljVrata(s, bool:otvori)
{
    if (!gPlj[s][pjVrata]) return;
    gPlj[s][pjVrataCilj] = otvori ? 1.0 : 0.0;
    PljVrataKorak(s, ++gPlj[s][pjVrataToken]);
}

static PljSefovi(s, bool:otvori)
{
    for (new k = 0; k < gPljSefBroj[s]; k++)
    {
        if (!otvori) gPljSef[s][k][psUzet] = false;
        if (gPljSef[s][k][psObjekt] && (!otvori || gPlj[s][pjOtvoreniModel])) Streamer_SetIntData(STREAMER_TYPE_OBJECT, gPljSef[s][k][psObjekt], E_STREAMER_MODEL_ID, otvori ? gPlj[s][pjOtvoreniModel] : gPljSef[s][k][psModel]);
    }
}

static PljTrezor(s, bool:otvori)
{
    PljPodizna(s, otvori);
    PljVrata(s, otvori);
}

static PljSefDodaj(s, STREAMER_TAG_OBJECT:o, model)
{
    if (gPljSefBroj[s] >= PLJ_SEFOVA) return -1;
    new k = gPljSefBroj[s]++;
    gPljSef[s][k][psModel] = model;
    gPljSef[s][k][psObjekt] = o;
    gPljSef[s][k][psUzet] = false;
    GetDynamicObjectPos(o, gPljSef[s][k][psX], gPljSef[s][k][psY], gPljSef[s][k][psZ]);
    Streamer_SetIntData(STREAMER_TYPE_OBJECT, o, E_STREAMER_MODEL_ID, model);
    return k;
}

static PljPodiznaDodaj(s, STREAMER_TAG_OBJECT:o, Float:vis)
{
    new k = gPljPodBroj[s]++;
    gPljPod[s][k][ppObjekt] = o;
    gPljPod[s][k][ppVis] = vis;
    GetDynamicObjectPos(o, gPljPod[s][k][ppX], gPljPod[s][k][ppY], gPljPod[s][k][ppZ]);
    GetDynamicObjectRot(o, gPljPod[s][k][ppRx], gPljPod[s][k][ppRy], gPljPod[s][k][ppRz]);
}

static PljResetiraj(s)
{
    gBanka[s][bkPljacka] = 0;
    gPlj[s][pjStanje] = PS_MIRNO;
    SalteriSkloni(s, false);
    PljTrezor(s, false);
    for (new k = 0; k < gPljEksBroj[s]; k++)
    {
        if (gPljEks[s][k][peObjekt]) DestroyDynamicObject(gPljEks[s][k][peObjekt]);
        gPljEks[s][k][peObjekt] = STREAMER_TAG_OBJECT:0;
    }
    PljSefovi(s, false);
    if (gPlj[s][pjIma]) PljMenadzer(s);
    for (new c = 0; c < BANKA_CUVARI; c++)
    {
        new lik = gBankaCuvar[s][c];
        if (lik == INVALID_LIK) continue;
        if (Lik_Mrtav(lik)) Lik_Ozivi(lik, gBankaCuvarPoz[s][c][0], gBankaCuvarPoz[s][c][1], gBankaCuvarPoz[s][c][2], gBankaCuvarPoz[s][c][3]);
        else Lik_PrestaniCiljati(lik);
    }
}

BankaClanovi()
{
    return gPljCfg[PP_CLANOVI];
}

static PljPostavkeUcitaj()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_heist_cfg (k TEXT PRIMARY KEY, v INTEGER NOT NULL)"));
    for (new i = 0; i < sizeof gPljPostavke; i++)
    {
        gPljCfg[i] = gPljPostavke[i][ppZadano];
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT v FROM bank_heist_cfg WHERE k = '%q'", gPljPostavke[i][ppKljuc]);
        if (DB_GetRowCount(res)) gPljCfg[i] = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
    }
}

static PljPostavkePrikazi(playerid)
{
    new list[1024], row[96], len = 0;
    for (new i = 0; i < sizeof gPljPostavke; i++)
    {
        format(row, sizeof row, "%s\t{8AB4F8}%d", T(playerid, gPljPostavke[i][ppOpis]), gPljCfg[i]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_PLJACKA_POSTAVKE, DIALOG_STYLE_TABLIST, T(playerid, "Postavke pljacke"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DLG:pljacka_postavke(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gPljPostavke || !ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    gPljUredi[playerid] = listitem;
    new info[128];
    format(info, sizeof info, T(playerid, "Unesite novu vrijednost (%d - %d):"), gPljPostavke[listitem][ppMin], gPljPostavke[listitem][ppMax]);
    return PrikaziDialog(playerid, DLG_PLJACKA_POSTAVKA, DIALOG_STYLE_INPUT, T(playerid, gPljPostavke[listitem][ppOpis]), info, T(playerid, "Spremi"), T(playerid, "Natrag"));
}

DLG:pljacka_postavka(playerid, response, const inputtext[])
{
    if (!response) return PljPostavkePrikazi(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new i = gPljUredi[playerid];
    if (!ProvjeriBroj(inputtext) || strval(inputtext) < gPljPostavke[i][ppMin] || strval(inputtext) > gPljPostavke[i][ppMax])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vrijednost mora biti izmedu %d i %d."), gPljPostavke[i][ppMin], gPljPostavke[i][ppMax]);
        return PljPostavkePrikazi(playerid);
    }
    gPljCfg[i] = strval(inputtext);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO bank_heist_cfg (k, v) VALUES ('%q', %d)", gPljPostavke[i][ppKljuc], gPljCfg[i]));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Postavka spremljena."));
    return PljPostavkePrikazi(playerid);
}

static PljUcitaj()
{
    PljPostavkeUcitaj();
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_heist (bank INTEGER PRIMARY KEY, mx REAL NOT NULL DEFAULT 0, my REAL NOT NULL DEFAULT 0, mz REAL NOT NULL DEFAULT 0, ma REAL NOT NULL DEFAULT 0, vx REAL NOT NULL DEFAULT 0, vy REAL NOT NULL DEFAULT 0, vz REAL NOT NULL DEFAULT 0, dmodel INTEGER NOT NULL DEFAULT 0, dx REAL NOT NULL DEFAULT 0, dy REAL NOT NULL DEFAULT 0, dz REAL NOT NULL DEFAULT 0, dkut REAL NOT NULL DEFAULT 90)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_heist_spots (id INTEGER PRIMARY KEY AUTOINCREMENT, bank INTEGER NOT NULL, tip INTEGER NOT NULL, model INTEGER NOT NULL DEFAULT 0, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, kut REAL NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_heist SET dmodel = 19799, dy = dy - 1.0, dkut = 90.0 WHERE dmodel = 2634 AND abs(dx - 1411.879) < 0.1 AND abs(dy + 453.306) < 0.1"));
    for (new s = 0; s < BANKA_MAX; s++)
    {
        gPlj[s][pjMenadzer] = INVALID_LIK;
        if (!gBanka[s][bkIma]) continue;
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT mx, my, mz, ma, vx, vy, vz, dmodel, dx, dy, dz, dkut FROM bank_heist WHERE bank = %d", gBanka[s][bkDb]);
        if (DB_GetRowCount(res) && DB_GetFieldInt(res, 7))
        {
            gPlj[s][pjMx] = DB_GetFieldFloat(res, 0);
            gPlj[s][pjMy] = DB_GetFieldFloat(res, 1);
            gPlj[s][pjMz] = DB_GetFieldFloat(res, 2);
            gPlj[s][pjMa] = DB_GetFieldFloat(res, 3);
            gPlj[s][pjVx] = DB_GetFieldFloat(res, 4);
            gPlj[s][pjVy] = DB_GetFieldFloat(res, 5);
            gPlj[s][pjVz] = DB_GetFieldFloat(res, 6);
            gPlj[s][pjDModel] = DB_GetFieldInt(res, 7);
            gPlj[s][pjDkut] = DB_GetFieldFloat(res, 11);
            new STREAMER_TAG_OBJECT:o = ObjektBlizu(DB_GetFieldFloat(res, 8), DB_GetFieldFloat(res, 9), DB_GetFieldFloat(res, 10), 0.5, gPlj[s][pjDModel]);
            if (o && gPlj[s][pjMx] != 0.0)
            {
                gPlj[s][pjVrata] = o;
                GetDynamicObjectPos(o, gPlj[s][pjDx], gPlj[s][pjDy], gPlj[s][pjDz]);
                GetDynamicObjectRot(o, gPlj[s][pjDrx], gPlj[s][pjDry], gPlj[s][pjDrz]);
                gPlj[s][pjIma] = true;
                PljMenadzer(s);
            }
        }
        DB_FreeResultSet(res);
        res = DB_ExecuteQuery(gDB, "SELECT tip, model, x, y, z, kut FROM bank_heist_spots WHERE bank = %d ORDER BY id", gBanka[s][bkDb]);
        if (DB_GetRowCount(res)) do
        {
            if (DB_GetFieldInt(res, 0) == 0 && gPljEksBroj[s] < PLJ_EKSPLOZIVA)
            {
                new k = gPljEksBroj[s]++;
                gPljEks[s][k][peX] = DB_GetFieldFloat(res, 2);
                gPljEks[s][k][peY] = DB_GetFieldFloat(res, 3);
                gPljEks[s][k][peZ] = DB_GetFieldFloat(res, 4);
            }
            else if (DB_GetFieldInt(res, 0) == 2 && gPljPutBroj[s] < PLJ_PUT)
            {
                new k = gPljPutBroj[s]++;
                gPljPut[s][k * 3] = DB_GetFieldFloat(res, 2);
                gPljPut[s][k * 3 + 1] = DB_GetFieldFloat(res, 3);
                gPljPut[s][k * 3 + 2] = DB_GetFieldFloat(res, 4);
            }
            else if (DB_GetFieldInt(res, 0) == 3 && gPljPodBroj[s] < PLJ_PODIZNA)
            {
                new STREAMER_TAG_OBJECT:o = ObjektBlizu(DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), 0.5, DB_GetFieldInt(res, 1));
                if (o) PljPodiznaDodaj(s, o, DB_GetFieldFloat(res, 5));
            }
            else if (DB_GetFieldInt(res, 0) == 1)
            {
                new STREAMER_TAG_OBJECT:o = ObjektBlizu(DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), 0.3);
                if (o) PljSefDodaj(s, o, DB_GetFieldInt(res, 1));
            }
            else if (DB_GetFieldInt(res, 0) == 4)
            {
                gPlj[s][pjSarka] = true;
                gPlj[s][pjSx] = DB_GetFieldFloat(res, 2);
                gPlj[s][pjSy] = DB_GetFieldFloat(res, 3);
            }
            else if (DB_GetFieldInt(res, 0) == 5) gPlj[s][pjOtvoreniModel] = DB_GetFieldInt(res, 1);
        }
        while (DB_SelectNextRow(res));
        DB_FreeResultSet(res);
    }
}

BankaUpdate(playerid)
{
    if (!gUlogiran[playerid] || !gOrg[playerid]) return;
    new s = BankaIgraca(playerid), WEAPON:w = GetPlayerWeapon(playerid);
    if (s == -1 || !gPlj[s][pjIma] || gBanka[s][bkPljacka] || w < WEAPON_COLT45 || w > WEAPON_MINIGUN || !OruzjeLegit(playerid, w)) return;
    if ((gPlj[s][pjMenadzer] == INVALID_LIK || Lik_CiljIgraca(playerid) != gPlj[s][pjMenadzer]) && !SalterCilj(playerid, s)) return;
    new sad = gettime();
    if (!BankaMafija(playerid) || BankaEkipa(playerid, s) < gPljCfg[PP_CLANOVI])
    {
        if (sad - gPljPoruka[playerid] < 10) return;
        gPljPoruka[playerid] = sad;
        new tekst[128];
        format(tekst, sizeof tekst, T(playerid, "Banku mogu opljackati samo mafije s najmanje %d clana u banci."), gPljCfg[PP_CLANOVI]);
        ObavijestGreska(playerid, tekst);
        return;
    }
    if (gBanka[s][bkPljackano] && sad - gBanka[s][bkPljackano] < gPljCfg[PP_PAUZA] * 60)
    {
        if (sad - gPljPoruka[playerid] < 10) return;
        gPljPoruka[playerid] = sad;
        ObavijestGreska(playerid, T(playerid, "Ova banka je nedavno opljackana."));
        return;
    }
    gBanka[s][bkPljacka] = gOrg[playerid];
    gBanka[s][bkPljackano] = sad;
    gPlj[s][pjStanje] = PS_TRCI;
    gPlj[s][pjKraj] = sad + gPljCfg[PP_TRAJANJE] * 60;
    SalteriSkloni(s, true);
    new lik = gPlj[s][pjMenadzer], n = gPljPutBroj[s];
    if (gPljPodBroj[s] && n) Lik_IdiPutem(lik, gPljPut[s], n, LIK_SPRINT);
    else
    {
        gPlj[s][pjStanje] = PS_TRCI2;
        gPljPut[s][n * 3] = gPlj[s][pjVx];
        gPljPut[s][n * 3 + 1] = gPlj[s][pjVy];
        gPljPut[s][n * 3 + 2] = gPlj[s][pjVz];
        Lik_IdiPutem(lik, gPljPut[s], n + 1, LIK_SPRINT);
    }
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i]) continue;
        if (BankaIgraca(i) == s)
        {
            SendClientMessage(i, COLOR_CHAT, T(i, "Menadzer kaze: Dobro, dobro! Otvorit cu vam trezor, samo ne pucajte!"));
            if (gOrg[i] == gOrg[playerid])
            {
                for (new k = 0; k < gPljCfg[PP_WANTED]; k++) DodajWanted(i);
                ObavijestInfo(i, T(i, "Pljacka banke"), T(i, "Pljacka je pocela! Menadzer otvara trezor u podrumu."));
            }
        }
        else if (PolicajacNaDuznosti(i)) SendClientMessage(i, COLOR_ERROR, T(i, "Dojava: pljacka banke u tijeku, %s."), CestaZona(gBanka[s][bkX], gBanka[s][bkY]));
    }
}

BankaLikStigao(lik)
{
    for (new s = 0; s < BANKA_MAX; s++)
    {
        if (!gPlj[s][pjIma] || gPlj[s][pjMenadzer] != lik) continue;
        if (gPlj[s][pjStanje] == PS_TRCI)
        {
            gPlj[s][pjStanje] = PS_CEKA;
            gPlj[s][pjFaza] = gettime() + floatround(PLJ_VRATA_S, floatround_ceil) + 1;
            PljPodizna(s, true);
            BankaPljObavijest(s, "Menadzer otvara prva vrata trezora...");
        }
        else if (gPlj[s][pjStanje] == PS_TRCI2)
        {
            gPlj[s][pjStanje] = PS_OTKLJUCAVA;
            gPlj[s][pjFaza] = gettime() + gPljCfg[PP_SIFRA];
            Lik_Animacija(lik, "PED", "ATM", 4.1, 1, 0, 0, 0, 0);
            BankaPljObavijest(s, "Menadzer upisuje sifru trezora...");
        }
        return 1;
    }
    return 0;
}

static PljTrezorOtvori(s)
{
    gPlj[s][pjStanje] = PS_OTVORENO;
    gPlj[s][pjKraj] = gettime() + gPljCfg[PP_TRAJANJE] * 60;
    PljVrata(s, true);
    Lik_Animacija(gPlj[s][pjMenadzer], "ROB_BANK", "SHP_HandsUp_Scr", 4.1, 0, 0, 0, 1, 0);
    BankaPljObavijest(s, "Trezor je otvoren! Postavite eksploziv na oznacena mjesta.");
}

static PljVrataPomakni(s, bool:otvori)
{
    PljTrezor(s, otvori);
    PljSefovi(s, otvori);
}

static bool:PljEksPostavljen(s)
{
    for (new k = 0; k < gPljEksBroj[s]; k++) if (!gPljEks[s][k][peObjekt]) return false;
    return gPljEksBroj[s] > 0;
}

static PljPostavljeno(s)
{
    new n = 0;
    for (new k = 0; k < gPljEksBroj[s]; k++) if (gPljEks[s][k][peObjekt]) n++;
    return n;
}

BankaPljackaAlt(playerid)
{
    new s = BankaIgraca(playerid);
    if (s == -1 || !gBanka[s][bkPljacka] || gOrg[playerid] != gBanka[s][bkPljacka] || IsRanjen(playerid)) return 0;
    if (gPlj[s][pjStanje] == PS_OTVORENO)
    {
        for (new k = 0; k < gPljEksBroj[s]; k++)
        {
            if (gPljEks[s][k][peObjekt] || !IsPlayerInRangeOfPoint(playerid, PLJ_DOMET, gPljEks[s][k][peX], gPljEks[s][k][peY], gPljEks[s][k][peZ])) continue;
            if (!GiveItem(playerid, ITEM_EKSPLOZIV, -1)) return ObavijestGreska(playerid, T(playerid, "Nemate eksploziv.")), 1;
            ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, false, false, false, false, 0);
            gPljEks[s][k][peObjekt] = CreateDynamicObject(1654, gPljEks[s][k][peX], gPljEks[s][k][peY], gPljEks[s][k][peZ] - 0.85, 0.0, 0.0, 0.0, BANKA_SVIJET + s, gBanka[s][bkInt]);
            new tekst[96];
            format(tekst, sizeof tekst, T(playerid, "Eksploziv postavljen (%d/%d)."), PljPostavljeno(s), gPljEksBroj[s]);
            ObavijestInfo(playerid, T(playerid, "Pljacka banke"), tekst);
            return 1;
        }
        new n = PljPostavljeno(s);
        if (!n) return 0;
        if (!PljEksPostavljen(s))
        {
            new tekst[96];
            format(tekst, sizeof tekst, T(playerid, "Postavite eksploziv na sva mjesta (%d/%d)."), n, gPljEksBroj[s]);
            return ObavijestGreska(playerid, tekst), 1;
        }
        for (new k = 0; k < gPljEksBroj[s]; k++)
            if (IsPlayerInRangeOfPoint(playerid, float(gPljCfg[PP_SIGURNO]), gPljEks[s][k][peX], gPljEks[s][k][peY], gPljEks[s][k][peZ])) return ObavijestGreska(playerid, T(playerid, "Udaljite se od eksploziva prije detonacije.")), 1;
        for (new k = 0; k < gPljEksBroj[s]; k++)
        {
            for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && BankaIgraca(i) == s) CreateExplosionForPlayer(i, gPljEks[s][k][peX], gPljEks[s][k][peY], gPljEks[s][k][peZ], 12, 3.0);
            DestroyDynamicObject(gPljEks[s][k][peObjekt]);
            gPljEks[s][k][peObjekt] = STREAMER_TAG_OBJECT:0;
        }
        PljSefovi(s, true);
        gPlj[s][pjStanje] = PS_RAZNESENO;
        BankaPljObavijest(s, "Pretinci su otvoreni! Pokupite novac.");
        return 1;
    }
    if (gPlj[s][pjStanje] != PS_RAZNESENO) return 0;
    for (new k = 0; k < gPljSefBroj[s]; k++)
    {
        if (gPljSef[s][k][psUzet] || !IsPlayerInRangeOfPoint(playerid, PLJ_DOMET + 0.4, gPljSef[s][k][psX], gPljSef[s][k][psY], gPljSef[s][k][psZ])) continue;
        gPljSef[s][k][psUzet] = true;
        new od = gPljCfg[PP_VRECA_MIN], do_ = gPljCfg[PP_VRECA_MAX] > od ? gPljCfg[PP_VRECA_MAX] : od, iznos = od + random(do_ - od + 1), tekst[96];
        gPljVreca[playerid] += iznos;
        ApplyAnimation(playerid, "ROB_BANK", "CAT_Safe_Rob", 4.1, false, false, false, false, 0);
        SetPlayerAttachedObject(playerid, PLJ_VRECA_SLOT, 1550, 1, 0.1, -0.25, 0.0, 0.0, 90.0, 0.0);
        format(tekst, sizeof tekst, T(playerid, "Uzeli ste vrecu novca ($%d). Pobjegnite iz banke."), iznos);
        ObavijestInfo(playerid, T(playerid, "Pljacka banke"), tekst);
        return 1;
    }
    return 0;
}

static PljSavjeti(s)
{
    new igraci[MAX_PLAYERS], n = BankaPljackasi(s, igraci);
    for (new j = 0; j < n; j++)
    {
        new i = igraci[j], bool:blizu = false;
        if (gPlj[s][pjStanje] == PS_OTVORENO)
        {
            for (new k = 0; k < gPljEksBroj[s] && !blizu; k++)
                if (!gPljEks[s][k][peObjekt] && IsPlayerInRangeOfPoint(i, PLJ_DOMET, gPljEks[s][k][peX], gPljEks[s][k][peY], gPljEks[s][k][peZ])) blizu = true;
            if (blizu) Savjet(i, T(i, "~y~ALT~w~ - postavi eksploziv"), 1200);
            else if (PljEksPostavljen(s))
            {
                new bool:sigurno = true;
                for (new k = 0; k < gPljEksBroj[s]; k++) if (IsPlayerInRangeOfPoint(i, float(gPljCfg[PP_SIGURNO]), gPljEks[s][k][peX], gPljEks[s][k][peY], gPljEks[s][k][peZ])) sigurno = false;
                Savjet(i, sigurno ? T(i, "~y~ALT~w~ - detoniraj") : T(i, "~r~Udaljite se od eksploziva"), 1200);
            }
        }
        else if (gPlj[s][pjStanje] == PS_RAZNESENO)
        {
            for (new k = 0; k < gPljSefBroj[s] && !blizu; k++)
                if (!gPljSef[s][k][psUzet] && IsPlayerInRangeOfPoint(i, PLJ_DOMET + 0.4, gPljSef[s][k][psX], gPljSef[s][k][psY], gPljSef[s][k][psZ])) blizu = true;
            if (blizu) Savjet(i, T(i, "~y~ALT~w~ - uzmi novac"), 1200);
        }
    }
}

static BankaPljackaTick()
{
    new sad = gettime();
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gPljVreca[i] || !gUlogiran[i]) continue;
        if (IsRanjen(i))
        {
            gPljVreca[i] = 0;
            RemovePlayerAttachedObject(i, PLJ_VRECA_SLOT);
            ObavijestGreska(i, T(i, "Izgubili ste vrecu novca."));
        }
        else if (GetPlayerVirtualWorld(i) == 0 && GetPlayerInterior(i) == 0)
        {
            new tekst[96];
            DajNovac(i, gPljVreca[i]);
            format(tekst, sizeof tekst, T(i, "Pobjegli ste s plijenom: $%d."), gPljVreca[i]);
            ObavijestInfo(i, T(i, "Pljacka banke"), tekst);
            if (gOrg[i]) OrgDnevnik(gOrg[i], i, DNEVNIK_BANKA, gPljVreca[i]);
            gPljVreca[i] = 0;
            RemovePlayerAttachedObject(i, PLJ_VRECA_SLOT);
        }
    }
    for (new s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma] || !gBanka[s][bkPljacka]) continue;
        new prisutni[MAX_PLAYERS], n = BankaPljackasi(s, prisutni);
        if (!n || sad >= gPlj[s][pjKraj])
        {
            if (gPlj[s][pjStanje] < PS_RAZNESENO) for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && gOrg[i] == gBanka[s][bkPljacka]) ObavijestGreska(i, T(i, "Pljacka banke je propala."));
            PljResetiraj(s);
            continue;
        }
        if (gPlj[s][pjStanje] == PS_CEKA && sad >= gPlj[s][pjFaza])
        {
            gPlj[s][pjStanje] = PS_TRCI2;
            Lik_IdiDo(gPlj[s][pjMenadzer], gPlj[s][pjVx], gPlj[s][pjVy], gPlj[s][pjVz], LIK_SPRINT);
        }
        if (gPlj[s][pjStanje] == PS_OTKLJUCAVA && sad >= gPlj[s][pjFaza]) PljTrezorOtvori(s);
        PljSavjeti(s);
        for (new c = 0; c < BANKA_CUVARI; c++)
        {
            new lik = gBankaCuvar[s][c], meta = INVALID_PLAYER_ID, Float:naj = 100.0, Float:x, Float:y, Float:z;
            if (lik == INVALID_LIK || Lik_Mrtav(lik)) continue;
            Lik_Pozicija(lik, x, y, z);
            for (new k = 0; k < n; k++)
            {
                new Float:d = GetPlayerDistanceFromPoint(prisutni[k], x, y, z);
                if (d < naj) { naj = d; meta = prisutni[k]; }
            }
            if (meta != INVALID_PLAYER_ID && !Lik_Cilja(lik, meta)) Lik_Nacilja(lik, meta, true, 400 + random(400));
        }
    }
}

PljVrecaOdlazak(playerid)
{
    gPljVreca[playerid] = 0;
}

static PljPomoc(playerid)
{
    new tekst[1536];
    strcat(tekst, T(playerid, "/pljacka postavke - brojke pljacke (clanovi, cekanje, novac...)"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka menadzer - mjesto menadzera"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka tocka - tocka puta menadzera (npr. stepenice)"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka tockebrisi - obrisi put menadzera"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka podizna [metara] - podizna vrata ispred vas"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka vrata [kut] - okretna vrata ispred vas, menadzer staje ovdje"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka eksploziv - mjesto za eksploziv i zatvoreni sef ispred vas"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka otvoreni - otvoreni sef ispred vas (model nakon eksplozije)"));
    strcat(tekst, "
");
    strcat(tekst, T(playerid, "/pljacka sarka - stanite na sarku okretnih vrata trezora"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka otvori / zatvori - vrata trezora i pretinci"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka reset [sve] - zavrsi pljacku i ponisti cekanje"));
    strcat(tekst, "\n");
    strcat(tekst, T(playerid, "/pljacka brisi - obrisi postavke pljacke ove banke"));
    strcat(tekst, "\n");
    return PrikaziDialog(playerid, DLG_PLJACKA_POMOC, DIALOG_STYLE_MSGBOX, T(playerid, "Postavke pljacke"), tekst, T(playerid, "Zatvori"));
}

CMD:pljacka(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!params[0]) return PljPomoc(playerid);
    new s = BankaIgraca(playerid), idx = 0, opcija[16], p2[16];
    if (!strcmp(params, "postavke", true)) return PljPostavkePrikazi(playerid);
    if (s == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti u banci."));
    SljedeciParam(params, idx, opcija);
    SljedeciParam(params, idx, p2);
    new Float:x, Float:y, Float:z, Float:a, Float:kut = p2[0] ? floatstr(p2) : 90.0;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    if (!strcmp(opcija, "menadzer", true))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO bank_heist (bank) VALUES (%d)", gBanka[s][bkDb]));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_heist SET mx = %f, my = %f, mz = %f, ma = %f WHERE bank = %d", x, y, z, a, gBanka[s][bkDb]));
        gPlj[s][pjMx] = x;
        gPlj[s][pjMy] = y;
        gPlj[s][pjMz] = z;
        gPlj[s][pjMa] = a;
        if (gPlj[s][pjIma]) PljMenadzer(s);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Mjesto menadzera postavljeno."));
    }
    if (!strcmp(opcija, "sarka", true))
    {
        if (!gPlj[s][pjVrata]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ova banka nema postavljena vrata trezora."));
        PljVrata(s, false);
        gPlj[s][pjSarka] = true;
        gPlj[s][pjSx] = x;
        gPlj[s][pjSy] = y;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_heist_spots WHERE bank = %d AND tip = 4", gBanka[s][bkDb]));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_heist_spots (bank, tip, x, y, z) VALUES (%d, 4, %f, %f, %f)", gBanka[s][bkDb], x, y, z));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Sarka vrata trezora postavljena. Probajte s /pljacka otvori."));
    }
    if (!strcmp(opcija, "vrata", true) || !strcmp(opcija, "otvoreni", true) || !strcmp(opcija, "podizna", true))
    {
        new STREAMER_TAG_OBJECT:o = ObjektBlizu(x - floatsin(a, degrees) * 1.2, y + floatcos(a, degrees) * 1.2, z, 2.5);
        if (!o) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ispred vas nema objekta."));
        new model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, o, E_STREAMER_MODEL_ID), Float:ox, Float:oy, Float:oz, Float:rx, Float:ry, Float:rz;
        GetDynamicObjectPos(o, ox, oy, oz);
        GetDynamicObjectRot(o, rx, ry, rz);
        if (opcija[0] == 'v' || opcija[0] == 'V')
        {
            if (gPlj[s][pjMx] == 0.0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najprije postavite mjesto menadzera."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_heist SET vx = %f, vy = %f, vz = %f, dmodel = %d, dx = %f, dy = %f, dz = %f, dkut = %f WHERE bank = %d", x, y, z, model, ox, oy, oz, kut, gBanka[s][bkDb]));
            gPlj[s][pjVx] = x;
            gPlj[s][pjVy] = y;
            gPlj[s][pjVz] = z;
            gPlj[s][pjDModel] = model;
            gPlj[s][pjDkut] = kut;
            gPlj[s][pjVrata] = o;
            gPlj[s][pjDx] = ox;
            gPlj[s][pjDy] = oy;
            gPlj[s][pjDz] = oz;
            gPlj[s][pjDrx] = rx;
            gPlj[s][pjDry] = ry;
            gPlj[s][pjDrz] = rz;
            if (!gPlj[s][pjIma])
            {
                gPlj[s][pjIma] = true;
                PljMenadzer(s);
            }
            return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vrata trezora: objekt %d. Menadzer ce stati na vase mjesto."), model);
        }
        if (opcija[0] == 'p' || opcija[0] == 'P')
        {
            if (gPljPodBroj[s] >= PLJ_PODIZNA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj."));
            new Float:vis = p2[0] ? floatstr(p2) : 3.0;
            PljPodiznaDodaj(s, o, vis);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_heist_spots (bank, tip, model, x, y, z, kut) VALUES (%d, 3, %d, %f, %f, %f, %f)", gBanka[s][bkDb], model, ox, oy, oz, vis));
            return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Podizna vrata trezora: objekt %d (%d)."), model, gPljPodBroj[s]);
        }
        if (!gPljSefBroj[s]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najprije s /pljacka eksploziv oznacite zatvorene sefove."));
        new k = PljSefDodaj(s, o, gPljSef[s][0][psModel]);
        if (k == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj."));
        gPlj[s][pjOtvoreniModel] = model;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_heist_spots WHERE bank = %d AND tip = 5", gBanka[s][bkDb]));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_heist_spots (bank, tip, model, x, y, z) VALUES (%d, 5, %d, %f, %f, %f)", gBanka[s][bkDb], model, ox, oy, oz));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_heist_spots (bank, tip, model, x, y, z) VALUES (%d, 1, %d, %f, %f, %f)", gBanka[s][bkDb], gPljSef[s][k][psModel], ox, oy, oz));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Otvoreni sef: objekt %d. Sada je zatvoren, otvorit ce se eksplozijom."), model);
    }
    if (!strcmp(opcija, "postavke", true)) return PljPostavkePrikazi(playerid);
    if (!strcmp(opcija, "tocka", true))
    {
        if (gPljPutBroj[s] >= PLJ_PUT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj."));
        new k = gPljPutBroj[s]++;
        gPljPut[s][k * 3] = x;
        gPljPut[s][k * 3 + 1] = y;
        gPljPut[s][k * 3 + 2] = z;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_heist_spots (bank, tip, x, y, z) VALUES (%d, 2, %f, %f, %f)", gBanka[s][bkDb], x, y, z));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Tocka puta menadzera dodana (%d)."), gPljPutBroj[s]);
    }
    if (!strcmp(opcija, "tockebrisi", true))
    {
        gPljPutBroj[s] = 0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_heist_spots WHERE bank = %d AND tip = 2", gBanka[s][bkDb]));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Put menadzera je obrisan."));
    }
    if (!strcmp(opcija, "otvori", true) || !strcmp(opcija, "zatvori", true))
    {
        if (!gPlj[s][pjVrata] && !gPljSefBroj[s] && !gPljPodBroj[s]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ova banka nema postavljena vrata trezora."));
        PljVrataPomakni(s, opcija[0] == 'o' || opcija[0] == 'O');
        return SendClientMessage(playerid, COLOR_INFO, opcija[0] == 'o' || opcija[0] == 'O' ? T(playerid, "Vrata trezora i pretinci se otvaraju.") : T(playerid, "Vrata trezora i pretinci se zatvaraju."));
    }
    if (!strcmp(opcija, "reset", true))
    {
        if (!strcmp(p2, "sve", true))
        {
            for (new b = 0; b < BANKA_MAX; b++)
            {
                if (!gBanka[b][bkIma]) continue;
                if (gBanka[b][bkPljacka]) PljResetiraj(b);
                gBanka[b][bkPljackano] = 0;
            }
            return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pljacke i cekanje resetirani u svim bankama."));
        }
        if (gBanka[s][bkPljacka]) PljResetiraj(s);
        gBanka[s][bkPljackano] = 0;
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pljacka i cekanje u ovoj banci su resetirani."));
    }
    if (!strcmp(opcija, "eksploziv", true))
    {
        if (gPljEksBroj[s] >= PLJ_EKSPLOZIVA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj."));
        new k = gPljEksBroj[s]++;
        gPljEks[s][k][peX] = x;
        gPljEks[s][k][peY] = y;
        gPljEks[s][k][peZ] = z;
        gPljEks[s][k][peObjekt] = STREAMER_TAG_OBJECT:0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_heist_spots (bank, tip, x, y, z) VALUES (%d, 0, %f, %f, %f)", gBanka[s][bkDb], x, y, z));
        new STREAMER_TAG_OBJECT:o = ObjektBlizu(x - floatsin(a, degrees) * 1.2, y + floatcos(a, degrees) * 1.2, z, 2.0);
        if (!o) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Mjesto za eksploziv dodano (%d)."), gPljEksBroj[s]);
        new model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, o, E_STREAMER_MODEL_ID), j = PljSefDodaj(s, o, model);
        if (j == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj."));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_heist_spots (bank, tip, model, x, y, z) VALUES (%d, 1, %d, %f, %f, %f)", gBanka[s][bkDb], model, gPljSef[s][j][psX], gPljSef[s][j][psY], gPljSef[s][j][psZ]));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Eksploziv (%d) i sef ispred vas (objekt %d) dodani."), gPljEksBroj[s], model);
    }
    if (!strcmp(opcija, "brisi", true))
    {
        if (gBanka[s][bkPljacka]) PljResetiraj(s);
        if (gPlj[s][pjMenadzer] != INVALID_LIK) Lik_Ukloni(gPlj[s][pjMenadzer]);
        gPlj[s][pjMenadzer] = INVALID_LIK;
        gPlj[s][pjIma] = false;
        gPlj[s][pjVrata] = STREAMER_TAG_OBJECT:0;
        gPlj[s][pjMx] = 0.0;
        gPljEksBroj[s] = 0;
        gPljSefBroj[s] = 0;
        gPljPodBroj[s] = 0;
        gPljPutBroj[s] = 0;
        gPlj[s][pjSarka] = false;
        gPlj[s][pjOtvoreniModel] = 0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_heist WHERE bank = %d", gBanka[s][bkDb]));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_heist_spots WHERE bank = %d", gBanka[s][bkDb]));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Postavke pljacke su obrisane."));
    }
    return PljPomoc(playerid);
}

BankaUlaz(playerid)
{
    new s = BankaIgraca(playerid);
    if (s != -1)
    {
        if (!IsPlayerInRangeOfPoint(playerid, BANKA_DOMET, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz])) return 0;
        BankaVani(playerid, s);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma] || !IsPlayerInRangeOfPoint(playerid, BANKA_DOMET, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ])) continue;
        PrebaciIgraca(playerid, gBanka[s][bkInt], BANKA_SVIJET + s, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz]);
        new k = BankaPrviSalter(s);
        if (k != -1 && !gBanka[s][bkPljacka]) AktorKaze(playerid, gSalterAktor[s][k], "Sluzbenik", "Dobar dan, izvolite na salter.");
        return 1;
    }
    return 0;
}

static bool:BankaUcitaj(racun, &stanje, &stednja, &kredit, &rata, &rok)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT balance, savings, loan, loan_rate, loan_due FROM bank_accounts WHERE account = %d", racun);
    new bool:ima = DB_GetRowCount(res) > 0;
    if (ima)
    {
        stanje = DB_GetFieldInt(res, 0);
        stednja = DB_GetFieldInt(res, 1);
        kredit = DB_GetFieldInt(res, 2);
        rata = DB_GetFieldInt(res, 3);
        rok = DB_GetFieldInt(res, 4);
    }
    DB_FreeResultSet(res);
    return ima;
}

BankaZapis(racun, iznos, const vrsta[], druga = 0)
{
    if (!racun || !iznos) return;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_log (account, amount, kind, other, created) VALUES (%d, %d, '%q', %d, %d)", racun, iznos, vrsta, druga, gettime()));
}

BankaPovijest(playerid, tekst[], size, broj)
{
    new vrsta[32], ime[MAX_PLAYER_NAME + 1], red[128], g, m, d, sat, minuta;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT l.amount, l.kind, l.created, IFNULL(replace(a.name, '_', ' '), '') FROM bank_log l LEFT JOIN accounts a ON a.id = l.other AND l.other > 0 WHERE l.account = %d ORDER BY l.id DESC LIMIT %d", gAccountId[playerid], broj);
    new n = DB_GetRowCount(res);
    format(red, sizeof red, "~n~~n~~b~%s~l~", T(playerid, "Zadnje transakcije"));
    strcat(tekst, red, size);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new iznos = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, vrsta, sizeof vrsta);
        DB_GetFieldString(res, 3, ime, sizeof ime);
        TdCisti(ime);
        VrijemeTs(DB_GetFieldInt(res, 2), g, m, d, sat, minuta);
        format(red, sizeof red, "~n~%02d.%02d. %02d:%02d  %s$%d~l~  %s%s%s", d, m, sat, minuta, iznos > 0 ? ("~g~+") : ("~r~-"), iznos > 0 ? iznos : -iznos, T(playerid, vrsta), ime[0] ? (": ") : (""), ime);
        strcat(tekst, red, size);
    }
    DB_FreeResultSet(res);
    if (!n)
    {
        strcat(tekst, "~n~", size);
        strcat(tekst, T(playerid, "Nema transakcija."), size);
    }
}

bool:BankaTekst(playerid, tekst[], size = sizeof (tekst))
{
    new stanje, stednja, kredit, rata, rok;
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok)) return false;
    format(tekst, size, T(playerid, "Racun: $%d~n~Stednja: $%d"), stanje, stednja);
    if (!kredit)
    {
        strcat(tekst, T(playerid, "~n~Kredit: nema"), size);
        return true;
    }
    new red[96], sati = (rok - gettime()) / 3600;
    format(red, sizeof red, T(playerid, "~n~Kredit: $%d~n~Rata $%d za %d h"), kredit, rata < kredit ? rata : kredit, sati < 0 ? 0 : sati);
    strcat(tekst, red, size);
    return true;
}

static bool:BankaPosalji(playerid, racun, iznos)
{
    new stanje, stednja, kredit, rata, rok, a, b, c, d, e;
    if (iznos <= 0 || iznos > MAX_KES)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Iznos mora biti veci od 0."));
        return false;
    }
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovni racun."));
        return false;
    }
    if (racun == gAccountId[playerid])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to sami sebi."));
        return false;
    }
    if (kredit)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete slati novac dok ne otplatite kredit."));
        return false;
    }
    if (!racun || !BankaUcitaj(racun, a, b, c, d, e))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Primatelj nema bankovni racun."));
        return false;
    }
    if (stanje < iznos)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
        return false;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d WHERE account = %d", iznos, gAccountId[playerid]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d WHERE account = %d", iznos, racun));
    BankaZapis(gAccountId[playerid], -iznos, "Poslano", racun);
    BankaZapis(racun, iznos, "Primljeno", gAccountId[playerid]);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Poslali ste $%d."), iznos);
    new i = IgracRacuna(racun);
    if (i != INVALID_PLAYER_ID)
    {
        new poruka[144];
        format(poruka, sizeof poruka, T(i, "Na vas racun je stiglo $%d od %s."), iznos, GetRPIme(playerid));
        MobitelBankaSms(i, poruka);
    }
    return true;
}

BankaTick()
{
    BankaPljackaTick();
    if (++gBankaSat < 60) return;
    gBankaSat = 0;
    new sad = gettime();
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i]) continue;
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT balance, savings, loan, loan_rate, loan_due, savings_tick FROM bank_accounts WHERE account = %d", gAccountId[i]);
        if (!DB_GetRowCount(res))
        {
            DB_FreeResultSet(res);
            continue;
        }
        new stanje = DB_GetFieldInt(res, 0), stednja = DB_GetFieldInt(res, 1), kredit = DB_GetFieldInt(res, 2), rata = DB_GetFieldInt(res, 3), rok = DB_GetFieldInt(res, 4), tick = DB_GetFieldInt(res, 5), poruka[144];
        DB_FreeResultSet(res);
        if (stednja > 0 && sad - tick >= BANKA_DAN)
        {
            new kamata = BankaKamata(stednja);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET savings = savings + %d, savings_tick = %d WHERE account = %d", kamata, tick + BANKA_DAN, gAccountId[i]));
            BankaZapis(gAccountId[i], kamata, "Kamata na stednju");
            if (kamata)
            {
                format(poruka, sizeof poruka, T(i, "Kamata na vasu stednju: $%d."), kamata);
                MobitelBankaSms(i, poruka);
            }
        }
        if (!kredit || sad < rok) continue;
        new placa = rata < kredit ? rata : kredit, sRacuna = stanje < placa ? stanje : placa, gotovina = placa - sRacuna;
        if (gNovac[i] < gotovina)
        {
            new kazna = kredit * BANKA_KAZNA / 100;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET loan = loan + %d, loan_due = %d WHERE account = %d", kazna, rok + BANKA_DAN, gAccountId[i]));
            format(poruka, sizeof poruka, T(i, "Nemate dovoljno za ratu kredita ($%d). Zaracunata je zatezna kamata $%d."), placa, kazna);
            MobitelBankaSms(i, poruka);
            continue;
        }
        if (gotovina) DajNovac(i, -gotovina);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d, loan = loan - %d, loan_due = %d WHERE account = %d", sRacuna, placa, rok + BANKA_DAN, gAccountId[i]));
        BankaZapis(gAccountId[i], -sRacuna, "Rata kredita");
        format(poruka, sizeof poruka, T(i, "Naplacena je rata kredita $%d. Preostali dug: $%d."), placa, kredit - placa);
        MobitelBankaSms(i, poruka);
    }
}

BankaKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new s = BankaIgraca(playerid), k = -1, Float:naj = BANKA_KASA_DOMET;
    if (s == -1) return 0;
    for (new i = 0; i < BANKA_SALTERI; i++)
    {
        if (gSalterKat[s][i] == -1) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, gSalterPoz[s][i][0], gSalterPoz[s][i][1], gSalterPoz[s][i][2]);
        if (d < naj) { naj = d; k = i; }
    }
    if (k == -1) return 0;
    if (gBanka[s][bkPljacka])
    {
        if (gOrg[playerid] == gBanka[s][bkPljacka]) return 0;
        ObavijestGreska(playerid, T(playerid, "Sluzbenici se skrivaju, banka se pljacka."));
        return 1;
    }
    new kat = gSalterKat[s][k], stanje, stednja, kredit, rata, rok, list[512], row[64], len = 0, n = 0;
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok))
    {
        if (kat != SK_RACUN && kat != SK_SVE)
        {
            AktorKaze(playerid, gSalterAktor[s][k], "Sluzbenik", "Najprije otvorite racun na salteru za racune.");
            return 1;
        }
        gBankaMeni[playerid][n++] = BA_OTVORI;
    }
    else for (new a = BA_INFO; a <= BA_KARTICA; a++)
    {
        if (kat != SK_SVE && gBankaAkcijaKat[a] != kat) continue;
        if ((a == BA_KREDIT && kredit) || (a == BA_OTPLATI && !kredit) || (a == BA_KARTICA && gInv[playerid][ITEM_KARTICA])) continue;
        gBankaMeni[playerid][n++] = a;
    }
    for (new i = 0; i < n; i++)
    {
        format(row, sizeof row, T(playerid, gBankaAkcija[gBankaMeni[playerid][i]]), gBankaMeni[playerid][i] == BA_OTVORI ? BANKA_OTVARANJE : BANKA_KARTICA);
        DodajStupac(list, len, row);
    }
    for (new i = n; i < sizeof gBankaAkcija; i++) gBankaMeni[playerid][i] = -1;
    AktorKaze(playerid, gSalterAktor[s][k], "Sluzbenik", "Izvolite, kako vam mogu pomoci?");
    PrikaziDialog(playerid, DLG_BANKA, DIALOG_STYLE_LIST, T(playerid, "Banka"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
    return 1;
}

DLG:banka(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gBankaAkcija || BankaIgraca(playerid) == -1) return 1;
    new a = gBankaMeni[playerid][listitem], body[160];
    if (a == -1) return 1;
    if (a == BA_INFO)
    {
        new tekst[160];
        BankaTekst(playerid, tekst);
        for (new i = 0; tekst[i]; i++) if (tekst[i] == '~' && tekst[i + 1] == 'n' && tekst[i + 2] == '~')
        {
            tekst[i] = '\n';
            strdel(tekst, i + 1, i + 3);
        }
        return PrikaziDialog(playerid, DLG_BANKA_INFO, DIALOG_STYLE_MSGBOX, T(playerid, "Informacije"), tekst, T(playerid, "Natrag"));
    }
    if (a == BA_OTVORI || a == BA_KARTICA)
    {
        new cijena = a == BA_OTVORI ? BANKA_OTVARANJE : BANKA_KARTICA;
        if (gNovac[playerid] < cijena) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        if (!GiveItem(playerid, ITEM_KARTICA, 1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        DajNovac(playerid, -cijena);
        if (a == BA_OTVORI) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO bank_accounts (account) VALUES (%d)", gAccountId[playerid]));
        SendClientMessage(playerid, COLOR_INFO, T(playerid, a == BA_OTVORI ? ("Otvorili ste racun i dobili bankovnu karticu.") : ("Dobili ste novu bankovnu karticu.")));
        return BankaKasa(playerid);
    }
    if (a != BA_UPLATI && !gInv[playerid][ITEM_KARTICA]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovnu karticu."));
    gBankaOdabir[playerid] = a;
    if (a == BA_POSALJI) format(body, sizeof body, T(playerid, "Unesite: [id/ime primatelja] [iznos]"));
    else if (a == BA_KREDIT) format(body, sizeof body, T(playerid, "Iznos kredita (najvise $%d). Kamata %d%%, otplata u %d dnevnih rata:"), BANKA_KREDIT_MAX, BANKA_KAMATA, BANKA_RATA);
    else format(body, sizeof body, T(playerid, "Unesite iznos:"));
    return PrikaziDialog(playerid, DLG_BANKA_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Banka"), body, T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

bool:BankaPosaljiUnos(playerid, const unos[])
{
    new idx = 0, p1[32], p2[16];
    if (!SljedeciParam(unos, idx, p1) || !SljedeciParam(unos, idx, p2) || !ProvjeriBroj(p2) || strlen(p2) > 9)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [id/ime primatelja] [iznos]"));
        return false;
    }
    new t = FindPlayer(p1), primatelj = 0;
    if (t != INVALID_PLAYER_ID && gUlogiran[t]) primatelj = gAccountId[t];
    else
    {
        for (new i = 0; p1[i]; i++) if (p1[i] == ' ') p1[i] = '_';
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q' COLLATE NOCASE", p1);
        if (DB_GetRowCount(res)) primatelj = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
    }
    return BankaPosalji(playerid, primatelj, strval(p2));
}

bool:BankaOtplati(playerid, n)
{
    new stanje, stednja, kredit, rata, rok;
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok) || !kredit) return false;
    if (n > kredit) n = kredit;
    if (n <= 0 || stanje < n)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
        return false;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d, loan = loan - %d WHERE account = %d", n, n, gAccountId[playerid]));
    BankaZapis(gAccountId[playerid], -n, "Otplata kredita");
    return true;
}

bool:BankaUplati(racun, iznos, const vrsta[] = "Uplata")
{
    new stanje, stednja, kredit, rata, rok;
    if (!BankaUcitaj(racun, stanje, stednja, kredit, rata, rok) || stanje + iznos < 0) return false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d WHERE account = %d", iznos, racun));
    BankaZapis(racun, iznos, vrsta);
    return true;
}

bool:BankaPromijeni(playerid, iznos, const vrsta[])
{
    new stanje, stednja, kredit, rata, rok;
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovni racun."));
        return false;
    }
    if (stanje + iznos < 0)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
        return false;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d WHERE account = %d", iznos, gAccountId[playerid]));
    BankaZapis(gAccountId[playerid], iznos, vrsta);
    return true;
}

DLG:banka_info(playerid)
{
    return BankaKasa(playerid);
}

DLG:banka_unos(playerid, response, const inputtext[])
{
    if (BankaIgraca(playerid) == -1) return 1;
    if (!response) return BankaKasa(playerid);
    new a = gBankaOdabir[playerid], racun = gAccountId[playerid], stanje, stednja, kredit, rata, rok;
    if (!BankaUcitaj(racun, stanje, stednja, kredit, rata, rok)) return 1;
    if (a == BA_POSALJI)
    {
        BankaPosaljiUnos(playerid, inputtext);
        return BankaKasa(playerid);
    }
    new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 9 ? strval(inputtext) : 0;
    if (n <= 0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Iznos mora biti veci od 0."));
    switch (a)
    {
        case BA_UPLATI:
        {
            if (gNovac[playerid] < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
            DajNovac(playerid, -n);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d WHERE account = %d", n, racun));
            BankaZapis(racun, n, "Uplata gotovine");
        }
        case BA_ISPLATI:
        {
            if (stanje < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
            if (!DajNovac(playerid, n)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d WHERE account = %d", n, racun));
            BankaZapis(racun, -n, "Isplata gotovine");
        }
        case BA_NA_STEDNJU:
        {
            if (stanje < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d, savings = savings + %d, savings_tick = CASE WHEN savings = 0 THEN %d ELSE savings_tick END WHERE account = %d", n, n, gettime(), racun));
            BankaZapis(racun, -n, "Na stednju");
        }
        case BA_SA_STEDNJE:
        {
            if (stednja < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko na stednji."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d, savings = savings - %d WHERE account = %d", n, n, racun));
            BankaZapis(racun, n, "Sa stednje");
        }
        case BA_KREDIT:
        {
            if (kredit) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vec imate kredit."));
            if (StatsPlaytime(playerid) < BANKA_KREDIT_SATI * 3600) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kredit mozete dobiti tek nakon %d sati igre."), BANKA_KREDIT_SATI);
            if (n > BANKA_KREDIT_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najveci kredit je $%d."), BANKA_KREDIT_MAX);
            new dug = BankaDug(n);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d, loan = %d, loan_rate = %d, loan_due = %d WHERE account = %d", n, dug, BankaRata(dug), gettime() + BANKA_DAN, racun));
            BankaZapis(racun, n, "Kredit");
            new poruka[144];
            format(poruka, sizeof poruka, T(playerid, "Kredit odobren: $%d na racun. Dug $%d, rata $%d svaka 24 h."), n, dug, BankaRata(dug));
            MobitelBankaSms(playerid, poruka);
        }
        case BA_OTPLATI: if (!BankaOtplati(playerid, n)) return 1;
    }
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Transakcija uspjesna."));
    return BankaKasa(playerid);
}

ShowBanke(playerid)
{
    new list[1024], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova banka na mojoj poziciji"));
    for (new s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma]) continue;
        format(row, sizeof row, "[%d] %s%s", gBanka[s][bkDb], CestaZona(gBanka[s][bkX], gBanka[s][bkY]), T(playerid, BankaPrviSalter(s) != -1 ? ("") : (" - bez sluzbenika")));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_BANKA, DIALOG_STYLE_LIST, T(playerid, "Banke"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_banka(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Banka se postavlja samo izvana."));
        GetPlayerPos(playerid, gApBankaPoz[playerid][0], gApBankaPoz[playerid][1], gApBankaPoz[playerid][2]);
        GetPlayerFacingAngle(playerid, gApBankaPoz[playerid][3]);
        new list[256], len = 0;
        for (new i = 0; i < sizeof gBankaInt; i++) DodajStupac(list, len, T(playerid, gBankaInt[i][biIme]));
        DodajStupac(list, len, T(playerid, "Prilagodeni (postavite ulaz kasnije na svojoj poziciji)"));
        return PrikaziDialog(playerid, DLG_APANEL_BANKA_INT, DIALOG_STYLE_LIST, T(playerid, "Odaberite interijer"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    new n = listitem;
    for (new s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma] || --n) continue;
        gApBanka[playerid] = s;
        return PrikaziDialog(playerid, DLG_APANEL_BANKA_MENI, DIALOG_STYLE_LIST, T(playerid, "Banke"), T(playerid, "Postavi sluzbenika na mojoj poziciji\nPostavi unutarnji ulaz na mojoj poziciji\nDodaj cuvara na mojoj poziciji\nObrisi sve cuvare\nObrisi banku"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    return ShowBanke(playerid);
}

DLG:apanel_banka_int(playerid, response, listitem)
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!response || listitem < 0 || listitem > sizeof gBankaInt) return ShowBanke(playerid);
    new s = 0, i = listitem < sizeof gBankaInt ? listitem : 0;
    while (s < BANKA_MAX && gBanka[s][bkIma]) s++;
    if (s == BANKA_MAX)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj banaka."));
        return ShowBanke(playerid);
    }
    gBanka[s][bkX] = gApBankaPoz[playerid][0];
    gBanka[s][bkY] = gApBankaPoz[playerid][1];
    gBanka[s][bkZ] = gApBankaPoz[playerid][2];
    gBanka[s][bkA] = gApBankaPoz[playerid][3];
    gBanka[s][bkInt] = gBankaInt[i][biInt];
    gBanka[s][bkIx] = gBankaInt[i][biX];
    gBanka[s][bkIy] = gBankaInt[i][biY];
    gBanka[s][bkIz] = gBankaInt[i][biZ];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO banks (x, y, z, a, interior, ix, iy, iz) VALUES (%f, %f, %f, %f, %d, %f, %f, %f)", gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ], gBanka[s][bkA], gBanka[s][bkInt], gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz]));
    gBanka[s][bkDb] = ZadnjiId();
    BankaStvori(s);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Banka dodana: %s. Udite i postavite sluzbenika."), CestaZona(gBanka[s][bkX], gBanka[s][bkY]));
    return ShowBanke(playerid);
}

static BankaObrisi(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && BankaIgraca(i) == s) BankaVani(i, s);
    gBanka[s][bkPljacka] = 0;
    for (new c = 0; c < BANKA_CUVARI; c++) BankaCuvarMakni(s, c);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_guards WHERE bank = %d", gBanka[s][bkDb]));
    SalteriMakni(s);
    DestroyDynamicPickup(gBanka[s][bkPickup]);
    MapaIkonaMakni(gBanka[s][bkIkona]);
    DestroyDynamicPickup(gBanka[s][bkPickupU]);
    DestroyDynamic3DTextLabel(gBanka[s][bkTekst]);
    DestroyDynamic3DTextLabel(gBanka[s][bkTekstU]);
    gBanka[s][bkIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM banks WHERE id = %d", gBanka[s][bkDb]));
}

DLG:apanel_banka_salter(playerid, response, listitem)
{
    new s = gApBanka[playerid];
    if (!response || !gBanka[s][bkIma] || listitem < 0 || listitem > SK_SVE + 1) return ShowBanke(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (BankaIgraca(playerid) != s)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te banke."));
        return ShowBanke(playerid);
    }
    if (listitem == SK_SVE + 1)
    {
        SalteriMakni(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Svi sluzbenici su uklonjeni."));
        return ShowBanke(playerid);
    }
    new k = -1;
    for (new i = 0; i < BANKA_SALTERI && k == -1; i++) if (gSalterKat[s][i] == listitem) k = i;
    for (new i = 0; i < BANKA_SALTERI && k == -1; i++) if (gSalterKat[s][i] == -1) k = i;
    if (k == -1)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Banka vec ima 4 sluzbenika."));
        return ShowBanke(playerid);
    }
    SalterMakni(s, k);
    gSalterKat[s][k] = listitem;
    GetPlayerPos(playerid, gSalterPoz[s][k][0], gSalterPoz[s][k][1], gSalterPoz[s][k][2]);
    GetPlayerFacingAngle(playerid, gSalterPoz[s][k][3]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO bank_tellers (bank, slot, kat, x, y, z, a) VALUES (%d, %d, %d, %f, %f, %f, %f)", gBanka[s][bkDb], k, listitem, gSalterPoz[s][k][0], gSalterPoz[s][k][1], gSalterPoz[s][k][2], gSalterPoz[s][k][3]));
    SalterStvori(s, k);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Sluzbenik je postavljen."));
    return ShowBanke(playerid);
}

DLG:apanel_banka_meni(playerid, response, listitem)
{
    new s = gApBanka[playerid];
    if (!response || !gBanka[s][bkIma]) return ShowBanke(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        if (BankaIgraca(playerid) != s)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te banke."));
            return ShowBanke(playerid);
        }
        new list[256], len = 0;
        for (new i = 0; i < sizeof gSalterIme; i++) DodajStupac(list, len, T(playerid, gSalterIme[i]));
        DodajStupac(list, len, T(playerid, "Obrisi sve sluzbenike"));
        return PrikaziDialog(playerid, DLG_APANEL_BANKA_SALTER, DIALOG_STYLE_LIST, T(playerid, "Odaberite vrstu saltera"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    else if (listitem == 1)
    {
        if (!GetPlayerInterior(playerid))
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Stanite u interijer koji zelite koristiti."));
            return ShowBanke(playerid);
        }
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && BankaIgraca(i) == s) BankaVani(i, s);
        SalteriMakni(s);
        DestroyDynamicPickup(gBanka[s][bkPickup]);
        MapaIkonaMakni(gBanka[s][bkIkona]);
        DestroyDynamicPickup(gBanka[s][bkPickupU]);
        DestroyDynamic3DTextLabel(gBanka[s][bkTekst]);
        DestroyDynamic3DTextLabel(gBanka[s][bkTekstU]);
        gBanka[s][bkInt] = GetPlayerInterior(playerid);
        GetPlayerPos(playerid, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz]);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE banks SET interior = %d, ix = %f, iy = %f, iz = %f, Aktor = 0 WHERE id = %d", gBanka[s][bkInt], gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz], gBanka[s][bkDb]));
        BankaStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Unutarnji ulaz postavljen. Ponovno postavite sluzbenika."));
    }
    else if (listitem == 2)
    {
        new c = 0;
        while (c < BANKA_CUVARI && gBankaCuvarDb[s][c]) c++;
        if (BankaIgraca(playerid) != s) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te banke."));
        else if (c == BANKA_CUVARI) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Banka vec ima %d cuvara."), BANKA_CUVARI);
        else
        {
            GetPlayerPos(playerid, gBankaCuvarPoz[s][c][0], gBankaCuvarPoz[s][c][1], gBankaCuvarPoz[s][c][2]);
            GetPlayerFacingAngle(playerid, gBankaCuvarPoz[s][c][3]);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_guards (bank, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", gBanka[s][bkDb], gBankaCuvarPoz[s][c][0], gBankaCuvarPoz[s][c][1], gBankaCuvarPoz[s][c][2], gBankaCuvarPoz[s][c][3]));
            gBankaCuvarDb[s][c] = ZadnjiId();
            BankaCuvarStvori(s, c);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Cuvar je postavljen."));
        }
    }
    else if (listitem == 3)
    {
        for (new c = 0; c < BANKA_CUVARI; c++) BankaCuvarMakni(s, c);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_guards WHERE bank = %d", gBanka[s][bkDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Cuvari obrisani."));
    }
    else
    {
        BankaObrisi(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Banka obrisana."));
    }
    return ShowBanke(playerid);
}

BankaLokacija(i, &Float:x, &Float:y, &Float:z, &Float:a)
{
    if (i >= BANKA_MAX) return -1;
    if (!gBanka[i][bkIma]) return 0;
    x = gBanka[i][bkX];
    y = gBanka[i][bkY];
    z = gBanka[i][bkZ];
    a = gBanka[i][bkA];
    return 1;
}
