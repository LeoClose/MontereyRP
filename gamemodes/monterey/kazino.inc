#define KAZINO_MAX          32
#define KAZINO_DOMET        2.5
#define KAZINO_SKIN         171
#define KAZINO_MIN_ULOG     10
#define KAZINO_MAX_ULOG     5000
#define KZ_TD               64
#define KZ_VRTI_MS          110
#define KZ_KORACI           14
#define KZ_SREDINA          320.0
#define BJ_KARATA           8
#define BJ_RUKE             4
#define BJ_W                214.0
#define BJ_RED              58.0
#define BJ_RAZMAK           18.0

enum { KZ_BLACKJACK, KZ_RULET, KZ_POKER, KZ_KOCKE, KZ_SLOT }
enum { KZI_POZADINA, KZI_NASLOV, KZI_STATUS, KZI_LIJEVO, KZI_DESNO, KZI_PRVI }
enum { BTD_DILER = KZI_PRVI, BTD_DOUBLE, BTD_SPLIT, BTD_SURRENDER, BTD_RUKA, BTD_KARTA = BTD_RUKA + BJ_RUKE }
enum { VTD_KARTA = KZI_PRVI, VTD_DRZI = VTD_KARTA + 5 }
enum { RTD_BROJ = KZI_PRVI, RTD_OPIS }
enum { DTD_IGRAC = KZI_PRVI, DTD_DILER }
enum { STD_OKVIR = KZI_PRVI, STD_VALJAK = STD_OKVIR + 3 }

enum E_KAZINO { bool:kzIma, kzDb, kzTip, Float:kzX, Float:kzY, Float:kzZ, Float:kzA, kzInt, kzSvijet, kzAktor, STREAMER_TAG_3D_TEXT_LABEL:kzTekst }
enum E_BJ { bjRuke, bjAktivna, bjDilerKarte[BJ_KARATA], bjDilerN, bool:bjOsiguranje, bjOsigUlog, bool:bjPrvi, bool:bjPredao, bool:bjAsovi }

static gKaz[KAZINO_MAX][E_KAZINO];
static gApKazino[MAX_PLAYERS];
static PlayerText:gKzTd[MAX_PLAYERS][KZ_TD], gKzTdN[MAX_PLAYERS], bool:gKzOtvoren[MAX_PLAYERS], bool:gKzIgra[MAX_PLAYERS], gKzStol[MAX_PLAYERS], gKzTip[MAX_PLAYERS], gKzUlog[MAX_PLAYERS], gKzRunda[MAX_PLAYERS];
static gBj[MAX_PLAYERS][E_BJ], gBjKarte[MAX_PLAYERS][BJ_RUKE][BJ_KARATA], gBjN[MAX_PLAYERS][BJ_RUKE], gBjUlog[MAX_PLAYERS][BJ_RUKE];
static gVpKarte[MAX_PLAYERS][5], bool:gVpDrzi[MAX_PLAYERS][5], bool:gVpKoristeno[MAX_PLAYERS][52];
static gRulTip[MAX_PLAYERS], gRulBroj[MAX_PLAYERS], gRulIshod[MAX_PLAYERS];
static gKocke[MAX_PLAYERS][4], gSlot[MAX_PLAYERS][3];

