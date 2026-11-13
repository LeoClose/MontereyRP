#define KAZINO_MAX          32
#define KAZINO_DOMET        2.5
#define KAZINO_SKIN         171
#define KAZINO_MIN_ULOG     10
#define KAZINO_MAX_ULOG     5000
#define BJ_KARATA           8
#define BJ_W                190.0
#define BJ_X                225.0
#define BJ_Y                120.0
#define BJ_RED              60.0
#define BJ_RAZMAK           18.0

enum E_KAZINO { bool:kzIma, kzDb, Float:kzX, Float:kzY, Float:kzZ, Float:kzA, kzInt, kzSvijet, kzAktor, STREAMER_TAG_3D_TEXT_LABEL:kzTekst }
enum E_BJ { bool:bjIgra, bjUlog, bjDiler, bjRuke, bjAktivna, bjDilerKarte[BJ_KARATA], bjDilerN }
enum { BTD_POZADINA, BTD_NASLOV, BTD_DILER, BTD_STATUS, BTD_HIT, BTD_SPLIT, BTD_STAND, BTD_RUKA, BTD_KARTA = BTD_RUKA + 2 }

static gKaz[KAZINO_MAX][E_KAZINO];
static gBj[MAX_PLAYERS][E_BJ];
static gApKazino[MAX_PLAYERS];
static PlayerText:gBjTd[MAX_PLAYERS][BTD_KARTA + BJ_KARATA * 3], bool:gBjOtvoren[MAX_PLAYERS];
static gBjKarte[MAX_PLAYERS][2][BJ_KARATA], gBjN[MAX_PLAYERS][2];
static const gBjBoja[] = "cdhs";

InitKazino()
{
    new k1[] = { 0, 25 }, k2[] = { 13, 39, 8 }, k3[] = { 9, 18, 45 };
    assert(BjVrijednost(k1, 2) == 21 && BjVrijednost(k2, 3) == 21 && BjVrijednost(k3, 3) == 23);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS casino_dealers (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM casino_dealers ORDER BY id LIMIT %d", KAZINO_MAX);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++)
    {
        gKaz[k][kzDb] = DB_GetFieldInt(res, 0);
        gKaz[k][kzX] = DB_GetFieldFloat(res, 1);
        gKaz[k][kzY] = DB_GetFieldFloat(res, 2);
        gKaz[k][kzZ] = DB_GetFieldFloat(res, 3);
        gKaz[k][kzA] = DB_GetFieldFloat(res, 4);
        gKaz[k][kzInt] = DB_GetFieldInt(res, 5);
        gKaz[k][kzSvijet] = DB_GetFieldInt(res, 6);
        KazinoStvori(k);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static KazinoStvori(k)
{
    gKaz[k][kzIma] = true;
    gKaz[k][kzAktor] = CreateActor(KAZINO_SKIN, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ], gKaz[k][kzA]);
    SetActorVirtualWorld(gKaz[k][kzAktor], gKaz[k][kzSvijet]);
    SetActorInvulnerable(gKaz[k][kzAktor], true);
    gKaz[k][kzTekst] = CreateDynamic3DTextLabel("Blackjack\n[Y]", COLOR_ORG, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ] + 1.2, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gKaz[k][kzSvijet], gKaz[k][kzInt]);
}

static bool:KazinoBlizu(playerid, k)
{
    return k >= 0 && k < KAZINO_MAX && gKaz[k][kzIma] && GetPlayerVirtualWorld(playerid) == gKaz[k][kzSvijet] && GetPlayerInterior(playerid) == gKaz[k][kzInt] && IsPlayerInRangeOfPoint(playerid, KAZINO_DOMET, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ]);
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

static BjDiler(playerid, const lib[], const anim[])
{
    new k = gBj[playerid][bjDiler];
    if (k >= 0 && k < KAZINO_MAX && gKaz[k][kzIma]) ApplyActorAnimation(gKaz[k][kzAktor], lib, anim, 4.1, false, false, false, false, 0);
}

KazinoKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new k = 0;
    while (k < KAZINO_MAX && !KazinoBlizu(playerid, k)) k++;
    if (k == KAZINO_MAX) return 0;
    if (gBj[playerid][bjIgra]) return 1;
    new body[96];
    gBj[playerid][bjDiler] = k;
    format(body, sizeof body, T(playerid, "Unesite ulog ($%d - $%d):"), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
    AktorKaze(playerid, gKaz[k][kzAktor], "Krupije", "Dobrodosli za stol, koliko ulazete?");
    PrikaziDialog(playerid, DLG_KAZINO_ULOG, DIALOG_STYLE_INPUT, "Blackjack", body, T(playerid, "Igraj"), T(playerid, "Zatvori"));
    return 1;
}

static PlayerText:BjTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawBackgroundColour(playerid, td, 0x000000AA);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    return td;
}

