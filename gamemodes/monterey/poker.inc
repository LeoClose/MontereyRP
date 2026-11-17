#define POKER_MAX           8
#define POKER_MJESTA        6
#define POKER_MODEL         19474
#define POKER_MALI          25
#define POKER_VELIKI        50
#define POKER_MIN_ULAZ      1000
#define POKER_MAX_ULAZ      20000
#define POKER_RAKE          5
#define POKER_RAKE_MAX      500
#define POKER_POTEZ_S       30
#define POKER_PAUZA_S       6
#define POKER_DOMET         2.5
#define POKER_NAPUSTI       8.0
#define POKER_TD            16

enum { PO_CEKA, PO_RUKA }
enum { PTD_POZADINA, PTD_PLOCA, PTD_MOJA = PTD_PLOCA + 5, PTD_BANKA = PTD_MOJA + 2, PTD_IGRACI, PTD_STATUS, PTD_FOLD, PTD_CALL, PTD_RAISE, PTD_ALLIN, PTD_USTANI }
enum E_POKER { bool:poIma, poDb, Float:poX, Float:poY, Float:poZ, Float:poA, poInt, poSvijet, STREAMER_TAG_OBJECT:poObj, STREAMER_TAG_3D_TEXT_LABEL:poTekst,
    poFaza, poIgrac[POKER_MJESTA], poStog[POKER_MJESTA], poUlog[POKER_MJESTA], poUkupno[POKER_MJESTA], bool:poU[POKER_MJESTA], bool:poFold[POKER_MJESTA], bool:poAllin[POKER_MJESTA], bool:poOdigrao[POKER_MJESTA],
    poKarte[POKER_MJESTA * 2], poPloca[5], poPlocaN, poSpil[52], poSpilI, poDiler, poNaPotezu, poNajveci, poMinRaise, poRunda, poRok, poPauza }

static const gPokerRang[] = "23456789TJQKA";
static const gPokerBoja[] = "cdhs";
static const gPokerKat[9][] = { "Visoka karta", "Par", "Dva para", "Tris", "Skala", "Boja", "Ful", "Poker", "Skala u boji" };

static gPo[POKER_MAX][E_POKER], gPokerStol[MAX_PLAYERS] = { -1, ... }, gPokerMjesto[MAX_PLAYERS], gApPoker[MAX_PLAYERS][POKER_MAX], gPokerOdabran[MAX_PLAYERS];
static PlayerText:gPoTd[MAX_PLAYERS][POKER_TD], bool:gPoTdIma[MAX_PLAYERS];

static PokerRang(c)
{
    return (c % 13 + 12) % 13;
}

static PokerVr(kat, a = 0, b = 0, c = 0, d = 0, e = 0)
{
    return (((((kat * 16 + a) * 16 + b) * 16 + c) * 16 + d) * 16) + e;
}

static PokerSkala(maska)
{
    for (new v = 12; v >= 4; v--) if ((maska >> (v - 4) & 0x1F) == 0x1F) return v;
    if ((maska & 0x100F) == 0x100F) return 3;
    return -1;
}

static PokerVrh(maska, out[], n)
{
    new k = 0;
    for (new r = 12; r >= 0 && k < n; r--) if (maska & (1 << r)) out[k++] = r;
    while (k < n) out[k++] = 0;
}

PokerOcjena(const k[], n)
{
    new broj[13], boja[4], bojaMaska[4], maska = 0, v[5];
    for (new i = 0; i < n; i++)
    {
        new r = PokerRang(k[i]), b = k[i] / 13;
        broj[r]++;
        boja[b]++;
        bojaMaska[b] |= 1 << r;
        maska |= 1 << r;
    }
    for (new b = 0; b < 4; b++)
    {
        if (boja[b] < 5) continue;
        new sk = PokerSkala(bojaMaska[b]);
        if (sk != -1) return PokerVr(8, sk);
    }
    new cetiri = -1, tri = -1, tri2 = -1, par = -1, par2 = -1;
    for (new r = 12; r >= 0; r--)
    {
        if (broj[r] == 4 && cetiri == -1) cetiri = r;
        else if (broj[r] == 3)
        {
            if (tri == -1) tri = r;
            else if (tri2 == -1) tri2 = r;
        }
        else if (broj[r] == 2)
        {
            if (par == -1) par = r;
            else if (par2 == -1) par2 = r;
        }
    }
    if (cetiri != -1)
    {
        PokerVrh(maska & ~(1 << cetiri), v, 1);
        return PokerVr(7, cetiri, v[0]);
    }
    if (tri != -1 && (par != -1 || tri2 != -1)) return PokerVr(6, tri, par > tri2 ? par : tri2);
    for (new b = 0; b < 4; b++)
    {
        if (boja[b] < 5) continue;
        PokerVrh(bojaMaska[b], v, 5);
        return PokerVr(5, v[0], v[1], v[2], v[3], v[4]);
    }
    new sk = PokerSkala(maska);
    if (sk != -1) return PokerVr(4, sk);
    if (tri != -1)
    {
        PokerVrh(maska & ~(1 << tri), v, 2);
        return PokerVr(3, tri, v[0], v[1]);
    }
    if (par != -1 && par2 != -1)
    {
        PokerVrh(maska & ~(1 << par) & ~(1 << par2), v, 1);
        return PokerVr(2, par, par2, v[0]);
    }
    if (par != -1)
    {
        PokerVrh(maska & ~(1 << par), v, 3);
        return PokerVr(1, par, v[0], v[1], v[2]);
    }
    PokerVrh(maska, v, 5);
    return PokerVr(0, v[0], v[1], v[2], v[3], v[4]);
}