static const gKzIme[][] = { "Blackjack", "Ruleta", "Video poker", "Kocke", "Slot" };
static const gBjBoja[] = "cdhs";
static const gRulTipovi[][] = { "Crveno", "Crno", "Par", "Nepar", "1-18", "19-36", "1. tucet (1-12)", "2. tucet (13-24)", "3. tucet (25-36)", "Jedan broj (0-36)" };
static const gVpIme[][] = { "Nista", "Jacks or Better", "Two Pair", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush", "Royal Flush" };
static const gVpIsplata[] = { 0, 1, 2, 3, 4, 6, 9, 25, 50, 250 };
static const gSlotSprite[][] = { "LD_SLOT:cherry", "LD_SLOT:grapes", "LD_SLOT:bell", "LD_SLOT:bar1_o", "LD_SLOT:bar2_o", "LD_SLOT:r_69" };
static const gSlotTezina[] = { 35, 25, 18, 12, 7, 3 };
static const gSlotIsplata[] = { 6, 12, 20, 40, 80, 200 };
enum { VP_NISTA, VP_JOB, VP_2P, VP_3K, VP_STRAIGHT, VP_FLUSH, VP_FH, VP_4K, VP_SF, VP_ROYAL }

InitKazino()
{
    new k1[] = { 0, 25 }, k2[] = { 13, 39, 8 }, k3[] = { 9, 18, 45 };
    assert(BjVrijednost(k1, 2) == 21 && BjVrijednost(k2, 3) == 21 && BjVrijednost(k3, 3) == 23);
    new r1[] = { 0, 9, 10, 11, 12 }, r2[] = { 0, 1, 2, 3, 17 }, r3[] = { 10, 23, 4, 5, 7 }, r4[] = { 1, 14, 27, 4, 17 }, r5[] = { 9, 22, 2, 16, 30 }, r6[] = { 5, 18, 7, 20, 32 };
    assert(VpOcjena(r1) == VP_ROYAL && VpOcjena(r2) == VP_STRAIGHT && VpOcjena(r3) == VP_JOB && VpOcjena(r4) == VP_FH && VpOcjena(r5) == VP_NISTA && VpOcjena(r6) == VP_2P);
    assert(RulDobitak(0, 0, 1) == 2 && RulDobitak(0, 0, 2) == 0 && RulDobitak(8, 0, 36) == 3 && RulDobitak(9, 17, 17) == 36 && RulDobitak(2, 0, 0) == 0 && RulDobitak(9, 0, 0) == 36);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS casino_dealers (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT igra FROM casino_dealers LIMIT 1");
    if (res) DB_FreeResultSet(res);
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE casino_dealers ADD COLUMN igra INTEGER NOT NULL DEFAULT 0"));
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world, igra FROM casino_dealers ORDER BY id LIMIT %d", KAZINO_MAX);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++)
    {
        gKaz[k][kzDb] = DB_GetFieldInt(res, 0);
        gKaz[k][kzX] = DB_GetFieldFloat(res, 1);
        gKaz[k][kzY] = DB_GetFieldFloat(res, 2);
        gKaz[k][kzZ] = DB_GetFieldFloat(res, 3);
        gKaz[k][kzA] = DB_GetFieldFloat(res, 4);
        gKaz[k][kzInt] = DB_GetFieldInt(res, 5);
        gKaz[k][kzSvijet] = DB_GetFieldInt(res, 6);
        gKaz[k][kzTip] = clamp(DB_GetFieldInt(res, 7), 0, sizeof gKzIme - 1);
        KazinoStvori(k);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static bool:KazinoMasina(tip)
{
    return tip == KZ_POKER || tip == KZ_SLOT;
}

static KazinoStvori(k)
{
    new tekst[32];
    gKaz[k][kzIma] = true;
    gKaz[k][kzAktor] = INVALID_ACTOR_ID;
    if (!KazinoMasina(gKaz[k][kzTip]))
    {
        gKaz[k][kzAktor] = CreateActor(KAZINO_SKIN, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ], gKaz[k][kzA]);
        SetActorVirtualWorld(gKaz[k][kzAktor], gKaz[k][kzSvijet]);
        SetActorInvulnerable(gKaz[k][kzAktor], true);
    }
    format(tekst, sizeof tekst, "%s\n[Y]", gKzIme[gKaz[k][kzTip]]);
    gKaz[k][kzTekst] = CreateDynamic3DTextLabel(tekst, COLOR_ORG, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ] + (KazinoMasina(gKaz[k][kzTip]) ? 0.5 : 1.2), 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gKaz[k][kzSvijet], gKaz[k][kzInt]);
}

static bool:KazinoBlizu(playerid, k)
{
    return k >= 0 && k < KAZINO_MAX && gKaz[k][kzIma] && GetPlayerVirtualWorld(playerid) == gKaz[k][kzSvijet] && GetPlayerInterior(playerid) == gKaz[k][kzInt] && IsPlayerInRangeOfPoint(playerid, KAZINO_DOMET, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ]);
}

static KzDiler(playerid, const anim[])
{
    new k = gKzStol[playerid];
    if (k >= 0 && k < KAZINO_MAX && gKaz[k][kzIma] && gKaz[k][kzAktor] != INVALID_ACTOR_ID) ApplyActorAnimation(gKaz[k][kzAktor], "CASINO", anim, 4.1, false, false, false, false, 0);
}

static KzKarta(playerid)
{
    new c;
    do c = random(52);
    while (gVpKoristeno[playerid][c]);
    gVpKoristeno[playerid][c] = true;
    return c;
}

static KzSprite(playerid, idx, Float:x, Float:y, Float:w, Float:h, const sprite[], boja = -1)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, sprite);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gKzTd[playerid][idx] = td;
    if (idx >= gKzTdN[playerid]) gKzTdN[playerid] = idx + 1;
}

static KzTekst(playerid, idx, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawBackgroundColour(playerid, td, 0x000000AA);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    gKzTd[playerid][idx] = td;
    if (idx >= gKzTdN[playerid]) gKzTdN[playerid] = idx + 1;
}

static KzGumb(playerid, idx, Float:x, Float:y, Float:w, boja)
{
    KzTekst(playerid, idx, x, y, "_", 0xFFFFFFFF, 0.2, 1.0, TEXT_DRAW_ALIGN_CENTER);
    new PlayerText:td = gKzTd[playerid][idx];
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 12.0, w);
    PlayerTextDrawSetSelectable(playerid, td, true);
}