static PlayerText:BjSprite(playerid, Float:x, Float:y, Float:w, Float:h, const sprite[], boja = 0xFFFFFFFF)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, sprite);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    return td;
}

static PlayerText:BjGumb(playerid, Float:x, Float:y, boja)
{
    new PlayerText:td = BjTekst(playerid, x, y, "_", 0xFFFFFFFF, 0.22, 1.1, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 12.0, 52.0);
    PlayerTextDrawSetSelectable(playerid, td, true);
    return td;
}

static BjNapravi(playerid)
{
    new Float:s = gBj[playerid][bjRuke] == 2 ? BJ_RED : 0.0;
    gBjTd[playerid][BTD_POZADINA] = BjSprite(playerid, BJ_X, BJ_Y, BJ_W, 184.0 + s, "LD_SPAC:white", 0x0F4D2AEE);
    gBjTd[playerid][BTD_NASLOV] = BjTekst(playerid, BJ_X + BJ_W / 2.0, BJ_Y + 4.0, "BLACKJACK", 0xE8C15AFF, 0.4, 1.6, TEXT_DRAW_ALIGN_CENTER);
    gBjTd[playerid][BTD_DILER] = BjTekst(playerid, BJ_X + 12.0, BJ_Y + 26.0, "_", 0xFFFFFFFF, 0.2, 0.9);
    gBjTd[playerid][BTD_STATUS] = BjTekst(playerid, BJ_X + BJ_W / 2.0, BJ_Y + 146.0 + s, "_", 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_ALIGN_CENTER);
    gBjTd[playerid][BTD_HIT] = BjGumb(playerid, BJ_X + 34.0, BJ_Y + 166.0 + s, 0x1E5AA8FF);
    gBjTd[playerid][BTD_SPLIT] = BjGumb(playerid, BJ_X + BJ_W / 2.0, BJ_Y + 166.0 + s, 0x8A6D1EFF);
    gBjTd[playerid][BTD_STAND] = BjGumb(playerid, BJ_X + BJ_W - 34.0, BJ_Y + 166.0 + s, 0x8B2E2EFF);
    for (new h = 0; h < 2; h++) gBjTd[playerid][BTD_RUKA + h] = BjTekst(playerid, BJ_X + 12.0, BJ_Y + 86.0 + float(h) * BJ_RED, "_", 0xFFFFFFFF, 0.2, 0.9);
    for (new r = 0; r < 3; r++)
        for (new i = 0; i < BJ_KARATA; i++)
            gBjTd[playerid][BTD_KARTA + r * BJ_KARATA + i] = BjSprite(playerid, BJ_X + 12.0 + float(i) * BJ_RAZMAK, BJ_Y + (r ? 98.0 + float(r - 1) * BJ_RED : 38.0), 32.0, 46.0, "LD_CARD:cdback");
    for (new i = BTD_POZADINA; i <= BTD_STAND; i++) if (i != BTD_SPLIT) PlayerTextDrawShow(playerid, gBjTd[playerid][i]);
    PlayerTextDrawShow(playerid, gBjTd[playerid][BTD_RUKA]);
    if (s > 0.0) PlayerTextDrawShow(playerid, gBjTd[playerid][BTD_RUKA + 1]);
}

static BjUnisti(playerid)
{
    for (new i = 0; i < sizeof gBjTd[]; i++) PlayerTextDrawDestroy(playerid, gBjTd[playerid][i]);
}

