#define KAZINO_MAX          32
#define KAZINO_DOMET        2.5
#define KAZINO_SKIN         171
#define KAZINO_SKIN_BLAGAJNA 172
#define KAZINO_MIN_ULOG     1
#define KAZINO_MAX_ULOG     5000
#define KZ_TD               80
#define KZ_VRTI_MS          110
#define KZ_KORACI           14
#define KZ_SREDINA          320.0
#define RUL_POLJA           49
#define BJ_KARATA           8
#define BJ_RUKE             4
#define BJ_W                214.0
#define BJ_RED              58.0
#define BJ_RAZMAK           18.0

enum { KZ_BLACKJACK, KZ_RULET, KZ_POKER, KZ_KOCKE, KZ_SLOT, KZ_BLAGAJNA }
enum { KZI_POZADINA, KZI_NASLOV, KZI_STATUS, KZI_LIJEVO, KZI_DESNO, KZI_PRVI }
enum { BTD_DILER = KZI_PRVI, BTD_DOUBLE, BTD_SPLIT, BTD_SURRENDER, BTD_RUKA, BTD_KARTA = BTD_RUKA + BJ_RUKE }
enum { VTD_KARTA = KZI_PRVI, VTD_DRZI = VTD_KARTA + 5 }
enum { RTD_BROJ = KZI_PRVI, RTD_PONISTI, RTD_ZETON, RTD_POLJE = RTD_ZETON + 16 }
enum { UTD_PONISTI = KZI_PRVI, UTD_ZETON }
enum { DTD_IGRAC = KZI_PRVI, DTD_DILER }
enum { STD_OKVIR = KZI_PRVI, STD_VALJAK = STD_OKVIR + 3 }

enum E_KAZINO { bool:kzIma, kzDb, kzTip, Float:kzX, Float:kzY, Float:kzZ, Float:kzA, kzInt, kzSvijet, kzAktor, STREAMER_TAG_3D_TEXT_LABEL:kzTekst }
enum E_BJ { bjRuke, bjAktivna, bjDilerKarte[BJ_KARATA], bjDilerN, bool:bjOsiguranje, bjOsigUlog, bool:bjPrvi, bool:bjPredao, bool:bjAsovi }

static gKaz[KAZINO_MAX][E_KAZINO];
static gApKazino[MAX_PLAYERS], gKzBlagajna[MAX_PLAYERS], bool:gKzBira[MAX_PLAYERS], gKzNovi[MAX_PLAYERS];
static PlayerText:gKzTd[MAX_PLAYERS][KZ_TD], gKzTdN[MAX_PLAYERS], bool:gKzOtvoren[MAX_PLAYERS], bool:gKzIgra[MAX_PLAYERS], gKzStol[MAX_PLAYERS], gKzTip[MAX_PLAYERS], gKzUlog[MAX_PLAYERS], gKzRunda[MAX_PLAYERS];
static gBj[MAX_PLAYERS][E_BJ], gBjKarte[MAX_PLAYERS][BJ_RUKE][BJ_KARATA], gBjN[MAX_PLAYERS][BJ_RUKE], gBjUlog[MAX_PLAYERS][BJ_RUKE];
static gVpKarte[MAX_PLAYERS][5], bool:gVpDrzi[MAX_PLAYERS][5], bool:gVpKoristeno[MAX_PLAYERS][52];
static gRulIshod[MAX_PLAYERS], gRulZeton[MAX_PLAYERS], gRulUlog[MAX_PLAYERS][RUL_POLJA];
static gKocke[MAX_PLAYERS][4], gSlot[MAX_PLAYERS][3];