static PokerKarta(rang, boja)
{
    return boja * 13 + (rang + 1) % 13;
}

InitPoker()
{
    new a[7], b[7];
    a[0] = PokerKarta(12, 3); a[1] = PokerKarta(11, 3); a[2] = PokerKarta(10, 3); a[3] = PokerKarta(9, 3); a[4] = PokerKarta(8, 3); a[5] = PokerKarta(0, 0); a[6] = PokerKarta(1, 1);
    b[0] = PokerKarta(5, 0); b[1] = PokerKarta(5, 1); b[2] = PokerKarta(5, 2); b[3] = PokerKarta(5, 3); b[4] = PokerKarta(12, 0); b[5] = PokerKarta(0, 1); b[6] = PokerKarta(1, 2);
    assert(PokerOcjena(a, 7) / 1048576 == 8 && PokerOcjena(b, 7) / 1048576 == 7 && PokerOcjena(a, 7) > PokerOcjena(b, 7));
    a[0] = PokerKarta(12, 0); a[1] = PokerKarta(0, 1); a[2] = PokerKarta(1, 2); a[3] = PokerKarta(2, 3); a[4] = PokerKarta(3, 0); a[5] = PokerKarta(8, 1); a[6] = PokerKarta(9, 2);
    b[0] = PokerKarta(4, 0); b[1] = PokerKarta(0, 1); b[2] = PokerKarta(1, 2); b[3] = PokerKarta(2, 3); b[4] = PokerKarta(3, 0); b[5] = PokerKarta(8, 1); b[6] = PokerKarta(9, 2);
    assert(PokerOcjena(a, 7) / 1048576 == 4 && PokerOcjena(b, 7) > PokerOcjena(a, 7));
    a[0] = PokerKarta(10, 0); a[1] = PokerKarta(10, 1); a[2] = PokerKarta(10, 2); a[3] = PokerKarta(3, 3); a[4] = PokerKarta(3, 0); a[5] = PokerKarta(3, 1); a[6] = PokerKarta(0, 2);
    assert(PokerOcjena(a, 7) == PokerVr(6, 10, 3));
    a[0] = PokerKarta(10, 0); a[1] = PokerKarta(10, 1); a[2] = PokerKarta(7, 2); a[3] = PokerKarta(7, 3); a[4] = PokerKarta(2, 0); a[5] = PokerKarta(2, 1); a[6] = PokerKarta(12, 2);
    assert(PokerOcjena(a, 7) == PokerVr(2, 10, 7, 12));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS poker_tables (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM poker_tables LIMIT %d", POKER_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gPo[s][poDb] = DB_GetFieldInt(res, 0);
        gPo[s][poX] = DB_GetFieldFloat(res, 1);
        gPo[s][poY] = DB_GetFieldFloat(res, 2);
        gPo[s][poZ] = DB_GetFieldFloat(res, 3);
        gPo[s][poA] = DB_GetFieldFloat(res, 4);
        gPo[s][poInt] = DB_GetFieldInt(res, 5);
        gPo[s][poSvijet] = DB_GetFieldInt(res, 6);
        PokerStvori(s);
    }
    DB_FreeResultSet(res);
}

PokerConnect(playerid)
{
    gPokerStol[playerid] = -1;
    gPoTdIma[playerid] = false;
}

static PokerStvori(s)
{
    gPo[s][poIma] = true;
    gPo[s][poFaza] = PO_CEKA;
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        gPo[s][poIgrac][m] = INVALID_PLAYER_ID;
        gPo[s][poU][m] = false;
    }
    gPo[s][poObj] = CreateDynamicObject(POKER_MODEL, gPo[s][poX], gPo[s][poY], gPo[s][poZ], 0.0, 0.0, gPo[s][poA], gPo[s][poSvijet], gPo[s][poInt]);
    new tekst[64];
    format(tekst, sizeof tekst, "Poker\n$%d/$%d\n[Y]", POKER_MALI, POKER_VELIKI);
    gPo[s][poTekst] = CreateDynamic3DTextLabel(tekst, COLOR_ORG, gPo[s][poX], gPo[s][poY], gPo[s][poZ] + 1.4, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gPo[s][poSvijet], gPo[s][poInt]);
}

static PokerImeKarte(c, out[], size)
{
    format(out, size, "%c%c", gPokerRang[PokerRang(c)], gPokerBoja[c / 13]);
}

static PokerSprite(c, out[], size)
{
    format(out, size, "LD_CARD:cd%d%c", c % 13 + 1, gPokerBoja[c / 13]);
}

