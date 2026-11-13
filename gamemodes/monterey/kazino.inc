#define KAZINO_MAX          32
#define KAZINO_DOMET        2.5
#define KAZINO_SKIN         171
#define KAZINO_MIN_ULOG     10
#define KAZINO_MAX_ULOG     5000
#define BJ_KARATA           8
#define BJ_X                160.0
#define BJ_Y                110.0

enum E_KAZINO { bool:kzIma, kzDb, Float:kzX, Float:kzY, Float:kzZ, Float:kzA, kzInt, kzSvijet, kzAktor, STREAMER_TAG_3D_TEXT_LABEL:kzTekst }
enum E_BJ { bjUlog, bjDiler, bjIgrac[BJ_KARATA], bjIgracN, bjDilerKarte[BJ_KARATA], bjDilerN }
enum { BTD_POZADINA, BTD_NASLOV, BTD_DILER, BTD_IGRAC, BTD_STATUS, BTD_LIJEVO, BTD_DESNO, BTD_KARTA }

static gKaz[KAZINO_MAX][E_KAZINO];
static gBj[MAX_PLAYERS][E_BJ];
static gApKazino[MAX_PLAYERS];
static PlayerText:gBjTd[MAX_PLAYERS][BTD_KARTA + BJ_KARATA * 2], bool:gBjOtvoren[MAX_PLAYERS];
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
    new body[96];
    gBj[playerid][bjUlog] = 0;
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

static PlayerText:BjGumb(playerid, Float:x, boja)
{
    new PlayerText:td = BjTekst(playerid, x, BJ_Y + 214.0, "_", 0xFFFFFFFF, 0.25, 1.2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 14.0, 100.0);
    PlayerTextDrawSetSelectable(playerid, td, true);
    return td;
}

static BjOtvori(playerid)
{
    if (gBjOtvoren[playerid]) return;
    gBjOtvoren[playerid] = true;
    gBjTd[playerid][BTD_POZADINA] = BjSprite(playerid, BJ_X, BJ_Y, 320.0, 240.0, "LD_SPAC:white", 0x0F4D2AEE);
    gBjTd[playerid][BTD_NASLOV] = BjTekst(playerid, BJ_X + 160.0, BJ_Y + 4.0, "BLACKJACK", 0xE8C15AFF, 0.45, 1.8, TEXT_DRAW_ALIGN_CENTER);
    gBjTd[playerid][BTD_DILER] = BjTekst(playerid, BJ_X + 16.0, BJ_Y + 30.0, "_", 0xFFFFFFFF, 0.22, 1.0);
    gBjTd[playerid][BTD_IGRAC] = BjTekst(playerid, BJ_X + 16.0, BJ_Y + 104.0, "_", 0xFFFFFFFF, 0.22, 1.0);
    gBjTd[playerid][BTD_STATUS] = BjTekst(playerid, BJ_X + 160.0, BJ_Y + 182.0, "_", 0xFFFFFFFF, 0.2, 0.95, TEXT_DRAW_ALIGN_CENTER);
    gBjTd[playerid][BTD_LIJEVO] = BjGumb(playerid, BJ_X + 100.0, 0x1E5AA8FF);
    gBjTd[playerid][BTD_DESNO] = BjGumb(playerid, BJ_X + 220.0, 0x8B2E2EFF);
    for (new i = 0; i < BJ_KARATA; i++)
    {
        gBjTd[playerid][BTD_KARTA + i] = BjSprite(playerid, BJ_X + 16.0 + float(i) * 36.0, BJ_Y + 44.0, 34.0, 50.0, "LD_CARD:cdback");
        gBjTd[playerid][BTD_KARTA + BJ_KARATA + i] = BjSprite(playerid, BJ_X + 16.0 + float(i) * 36.0, BJ_Y + 118.0, 34.0, 50.0, "LD_CARD:cdback");
    }
    for (new i = 0; i < BTD_KARTA; i++) PlayerTextDrawShow(playerid, gBjTd[playerid][i]);
    SelectTextDraw(playerid, 0xE8C15AFF);
}

KazinoZatvori(playerid)
{
    if (!gBjOtvoren[playerid]) return;
    if (gBj[playerid][bjUlog]) BjKraj(playerid);
    gBjOtvoren[playerid] = false;
    for (new i = 0; i < sizeof gBjTd[]; i++) PlayerTextDrawDestroy(playerid, gBjTd[playerid][i]);
    CancelSelectTextDraw(playerid);
}

KazinoDisconnect(playerid)
{
    gBjOtvoren[playerid] = false;
    gBj[playerid][bjUlog] = 0;
}

