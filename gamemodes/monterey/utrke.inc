#define UTRKA_MAX_PO_IGRACU 10
#define UTRKA_TOCAKA        20
#define UTRKA_AKTIVNIH      10
#define UTRKA_VOZACA        12
#define UTRKA_IME           25
#define UTRKA_CP            8.0
#define UTRKA_UKLONI_DOMET  6.0
#define UTRKA_ODBROJ        5
#define UTRKA_LOBBY_S       300
#define UTRKA_TRAJE_S       1200
#define UTRKA_NAKON_PRVOG_S 120
#define UTRKA_OBJAVA_MS     3000
#define UTRKA_MAX_ULOG      100000
#define UTRKA_BOJA          0xE67E22E6
#define UTRKA_MARKER        1559

enum { UV_NE, UV_NA_START, UV_GRID, UV_VOZI, UV_CILJ }
enum { UK_NEMA, UK_LOBBY, UK_ODBROJ, UK_VOZI }
enum E_UTRKA { ukStanje, ukDomacin, ukStvoreno, ukPot, ukIme[UTRKA_IME], ukBroj, ukOdbroj, ukRok, ukPrvi, ukPlasman, ukGrid, ukObjava }

static gUk[UTRKA_AKTIVNIH][E_UTRKA], Float:gUkTocke[UTRKA_AKTIVNIH][UTRKA_TOCAKA][3];
static gUv[MAX_PLAYERS], gUvStanje[MAX_PLAYERS], gUvCp[MAX_PLAYERS], gUvUlog[MAX_PLAYERS], gUvMjesto[MAX_PLAYERS], STREAMER_TAG_MAP_ICON:gUvIkona[MAX_PLAYERS];
static bool:gUrIma[MAX_PLAYERS], bool:gUrIme[MAX_PLAYERS], gUrBroj[MAX_PLAYERS], Float:gUrTocke[MAX_PLAYERS][UTRKA_TOCAKA][3], STREAMER_TAG_OBJECT:gUrObjekt[MAX_PLAYERS][UTRKA_TOCAKA];
static gUlBroj[MAX_PLAYERS], gUlId[MAX_PLAYERS][UTRKA_MAX_PO_IGRACU], gUlIme[MAX_PLAYERS][UTRKA_MAX_PO_IGRACU][UTRKA_IME];

InitUtrke()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS races (id INTEGER PRIMARY KEY AUTOINCREMENT, owner INTEGER NOT NULL, name TEXT NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS race_points (race INTEGER NOT NULL, idx INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, PRIMARY KEY (race, idx))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS race_stats (account INTEGER PRIMARY KEY, won INTEGER NOT NULL DEFAULT 0, lost INTEGER NOT NULL DEFAULT 0, earned INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS races_owner ON races (owner)"));
}

static UtrkaPozicija(playerid, &Float:x, &Float:y, &Float:z)
{
    new v = GetPlayerVehicleID(playerid);
    if (v) GetVehiclePos(v, x, y, z);
    else GetPlayerPos(playerid, x, y, z);
}

UtrkeUcitaj(playerid)
{
    gUlBroj[playerid] = 0;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, name FROM races WHERE owner = %d ORDER BY id LIMIT %d", gAccountId[playerid], UTRKA_MAX_PO_IGRACU);
    for (new n = DB_GetRowCount(res); gUlBroj[playerid] < n; DB_SelectNextRow(res))
    {
        new i = gUlBroj[playerid]++;
        gUlId[playerid][i] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, gUlIme[playerid][i], UTRKA_IME);
    }
    DB_FreeResultSet(res);
    return gUlBroj[playerid];
}

UtrkeBroj(playerid)
{
    return gUlBroj[playerid];
}

UtrkeIme(playerid, i, ime[], len)
{
    ime[0] = EOS;
    if (i >= 0 && i < gUlBroj[playerid]) strcat(ime, gUlIme[playerid][i], len);
}

