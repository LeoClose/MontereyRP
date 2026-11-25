#define TAKSI_FLOTA         16
#define TAKSI_MAX_TOCAKA    1536
#define TAKSI_PROSJEK       11.0
#define TAKSI_PREMIUM       4
#define TAKSI_SLOTOVA       (TAKSI_FLOTA + TAKSI_PREMIUM)
#define TAKSI_MODEL         420
#define TAKSI_SKIN          262
#define TAKSI_VIDIK_BLIZU   12.0
#define TAKSI_RUB_RAVNO     6.0
#define TAKSI_RUB_PRIJELAZ  30.0
#define TAKSI_RUB_Z         0.15
#define TAKSI_RUB           2.1
#define TAKSI_BLIZU         50.0
#define TAKSI_POLICIJA_MS   20000
#define TAKSI_RUB_PRVI      9.0
#define TAKSI_RUB_DRUGI     6.0
#define TAKSI_RUB_SPORO     3.0
#define TAKSI_NAPOJNICA_POSTO 80
#define TAKSI_STANI_MS      5000
#define TAKSI_METAR_BOJA    0xF1C40FE6
#define TAKSI_KONTROLE_MS   6000
#define TAKSI_NAPOJNICA_BRZE 1.3
#define TAKSI_CEKA_MS       8000
#define TAKSI_PROGURAJ_MS   8000
#define TAKSI_CEKANJE_MS    60000
#define TAKSI_ODREDISTE_MS  120000
#define TAKSI_IZLAZAK_MS    3000
#define TAKSI_SKRIVENO      250.0
#define TAKSI_MAX_OD_CESTE  150.0
#define TAKSI_MIN_VOZNJA    50
#define TAKSI_START         4
#define TAKSI_PO_KM         2
#define PREMIUM_START       6
#define PREMIUM_PO_KM       3
#define PREMIUM_MODEL       579
#define PREMIUM_ZNAK        19308
#define PREMIUM_ZNAK_Z      1.32
#define PREMIUM_SKIN        165
#define PREMIUM_METARA      25.0
#define PREMIUM_METARA_MAX  100.0
#define PREMIUM_VOZNJA_MS   6000
#define PREMIUM_PRIJE_CILJA 40.0
#define PREMIUM_VIDIK       100.0
#define PREMIUM_ODLAZAK_MS  5000
#define COLOR_POMOC         0x8AB4F8FF
#define COLOR_SMS           0xF5C542FF
#define TAKSI_MOBITEL_MS    4500
#define TAKSI_IGRAC_MS      20000
#define TAKSI_ODLAZAK_M     200.0
#define FADE_SLOJEVA        8
#define FADE_KORAK_MS       50

enum { TX_SLOBODAN, TX_DOLAZI, TX_CEKA, TX_KARTA, TX_POTVRDA, TX_VOZI, TX_TELEPORT, TX_IZLAZ, TX_ODLAZI, TX_KUCI, TX_KRAJ, TX_NESTAO, TX_NEMA }
static Float:gTaksiPoziv[MAX_PLAYERS][2];
enum E_TAKSI { tStanje, tMusterija, tNpc, tVozilo, tZnak, tBroj, tIdx, tCijena, tRok, tKlik, tSkok, tCeka, tProguraj, tSporo, tDb, tStaje, tRub, tMetar, bool:tZavrsi, bool:tBrzo, bool:tObilazi, Float:tCiljX, Float:tCiljY }
static gTaksi[TAKSI_SLOTOVA][E_TAKSI];
static Float:gTaksiTocke[TAKSI_SLOTOVA][TAKSI_MAX_TOCAKA][3], Float:gTaksiBuf[TAKSI_MAX_TOCAKA * 3], Float:gTaksiKuca[TAKSI_SLOTOVA][3];
static gTaksiOd[MAX_PLAYERS], gTaksiCeka[MAX_PLAYERS], gTaksiPonuda[MAX_PLAYERS], bool:gTaksiVozac[MAX_PLAYERS];
static bool:gTaksiMetar[MAX_PLAYERS];
static Text:gFade[FADE_SLOJEVA];

static const Float:gTaksiStajaliste[][2] = {
    { 2380.0, -1660.0 }, { 2070.0, -1760.0 }, { 2330.0, -1370.0 }, { 2400.0, -1960.0 },
    { 2200.0, -2220.0 }, { 1690.0, -2319.0 }, { 1820.0, -1850.0 }, { 1500.0, -1730.0 },
    { 1130.0, -1400.0 }, {  800.0, -1340.0 }, {  480.0, -1500.0 }, {  420.0, -1780.0 },
    { 1010.0, -1130.0 }, { 2050.0, -1250.0 }
};
static Float:gTaksiPoza[TAKSI_FLOTA][4];
static gApTaksiDb[MAX_PLAYERS];

static const Float:gTaksiBaza[][3] = {
    { 2683.8, -2504.0, 13.2 }, { 1799.1, -2670.0, 5.6 }, { 1154.6, -859.0, 47.0 },
    { 2697.0, -1086.0, 69.1 }, { 1045.9, -2093.0, 12.7 }, { 2899.4, -1510.8, 10.7 }
};

static TaksiMotor(s, motor)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(gTaksi[s][tVozilo], e, l, a, d, bn, bt, o);
    SetVehicleParamsEx(gTaksi[s][tVozilo], motor, motor, a, d, bn, bt, o);
}

static TaksiPrimiRutu(s, n, &sekundi)
{
    if (n < 2) return -1;
    new Float:m = 0.0;
    for (new i = 0; i < n; i++)
    {
        gTaksiTocke[s][i][0] = gTaksiBuf[i * 3];
        gTaksiTocke[s][i][1] = gTaksiBuf[i * 3 + 1];
        gTaksiTocke[s][i][2] = gTaksiBuf[i * 3 + 2];
        if (i) m += VectorSize(gTaksiTocke[s][i][0] - gTaksiTocke[s][i - 1][0], gTaksiTocke[s][i][1] - gTaksiTocke[s][i - 1][1], 0.0);
    }
    gTaksi[s][tBroj] = n;
    sekundi = floatround(m / TAKSI_PROSJEK, floatround_ceil);
    return floatround(m * 10.0);
}

static TaksiSmjer(s, &Float:x, &Float:y, &Float:z, &Float:fx, &Float:fy)
{
    new Float:a;
    VoziloSmjer(gTaksi[s][tVozilo], x, y, z, fx, fy);
    if (!Lik_DajKut(gTaksi[s][tNpc], a)) return;
    fx = -floatsin(a, degrees);
    fy = floatcos(a, degrees);
}

static TaksiNastavi(s)
{
    new Float:x, Float:y, Float:z, Float:fx, Float:fy;
    TaksiSmjer(s, x, y, z, fx, fy);
    while (gTaksi[s][tIdx] < gTaksi[s][tBroj] - 1)
    {
        new i = gTaksi[s][tIdx], Float:dx = gTaksiTocke[s][i][0] - x, Float:dy = gTaksiTocke[s][i][1] - y;
        if (dx * fx + dy * fy > 4.0 || VectorSize(dx, dy, 0.0) > 30.0) break;
        gTaksi[s][tIdx]++;
    }
}

static TaksiSlobodno(Float:x, Float:y, Float:z, Float:px, Float:py, Float:pz)
{
    new Float:dx = px - x, Float:dy = py - y, Float:d = VectorSize(dx, dy, 0.0);
    if (d < 0.1) return 1;
    new Float:bx = -dy / d * 0.9, Float:by = dx / d * 0.9;
    return !Kolizija_Zraka(x, y, z, px, py, pz) && !Kolizija_Zraka(x + bx, y + by, z, px + bx, py + by, pz) && !Kolizija_Zraka(x - bx, y - by, z, px - bx, py - by, pz);
}

