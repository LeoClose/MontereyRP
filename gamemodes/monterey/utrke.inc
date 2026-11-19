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
#define UTRKA_OBJAVA_MS     5000
#define UTRKA_REZULTAT_MS   10000
#define UTRKA_START_SD      300.0
#define UTRKA_MAX_ULOG      100000
#define UTRKA_BOJA          0xE67E22E6
#define UTRKA_MARKER        1559

enum { UV_NE, UV_NA_START, UV_GRID, UV_VOZI, UV_CILJ }
enum { UK_NEMA, UK_LOBBY, UK_ODBROJ, UK_VOZI }
enum E_UTRKA { ukStanje, ukDomacin, ukRacun, STREAMER_TAG_RACE_CP:ukCp, ukPot, ukStart, ukIme[UTRKA_IME], ukBroj, ukOdbroj, ukRok, ukPrvi, ukPlasman, ukGrid, ukObjava }

static gUk[UTRKA_AKTIVNIH][E_UTRKA], Float:gUkTocke[UTRKA_AKTIVNIH][UTRKA_TOCAKA][3];
static bool:gUvUlogDan[MAX_PLAYERS], gUv[MAX_PLAYERS], gUvStanje[MAX_PLAYERS], gUvCp[MAX_PLAYERS], gUvUlog[MAX_PLAYERS], gUvMjesto[MAX_PLAYERS], STREAMER_TAG_MAP_ICON:gUvIkona[MAX_PLAYERS];
static bool:gUrIma[MAX_PLAYERS], bool:gUrIme[MAX_PLAYERS], gUrBroj[MAX_PLAYERS], Float:gUrTocke[MAX_PLAYERS][UTRKA_TOCAKA][3], STREAMER_TAG_OBJECT:gUrObjekt[MAX_PLAYERS][UTRKA_TOCAKA];
static gUlBroj[MAX_PLAYERS], gUlId[MAX_PLAYERS][UTRKA_MAX_PO_IGRACU], gUlIme[MAX_PLAYERS][UTRKA_MAX_PO_IGRACU][UTRKA_IME];

InitUtrke()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS races (id INTEGER PRIMARY KEY AUTOINCREMENT, owner INTEGER NOT NULL, name TEXT NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS race_points (race INTEGER NOT NULL, idx INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, PRIMARY KEY (race, idx))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS race_stats (account INTEGER PRIMARY KEY, won INTEGER NOT NULL DEFAULT 0, second INTEGER NOT NULL DEFAULT 0, third INTEGER NOT NULL DEFAULT 0, lost INTEGER NOT NULL DEFAULT 0, dnf INTEGER NOT NULL DEFAULT 0, earned INTEGER NOT NULL DEFAULT 0)"));
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
    new s[6], DBResult:res = DB_ExecuteQuery(gDB, "SELECT won, second, third, lost, dnf, earned FROM race_stats WHERE account = %d", gAccountId[playerid]);
    if (DB_GetRowCount(res)) for (new i = 0; i < sizeof s; i++) s[i] = DB_GetFieldInt(res, i);
    DB_FreeResultSet(res);
    format(tekst, len, T(playerid, "1. mjesto: %d~n~2. mjesto: %d~n~3. mjesto: %d~n~Ostala mjesta: %d~n~DNF: %d~n~Zarada: $%d"), s[0], s[1], s[2], s[3], s[4], s[5]);
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
    else format(tekst, sizeof tekst, T(playerid, "~y~ALT~w~ / ~y~H~w~ (vozilo) - postavi / ukloni marker~n~~y~Y~w~ - cilj i ime utrke~n~~y~N~w~ - odustani~n~Markera: %d/%d"), gUrBroj[playerid], UTRKA_TOCAKA);
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
    new u = gUv[playerid] - 1;
    if (u >= 0 && gUk[u][ukStanje] == UK_LOBBY && !gUvUlogDan[playerid] && ProvjeriBroj(text) && strlen(text) <= 6)
    {
        UtrkaUlog(playerid, u, strval(text));
        return 1;
    }
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
    new tekst[96];
    format(tekst, sizeof tekst, T(playerid, "Utrka %s je spremljena."), text);
    ObavijestInfo(playerid, T(playerid, "Utrka"), tekst);
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

static UtrkaStats(playerid, mjesto, zarada)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO race_stats (account) VALUES (%d)", gAccountId[playerid]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE race_stats SET won = won + %d, second = second + %d, third = third + %d, lost = lost + %d, dnf = dnf + %d, earned = earned + %d WHERE account = %d",
        mjesto == 1 ? 1 : 0, mjesto == 2 ? 1 : 0, mjesto == 3 ? 1 : 0, mjesto > 3 ? 1 : 0, mjesto == 0 ? 1 : 0, zarada, gAccountId[playerid]));
}