static Float:KzPanel(playerid, Float:w, Float:h)
{
    new Float:x = KZ_SREDINA - w / 2.0, Float:y = 224.0 - h / 2.0;
    KzSprite(playerid, KZI_POZADINA, x, y, w, h, "LD_SPAC:white", 0x0F4D2AEE);
    KzTekst(playerid, KZI_NASLOV, KZ_SREDINA, y + 4.0, gKzIme[gKzTip[playerid]], 0xE8C15AFF, 0.4, 1.6, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetString(playerid, gKzTd[playerid][KZI_NASLOV], T(playerid, gKzIme[gKzTip[playerid]]));
    KzTekst(playerid, KZI_STATUS, KZ_SREDINA, y + h - 58.0, "_", 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_ALIGN_CENTER);
    KzGumb(playerid, KZI_LIJEVO, x + 38.0, y + h - 38.0, 60.0, 0x1E5AA8FF);
    KzGumb(playerid, KZI_DESNO, x + w - 38.0, y + h - 38.0, 60.0, 0x8B2E2EFF);
    for (new i = KZI_POZADINA; i <= KZI_STATUS; i++) PlayerTextDrawShow(playerid, gKzTd[playerid][i]);
    return y;
}

static KzPostavi(playerid, idx, const tekst[], bool:vidljivo = true)
{
    PlayerTextDrawSetString(playerid, gKzTd[playerid][idx], tekst);
    if (vidljivo) PlayerTextDrawShow(playerid, gKzTd[playerid][idx]);
    else PlayerTextDrawHide(playerid, gKzTd[playerid][idx]);
}

static KzUnisti(playerid)
{
    for (new i = 0; i < gKzTdN[playerid]; i++) PlayerTextDrawDestroy(playerid, gKzTd[playerid][i]);
    gKzTdN[playerid] = 0;
}

static KzKraj(playerid, const ishod[], ulog, isplata)
{
    new tekst[128];
    gKzIgra[playerid] = false;
    if (isplata) DajNovac(playerid, isplata);
    KzDiler(playerid, isplata > ulog ? ("cards_lose") : ("cards_win"));
    format(tekst, sizeof tekst, T(playerid, "%s Ulog: $%d, isplata: $%d"), T(playerid, ishod), ulog, isplata);
    SendClientMessage(playerid, isplata > ulog ? COLOR_INFO : COLOR_ERROR, "%s: %s", T(playerid, gKzIme[gKzTip[playerid]]), tekst);
    if (!gKzOtvoren[playerid]) return;
    format(tekst, sizeof tekst, T(playerid, "%s~n~Ulog: $%d, isplata: $%d"), T(playerid, ishod), ulog, isplata);
    KzPostavi(playerid, KZI_STATUS, tekst);
    KzPostavi(playerid, KZI_LIJEVO, T(playerid, "Ponovno"));
    KzPostavi(playerid, KZI_DESNO, T(playerid, "Zatvori"));
}

KazinoKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new k = 0;
    while (k < KAZINO_MAX && !KazinoBlizu(playerid, k)) k++;
    if (k == KAZINO_MAX) return 0;
    if (gKzIgra[playerid]) return 1;
    gKzStol[playerid] = k;
    gKzTip[playerid] = gKaz[k][kzTip];
    if (gKaz[k][kzAktor] != INVALID_ACTOR_ID) AktorKaze(playerid, gKaz[k][kzAktor], "Krupije", "Dobrodosli za stol, koliko ulazete?");
    if (gKzTip[playerid] == KZ_RULET)
    {
        new list[512], len = 0;
        for (new i = 0; i < sizeof gRulTipovi; i++) DodajStupac(list, len, T(playerid, gRulTipovi[i]));
        PrikaziDialog(playerid, DLG_KAZINO_RULET, DIALOG_STYLE_LIST, T(playerid, "Ruleta"), list, T(playerid, "Dalje"), T(playerid, "Zatvori"));
        return 1;
    }
    return KazinoUlogDialog(playerid);
}

static KazinoUlogDialog(playerid)
{
    new body[128];
    if (gKzTip[playerid] == KZ_RULET && gRulTip[playerid] == 9) format(body, sizeof body, T(playerid, "Unesite broj (0-36) i ulog ($%d - $%d), npr. 17 100:"), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
    else format(body, sizeof body, T(playerid, "Unesite ulog ($%d - $%d):"), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
    PrikaziDialog(playerid, DLG_KAZINO_ULOG, DIALOG_STYLE_INPUT, T(playerid, gKzIme[gKzTip[playerid]]), body, T(playerid, "Igraj"), T(playerid, "Zatvori"));
    return 1;
}

DLG:kazino_rulet(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gRulTipovi || gKzIgra[playerid])
    {
        KazinoZatvori(playerid);
        return 1;
    }
    gRulTip[playerid] = listitem;
    return KazinoUlogDialog(playerid);
}

static bool:KzPokreni(playerid, ulog)
{
    if (ulog < KAZINO_MIN_ULOG || ulog > KAZINO_MAX_ULOG)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulog mora biti izmedu $%d i $%d."), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
        return false;
    }
    if (gNovac[playerid] < ulog)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        return false;
    }
    DajNovac(playerid, -ulog);
    gKzUlog[playerid] = ulog;
    gKzIgra[playerid] = true;
    gKzRunda[playerid]++;
    KzUnisti(playerid);
    if (!gKzOtvoren[playerid])
    {
        gKzOtvoren[playerid] = true;
        SelectTextDraw(playerid, 0xE8C15AFF);
    }
    switch (gKzTip[playerid])
    {
        case KZ_BLACKJACK: BjPocni(playerid);
        case KZ_POKER: VpPocni(playerid);
        case KZ_RULET: RulPocni(playerid);
        case KZ_KOCKE: KockePocni(playerid);
        case KZ_SLOT: SlotPocni(playerid);
    }
    return true;
}

DLG:kazino_ulog(playerid, response, const inputtext[])
{
    if (gKzIgra[playerid]) return 1;
    if (!response || !KazinoBlizu(playerid, gKzStol[playerid]))
    {
        KazinoZatvori(playerid);
        return 1;
    }
    new idx = 0, p1[12], p2[12];
    SljedeciParam(inputtext, idx, p1);
    if (gKzTip[playerid] == KZ_RULET && gRulTip[playerid] == 9)
    {
        SljedeciParam(inputtext, idx, p2);
        if (!ProvjeriBroj(p1) || strval(p1) > 36) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Broj mora biti od 0 do 36."));
        gRulBroj[playerid] = strval(p1);
        p1 = p2;
    }
    KzPokreni(playerid, ProvjeriBroj(p1) && strlen(p1) < 9 ? strval(p1) : 0);
    return 1;
}

static KzVrti(playerid)
{
    SetTimerEx("KazinoVrti", KZ_VRTI_MS, false, "iii", playerid, gKzRunda[playerid], 0);
}

forward KazinoVrti(playerid, runda, korak);
public KazinoVrti(playerid, runda, korak)
{
    if (!gKzIgra[playerid] || gKzRunda[playerid] != runda) return;
    if (korak >= KZ_KORACI)
    {
        KzZavrsi(playerid);
        return;
    }
    switch (gKzTip[playerid])
    {
        case KZ_RULET: RulCrtaj(playerid, random(37));
        case KZ_KOCKE: KockeCrtaj(playerid, true);
        case KZ_SLOT: SlotCrtaj(playerid, korak);
    }
    SetTimerEx("KazinoVrti", KZ_VRTI_MS, false, "iii", playerid, runda, korak + 1);
}