static TaksiUzRub(s, n)
{
    if (n < 2) return;
    new Float:dx = gTaksiTocke[s][n - 1][0] - gTaksiTocke[s][n - 2][0], Float:dy = gTaksiTocke[s][n - 1][1] - gTaksiTocke[s][n - 2][1], Float:dd = VectorSize(dx, dy, 0.0);
    if (dd < 0.01) return;
    dx /= dd;
    dy /= dd;
    new Float:ex = gTaksiTocke[s][n - 1][0] + dy * TAKSI_RUB, Float:ey = gTaksiTocke[s][n - 1][1] - dx * TAKSI_RUB;
    for (new i = n - 1; i >= 1; i--)
    {
        new Float:uz = (ex - gTaksiTocke[s][i][0]) * dx + (ey - gTaksiTocke[s][i][1]) * dy;
        if (uz > TAKSI_RUB_PRIJELAZ) break;
        new Float:f = uz <= TAKSI_RUB_RAVNO ? 1.0 : (TAKSI_RUB_PRIJELAZ - uz) / (TAKSI_RUB_PRIJELAZ - TAKSI_RUB_RAVNO);
        gTaksiTocke[s][i][0] += (ex - dx * uz - gTaksiTocke[s][i][0]) * f;
        gTaksiTocke[s][i][1] += (ey - dy * uz - gTaksiTocke[s][i][1]) * f;
        gTaksiTocke[s][i][2] += TAKSI_RUB_Z * f;
    }
}

static TaksiVozilo(s, Float:x, Float:y, Float:z, Float:a)
{
    new premium = s >= TAKSI_FLOTA;
    new v = CreateVehicle(premium ? PREMIUM_MODEL : TAKSI_MODEL, x, y, z, a, premium ? 0 : 6, premium ? 0 : 1, -1);
    gTaksi[s][tVozilo] = v;
    if (v == INVALID_VEHICLE_ID) return;
    SetVehicleParamsEx(v, 0, 0, 0, 0, 0, 0, 0);
    if (premium)
    {
        gTaksi[s][tZnak] = _:CreateDynamicObject(PREMIUM_ZNAK, x, y, z, 0.0, 0.0, 0.0);
        AttachDynamicObjectToVehicle(STREAMER_TAG_OBJECT:gTaksi[s][tZnak], v, 0.0, -0.2, PREMIUM_ZNAK_Z, 0.0, 0.0, 0.0);
    }
}

static bool:TaksiStanica(s, Float:x, Float:y, Float:z)
{
    new Float:kx, Float:ky, Float:kz, sek;
    if (Lik_Cesta(x, y, z, kx, ky, kz) > TAKSI_MAX_OD_CESTE || TaksiPrimiRutu(s, Lik_RutaDo(kx, ky, kz, 40.0, gTaksiBuf), sek) <= 0) return false;
    new n = gTaksi[s][tBroj];
    TaksiUzRub(s, n);
    gTaksiKuca[s][0] = kx;
    gTaksiKuca[s][1] = ky;
    gTaksiKuca[s][2] = kz;
    gTaksiPoza[s][0] = gTaksiTocke[s][n - 1][0];
    gTaksiPoza[s][1] = gTaksiTocke[s][n - 1][1];
    gTaksiPoza[s][2] = gTaksiTocke[s][n - 1][2];
    gTaksiPoza[s][3] = atan2(gTaksiTocke[s][n - 1][1] - gTaksiTocke[s][n - 2][1], gTaksiTocke[s][n - 1][0] - gTaksiTocke[s][n - 2][0]) - 90.0;
    TaksiVozilo(s, gTaksiPoza[s][0], gTaksiPoza[s][1], gTaksiPoza[s][2], gTaksiPoza[s][3]);
    gTaksi[s][tStanje] = gTaksi[s][tVozilo] == INVALID_VEHICLE_ID ? TX_NESTAO : TX_SLOBODAN;
    return true;
}

static TaksiUcitajStanice()
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = 'taxi_stands'");
    new ima = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    if (!ima)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE taxi_stands (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
        for (new i = 0; i < sizeof gTaksiStajaliste; i++)
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO taxi_stands (x, y, z) VALUES (%f, %f, 0.0)", gTaksiStajaliste[i][0], gTaksiStajaliste[i][1]));
    }
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z FROM taxi_stands ORDER BY id LIMIT %d", TAKSI_FLOTA);
    new s = 0;
    for (new i = DB_GetRowCount(res); i > 0; i--)
    {
        gTaksi[s][tDb] = DB_GetFieldInt(res, 0);
        if (TaksiStanica(s, DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3))) s++;
        else printf("[MRP] Taksi stanica %d nije blizu ceste.", DB_GetFieldInt(res, 0));
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static TaksiLimitStanica()
{
    new l = GetConsoleVarAsInt("max_bots") - TAKSI_PREMIUM;
    return l > TAKSI_FLOTA ? TAKSI_FLOTA : l;
}

static TaksiBrojStanica()
{
    new n = 0;
    for (new s = 0; s < TAKSI_FLOTA; s++) if (gTaksi[s][tStanje] != TX_NEMA) n++;
    return n;
}

ShowTaksiStanice(playerid)
{
    new list[1024], row[64], title[64], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova stanica na mojoj poziciji"));
    for (new s = 0; s < TAKSI_FLOTA; s++)
    {
        if (gTaksi[s][tStanje] == TX_NEMA) continue;
        format(row, sizeof row, "[%d] %s", gTaksi[s][tDb], CestaZona(gTaksiPoza[s][0], gTaksiPoza[s][1]));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, T(playerid, "Taksi stanice (%d/%d)"), TaksiBrojStanica(), TaksiLimitStanica());
    return PrikaziDialog(playerid, DLG_APANEL_TAKSI, DIALOG_STYLE_LIST, title, list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static TaksiNovaStanica(playerid)
{
    new Float:x, Float:y, Float:z, s = 0, limit = TaksiLimitStanica();
    if (TaksiBrojStanica() >= limit) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dostignut je limit taksi stanica (%d)."), limit);
    GetPlayerPos(playerid, x, y, z);
    while (s < TAKSI_FLOTA && gTaksi[s][tStanje] != TX_NEMA) s++;
    if (s == TAKSI_FLOTA || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || !TaksiStanica(s, x, y, z))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Taksi stanica se ovdje ne moze stvoriti."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO taxi_stands (x, y, z) VALUES (%f, %f, %f)", x, y, z));
    gTaksi[s][tDb] = ZadnjiId();
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Taksi stanica dodana: %s."), CestaZona(x, y));
}

DLG:apanel_taksi(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        TaksiNovaStanica(playerid);
        return ShowTaksiStanice(playerid);
    }
    new n = listitem;
    for (new s = 0; s < TAKSI_FLOTA; s++)
    {
        if (gTaksi[s][tStanje] == TX_NEMA || --n) continue;
        new body[96];
        gApTaksiDb[playerid] = gTaksi[s][tDb];
        format(body, sizeof body, T(playerid, "Obrisati taksi stanicu: %s?"), CestaZona(gTaksiPoza[s][0], gTaksiPoza[s][1]));
        return PrikaziDialog(playerid, DLG_APANEL_TAKSI_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Taksi stanice"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowTaksiStanice(playerid);
}

DLG:apanel_taksi_brisi(playerid, response)
{
    if (!response) return ShowTaksiStanice(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new s = 0;
    while (s < TAKSI_FLOTA && (gTaksi[s][tStanje] == TX_NEMA || gTaksi[s][tDb] != gApTaksiDb[playerid])) s++;
    if (s == TAKSI_FLOTA) return ShowTaksiStanice(playerid);
    if (gTaksi[s][tStanje] != TX_SLOBODAN && gTaksi[s][tStanje] != TX_NESTAO)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Taksi je trenutno u voznji, pokusajte kasnije."));
        return ShowTaksiStanice(playerid);
    }
    TaksiBezVozaca(s);
    DestroyVehicle(gTaksi[s][tVozilo]);
    gTaksi[s][tVozilo] = INVALID_VEHICLE_ID;
    gTaksi[s][tStanje] = TX_NEMA;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM taxi_stands WHERE id = %d", gApTaksiDb[playerid]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Taksi stanica obrisana."));
    return ShowTaksiStanice(playerid);
}