static BjRed(playerid, prva, const karte[], n, skrivena)
{
    new sprite[16];
    for (new i = 0; i < BJ_KARATA; i++)
    {
        new PlayerText:td = gBjTd[playerid][prva + i];
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

static BjCrtaj(playerid, bool:kraj)
{
    new tekst[64];
    BjRed(playerid, BTD_KARTA, gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN], kraj ? -1 : 1);
    BjRed(playerid, BTD_KARTA + BJ_KARATA, gBj[playerid][bjIgrac], gBj[playerid][bjIgracN], -1);
    format(tekst, sizeof tekst, T(playerid, "Diler: %d"), BjVrijednost(gBj[playerid][bjDilerKarte], kraj ? gBj[playerid][bjDilerN] : 1));
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_DILER], tekst);
    format(tekst, sizeof tekst, T(playerid, "Vi: %d"), BjVrijednost(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN]));
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_IGRAC], tekst);
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_LIJEVO], T(playerid, kraj ? ("Ponovno") : ("Karta")));
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_DESNO], T(playerid, kraj ? ("Zatvori") : ("Stani")));
    if (kraj) return;
    format(tekst, sizeof tekst, T(playerid, "Ulog: $%d"), gBj[playerid][bjUlog]);
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_STATUS], tekst);
}

static BjKraj(playerid)
{
    new ulog = gBj[playerid][bjUlog], ig = BjVrijednost(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN]), bool:prirodni = ig == 21 && gBj[playerid][bjIgracN] == 2;
    gBj[playerid][bjUlog] = 0;
    if (ig <= 21 && !prirodni)
        while (BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]) < 17 && gBj[playerid][bjDilerN] < BJ_KARATA)
            gBj[playerid][bjDilerKarte][gBj[playerid][bjDilerN]++] = random(52);
    new dl = BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]), bool:dilerPrirodni = dl == 21 && gBj[playerid][bjDilerN] == 2, isplata = 0;
    new tekst[128], ishod[64];
    if (ig > 21) ishod = "Prebacili ste 21. Gubite ulog.";
    else if (prirodni && !dilerPrirodni) { isplata = ulog * 5 / 2; ishod = "Blackjack! Isplata 3:2."; }
    else if (dilerPrirodni && !prirodni) ishod = "Diler ima blackjack. Gubite ulog.";
    else if (dl > 21 || ig > dl) { isplata = ulog * 2; ishod = "Pobijedili ste!"; }
    else if (ig == dl) { isplata = ulog; ishod = "Nerijeseno, ulog vam se vraca."; }
    else ishod = "Diler pobjeduje.";
    if (isplata) DajNovac(playerid, isplata);
    BjDiler(playerid, "CASINO", isplata > ulog ? ("cards_lose") : ("cards_win"));
    format(tekst, sizeof tekst, T(playerid, "%s Ulog: $%d, isplata: $%d"), T(playerid, ishod), ulog, isplata);
    SendClientMessage(playerid, isplata > ulog ? COLOR_INFO : COLOR_ERROR, "Blackjack: %s", tekst);
    if (!gBjOtvoren[playerid]) return;
    BjCrtaj(playerid, true);
    PlayerTextDrawSetString(playerid, gBjTd[playerid][BTD_STATUS], tekst);
}

DLG:kazino_ulog(playerid, response, const inputtext[])
{
    if (!response || gBj[playerid][bjUlog] || !KazinoBlizu(playerid, gBj[playerid][bjDiler]))
    {
        KazinoZatvori(playerid);
        return 1;
    }
    new ulog = ProvjeriBroj(inputtext) && strlen(inputtext) < 9 ? strval(inputtext) : 0;
    if (ulog < KAZINO_MIN_ULOG || ulog > KAZINO_MAX_ULOG) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulog mora biti izmedu $%d i $%d."), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
    if (gNovac[playerid] < ulog) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    DajNovac(playerid, -ulog);
    gBj[playerid][bjUlog] = ulog;
    gBj[playerid][bjIgracN] = 2;
    gBj[playerid][bjDilerN] = 2;
    for (new i = 0; i < 2; i++)
    {
        gBj[playerid][bjIgrac][i] = random(52);
        gBj[playerid][bjDilerKarte][i] = random(52);
    }
    BjDiler(playerid, "CASINO", "dealone");
    BjOtvori(playerid);
    BjCrtaj(playerid, false);
    if (BjVrijednost(gBj[playerid][bjIgrac], 2) == 21) BjKraj(playerid);
    return 1;
}

KazinoKlik(playerid, PlayerText:td)
{
    if (!gBjOtvoren[playerid]) return 0;
    if (td == gBjTd[playerid][BTD_LIJEVO])
    {
        if (!gBj[playerid][bjUlog])
        {
            if (!KazinoKey(playerid)) KazinoZatvori(playerid);
            return 1;
        }
        gBj[playerid][bjIgrac][gBj[playerid][bjIgracN]++] = random(52);
        BjDiler(playerid, "CASINO", "dealone");
        BjCrtaj(playerid, false);
        if (BjVrijednost(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN]) >= 21 || gBj[playerid][bjIgracN] == BJ_KARATA) BjKraj(playerid);
    }
    else if (td == gBjTd[playerid][BTD_DESNO])
    {
        if (gBj[playerid][bjUlog]) BjKraj(playerid);
        else KazinoZatvori(playerid);
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