UtrkeStatistika(playerid, tekst[], len)
{
    new won = 0, lost = 0, earned = 0, DBResult:res = DB_ExecuteQuery(gDB, "SELECT won, lost, earned FROM race_stats WHERE account = %d", gAccountId[playerid]);
    if (DB_GetRowCount(res))
    {
        won = DB_GetFieldInt(res, 0);
        lost = DB_GetFieldInt(res, 1);
        earned = DB_GetFieldInt(res, 2);
    }
    DB_FreeResultSet(res);
    format(tekst, len, T(playerid, "Pobjede: %d~n~Porazi: %d~n~Zarada: $%d"), won, lost, earned);
}

UtrkaObrisi(playerid, i)
{
    if (i < 0 || i >= gUlBroj[playerid]) return;
    new id = gUlId[playerid][i];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM race_points WHERE race = %d", id));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM races WHERE id = %d AND owner = %d", id, gAccountId[playerid]));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Utrka %s je obrisana."), gUlIme[playerid][i]);
    UtrkeUcitaj(playerid);
}

static UtrkaUrediPanel(playerid)
{
    new tekst[256];
    if (gUrIme[playerid]) format(tekst, sizeof tekst, T(playerid, "Upisite ime utrke u chat.~n~~y~N~w~ - odustani"));
    else format(tekst, sizeof tekst, T(playerid, "~y~ALT~w~ - postavi / ukloni marker~n~~y~Y~w~ - cilj i ime utrke~n~~y~N~w~ - odustani~n~Markera: %d/%d"), gUrBroj[playerid], UTRKA_TOCAKA);
    IgraObavijestPanel(playerid, T(playerid, "Nova utrka"), UTRKA_BOJA, tekst);
}

static UtrkaUrediKraj(playerid)
{
    for (new i = 0; i < gUrBroj[playerid]; i++) DestroyDynamicObject(gUrObjekt[playerid][i]);
    if (gUrIma[playerid]) IgraObavijestUkloni(playerid);
    gUrIma[playerid] = false;
    gUrIme[playerid] = false;
    gUrBroj[playerid] = 0;
}

UtrkaNapravi(playerid)
{
    if (gUrIma[playerid] || gUv[playerid]) return ObavijestGreska(playerid, T(playerid, "Vec uredujete ili vozite utrku."));
    if (UtrkeUcitaj(playerid) >= UTRKA_MAX_PO_IGRACU) return ObavijestGreska(playerid, T(playerid, "Mozete imati najvise 10 utrka."));
    MobitelZatvori(playerid);
    gUrIma[playerid] = true;
    gUrIme[playerid] = false;
    gUrBroj[playerid] = 0;
    UtrkaUrediPanel(playerid);
    return 1;
}

static UtrkaDodajTocku(playerid, Float:x, Float:y, Float:z)
{
    new i = gUrBroj[playerid]++;
    gUrTocke[playerid][i][0] = x;
    gUrTocke[playerid][i][1] = y;
    gUrTocke[playerid][i][2] = z;
    gUrObjekt[playerid][i] = CreateDynamicObject(UTRKA_MARKER, x, y, z + 1.0, 0.0, 0.0, 0.0, -1, -1, playerid);
}

static UtrkaUrediAlt(playerid)
{
    new Float:x, Float:y, Float:z;
    UtrkaPozicija(playerid, x, y, z);
    for (new i = 0; i < gUrBroj[playerid]; i++)
    {
        if (VectorSize(x - gUrTocke[playerid][i][0], y - gUrTocke[playerid][i][1], z - gUrTocke[playerid][i][2]) > UTRKA_UKLONI_DOMET) continue;
        DestroyDynamicObject(gUrObjekt[playerid][i]);
        for (new j = i + 1; j < gUrBroj[playerid]; j++)
        {
            gUrObjekt[playerid][j - 1] = gUrObjekt[playerid][j];
            for (new k = 0; k < 3; k++) gUrTocke[playerid][j - 1][k] = gUrTocke[playerid][j][k];
        }
        gUrBroj[playerid]--;
        UtrkaUrediPanel(playerid);
        return;
    }
    if (gUrBroj[playerid] >= UTRKA_TOCAKA - 1)
    {
        ObavijestGreska(playerid, T(playerid, "Dostignut je najveci broj markera, postavite cilj tipkom Y."));
        return;
    }
    UtrkaDodajTocku(playerid, x, y, z);
    UtrkaUrediPanel(playerid);
}