static const gKzIme[][] = { "Blackjack", "Rulet", "Video poker", "Kocke", "Slot", "Blagajna" };
static const gBjBoja[] = "cdhs";
static const gKzZetoni[] = { 1, 5, 10, 25, 100, 500, 1000, 5000 };
static const gKzZetonSprite[][] = { "LD_ROUL:roulred", "LD_ROUL:roulred", "LD_ROUL:roulred", "LD_ROUL:roulgre", "LD_ROUL:roulgre", "LD_ROUL:roulbla", "LD_ROUL:roulbla", "LD_ROUL:roulbla" };
static const gRulVani[][] = { "1-12", "13-24", "25-36", "2:1", "2:1", "2:1", "1-18", "Par", "Crveno", "Crno", "Nepar", "19-36" };
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
    assert(RulDobitak(45, 1) == 2 && RulDobitak(45, 2) == 0 && RulDobitak(39, 36) == 3 && RulDobitak(17, 17) == 36 && RulDobitak(44, 0) == 0 && RulDobitak(0, 0) == 36 && RulDobitak(40, 4) == 3 && RulDobitak(42, 36) == 3 && RulDobitak(41, 36) == 0);
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
        gKaz[k][kzAktor] = CreateActor(gKaz[k][kzTip] == KZ_BLAGAJNA ? KAZINO_SKIN_BLAGAJNA : KAZINO_SKIN, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ], gKaz[k][kzA]);
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
    if (isplata && !GiveItem(playerid, ITEM_ZETONI, isplata)) DajNovac(playerid, isplata);
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
    if (gKzIgra[playerid] || gKzBira[playerid] || (gKzOtvoren[playerid] && gKzTip[playerid] == KZ_RULET)) return 1;
    gKzStol[playerid] = k;
    gKzTip[playerid] = gKaz[k][kzTip];
    if (gKzTip[playerid] == KZ_BLAGAJNA)
    {
        AktorKaze(playerid, gKaz[k][kzAktor], "Blagajnica", "Izvolite, zetoni ili isplata?");
        new list[128], len = 0;
        DodajStupac(list, len, T(playerid, "Kupi zetone"));
        DodajStupac(list, len, T(playerid, "Zamijeni zetone za novac"));
        return PrikaziDialog(playerid, DLG_KAZINO_BLAGAJNA, DIALOG_STYLE_LIST, T(playerid, "Blagajna"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
    }
    if (!gInv[playerid][ITEM_ZETONI]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate zetona. Kupite ih na blagajni kazina."));
    if (gKaz[k][kzAktor] != INVALID_ACTOR_ID) AktorKaze(playerid, gKaz[k][kzAktor], "Krupije", "Dobrodosli za stol, koliko ulazete?");
    if (gKzTip[playerid] == KZ_RULET) return RulOtvori(playerid);
    return KzUlogOtvori(playerid);
}

DLG:kazino_blagajna(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem > 1 || !KazinoBlizu(playerid, gKzStol[playerid])) return 1;
    new body[128];
    gKzBlagajna[playerid] = listitem;
    if (listitem) format(body, sizeof body, T(playerid, "Imate %d zetona. Koliko zamjenjujete za novac?"), gInv[playerid][ITEM_ZETONI]);
    else format(body, sizeof body, T(playerid, "Imate $%d. Koliko zetona kupujete? (1 zeton = $1)"), gNovac[playerid]);
    return PrikaziDialog(playerid, DLG_KAZINO_BLAGAJNA_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Blagajna"), body, T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

DLG:kazino_blagajna_unos(playerid, response, const inputtext[])
{
    if (!KazinoBlizu(playerid, gKzStol[playerid])) return 1;
    if (!response) return KazinoKey(playerid);
    new n = ProvjeriBroj(inputtext) && strlen(inputtext) < 9 ? strval(inputtext) : 0;
    if (n <= 0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Iznos mora biti veci od 0."));
    if (gKzBlagajna[playerid])
    {
        if (gInv[playerid][ITEM_ZETONI] < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno zetona."));
        if (!DajNovac(playerid, n)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        GiveItem(playerid, ITEM_ZETONI, -n);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zamijenili ste %d zetona za $%d."), n, n);
    }
    if (gNovac[playerid] < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    if (!GiveItem(playerid, ITEM_ZETONI, n)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
    DajNovac(playerid, -n);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d zetona za $%d."), n, n);
}

static bool:KzPokreni(playerid, ulog)
{
    if (ulog < KAZINO_MIN_ULOG || ulog > KAZINO_MAX_ULOG)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulog mora biti izmedu $%d i $%d."), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
        return false;
    }
    if (gInv[playerid][ITEM_ZETONI] < ulog)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno zetona."));
        return false;
    }
    GiveItem(playerid, ITEM_ZETONI, -ulog);
    gKzBira[playerid] = false;
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
        case KZ_KOCKE: KockePocni(playerid);
        case KZ_SLOT: SlotPocni(playerid);
    }
    return true;
}

static KzSlobodno(playerid, ulozeno)
{
    return (gInv[playerid][ITEM_ZETONI] < KAZINO_MAX_ULOG ? gInv[playerid][ITEM_ZETONI] : KAZINO_MAX_ULOG) - ulozeno;
}

static KzZetoniNapravi(playerid, prvi, Float:x, Float:y)
{
    new tekst[8];
    for (new i = 0; i < sizeof gKzZetoni; i++)
    {
        new Float:cx = x + float(i) * 40.0;
        KzSprite(playerid, prvi + i, cx - 13.0, y, 26.0, 26.0, gKzZetonSprite[i]);
        PlayerTextDrawSetSelectable(playerid, gKzTd[playerid][prvi + i], true);
        format(tekst, sizeof tekst, "$%d", gKzZetoni[i]);
        KzTekst(playerid, prvi + sizeof gKzZetoni + i, cx, y + 8.0, tekst, 0xFFFFFFFF, 0.16, 0.75, TEXT_DRAW_ALIGN_CENTER);
    }
}

static KzZetoniSakrij(playerid, prvi)
{
    for (new i = 0; i < sizeof gKzZetoni * 2; i++) PlayerTextDrawHide(playerid, gKzTd[playerid][prvi + i]);
}

static KzZetoniPrikazi(playerid, prvi, slobodno, odabran = -1)
{
    for (new i = 0; i < sizeof gKzZetoni; i++)
    {
        new PlayerText:td = gKzTd[playerid][prvi + i], PlayerText:tekst = gKzTd[playerid][prvi + sizeof gKzZetoni + i];
        if (gKzZetoni[i] > slobodno)
        {
            PlayerTextDrawHide(playerid, td);
            PlayerTextDrawHide(playerid, tekst);
            continue;
        }
        PlayerTextDrawColour(playerid, td, i == odabran ? 0xFFD54FFF : 0xFFFFFFFF);
        PlayerTextDrawColour(playerid, tekst, i == odabran ? 0xFFD54FFF : 0xFFFFFFFF);
        PlayerTextDrawShow(playerid, td);
        PlayerTextDrawShow(playerid, tekst);
    }
}

static KzUlogCrtaj(playerid)
{
    new tekst[64];
    KzZetoniPrikazi(playerid, UTD_ZETON, KzSlobodno(playerid, gKzNovi[playerid]));
    format(tekst, sizeof tekst, T(playerid, "Zetoni: %d~n~Ulog: $%d"), gInv[playerid][ITEM_ZETONI] - gKzNovi[playerid], gKzNovi[playerid]);
    KzPostavi(playerid, KZI_STATUS, tekst);
}

static KzUlogOtvori(playerid)
{
    KzUnisti(playerid);
    if (!gKzOtvoren[playerid])
    {
        gKzOtvoren[playerid] = true;
        SelectTextDraw(playerid, 0xE8C15AFF);
    }
    gKzBira[playerid] = true;
    gKzNovi[playerid] = 0;
    new Float:y = KzPanel(playerid, 340.0, 120.0);
    KzGumb(playerid, UTD_PONISTI, KZ_SREDINA, y + 82.0, 60.0, 0x555555FF);
    KzZetoniNapravi(playerid, UTD_ZETON, KZ_SREDINA - 140.0, y + 34.0);
    KzPostavi(playerid, KZI_LIJEVO, T(playerid, "Igraj"));
    KzPostavi(playerid, KZI_DESNO, T(playerid, "Zatvori"));
    KzPostavi(playerid, UTD_PONISTI, T(playerid, "Ponisti"));
    KzUlogCrtaj(playerid);
    return 1;
}

static KzUlogKlik(playerid, PlayerText:td)
{
    if (td == gKzTd[playerid][KZI_DESNO]) KazinoZatvori(playerid);
    else if (td == gKzTd[playerid][KZI_LIJEVO])
    {
        if (!KazinoBlizu(playerid, gKzStol[playerid])) KazinoZatvori(playerid);
        else if (gKzNovi[playerid] < KAZINO_MIN_ULOG) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Odaberite zetone za ulog."));
        else KzPokreni(playerid, gKzNovi[playerid]);
    }
    else if (td == gKzTd[playerid][UTD_PONISTI])
    {
        gKzNovi[playerid] = 0;
        KzUlogCrtaj(playerid);
    }
    else
    {
        for (new i = 0; i < sizeof gKzZetoni; i++)
        {
            if (td != gKzTd[playerid][UTD_ZETON + i]) continue;
            if (gKzZetoni[i] <= KzSlobodno(playerid, gKzNovi[playerid])) gKzNovi[playerid] += gKzZetoni[i];
            KzUlogCrtaj(playerid);
            return 1;
        }
        return 0;
    }
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
    gKzBira[playerid] = false;
    KzUnisti(playerid);
    CancelSelectTextDraw(playerid);
}

KazinoDisconnect(playerid)
{
    gKzOtvoren[playerid] = false;
    gKzBira[playerid] = false;
    gKzIgra[playerid] = false;
    gKzTdN[playerid] = 0;
}

KazinoKlik(playerid, PlayerText:td)
{
    if (!gKzOtvoren[playerid]) return 0;
    if (gKzTip[playerid] == KZ_RULET) return RulKlik(playerid, td);
    if (gKzBira[playerid]) return KzUlogKlik(playerid, td);
    if (!gKzIgra[playerid])
    {
        if (td == gKzTd[playerid][KZI_DESNO]) KazinoZatvori(playerid);
        else if (td == gKzTd[playerid][KZI_LIJEVO])
        {
            if (!KazinoBlizu(playerid, gKzStol[playerid])) KazinoZatvori(playerid);
            else if (!KzPokreni(playerid, gKzUlog[playerid])) KzUlogOtvori(playerid);
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
    return !gBj[playerid][bjOsiguranje] && gBj[playerid][bjRuke] < BJ_RUKE && gBjN[playerid][h] == 2 && gBjKarte[playerid][h][0] % 13 == gBjKarte[playerid][h][1] % 13 && gInv[playerid][ITEM_ZETONI] >= gBjUlog[playerid][h];
}

static bool:BjMozeDouble(playerid)
{
    new h = gBj[playerid][bjAktivna];
    return !gBj[playerid][bjOsiguranje] && gBjN[playerid][h] == 2 && gInv[playerid][ITEM_ZETONI] >= gBjUlog[playerid][h];
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
            if (gInv[playerid][ITEM_ZETONI] < cijena)
            {
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno zetona."));
                return 1;
            }
            GiveItem(playerid, ITEM_ZETONI, -cijena);
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
        GiveItem(playerid, ITEM_ZETONI, -gBjUlog[playerid][h]);
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
        GiveItem(playerid, ITEM_ZETONI, -gBjUlog[playerid][h]);
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

static RulDobitak(polje, n)
{
    if (polje <= 36) return polje == n ? 36 : 0;
    if (!n) return 0;
    if (polje <= 39) return (n - 1) / 12 == polje - 37 ? 3 : 0;
    if (polje <= 42) return n % 3 == (polje - 39) % 3 ? 3 : 0;
    switch (polje)
    {
        case 43: return n <= 18 ? 2 : 0;
        case 44: return n % 2 ? 0 : 2;
        case 45: return RulCrveni(n) ? 2 : 0;
        case 46: return RulCrveni(n) ? 0 : 2;
        case 47: return n % 2 ? 2 : 0;
    }
    return n > 18 ? 2 : 0;
}

static RulBoja(polje)
{
    if (!polje) return 0x1B7F3AFF;
    if (polje <= 36) return RulCrveni(polje) ? 0xB22222FF : 0x161616FF;
    if (polje == 45) return 0xB22222FF;
    if (polje == 46) return 0x161616FF;
    return 0x1B5E35FF;
}

static RulUkupno(playerid)
{
    new ukupno = 0;
    for (new i = 0; i < RUL_POLJA; i++) ukupno += gRulUlog[playerid][i];
    return ukupno;
}

static RulOsvjezi(playerid, polje)
{
    new PlayerText:td = gKzTd[playerid][RTD_POLJE + polje], bool:ima = gRulUlog[playerid][polje] > 0;
    PlayerTextDrawBoxColour(playerid, td, ima ? 0xE8C15AFF : RulBoja(polje));
    PlayerTextDrawColour(playerid, td, ima ? 0x000000FF : 0xFFFFFFFF);
    PlayerTextDrawShow(playerid, td);
}

static RulZetoni(playerid)
{
    new slobodno = KzSlobodno(playerid, RulUkupno(playerid));
    while (gRulZeton[playerid] > 0 && gKzZetoni[gRulZeton[playerid]] > slobodno) gRulZeton[playerid]--;
    KzZetoniPrikazi(playerid, RTD_ZETON, slobodno, gRulZeton[playerid]);
}

static RulStatus(playerid, polje)
{
    new tekst[128], ime[24], ostalo = gInv[playerid][ITEM_ZETONI] - (gKzIgra[playerid] ? 0 : RulUkupno(playerid));
    RulZetoni(playerid);
    if (polje < 0)
    {
        format(tekst, sizeof tekst, T(playerid, "Zetoni: %d~n~Postavite zetone i kliknite Spin."), ostalo);
        KzPostavi(playerid, KZI_STATUS, tekst);
        return;
    }
    if (polje <= 36) valstr(ime, polje);
    else strcat(ime, T(playerid, gRulVani[polje - 37]));
    format(tekst, sizeof tekst, T(playerid, "Zetoni: %d, ukupni ulog: $%d~n~%s: $%d"), ostalo, RulUkupno(playerid), ime, gRulUlog[playerid][polje]);
    KzPostavi(playerid, KZI_STATUS, tekst);
}

static RulPolje(playerid, polje, Float:x, Float:y, Float:w, const tekst[])
{
    KzGumb(playerid, RTD_POLJE + polje, x, y, w, RulBoja(polje));
    PlayerTextDrawSetString(playerid, gKzTd[playerid][RTD_POLJE + polje], tekst);
    RulOsvjezi(playerid, polje);
}

static RulOtvori(playerid)
{
    new tekst[16];
    KzUnisti(playerid);
    if (!gKzOtvoren[playerid])
    {
        gKzOtvoren[playerid] = true;
        SelectTextDraw(playerid, 0xE8C15AFF);
    }
    for (new i = 0; i < RUL_POLJA; i++) gRulUlog[playerid][i] = 0;
    new Float:x = KZ_SREDINA - 190.0, Float:y = KzPanel(playerid, 380.0, 200.0);
    KzTekst(playerid, RTD_BROJ, x + 354.0, y + 112.0, "_", 0xFFFFFFFF, 0.4, 1.8, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gKzTd[playerid][RTD_BROJ], true);
    PlayerTextDrawTextSize(playerid, gKzTd[playerid][RTD_BROJ], 20.0, 36.0);
    KzGumb(playerid, RTD_PONISTI, KZ_SREDINA, y + 162.0, 60.0, 0x555555FF);
    KzZetoniNapravi(playerid, RTD_ZETON, x + 30.0, y + 114.0);
    RulPolje(playerid, 0, x + 24.0, y + 30.0, 30.0, "_~n~0~n~_");
    PlayerTextDrawLetterSize(playerid, gKzTd[playerid][RTD_POLJE], 0.2, 1.55);
    PlayerTextDrawShow(playerid, gKzTd[playerid][RTD_POLJE]);
    for (new n = 1; n <= 36; n++)
    {
        valstr(tekst, n);
        RulPolje(playerid, n, x + 54.0 + float((n - 1) / 3) * 24.0, y + 30.0 + float(2 - (n - 1) % 3) * 17.0, 22.0, tekst);
    }
    for (new i = 0; i < 3; i++)
    {
        RulPolje(playerid, 37 + i, x + 54.0 + (float(i) * 4.0 + 1.5) * 24.0, y + 83.0, 94.0, gRulVani[i]);
        RulPolje(playerid, 40 + i, x + 348.0, y + 30.0 + float(2 - i) * 17.0, 34.0, gRulVani[3 + i]);
    }
    for (new i = 0; i < 6; i++) RulPolje(playerid, 43 + i, x + 54.0 + (float(i) * 2.0 + 0.5) * 24.0, y + 101.0, 46.0, T(playerid, gRulVani[6 + i]));
    RulStatus(playerid, -1);
    KzPostavi(playerid, KZI_LIJEVO, "Spin");
    KzPostavi(playerid, KZI_DESNO, T(playerid, "Zatvori"));
    KzPostavi(playerid, RTD_PONISTI, T(playerid, "Ponisti"));
    return 1;
}

static RulCrtaj(playerid, n)
{
    new tekst[8];
    valstr(tekst, n);
    PlayerTextDrawBoxColour(playerid, gKzTd[playerid][RTD_BROJ], RulBoja(n));
    KzPostavi(playerid, RTD_BROJ, tekst);
}

static RulVrti(playerid)
{
    new ukupno = RulUkupno(playerid);
    if (!KazinoBlizu(playerid, gKzStol[playerid]))
    {
        KazinoZatvori(playerid);
        return;
    }
    if (ukupno < KAZINO_MIN_ULOG)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulog mora biti izmedu $%d i $%d."), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
        return;
    }
    if (gInv[playerid][ITEM_ZETONI] < ukupno)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno zetona."));
        return;
    }
    GiveItem(playerid, ITEM_ZETONI, -ukupno);
    gKzUlog[playerid] = ukupno;
    gKzIgra[playerid] = true;
    gKzRunda[playerid]++;
    gRulIshod[playerid] = random(37);
    KzPostavi(playerid, KZI_LIJEVO, "_", false);
    KzPostavi(playerid, KZI_DESNO, "_", false);
    KzPostavi(playerid, RTD_PONISTI, "_", false);
    KzZetoniSakrij(playerid, RTD_ZETON);
    KzDiler(playerid, "dealone");
    KzVrti(playerid);
}

static RulKraj(playerid)
{
    new n = gRulIshod[playerid], isplata = 0, ishod[48];
    for (new i = 0; i < RUL_POLJA; i++) isplata += gRulUlog[playerid][i] * RulDobitak(i, n);
    if (gKzOtvoren[playerid]) RulCrtaj(playerid, n);
    if (!n) valstr(ishod, n);
    else format(ishod, sizeof ishod, "%d %s", n, T(playerid, RulCrveni(n) ? ("crveno") : ("crno")));
    KzKraj(playerid, ishod, gKzUlog[playerid], isplata);
    if (!gKzOtvoren[playerid]) return;
    KzPostavi(playerid, KZI_LIJEVO, "Spin");
    KzPostavi(playerid, RTD_PONISTI, T(playerid, "Ponisti"));
    RulZetoni(playerid);
}

static RulKlik(playerid, PlayerText:td)
{
    if (gKzIgra[playerid]) return 1;
    if (td == gKzTd[playerid][KZI_DESNO]) KazinoZatvori(playerid);
    else if (td == gKzTd[playerid][KZI_LIJEVO]) RulVrti(playerid);
    else if (td == gKzTd[playerid][RTD_PONISTI])
    {
        for (new i = 0; i < RUL_POLJA; i++)
        {
            if (!gRulUlog[playerid][i]) continue;
            gRulUlog[playerid][i] = 0;
            RulOsvjezi(playerid, i);
        }
        RulStatus(playerid, -1);
    }
    else
    {
        for (new i = 0; i < sizeof gKzZetoni; i++)
        {
            if (td != gKzTd[playerid][RTD_ZETON + i]) continue;
            gRulZeton[playerid] = i;
            RulZetoni(playerid);
            return 1;
        }
        for (new p = 0; p < RUL_POLJA; p++)
        {
            if (td != gKzTd[playerid][RTD_POLJE + p]) continue;
            if (gKzZetoni[gRulZeton[playerid]] > KzSlobodno(playerid, RulUkupno(playerid)))
            {
                if (RulUkupno(playerid) + gKzZetoni[gRulZeton[playerid]] > KAZINO_MAX_ULOG) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najveci ukupni ulog je $%d."), KAZINO_MAX_ULOG);
                else SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno zetona."));
                return 1;
            }
            gRulUlog[playerid][p] += gKzZetoni[gRulZeton[playerid]];
            RulOsvjezi(playerid, p);
            RulStatus(playerid, p);
            return 1;
        }
        return 0;
    }
    return 1;
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
