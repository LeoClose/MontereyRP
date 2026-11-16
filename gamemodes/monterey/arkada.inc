#define ARK_MAX             24
#define ARK_CIJENA          5
#define ARK_SW              14
#define ARK_SH              10
#define ARK_CELIJA          (ARK_SW * ARK_SH)
#define ARK_CEL             12.0
#define ARK_X               (320.0 - ARK_SW * ARK_CEL / 2.0)
#define ARK_Y               150.0
#define ARK_TD              (ARK_CELIJA + 7)
#define ARK_KRAJ_MS         5000
#define ARK_DOMET           1.6

enum { AI_ZMIJA, AI_REAKCIJA }
enum E_ARK { bool:amIma, amDb, Float:amX, Float:amY, Float:amZ, Float:amA, amInt, amSvijet, amIgra, STREAMER_TAG_OBJECT:amObj, STREAMER_TAG_3D_TEXT_LABEL:amTekst,
    amIgrac, amToken, amBod, amZivoti, amSmjer, amRep, amDuz, amHrana, amMeta, amMs, bool:amKraj }

static const gArkModel[] = { 2681, 2779 };
static const gArkIme[][] = { "Snake", "Reakcija" };
static const gArkSmjerX[] = { 0, 1, 0, -1 };
static const gArkSmjerY[] = { -1, 0, 1, 0 };

static gArk[ARK_MAX][E_ARK], gArkZmija[ARK_MAX][ARK_CELIJA], bool:gArkPolje[ARK_MAX][ARK_CELIJA];
static gArkAutomat[MAX_PLAYERS] = { -1, ... }, PlayerText:gArkTd[MAX_PLAYERS][ARK_TD], gArkBr[MAX_PLAYERS], PlayerText:gArkBodTd[MAX_PLAYERS], PlayerText:gArkInfoTd[MAX_PLAYERS], gApArk[MAX_PLAYERS][ARK_MAX];

InitArkada()
{
    assert(gArkSmjerX[1] + gArkSmjerX[3] == 0 && gArkSmjerY[0] + gArkSmjerY[2] == 0);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS arcade_machines (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL, game INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS arcade_scores (machine INTEGER NOT NULL, account INTEGER NOT NULL, score INTEGER NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS arcade_scores_machine ON arcade_scores (machine, score DESC)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world, game FROM arcade_machines LIMIT %d", ARK_MAX);
    for (new m = 0, n = DB_GetRowCount(res); m < n; m++, DB_SelectNextRow(res))
    {
        gArk[m][amDb] = DB_GetFieldInt(res, 0);
        gArk[m][amX] = DB_GetFieldFloat(res, 1);
        gArk[m][amY] = DB_GetFieldFloat(res, 2);
        gArk[m][amZ] = DB_GetFieldFloat(res, 3);
        gArk[m][amA] = DB_GetFieldFloat(res, 4);
        gArk[m][amInt] = DB_GetFieldInt(res, 5);
        gArk[m][amSvijet] = DB_GetFieldInt(res, 6);
        gArk[m][amIgra] = DB_GetFieldInt(res, 7) == AI_REAKCIJA ? AI_REAKCIJA : AI_ZMIJA;
        ArkStvori(m);
    }
    DB_FreeResultSet(res);
}

static ArkStvori(m)
{
    new tekst[64];
    format(tekst, sizeof tekst, "{F39C12}%s\n{FFFFFF}[Y] $%d", gArkIme[gArk[m][amIgra]], ARK_CIJENA);
    gArk[m][amIma] = true;
    gArk[m][amIgrac] = INVALID_PLAYER_ID;
    gArk[m][amObj] = CreateDynamicObject(gArkModel[gArk[m][amIgra]], gArk[m][amX], gArk[m][amY], gArk[m][amZ], 0.0, 0.0, gArk[m][amA], gArk[m][amSvijet], gArk[m][amInt]);
    gArk[m][amTekst] = CreateDynamic3DTextLabel(tekst, 0xFFFFFFFF, gArk[m][amX], gArk[m][amY], gArk[m][amZ] + 2.1, 6.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gArk[m][amSvijet], gArk[m][amInt]);
}

static PlayerText:ArkKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja, bool:klik = false)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "_");
    PlayerTextDrawLetterSize(playerid, td, 0.0, h / 9.0);
    PlayerTextDrawTextSize(playerid, td, x + w, y + h);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    if (klik) PlayerTextDrawSetSelectable(playerid, td, true);
    PlayerTextDrawShow(playerid, td);
    gArkTd[playerid][gArkBr[playerid]++] = td;
    return td;
}