static PokerPoruka(s, const tekst[], igrac = INVALID_PLAYER_ID, a = 0)
{
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        new p = gPo[s][poIgrac][m];
        if (p == INVALID_PLAYER_ID) continue;
        if (igrac != INVALID_PLAYER_ID) SendClientMessage(p, 0x2DD9A9FF, T(p, tekst), GetRPIme(igrac), a);
        else SendClientMessage(p, 0x2DD9A9FF, T(p, tekst));
    }
}

static PokerDajZetone(playerid, n)
{
    if (n <= 0) return;
    if (!GiveItem(playerid, ITEM_ZETONI, n)) DajNovac(playerid, n);
}

static PlayerText:PokerTdTekst(playerid, Float:x, Float:y, const tekst[], Float:vel, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, td, vel, vel * 4.2);
    PlayerTextDrawColour(playerid, td, 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawBackgroundColour(playerid, td, 0x000000AA);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    return td;
}

static PlayerText:PokerTdGumb(playerid, Float:x, Float:y, Float:w, const tekst[], boja)
{
    new PlayerText:td = PokerTdTekst(playerid, x, y, tekst, 0.24, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 12.0, w);
    PlayerTextDrawSetSelectable(playerid, td, true);
    return td;
}

static PlayerText:PokerTdSprite(playerid, Float:x, Float:y, Float:w, Float:h, const sprite[], boja = -1)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, sprite);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    return td;
}

static PokerTdUnisti(playerid)
{
    if (!gPoTdIma[playerid]) return;
    gPoTdIma[playerid] = false;
    for (new i = 0; i < POKER_TD; i++) PlayerTextDrawDestroy(playerid, gPoTd[playerid][i]);
    CancelSelectTextDraw(playerid);
}

static PokerTdStvori(playerid)
{
    PokerTdUnisti(playerid);
    gPoTdIma[playerid] = true;
    gPoTd[playerid][PTD_POZADINA] = PokerTdSprite(playerid, 150.0, 250.0, 340.0, 190.0, "LD_SPAC:white", 0x0F4D2AEE);
    for (new i = 0; i < 5; i++) gPoTd[playerid][PTD_PLOCA + i] = PokerTdSprite(playerid, 245.0 + float(i) * 31.0, 258.0, 28.0, 40.0, "LD_CARD:cdback");
    for (new i = 0; i < 2; i++) gPoTd[playerid][PTD_MOJA + i] = PokerTdSprite(playerid, 160.0 + float(i) * 37.0, 258.0, 34.0, 48.0, "LD_CARD:cdback");
    gPoTd[playerid][PTD_BANKA] = PokerTdTekst(playerid, 320.0, 302.0, "_", 0.24, TEXT_DRAW_ALIGN_CENTER);
    gPoTd[playerid][PTD_IGRACI] = PokerTdTekst(playerid, 160.0, 316.0, "_", 0.2);
    gPoTd[playerid][PTD_STATUS] = PokerTdTekst(playerid, 480.0, 262.0, "_", 0.22, TEXT_DRAW_ALIGN_RIGHT);
    gPoTd[playerid][PTD_FOLD] = PokerTdGumb(playerid, 190.0, 418.0, 54.0, "FOLD", 0x8B2E2EFF);
    gPoTd[playerid][PTD_CALL] = PokerTdGumb(playerid, 255.0, 418.0, 64.0, "CHECK", 0x1E5AA8FF);
    gPoTd[playerid][PTD_RAISE] = PokerTdGumb(playerid, 325.0, 418.0, 64.0, "RAISE", 0x1E8449FF);
    gPoTd[playerid][PTD_ALLIN] = PokerTdGumb(playerid, 390.0, 418.0, 54.0, "ALL-IN", 0xB7950BFF);
    gPoTd[playerid][PTD_USTANI] = PokerTdGumb(playerid, 450.0, 418.0, 54.0, "USTANI", 0x555555FF);
    PlayerTextDrawSetString(playerid, gPoTd[playerid][PTD_USTANI], T(playerid, "USTANI"));
    for (new i = 0; i < POKER_TD; i++) PlayerTextDrawShow(playerid, gPoTd[playerid][i]);
    SelectTextDraw(playerid, 0xE8C15AFF);
}

static PokerZivi(s, bool:mozeIgrati = false)
{
    new n = 0;
    for (new m = 0; m < POKER_MJESTA; m++) if (gPo[s][poU][m] && !gPo[s][poFold][m] && (!mozeIgrati || !gPo[s][poAllin][m])) n++;
    return n;
}

static PokerBanka(s)
{
    new b = 0;
    for (new m = 0; m < POKER_MJESTA; m++) b += gPo[s][poUkupno][m];
    return b;
}