static KzZavrsi(playerid)
{
    switch (gKzTip[playerid])
    {
        case KZ_BLACKJACK: BjKraj(playerid);
        case KZ_POKER: VpVuci(playerid);
        case KZ_RULET: RulKraj(playerid);
        case KZ_KOCKE: KockeKraj(playerid);
        case KZ_SLOT: SlotKraj(playerid);
    }
}

KazinoZatvori(playerid)
{
    if (!gKzOtvoren[playerid]) return;
    if (gKzIgra[playerid]) KzZavrsi(playerid);
    gKzOtvoren[playerid] = false;
    KzUnisti(playerid);
    CancelSelectTextDraw(playerid);
}

KazinoDisconnect(playerid)
{
    gKzOtvoren[playerid] = false;
    gKzIgra[playerid] = false;
    gKzTdN[playerid] = 0;
}

KazinoKlik(playerid, PlayerText:td)
{
    if (!gKzOtvoren[playerid]) return 0;
    if (!gKzIgra[playerid])
    {
        if (td == gKzTd[playerid][KZI_DESNO]) KazinoZatvori(playerid);
        else if (td == gKzTd[playerid][KZI_LIJEVO])
        {
            if (!KazinoBlizu(playerid, gKzStol[playerid])) KazinoZatvori(playerid);
            else if (!KzPokreni(playerid, gKzUlog[playerid])) KazinoUlogDialog(playerid);
        }
        else return 0;
        return 1;
    }
    switch (gKzTip[playerid])
    {
        case KZ_BLACKJACK: return BjKlik(playerid, td);
        case KZ_POKER: return VpKlik(playerid, td);
    }
    return 0;
}

static BjVrijednost(const karte[], n)
{
    new zbroj = 0, asovi = 0;
    for (new i = 0; i < n; i++)
    {
        new r = karte[i] % 13 + 1;
        if (r == 1) asovi++;
        zbroj += r == 1 ? 11 : r > 10 ? 10 : r;
    }
    while (zbroj > 21 && asovi-- > 0) zbroj -= 10;
    return zbroj;
}

static BjRed(playerid, red, const karte[], n, skrivena)
{
    new sprite[16];
    for (new i = 0; i < BJ_KARATA; i++)
    {
        new idx = BTD_KARTA + red * BJ_KARATA + i;
        if (i >= n)
        {
            PlayerTextDrawHide(playerid, gKzTd[playerid][idx]);
            continue;
        }
        if (i == skrivena) sprite = "LD_CARD:cdback";
        else format(sprite, sizeof sprite, "LD_CARD:cd%d%c", karte[i] % 13 + 1, gBjBoja[karte[i] / 13]);
        KzPostavi(playerid, idx, sprite);
    }
}

static BjNapravi(playerid)
{
    new Float:s = float(gBj[playerid][bjRuke] - 1) * BJ_RED, Float:x = KZ_SREDINA - BJ_W / 2.0, Float:y = KzPanel(playerid, BJ_W, 222.0 + s);
    KzTekst(playerid, BTD_DILER, x + 12.0, y + 24.0, "_", 0xFFFFFFFF, 0.2, 0.9);
    KzGumb(playerid, BTD_DOUBLE, KZ_SREDINA, y + 184.0 + s, 60.0, 0x2E7D32FF);
    KzGumb(playerid, BTD_SPLIT, KZ_SREDINA - 34.0, y + 202.0 + s, 62.0, 0x8A6D1EFF);
    KzGumb(playerid, BTD_SURRENDER, KZ_SREDINA + 34.0, y + 202.0 + s, 62.0, 0x555555FF);
    for (new h = 0; h < BJ_RUKE; h++) KzTekst(playerid, BTD_RUKA + h, x + 12.0, y + 84.0 + float(h) * BJ_RED, "_", 0xFFFFFFFF, 0.2, 0.9);
    for (new r = 0; r <= BJ_RUKE; r++)
        for (new i = 0; i < BJ_KARATA; i++)
            KzSprite(playerid, BTD_KARTA + r * BJ_KARATA + i, x + 12.0 + float(i) * BJ_RAZMAK, y + (r ? 96.0 + float(r - 1) * BJ_RED : 36.0), 32.0, 46.0, "LD_CARD:cdback");
    PlayerTextDrawShow(playerid, gKzTd[playerid][BTD_DILER]);
}

static bool:BjMozeSplit(playerid)
{
    new h = gBj[playerid][bjAktivna];
    return !gBj[playerid][bjOsiguranje] && gBj[playerid][bjRuke] < BJ_RUKE && gBjN[playerid][h] == 2 && gBjKarte[playerid][h][0] % 13 == gBjKarte[playerid][h][1] % 13 && gNovac[playerid] >= gBjUlog[playerid][h];
}

static bool:BjMozeDouble(playerid)
{
    new h = gBj[playerid][bjAktivna];
    return !gBj[playerid][bjOsiguranje] && gBjN[playerid][h] == 2 && gNovac[playerid] >= gBjUlog[playerid][h];
}

static bool:BjMozeSurrender(playerid)
{
    return !gBj[playerid][bjOsiguranje] && gBj[playerid][bjPrvi] && gBj[playerid][bjRuke] == 1;
}