static PlayerText:ArkTekst(playerid, Float:x, Float:y, const tekst[], Float:vel, boja, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_CENTER)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawLetterSize(playerid, td, vel * 0.25, vel);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawBackgroundColour(playerid, td, 0x000000AA);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawSetProportional(playerid, td, true);
    PlayerTextDrawShow(playerid, td);
    gArkTd[playerid][gArkBr[playerid]++] = td;
    return td;
}

static ArkCelija(playerid, c, boja)
{
    PlayerTextDrawBoxColour(playerid, gArkTd[playerid][c + 1], boja);
    PlayerTextDrawShow(playerid, gArkTd[playerid][c + 1]);
}

static ArkBodovi(m)
{
    new p = gArk[m][amIgrac], tekst[64];
    if (gArk[m][amIgra] == AI_REAKCIJA) format(tekst, sizeof tekst, "~w~%s: ~y~%d   ~w~%s: ~r~%d", T(p, "BODOVI"), gArk[m][amBod], T(p, "ZIVOTI"), gArk[m][amZivoti]);
    else format(tekst, sizeof tekst, "~w~%s: ~y~%d", T(p, "BODOVI"), gArk[m][amBod]);
    PlayerTextDrawSetString(p, gArkBodTd[p], tekst);
}

bool:ArkadaKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || gArkAutomat[playerid] != -1) return false;
    new m = -1;
    for (new i = 0; i < ARK_MAX && m == -1; i++)
        if (gArk[i][amIma] && gArk[i][amSvijet] == GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, ARK_DOMET, gArk[i][amX], gArk[i][amY], gArk[i][amZ] + 1.0)) m = i;
    if (m == -1) return false;
    if (gArk[m][amIgrac] != INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Netko vec igra na ovom automatu.")), true;
    if (MobitelOtvoren(playerid) || MdtOtvoren(playerid)) return true;
    if (!DajNovac(playerid, -ARK_CIJENA)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), true;
    BlagajnaDodaj(ARK_CIJENA);
    ArkPokreni(playerid, m);
    return true;
}