static PokerOsvjeziIgraca(s, playerid)
{
    new m = gPokerMjesto[playerid], sprite[20], tekst[512], red[96], bool:ruka = gPo[s][poFaza] == PO_RUKA;
    for (new i = 0; i < 5; i++)
    {
        if (ruka && i < gPo[s][poPlocaN])
        {
            PokerSprite(gPo[s][poPloca][i], sprite, sizeof sprite);
            PlayerTextDrawSetString(playerid, gPoTd[playerid][PTD_PLOCA + i], sprite);
            PlayerTextDrawShow(playerid, gPoTd[playerid][PTD_PLOCA + i]);
        }
        else PlayerTextDrawHide(playerid, gPoTd[playerid][PTD_PLOCA + i]);
    }
    for (new i = 0; i < 2; i++)
    {
        if (ruka && gPo[s][poU][m] && !gPo[s][poFold][m])
        {
            PokerSprite(gPo[s][poKarte][m * 2 + i], sprite, sizeof sprite);
            PlayerTextDrawSetString(playerid, gPoTd[playerid][PTD_MOJA + i], sprite);
            PlayerTextDrawShow(playerid, gPoTd[playerid][PTD_MOJA + i]);
        }
        else PlayerTextDrawHide(playerid, gPoTd[playerid][PTD_MOJA + i]);
    }
    format(tekst, sizeof tekst, T(playerid, "Banka: %d"), ruka ? PokerBanka(s) : 0);
    PlayerTextDrawSetString(playerid, gPoTd[playerid][PTD_BANKA], tekst);
    tekst[0] = EOS;
    for (new i = 0; i < POKER_MJESTA; i++)
    {
        new p = gPo[s][poIgrac][i];
        if (p == INVALID_PLAYER_ID) continue;
        format(red, sizeof red, "%s%s%s ~w~%d", ruka && gPo[s][poNaPotezu] == i ? ("~y~> ") : ("~w~"), ruka && gPo[s][poDiler] == i ? ("(D) ") : (""), GetRPIme(p), gPo[s][poStog][i]);
        strcat(tekst, red);
        if (ruka && gPo[s][poU][i])
        {
            if (gPo[s][poFold][i]) strcat(tekst, " ~r~FOLD");
            else
            {
                if (gPo[s][poUlog][i]) format(red, sizeof red, " ~g~[%d]", gPo[s][poUlog][i]), strcat(tekst, red);
                if (gPo[s][poAllin][i]) strcat(tekst, " ~y~ALL-IN");
            }
        }
        strcat(tekst, "~n~");
    }
    PlayerTextDrawSetString(playerid, gPoTd[playerid][PTD_IGRACI], tekst);
    new bool:moj = ruka && gPo[s][poNaPotezu] == m;
    if (!ruka) format(tekst, sizeof tekst, T(playerid, "Ceka se igrace~n~(najmanje 2)"));
    else if (moj) format(tekst, sizeof tekst, T(playerid, "~g~Vi ste na potezu~n~~w~%d s"), gPo[s][poRok]);
    else format(tekst, sizeof tekst, T(playerid, "Na potezu:~n~%s (%d s)"), GetRPIme(gPo[s][poIgrac][gPo[s][poNaPotezu]]), gPo[s][poRok]);
    PlayerTextDrawSetString(playerid, gPoTd[playerid][PTD_STATUS], tekst);
    new za = gPo[s][poNajveci] - gPo[s][poUlog][m];
    if (za > 0) format(tekst, sizeof tekst, "CALL %d", za > gPo[s][poStog][m] ? gPo[s][poStog][m] : za);
    else format(tekst, sizeof tekst, "CHECK");
    PlayerTextDrawSetString(playerid, gPoTd[playerid][PTD_CALL], tekst);
    for (new i = PTD_FOLD; i <= PTD_ALLIN; i++)
    {
        if (moj) PlayerTextDrawShow(playerid, gPoTd[playerid][i]);
        else PlayerTextDrawHide(playerid, gPoTd[playerid][i]);
    }
}

static PokerOsvjezi(s)
{
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        new p = gPo[s][poIgrac][m];
        if (p != INVALID_PLAYER_ID && gPoTdIma[p]) PokerOsvjeziIgraca(s, p);
    }
}

static PokerSljedeci(s, m, bool:samoAktivni)
{
    for (new i = 1; i <= POKER_MJESTA; i++)
    {
        new k = (m + i) % POKER_MJESTA;
        if (samoAktivni ? (gPo[s][poU][k] && !gPo[s][poFold][k] && !gPo[s][poAllin][k]) : (gPo[s][poIgrac][k] != INVALID_PLAYER_ID && gPo[s][poStog][k] > 0)) return k;
    }
    return -1;
}

static PokerUlozi(s, m, iznos)
{
    if (iznos >= gPo[s][poStog][m])
    {
        iznos = gPo[s][poStog][m];
        gPo[s][poAllin][m] = true;
    }
    gPo[s][poStog][m] -= iznos;
    gPo[s][poUlog][m] += iznos;
    gPo[s][poUkupno][m] += iznos;
}

static PokerDijeli(s)
{
    return gPo[s][poSpil][gPo[s][poSpilI]++];
}