static UtrkaUrediCilj(playerid)
{
    if (gUrBroj[playerid] < 1)
    {
        ObavijestGreska(playerid, T(playerid, "Prvo postavite start tipkom ALT."));
        return;
    }
    new Float:x, Float:y, Float:z;
    UtrkaPozicija(playerid, x, y, z);
    UtrkaDodajTocku(playerid, x, y, z);
    gUrIme[playerid] = true;
    UtrkaUrediPanel(playerid);
}

UtrkaChat(playerid, const text[])
{
    if (!gUrIme[playerid]) return 0;
    new len = strlen(text);
    if (len < 1 || len >= UTRKA_IME || strfind(text, "~") != -1)
    {
        ObavijestGreska(playerid, T(playerid, "Ime utrke mora imati 1-24 znaka i bez znaka ~."));
        return 1;
    }
    if (UtrkeUcitaj(playerid) >= UTRKA_MAX_PO_IGRACU)
    {
        ObavijestGreska(playerid, T(playerid, "Mozete imati najvise 10 utrka."));
        UtrkaUrediKraj(playerid);
        return 1;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO races (owner, name) VALUES (%d, '%q')", gAccountId[playerid], text));
    new id = ZadnjiId();
    for (new i = 0; i < gUrBroj[playerid]; i++)
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO race_points (race, idx, x, y, z) VALUES (%d, %d, %f, %f, %f)", id, i, gUrTocke[playerid][i][0], gUrTocke[playerid][i][1], gUrTocke[playerid][i][2]));
    UtrkaUrediKraj(playerid);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Utrka %s je spremljena."), text);
    return 1;
}

static UtrkaVozaci(u, &naStartu = 0)
{
    new n = 0;
    naStartu = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        n++;
        if (gUvStanje[i] == UV_GRID) naStartu++;
    }
    return n;
}

static UtrkaStats(playerid, bool:pobjeda, zarada)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO race_stats (account) VALUES (%d)", gAccountId[playerid]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE race_stats SET won = won + %d, lost = lost + %d, earned = earned + %d WHERE account = %d", pobjeda ? 1 : 0, pobjeda ? 0 : 1, zarada, gAccountId[playerid]));
}

static UtrkaIkonaMakni(playerid)
{
    if (!gUvIkona[playerid]) return;
    DestroyDynamicMapIcon(gUvIkona[playerid]);
    gUvIkona[playerid] = STREAMER_TAG_MAP_ICON:0;
}

static UtrkaIzbaci(playerid)
{
    new u = gUv[playerid] - 1;
    if (u < 0) return;
    if (gUvStanje[playerid] == UV_GRID) TogglePlayerControllable(playerid, true);
    DisablePlayerRaceCheckpoint(playerid);
    UtrkaIkonaMakni(playerid);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUv[i] == u + 1 && i != playerid && gUvIkona[i]) Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, gUvIkona[i], E_STREAMER_PLAYER_ID, playerid);
    if (gUk[u][ukStanje] >= UK_ODBROJ) IgraObavijestUkloni(playerid);
    gUv[playerid] = 0;
    gUvStanje[playerid] = UV_NE;
}

static UtrkaKraj(u)
{
    new bool:vozilo = gUk[u][ukStanje] >= UK_ODBROJ, pot = gUk[u][ukPot], dobitnik = INVALID_PLAYER_ID, najbolje = cellmax;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        if (!vozilo)
        {
            if (gUvUlog[i]) DajNovac(i, gUvUlog[i]);
            continue;
        }
        pot += gUvUlog[i];
        if (gUvUlog[i] && gUvMjesto[i] && gUvMjesto[i] < najbolje)
        {
            najbolje = gUvMjesto[i];
            dobitnik = i;
        }
    }
    if (vozilo && dobitnik == INVALID_PLAYER_ID && pot)
    {
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUv[i] == u + 1 && gUvUlog[i]) DajNovac(i, gUvUlog[i]);
        pot = 0;
    }
    if (dobitnik != INVALID_PLAYER_ID) DajNovac(dobitnik, pot);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        if (vozilo)
        {
            new zarada = (i == dobitnik ? pot : 0) - (dobitnik == INVALID_PLAYER_ID ? 0 : gUvUlog[i]);
            UtrkaStats(i, gUvMjesto[i] == 1, zarada);
            if (gUvMjesto[i]) SendClientMessage(i, COLOR_INFO, T(i, "Utrka %s je zavrsena. Vase mjesto: %d."), gUk[u][ukIme], gUvMjesto[i]);
            else SendClientMessage(i, COLOR_INFO, T(i, "Utrka %s je zavrsena. Niste stigli do cilja."), gUk[u][ukIme]);
            if (i == dobitnik && pot) SendClientMessage(i, COLOR_INFO, T(i, "Osvojili ste $%d iz uloga."), pot);
        }
        else SendClientMessage(i, COLOR_INFO, T(i, "Utrka %s je otkazana, ulog je vracen."), gUk[u][ukIme]);
        UtrkaIzbaci(i);
    }
    gUk[u][ukStanje] = UK_NEMA;
}