static ArkPokreni(playerid, m)
{
    gArkAutomat[playerid] = m;
    gArkBr[playerid] = 0;
    gArk[m][amIgrac] = playerid;
    gArk[m][amToken]++;
    gArk[m][amBod] = 0;
    gArk[m][amKraj] = false;
    TogglePlayerControllable(playerid, false);
    SetPlayerFacingAngle(playerid, gArk[m][amA] + 180.0);
    if (gArk[m][amIgra] == AI_ZMIJA)
    {
        ArkKutija(playerid, ARK_X - 8.0, ARK_Y - 34.0, ARK_SW * ARK_CEL + 14.5, ARK_SH * ARK_CEL + 64.0, 0x0B1A10E6);
        for (new c = 0; c < ARK_CELIJA; c++)
        {
            ArkKutija(playerid, ARK_X + float(c % ARK_SW) * ARK_CEL, ARK_Y + float(c / ARK_SW) * ARK_CEL, ARK_CEL - 1.5, ARK_CEL - 1.5, 0x10301AFF);
            gArkPolje[m][c] = false;
        }
        ArkTekst(playerid, 320.0, ARK_Y - 30.0, "SNAKE", 1.6, 0x7CFC00FF);
        gArkBodTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_SH * ARK_CEL + 6.0, "_", 1.0, 0xFFFFFFFF);
        gArkInfoTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_SH * ARK_CEL + 18.0, T(playerid, "W/A/S/D - smjer   ENTER - izlaz"), 0.8, 0xAAAAAAFF);
        gArk[m][amSmjer] = 1;
        gArk[m][amRep] = 0;
        gArk[m][amDuz] = 3;
        gArk[m][amMs] = 170;
        for (new i = 0; i < 3; i++)
        {
            new c = (ARK_SH / 2) * ARK_SW + 3 + i;
            gArkZmija[m][i] = c;
            gArkPolje[m][c] = true;
            ArkCelija(playerid, c, i == 2 ? 0xB6FF6AFF : 0x4CD137FF);
        }
        ArkHrana(m);
    }
    else
    {
        ArkKutija(playerid, 320.0 - 88.0, ARK_Y - 34.0, 176.0, 194.0, 0x14152AE6);
        for (new c = 0; c < 9; c++) ArkKutija(playerid, 320.0 - 75.0 + float(c % 3) * 52.0, ARK_Y + float(c / 3) * 40.0, 46.0, 34.0, 0x2C2F45FF, true);
        ArkTekst(playerid, 320.0, ARK_Y - 30.0, "REAKCIJA", 1.6, 0x00E5FFFF);
        gArkBodTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + 124.0, "_", 1.0, 0xFFFFFFFF);
        gArkInfoTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + 138.0, T(playerid, "Kliknite svijetli kvadrat   ESC - izlaz"), 0.8, 0xAAAAAAFF);
        gArk[m][amZivoti] = 3;
        gArk[m][amMeta] = -1;
        SelectTextDraw(playerid, 0x00E5FFFF);
        ArkMeta(m);
    }
    ArkBodovi(m);
    if (gArk[m][amIgra] == AI_ZMIJA) SetTimerEx("ArkZmijaTick", gArk[m][amMs], false, "ii", m, gArk[m][amToken]);
}

static ArkHrana(m)
{
    new slobodno = ARK_CELIJA - gArk[m][amDuz], r = random(slobodno);
    for (new c = 0; c < ARK_CELIJA; c++)
    {
        if (gArkPolje[m][c] || r-- > 0) continue;
        gArk[m][amHrana] = c;
        ArkCelija(gArk[m][amIgrac], c, 0xFF4757FF);
        return;
    }
    gArk[m][amHrana] = -1;
}

forward ArkZmijaTick(m, token);
public ArkZmijaTick(m, token)
{
    if (token != gArk[m][amToken] || gArk[m][amKraj]) return;
    new p = gArk[m][amIgrac], KEY:tipke, gore, strana, s = gArk[m][amSmjer], n = -1;
    GetPlayerKeys(p, tipke, gore, strana);
    if (gore < 0) n = 0;
    else if (gore > 0) n = 2;
    else if (strana < 0) n = 3;
    else if (strana > 0) n = 1;
    if (n != -1 && n != (s + 2) % 4) s = gArk[m][amSmjer] = n;
    new glava = gArkZmija[m][(gArk[m][amRep] + gArk[m][amDuz] - 1) % ARK_CELIJA], x = glava % ARK_SW + gArkSmjerX[s], y = glava / ARK_SW + gArkSmjerY[s];
    if (x < 0 || x >= ARK_SW || y < 0 || y >= ARK_SH)
    {
        ArkKraj(m);
        return;
    }
    new c = y * ARK_SW + x, bool:jede = c == gArk[m][amHrana];
    if (!jede)
    {
        new rep = gArkZmija[m][gArk[m][amRep]];
        gArkPolje[m][rep] = false;
        ArkCelija(p, rep, 0x10301AFF);
        gArk[m][amRep] = (gArk[m][amRep] + 1) % ARK_CELIJA;
        gArk[m][amDuz]--;
    }
    if (gArkPolje[m][c])
    {
        ArkKraj(m);
        return;
    }
    ArkCelija(p, glava, 0x4CD137FF);
    gArkZmija[m][(gArk[m][amRep] + gArk[m][amDuz]) % ARK_CELIJA] = c;
    gArk[m][amDuz]++;
    gArkPolje[m][c] = true;
    ArkCelija(p, c, 0xB6FF6AFF);
    if (jede)
    {
        gArk[m][amBod] += 10;
        if (gArk[m][amMs] > 75) gArk[m][amMs] -= 5;
        PlayerPlaySound(p, 1056, 0.0, 0.0, 0.0);
        ArkBodovi(m);
        if (gArk[m][amDuz] >= ARK_CELIJA)
        {
            ArkKraj(m);
            return;
        }
        ArkHrana(m);
    }
    SetTimerEx("ArkZmijaTick", gArk[m][amMs], false, "ii", m, token);
}