static UtrkaIkonaMakni(playerid)
{
    if (!gUvIkona[playerid]) return;
    DestroyDynamicMapIcon(gUvIkona[playerid]);
    gUvIkona[playerid] = STREAMER_TAG_MAP_ICON:0;
}

static UtrkaIzbaci(playerid, bool:makniObavijest = true)
{
    new u = gUv[playerid] - 1;
    if (u < 0) return;
    if (gUvStanje[playerid] == UV_GRID) TogglePlayerControllable(playerid, true);
    DisablePlayerRaceCheckpoint(playerid);
    UtrkaIkonaMakni(playerid);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUv[i] == u + 1 && i != playerid && gUvIkona[i]) Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, gUvIkona[i], E_STREAMER_PLAYER_ID, playerid);
    if (makniObavijest) IgraObavijestUkloni(playerid);
    gUv[playerid] = 0;
    gUvStanje[playerid] = UV_NE;
}

static UtrkaKraj(u)
{
    new bool:vozilo = gUk[u][ukStanje] >= UK_ODBROJ, pot = gUk[u][ukPot], dobitnik = INVALID_PLAYER_ID, najbolje = cellmax;
    if (gUk[u][ukCp])
    {
        DestroyDynamicRaceCP(gUk[u][ukCp]);
        gUk[u][ukCp] = STREAMER_TAG_RACE_CP:0;
    }
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
        new tekst[192], red[96];
        if (vozilo)
        {
            UtrkaStats(i, gUvMjesto[i], (i == dobitnik ? pot : 0) - (dobitnik == INVALID_PLAYER_ID ? 0 : gUvUlog[i]));
            if (gUvMjesto[i]) format(tekst, sizeof tekst, T(i, "Utrka %s je zavrsena. Vase mjesto: %d."), gUk[u][ukIme], gUvMjesto[i]);
            else format(tekst, sizeof tekst, T(i, "Utrka %s je zavrsena. DNF - niste stigli do cilja."), gUk[u][ukIme]);
            if (i == dobitnik && pot)
            {
                format(red, sizeof red, T(i, "Osvojili ste $%d iz uloga."), pot);
                strcat(tekst, "~n~");
                strcat(tekst, red);
            }
        }
        else format(tekst, sizeof tekst, T(i, "Utrka %s je otkazana, ulog je vracen."), gUk[u][ukIme]);
        UtrkaIzbaci(i, false);
        IgraObavijest(i, T(i, "Utrka"), UTRKA_BOJA, tekst, UTRKA_REZULTAT_MS);
    }
    if (IsPlayerConnected(gUk[u][ukDomacin]) && gAccountId[gUk[u][ukDomacin]] == gUk[u][ukRacun] && !vozilo && gUv[gUk[u][ukDomacin]] == 0)
        ObavijestInfo(gUk[u][ukDomacin], T(gUk[u][ukDomacin], "Utrka"), T(gUk[u][ukDomacin], "Utrka je zatvorena."));
    gUk[u][ukStanje] = UK_NEMA;
}