InitTaksi()
{
    new va = CreateVehicle(TAKSI_MODEL, 0.0, 0.0, 3.0, 0.0, 0, 0, -1), vb = CreateVehicle(TAKSI_MODEL, 5.0, 0.0, 3.0, 0.0, 0, 0, -1), vn = gVozilaCount;
    DestroyVehicle(va);
    assert(gVozilaCount == vn - 1 && !gVoziloMjesto[va] && gVozila[gVoziloMjesto[vb] - 1] == vb);
    DestroyVehicle(vb);
    for (new s = 0; s < TAKSI_SLOTOVA; s++)
    {
        gTaksi[s][tVozilo] = INVALID_VEHICLE_ID;
        gTaksi[s][tNpc] = INVALID_LIK;
        gTaksi[s][tMusterija] = INVALID_PLAYER_ID;
        if (s < TAKSI_FLOTA) gTaksi[s][tStanje] = TX_NEMA;
    }
    TaksiUcitajStanice();
    for (new i = 0; i < FADE_SLOJEVA; i++)
    {
        gFade[i] = TextDrawCreate(-20.0, -20.0, "_");
        TextDrawLetterSize(gFade[i], 0.0, 60.0);
        TextDrawTextSize(gFade[i], 660.0, 0.0);
        TextDrawUseBox(gFade[i], true);
        TextDrawBoxColour(gFade[i], i == FADE_SLOJEVA - 1 ? 0x000000FF : 0x00000040);
    }
    SetTimer("TaksiPrepreke", 250, true);
}

static bool:TaksiVidljivo(Float:x, Float:y, Float:z, Float:radijus = TAKSI_SKRIVENO)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && IsPlayerInRangeOfPoint(i, radijus, x, y, z)) return true;
    return false;
}

static bool:TaksiNaVidiku(Float:x, Float:y, Float:z, osimVozila = INVALID_VEHICLE_ID, Float:radijus = TAKSI_SKRIVENO)
{
    return !Lik_Skriveno(x, y, z, TAKSI_VIDIK_BLIZU, radijus, osimVozila == INVALID_VEHICLE_ID ? -1 : osimVozila);
}

static bool:TaksiNoviVozac(s)
{
    new Float:x = 0.0, Float:y = 0.0, Float:z = -50.0;
    if (gTaksi[s][tVozilo] != INVALID_VEHICLE_ID && GetVehicleModel(gTaksi[s][tVozilo])) GetVehiclePos(gTaksi[s][tVozilo], x, y, z);
    new lik = Lik_Stvori(s >= TAKSI_FLOTA ? PREMIUM_SKIN : TAKSI_SKIN, x, y, z);
    if (lik == INVALID_LIK) return false;
    Lik_Neranjiv(lik, true);
    gTaksi[s][tNpc] = lik;
    return true;
}

static TaksiVozacUdji(s)
{
    Lik_UVozilo(gTaksi[s][tNpc], gTaksi[s][tVozilo], 0);
    SetTimerEx("TaksiKreni", 1000, false, "i", s);
}

TaksiJeLik(lik)
{
    return TaksiLika(lik) >= 0;
}

static TaksiLika(lik)
{
    if (lik == INVALID_LIK) return -1;
    for (new s = 0; s < TAKSI_SLOTOVA; s++) if (gTaksi[s][tNpc] == lik) return s;
    return -1;
}

static TaksiNajblizaBaza(Float:x, Float:y)
{
    new best = 0, Float:bd = 1.0e12;
    for (new b = 0; b < sizeof gTaksiBaza; b++)
    {
        new Float:d = VectorSize(gTaksiBaza[b][0] - x, gTaksiBaza[b][1] - y, 0.0);
        if (d < bd && !TaksiNaVidiku(gTaksiBaza[b][0], gTaksiBaza[b][1], gTaksiBaza[b][2])) { bd = d; best = b; }
    }
    return best;
}

static TaksiPutOdVozila(s, Float:tx, Float:ty, Float:tz, &sekundi = 0)
{
    new Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(gTaksi[s][tVozilo], x, y, z);
    if (!Lik_DajKut(gTaksi[s][tNpc], a)) GetVehicleZAngle(gTaksi[s][tVozilo], a);
    new n = Lik_Ruta(x, y, a, tx, ty, tz, gTaksiBuf);
    if (n == 1)
    {
        gTaksiBuf[3] = gTaksiBuf[0];
        gTaksiBuf[4] = gTaksiBuf[1];
        gTaksiBuf[5] = gTaksiBuf[2];
        gTaksiBuf[0] = x;
        gTaksiBuf[1] = y;
        gTaksiBuf[2] = z;
        n = 2;
    }
    return TaksiPrimiRutu(s, n, sekundi);
}

static TaksiKorak(s)
{
    new n = gTaksi[s][tBroj] - 1;
    if (n < 0) return;
    Lik_Vozi(gTaksi[s][tNpc], gTaksiTocke[s][n][0], gTaksiTocke[s][n][1], gTaksiTocke[s][n][2], gTaksi[s][tBrzo] ? 13.0 * TAKSI_NAPOJNICA_BRZE : 13.0, false);
}

static TaksiVozi(s)
{
    Lik_Stani(gTaksi[s][tNpc]);
    TaksiMotor(s, 1);
    gTaksi[s][tCeka] = 0;
    gTaksi[s][tObilazi] = false;
    gTaksi[s][tStaje] = 0;
    gTaksi[s][tSporo] = 100;
    gTaksi[s][tBrzo] = false;
    gTaksi[s][tIdx] = 1;
    TaksiKorak(s);
}

TaksiPoVozilu(veh)
{
    for (new s = 0; s < TAKSI_SLOTOVA; s++) if (gTaksi[s][tVozilo] == veh) return s;
    return -1;
}

static TaksiVozac(s, p, const tekst[], const a[] = "", const b[] = "")
{
    #pragma unused s
    PosaljiBlizu(p, CHAT_RANGE, COLOR_CHAT, tekst, a, b, 1);
}

static TaksiDispecer(p, const tekst[], const a[] = "")
{
    SendClientMessage(p, COLOR_CHAT, T(p, tekst), a);
}

static TaksiSms(p, const tekst[])
{
    MobitelTaksiSms(p, T(p, tekst));
}

BlackScreen(playerid, bool:crn)
{
    if (crn) TextDrawShowForPlayer(playerid, gFade[FADE_SLOJEVA - 1]);
    else TextDrawHideForPlayer(playerid, gFade[FADE_SLOJEVA - 1]);
}

forward Fade(playerid, korak);
public Fade(playerid, korak)
{
    if (!IsPlayerConnected(playerid) || korak == FADE_SLOJEVA) return;
    if (korak < FADE_SLOJEVA) TextDrawShowForPlayer(playerid, gFade[korak]);
    else TextDrawHideForPlayer(playerid, gFade[2 * FADE_SLOJEVA - korak]);
    if (korak < 2 * FADE_SLOJEVA) SetTimerEx("Fade", FADE_KORAK_MS, false, "ii", playerid, korak + 1);
}

static TaksiIsprazni(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && IsPlayerInVehicle(i, gTaksi[s][tVozilo])) RemovePlayerFromVehicle(i);
}

TaksiPrekini(s)
{
    new p = gTaksi[s][tMusterija];
    TaksiMetarMakni(s);
    if (p != INVALID_PLAYER_ID)
    {
        SendClientMessage(p, COLOR_POMOC, T(p, "Pomoc: Taksi je unisten, voznja je otkazana."));
        gTaksiOd[p] = 0;
        gTaksi[s][tMusterija] = INVALID_PLAYER_ID;
    }
    gTaksi[s][tStanje] = TX_KRAJ;
}

static TaksiOtidi(s)
{
    TaksiIsprazni(s);
    new p = gTaksi[s][tMusterija], Float:x, Float:y, Float:z, Float:cx = gTaksiKuca[s][0], Float:cy = gTaksiKuca[s][1], Float:cz = gTaksiKuca[s][2];
    SetVehicleParamsForPlayer(gTaksi[s][tVozilo], p, 0, 0);
    gTaksiOd[p] = 0;
    gTaksi[s][tMusterija] = INVALID_PLAYER_ID;
    gTaksi[s][tStanje] = TX_ODLAZI;
    gTaksi[s][tRok] = GetTickCount();
    GetVehiclePos(gTaksi[s][tVozilo], x, y, z);
    if (s < TAKSI_FLOTA && VectorSize(x - gTaksiPoza[s][0], y - gTaksiPoza[s][1], 0.0) < 25.0)
    {
        Lik_Stani(gTaksi[s][tNpc]);
        gTaksi[s][tStanje] = TX_KUCI;
        return;
    }
    if (s >= TAKSI_FLOTA)
    {
        new b = TaksiNajblizaBaza(x, y);
        Lik_Cesta(gTaksiBaza[b][0], gTaksiBaza[b][1], gTaksiBaza[b][2], cx, cy, cz);
    }
    if (TaksiPutOdVozila(s, cx, cy, cz) < 0)
    {
        gTaksi[s][tStanje] = TX_KRAJ;
        return;
    }
    if (s < TAKSI_FLOTA) TaksiUzRub(s, gTaksi[s][tBroj]);
    TaksiVozi(s);
}

