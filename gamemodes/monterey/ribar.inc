#define RIBAR_BRODOVA       12
#define RIBAR_OTKUPA        8
#define RIBAR_MODEL         453
#define RIBAR_DOMET         2.5
#define RIBAR_MARINA_DOMET  80.0
#define RIBAR_MIRUJE_S      900
#define RIBAR_CILJ_DOMET    15.0
#define RIBAR_RUNDI         3
#define RIBAR_TRAKA_MS      40
#define RIBAR_TRAKA_ZELENO  16.0
#define RIBAR_SKIN          34
#define RIBAR_BOJA          0x2980B9FF
#define RIBAR_SKIN_RADNI    35

static const Float:gRibarZadano[3] = { 715.4897, -1625.8121, 2.4297 };
static const gRibe[] = { ITEM_SKUSA, ITEM_ORADA, ITEM_BRANCIN, ITEM_ZUBATAC, ITEM_TUNA };
static const gRibaCijena[] = { 35, 60, 85, 130, 240 };
static const gRibaSansa[] = { 45, 25, 17, 9, 4 };

enum E_RIBAR_BROD { bool:rbIma, rbDb, rbVozilo, rbIgrac, rbZadnje, Float:rbX, Float:rbY }
static gRibarBrod[RIBAR_BRODOVA][E_RIBAR_BROD];
enum E_RIBAR_OTKUP { bool:roIma, roDb, roAktor, Float:roX, Float:roY, Float:roZ, STREAMER_TAG_3D_TEXT_LABEL:roTekst }
static gRibarOtkup[RIBAR_OTKUPA][E_RIBAR_OTKUP];
static Float:gRibarMjesto[3], gRibarIkona, STREAMER_TAG_3D_TEXT_LABEL:gRibarTekst, bool:gRibarImaMjesto;
static gRibarCilj[MAX_PLAYERS], Float:gRibarCiljPoz[MAX_PLAYERS][3];
static bool:gRibarIgra[MAX_PLAYERS], gRibarTimer[MAX_PLAYERS], gRibarRunda[MAX_PLAYERS], gRibarPogodak[MAX_PLAYERS];
static Float:gRibarTraka[MAX_PLAYERS], Float:gRibarSmjer[MAX_PLAYERS], Float:gRibarZel[MAX_PLAYERS];
static gRibarApPopis[MAX_PLAYERS][RIBAR_BRODOVA + RIBAR_OTKUPA];