static PokerNovaRuka(s)
{
    new n = 0;
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        gPo[s][poU][m] = gPo[s][poIgrac][m] != INVALID_PLAYER_ID && gPo[s][poStog][m] > 0;
        gPo[s][poFold][m] = gPo[s][poAllin][m] = gPo[s][poOdigrao][m] = false;
        gPo[s][poUlog][m] = gPo[s][poUkupno][m] = 0;
        if (gPo[s][poU][m]) n++;
    }
    if (n < 2)
    {
        for (new m = 0; m < POKER_MJESTA; m++) gPo[s][poU][m] = false;
        gPo[s][poFaza] = PO_CEKA;
        PokerOsvjezi(s);
        return;
    }
    for (new i = 0; i < 52; i++) gPo[s][poSpil][i] = i;
    for (new i = 51; i > 0; i--)
    {
        new j = random(i + 1), t = gPo[s][poSpil][i];
        gPo[s][poSpil][i] = gPo[s][poSpil][j];
        gPo[s][poSpil][j] = t;
    }
    gPo[s][poSpilI] = 0;
    gPo[s][poPlocaN] = 0;
    gPo[s][poRunda] = 0;
    gPo[s][poFaza] = PO_RUKA;
    gPo[s][poDiler] = PokerSljedeci(s, gPo[s][poDiler], false);
    new mali = n == 2 ? gPo[s][poDiler] : PokerSljedeci(s, gPo[s][poDiler], false), veliki = PokerSljedeci(s, mali, false);
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        if (!gPo[s][poU][m]) continue;
        gPo[s][poKarte][m * 2] = PokerDijeli(s);
        gPo[s][poKarte][m * 2 + 1] = PokerDijeli(s);
    }
    PokerUlozi(s, mali, POKER_MALI);
    PokerUlozi(s, veliki, POKER_VELIKI);
    gPo[s][poNajveci] = POKER_VELIKI;
    gPo[s][poMinRaise] = POKER_VELIKI;
    PokerPoruka(s, "[Poker] Nova ruka. Mali blind: %s.", gPo[s][poIgrac][mali]);
    new prvi = PokerSljedeci(s, veliki, true);
    if (prvi == -1 || PokerZivi(s, true) < 2 && gPo[s][poUlog][prvi] >= gPo[s][poNajveci])
    {
        PokerKrajRunde(s);
        return;
    }
    PokerNaPotez(s, prvi);
}

static PokerNaPotez(s, m)
{
    gPo[s][poNaPotezu] = m;
    gPo[s][poRok] = POKER_POTEZ_S;
    new p = gPo[s][poIgrac][m];
    if (p != INVALID_PLAYER_ID && gPoTdIma[p]) SelectTextDraw(p, 0xE8C15AFF);
    PokerOsvjezi(s);
}

static PokerKrajRunde(s)
{
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        gPo[s][poUlog][m] = 0;
        gPo[s][poOdigrao][m] = false;
    }
    gPo[s][poNajveci] = 0;
    gPo[s][poMinRaise] = POKER_VELIKI;
    for (;;)
    {
        gPo[s][poRunda]++;
        if (gPo[s][poRunda] == 1) for (new i = 0; i < 3; i++) gPo[s][poPloca][gPo[s][poPlocaN]++] = PokerDijeli(s);
        else if (gPo[s][poRunda] <= 3) gPo[s][poPloca][gPo[s][poPlocaN]++] = PokerDijeli(s);
        if (gPo[s][poRunda] >= 4)
        {
            PokerObracun(s);
            return;
        }
        if (PokerZivi(s, true) >= 2) break;
    }
    PokerNaPotez(s, PokerSljedeci(s, gPo[s][poDiler], true));
}

static PokerDalje(s)
{
    if (PokerZivi(s) == 1)
    {
        PokerObracun(s);
        return;
    }
    new aktivni = PokerZivi(s, true);
    for (new i = 1; i <= POKER_MJESTA; i++)
    {
        new k = (gPo[s][poNaPotezu] + i) % POKER_MJESTA;
        if (!gPo[s][poU][k] || gPo[s][poFold][k] || gPo[s][poAllin][k]) continue;
        if (gPo[s][poOdigrao][k] && gPo[s][poUlog][k] >= gPo[s][poNajveci]) continue;
        if (aktivni == 1 && gPo[s][poUlog][k] >= gPo[s][poNajveci]) continue;
        PokerNaPotez(s, k);
        return;
    }
    PokerKrajRunde(s);
}

static PokerAkcija(s, m, akcija, iznos = 0)
{
    new p = gPo[s][poIgrac][m], za = gPo[s][poNajveci] - gPo[s][poUlog][m];
    switch (akcija)
    {
        case PTD_FOLD:
        {
            gPo[s][poFold][m] = true;
            if (p != INVALID_PLAYER_ID) PokerPoruka(s, "[Poker] %s odustaje.", p);
        }
        case PTD_CALL:
        {
            if (za > 0)
            {
                PokerUlozi(s, m, za);
                PokerPoruka(s, gPo[s][poAllin][m] ? ("[Poker] %s ide all-in (%d).") : ("[Poker] %s prati %d."), p, gPo[s][poUlog][m]);
            }
            else PokerPoruka(s, "[Poker] %s cheka.", p);
        }
        default:
        {
            new prije = gPo[s][poNajveci];
            PokerUlozi(s, m, iznos - gPo[s][poUlog][m]);
            if (gPo[s][poUlog][m] > prije)
            {
                if (gPo[s][poUlog][m] - prije >= gPo[s][poMinRaise]) gPo[s][poMinRaise] = gPo[s][poUlog][m] - prije;
                gPo[s][poNajveci] = gPo[s][poUlog][m];
                for (new k = 0; k < POKER_MJESTA; k++) gPo[s][poOdigrao][k] = false;
            }
            PokerPoruka(s, gPo[s][poAllin][m] ? ("[Poker] %s ide all-in (%d).") : ("[Poker] %s dize na %d."), p, gPo[s][poUlog][m]);
        }
    }
    gPo[s][poOdigrao][m] = true;
    PokerDalje(s);
}