static TaksiOstavi(s)
{
    TaksiMetarMakni(s);
    if (gTaksi[s][tMusterija] != INVALID_PLAYER_ID) UpitMakni(gTaksi[s][tMusterija], UPIT_TAKSI);
    gTaksi[s][tStanje] = TX_IZLAZ;
    gTaksi[s][tRok] = GetTickCount() + TAKSI_IZLAZAK_MS;
    TaksiIsprazni(s);
}

static TaksiNajbliziSlobodan(Float:x, Float:y)
{
    new best = -1, Float:bd = 1.0e12, Float:vx, Float:vy, Float:vz;
    for (new s = 0; s < TAKSI_FLOTA; s++)
    {
        if (gTaksi[s][tStanje] != TX_SLOBODAN && (gTaksi[s][tStanje] != TX_ODLAZI || gTaksi[s][tNpc] == INVALID_LIK)) continue;
        GetVehiclePos(gTaksi[s][tVozilo], vx, vy, vz);
        new Float:d = VectorSize(vx - x, vy - y, 0.0);
        if (d < bd) { bd = d; best = s; }
    }
    return best;
}

static PremiumPrilaz(s, Float:cx, Float:cy, Float:cz, &sekundi)
{
    new duljina = -1;
    for (new Float:m = PREMIUM_METARA; m <= PREMIUM_METARA_MAX; m += 15.0)
    {
        duljina = TaksiPrimiRutu(s, Lik_RutaDo(cx, cy, cz, m, gTaksiBuf), sekundi);
        if (duljina >= 0 && !TaksiNaVidiku(gTaksiTocke[s][0][0], gTaksiTocke[s][0][1], gTaksiTocke[s][0][2])) break;
    }
    return duljina;
}

TaksiPozovi(playerid)
{
    if (IsRanjen(playerid)) return 1;
    if (!gUlogiran[playerid]) return 1;
    if (gTaksiOd[playerid] || gTaksiCeka[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vec ste pozvali taksi."));
    if (IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Izadite iz vozila da biste pozvali taksi."));
    new Float:x, Float:y, Float:z, Float:d;
    GetPlayerPos(playerid, x, y, z);
    new Float:cx, Float:cy, Float:cz;
    d = Lik_Cesta(x, y, z, cx, cy, cz);
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || d > TAKSI_MAX_OD_CESTE)
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Taksi sluzba radi samo na ulicama Los Santosa."));
    if (TaksiPonudiIgracima(playerid, x, y, z))
    {
        gTaksiPoziv[playerid][0] = x;
        gTaksiPoziv[playerid][1] = y;
        MobitelPoziv(playerid, T(playerid, "Taksi"));
        SetTimerEx("TaksiSpustiMobitel", TAKSI_MOBITEL_MS, false, "i", playerid);
        Govor(playerid, "%s kaze (telefon): Dobar dan, trebao bih taksi. Nalazim se u %s.", GetRPIme(playerid), CestaZona(x, y));
        TaksiDispecer(playerid, "Dispecer (telefon): U redu, javljamo vozacima.");
        return 1;
    }
    return TaksiNpcPoziv(playerid, true);
}

static TaksiPonudiIgracima(playerid, Float:x, Float:y, Float:z)
{
    new ima = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (i == playerid || !gUlogiran[i] || !gOrg[i] || GetOrgTip(gOrg[i]) != ORG_TAKSI || !NaDuznosti(i) || IsRanjen(i) || ImaLisice(i)) continue;
        new body[128];
        format(body, sizeof body, T(i, "Musterija %d m od vas. Prihvacate li poziv?"), floatround(GetPlayerDistanceFromPoint(i, x, y, z)));
        gTaksiPonuda[i] = playerid + 1;
        PrikaziDialog(i, DLG_TAKSI_POZIV, DIALOG_STYLE_MSGBOX, T(i, "Poziv taksiju"), body, T(i, "Prihvati"), T(i, "Odbij"));
        ima = 1;
    }
    if (!ima) return 0;
    gTaksiCeka[playerid] = GetTickCount() | 1;
    SetTimerEx("TaksiBezIgraca", TAKSI_IGRAC_MS, false, "ii", playerid, gTaksiCeka[playerid]);
    return 1;
}

static TaksiOcistiPonude(playerid)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gTaksiPonuda[i] == playerid + 1) gTaksiPonuda[i] = 0;
}

forward TaksiBezIgraca(playerid, ceka);
public TaksiBezIgraca(playerid, ceka)
{
    if (gTaksiCeka[playerid] != ceka) return;
    gTaksiCeka[playerid] = 0;
    TaksiOcistiPonude(playerid);
    if (gUlogiran[playerid] && !gTaksiOd[playerid] && !IsRanjen(playerid) && !IsPlayerInAnyVehicle(playerid)) TaksiNpcPoziv(playerid, false);
}

DLG:taksi_poziv(playerid, response)
{
    new c = gTaksiPonuda[playerid] - 1, Float:x, Float:y, Float:z;
    gTaksiPonuda[playerid] = 0;
    if (!response) return 1;
    if (c < 0 || !gTaksiCeka[c]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Netko je vec prihvatio poziv."));
    gTaksiCeka[c] = 0;
    TaksiOcistiPonude(c);
    GetPlayerPos(c, x, y, z);
    gTaksiVozac[playerid] = true;
    SetPlayerCheckpoint(playerid, x, y, z, 5.0);
    SendClientMessage(playerid, COLOR_ORG, T(playerid, "Musterija je oznacena na karti."));
    SendClientMessage(c, COLOR_CHAT, T(c, "Taksist %s dolazi po vas."), GetRPIme(playerid));
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
    if (KaznaCheckpoint(playerid)) return 1;
    if (DrogaCheckpoint(playerid) || DostavaCheckpoint(playerid) || StandCheckpoint(playerid)) return 1;
    if (!gTaksiVozac[playerid]) return MobitelGpsStigao(playerid);
    gTaksiVozac[playerid] = false;
    DisablePlayerCheckpoint(playerid);
    return 1;
}

static TaksiNpcPoziv(playerid, bool:telefon)
{
    new Float:x, Float:y, Float:z, Float:cx, Float:cy, Float:cz;
    GetPlayerPos(playerid, x, y, z);
    Lik_Cesta(x, y, z, cx, cy, cz);
    new s = -1, sekundi, duljina = -1;
    if (gPremium[playerid] && GetWanted(playerid)) SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Premium taksi nije dostupan dok vas policija trazi."));
    else if (gPremium[playerid] && NaPoslu(playerid)) SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Premium taksi nije dostupan dok radite."));
    if (gPremium[playerid] && !GetWanted(playerid) && !NaPoslu(playerid))
    {
        s = TAKSI_FLOTA;
        while (s < TAKSI_SLOTOVA && gTaksi[s][tStanje] != TX_SLOBODAN) s++;
        if (s == TAKSI_SLOTOVA) s = -1;
        else duljina = PremiumPrilaz(s, cx, cy, cz, sekundi);
    }
    else
    {
        s = TaksiNajbliziSlobodan(x, y);
        if (s != -1) duljina = TaksiPutOdVozila(s, cx, cy, cz, sekundi);
    }
    new bool:imaVozaca = s != -1 && gTaksi[s][tNpc] != INVALID_LIK;
    if (duljina < 0 || (!imaVozaca && !TaksiNoviVozac(s))) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Svi taksiji su zauzeti, pokusajte kasnije."));

    gTaksi[s][tMusterija] = playerid;
    gTaksi[s][tStanje] = TX_DOLAZI;
    gTaksiOd[playerid] = s + 1;
    if (s >= TAKSI_FLOTA)
    {
        TaksiVozilo(s, gTaksiTocke[s][0][0], gTaksiTocke[s][0][1], gTaksiTocke[s][0][2], atan2(gTaksiTocke[s][1][1] - gTaksiTocke[s][0][1], gTaksiTocke[s][1][0] - gTaksiTocke[s][0][0]) - 90.0);
        if (gTaksi[s][tVozilo] == INVALID_VEHICLE_ID)
        {
            TaksiPrekini(s);
            return 1;
        }
    }
    if (imaVozaca) TaksiVozi(s);
    else TaksiVozacUdji(s);
    gTaksiPoziv[playerid][0] = x;
    gTaksiPoziv[playerid][1] = y;
    if (telefon)
    {
        MobitelPoziv(playerid, T(playerid, "Taksi"));
        SetTimerEx("TaksiSpustiMobitel", TAKSI_MOBITEL_MS, false, "i", playerid);
        Govor(playerid, "%s kaze (telefon): Dobar dan, trebao bih taksi. Nalazim se u %s.", GetRPIme(playerid), CestaZona(x, y));
    }
    SetTimerEx("TaksiOdgovor", s >= TAKSI_FLOTA ? 500 : 2000, false, "iii", s, playerid, (sekundi + 59) / 60);
    return 1;
}

forward TaksiSpustiMobitel(playerid);
public TaksiSpustiMobitel(playerid)
{
    MobitelKrajPoziva(playerid);
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USECELLPHONE) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
}