static UtrkaPostaviCp(playerid, u)
{
    new c = gUvCp[playerid], n = c + 1 < gUk[u][ukBroj] ? c + 1 : c;
    SetPlayerRaceCheckpoint(playerid, c == gUk[u][ukBroj] - 1 ? CP_TYPE_GROUND_FINISH : CP_TYPE_GROUND_NORMAL, gUkTocke[u][c][0], gUkTocke[u][c][1], gUkTocke[u][c][2], gUkTocke[u][n][0], gUkTocke[u][n][1], gUkTocke[u][n][2], UTRKA_CP);
}

UtrkaPokreni(playerid, i)
{
    if (i < 0 || i >= gUlBroj[playerid]) return 0;
    if (gUv[playerid] || gUrIma[playerid]) return ObavijestGreska(playerid, T(playerid, "Vec uredujete ili vozite utrku."));
    new u = -1;
    for (new k = 0; k < UTRKA_AKTIVNIH && u == -1; k++) if (gUk[k][ukStanje] == UK_NEMA) u = k;
    if (u == -1) return ObavijestGreska(playerid, T(playerid, "Trenutno se vozi previse utrka, pokusajte kasnije."));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z FROM race_points WHERE race = %d ORDER BY idx LIMIT %d", gUlId[playerid][i], UTRKA_TOCAKA), n = 0;
    for (new r = DB_GetRowCount(res); n < r; DB_SelectNextRow(res))
    {
        for (new k = 0; k < 3; k++) gUkTocke[u][n][k] = DB_GetFieldFloat(res, k);
        n++;
    }
    DB_FreeResultSet(res);
    if (n < 2) return ObavijestGreska(playerid, T(playerid, "Utrka nema dovoljno markera."));
    gUk[u][ukStanje] = UK_LOBBY;
    gUk[u][ukDomacin] = playerid;
    gUk[u][ukBroj] = n;
    gUk[u][ukRok] = gettime() + UTRKA_LOBBY_S;
    gUk[u][ukPlasman] = 0;
    gUk[u][ukGrid] = 0;
    gUk[u][ukPrvi] = 0;
    gUk[u][ukObjava] = 0;
    gUk[u][ukPot] = 0;
    gUk[u][ukStvoreno] = gettime();
    strcopy(gUk[u][ukIme], gUlIme[playerid][i], UTRKA_IME);
    UtrkaPonudiUlog(playerid, u);
    return 1;
}

static UtrkaPonudiUlog(playerid, u)
{
    new info[160];
    if (playerid == gUk[u][ukDomacin]) format(info, sizeof info, T(playerid, "Utrka: %s"), gUk[u][ukIme]);
    else format(info, sizeof info, T(playerid, "%s vas poziva na utrku %s."), GetRPIme(gUk[u][ukDomacin]), gUk[u][ukIme]);
    UpitKolicina(playerid, UPIT_UTRKA, u, gAccountId[gUk[u][ukDomacin]], T(playerid, "Utrka - ulog na sebe"), info);
}