static PokerObracun(s)
{
    new dio[POKER_MJESTA], ocjena[POKER_MJESTA], karte[7], ime[3][4], ukupno = PokerBanka(s), rake = 0;
    for (new m = 0; m < POKER_MJESTA; m++) dio[m] = gPo[s][poUkupno][m];
    if (gPo[s][poRunda] > 0)
    {
        rake = ukupno * POKER_RAKE / 100;
        if (rake > POKER_RAKE_MAX) rake = POKER_RAKE_MAX;
    }
    new bool:prikazi = PokerZivi(s) > 1;
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        if (!gPo[s][poU][m] || gPo[s][poFold][m]) continue;
        for (new i = 0; i < 5; i++) karte[i] = gPo[s][poPloca][i];
        karte[5] = gPo[s][poKarte][m * 2];
        karte[6] = gPo[s][poKarte][m * 2 + 1];
        ocjena[m] = PokerOcjena(karte, prikazi ? 7 : 2);
        if (!prikazi || gPo[s][poIgrac][m] == INVALID_PLAYER_ID) continue;
        PokerImeKarte(karte[5], ime[0], 4);
        PokerImeKarte(karte[6], ime[1], 4);
        for (new k = 0; k < POKER_MJESTA; k++)
        {
            new q = gPo[s][poIgrac][k];
            if (q != INVALID_PLAYER_ID) SendClientMessage(q, 0x2DD9A9FF, T(q, "[Poker] %s: %s %s - %s"), GetRPIme(gPo[s][poIgrac][m]), ime[0], ime[1], T(q, gPokerKat[ocjena[m] / 1048576]));
        }
    }
    for (;;)
    {
        new razina = 0;
        for (new m = 0; m < POKER_MJESTA; m++) if (gPo[s][poU][m] && !gPo[s][poFold][m] && dio[m] > 0 && (!razina || dio[m] < razina)) razina = dio[m];
        if (!razina) break;
        new banka = 0, najbolja = -1, pobjednici[POKER_MJESTA], n = 0;
        for (new m = 0; m < POKER_MJESTA; m++)
        {
            new d = dio[m] < razina ? dio[m] : razina;
            banka += d;
            dio[m] -= d;
        }
        new ostalo = 0;
        for (new m = 0; m < POKER_MJESTA; m++) if (gPo[s][poU][m] && !gPo[s][poFold][m] && dio[m] > 0) ostalo++;
        if (!ostalo) for (new m = 0; m < POKER_MJESTA; m++) banka += dio[m], dio[m] = 0;
        new r = rake < banka ? rake : banka;
        banka -= r;
        rake -= r;
        for (new m = 0; m < POKER_MJESTA; m++)
        {
            if (!gPo[s][poU][m] || gPo[s][poFold][m] || gPo[s][poUkupno][m] < razina) continue;
            if (ocjena[m] > najbolja)
            {
                najbolja = ocjena[m];
                n = 0;
            }
            if (ocjena[m] == najbolja) pobjednici[n++] = m;
        }
        for (new i = 0; i < n; i++)
        {
            new m = pobjednici[i], d = banka / n + (i == 0 ? banka % n : 0);
            gPo[s][poStog][m] += d;
            if (gPo[s][poIgrac][m] != INVALID_PLAYER_ID) PokerPoruka(s, "[Poker] %s osvaja %d.", gPo[s][poIgrac][m], d);
        }
        for (new m = 0; m < POKER_MJESTA; m++) if (gPo[s][poUkupno][m] >= razina) gPo[s][poUkupno][m] -= razina;
    }
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        gPo[s][poU][m] = false;
        gPo[s][poUlog][m] = gPo[s][poUkupno][m] = 0;
    }
    gPo[s][poFaza] = PO_CEKA;
    gPo[s][poPauza] = POKER_PAUZA_S;
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        new p = gPo[s][poIgrac][m];
        if (p == INVALID_PLAYER_ID || gPo[s][poStog][m] > 0) continue;
        SendClientMessage(p, COLOR_INFO, T(p, "Ostali ste bez zetona za stolom."));
        PokerUstani(p);
    }
    PokerOsvjezi(s);
}

PokerUstani(playerid)
{
    new s = gPokerStol[playerid];
    if (s == -1) return;
    new m = gPokerMjesto[playerid];
    if (gPo[s][poFaza] == PO_RUKA && gPo[s][poU][m] && !gPo[s][poFold][m])
    {
        gPo[s][poFold][m] = true;
        PokerPoruka(s, "[Poker] %s odustaje.", playerid);
    }
    PokerDajZetone(playerid, gPo[s][poStog][m]);
    gPo[s][poStog][m] = 0;
    gPo[s][poIgrac][m] = INVALID_PLAYER_ID;
    gPokerStol[playerid] = -1;
    if (IsPlayerConnected(playerid)) PokerTdUnisti(playerid);
    if (gPo[s][poFaza] == PO_RUKA && gPo[s][poNaPotezu] == m) PokerDalje(s);
    else if (gPo[s][poFaza] == PO_RUKA && PokerZivi(s) == 1) PokerObracun(s);
    else PokerOsvjezi(s);
}