forward TaksiOdgovor(s, playerid, minuta);
public TaksiOdgovor(s, playerid, minuta)
{
    if (gTaksiOd[playerid] != s + 1) return;
    new m[12];
    valstr(m, minuta);
    if (s >= TAKSI_FLOTA) TaksiDispecer(playerid, "Dispecer (telefon): Vase crno vozilo je odmah kod vas.");
    else TaksiDispecer(playerid, "Dispecer (telefon): U redu, saljemo vozilo. Stize za oko %s min.", m);
    SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Pricekajte taksi na mjestu gdje ste ga pozvali."));
}

forward TaksiKreni(s);
public TaksiKreni(s)
{
    if (gTaksi[s][tStanje] == TX_DOLAZI || gTaksi[s][tStanje] == TX_ODLAZI) TaksiVozi(s);
}

static TaksiStigao(s)
{
    new p = gTaksi[s][tMusterija];
    gTaksi[s][tStanje] = TX_CEKA;
    gTaksi[s][tRok] = GetTickCount() + TAKSI_CEKANJE_MS;
    SetVehicleParamsForPlayer(gTaksi[s][tVozilo], p, 1, 0);
    TaksiTrubi(s, 0);
    TaksiSms(p, "Vas taksi je stigao. Ugodnu voznju!");
}

forward TaksiTrubi(s, korak);
public TaksiTrubi(s, korak)
{
    #pragma unused korak
    if (gTaksi[s][tNpc] == INVALID_LIK) return;
    Lik_Trubi(gTaksi[s][tNpc], 900);
}

TaksiUVoznji(v)
{
    new s = TaksiPoVozilu(v);
    if (s == -1 || gTaksi[s][tNpc] == INVALID_LIK || gTaksi[s][tStaje]) return 0;
    new st = gTaksi[s][tStanje];
    return st == TX_DOLAZI || st == TX_VOZI || st == TX_ODLAZI || st == TX_SLOBODAN ? s + 1 : 0;
}

TaksiNaRub(s, ms = TAKSI_POLICIJA_MS)
{
    new sad = GetTickCount();
    if (gTaksi[s][tStaje] && sad - gTaksi[s][tStaje] < 0)
    {
        if (sad + ms - gTaksi[s][tStaje] > 0) gTaksi[s][tStaje] = sad + ms;
        return;
    }
    new Float:x, Float:y, Float:z, Float:fx, Float:fy, Float:vx, Float:vy, Float:vz;
    TaksiSmjer(s, x, y, z, fx, fy);
    GetVehicleVelocity(gTaksi[s][tVozilo], vx, vy, vz);
    new Float:brzina = VectorSize(vx, vy, 0.0) * 50.0;
    if (brzina < TAKSI_RUB_SPORO * 2.0) brzina = TAKSI_RUB_SPORO * 2.0;
    gTaksi[s][tStaje] = sad + ms;
    gTaksi[s][tRub] = 1;
    gTaksi[s][tCeka] = 0;
    gTaksi[s][tObilazi] = false;
    new Float:bok = TAKSI_RUB * 0.6;
    if (!TaksiSlobodno(x, y, z, x + fx * TAKSI_RUB_PRVI + fy * bok, y + fy * TAKSI_RUB_PRVI - fx * bok, z)) bok = 0.0;
    Lik_Vozi(gTaksi[s][tNpc], x + fx * TAKSI_RUB_PRVI + fy * bok, y + fy * TAKSI_RUB_PRVI - fx * bok, z, brzina, false);
}

static TaksiRubKraj(s)
{
    new Float:x, Float:y, Float:z, Float:fx, Float:fy;
    TaksiSmjer(s, x, y, z, fx, fy);
    gTaksi[s][tRub] = 2;
    new Float:bok = TAKSI_RUB * 0.4;
    if (!TaksiSlobodno(x, y, z, x + fx * TAKSI_RUB_DRUGI + fy * bok, y + fy * TAKSI_RUB_DRUGI - fx * bok, z)) bok = 0.0;
    Lik_Vozi(gTaksi[s][tNpc], x + fx * TAKSI_RUB_DRUGI + fy * bok, y + fy * TAKSI_RUB_DRUGI - fx * bok, z, TAKSI_RUB_SPORO, false);
}

forward TaksiPrepreke();
public TaksiPrepreke()
{
    new sad = GetTickCount();
    for (new s = 0; s < TAKSI_SLOTOVA; s++)
    {
        new st = gTaksi[s][tStanje];
        if (st != TX_DOLAZI && st != TX_VOZI && st != TX_ODLAZI && (st != TX_SLOBODAN || gTaksi[s][tNpc] == INVALID_LIK)) continue;
        if (gTaksi[s][tStaje])
        {
            if (sad - gTaksi[s][tStaje] >= 0)
            {
                gTaksi[s][tStaje] = 0;
                gTaksi[s][tRub] = 0;
                if (gTaksi[s][tZavrsi] && st == TX_VOZI)
                {
                    TaksiZavrsiRano(s);
                    continue;
                }
                if (st == TX_SLOBODAN) TaksiKruzi(s);
                else
                {
                    TaksiNastavi(s);
                    TaksiKorak(s);
                }
            }
            continue;
        }
        if (Lik_Zaglavio(gTaksi[s][tNpc], 4000) && sad - gTaksi[s][tCeka] >= TAKSI_CEKA_MS)
        {
            gTaksi[s][tCeka] = sad;
            TaksiTrubi(s, 0);
        }
    }
}

TaksiLikStigao(lik)
{
    new s = TaksiLika(lik);
    if (s < 0) return 0;
    if (gTaksi[s][tStaje] && gTaksi[s][tRub] == 1)
    {
        TaksiRubKraj(s);
        return 1;
    }
    if (gTaksi[s][tStaje]) return 1;
    if (gTaksi[s][tStanje] == TX_SLOBODAN)
    {
        TaksiKruzi(s);
        return 1;
    }
    new p = gTaksi[s][tMusterija];
    switch (gTaksi[s][tStanje])
    {
        case TX_DOLAZI: TaksiStigao(s);
        case TX_VOZI:
        {
            TaksiNaplati(p, gTaksi[s][tCijena]);
            TaksiVozac(s, p, "Vozac kaze: Stigli smo. Hvala na voznji i ugodan dan!");
            Govor(p, "%s kaze: Hvala, dovidenja.", GetRPIme(p));
            TaksiOstavi(s);
        }
        case TX_ODLAZI: gTaksi[s][tStanje] = s < TAKSI_FLOTA ? TX_KUCI : TX_KRAJ;
    }
    return 1;
}