static BjCrtaj(playerid)
{
    new tekst[64], ruke = gBj[playerid][bjRuke], bool:kraj = !gKzIgra[playerid], bool:osig = gBj[playerid][bjOsiguranje];
    BjRed(playerid, 0, gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN], kraj ? -1 : 1);
    format(tekst, sizeof tekst, T(playerid, "Diler: %d"), BjVrijednost(gBj[playerid][bjDilerKarte], kraj ? gBj[playerid][bjDilerN] : 1));
    KzPostavi(playerid, BTD_DILER, tekst);
    for (new h = 0; h < ruke; h++)
    {
        new v = BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]);
        BjRed(playerid, h + 1, gBjKarte[playerid][h], gBjN[playerid][h], -1);
        if (ruke == 1) format(tekst, sizeof tekst, T(playerid, "Vi: %d"), v);
        else format(tekst, sizeof tekst, T(playerid, "Ruka %d: %d"), h + 1, v);
        PlayerTextDrawColour(playerid, gKzTd[playerid][BTD_RUKA + h], !kraj && ruke > 1 && h == gBj[playerid][bjAktivna] ? 0xE8C15AFF : 0xFFFFFFFF);
        KzPostavi(playerid, BTD_RUKA + h, tekst);
    }
    KzPostavi(playerid, BTD_DOUBLE, "Double", !kraj && BjMozeDouble(playerid));
    KzPostavi(playerid, BTD_SPLIT, "Split", !kraj && BjMozeSplit(playerid));
    KzPostavi(playerid, BTD_SURRENDER, "Surrender", !kraj && BjMozeSurrender(playerid));
    if (kraj) return;
    KzPostavi(playerid, KZI_LIJEVO, osig ? ("Insurance") : ("Hit"));
    KzPostavi(playerid, KZI_DESNO, osig ? ("No") : ("Stand"));
    new ukupno = gBj[playerid][bjOsigUlog];
    for (new h = 0; h < ruke; h++) ukupno += gBjUlog[playerid][h];
    if (osig) format(tekst, sizeof tekst, T(playerid, "Diler ima asa. Osiguranje za $%d?"), gBjUlog[playerid][0] / 2);
    else format(tekst, sizeof tekst, T(playerid, "Ulog: $%d"), ukupno);
    KzPostavi(playerid, KZI_STATUS, tekst);
}

static BjPocni(playerid)
{
    gBj[playerid][bjRuke] = 1;
    gBj[playerid][bjAktivna] = 0;
    gBj[playerid][bjDilerN] = 2;
    gBj[playerid][bjOsigUlog] = 0;
    gBj[playerid][bjPrvi] = true;
    gBj[playerid][bjPredao] = false;
    gBj[playerid][bjAsovi] = false;
    gBjN[playerid][0] = 2;
    gBjUlog[playerid][0] = gKzUlog[playerid];
    for (new i = 0; i < 2; i++)
    {
        gBjKarte[playerid][0][i] = random(52);
        gBj[playerid][bjDilerKarte][i] = random(52);
    }
    gBj[playerid][bjOsiguranje] = gBj[playerid][bjDilerKarte][0] % 13 == 0;
    KzDiler(playerid, "dealone");
    BjNapravi(playerid);
    BjCrtaj(playerid);
    if (!gBj[playerid][bjOsiguranje]) BjProvjeri(playerid);
}

static BjProvjeri(playerid)
{
    gBj[playerid][bjOsiguranje] = false;
    if (BjVrijednost(gBj[playerid][bjDilerKarte], 2) == 21 || BjVrijednost(gBjKarte[playerid][0], 2) == 21) BjKraj(playerid);
    else BjCrtaj(playerid);
}

static BjStani(playerid)
{
    gBj[playerid][bjPrvi] = false;
    if (++gBj[playerid][bjAktivna] >= gBj[playerid][bjRuke] || gBj[playerid][bjAsovi])
    {
        BjKraj(playerid);
        return;
    }
    new h = gBj[playerid][bjAktivna];
    BjCrtaj(playerid);
    if (BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]) == 21) BjStani(playerid);
}

static BjKraj(playerid)
{
    new ruke = gBj[playerid][bjRuke], bool:prirodni = ruke == 1 && gBjN[playerid][0] == 2 && BjVrijednost(gBjKarte[playerid][0], 2) == 21, bool:vuci = false;
    gBj[playerid][bjOsiguranje] = false;
    for (new h = 0; h < ruke; h++) if (BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]) <= 21) vuci = true;
    if (vuci && !prirodni && !gBj[playerid][bjPredao] && BjVrijednost(gBj[playerid][bjDilerKarte], 2) != 21)
        while (BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]) < 17 && gBj[playerid][bjDilerN] < BJ_KARATA)
            gBj[playerid][bjDilerKarte][gBj[playerid][bjDilerN]++] = random(52);
    new dl = BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]), bool:dilerPrirodni = dl == 21 && gBj[playerid][bjDilerN] == 2;
    new ulozeno = gBj[playerid][bjOsigUlog], ukupno = 0, ishod[64], tekst[128];
    if (gBj[playerid][bjOsigUlog] && dilerPrirodni)
    {
        ukupno += gBj[playerid][bjOsigUlog] * 3;
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Osiguranje isplaceno: $%d."), gBj[playerid][bjOsigUlog] * 3);
    }
    for (new h = 0; h < ruke; h++)
    {
        new u = gBjUlog[playerid][h], ig = BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]), isplata = 0;
        ulozeno += u;
        if (gBj[playerid][bjPredao]) { isplata = u / 2; ishod = "Predali ste, vraca vam se pola uloga."; }
        else if (ig > 21) ishod = "Prebacili ste 21. Gubite ulog.";
        else if (prirodni && !dilerPrirodni) { isplata = u * 5 / 2; ishod = "Blackjack! Isplata 3:2."; }
        else if (dilerPrirodni && !prirodni) ishod = "Diler ima blackjack. Gubite ulog.";
        else if (dl > 21 || ig > dl) { isplata = u * 2; ishod = "Pobijedili ste!"; }
        else if (ig == dl) { isplata = u; ishod = "Nerijeseno, ulog vam se vraca."; }
        else ishod = "Diler pobjeduje.";
        ukupno += isplata;
        if (ruke == 1) continue;
        format(tekst, sizeof tekst, T(playerid, "%s Ulog: $%d, isplata: $%d"), T(playerid, ishod), u, isplata);
        SendClientMessage(playerid, isplata > u ? COLOR_INFO : COLOR_ERROR, "Blackjack (%d/%d): %s", h + 1, ruke, tekst);
    }
    KzKraj(playerid, ruke == 1 ? ishod : ("Split"), ulozeno, ukupno);
    if (gKzOtvoren[playerid]) BjCrtaj(playerid);
}