static BjOtvori(playerid)
{
    if (gBjOtvoren[playerid]) return;
    gBjOtvoren[playerid] = true;
    BjNapravi(playerid);
    SelectTextDraw(playerid, 0xE8C15AFF);
}

KazinoZatvori(playerid)
{
    if (!gBjOtvoren[playerid]) return;
    if (gBj[playerid][bjIgra]) BjKraj(playerid);
    gBjOtvoren[playerid] = false;
    BjUnisti(playerid);
    CancelSelectTextDraw(playerid);
}

KazinoDisconnect(playerid)
{
    gBjOtvoren[playerid] = false;
    gBj[playerid][bjIgra] = false;
}

static BjRed(playerid, red, const karte[], n, skrivena)
{
    new sprite[16];
    for (new i = 0; i < BJ_KARATA; i++)
    {
        new PlayerText:td = gBjTd[playerid][BTD_KARTA + red * BJ_KARATA + i];
        if (i >= n)
        {
            PlayerTextDrawHide(playerid, td);
            continue;
        }
        if (i == skrivena) sprite = "LD_CARD:cdback";
        else format(sprite, sizeof sprite, "LD_CARD:cd%d%c", karte[i] % 13 + 1, gBjBoja[karte[i] / 13]);
        PlayerTextDrawSetString(playerid, td, sprite);
        PlayerTextDrawShow(playerid, td);
    }
}

static bool:BjMozeSplit(playerid)
{
    return gBj[playerid][bjRuke] == 1 && gBjN[playerid][0] == 2 && gBjKarte[playerid][0][0] % 13 == gBjKarte[playerid][0][1] % 13 && gNovac[playerid] >= gBj[playerid][bjUlog];
}

static BjCrtaj(playerid, bool:kraj)
{
    new tekst[64], ruke = gBj[playerid][bjRuke];
    BjRed(playerid, 0, gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN], kraj ? -1 : 1);
    format(tekst, sizeof tekst, T(playerid, "Diler: %d"), BjVrijednost(gBj[playerid][bjDilerKarte], kraj ? gBj[playerid][bjDilerN] : 1));
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_DILER], tekst);
    for (new h = 0; h < ruke; h++)
    {
        new PlayerText:td = gBjTd[playerid][BTD_RUKA + h], v = BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]);
        BjRed(playerid, h + 1, gBjKarte[playerid][h], gBjN[playerid][h], -1);
        if (ruke == 1) format(tekst, sizeof tekst, T(playerid, "Vi: %d"), v);
        else format(tekst, sizeof tekst, T(playerid, "Ruka %d: %d"), h + 1, v);
        PlayerTextDrawColour(playerid, td, !kraj && ruke == 2 && h == gBj[playerid][bjAktivna] ? 0xE8C15AFF : 0xFFFFFFFF);
        PlayerTextDrawSetString(playerid, td, tekst);
        PlayerTextDrawShow(playerid, td);
    }
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_HIT], kraj ? T(playerid, "Ponovno") : ("Hit"));
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_STAND], kraj ? T(playerid, "Zatvori") : ("Stand"));
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_SPLIT], "Split");
    if (!kraj && BjMozeSplit(playerid)) PlayerTextDrawShow(playerid, gBjTd[playerid][BTD_SPLIT]);
    else PlayerTextDrawHide(playerid, gBjTd[playerid][BTD_SPLIT]);
    if (kraj) return;
    format(tekst, sizeof tekst, T(playerid, "Ulog: $%d"), gBj[playerid][bjUlog] * ruke);
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_STATUS], tekst);
}