PokerOdlazi(playerid)
{
    PokerUstani(playerid);
}

PokerVratiSve()
{
    for (new s = 0; s < POKER_MAX; s++)
    {
        if (!gPo[s][poIma]) continue;
        for (new m = 0; m < POKER_MJESTA; m++)
        {
            new p = gPo[s][poIgrac][m];
            if (p == INVALID_PLAYER_ID) continue;
            PokerDajZetone(p, gPo[s][poStog][m] + gPo[s][poUkupno][m]);
            gPo[s][poStog][m] = gPo[s][poUkupno][m] = 0;
        }
    }
}

static PokerBlizu(playerid)
{
    for (new s = 0; s < POKER_MAX; s++)
        if (gPo[s][poIma] && GetPlayerVirtualWorld(playerid) == gPo[s][poSvijet] && GetPlayerInterior(playerid) == gPo[s][poInt] && IsPlayerInRangeOfPoint(playerid, POKER_DOMET, gPo[s][poX], gPo[s][poY], gPo[s][poZ] + 1.0)) return s;
    return -1;
}

PokerKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new s = gPokerStol[playerid];
    if (s != -1)
    {
        if (!gPoTdIma[playerid]) PokerTdStvori(playerid);
        PokerOsvjeziIgraca(s, playerid);
        SelectTextDraw(playerid, 0xE8C15AFF);
        return 1;
    }
    s = PokerBlizu(playerid);
    if (s == -1) return 0;
    new slobodno = 0;
    for (new m = 0; m < POKER_MJESTA; m++) if (gPo[s][poIgrac][m] == INVALID_PLAYER_ID && !gPo[s][poU][m]) slobodno++;
    if (!slobodno) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Za ovim stolom nema slobodnih mjesta.")), 1;
    if (gInv[playerid][ITEM_ZETONI] < POKER_MIN_ULAZ) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Za poker trebate najmanje %d zetona. Kupite ih na blagajni kazina."), POKER_MIN_ULAZ), 1;
    new body[160], najvise = gInv[playerid][ITEM_ZETONI] < POKER_MAX_ULAZ ? gInv[playerid][ITEM_ZETONI] : POKER_MAX_ULAZ;
    gPokerOdabran[playerid] = s;
    format(body, sizeof body, T(playerid, "Blindovi %d/%d. S koliko zetona sjedate? (%d-%d)"), POKER_MALI, POKER_VELIKI, POKER_MIN_ULAZ, najvise);
    PrikaziDialog(playerid, DLG_POKER_ULAZ, DIALOG_STYLE_INPUT, T(playerid, "Poker"), body, T(playerid, "Sjedi"), T(playerid, "Odustani"));
    return 1;
}

