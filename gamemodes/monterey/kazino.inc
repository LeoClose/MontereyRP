#define KAZINO_MAX          32
#define KAZINO_DOMET        2.5
#define KAZINO_SKIN         171
#define KAZINO_MIN_ULOG     10
#define KAZINO_MAX_ULOG     5000
#define BJ_KARATA           12

enum E_KAZINO { bool:kzIma, kzDb, Float:kzX, Float:kzY, Float:kzZ, Float:kzA, kzInt, kzSvijet, kzAktor, STREAMER_TAG_3D_TEXT_LABEL:kzTekst }
enum E_BJ { bjUlog, bjDiler, bjIgrac[BJ_KARATA], bjIgracN, bjDilerKarte[BJ_KARATA], bjDilerN }

static gKaz[KAZINO_MAX][E_KAZINO];
static gBj[MAX_PLAYERS][E_BJ];
static gApKazino[MAX_PLAYERS];
static const gBjKarta[][] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

InitKazino()
{
    new k1[] = { 1, 13 }, k2[] = { 1, 1, 9 }, k3[] = { 10, 6, 7 };
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
        if (karte[i] == 1) asovi++;
        zbroj += karte[i] == 1 ? 11 : karte[i] > 10 ? 10 : karte[i];
    }
    while (zbroj > 21 && asovi-- > 0) zbroj -= 10;
    return zbroj;
}

static BjRuka(const karte[], n, str[], size = sizeof (str))
{
    str[0] = EOS;
    for (new i = 0; i < n; i++)
    {
        if (i) strcat(str, " ", size);
        strcat(str, gBjKarta[karte[i]], size);
    }
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

static BjPrikazi(playerid)
{
    new body[256], diler[48], igrac[48];
    BjRuka(gBj[playerid][bjDilerKarte], 1, diler);
    BjRuka(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN], igrac);
    format(body, sizeof body, T(playerid, "Diler: %s ?\nVi: %s (%d)\nUlog: $%d"), diler, igrac, BjVrijednost(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN]), gBj[playerid][bjUlog]);
    PrikaziDialog(playerid, DLG_KAZINO, DIALOG_STYLE_MSGBOX, "Blackjack", body, T(playerid, "Karta"), T(playerid, "Stani"));
}

static BjKraj(playerid)
{
    new ulog = gBj[playerid][bjUlog], ig = BjVrijednost(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN]), bool:prirodni = ig == 21 && gBj[playerid][bjIgracN] == 2;
    gBj[playerid][bjUlog] = 0;
    if (ig <= 21 && !prirodni)
        while (BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]) < 17 && gBj[playerid][bjDilerN] < BJ_KARATA)
            gBj[playerid][bjDilerKarte][gBj[playerid][bjDilerN]++] = 1 + random(13);
    new dl = BjVrijednost(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN]), bool:dilerPrirodni = dl == 21 && gBj[playerid][bjDilerN] == 2, isplata = 0;
    new body[320], diler[48], igrac[48], ishod[64];
    if (ig > 21) ishod = "Prebacili ste 21. Gubite ulog.";
    else if (prirodni && !dilerPrirodni) { isplata = ulog * 5 / 2; ishod = "Blackjack! Isplata 3:2."; }
    else if (dilerPrirodni && !prirodni) ishod = "Diler ima blackjack. Gubite ulog.";
    else if (dl > 21 || ig > dl) { isplata = ulog * 2; ishod = "Pobijedili ste!"; }
    else if (ig == dl) { isplata = ulog; ishod = "Nerijeseno, ulog vam se vraca."; }
    else ishod = "Diler pobjeduje.";
    if (isplata) DajNovac(playerid, isplata);
    BjDiler(playerid, "CASINO", isplata > ulog ? ("cards_lose") : ("cards_win"));
    BjRuka(gBj[playerid][bjDilerKarte], gBj[playerid][bjDilerN], diler);
    BjRuka(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN], igrac);
    format(body, sizeof body, T(playerid, "Diler: %s (%d)\nVi: %s (%d)\n\n%s\nUlog: $%d, isplata: $%d"), diler, dl, igrac, ig, T(playerid, ishod), ulog, isplata);
    PrikaziDialog(playerid, DLG_KAZINO_KRAJ, DIALOG_STYLE_MSGBOX, "Blackjack", body, T(playerid, "Ponovno"), T(playerid, "Zatvori"));
}

DLG:kazino_ulog(playerid, response, const inputtext[])
{
    if (!response || gBj[playerid][bjUlog] || !KazinoBlizu(playerid, gBj[playerid][bjDiler])) return 1;
    new ulog = ProvjeriBroj(inputtext) && strlen(inputtext) < 9 ? strval(inputtext) : 0;
    if (ulog < KAZINO_MIN_ULOG || ulog > KAZINO_MAX_ULOG) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulog mora biti izmedu $%d i $%d."), KAZINO_MIN_ULOG, KAZINO_MAX_ULOG);
    if (gNovac[playerid] < ulog) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    DajNovac(playerid, -ulog);
    gBj[playerid][bjUlog] = ulog;
    gBj[playerid][bjIgracN] = 2;
    gBj[playerid][bjDilerN] = 2;
    for (new i = 0; i < 2; i++)
    {
        gBj[playerid][bjIgrac][i] = 1 + random(13);
        gBj[playerid][bjDilerKarte][i] = 1 + random(13);
    }
    BjDiler(playerid, "CASINO", "dealone");
    if (BjVrijednost(gBj[playerid][bjIgrac], 2) == 21) BjKraj(playerid);
    else BjPrikazi(playerid);
    return 1;
}

DLG:kazino(playerid, response)
{
    if (!gBj[playerid][bjUlog]) return 1;
    if (response && gBj[playerid][bjIgracN] < BJ_KARATA)
    {
        gBj[playerid][bjIgrac][gBj[playerid][bjIgracN]++] = 1 + random(13);
        BjDiler(playerid, "CASINO", "dealone");
        if (BjVrijednost(gBj[playerid][bjIgrac], gBj[playerid][bjIgracN]) < 21)
        {
            BjPrikazi(playerid);
            return 1;
        }
    }
    BjKraj(playerid);
    return 1;
}

DLG:kazino_kraj(playerid, response)
{
    if (response) KazinoKey(playerid);
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