static BjKraj(playerid)
{
    new ulog = gBj[playerid][bjUlog], ruke = gBj[playerid][bjRuke], bool:prirodni = ruke == 1 && gBjN[playerid][0] == 2 && BjVrijednost(gBjKarte[playerid][0], 2) == 21, bool:vuci = false;
    gBj[playerid][bjIgra] = false;
    for (new h = 0; h < ruke; h++) if (BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]) <= 21) vuci = true;
    if (vuci && !prirodni)
        while (BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]) < 17 && gBj[playerid][bjDilerN] < BJ_KARATA)
            gBj[playerid][bjDilerKarte][gBj[playerid][bjDilerN]++] = random(52);
    new dl = BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]), bool:dilerPrirodni = dl == 21 && gBj[playerid][bjDilerN] == 2, ukupno = 0;
    new tekst[128], ishod[64];
    for (new h = 0; h < ruke; h++)
    {
        new ig = BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]), isplata = 0;
        if (ig > 21) ishod = "Prebacili ste 21. Gubite ulog.";
        else if (prirodni && !dilerPrirodni) { isplata = ulog * 5 / 2; ishod = "Blackjack! Isplata 3:2."; }
        else if (dilerPrirodni && !prirodni) ishod = "Diler ima blackjack. Gubite ulog.";
        else if (dl > 21 || ig > dl) { isplata = ulog * 2; ishod = "Pobijedili ste!"; }
        else if (ig == dl) { isplata = ulog; ishod = "Nerijeseno, ulog vam se vraca."; }
        else ishod = "Diler pobjeduje.";
        ukupno += isplata;
        format(tekst, sizeof tekst, T(playerid, "%s Ulog: $%d, isplata: $%d"), T(playerid, ishod), ulog, isplata);
        if (ruke == 1) SendClientMessage(playerid, isplata > ulog ? COLOR_INFO : COLOR_ERROR, "Blackjack: %s", tekst);
        else SendClientMessage(playerid, isplata > ulog ? COLOR_INFO : COLOR_ERROR, "Blackjack (%d/2): %s", h + 1, tekst);
    }
    if (ukupno) DajNovac(playerid, ukupno);
    BjDiler(playerid, "CASINO", ukupno > ulog * ruke ? ("cards_lose") : ("cards_win"));
    if (!gBjOtvoren[playerid]) return;
    BjCrtaj(playerid, true);
    format(tekst, sizeof tekst, T(playerid, "%s~n~Ulog: $%d, isplata: $%d"), ruke == 1 ? T(playerid, ishod) : ("Split"), ulog * ruke, ukupno);
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_STATUS], tekst);
}

static bool:BjDijeli(playerid, ulog)
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
    new bool:bio = gBj[playerid][bjRuke] == 2;
    gBj[playerid][bjIgra] = true;
    gBj[playerid][bjUlog] = ulog;
    gBj[playerid][bjRuke] = 1;
    gBj[playerid][bjAktivna] = 0;
    gBj[playerid][bjDilerN] = 2;
    gBjN[playerid][0] = 2;
    gBjN[playerid][1] = 0;
    for (new i = 0; i < 2; i++)
    {
        gBjKarte[playerid][0][i] = random(52);
        gBj[playerid][bjDilerKarte][i] = random(52);
    }
    BjDiler(playerid, "CASINO", "dealone");
    if (gBjOtvoren[playerid] && bio)
    {
        BjUnisti(playerid);
        BjNapravi(playerid);
    }
    BjOtvori(playerid);
    BjCrtaj(playerid, false);
    if (BjVrijednost(gBjKarte[playerid][0], 2) == 21) BjKraj(playerid);
    return true;
}

DLG:kazino_ulog(playerid, response, const inputtext[])
{
    if (gBj[playerid][bjIgra]) return 1;
    if (!response || !KazinoBlizu(playerid, gBj[playerid][bjDiler]))
    {
        KazinoZatvori(playerid);
        return 1;
    }
    BjDijeli(playerid, ProvjeriBroj(inputtext) && strlen(inputtext) < 9 ? strval(inputtext) : 0);
    return 1;
}

static BjStani(playerid)
{
    if (gBj[playerid][bjRuke] == 2 && gBj[playerid][bjAktivna] == 0)
    {
        gBj[playerid][bjAktivna] = 1;
        BjCrtaj(playerid, false);
        if (BjVrijednost(gBjKarte[playerid][1], gBjN[playerid][1]) == 21) BjStani(playerid);
        return;
    }
    BjKraj(playerid);
}