TaksiTeleportProvjera(s)
{
    if (s >= TAKSI_FLOTA && gTaksi[s][tStanje] == TX_VOZI && !gTaksi[s][tSkok] && GetTickCount() - gTaksi[s][tRok] >= PREMIUM_VOZNJA_MS) TaksiPocniSkok(s);
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if (KaznaVozilo(playerid)) return 1;
    VoziloAlarmUlaz(vehicleid);
    MobitelEnterVehicle(playerid);
    if (VlakEnter(playerid, vehicleid) || AvionEnter(playerid, vehicleid)) return 1;
    if (ImaLisice(playerid) && !Pritvoren(playerid))
    {
        ClearAnimations(playerid);
        VratiLisice(playerid);
        return 1;
    }
    new t = TaksiPoVozilu(vehicleid), s = gTaksiOd[playerid] - 1;
    if (t == -1 || IsPlayerNPC(playerid)) return 1;
    if (ispassenger && ((t == s && gTaksi[t][tStanje] == TX_CEKA) || (s < 0 && t < TAKSI_FLOTA && gTaksi[t][tStanje] == TX_SLOBODAN) || (s < 0 && gTaksi[t][tStanje] >= TX_CEKA && gTaksi[t][tStanje] <= TX_POTVRDA))) return 1;
    ClearAnimations(playerid);
    if (!ispassenger) SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Ovo je taksi, udite kao putnik tipkom G."));
    else if (s >= 0) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vec ste pozvali taksi."));
    else SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Pozovite taksi mobitelom (/mobitel, Imenik)."));
    return 1;
}

static TaksiKruzi(s)
{
    if (gTaksi[s][tNpc] == INVALID_LIK || gTaksi[s][tStanje] != TX_SLOBODAN || gTaksi[s][tStaje]) return;
    TaksiMotor(s, 1);
    Lik_Kruzi(gTaksi[s][tNpc], 11.0);
}

static TaksiNaKartu(s, playerid)
{
    if (gTaksi[s][tNpc] != INVALID_LIK) Lik_Stani(gTaksi[s][tNpc]);
    gTaksi[s][tStanje] = TX_KARTA;
    gTaksi[s][tKlik] = 0;
    gTaksi[s][tRok] = GetTickCount() + TAKSI_ODREDISTE_MS;
    TaksiVozac(s, playerid, "Vozac kaze: Dobar dan! Kamo cemo?");
    SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Oznacite odrediste na karti (ESC, Karta, desni klik) ili ga odaberite na mobitelu (Internet, Lokacije)."));
}

TaksiStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (IsPlayerNPC(playerid)) return;
    new s = gTaksiOd[playerid] - 1, t = TaksiPoVozilu(GetPlayerVehicleID(playerid));
    if (newstate == PLAYER_STATE_DRIVER && t != -1)
    {
        RemovePlayerFromVehicle(playerid);
        SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Ovo je taksi, udite kao putnik tipkom G."));
        return;
    }
    if (newstate == PLAYER_STATE_PASSENGER && t != -1 && t != s)
    {
        if (s < 0 && gTaksi[t][tStanje] >= TX_CEKA && gTaksi[t][tStanje] <= TX_POTVRDA) return;
        if (s < 0 && t < TAKSI_FLOTA && gTaksi[t][tStanje] == TX_SLOBODAN)
        {
            new bool:imaVozaca = gTaksi[t][tNpc] != INVALID_LIK;
            if (imaVozaca || TaksiNoviVozac(t))
            {
                if (!imaVozaca) TaksiVozacUdji(t);
                gTaksi[t][tMusterija] = playerid;
                gTaksiOd[playerid] = t + 1;
                TaksiNaKartu(t, playerid);
                return;
            }
        }
        RemovePlayerFromVehicle(playerid);
        SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Pozovite taksi mobitelom (/mobitel, Imenik)."));
        return;
    }
    if (s < 0) return;
    if (newstate == PLAYER_STATE_PASSENGER && gTaksi[s][tStanje] == TX_CEKA && t == s)
    {
        SetVehicleParamsForPlayer(gTaksi[s][tVozilo], playerid, 0, 0);
        TaksiNaKartu(s, playerid);
    }
    else if (oldstate == PLAYER_STATE_PASSENGER && gTaksi[s][tStanje] >= TX_KARTA && gTaksi[s][tStanje] <= TX_VOZI)
    {
        TaksiVozac(s, playerid, "Vozac kaze: U redu, ugodan dan.");
        TaksiOtidi(s);
    }
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    new s = gTaksiOd[playerid] - 1;
    if (s < 0 || gTaksi[s][tStanje] != TX_KARTA) return 1;
    gTaksi[s][tCiljX] = fX;
    gTaksi[s][tCiljY] = fY;
    gTaksi[s][tKlik] = GetTickCount();
    return 1;
}

TaksiOdrediste(playerid, Float:x, Float:y)
{
    new s = gTaksiOd[playerid] - 1;
    if (s < 0 || gTaksi[s][tStanje] != TX_KARTA) return 0;
    gTaksi[s][tCiljX] = x;
    gTaksi[s][tCiljY] = y;
    gTaksi[s][tKlik] = GetTickCount() - 1000;
    TaksiPovratak(playerid);
    return 1;
}

TaksiPovratak(playerid)
{
    new s = gTaksiOd[playerid] - 1;
    if (s < 0 || gTaksi[s][tStanje] != TX_KARTA || !gTaksi[s][tKlik] || GetTickCount() - gTaksi[s][tKlik] < 300) return;
    new Float:d, Float:cx = gTaksi[s][tCiljX], Float:cy = gTaksi[s][tCiljY];
    gTaksi[s][tKlik] = 0;
    gTaksi[s][tRok] = GetTickCount() + TAKSI_ODREDISTE_MS;
    new Float:kx, Float:ky, Float:kz;
    d = Lik_Cesta(cx, cy, 0.0, kx, ky, kz);
    new duljina = d > TAKSI_MAX_OD_CESTE ? -1 : TaksiPutOdVozila(s, kx, ky, kz);
    if (duljina < TAKSI_MIN_VOZNJA)
    {
        TaksiVozac(s, playerid, "Vozac kaze: Onamo ne mogu, zao mi je. Mozete li odabrati drugo mjesto?");
        SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Oznacite odrediste na karti (ESC, Karta, desni klik) ili ga odaberite na mobitelu (Internet, Lokacije)."));
        return;
    }
    gTaksi[s][tCijena] = s >= TAKSI_FLOTA ? PREMIUM_START + (duljina * PREMIUM_PO_KM + 500) / 1000 : TAKSI_START + (duljina * TAKSI_PO_KM + 500) / 1000;
    gTaksi[s][tStanje] = TX_POTVRDA;
    new zona[28], cijena[12];
    zona = CestaZona(cx, cy);
    valstr(cijena, gTaksi[s][tCijena]);
    Govor(playerid, "%s kaze: Mozete li me odvesti do %s?", GetRPIme(playerid), zona);
    TaksiVozac(s, playerid, "Vozac kaze: Naravno, do %s ce biti oko $%s.", zona, cijena);
    TaksiPotvrda(s, playerid);
}

static TaksiPotvrda(s, playerid)
{
    new info[128];
    format(info, sizeof info, T(playerid, "Odrediste: %s~n~Cijena: $%d"), CestaZona(gTaksi[s][tCiljX], gTaksi[s][tCiljY]), gTaksi[s][tCijena]);
    UpitDaNe(playerid, UPIT_TAKSI, s, 0, T(playerid, "Taksi"), info);
}

TaksiUpit(playerid, s, bool:da)
{
    if (s < 0 || s >= TAKSI_SLOTOVA || gTaksiOd[playerid] != s + 1 || gTaksi[s][tStanje] != TX_POTVRDA) return;
    if (!da)
    {
        Govor(playerid, "%s kaze: Ipak necu, hvala.", GetRPIme(playerid));
        TaksiVozac(s, playerid, "Vozac kaze: U redu, ugodan dan.");
        TaksiOstavi(s);
        return;
    }
    if (gNovac[playerid] < gTaksi[s][tCijena])
    {
        TaksiVozac(s, playerid, "Vozac kaze: Zao mi je, nemate dovoljno novca za ovu voznju.");
        TaksiOstavi(s);
        return;
    }
    Govor(playerid, "%s kaze: U redu, krenimo.", GetRPIme(playerid));
    TaksiVozac(s, playerid, "Vozac kaze: Krecemo, vezite se.");
    gTaksi[s][tStanje] = TX_VOZI;
    gTaksi[s][tSkok] = 0;
    gTaksi[s][tZavrsi] = false;
    gTaksi[s][tRok] = GetTickCount();
    TaksiVozi(s);
    TaksiMetar(s, true);
    TaksiKontrole(s, playerid);
}