UtrkaPozovi(playerid, cilj)
{
    new u = gUv[playerid] - 1;
    if (u < 0 || gUk[u][ukDomacin] != playerid || gUk[u][ukStanje] != UK_LOBBY) return ObavijestGreska(playerid, T(playerid, "Niste domacin utrke koja ceka vozace."));
    if (cilj == playerid || !IsPlayerConnected(cilj) || !gUlogiran[cilj] || IsPlayerNPC(cilj)) return ObavijestGreska(playerid, T(playerid, "Taj igrac nije dostupan."));
    if (gUv[cilj] || gUrIma[cilj]) return ObavijestGreska(playerid, T(playerid, "Taj igrac je vec u utrci."));
    if (UtrkaVozaci(u) >= UTRKA_VOZACA) return ObavijestGreska(playerid, T(playerid, "Utrka je puna."));
    UtrkaPonudiUlog(cilj, u);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pozvali ste %s na utrku."), GetRPIme(cilj));
    return 1;
}

UtrkaUlog(playerid, u, racun, ulog)
{
    if (u < 0 || u >= UTRKA_AKTIVNIH || gUk[u][ukStanje] != UK_LOBBY || gAccountId[gUk[u][ukDomacin]] != racun) return ObavijestGreska(playerid, T(playerid, "Utrka vise ne prima vozace."));
    if (gUv[playerid] || gUrIma[playerid]) return ObavijestGreska(playerid, T(playerid, "Vec uredujete ili vozite utrku."));
    if (UtrkaVozaci(u) >= UTRKA_VOZACA) return ObavijestGreska(playerid, T(playerid, "Utrka je puna."));
    if (ulog < 0 || ulog > UTRKA_MAX_ULOG) return ObavijestGreska(playerid, T(playerid, "Ulog mora biti od $0 do $100000."));
    if (ulog && !DajNovac(playerid, -ulog)) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
    gUv[playerid] = u + 1;
    gUvStanje[playerid] = UV_NA_START;
    gUvUlog[playerid] = ulog;
    gUvMjesto[playerid] = 0;
    gUvCp[playerid] = 0;
    UtrkaPostaviCp(playerid, u);
    ObavijestInfo(playerid, T(playerid, "Utrka"), T(playerid, "Dovezite se do starta oznacenog na karti."));
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUv[i] == u + 1 && i != playerid) SendClientMessage(i, COLOR_INFO, T(i, "%s se prijavio na utrku (ulog $%d)."), GetRPIme(playerid), ulog);
    if (playerid == gUk[u][ukDomacin]) MobitelUtrkaPoziv(playerid);
    return 1;
}