static UtrkaPostaviCp(playerid, u)
{
    new c = gUvCp[playerid], n = c + 1 < gUk[u][ukBroj] ? c + 1 : c;
    SetPlayerRaceCheckpoint(playerid, c == gUk[u][ukBroj] - 1 ? CP_TYPE_GROUND_FINISH : CP_TYPE_GROUND_NORMAL, gUkTocke[u][c][0], gUkTocke[u][c][1], gUkTocke[u][c][2], gUkTocke[u][n][0], gUkTocke[u][n][1], gUkTocke[u][n][2], UTRKA_CP);
}

static UtrkaDomacina(playerid)
{
    for (new u = 0; u < UTRKA_AKTIVNIH; u++)
        if (gUk[u][ukStanje] == UK_LOBBY && gUk[u][ukDomacin] == playerid && gUk[u][ukRacun] == gAccountId[playerid]) return u;
    return -1;
}

UtrkaPokreni(playerid, i)
{
    if (i < 0 || i >= gUlBroj[playerid]) return 0;
    if (gUv[playerid] || gUrIma[playerid] || UtrkaDomacina(playerid) != -1) return ObavijestGreska(playerid, T(playerid, "Vec uredujete ili vozite utrku."));
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
    gUk[u][ukRacun] = gAccountId[playerid];
    gUk[u][ukBroj] = n;
    gUk[u][ukRok] = gettime() + UTRKA_LOBBY_S;
    gUk[u][ukPlasman] = 0;
    gUk[u][ukGrid] = 0;
    gUk[u][ukPrvi] = 0;
    gUk[u][ukObjava] = 0;
    gUk[u][ukPot] = 0;
    strcopy(gUk[u][ukIme], gUlIme[playerid][i], UTRKA_IME);
    gUk[u][ukCp] = CreateDynamicRaceCP(_:CP_TYPE_GROUND_NORMAL, gUkTocke[u][0][0], gUkTocke[u][0][1], gUkTocke[u][0][2], gUkTocke[u][1][0], gUkTocke[u][1][1], gUkTocke[u][1][2], UTRKA_CP, 0, 0, -1, UTRKA_START_SD);
    ObavijestInfo(playerid, T(playerid, "Utrka"), T(playerid, "Start je postavljen. Tko ude u start vozilom, ulazi u utrku. Na startu Y pokrece utrku, N je otkazuje."));
    return 1;
}

public OnPlayerEnterDynamicRaceCP(playerid, STREAMER_TAG_RACE_CP:checkpointid)
{
    new u = -1;
    for (new k = 0; k < UTRKA_AKTIVNIH && u == -1; k++) if (gUk[k][ukStanje] == UK_LOBBY && gUk[k][ukCp] == checkpointid) u = k;
    if (u == -1 || !gUlogiran[playerid] || gUv[playerid]) return 1;
    if (gUrIma[playerid]) return ObavijestGreska(playerid, T(playerid, "Vec uredujete ili vozite utrku."));
    if (!GetPlayerVehicleID(playerid) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return ObavijestGreska(playerid, T(playerid, "Na start morate doci vozilom."));
    if (UtrkaVozaci(u) >= UTRKA_VOZACA) return ObavijestGreska(playerid, T(playerid, "Utrka je puna."));
    gUv[playerid] = u + 1;
    gUvUlog[playerid] = 0;
    gUvMjesto[playerid] = 0;
    gUvCp[playerid] = 0;
    gUvUlogDan[playerid] = false;
    UtrkaNaGrid(playerid, u);
    return 1;
}

static UtrkaUlog(playerid, u, ulog)
{
    if (ulog < 0 || ulog > UTRKA_MAX_ULOG) return ObavijestGreska(playerid, T(playerid, "Ulog mora biti od $0 do $100000."));
    if (ulog && !DajNovac(playerid, -ulog)) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca."));
    gUvUlogDan[playerid] = true;
    gUvUlog[playerid] = ulog;
    UtrkaLobbyPanel(u);
    if (!ulog) return 1;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUv[i] == u + 1) SendClientMessage(i, COLOR_INFO, T(i, "%s ulaze $%d na sebe."), GetRPIme(playerid), ulog);
    return 1;
}