static TaksiKontrole(s, playerid)
{
    new tekst[96];
    if (s < TAKSI_FLOTA) format(tekst, sizeof tekst, T(playerid, "~y~Y~w~ - brze ($%d)~n~~y~N~w~ - stani"), TaksiNapojnica(s));
    else strcat(tekst, T(playerid, "~y~N~w~ - stani"));
    Savjet(playerid, tekst, TAKSI_KONTROLE_MS);
}

static TaksiNaplati(p, iznos)
{
    if (p == INVALID_PLAYER_ID || !IsPlayerConnected(p)) return;
    if (gNovac[p] < iznos) iznos = gNovac[p];
    if (iznos > 0) DajNovac(p, -iznos);
    SendClientMessage(p, COLOR_INFO, T(p, "Platili ste voznju taksijem: $%d."), iznos);
}

static TaksiPredeno(s)
{
    new Float:m = 0.0, kraj = gTaksi[s][tIdx] < gTaksi[s][tBroj] ? gTaksi[s][tIdx] : gTaksi[s][tBroj] - 1;
    for (new i = 1; i <= kraj; i++) m += VectorSize(gTaksiTocke[s][i][0] - gTaksiTocke[s][i - 1][0], gTaksiTocke[s][i][1] - gTaksiTocke[s][i - 1][1], 0.0);
    return floatround(m);
}

static TaksiIznos(s)
{
    new m = TaksiPredeno(s), iznos = s >= TAKSI_FLOTA ? PREMIUM_START + (m * PREMIUM_PO_KM + 500) / 1000 : TAKSI_START + (m * TAKSI_PO_KM + 500) / 1000;
    return iznos > gTaksi[s][tCijena] ? gTaksi[s][tCijena] : iznos;
}

static TaksiMetar(s, bool:odmah = false)
{
    new p = gTaksi[s][tMusterija], iznos = TaksiIznos(s), info[96];
    if (p == INVALID_PLAYER_ID || (!odmah && iznos == gTaksi[s][tMetar])) return;
    gTaksi[s][tMetar] = iznos;
    format(info, sizeof info, T(p, "Trenutno: $%d~n~Do cilja: $%d"), iznos, gTaksi[s][tCijena]);
    IgraObavijestPanel(p, T(p, "Taksimetar"), TAKSI_METAR_BOJA, info);
    gTaksiMetar[p] = true;
}

static TaksiMetarMakni(s)
{
    TaksiMetarSkrij(gTaksi[s][tMusterija]);
}

static TaksiMetarSkrij(p)
{
    if (p == INVALID_PLAYER_ID || !gTaksiMetar[p]) return;
    gTaksiMetar[p] = false;
    IgraObavijestUkloni(p);
}

static TaksiZavrsiRano(s)
{
    new p = gTaksi[s][tMusterija], iznos = TaksiIznos(s), cijena[12];
    gTaksi[s][tZavrsi] = false;
    Lik_Stani(gTaksi[s][tNpc]);
    valstr(cijena, iznos);
    TaksiVozac(s, p, "Vozac kaze: Izvolite, to je $%s. Ugodan dan!", cijena);
    TaksiNaplati(p, iznos);
    TaksiOstavi(s);
}

static TaksiPocniSkok(s)
{
    new p = gTaksi[s][tMusterija], Float:x, Float:y, Float:z, Float:m = 0.0, i = gTaksi[s][tBroj] - 1;
    gTaksi[s][tRok] = GetTickCount() - PREMIUM_VOZNJA_MS + 2000;
    GetVehiclePos(gTaksi[s][tVozilo], x, y, z);
    if (TaksiNaVidiku(x, y, z, gTaksi[s][tVozilo], PREMIUM_VIDIK)) return;
    while (i > gTaksi[s][tIdx] + 20)
    {
        m += VectorSize(gTaksiTocke[s][i][0] - gTaksiTocke[s][i - 1][0], gTaksiTocke[s][i][1] - gTaksiTocke[s][i - 1][1], 0.0);
        i--;
        if (m < PREMIUM_PRIJE_CILJA || i % 4 || TaksiNaVidiku(gTaksiTocke[s][i][0], gTaksiTocke[s][i][1], gTaksiTocke[s][i][2], gTaksi[s][tVozilo], PREMIUM_VIDIK)) continue;
        gTaksi[s][tSkok] = i;
        gTaksi[s][tStanje] = TX_TELEPORT;
        TaksiFade(s, p, 0);
        for (new o = 0, oZadnji = gIgracZadnji; o <= oZadnji; o++) if (o != p && gUlogiran[o] && IsPlayerInVehicle(o, gTaksi[s][tVozilo])) TaksiFade(s, o, 0);
        return;
    }
}

static TaksiPreskoci(s, playerid)
{
    new j = gTaksi[s][tSkok], stari = gTaksi[s][tVozilo], stariZnak = gTaksi[s][tZnak], sjedalo = GetPlayerVehicleSeat(playerid);
    Lik_Stani(gTaksi[s][tNpc]);
    TaksiVozilo(s, gTaksiTocke[s][j][0], gTaksiTocke[s][j][1], gTaksiTocke[s][j][2], atan2(gTaksiTocke[s][j + 1][1] - gTaksiTocke[s][j][1], gTaksiTocke[s][j + 1][0] - gTaksiTocke[s][j][0]) - 90.0);
    if (gTaksi[s][tVozilo] == INVALID_VEHICLE_ID)
    {
        gTaksi[s][tVozilo] = stari;
        gTaksi[s][tZnak] = stariZnak;
        gTaksi[s][tStanje] = TX_VOZI;
        TaksiKorak(s);
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (i == playerid || (gUlogiran[i] && IsPlayerInVehicle(i, stari))) TaksiFade(s, i, FADE_SLOJEVA + 1);
        return;
    }
    Lik_UVozilo(gTaksi[s][tNpc], gTaksi[s][tVozilo], 0);
    TaksiMotor(s, 1);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (i == playerid || !gUlogiran[i] || !IsPlayerInVehicle(i, stari)) continue;
        SetTimerEx("TaksiPutnikSjedi", 600, false, "iiii", s, i, GetPlayerVehicleSeat(i), 0);
        SetPlayerPos(i, gTaksiTocke[s][j][0], gTaksiTocke[s][j][1], gTaksiTocke[s][j][2] + 2.0);
    }
    if (stariZnak) DestroyDynamicObject(STREAMER_TAG_OBJECT:stariZnak);
    DestroyVehicle(stari);
    gTaksi[s][tIdx] = j;
    SetPlayerPos(playerid, gTaksiTocke[s][j][0], gTaksiTocke[s][j][1], gTaksiTocke[s][j][2] + 2.0);
    SetTimerEx("TaksiSjedi", 600, false, "iiii", s, playerid, sjedalo > 0 ? sjedalo : 1, 0);
}

forward TaksiPutnikSjedi(s, playerid, sjedalo, pokusaj);
public TaksiPutnikSjedi(s, playerid, sjedalo, pokusaj)
{
    if (!gUlogiran[playerid]) return;
    if (IsPlayerInVehicle(playerid, gTaksi[s][tVozilo]) || pokusaj >= 8)
    {
        TaksiFade(s, playerid, FADE_SLOJEVA + 1);
        return;
    }
    PutPlayerInVehicle(playerid, gTaksi[s][tVozilo], sjedalo);
    SetTimerEx("TaksiPutnikSjedi", 400, false, "iiii", s, playerid, sjedalo, pokusaj + 1);
}