static ArkMeta(m)
{
    new p = gArk[m][amIgrac], stara = gArk[m][amMeta], nova = random(8);
    if (nova >= stara && stara != -1) nova++;
    if (stara != -1) ArkCelija(p, stara, 0x2C2F45FF);
    gArk[m][amMeta] = nova;
    ArkCelija(p, nova, 0x00E5FFFF);
    gArk[m][amMs] = 1100 - gArk[m][amBod] * 25;
    if (gArk[m][amMs] < 380) gArk[m][amMs] = 380;
    SetTimerEx("ArkReakcijaIstek", gArk[m][amMs], false, "ii", m, ++gArk[m][amToken]);
}

static ArkPromasaj(m)
{
    new p = gArk[m][amIgrac];
    PlayerPlaySound(p, 1055, 0.0, 0.0, 0.0);
    if (--gArk[m][amZivoti] <= 0)
    {
        ArkBodovi(m);
        ArkKraj(m);
        return;
    }
    ArkBodovi(m);
    ArkMeta(m);
}

forward ArkReakcijaIstek(m, token);
public ArkReakcijaIstek(m, token)
{
    if (token != gArk[m][amToken] || gArk[m][amKraj]) return;
    ArkPromasaj(m);
}

bool:ArkadaKlik(playerid, PlayerText:td)
{
    new m = gArkAutomat[playerid];
    if (m == -1 || gArk[m][amIgra] != AI_REAKCIJA) return false;
    if (gArk[m][amKraj]) return true;
    for (new c = 0; c < 9; c++)
    {
        if (td != gArkTd[playerid][c + 1]) continue;
        if (c != gArk[m][amMeta]) ArkPromasaj(m);
        else
        {
            gArk[m][amBod]++;
            PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
            ArkBodovi(m);
            ArkMeta(m);
        }
        break;
    }
    return true;
}

static ArkKraj(m)
{
    if (gArk[m][amKraj]) return;
    gArk[m][amKraj] = true;
    gArk[m][amToken]++;
    new p = gArk[m][amIgrac], tekst[512], red[64];
    if (gArk[m][amBod] > 0) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO arcade_scores (machine, account, score, created) VALUES (%d, %d, %d, %d)", gArk[m][amDb], gAccountId[p], gArk[m][amBod], gettime()));
    format(tekst, sizeof tekst, "~r~%s~n~~w~%s: ~y~%d~n~~n~~w~TOP 5", T(p, "KRAJ IGRE"), T(p, "BODOVI"), gArk[m][amBod]);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT replace(a.name, '_', ' '), MAX(s.score) AS best FROM arcade_scores s JOIN accounts a ON a.id = s.account WHERE s.machine = %d GROUP BY s.account ORDER BY best DESC LIMIT 5", gArk[m][amDb]);
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        new ime[MAX_PLAYER_NAME + 1];
        DB_GetFieldString(res, 0, ime, sizeof ime);
        TdCisti(ime);
        format(red, sizeof red, "~n~~w~%d. %s ~y~%d", i + 1, ime, DB_GetFieldInt(res, 1));
        strcat(tekst, red);
    }
    DB_FreeResultSet(res);
    ArkKutija(p, 320.0 - 90.0, ARK_Y - 4.0, 180.0, 128.0, 0x000000E6);
    ArkTekst(p, 320.0, ARK_Y, tekst, 1.0, 0xFFFFFFFF);
    SetTimerEx("ArkZatvoriTimer", ARK_KRAJ_MS, false, "ii", m, gArk[m][amToken]);
}