static BjKlik(playerid, PlayerText:td)
{
    new h = gBj[playerid][bjAktivna];
    if (gBj[playerid][bjOsiguranje])
    {
        if (td == gKzTd[playerid][KZI_LIJEVO])
        {
            new cijena = gBjUlog[playerid][0] / 2;
            if (gNovac[playerid] < cijena)
            {
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
                return 1;
            }
            DajNovac(playerid, -cijena);
            gBj[playerid][bjOsigUlog] = cijena;
        }
        else if (td != gKzTd[playerid][KZI_DESNO]) return 0;
        BjProvjeri(playerid);
        return 1;
    }
    if (td == gKzTd[playerid][KZI_LIJEVO])
    {
        gBj[playerid][bjPrvi] = false;
        gBjKarte[playerid][h][gBjN[playerid][h]++] = random(52);
        KzDiler(playerid, "dealone");
        BjCrtaj(playerid);
        if (BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]) >= 21 || gBjN[playerid][h] == BJ_KARATA) BjStani(playerid);
    }
    else if (td == gKzTd[playerid][KZI_DESNO]) BjStani(playerid);
    else if (td == gKzTd[playerid][BTD_DOUBLE])
    {
        if (!BjMozeDouble(playerid)) return 1;
        DajNovac(playerid, -gBjUlog[playerid][h]);
        gBjUlog[playerid][h] *= 2;
        gBjKarte[playerid][h][gBjN[playerid][h]++] = random(52);
        KzDiler(playerid, "dealone");
        BjStani(playerid);
    }
    else if (td == gKzTd[playerid][BTD_SURRENDER])
    {
        if (!BjMozeSurrender(playerid)) return 1;
        gBj[playerid][bjPredao] = true;
        BjKraj(playerid);
    }
    else if (td == gKzTd[playerid][BTD_SPLIT])
    {
        if (!BjMozeSplit(playerid)) return 1;
        new n = gBj[playerid][bjRuke]++;
        DajNovac(playerid, -gBjUlog[playerid][h]);
        gBjUlog[playerid][n] = gBjUlog[playerid][h];
        gBjKarte[playerid][n][0] = gBjKarte[playerid][h][1];
        gBjKarte[playerid][h][1] = random(52);
        gBjKarte[playerid][n][1] = random(52);
        gBjN[playerid][n] = 2;
        gBj[playerid][bjPrvi] = false;
        gBj[playerid][bjAsovi] = gBjKarte[playerid][h][0] % 13 == 0;
        KzDiler(playerid, "dealone");
        KzUnisti(playerid);
        BjNapravi(playerid);
        BjCrtaj(playerid);
        if (gBj[playerid][bjAsovi]) BjKraj(playerid);
        else if (BjVrijednost(gBjKarte[playerid][h], 2) == 21) BjStani(playerid);
    }
    else return 0;
    return 1;
}

static VpOcjena(const karte[])
{
    new broj[13], boja = karte[0] / 13, bool:flush = true, parovi = 0, trica = 0, poker = 0, bool:job = false, najmanji = 13, najveci = -1;
    for (new i = 0; i < 5; i++)
    {
        new r = karte[i] % 13;
        broj[r]++;
        if (karte[i] / 13 != boja) flush = false;
    }
    for (new r = 0; r < 13; r++)
    {
        if (!broj[r]) continue;
        if (r < najmanji) najmanji = r;
        if (r > najveci) najveci = r;
        if (broj[r] == 4) poker++;
        else if (broj[r] == 3) trica++;
        else if (broj[r] == 2)
        {
            parovi++;
            if (r == 0 || r >= 10) job = true;
        }
    }
    new bool:razliciti = !parovi && !trica && !poker, bool:skala = razliciti && (najveci - najmanji == 4 || (broj[0] && broj[9] && broj[10] && broj[11] && broj[12]));
    if (skala && flush) return broj[0] && broj[12] ? VP_ROYAL : VP_SF;
    if (poker) return VP_4K;
    if (trica && parovi) return VP_FH;
    if (flush) return VP_FLUSH;
    if (skala) return VP_STRAIGHT;
    if (trica) return VP_3K;
    if (parovi == 2) return VP_2P;
    return job ? VP_JOB : VP_NISTA;
}

static VpCrtaj(playerid)
{
    new sprite[16];
    for (new i = 0; i < 5; i++)
    {
        format(sprite, sizeof sprite, "LD_CARD:cd%d%c", gVpKarte[playerid][i] % 13 + 1, gBjBoja[gVpKarte[playerid][i] / 13]);
        KzPostavi(playerid, VTD_KARTA + i, sprite);
        KzPostavi(playerid, VTD_DRZI + i, T(playerid, "DRZI"), gKzIgra[playerid] && gVpDrzi[playerid][i]);
    }
}

static VpPocni(playerid)
{
    new Float:x = KZ_SREDINA - 120.0, Float:y = KzPanel(playerid, 240.0, 168.0);
    for (new c = 0; c < 52; c++) gVpKoristeno[playerid][c] = false;
    for (new i = 0; i < 5; i++)
    {
        gVpKarte[playerid][i] = KzKarta(playerid);
        gVpDrzi[playerid][i] = false;
        KzSprite(playerid, VTD_KARTA + i, x + 14.0 + float(i) * 44.0, y + 28.0, 40.0, 58.0, "LD_CARD:cdback");
        PlayerTextDrawSetSelectable(playerid, gKzTd[playerid][VTD_KARTA + i], true);
    }
    for (new i = 0; i < 5; i++) KzTekst(playerid, VTD_DRZI + i, x + 34.0 + float(i) * 44.0, y + 90.0, "_", 0xE8C15AFF, 0.18, 0.85, TEXT_DRAW_ALIGN_CENTER);
    VpCrtaj(playerid);
    KzPostavi(playerid, KZI_STATUS, T(playerid, "Kliknite karte koje zadrzavate."));
    KzPostavi(playerid, KZI_LIJEVO, "Draw");
    KzPostavi(playerid, KZI_DESNO, "_", false);
}