InitRibar()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_fisher (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS fish_boats (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS fish_buyers (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    for (new k = 0; k < 3; k++) gRibarMjesto[k] = gRibarZadano[k];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z FROM job_fisher WHERE id = 1");
    if (DB_GetRowCount(res)) for (new k = 0; k < 3; k++) gRibarMjesto[k] = DB_GetFieldFloat(res, k);
    DB_FreeResultSet(res);
    RibarMjestoStvori();
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM fish_boats ORDER BY id LIMIT %d", RIBAR_BRODOVA);
    for (new b = 0, n = DB_GetRowCount(res); b < n; b++, DB_SelectNextRow(res))
        RibarBrodStvori(b, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM fish_buyers ORDER BY id LIMIT %d", RIBAR_OTKUPA);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++, DB_SelectNextRow(res))
        RibarOtkupStvori(k, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
    DB_FreeResultSet(res);
}

static RibarMjestoStvori()
{
    if (gRibarImaMjesto)
    {
        MapaIkonaMakni(gRibarIkona);
        DestroyDynamic3DTextLabel(gRibarTekst);
    }
    gRibarImaMjesto = true;
    gRibarIkona = MapaIkona(gRibarMjesto[0], gRibarMjesto[1], gRibarMjesto[2], 9);
    gRibarTekst = CreateDynamic3DTextLabel("Ribar\n/posao", RIBAR_BOJA, gRibarMjesto[0], gRibarMjesto[1], gRibarMjesto[2] + 0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gRibarTekst, RIBAR_DOMET);
}

static RibarBrodStvori(b, db, Float:x, Float:y, Float:z, Float:a)
{
    gRibarBrod[b][rbIma] = true;
    gRibarBrod[b][rbDb] = db;
    gRibarBrod[b][rbX] = x;
    gRibarBrod[b][rbY] = y;
    gRibarBrod[b][rbIgrac] = INVALID_PLAYER_ID;
    gRibarBrod[b][rbVozilo] = CreateVehicle(RIBAR_MODEL, x, y, z, a, 1, 1, -1);
}

static RibarOtkupStvori(k, db, Float:x, Float:y, Float:z, Float:a)
{
    gRibarOtkup[k][roIma] = true;
    gRibarOtkup[k][roDb] = db;
    gRibarOtkup[k][roX] = x;
    gRibarOtkup[k][roY] = y;
    gRibarOtkup[k][roZ] = z;
    gRibarOtkup[k][roAktor] = CreateActor(RIBAR_SKIN, x, y, z, a);
    SetActorInvulnerable(gRibarOtkup[k][roAktor], true);
    gRibarOtkup[k][roTekst] = CreateDynamic3DTextLabel("Otkupljivac ribe", RIBAR_BOJA, x, y, z + 1.2, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gRibarOtkup[k][roTekst], RIBAR_DOMET);
}

static RibarBrodIgraca(playerid)
{
    for (new b = 0; b < RIBAR_BRODOVA; b++) if (gRibarBrod[b][rbIma] && gRibarBrod[b][rbIgrac] == playerid) return b;
    return -1;
}

static RibarBrodPoVozilu(v)
{
    for (new b = 0; b < RIBAR_BRODOVA; b++) if (gRibarBrod[b][rbIma] && gRibarBrod[b][rbVozilo] == v) return b;
    return -1;
}

static bool:RibarUMarini(playerid, Float:r = RIBAR_DOMET)
{
    return !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, r, gRibarMjesto[0], gRibarMjesto[1], gRibarMjesto[2]);
}

static RibarOtkupBlizu(playerid)
{
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return -1;
    for (new k = 0; k < RIBAR_OTKUPA; k++)
        if (gRibarOtkup[k][roIma] && IsPlayerInRangeOfPoint(playerid, RIBAR_DOMET, gRibarOtkup[k][roX], gRibarOtkup[k][roY], gRibarOtkup[k][roZ])) return k;
    return -1;
}

static RibarUlov(playerid)
{
    new n = 0;
    for (new r = 0; r < sizeof gRibe; r++) n += gInv[playerid][gRibe[r]];
    return n;
}

static RibarIgraKraj(playerid, bool:gotovo)
{
    if (!gRibarIgra[playerid]) return;
    gRibarIgra[playerid] = false;
    if (gRibarTimer[playerid]) KillTimer(gRibarTimer[playerid]);
    gRibarTimer[playerid] = 0;
    IgraTdMakni(playerid);
    if (!gotovo) return;
    if (gRibarPogodak[playerid] < 2)
    {
        IgraObavijest(playerid, T(playerid, "Ribolov"), OBAVIJEST_GRESKA, T(playerid, "Riba se otkacila. Pokusajte na sljedecoj oznaci."));
    }
    else
    {
        new r = RibaNasumicno(), poruka[144];
        if (gRibarPogodak[playerid] == RIBAR_RUNDI) r = max(r, RibaNasumicno());
        if (!GiveItem(playerid, gRibe[r], 1)) IgraObavijest(playerid, T(playerid, "Ribolov"), OBAVIJEST_GRESKA, T(playerid, "Nemate mjesta za tu ribu. Prodajte ulov otkupljivacu ribe."));
        else
        {
            format(poruka, sizeof poruka, T(playerid, "Ulovili ste: %s~n~Ulov prodajte otkupljivacu ribe."), T(playerid, gItems[gRibe[r]][ITEM_NAME]));
            IgraObavijest(playerid, T(playerid, "Ribolov"), RIBAR_BOJA, poruka);
        }
    }
    if (RibarBrodIgraca(playerid) != -1) RibarNoviCilj(playerid);
}

static RibaNasumicno()
{
    new bacanje = random(100);
    for (new r = 0; r < sizeof gRibe; r++)
    {
        if (bacanje < gRibaSansa[r]) return r;
        bacanje -= gRibaSansa[r];
    }
    return 0;
}

static RibarCiljMakni(playerid)
{
    if (gRibarCilj[playerid]) DisablePlayerCheckpoint(playerid);
    gRibarCilj[playerid] = 0;
}

static RibarNoviCilj(playerid)
{
    new Float:px, Float:py, Float:pz, Float:x, Float:y;
    GetPlayerPos(playerid, px, py, pz);
    for (new pokusaj = 0; pokusaj < 10; pokusaj++)
    {
        x = 450.0 + float(random(550));
        y = -2150.0 - float(random(200));
        if (VectorSize(x - px, y - py, 0.0) >= 250.0) break;
    }
    gRibarCiljPoz[playerid][0] = x;
    gRibarCiljPoz[playerid][1] = y;
    gRibarCiljPoz[playerid][2] = 0.0;
    gRibarCilj[playerid] = 1;
    SetPlayerCheckpoint(playerid, x, y, 0.0, RIBAR_CILJ_DOMET);
}

static RibarBrodOslobodi(b)
{
    new i = gRibarBrod[b][rbIgrac];
    gRibarBrod[b][rbIgrac] = INVALID_PLAYER_ID;
    if (i != INVALID_PLAYER_ID && IsPlayerConnected(i))
    {
        RibarIgraKraj(i, false);
        if (gRibarCilj[i] == 1) RibarCiljMakni(i);
        if (GetPlayerSkin(i) == RIBAR_SKIN_RADNI) SetPlayerSkin(i, OdjecaSkinIgraca(i));
    }
    SetVehicleToRespawn(gRibarBrod[b][rbVozilo]);
}

RibarLokacija(&Float:x, &Float:y, &Float:z)
{
    x = gRibarMjesto[0];
    y = gRibarMjesto[1];
    z = gRibarMjesto[2];
    return 1;
}

RibarKraj(playerid)
{
    new b = RibarBrodIgraca(playerid);
    if (b != -1) RibarBrodOslobodi(b);
    RibarIgraKraj(playerid, false);
    RibarCiljMakni(playerid);
}

static RibarUzmiBrod(playerid)
{
    for (new b = 0; b < RIBAR_BRODOVA; b++)
    {
        if (!gRibarBrod[b][rbIma] || gRibarBrod[b][rbIgrac] != INVALID_PLAYER_ID) continue;
        gRibarBrod[b][rbIgrac] = playerid;
        gRibarBrod[b][rbZadnje] = gettime();
        Fade(playerid, 0);
        SetTimerEx("RibarBrodDan", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "iii", playerid, gAccountId[playerid], b);
        return 1;
    }
    return ObavijestGreska(playerid, T(playerid, "Trenutno nema slobodnih brodova."));
}

forward RibarBrodDan(playerid, racun, b);
public RibarBrodDan(playerid, racun, b)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    Fade(playerid, FADE_SLOJEVA + 1);
    if (gRibarBrod[b][rbIgrac] != playerid) return;
    gRibarBrod[b][rbZadnje] = gettime();
    SetPlayerSkin(playerid, RIBAR_SKIN_RADNI);
    PutPlayerInVehicle(playerid, gRibarBrod[b][rbVozilo], 0);
    RibarCiljMakni(playerid);
    RibarNoviCilj(playerid);
    ObavijestInfo(playerid, T(playerid, "Ribar"), T(playerid, "Dobili ste brod. Plovite do oznake na pucini i ulovite ribu. Brod vracate ovdje (/posao)."));
}

static RibarVrati(playerid)
{
    if (RibarBrodIgraca(playerid) == -1) return 0;
    Fade(playerid, 0);
    SetTimerEx("RibarVratiKraj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
    return 1;
}

forward RibarVratiKraj(playerid, racun);
public RibarVratiKraj(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return 0;
    Fade(playerid, FADE_SLOJEVA + 1);
    new b = RibarBrodIgraca(playerid);
    if (b == -1) return 0;
    new bool:uBrodu = GetPlayerVehicleID(playerid) == gRibarBrod[b][rbVozilo];
    RibarBrodOslobodi(b);
    if (uBrodu) SetPlayerPos(playerid, gRibarMjesto[0], gRibarMjesto[1], gRibarMjesto[2]);
    if (!RibarUlov(playerid)) return ObavijestInfo(playerid, T(playerid, "Ribar"), T(playerid, "Vratili ste brod.")), 1;
    new najblizi = -1, Float:najblize = 1.0e9;
    for (new k = 0; k < RIBAR_OTKUPA; k++)
    {
        if (!gRibarOtkup[k][roIma]) continue;
        new Float:d = VectorSize(gRibarOtkup[k][roX] - gRibarMjesto[0], gRibarOtkup[k][roY] - gRibarMjesto[1], 0.0);
        if (d < najblize) najblize = d, najblizi = k;
    }
    if (najblizi != -1)
    {
        RibarCiljMakni(playerid);
        gRibarCilj[playerid] = 2;
        gRibarCiljPoz[playerid][0] = gRibarOtkup[najblizi][roX];
        gRibarCiljPoz[playerid][1] = gRibarOtkup[najblizi][roY];
        gRibarCiljPoz[playerid][2] = gRibarOtkup[najblizi][roZ];
        SetPlayerCheckpoint(playerid, gRibarCiljPoz[playerid][0], gRibarCiljPoz[playerid][1], gRibarCiljPoz[playerid][2], 2.0);
    }
    return ObavijestInfo(playerid, T(playerid, "Ribar"), T(playerid, "Vratili ste brod. Ulov prodajte otkupljivacu ribe (oznaceno na karti).")), 1;
}

static RibarIzbornik(playerid)
{
    if (PosaoIgraca(playerid) != POSAO_RIBAR) return UpitDaNe(playerid, UPIT_RIBAR, 0, 0, T(playerid, "Ribar"), T(playerid, "Zelite li se zaposliti kao ribar?"));
    if (RibarBrodIgraca(playerid) == -1) return UpitDaNe(playerid, UPIT_RIBAR, 0, 1, T(playerid, "Ribar"), T(playerid, "Zelite li uzeti brod?"));
    return UpitDaNe(playerid, UPIT_RIBAR, 0, 2, T(playerid, "Ribar"), T(playerid, "Zelite li vratiti brod?"));
}

RibarUpit(playerid, opcija, bool:da)
{
    if (!da || !RibarUMarini(playerid, RIBAR_DOMET + 2.0)) return 1;
    if (opcija == 0 && PosaoIgraca(playerid) != POSAO_RIBAR)
    {
        PosaoPostavi(playerid, POSAO_RIBAR);
        return ObavijestInfo(playerid, T(playerid, "Ribar"), T(playerid, "Zaposleni ste kao ribar. Pritisnite ALT ovdje (ili /posao) da dobijete brod."));
    }
    if (PosaoIgraca(playerid) != POSAO_RIBAR) return 1;
    if (opcija == 1 && RibarBrodIgraca(playerid) == -1) return RibarUzmiBrod(playerid);
    if (opcija == 2) return RibarVrati(playerid);
    return 1;
}

RibarPosao(playerid)
{
    if (RibarBrodIgraca(playerid) != -1) return RibarVrati(playerid);
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && RibarUMarini(playerid)) return RibarIzbornik(playerid), 1;
    if (PosaoIgraca(playerid) != POSAO_RIBAR) return 0;
    return ObavijestGreska(playerid, T(playerid, "Brod uzimate kod ribara u marini (oznaceno na karti).")), 1;
}

RibarAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    if (RibarUMarini(playerid)) return RibarIzbornik(playerid), 1;
    new k = RibarOtkupBlizu(playerid);
    if (k == -1) return 0;
    new ukupno = 0, n = 0, poruka[128];
    for (new r = 0; r < sizeof gRibe; r++) ukupno += gInv[playerid][gRibe[r]] * gRibaCijena[r], n += gInv[playerid][gRibe[r]];
    if (!n) return ObavijestGreska(playerid, T(playerid, "Nemate ribe za prodaju.")), 1;
    new iznos = BlagajnaIsplati(ukupno);
    if (iznos < ukupno)
    {
        BlagajnaDodaj(iznos);
        return ObavijestGreska(playerid, T(playerid, "Otkupljivac trenutno nema novca. Pokusajte kasnije.")), 1;
    }
    if (!DajNovac(playerid, iznos))
    {
        BlagajnaDodaj(iznos);
        return ObavijestGreska(playerid, T(playerid, "Ne mozete nositi vise novca.")), 1;
    }
    for (new r = 0; r < sizeof gRibe; r++) GiveItem(playerid, gRibe[r], -gInv[playerid][gRibe[r]]);
    if (gRibarCilj[playerid] == 2) RibarCiljMakni(playerid);
    ApplyActorAnimation(gRibarOtkup[k][roAktor], "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0);
    format(poruka, sizeof poruka, T(playerid, "Prodali ste %d riba za $%d."), n, iznos);
    return ObavijestInfo(playerid, T(playerid, "Otkupljivac ribe"), poruka), 1;
}

RibarCheckpoint(playerid)
{
    if (!gRibarCilj[playerid] || !IsPlayerInRangeOfPoint(playerid, RIBAR_CILJ_DOMET + 5.0, gRibarCiljPoz[playerid][0], gRibarCiljPoz[playerid][1], gRibarCiljPoz[playerid][2])) return 0;
    if (gRibarCilj[playerid] == 2)
    {
        RibarCiljMakni(playerid);
        return 1;
    }
    if (gRibarIgra[playerid]) return 1;
    new b = RibarBrodIgraca(playerid);
    if (b == -1 || GetPlayerVehicleID(playerid) != gRibarBrod[b][rbVozilo] || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        return ObavijestGreska(playerid, T(playerid, "Do ribolova morate doci svojim brodom.")), 1;
    RibarCiljMakni(playerid);
    gRibarIgra[playerid] = true;
    gRibarRunda[playerid] = 0;
    gRibarPogodak[playerid] = 0;
    gRibarTraka[playerid] = 0.0;
    gRibarSmjer[playerid] = 2.4;
    gRibarZel[playerid] = float(20 + random(60));
    RibarPanel(playerid);
    gRibarTimer[playerid] = SetTimerEx("RibarTrakaKorak", RIBAR_TRAKA_MS, true, "i", playerid);
    return 1;
}

static RibarPanel(playerid)
{
    new tekst[144];
    format(tekst, sizeof tekst, T(playerid, "Izvlacite mrezu: ~y~SPACE~w~ kad je oznaka u zelenom (%d/%d)."), gRibarRunda[playerid] + 1, RIBAR_RUNDI);
    IgraPanel(playerid, T(playerid, "Ribolov"), 60.0, RIBAR_BOJA);
    IgraTraka(playerid, gRibarZel[playerid], gRibarZel[playerid] + RIBAR_TRAKA_ZELENO, 0x1F3A5FFF, 0x27AE60FF, 0x1F3A5FFF, tekst);
    IgraOznakaPomakni(playerid, gRibarTraka[playerid]);
}

forward RibarTrakaKorak(playerid);
public RibarTrakaKorak(playerid)
{
    if (!gRibarIgra[playerid]) return;
    gRibarTraka[playerid] += gRibarSmjer[playerid];
    if (gRibarTraka[playerid] >= 100.0 || gRibarTraka[playerid] <= 0.0) gRibarSmjer[playerid] = -gRibarSmjer[playerid];
    IgraOznakaPomakni(playerid, gRibarTraka[playerid]);
}

bool:RibarKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gRibarIgra[playerid]) return false;
    if (!(newkeys & KEY_HANDBRAKE) || (oldkeys & KEY_HANDBRAKE)) return true;
    new bool:pogodak = gRibarTraka[playerid] >= gRibarZel[playerid] && gRibarTraka[playerid] <= gRibarZel[playerid] + RIBAR_TRAKA_ZELENO;
    if (pogodak) gRibarPogodak[playerid]++;
    PlayerPlaySound(playerid, pogodak ? 1056 : 1055, 0.0, 0.0, 0.0);
    if (++gRibarRunda[playerid] >= RIBAR_RUNDI)
    {
        RibarIgraKraj(playerid, true);
        return true;
    }
    gRibarSmjer[playerid] *= 1.2;
    gRibarZel[playerid] = float(10 + random(70));
    RibarPanel(playerid);
    return true;
}

RibarStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
    {
        new m = RibarBrodIgraca(playerid), Float:x, Float:y, Float:z;
        if (m == -1) return;
        GetVehiclePos(gRibarBrod[m][rbVozilo], x, y, z);
        if (!IsPlayerInRangeOfPoint(playerid, 10.0, x, y, z)) return;
        if (RibarUMarini(playerid, RIBAR_MARINA_DOMET))
        {
            RibarVrati(playerid);
            return;
        }
        PutPlayerInVehicle(playerid, gRibarBrod[m][rbVozilo], 0);
        ObavijestGreska(playerid, T(playerid, "Brod ne mozete napustiti na moru. Vratite ga u marinu."));
        return;
    }
    if (newstate != PLAYER_STATE_DRIVER) return;
    new b = RibarBrodPoVozilu(GetPlayerVehicleID(playerid));
    if (b == -1) return;
    if (gRibarBrod[b][rbIgrac] == playerid)
    {
        gRibarBrod[b][rbZadnje] = gettime();
        return;
    }
    RemovePlayerFromVehicle(playerid);
    ObavijestGreska(playerid, gRibarBrod[b][rbIgrac] == INVALID_PLAYER_ID ? T(playerid, "Brod uzimate kod ribara u marini (oznaceno na karti).") : T(playerid, "Ovaj brod koristi drugi ribar."));
}