static UtrkaNaGrid(playerid, u)
{
    new v = GetPlayerVehicleID(playerid);
    if (!v || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
    {
        ObavijestGreska(playerid, T(playerid, "Na start morate doci vozilom."));
        return;
    }
    new s = gUk[u][ukGrid]++, red = s / 2, Float:dx = gUkTocke[u][1][0] - gUkTocke[u][0][0], Float:dy = gUkTocke[u][1][1] - gUkTocke[u][0][1];
    new Float:d = VectorSize(dx, dy, 0.0);
    if (d < 0.1) dx = 0.0, dy = 1.0, d = 1.0;
    dx /= d;
    dy /= d;
    new Float:strana = s % 2 ? 2.8 : -2.8, Float:nazad = 4.0 + float(red) * 8.0;
    new Float:x = gUkTocke[u][0][0] - dx * nazad - dy * strana, Float:y = gUkTocke[u][0][1] - dy * nazad + dx * strana, Float:z = gUkTocke[u][0][2], Float:hx, Float:hy, Float:hz;
    if (Kolizija_Zraka(x, y, z + 6.0, x, y, z - 20.0, hx, hy, hz)) z = hz + 1.0;
    SetVehiclePos(v, x, y, z);
    SetVehicleZAngle(v, atan2(dy, dx) - 90.0);
    SetVehicleVelocity(v, 0.0, 0.0, 0.0);
    TogglePlayerControllable(playerid, false);
    DisablePlayerRaceCheckpoint(playerid);
    gUvStanje[playerid] = UV_GRID;
    new naStartu, ukupno = UtrkaVozaci(u, naStartu);
    if (ukupno >= 2 && naStartu == ukupno) UtrkaOdbroj(u);
    else ObavijestInfo(playerid, T(playerid, "Utrka"), T(playerid, "Cekamo ostale vozace na startu."));
}

static UtrkaOdbroj(u)
{
    gUk[u][ukStanje] = UK_ODBROJ;
    gUk[u][ukOdbroj] = UTRKA_ODBROJ;
    gUk[u][ukRok] = gettime() + UTRKA_TRAJE_S;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        UpitMakni(i, UPIT_UTRKA);
        new Float:x, Float:y, Float:z, ostali[UTRKA_VOZACA], n = 0;
        for (new j = 0, jZadnji = gIgracZadnji; j <= jZadnji && n < UTRKA_VOZACA; j++) if (gUv[j] == u + 1 && j != i) ostali[n++] = j;
        if (!n) continue;
        GetPlayerPos(i, x, y, z);
        gUvIkona[i] = CreateDynamicMapIconEx(x, y, z, 0, UTRKA_BOJA | 0xFF, MAPICON_GLOBAL, 6000.0, .players = ostali, .maxplayers = n);
    }
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    new u = gUv[playerid] - 1;
    if (u < 0) return 1;
    if (gUvStanje[playerid] == UV_NA_START && gUk[u][ukStanje] == UK_LOBBY)
    {
        UtrkaNaGrid(playerid, u);
        return 1;
    }
    if (gUvStanje[playerid] != UV_VOZI || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
    if (++gUvCp[playerid] < gUk[u][ukBroj])
    {
        UtrkaPostaviCp(playerid, u);
        return 1;
    }
    DisablePlayerRaceCheckpoint(playerid);
    gUvStanje[playerid] = UV_CILJ;
    gUvMjesto[playerid] = ++gUk[u][ukPlasman];
    if (gUvMjesto[playerid] == 1) gUk[u][ukPrvi] = gettime();
    gUk[u][ukObjava] = GetTickCount() + UTRKA_OBJAVA_MS;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        new tekst[96];
        format(tekst, sizeof tekst, T(i, "%s je zavrsio utrku kao %d."), GetRPIme(playerid), gUvMjesto[playerid]);
        IgraObavijest(i, T(i, "Utrka"), UTRKA_BOJA, tekst, UTRKA_OBJAVA_MS);
    }
    return 1;
}

static Float:UtrkaNapredak(playerid, u)
{
    if (gUvStanje[playerid] == UV_CILJ) return 100000.0 - float(gUvMjesto[playerid]);
    new c = gUvCp[playerid], Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    return float(c) * 10000.0 - VectorSize(x - gUkTocke[u][c][0], y - gUkTocke[u][c][1], z - gUkTocke[u][c][2]);
}

static UtrkaPoredak(u)
{
    new red[UTRKA_VOZACA], Float:napredak[UTRKA_VOZACA], n = 0, tekst[512];
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji && n < UTRKA_VOZACA; i++)
    {
        if (gUv[i] != u + 1) continue;
        new Float:p = UtrkaNapredak(i, u), k = n++;
        while (k > 0 && napredak[k - 1] < p)
        {
            red[k] = red[k - 1];
            napredak[k] = napredak[k - 1];
            k--;
        }
        red[k] = i;
        napredak[k] = p;
    }
    for (new k = 0; k < n; k++)
    {
        new linija[48];
        format(linija, sizeof linija, "%s%s%d. %s", k ? ("~n~") : (""), gUvStanje[red[k]] == UV_CILJ ? ("~g~") : ("~w~"), k + 1, GetRPIme(red[k]));
        strcat(tekst, linija);
    }
    for (new k = 0; k < n; k++) IgraObavijestPanel(red[k], T(red[k], "Poredak"), UTRKA_BOJA, tekst);
}