static UtrkaNaGrid(playerid, u)
{
    new v = GetPlayerVehicleID(playerid);
    new s = gUk[u][ukGrid]++, red = s / 2, Float:dx = gUkTocke[u][1][0] - gUkTocke[u][0][0], Float:dy = gUkTocke[u][1][1] - gUkTocke[u][0][1];
    new Float:d = VectorSize(dx, dy, 0.0);
    if (d < 0.1) dx = 0.0, dy = 1.0, d = 1.0;
    dx /= d;
    dy /= d;
    new Float:strana = s % 2 ? 2.8 : -2.8, Float:nazad = 4.0 + float(red) * 8.0;
    new Float:x = gUkTocke[u][0][0] - dx * nazad - dy * strana, Float:y = gUkTocke[u][0][1] - dy * nazad + dx * strana, Float:z = gUkTocke[u][0][2], Float:hx, Float:hy, Float:hz;
    if (Kolizija_Zraka(x, y, z + 6.0, x, y, z - 20.0, hx, hy, hz)) z = hz + 1.0;
    new Float:a = atan2(dy, dx) - 90.0;
    TogglePlayerControllable(playerid, false);
    gUvStanje[playerid] = UV_GRID;
    UtrkaPoravnaj(playerid, v, x, y, z, a, 0);
    UtrkaLobbyPanel(u);
}

forward UtrkaPoravnaj(playerid, v, Float:x, Float:y, Float:z, Float:a, korak);
public UtrkaPoravnaj(playerid, v, Float:x, Float:y, Float:z, Float:a, korak)
{
    if (!gUv[playerid] || gUvStanje[playerid] != UV_GRID || GetPlayerVehicleID(playerid) != v) return;
    SetVehicleVelocity(v, 0.0, 0.0, 0.0);
    SetVehicleZAngle(v, a);
    SetVehiclePos(v, x, y, z);
    SetVehicleZAngle(v, a);
    if (korak < 2) SetTimerEx("UtrkaPoravnaj", 400, false, "iiffffi", playerid, v, x, y, z, a, korak + 1);
}

static UtrkaLobbyPanel(u)
{
    new popis[400], naslov[48];
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        new linija[48];
        format(linija, sizeof linija, "~n~~g~%s", GetRPIme(i));
        strcat(popis, linija);
    }
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        new tekst[640], red[96];
        if (!gUvUlogDan[i]) strcat(tekst, T(i, "Upisite ulog na sebe u chat (0 = bez uloga)."));
        else
        {
            format(red, sizeof red, T(i, "Vas ulog: $%d"), gUvUlog[i]);
            strcat(tekst, red);
        }
        strcat(tekst, "~n~");
        strcat(tekst, i == gUk[u][ukDomacin] ? T(i, "~y~Y~w~ - pokreni utrku") : T(i, "Cekamo da domacin pokrene utrku."));
        strcat(tekst, "~n~");
        strcat(tekst, T(i, "~y~N~w~ - napusti utrku"));
        strcat(tekst, popis);
        format(naslov, sizeof naslov, T(i, "Utrka: %s"), gUk[u][ukIme]);
        IgraObavijestPanel(i, naslov, UTRKA_BOJA, tekst);
    }
}

static UtrkaDomacinKreni(playerid)
{
    new u = gUv[playerid] - 1;
    if (u < 0 || gUk[u][ukDomacin] != playerid || gUk[u][ukRacun] != gAccountId[playerid] || gUk[u][ukStanje] != UK_LOBBY) return 0;
    UtrkaOdbroj(u);
    return 1;
}