RibarTick()
{
    new sad = gettime();
    for (new b = 0; b < RIBAR_BRODOVA; b++)
    {
        new i = gRibarBrod[b][rbIgrac];
        if (!gRibarBrod[b][rbIma] || i == INVALID_PLAYER_ID) continue;
        if (!IsPlayerConnected(i) || PosaoIgraca(i) != POSAO_RIBAR || IsRanjen(i))
        {
            RibarKraj(i);
            if (IsPlayerConnected(i)) ObavijestGreska(i, T(i, "Smjena je zavrsena. Brod je vracen u marinu."));
            continue;
        }
        if (GetPlayerVehicleID(i) == gRibarBrod[b][rbVozilo])
        {
            gRibarBrod[b][rbZadnje] = sad;
            continue;
        }
        if (gRibarIgra[i]) RibarIgraKraj(i, false);
        if (sad - gRibarBrod[b][rbZadnje] < RIBAR_MIRUJE_S) continue;
        RibarBrodOslobodi(b);
        ObavijestGreska(i, T(i, "Brod je vracen u marinu jer ga 15 minuta niste koristili."));
    }
}

ShowRibarAdmin(playerid)
{
    new list[2048], row[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto zaposlenja ribara na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Brod za ribare na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Otkupljivac ribe na mojoj poziciji"));
    for (new b = 0; b < RIBAR_BRODOVA; b++)
    {
        if (!gRibarBrod[b][rbIma]) continue;
        format(row, sizeof row, T(playerid, "Brod [%d] %s - obrisi"), gRibarBrod[b][rbDb], CestaZona(gRibarBrod[b][rbX], gRibarBrod[b][rbY]));
        DodajStupac(list, len, row);
        gRibarApPopis[playerid][n++] = b;
    }
    for (new k = 0; k < RIBAR_OTKUPA; k++)
    {
        if (!gRibarOtkup[k][roIma]) continue;
        format(row, sizeof row, T(playerid, "Otkupljivac ribe [%d] %s - obrisi"), gRibarOtkup[k][roDb], CestaZona(gRibarOtkup[k][roX], gRibarOtkup[k][roY]));
        DodajStupac(list, len, row);
        gRibarApPopis[playerid][n++] = RIBAR_BRODOVA + k;
    }
    return PrikaziDialog(playerid, DLG_APANEL_RIBAR, DIALOG_STYLE_LIST, T(playerid, "Ribari"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_ribar(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new Float:x, Float:y, Float:z, Float:a;
    if (listitem <= 2)
    {
        if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ovo se postavlja samo izvana.")), ShowRibarAdmin(playerid);
        new v = GetPlayerVehicleID(playerid);
        if (v) GetVehiclePos(v, x, y, z), GetVehicleZAngle(v, a);
        else GetPlayerPos(playerid, x, y, z), GetPlayerFacingAngle(playerid, a);
    }
    if (listitem == 0)
    {
        gRibarMjesto[0] = x;
        gRibarMjesto[1] = y;
        gRibarMjesto[2] = z;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO job_fisher (id, x, y, z) VALUES (1, %f, %f, %f)", x, y, z));
        RibarMjestoStvori();
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto zaposlenja ribara postavljeno."));
        return ShowRibarAdmin(playerid);
    }
    if (listitem == 1)
    {
        new b = 0;
        while (b < RIBAR_BRODOVA && gRibarBrod[b][rbIma]) b++;
        if (b == RIBAR_BRODOVA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowRibarAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO fish_boats (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        RibarBrodStvori(b, ZadnjiId(), x, y, z, a);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Brod za ribare postavljen."));
        return ShowRibarAdmin(playerid);
    }
    if (listitem == 2)
    {
        new k = 0;
        while (k < RIBAR_OTKUPA && gRibarOtkup[k][roIma]) k++;
        if (k == RIBAR_OTKUPA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowRibarAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO fish_buyers (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        RibarOtkupStvori(k, ZadnjiId(), x, y, z, a);
        SetPlayerPos(playerid, x + 1.0, y, z);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Otkupljivac ribe postavljen."));
        return ShowRibarAdmin(playerid);
    }
    new i = listitem - 3;
    if (i < 0 || i >= sizeof gRibarApPopis[]) return 1;
    new k = gRibarApPopis[playerid][i];
    if (k < RIBAR_BRODOVA)
    {
        if (!gRibarBrod[k][rbIma]) return ShowRibarAdmin(playerid);
        if (gRibarBrod[k][rbIgrac] != INVALID_PLAYER_ID) RibarBrodOslobodi(k);
        DestroyVehicle(gRibarBrod[k][rbVozilo]);
        gRibarBrod[k][rbIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM fish_boats WHERE id = %d", gRibarBrod[k][rbDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Brod za ribare obrisan."));
        return ShowRibarAdmin(playerid);
    }
    k -= RIBAR_BRODOVA;
    if (!gRibarOtkup[k][roIma]) return ShowRibarAdmin(playerid);
    DestroyActor(gRibarOtkup[k][roAktor]);
    DestroyDynamic3DTextLabel(gRibarOtkup[k][roTekst]);
    gRibarOtkup[k][roIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM fish_buyers WHERE id = %d", gRibarOtkup[k][roDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Otkupljivac ribe obrisan."));
    return ShowRibarAdmin(playerid);
}