KazinoKlik(playerid, PlayerText:td)
{
    if (!gBjOtvoren[playerid]) return 0;
    new h = gBj[playerid][bjAktivna];
    if (td == gBjTd[playerid][BTD_HIT])
    {
        if (!gBj[playerid][bjIgra])
        {
            if (!KazinoBlizu(playerid, gBj[playerid][bjDiler])) KazinoZatvori(playerid);
            else if (!BjDijeli(playerid, gBj[playerid][bjUlog])) KazinoKey(playerid);
            return 1;
        }
        gBjKarte[playerid][h][gBjN[playerid][h]++] = random(52);
        BjDiler(playerid, "CASINO", "dealone");
        BjCrtaj(playerid, false);
        if (BjVrijednost(gBjKarte[playerid][h], gBjN[playerid][h]) >= 21 || gBjN[playerid][h] == BJ_KARATA) BjStani(playerid);
    }
    else if (td == gBjTd[playerid][BTD_STAND])
    {
        if (gBj[playerid][bjIgra]) BjStani(playerid);
        else KazinoZatvori(playerid);
    }
    else if (td == gBjTd[playerid][BTD_SPLIT])
    {
        if (!gBj[playerid][bjIgra] || !BjMozeSplit(playerid)) return 1;
        DajNovac(playerid, -gBj[playerid][bjUlog]);
        gBj[playerid][bjRuke] = 2;
        gBjKarte[playerid][1][0] = gBjKarte[playerid][0][1];
        gBjKarte[playerid][0][1] = random(52);
        gBjKarte[playerid][1][1] = random(52);
        gBjN[playerid][1] = 2;
        BjDiler(playerid, "CASINO", "dealone");
        BjUnisti(playerid);
        BjNapravi(playerid);
        BjCrtaj(playerid, false);
        if (BjVrijednost(gBjKarte[playerid][0], 2) == 21) BjStani(playerid);
    }
    else return 0;
    return 1;
}

ShowKazino(playerid)
{
    new list[1024], row[80], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Novi blackjack diler na mojoj poziciji"));
    for (new k = 0; k < KAZINO_MAX; k++)
    {
        if (!gKaz[k][kzIma]) continue;
        format(row, sizeof row, T(playerid, "[%d] %s (int %d, svijet %d)"), gKaz[k][kzDb], CestaZona(gKaz[k][kzX], gKaz[k][kzY]), gKaz[k][kzInt], gKaz[k][kzSvijet]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_KAZINO, DIALOG_STYLE_LIST, T(playerid, "Kazino"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static KazinoNovi(playerid)
{
    new k = 0;
    while (k < KAZINO_MAX && gKaz[k][kzIma]) k++;
    if (k == KAZINO_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj dilera."));
    GetPlayerPos(playerid, gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ]);
    GetPlayerFacingAngle(playerid, gKaz[k][kzA]);
    gKaz[k][kzInt] = GetPlayerInterior(playerid);
    gKaz[k][kzSvijet] = GetPlayerVirtualWorld(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO casino_dealers (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", gKaz[k][kzX], gKaz[k][kzY], gKaz[k][kzZ], gKaz[k][kzA], gKaz[k][kzInt], gKaz[k][kzSvijet]));
    gKaz[k][kzDb] = ZadnjiId();
    KazinoStvori(k);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Blackjack diler postavljen."));
}

DLG:apanel_kazino(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        KazinoNovi(playerid);
        return ShowKazino(playerid);
    }
    new n = listitem;
    for (new k = 0; k < KAZINO_MAX; k++)
    {
        if (!gKaz[k][kzIma] || --n) continue;
        gApKazino[playerid] = k;
        return PrikaziDialog(playerid, DLG_APANEL_KAZINO_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Kazino"), T(playerid, "Obrisati ovog dilera?"), T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowKazino(playerid);
}

DLG:apanel_kazino_brisi(playerid, response)
{
    new k = gApKazino[playerid];
    if (!response || !gKaz[k][kzIma]) return ShowKazino(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    DestroyActor(gKaz[k][kzAktor]);
    DestroyDynamic3DTextLabel(gKaz[k][kzTekst]);
    gKaz[k][kzIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM casino_dealers WHERE id = %d", gKaz[k][kzDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Diler obrisan."));
    return ShowKazino(playerid);
}