forward TaksiSjedi(s, playerid, sjedalo, pokusaj);
public TaksiSjedi(s, playerid, sjedalo, pokusaj)
{
    if (gTaksi[s][tStanje] != TX_TELEPORT || gTaksi[s][tMusterija] != playerid)
    {
        for (new i = 0; i < FADE_SLOJEVA; i++) TextDrawHideForPlayer(playerid, gFade[i]);
        return;
    }
    if (IsPlayerInVehicle(playerid, gTaksi[s][tVozilo]) || pokusaj >= 8)
    {
        gTaksi[s][tStanje] = TX_VOZI;
        gTaksi[s][tIdx]++;
        TaksiKorak(s);
        TaksiFade(s, playerid, FADE_SLOJEVA + 1);
        return;
    }
    PutPlayerInVehicle(playerid, gTaksi[s][tVozilo], sjedalo);
    SetTimerEx("TaksiSjedi", 400, false, "iiii", s, playerid, sjedalo, pokusaj + 1);
}

forward TaksiFade(s, playerid, korak);
public TaksiFade(s, playerid, korak)
{
    if (korak > FADE_SLOJEVA)
    {
        Fade(playerid, korak);
        return;
    }
    if (gTaksi[s][tStanje] != TX_TELEPORT)
    {
        for (new i = 0; i < FADE_SLOJEVA; i++) TextDrawHideForPlayer(playerid, gFade[i]);
        return;
    }
    if (korak == FADE_SLOJEVA)
    {
        if (playerid == gTaksi[s][tMusterija]) TaksiPreskoci(s, playerid);
        return;
    }
    TextDrawShowForPlayer(playerid, gFade[korak]);
    SetTimerEx("TaksiFade", FADE_KORAK_MS, false, "iii", s, playerid, korak + 1);
}

static TaksiNapojnica(s)
{
    return (gTaksi[s][tCijena] * TAKSI_NAPOJNICA_POSTO + 50) / 100;
}

TaksiKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    new s = gTaksiOd[playerid] - 1;
    if (s < 0 || gTaksi[s][tStanje] != TX_VOZI || !IsPlayerInVehicle(playerid, gTaksi[s][tVozilo])) return;
    if ((newkeys & KEY_NO) && !(oldkeys & KEY_NO))
    {
        if (gTaksi[s][tZavrsi]) return;
        gTaksi[s][tZavrsi] = true;
        Govor(playerid, "%s kaze: Stanite ovdje, molim vas.", GetRPIme(playerid));
        TaksiVozac(s, playerid, "Vozac kaze: U redu, stajem uz rub.");
        TaksiNaRub(s, TAKSI_STANI_MS);
        return;
    }
    if (!(newkeys & KEY_YES) || (oldkeys & KEY_YES) || s >= TAKSI_FLOTA || gTaksi[s][tBrzo] || gTaksi[s][tZavrsi]) return;
    if (!DajNovac(playerid, -TaksiNapojnica(s)))
    {
        TaksiVozac(s, playerid, "Vozac kaze: Zao mi je, nemate dovoljno novca za ovu voznju.");
        return;
    }
    gTaksi[s][tBrzo] = true;
    Govor(playerid, "%s kaze: Evo napojnica, pozurite malo.", GetRPIme(playerid));
    TaksiVozac(s, playerid, "Vozac kaze: Hvala! Drzite se.");
}

TaksiPlayerLeft(playerid)
{
    TaksiMetarSkrij(playerid);
    if (gTaksiOd[playerid]) TaksiMetarMakni(gTaksiOd[playerid] - 1);
    if (gTaksiOd[playerid]) TaksiOtidi(gTaksiOd[playerid] - 1);
    if (gTaksiCeka[playerid]) TaksiOcistiPonude(playerid);
    gTaksiCeka[playerid] = 0;
    gTaksiPonuda[playerid] = 0;
    gTaksiVozac[playerid] = false;
}

static TaksiBezVozaca(s)
{
    if (gTaksi[s][tNpc] == INVALID_LIK) return;
    Lik_Ukloni(gTaksi[s][tNpc]);
    gTaksi[s][tNpc] = INVALID_LIK;
}

TaksiTick()
{
    for (new p = 0, pZadnji = gIgracZadnji; p <= pZadnji; p++)
    {
        if (!gTaksiMetar[p]) continue;
        new s = gTaksiOd[p] - 1;
        if (s < 0 || (gTaksi[s][tStanje] != TX_VOZI && gTaksi[s][tStanje] != TX_TELEPORT) || gTaksi[s][tMusterija] != p) TaksiMetarSkrij(p);
    }
    for (new s = 0; s < TAKSI_SLOTOVA; s++)
    {
        new st = gTaksi[s][tStanje], Float:x, Float:y, Float:z;
        if (st == TX_VOZI)
        {
            TaksiMetar(s);
            TaksiTeleportProvjera(s);
        }
        if ((st == TX_DOLAZI || st == TX_CEKA) && gTaksi[s][tMusterija] != INVALID_PLAYER_ID)
        {
            new p = gTaksi[s][tMusterija];
            GetPlayerPos(p, x, y, z);
            if (VectorSize(x - gTaksiPoziv[p][0], y - gTaksiPoziv[p][1], 0.0) > TAKSI_ODLAZAK_M)
            {
                TaksiSms(p, "Vozac vas vise nece cekati, voznja je otkazana.");
                TaksiOtidi(s);
                continue;
            }
        }
        if (st == TX_SLOBODAN && s < TAKSI_FLOTA)
        {
            GetVehiclePos(gTaksi[s][tVozilo], x, y, z);
            if (gTaksi[s][tNpc] == INVALID_LIK)
            {
                if (TaksiVidljivo(x, y, z) && !TaksiNaVidiku(x, y, z) && TaksiNoviVozac(s))
                {
                    Lik_UVozilo(gTaksi[s][tNpc], gTaksi[s][tVozilo], 0);
                    TaksiKruzi(s);
                }
            }
            else if (!TaksiVidljivo(x, y, z, TAKSI_SKRIVENO + 50.0))
            {
                TaksiBezVozaca(s);
                TaksiMotor(s, 0);
                if (!TaksiNaVidiku(gTaksiPoza[s][0], gTaksiPoza[s][1], gTaksiPoza[s][2]))
                {
                    SetVehiclePos(gTaksi[s][tVozilo], gTaksiPoza[s][0], gTaksiPoza[s][1], gTaksiPoza[s][2]);
                    SetVehicleZAngle(gTaksi[s][tVozilo], gTaksiPoza[s][3]);
                }
            }
            else if (!Lik_Krece(gTaksi[s][tNpc]) || Lik_Zaglavio(gTaksi[s][tNpc], 8000)) TaksiKruzi(s);
        }
        else if (st == TX_KUCI)
        {
            TaksiMotor(s, 0);
            gTaksi[s][tStanje] = TX_SLOBODAN;
        }
        else if (st == TX_KRAJ)
        {
            TaksiBezVozaca(s);
            DestroyVehicle(gTaksi[s][tVozilo]);
            gTaksi[s][tVozilo] = INVALID_VEHICLE_ID;
            if (s >= TAKSI_FLOTA) DestroyDynamicObject(STREAMER_TAG_OBJECT:gTaksi[s][tZnak]);
            gTaksi[s][tStanje] = s < TAKSI_FLOTA ? TX_NESTAO : TX_SLOBODAN;
        }
        else if (st == TX_NESTAO)
        {
            if (TaksiNaVidiku(gTaksiPoza[s][0], gTaksiPoza[s][1], gTaksiPoza[s][2])) continue;
            TaksiVozilo(s, gTaksiPoza[s][0], gTaksiPoza[s][1], gTaksiPoza[s][2], gTaksiPoza[s][3]);
            if (gTaksi[s][tVozilo] != INVALID_VEHICLE_ID) gTaksi[s][tStanje] = TX_SLOBODAN;
        }
        else if (st == TX_ODLAZI && s >= TAKSI_FLOTA && GetTickCount() - gTaksi[s][tRok] >= PREMIUM_ODLAZAK_MS)
        {
            GetVehiclePos(gTaksi[s][tVozilo], x, y, z);
            if (!TaksiNaVidiku(x, y, z)) gTaksi[s][tStanje] = TX_KRAJ;
        }
        else if (st == TX_IZLAZ && GetTickCount() - gTaksi[s][tRok] >= 0) TaksiOtidi(s);
        else if (st >= TX_CEKA && st <= TX_POTVRDA && GetTickCount() - gTaksi[s][tRok] >= 0)
        {
            TaksiVozac(s, gTaksi[s][tMusterija], "Vozac kaze: Zao mi je, ne mogu vise cekati.");
            TaksiOstavi(s);
        }
    }
}