static VpVuci(playerid)
{
    for (new i = 0; i < 5; i++) if (!gVpDrzi[playerid][i]) gVpKarte[playerid][i] = KzKarta(playerid);
    new ruka = VpOcjena(gVpKarte[playerid]);
    gKzIgra[playerid] = false;
    if (gKzOtvoren[playerid]) VpCrtaj(playerid);
    KzKraj(playerid, gVpIme[ruka], gKzUlog[playerid], gKzUlog[playerid] * gVpIsplata[ruka]);
}

static VpKlik(playerid, PlayerText:td)
{
    if (td == gKzTd[playerid][KZI_LIJEVO])
    {
        VpVuci(playerid);
        return 1;
    }
    for (new i = 0; i < 5; i++)
    {
        if (td != gKzTd[playerid][VTD_KARTA + i]) continue;
        gVpDrzi[playerid][i] = !gVpDrzi[playerid][i];
        VpCrtaj(playerid);
        return 1;
    }
    return 0;
}

static bool:RulCrveni(n)
{
    static const crveni[] = { 1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36 };
    for (new i = 0; i < sizeof crveni; i++) if (crveni[i] == n) return true;
    return false;
}

static RulDobitak(tip, broj, n)
{
    if (tip == 9) return n == broj ? 36 : 0;
    if (!n) return 0;
    switch (tip)
    {
        case 0: return RulCrveni(n) ? 2 : 0;
        case 1: return RulCrveni(n) ? 0 : 2;
        case 2: return n % 2 ? 0 : 2;
        case 3: return n % 2 ? 2 : 0;
        case 4: return n <= 18 ? 2 : 0;
        case 5: return n > 18 ? 2 : 0;
    }
    return (n - 1) / 12 == tip - 6 ? 3 : 0;
}

static RulCrtaj(playerid, n)
{
    new tekst[8];
    valstr(tekst, n);
    PlayerTextDrawBoxColour(playerid, gKzTd[playerid][RTD_BROJ], !n ? 0x1B7F3AFF : RulCrveni(n) ? 0xB22222FF : 0x111111FF);
    KzPostavi(playerid, RTD_BROJ, tekst);
}

static RulPocni(playerid)
{
    new tekst[64], Float:y = KzPanel(playerid, 200.0, 150.0);
    KzTekst(playerid, RTD_BROJ, KZ_SREDINA, y + 30.0, "_", 0xFFFFFFFF, 0.6, 2.6, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gKzTd[playerid][RTD_BROJ], true);
    PlayerTextDrawTextSize(playerid, gKzTd[playerid][RTD_BROJ], 30.0, 56.0);
    KzTekst(playerid, RTD_OPIS, KZ_SREDINA, y + 66.0, "_", 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_ALIGN_CENTER);
    if (gRulTip[playerid] == 9) format(tekst, sizeof tekst, T(playerid, "Broj %d"), gRulBroj[playerid]);
    else strcat(tekst, T(playerid, gRulTipovi[gRulTip[playerid]]));
    KzPostavi(playerid, RTD_OPIS, tekst);
    KzPostavi(playerid, KZI_LIJEVO, "_", false);
    KzPostavi(playerid, KZI_DESNO, "_", false);
    gRulIshod[playerid] = random(37);
    RulCrtaj(playerid, random(37));
    KzDiler(playerid, "dealone");
    KzVrti(playerid);
}

static RulKraj(playerid)
{
    new n = gRulIshod[playerid], isplata = gKzUlog[playerid] * RulDobitak(gRulTip[playerid], gRulBroj[playerid], n), ishod[48];
    if (gKzOtvoren[playerid]) RulCrtaj(playerid, n);
    if (!n) valstr(ishod, n);
    else format(ishod, sizeof ishod, "%d %s", n, T(playerid, RulCrveni(n) ? ("crveno") : ("crno")));
    KzKraj(playerid, ishod, gKzUlog[playerid], isplata);
}

static KockeCrtaj(playerid, bool:vrti)
{
    new tekst[64];
    if (vrti) for (new i = 0; i < 4; i++) gKocke[playerid][i] = 1 + random(6);
    format(tekst, sizeof tekst, T(playerid, "Vi: %d + %d = %d"), gKocke[playerid][0], gKocke[playerid][1], gKocke[playerid][0] + gKocke[playerid][1]);
    KzPostavi(playerid, DTD_IGRAC, tekst);
    format(tekst, sizeof tekst, T(playerid, "Diler: %d + %d = %d"), gKocke[playerid][2], gKocke[playerid][3], gKocke[playerid][2] + gKocke[playerid][3]);
    KzPostavi(playerid, DTD_DILER, tekst);
}

static KockePocni(playerid)
{
    new Float:y = KzPanel(playerid, 200.0, 140.0);
    KzTekst(playerid, DTD_DILER, KZ_SREDINA, y + 30.0, "_", 0xFFFFFFFF, 0.3, 1.4, TEXT_DRAW_ALIGN_CENTER);
    KzTekst(playerid, DTD_IGRAC, KZ_SREDINA, y + 54.0, "_", 0xE8C15AFF, 0.3, 1.4, TEXT_DRAW_ALIGN_CENTER);
    KzPostavi(playerid, KZI_LIJEVO, "_", false);
    KzPostavi(playerid, KZI_DESNO, "_", false);
    KockeCrtaj(playerid, true);
    KzDiler(playerid, "dealone");
    KzVrti(playerid);
}