DLG:poker_ulaz(playerid, response, const inputtext[])
{
    new s = gPokerOdabran[playerid];
    if (!response || s < 0 || s >= POKER_MAX || !gPo[s][poIma] || gPokerStol[playerid] != -1 || PokerBlizu(playerid) != s) return 1;
    new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 6 ? strval(inputtext) : 0;
    if (n < POKER_MIN_ULAZ || n > POKER_MAX_ULAZ || n > gInv[playerid][ITEM_ZETONI]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravan broj zetona."));
    new m = -1;
    for (new i = 0; i < POKER_MJESTA && m == -1; i++) if (gPo[s][poIgrac][i] == INVALID_PLAYER_ID && !gPo[s][poU][i]) m = i;
    if (m == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Za ovim stolom nema slobodnih mjesta."));
    if (!GiveItem(playerid, ITEM_ZETONI, -n)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravan broj zetona."));
    gPo[s][poIgrac][m] = playerid;
    gPo[s][poStog][m] = n;
    gPokerStol[playerid] = s;
    gPokerMjesto[playerid] = m;
    PokerTdStvori(playerid);
    PokerPoruka(s, "[Poker] %s sjeda za stol sa %d zetona.", playerid, n);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Y - vrati kursor, USTANI - napusti stol (zetoni se vracaju u inventar)."));
    if (gPo[s][poFaza] == PO_CEKA && !gPo[s][poPauza]) gPo[s][poPauza] = POKER_PAUZA_S;
    PokerOsvjezi(s);
    return 1;
}

DLG:poker_povisi(playerid, response, const inputtext[])
{
    new s = gPokerStol[playerid];
    if (s == -1 || gPo[s][poFaza] != PO_RUKA || gPo[s][poNaPotezu] != gPokerMjesto[playerid]) return 1;
    if (!response) return SelectTextDraw(playerid, 0xE8C15AFF), 1;
    new m = gPokerMjesto[playerid], najmanje = gPo[s][poNajveci] + gPo[s][poMinRaise], najvise = gPo[s][poUlog][m] + gPo[s][poStog][m];
    new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 7 ? strval(inputtext) : 0;
    if (n >= najvise) n = najvise;
    else if (n < najmanje)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najmanje dizanje je na %d."), najmanje);
        return SelectTextDraw(playerid, 0xE8C15AFF), 1;
    }
    PokerAkcija(s, m, PTD_RAISE, n);
    return 1;
}

PokerKlik(playerid, PlayerText:td)
{
    new s = gPokerStol[playerid];
    if (s == -1 || !gPoTdIma[playerid]) return 0;
    if (td == gPoTd[playerid][PTD_USTANI])
    {
        PokerUstani(playerid);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Ustali ste od pokera."));
        return 1;
    }
    new m = gPokerMjesto[playerid], akcija = -1;
    for (new i = PTD_FOLD; i <= PTD_ALLIN; i++) if (td == gPoTd[playerid][i]) akcija = i;
    if (akcija == -1) return 0;
    if (gPo[s][poFaza] != PO_RUKA || gPo[s][poNaPotezu] != m) return 1;
    new najmanje = gPo[s][poNajveci] + gPo[s][poMinRaise], najvise = gPo[s][poUlog][m] + gPo[s][poStog][m], body[128];
    if (akcija == PTD_ALLIN || (akcija == PTD_RAISE && najvise <= najmanje))
    {
        PokerAkcija(s, m, PTD_ALLIN, najvise);
        return 1;
    }
    if (akcija != PTD_RAISE)
    {
        PokerAkcija(s, m, akcija);
        return 1;
    }
    format(body, sizeof body, T(playerid, "Dizete na koliko ukupno? (%d-%d)"), najmanje, najvise);
    PrikaziDialog(playerid, DLG_POKER_POVISI, DIALOG_STYLE_INPUT, T(playerid, "Poker"), body, T(playerid, "Digni"), T(playerid, "Natrag"));
    return 1;
}

PokerTick()
{
    for (new s = 0; s < POKER_MAX; s++)
    {
        if (!gPo[s][poIma]) continue;
        for (new m = 0; m < POKER_MJESTA; m++)
        {
            new p = gPo[s][poIgrac][m];
            if (p == INVALID_PLAYER_ID) continue;
            if (GetPlayerVirtualWorld(p) != gPo[s][poSvijet] || !IsPlayerInRangeOfPoint(p, POKER_NAPUSTI, gPo[s][poX], gPo[s][poY], gPo[s][poZ]))
            {
                SendClientMessage(p, COLOR_INFO, T(p, "Ustali ste od pokera."));
                PokerUstani(p);
            }
        }
        if (gPo[s][poFaza] == PO_CEKA)
        {
            if (gPo[s][poPauza] && !--gPo[s][poPauza]) PokerNovaRuka(s);
            continue;
        }
        if (--gPo[s][poRok] > 0)
        {
            PokerOsvjezi(s);
            continue;
        }
        new m = gPo[s][poNaPotezu];
        PokerAkcija(s, m, gPo[s][poNajveci] > gPo[s][poUlog][m] ? PTD_FOLD : PTD_CALL);
    }
}

ShowPoker(playerid)
{
    new list[1024], red[96], len = 0;
    DodajStupac(list, len, T(playerid, "+ Novi poker stol na mojoj poziciji"));
    for (new s = 0, n = 0; s < POKER_MAX; s++)
    {
        if (!gPo[s][poIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Poker stol (%s) - obrisi"), gPo[s][poDb], CestaZona(gPo[s][poX], gPo[s][poY]));
        DodajStupac(list, len, red);
        gApPoker[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_POKER, DIALOG_STYLE_LIST, T(playerid, "Poker stolovi"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_poker(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new s = 0, Float:x, Float:y, Float:z, Float:a;
        while (s < POKER_MAX && gPo[s][poIma]) s++;
        if (s == POKER_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj poker stolova.")), ShowPoker(playerid);
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        z -= 1.0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO poker_tables (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid)));
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT last_insert_rowid()");
        gPo[s][poDb] = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        gPo[s][poX] = x;
        gPo[s][poY] = y;
        gPo[s][poZ] = z;
        gPo[s][poA] = a;
        gPo[s][poInt] = GetPlayerInterior(playerid);
        gPo[s][poSvijet] = GetPlayerVirtualWorld(playerid);
        PokerStvori(s);
        SetPlayerPos(playerid, x + 2.0 * floatcos(a, degrees), y + 2.0 * floatsin(a, degrees), z + 1.0);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Poker stol je postavljen."));
        return ShowPoker(playerid);
    }
    new i = listitem - 1;
    if (i < 0 || i >= POKER_MAX) return 1;
    new s = gApPoker[playerid][i];
    if (!gPo[s][poIma]) return ShowPoker(playerid);
    for (new m = 0; m < POKER_MJESTA; m++)
    {
        new p = gPo[s][poIgrac][m];
        if (p == INVALID_PLAYER_ID) continue;
        gPo[s][poStog][m] += gPo[s][poUkupno][m];
        gPo[s][poUkupno][m] = 0;
        gPo[s][poU][m] = false;
        PokerUstani(p);
    }
    gPo[s][poFaza] = PO_CEKA;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM poker_tables WHERE id = %d", gPo[s][poDb]));
    DestroyDynamicObject(gPo[s][poObj]);
    DestroyDynamic3DTextLabel(gPo[s][poTekst]);
    gPo[s][poIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Poker stol je obrisan."));
    return ShowPoker(playerid);
}