UtrkaTick()
{
    new sad = gettime();
    for (new u = 0; u < UTRKA_AKTIVNIH; u++)
    {
        if (gUk[u][ukStanje] == UK_NEMA) continue;
        new naStartu, ukupno = UtrkaVozaci(u, naStartu);
        if ((!ukupno && (gUk[u][ukStanje] != UK_LOBBY || sad - gUk[u][ukStvoreno] > 60)) || sad >= gUk[u][ukRok] || (gUk[u][ukPrvi] && sad - gUk[u][ukPrvi] >= UTRKA_NAKON_PRVOG_S) || (gUk[u][ukStanje] == UK_VOZI && gUk[u][ukPlasman] >= ukupno))
        {
            UtrkaKraj(u);
            continue;
        }
        if (gUk[u][ukStanje] == UK_LOBBY)
        {
            if (ukupno >= 2 && naStartu == ukupno) UtrkaOdbroj(u);
            continue;
        }
        if (gUk[u][ukStanje] == UK_ODBROJ)
        {
            new o = gUk[u][ukOdbroj]--;
            for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
            {
                if (gUv[i] != u + 1) continue;
                if (o > 0)
                {
                    new t[16];
                    format(t, sizeof t, "~y~%d", o);
                    GameTextForPlayer(i, t, 1000, 3);
                    PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
                    continue;
                }
                GameTextForPlayer(i, T(i, "~g~KRENI!"), 1500, 3);
                PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
                TogglePlayerControllable(i, true);
                gUvStanje[i] = UV_VOZI;
                gUvCp[i] = 1;
                UtrkaPostaviCp(i, u);
            }
            if (o <= 0) gUk[u][ukStanje] = UK_VOZI;
            continue;
        }
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        {
            if (gUv[i] != u + 1 || !gUvIkona[i]) continue;
            new Float:x, Float:y, Float:z;
            GetPlayerPos(i, x, y, z);
            Streamer_SetFloatData(STREAMER_TYPE_MAP_ICON, gUvIkona[i], E_STREAMER_X, x);
            Streamer_SetFloatData(STREAMER_TYPE_MAP_ICON, gUvIkona[i], E_STREAMER_Y, y);
            Streamer_SetFloatData(STREAMER_TYPE_MAP_ICON, gUvIkona[i], E_STREAMER_Z, z);
        }
        if (GetTickCount() - gUk[u][ukObjava] >= 0) UtrkaPoredak(u);
    }
}

UtrkaKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gUrIma[playerid]) return 0;
    new PLAYER_STATE:st = GetPlayerState(playerid);
    new bool:alt = st == PLAYER_STATE_ONFOOT ? ((newkeys & KEY_WALK) && !(oldkeys & KEY_WALK)) : ((newkeys & KEY_ACTION) && !(oldkeys & KEY_ACTION));
    if ((newkeys & KEY_NO) && !(oldkeys & KEY_NO))
    {
        UtrkaUrediKraj(playerid);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Izrada utrke je otkazana."));
        return 1;
    }
    if (gUrIme[playerid]) return (alt || ((newkeys & KEY_YES) && !(oldkeys & KEY_YES))) ? 1 : 0;
    if (alt)
    {
        UtrkaUrediAlt(playerid);
        return 1;
    }
    if ((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        UtrkaUrediCilj(playerid);
        return 1;
    }
    return 0;
}

UtrkaAktivna(playerid)
{
    return gUv[playerid] != 0;
}

UtrkaNapusti(playerid)
{
    new u = gUv[playerid] - 1;
    if (u < 0) return;
    if (gUk[u][ukStanje] == UK_LOBBY && gUk[u][ukDomacin] == playerid)
    {
        UtrkaKraj(u);
        return;
    }
    if (gUk[u][ukStanje] == UK_LOBBY && gUvUlog[playerid]) DajNovac(playerid, gUvUlog[playerid]);
    else if (gUk[u][ukStanje] >= UK_ODBROJ) UtrkaStats(playerid, false, -gUvUlog[playerid]);
    if (gUk[u][ukStanje] >= UK_ODBROJ) gUk[u][ukPot] += gUvUlog[playerid], gUvUlog[playerid] = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUv[i] == u + 1 && i != playerid) SendClientMessage(i, COLOR_INFO, T(i, "%s je napustio utrku."), GetRPIme(playerid));
    UtrkaIzbaci(playerid);
}

UtrkaDisconnect(playerid)
{
    if (gUrIma[playerid]) UtrkaUrediKraj(playerid);
    UtrkaNapusti(playerid);
    gUv[playerid] = 0;
    gUvIkona[playerid] = STREAMER_TAG_MAP_ICON:0;
}