static KockeKraj(playerid)
{
    new ig, dl, isplata = 0;
    if (gKzOtvoren[playerid]) KockeCrtaj(playerid, false);
    ig = gKocke[playerid][0] + gKocke[playerid][1];
    dl = gKocke[playerid][2] + gKocke[playerid][3];
    if (ig > dl) isplata = gKzUlog[playerid] * 2;
    KzKraj(playerid, ig > dl ? ("Pobijedili ste!") : ig == dl ? ("Nerijeseno, kuca dobiva.") : ("Diler pobjeduje."), gKzUlog[playerid], isplata);
}

static SlotSimbol()
{
    new r = random(100), s = 0;
    while (r >= gSlotTezina[s]) r -= gSlotTezina[s++];
    return s;
}

static SlotCrtaj(playerid, korak)
{
    for (new i = 0; i < 3; i++)
    {
        new stani = KZ_KORACI - (2 - i) * 4;
        KzPostavi(playerid, STD_VALJAK + i, gSlotSprite[korak >= stani ? gSlot[playerid][i] : random(sizeof gSlotSprite)]);
    }
}

static SlotPocni(playerid)
{
    new Float:x = KZ_SREDINA - 100.0, Float:y = KzPanel(playerid, 200.0, 146.0);
    for (new i = 0; i < 3; i++)
    {
        gSlot[playerid][i] = SlotSimbol();
        KzSprite(playerid, STD_OKVIR + i, x + 20.0 + float(i) * 56.0, y + 26.0, 54.0, 54.0, "LD_SPAC:white", 0x111111FF);
        PlayerTextDrawShow(playerid, gKzTd[playerid][STD_OKVIR + i]);
    }
    for (new i = 0; i < 3; i++) KzSprite(playerid, STD_VALJAK + i, x + 22.0 + float(i) * 56.0, y + 28.0, 50.0, 50.0, gSlotSprite[0]);
    KzPostavi(playerid, KZI_LIJEVO, "_", false);
    KzPostavi(playerid, KZI_DESNO, "_", false);
    SlotCrtaj(playerid, 0);
    KzVrti(playerid);
}

static SlotKraj(playerid)
{
    new a = gSlot[playerid][0], b = gSlot[playerid][1], c = gSlot[playerid][2], puta = 0;
    if (gKzOtvoren[playerid]) SlotCrtaj(playerid, KZ_KORACI);
    if (a == b && b == c) puta = gSlotIsplata[a];
    else if (!a && !b) puta = 3;
    KzKraj(playerid, puta >= 40 ? ("Jackpot!") : puta ? ("Pobijedili ste!") : ("Bez dobitka."), gKzUlog[playerid], gKzUlog[playerid] * puta);
}

ShowKazino(playerid)
{
    new list[1024], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova igra na mojoj poziciji"));
    for (new k = 0; k < KAZINO_MAX; k++)
    {
        if (!gKaz[k][kzIma]) continue;
        format(row, sizeof row, T(playerid, "[%d] %s - %s (int %d, svijet %d)"), gKaz[k][kzDb], T(playerid, gKzIme[gKaz[k][kzTip]]), CestaZona(gKaz[k][kzX], gKaz[k][kzY]), gKaz[k][kzInt], gKaz[k][kzSvijet]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_KAZINO, DIALOG_STYLE_LIST, T(playerid, "Kazino"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static KazinoNovi(playerid, tip)
{
    new k = 0;
    while (k < KAZINO_MAX && gKaz[k][kzIma]) k++;
    if (k == KAZINO_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj dilera."));
    GetPlayerPos(playerid, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ]);
    GetPlayerFacingAngle(playerid, gKaz[k][kzA]);
    gKaz[k][kzInt] = GetPlayerInterior(playerid);
    gKaz[k][kzSvijet] = GetPlayerVirtualWorld(playerid);
    gKaz[k][kzTip] = tip;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO casino_dealers (x, y, z, a, interior, world, igra) VALUES (%f, %f, %f, %f, %d, %d, %d)", gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ], gKaz[k][kzA], gKaz[k][kzInt], gKaz[k][kzSvijet], tip));
    gKaz[k][kzDb] = ZadnjiId();
    KazinoStvori(k);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Igra postavljena: %s."), T(playerid, gKzIme[tip]));
}

DLG:apanel_kazino(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new list[256], len = 0;
        for (new i = 0; i < sizeof gKzIme; i++) DodajStupac(list, len, T(playerid, gKzIme[i]));
        return PrikaziDialog(playerid, DLG_APANEL_KAZINO_TIP, DIALOG_STYLE_LIST, T(playerid, "Kazino"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    new n = listitem;
    for (new k = 0; k < KAZINO_MAX; k++)
    {
        if (!gKaz[k][kzIma] || --n) continue;
        gApKazino[playerid] = k;
        return PrikaziDialog(playerid, DLG_APANEL_KAZINO_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Kazino"), T(playerid, "Obrisati ovu igru?"), T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowKazino(playerid);
}

DLG:apanel_kazino_tip(playerid, response, listitem)
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (response && listitem >= 0 && listitem < sizeof gKzIme) KazinoNovi(playerid, listitem);
    return ShowKazino(playerid);
}

DLG:apanel_kazino_brisi(playerid, response)
{
    new k = gApKazino[playerid];
    if (!response || !gKaz[k][kzIma]) return ShowKazino(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (gKaz[k][kzAktor] != INVALID_ACTOR_ID) DestroyActor(gKaz[k][kzAktor]);
    DestroyDynamic3DTextLabel(gKaz[k][kzTekst]);
    gKaz[k][kzIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM casino_dealers WHERE id = %d", gKaz[k][kzDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Igra obrisana."));
    return ShowKazino(playerid);
}