forward ArkZatvoriTimer(m, token);
public ArkZatvoriTimer(m, token)
{
    if (token == gArk[m][amToken] && gArk[m][amIgrac] != INVALID_PLAYER_ID) ArkZatvori(gArk[m][amIgrac]);
}

ArkZatvori(playerid)
{
    new m = gArkAutomat[playerid];
    if (m == -1) return;
    gArkAutomat[playerid] = -1;
    gArk[m][amIgrac] = INVALID_PLAYER_ID;
    gArk[m][amToken]++;
    for (new i = 0; i < gArkBr[playerid]; i++) PlayerTextDrawDestroy(playerid, gArkTd[playerid][i]);
    gArkBr[playerid] = 0;
    if (!IsPlayerConnected(playerid)) return;
    if (gArk[m][amIgra] == AI_REAKCIJA) CancelSelectTextDraw(playerid);
    TogglePlayerControllable(playerid, true);
}

ArkadaEsc(playerid)
{
    new m = gArkAutomat[playerid];
    if (m == -1) return;
    if (gArk[m][amKraj]) ArkZatvori(playerid);
    else ArkKraj(m);
}

bool:ArkadaTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (gArkAutomat[playerid] == -1) return false;
    if ((newkeys & KEY_SECONDARY_ATTACK) && !(oldkeys & KEY_SECONDARY_ATTACK)) ArkadaEsc(playerid);
    return true;
}

ShowArkade(playerid)
{
    new list[2048], red[96], len = 0, n = 0;
    for (new g = 0; g < sizeof gArkIme; g++)
    {
        format(red, sizeof red, T(playerid, "+ Novi automat: %s (ispred mene)"), T(playerid, gArkIme[g]));
        DodajStupac(list, len, red);
    }
    for (new m = 0; m < ARK_MAX; m++)
    {
        if (!gArk[m][amIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] %s (%s) - obrisi"), gArk[m][amDb], T(playerid, gArkIme[gArk[m][amIgra]]), CestaZona(gArk[m][amX], gArk[m][amY]));
        DodajStupac(list, len, red);
        gApArk[playerid][n++] = m;
    }
    return PrikaziDialog(playerid, DLG_APANEL_ARKADA, DIALOG_STYLE_LIST, T(playerid, "Arkade"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_arkada(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem < sizeof gArkIme)
    {
        new m = 0, Float:x, Float:y, Float:z, Float:a;
        while (m < ARK_MAX && gArk[m][amIma]) m++;
        if (m == ARK_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj automata.")), ShowArkade(playerid);
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        x -= floatsin(a, degrees) * 1.0;
        y += floatcos(a, degrees) * 1.0;
        z -= 1.0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO arcade_machines (x, y, z, a, interior, world, game) VALUES (%f, %f, %f, %f, %d, %d, %d)", x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid), listitem));
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT last_insert_rowid()");
        gArk[m][amDb] = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        gArk[m][amX] = x;
        gArk[m][amY] = y;
        gArk[m][amZ] = z;
        gArk[m][amA] = a;
        gArk[m][amInt] = GetPlayerInterior(playerid);
        gArk[m][amSvijet] = GetPlayerVirtualWorld(playerid);
        gArk[m][amIgra] = listitem;
        ArkStvori(m);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Automat je postavljen."));
        return ShowArkade(playerid);
    }
    new i = listitem - sizeof gArkIme;
    if (i < 0 || i >= ARK_MAX) return 1;
    new m = gApArk[playerid][i];
    if (!gArk[m][amIma]) return ShowArkade(playerid);
    if (gArk[m][amIgrac] != INVALID_PLAYER_ID) ArkZatvori(gArk[m][amIgrac]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM arcade_machines WHERE id = %d", gArk[m][amDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM arcade_scores WHERE machine = %d", gArk[m][amDb]));
    DestroyDynamicObject(gArk[m][amObj]);
    DestroyDynamic3DTextLabel(gArk[m][amTekst]);
    gArk[m][amIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Automat je obrisan."));
    return ShowArkade(playerid);
}