static UtrkaOdbroj(u)
{
    gUk[u][ukStanje] = UK_ODBROJ;
    gUk[u][ukOdbroj] = UTRKA_ODBROJ;
    gUk[u][ukRok] = gettime() + UTRKA_TRAJE_S;
    if (gUk[u][ukCp])
    {
        DestroyDynamicRaceCP(gUk[u][ukCp]);
        gUk[u][ukCp] = STREAMER_TAG_RACE_CP:0;
    }
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        IgraObavijestUkloni(i);
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
    new ms = GetTickCount() - gUk[u][ukStart], vrijeme[16];
    format(vrijeme, sizeof vrijeme, "%d:%02d.%d", ms / 60000, (ms / 1000) % 60, (ms / 100) % 10);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gUv[i] != u + 1) continue;
        new tekst[128];
        format(tekst, sizeof tekst, T(i, "%s je zavrsio utrku kao %d. (%s)"), GetRPIme(playerid), gUvMjesto[playerid], vrijeme);
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
        new ukupno = UtrkaVozaci(u), bool:domacin = IsPlayerConnected(gUk[u][ukDomacin]) && gAccountId[gUk[u][ukDomacin]] == gUk[u][ukRacun];
        if ((gUk[u][ukStanje] == UK_LOBBY && !domacin) || (gUk[u][ukStanje] != UK_LOBBY && !ukupno) || sad >= gUk[u][ukRok] || (gUk[u][ukPrvi] && sad - gUk[u][ukPrvi] >= UTRKA_NAKON_PRVOG_S)
            || (gUk[u][ukStanje] == UK_VOZI && gUk[u][ukPlasman] >= ukupno && GetTickCount() - gUk[u][ukObjava] >= 0))
        {
            UtrkaKraj(u);
            continue;
        }
        if (gUk[u][ukStanje] == UK_LOBBY)
        {
            UtrkaLobbyPanel(u);
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
            if (o <= 0)
            {
                gUk[u][ukStanje] = UK_VOZI;
                gUk[u][ukStart] = GetTickCount();
            }
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
    new u = gUv[playerid] - 1;
    if ((u >= 0 ? gUk[u][ukStanje] == UK_LOBBY : UtrkaDomacina(playerid) != -1) && !gUrIma[playerid] && (newkeys & KEY_NO) && !(oldkeys & KEY_NO))
    {
        UtrkaNapusti(playerid);
        ObavijestInfo(playerid, T(playerid, "Utrka"), T(playerid, "Napustili ste utrku."));
        return 1;
    }
    if (!gUrIma[playerid]) return (newkeys & KEY_YES) && !(oldkeys & KEY_YES) ? UtrkaDomacinKreni(playerid) : 0;
    new PLAYER_STATE:st = GetPlayerState(playerid);
    new bool:alt = st == PLAYER_STATE_ONFOOT ? ((newkeys & KEY_WALK) && !(oldkeys & KEY_WALK)) : ((newkeys & KEY_CROUCH) && !(oldkeys & KEY_CROUCH));
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
    return gUv[playerid] != 0 || UtrkaDomacina(playerid) != -1;
}

UtrkaNapusti(playerid)
{
    new u = gUv[playerid] - 1;
    if (u < 0)
    {
        u = UtrkaDomacina(playerid);
        if (u != -1) UtrkaKraj(u);
        return;
    }
    if (gUk[u][ukStanje] == UK_LOBBY && gUk[u][ukDomacin] == playerid)
    {
        UtrkaKraj(u);
        return;
    }
    if (gUk[u][ukStanje] == UK_LOBBY && gUvUlog[playerid]) DajNovac(playerid, gUvUlog[playerid]);
    else if (gUk[u][ukStanje] >= UK_ODBROJ) UtrkaStats(playerid, 0, -gUvUlog[playerid]);
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
