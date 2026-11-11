#define MAX_TER             16
#define MAX_TER_NPC         4
#define TER_NPC_RANGE       3.0
#define TER_OBRANA_RANGE    60.0
#define TER_PRIBLIZI_SE     12.0
#define TER_NAPAD_MS        60000
#define TER_MIN_NAPADACA    3
#define TER_ODABRANIH       2
#define TER_BLIZINA         100.0
#define TER_ZASTITA_MS      60000
#define TER_UDARAC          1.5
#define TER_RAST_MS         300000
#define TER_RAST_SJEMENKI   5
#define TER_GRAMA_PO_SJEMENKI 10
#define TER_MAX_ZALIHA      5000
#define TER_MAX_CIJENA      100000

enum { TER_CRACKHOUSE, TER_PLANTAZA }
static const gTerTip[2][] = { "Crackhouse", "Plantaza" };

enum E_TER
{
    terDb, terIme[32], terTip, Float:terMinX, Float:terMinY, Float:terMaxX, Float:terMaxY,
    terOrg, terZaliha, terCijena, terProfit, terSjemenke, terOruzje, terMunicija,
    terZona, terNapadac, terNapadKraj, terZastitaKraj
}
static gTer[MAX_TER][E_TER];
static gTerNpc[MAX_TER][MAX_TER_NPC];
static gTerNpcDb[MAX_TER][MAX_TER_NPC];
static Float:gTerNpcPos[MAX_TER][MAX_TER_NPC][4];
static gTerNpcSkin[MAX_TER][MAX_TER_NPC];
static gNpcTer[MAX_PLAYERS] = { -1, ... };
static gTerOdabran[MAX_PLAYERS];
static gTerPopis[MAX_PLAYERS][BROJ_ITEMA];
static gTerPorukaTick[MAX_PLAYERS];
static gTerAdminPopis[MAX_PLAYERS][MAX_TER];
static gTerOdabrani[MAX_TER][TER_ODABRANIH];
static gTerOdabraniAcc[MAX_TER][TER_ODABRANIH];
static gTerUmro[MAX_PLAYERS];
static gTipPamti[MAX_PLAYERS][2];
static bool:gTerMrtav[MAX_TER][MAX_TER_NPC];
static gTerNpcMunicija[MAX_TER][MAX_TER_NPC];

InitTeritorij()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_colors (org INTEGER PRIMARY KEY, color INTEGER NOT NULL)"));
    for (new t = 0; t < MAX_TER; t++) for (new n = 0; n < MAX_TER_NPC; n++) gTerNpc[t][n] = INVALID_PLAYER_ID;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS territories (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, type INTEGER NOT NULL,\
        minx REAL NOT NULL, miny REAL NOT NULL, maxx REAL NOT NULL, maxy REAL NOT NULL,\
        org INTEGER NOT NULL DEFAULT 0, stock INTEGER NOT NULL DEFAULT 0, price INTEGER NOT NULL DEFAULT 50,\
        profit INTEGER NOT NULL DEFAULT 0, seeds INTEGER NOT NULL DEFAULT 0, weapon INTEGER NOT NULL DEFAULT -1, ammo INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS territory_npcs (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, territory INTEGER NOT NULL,\
        x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, skin INTEGER NOT NULL)"));

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, name, type, minx, miny, maxx, maxy, org, stock, price, profit, seeds, weapon, ammo FROM territories ORDER BY id LIMIT %d", MAX_TER);
    new t = 0;
    if (DB_GetRowCount(res)) do
    {
        gTer[t][terDb] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, gTer[t][terIme], 32);
        gTer[t][terTip] = DB_GetFieldInt(res, 2);
        gTer[t][terMinX] = DB_GetFieldFloat(res, 3);
        gTer[t][terMinY] = DB_GetFieldFloat(res, 4);
        gTer[t][terMaxX] = DB_GetFieldFloat(res, 5);
        gTer[t][terMaxY] = DB_GetFieldFloat(res, 6);
        gTer[t][terOrg] = DB_GetFieldInt(res, 7);
        gTer[t][terZaliha] = DB_GetFieldInt(res, 8);
        gTer[t][terCijena] = DB_GetFieldInt(res, 9);
        gTer[t][terProfit] = DB_GetFieldInt(res, 10);
        gTer[t][terSjemenke] = DB_GetFieldInt(res, 11);
        gTer[t][terOruzje] = DB_GetFieldInt(res, 12);
        gTer[t][terMunicija] = DB_GetFieldInt(res, 13);
        gTer[t][terZona] = GangZoneCreate(gTer[t][terMinX], gTer[t][terMinY], gTer[t][terMaxX], gTer[t][terMaxY]);
        t++;
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);

    for (t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb]) continue;
        res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, skin FROM territory_npcs WHERE territory = %d ORDER BY id LIMIT %d", gTer[t][terDb], MAX_TER_NPC);
        new n = 0;
        if (DB_GetRowCount(res)) do
        {
            gTerNpcDb[t][n] = DB_GetFieldInt(res, 0);
            for (new i = 0; i < 4; i++) gTerNpcPos[t][n][i] = DB_GetFieldFloat(res, 1 + i);
            gTerNpcSkin[t][n] = DB_GetFieldInt(res, 5);
            KreirajNPCa(t, n++);
        } while (DB_SelectNextRow(res));
        DB_FreeResultSet(res);
    }
    SetTimer("TeritorijRast", TER_RAST_MS, true);
}

GetOrgBoja(org)
{
    if (!org) return 0xFFFFFF40;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT color FROM org_colors WHERE org = %d", org);
    new boja = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : (((org * 0x9E3779B1) & 0xFFFFFF00) | 0x80);
    DB_FreeResultSet(res);
    return boja;
}

OrgPostaviBoju(playerid, org, const unos[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new pocetak = unos[0] == '#' ? 1 : 0, n = strlen(unos), hex = 0;
    if (n - pocetak != 6) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Boja mora biti 6 hex znakova (0-9, A-F)."));
    for (new i = pocetak; i < n; i++)
    {
        new c = unos[i], z;
        if (c >= '0' && c <= '9') z = c - '0';
        else if (c >= 'a' && c <= 'f') z = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') z = c - 'A' + 10;
        else return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Boja mora biti 6 hex znakova (0-9, A-F)."));
        hex = (hex << 4) | z;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO org_colors (org, color) VALUES (%d, %d)", org, (hex << 8) | 0x80));
    for (new t = 0; t < MAX_TER; t++) if (gTer[t][terDb] && gTer[t][terOrg] == org) ShowZonu(t);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Boja organizacije je promijenjena."));
}

TeritorijPrijava(playerid)
{
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb]) continue;
        GangZoneShowForPlayer(playerid, gTer[t][terZona], GetOrgBoja(gTer[t][terOrg]));
        if (gTer[t][terNapadac]) GangZoneFlashForPlayer(playerid, gTer[t][terZona], 0xFF000080);
    }
}

static SaveTeritorij(t)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE territories SET org = %d, stock = %d, price = %d, profit = %d, seeds = %d, weapon = %d, ammo = %d WHERE id = %d",
        gTer[t][terOrg], gTer[t][terZaliha], gTer[t][terCijena], gTer[t][terProfit], gTer[t][terSjemenke], gTer[t][terOruzje], gTer[t][terMunicija], gTer[t][terDb]));
}

static ShowZonu(t)
{
    GangZoneStopFlashForAll(gTer[t][terZona]);
    GangZoneShowForAll(gTer[t][terZona], GetOrgBoja(gTer[t][terOrg]));
}

static TerPoDb(dbid)
{
    for (new t = 0; t < MAX_TER; t++) if (dbid && gTer[t][terDb] == dbid) return t;
    return -1;
}

static DrogaTeritorija(t)
{
    return gTer[t][terTip] == TER_CRACKHOUSE ? ITEM_KOKAIN : ITEM_MARIHUANA;
}

static DilerPostavi(npc, t, n)
{
    gTerMrtav[t][n] = false;
    NPC_SetSkin(npc, gTerNpcSkin[t][n]);
    NPC_SetPos(npc, gTerNpcPos[t][n][0], gTerNpcPos[t][n][1], gTerNpcPos[t][n][2]);
    NPC_SetFacingAngle(npc, gTerNpcPos[t][n][3]);
    NPC_SetHealth(npc, 100.0);
    NPC_SetInvulnerable(npc, true);
    NPC_SetWeapon(npc, WEAPON_FIST);
}

static DilerNaoruzaj(npc, t, n)
{
    new item = gTer[t][terOruzje];
    NPC_SetInvulnerable(npc, false);
    if (item == -1 || !gTerNpcMunicija[t][n]) return;
    new WEAPON:oruzje = WEAPON:gItems[item][ITEM_WEAPON], metaka = gTerNpcMunicija[t][n], klip = NPC_GetWeaponClipSize(npc, oruzje);
    NPC_SetAmmo(npc, metaka);
    NPC_SetWeapon(npc, oruzje);
    NPC_SetAmmoInClip(npc, klip > 0 && klip < metaka ? klip : metaka);
    NPC_EnableReloading(npc, true);
}

static KreirajNPCa(t, n)
{
    new name[MAX_PLAYER_NAME + 1];
    format(name, sizeof name, "Diler_%d_%d", gTer[t][terDb], gTerNpcDb[t][n]);
    new npc = NPC_Create(name);
    if (npc == INVALID_PLAYER_ID) return;
    gTerNpc[t][n] = npc;
    gNpcTer[npc] = t * MAX_TER_NPC + n;
    NPC_Spawn(npc);
}

static ObrisiNPCa(t, n)
{
    new npc = gTerNpc[t][n];
    if (npc == INVALID_PLAYER_ID) return;
    gNpcTer[npc] = -1;
    NPC_Destroy(npc);
    gTerNpc[t][n] = INVALID_PLAYER_ID;
}

static MakniDilera(t, n)
{
    ObrisiNPCa(t, n);
    gTerNpcDb[t][n] = 0;
}

public OnNPCSpawn(npcid)
{
    new k = gNpcTer[npcid];
    if (k == -1)
    {
        TaksiSpawn(npcid);
        KurvaSpawn(npcid);
        SlucajSpawn(npcid);
        HeliSpawn(npcid);
        VlakSpawn(npcid);
        return 1;
    }
    DilerPostavi(npcid, k / MAX_TER_NPC, k % MAX_TER_NPC);
    return 1;
}

static ZiviDileri(t)
{
    new count = 0;
    for (new n = 0; n < MAX_TER_NPC; n++) if (gTerNpcDb[t][n] && !gTerMrtav[t][n]) count++;
    return count;
}

static SendNapad(t, const poruka[], const a[] = "", const b[] = "", const c[] = "")
{
    new inner[144];
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || !gUlogiran[i] || !gOrg[i] || (gOrg[i] != gTer[t][terOrg] && gOrg[i] != gTer[t][terNapadac])) continue;
        format(inner, sizeof inner, T(i, poruka), a, b, c);
        SendClientMessage(i, COLOR_ORG, T(i, "[Teritorij %s] %s"), gTer[t][terIme], inner);
    }
}

static bool:IsBanda(playerid)
{
    new org = gOrg[playerid];
    if (!gUlogiran[playerid] || !org) return false;
    if (gTipPamti[playerid][0] != org)
    {
        gTipPamti[playerid][0] = org;
        gTipPamti[playerid][1] = GetOrgTip(org);
    }
    return gTipPamti[playerid][1] < 2;
}

static bool:IsUZoni(playerid, t, Float:rub = 0.0)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    return x >= gTer[t][terMinX] - rub && x <= gTer[t][terMaxX] + rub && y >= gTer[t][terMinY] - rub && y <= gTer[t][terMaxY] + rub;
}

static ClanoviUZoni(t, org, ids[], osim = INVALID_PLAYER_ID)
{
    new count = 0;
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (i != osim && gUlogiran[i] && gOrg[i] == org && GetPlayerState(i) != PLAYER_STATE_WASTED && IsUZoni(i, t)) ids[count++] = i;
    return count;
}

TeritorijConnect(playerid)
{
    gTerUmro[playerid] = 0;
    gTipPamti[playerid][0] = -1;
}

static StartNapad(t, org, const ids[], count)
{
    new alive = ZiviDileri(t), po = alive && gTer[t][terOruzje] != -1 ? gTer[t][terMunicija] / alive : 0;
    gTer[t][terNapadac] = org;
    gTer[t][terNapadKraj] = GetTickCount() + TER_NAPAD_MS;
    new a = random(count), b = random(count - 1);
    if (b >= a) b++;
    gTerOdabrani[t][0] = ids[a];
    gTerOdabrani[t][1] = ids[b];
    for (new i = 0; i < TER_ODABRANIH; i++) gTerOdabraniAcc[t][i] = gAccountId[gTerOdabrani[t][i]];
    GangZoneFlashForAll(gTer[t][terZona], 0xFF000080);
    for (new n = 0; n < MAX_TER_NPC; n++)
    {
        gTerNpcMunicija[t][n] = 0;
        NoviPolozaj(t, n);
        if (!gTerNpcDb[t][n] || gTerMrtav[t][n] || gTerNpc[t][n] == INVALID_PLAYER_ID) continue;
        gTerNpcMunicija[t][n] = po;
        gTer[t][terMunicija] -= po;
        DilerNaoruzaj(gTerNpc[t][n], t, n);
    }
    new sek[8];
    valstr(sek, TER_NAPAD_MS / 1000);
    SendNapad(t, "Napad je poceo! %s i %s moraju ostati zivi na teritoriju %s s.", GetRPIme(gTerOdabrani[t][0]), GetRPIme(gTerOdabrani[t][1]), sek);
}

static FinishNapad(t, bool:zauzet)
{
    for (new n = 0; n < MAX_TER_NPC; n++)
    {
        new npc = gTerNpc[t][n];
        if (npc == INVALID_PLAYER_ID) continue;
        if (!gTerMrtav[t][n] && NPC_GetWeapon(npc) != WEAPON_FIST) gTer[t][terMunicija] += NPC_GetAmmo(npc);
        NPC_StopMove(npc);
        NPC_StopAim(npc);
        if (gTerMrtav[t][n]) NPC_Respawn(npc);
        DilerPostavi(npc, t, n);
    }
    if (zauzet)
    {
        SendNapad(t, "Napadaci su zauzeli teritorij.");
        gTer[t][terOrg] = gTer[t][terNapadac];
    }
    else SendNapad(t, "Napad je propao.");
    for (new i = 0; i < MAX_PLAYERS; i++) if (gTerUmro[i] == t + 1) gTerUmro[i] = 0;
    gTer[t][terNapadac] = 0;
    gTer[t][terZastitaKraj] = GetTickCount() + TER_ZASTITA_MS;
    ShowZonu(t);
    SaveTeritorij(t);
}

static bool:OdabraniNaMjestu(t, i)
{
    new p = gTerOdabrani[t][i];
    return IsPlayerConnected(p) && gUlogiran[p] && gAccountId[p] == gTerOdabraniAcc[t][i] && gOrg[p] == gTer[t][terNapadac]
        && GetPlayerState(p) != PLAYER_STATE_WASTED && IsUZoni(p, t);
}

static Obavijesti(playerid, const poruka[])
{
    if (GetTickCount() - gTerPorukaTick[playerid] < 3000) return;
    gTerPorukaTick[playerid] = GetTickCount();
    SendClientMessage(playerid, COLOR_ERROR, poruka);
}

bool:IsShootingBlocked(playerid)
{
    new u = gTerUmro[playerid];
    if (u && gTer[u - 1][terNapadac] && IsUZoni(playerid, u - 1, TER_BLIZINA))
    {
        Obavijesti(playerid, "Poginuli ste u napadu: ne mozete pucati blizu teritorija dok napad traje.");
        return true;
    }
    if (!IsBanda(playerid)) return false;
    new ids[MAX_PLAYERS];
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb] || !gTer[t][terOrg] || gTer[t][terOrg] == gOrg[playerid] || !IsUZoni(playerid, t)) continue;
        if (ClanoviUZoni(t, gOrg[playerid], ids, playerid)) return false;
        Obavijesti(playerid, "Na neprijateljskom teritoriju ne mozete pucati bez barem jos jednog clana.");
        return true;
    }
    return false;
}

TeritorijDeath(playerid)
{
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb] || !gTer[t][terNapadac] || gOrg[playerid] != gTer[t][terNapadac]) continue;
        gTerUmro[playerid] = t + 1;
        for (new i = 0; i < TER_ODABRANIH; i++)
        {
            if (gTerOdabrani[t][i] != playerid || gTerOdabraniAcc[t][i] != gAccountId[playerid]) continue;
            SendNapad(t, "%s je ubijen.", GetRPIme(playerid));
            FinishNapad(t, false);
            break;
        }
    }
}

DilerPogodak(playerid, npcid)
{
    new k = gNpcTer[npcid];
    if (k == -1 || IsShootingBlocked(playerid)) return 1;
    new t = k / MAX_TER_NPC;
    if (!gTer[t][terNapadac])
    {
        new org = gOrg[playerid];
        if (!IsBanda(playerid) || org == gTer[t][terOrg]) return 1;
        if (GetTickCount() - gTer[t][terZastitaKraj] < 0)
        {
            new msg[96];
            format(msg, sizeof msg, T(playerid, "Teritorij %s se moze napasti tek za %d s."), gTer[t][terIme], (gTer[t][terZastitaKraj] - GetTickCount()) / 1000 + 1);
            Obavijesti(playerid, msg);
            return 1;
        }
        new ids[MAX_PLAYERS], count = ClanoviUZoni(t, org, ids);
        if (count < TER_MIN_NAPADACA)
        {
            new msg[96];
            format(msg, sizeof msg, T(playerid, "Za napad treba najmanje %d clana organizacije na teritoriju."), TER_MIN_NAPADACA);
            Obavijesti(playerid, msg);
            return 1;
        }
        StartNapad(t, org, ids, count);
    }
    return 1;
}

public OnNPCDeath(npcid, killerid, WEAPON:reason)
{
    new k = gNpcTer[npcid];
    TaksiNpcDead(npcid);
    if (k != -1) gTerMrtav[k / MAX_TER_NPC][k % MAX_TER_NPC] = true;
    return 1;
}

static GetNearestNapadac(t, Float:x, Float:y, Float:z)
{
    new best = INVALID_PLAYER_ID, Float:bestDist = TER_OBRANA_RANGE;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i] || gOrg[i] != gTer[t][terNapadac] || GetPlayerState(i) == PLAYER_STATE_WASTED || IsRanjen(i)) continue;
        new Float:d = GetPlayerDistanceFromPoint(i, x, y, z);
        if (d < bestDist) { bestDist = d; best = i; }
    }
    return best;
}

static Float:gTerKut[MAX_TER][MAX_TER_NPC];
static Float:gTerRazmak[MAX_TER][MAX_TER_NPC];

static NoviPolozaj(t, n)
{
    gTerKut[t][n] = float(n * (360 / MAX_TER_NPC) + random(70) - 35);
    gTerRazmak[t][n] = float(6 + random(7));
}

static IdiPremaMetu(npc, t, n, target, bool:saka, NPC_MOVE_TYPE:nacin)
{
    new Float:tx, Float:ty, Float:tz, Float:r = saka ? 0.8 : gTerRazmak[t][n];
    GetPlayerPos(target, tx, ty, tz);
    NPCHodaj(npc, tx + r * floatcos(gTerKut[t][n], degrees), ty + r * floatsin(gTerKut[t][n], degrees), tz, nacin, NPC_MOVE_SPEED_AUTO, 0.5);
}

static VodiDilera(t, n)
{
    new npc = gTerNpc[t][n], Float:x, Float:y, Float:z;
    if (npc == INVALID_PLAYER_ID || gTerMrtav[t][n]) return;
    if (NPC_GetWeapon(npc) != WEAPON_FIST && !NPC_GetAmmo(npc)) NPC_SetWeapon(npc, WEAPON_FIST);
    new bool:saka = NPC_GetWeapon(npc) == WEAPON_FIST;
    new target = GetNearestNapadac(t, gTerNpcPos[t][n][0], gTerNpcPos[t][n][1], gTerNpcPos[t][n][2]);
    NPC_GetPos(npc, x, y, z);
    if (target == INVALID_PLAYER_ID)
    {
        NPC_StopAim(npc);
        if (!NPC_IsMoving(npc) && VectorSize(x - gTerNpcPos[t][n][0], y - gTerNpcPos[t][n][1], z - gTerNpcPos[t][n][2]) > 2.0)
            NPCHodaj(npc, gTerNpcPos[t][n][0], gTerNpcPos[t][n][1], gTerNpcPos[t][n][2], NPC_MOVE_TYPE_JOG);
        return;
    }
    new Float:d = GetPlayerDistanceFromPoint(target, x, y, z);
    if (saka)
    {
        if (d > TER_UDARAC)
        {
            IdiPremaMetu(npc, t, n, target, true, NPC_MOVE_TYPE_SPRINT);
            return;
        }
        if (NPC_IsMoving(npc)) NPC_StopMove(npc);
        NPC_SetAngleToPlayer(npc, target);
        if (!NPC_IsMeleeAttacking(npc)) NPC_MeleeAttack(npc, 600 + random(500));
        return;
    }
    if (!random(4)) NoviPolozaj(t, n);
    if (d > gTerRazmak[t][n] + 4.0) IdiPremaMetu(npc, t, n, target, false, NPC_MOVE_TYPE_SPRINT);
    else if (!NPC_IsMoving(npc) && !random(3)) IdiPremaMetu(npc, t, n, target, false, NPC_MOVE_TYPE_JOG);
    if (!NPC_IsAimingAtPlayer(npc, target)) NPC_AimAtPlayer(npc, target, true, 300 + random(400));
}

TeritorijTick()
{
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb] || !gTer[t][terNapadac]) continue;
        new bool:propao = false;
        for (new i = 0; i < TER_ODABRANIH; i++) if (!OdabraniNaMjestu(t, i)) propao = true;
        if (propao)
        {
            SendNapad(t, "Odabrani napadac je poginuo ili napustio teritorij.");
            FinishNapad(t, false);
            continue;
        }
        if (GetTickCount() - gTer[t][terNapadKraj] >= 0) { FinishNapad(t, true); continue; }
        for (new n = 0; n < MAX_TER_NPC; n++) VodiDilera(t, n);
    }
}

forward TeritorijRast();
public TeritorijRast()
{
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb] || gTer[t][terTip] != TER_PLANTAZA || !gTer[t][terSjemenke]) continue;
        new s = min(gTer[t][terSjemenke], TER_RAST_SJEMENKI);
        gTer[t][terSjemenke] -= s;
        gTer[t][terZaliha] = min(gTer[t][terZaliha] + s * TER_GRAMA_PO_SJEMENKI, TER_MAX_ZALIHA);
        SaveTeritorij(t);
    }
}

TeritorijOrgObrisana(org)
{
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb]) continue;
        if (gTer[t][terNapadac] == org || (gTer[t][terNapadac] && gTer[t][terOrg] == org)) FinishNapad(t, false);
        if (gTer[t][terOrg] != org) continue;
        gTer[t][terOrg] = 0;
        ShowZonu(t);
        SaveTeritorij(t);
    }
}

static NpcBlizu(playerid)
{
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb] || gTer[t][terNapadac]) continue;
        for (new n = 0; n < MAX_TER_NPC; n++)
            if (gTerNpcDb[t][n] && !gTerMrtav[t][n] && gTerNpc[t][n] != INVALID_PLAYER_ID
                && IsPlayerInRangeOfPoint(playerid, TER_NPC_RANGE, gTerNpcPos[t][n][0], gTerNpcPos[t][n][1], gTerNpcPos[t][n][2])) return t * MAX_TER_NPC + n;
    }
    return -1;
}

DilerKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new k = NpcBlizu(playerid);
    if (k == -1) return 0;
    new t = k / MAX_TER_NPC, item = DrogaTeritorija(t), body[144];
    format(body, sizeof body, T(playerid, "%s: $%d/%s, na zalihi %d. Unesite kolicinu:"), T(playerid, gItems[item][ITEM_NAME]), gTer[t][terCijena], T(playerid, gItems[item][ITEM_UNIT]), gTer[t][terZaliha]);
    PrikaziDialog(playerid, DLG_DILER, DIALOG_STYLE_INPUT, T(playerid, "Diler"), body, T(playerid, "Kupi"), T(playerid, "Zatvori"));
    return 1;
}

CMD:diler(playerid, params[])
{
    if (gUlogiran[playerid] && !DilerKey(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod dilera."));
    return 1;
}

DLG:diler(playerid, response, const inputtext[])
{
    if (!response) return 1;
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 5)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna kolicina."));
        return DilerKey(playerid);
    }
    return DilerKupi(playerid, strval(inputtext));
}

static DilerKupi(playerid, amount)
{
    new k = NpcBlizu(playerid);
    if (k == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod dilera."));
    new t = k / MAX_TER_NPC, item = DrogaTeritorija(t);
    if (!amount || amount > gTer[t][terZaliha]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Diler nema toliko (zaliha: %d)."), gTer[t][terZaliha]);
    new cost = amount * gTer[t][terCijena];
    if (gNovac[playerid] < cost) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca ($%d)."), cost);
    if (!GiveItem(playerid, item, amount)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
    DajNovac(playerid, -cost);
    gTer[t][terZaliha] -= amount;
    gTer[t][terProfit] = min(gTer[t][terProfit] + cost, MAX_KES);
    SaveTeritorij(t);
    new n = k % MAX_TER_NPC, Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new Float:kut = atan2(gTerNpcPos[t][n][1] - y, gTerNpcPos[t][n][0] - x) - 90.0;
    SetPlayerFacingAngle(playerid, kut);
    NPC_SetFacingAngle(gTerNpc[t][n], kut + 180.0);
    ApplyAnimation(playerid, "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0, SYNC_ALL);
    NPC_ApplyAnimation(gTerNpc[t][n], "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0);
    SetTimerEx("DilerDone", 3000, false, "i", k);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d %s %s za $%d."), amount, T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]), cost);
}

forward DilerDone(k);
public DilerDone(k)
{
    new t = k / MAX_TER_NPC, n = k % MAX_TER_NPC;
    if (gTerNpc[t][n] == INVALID_PLAYER_ID) return;
    NPC_ClearAnimations(gTerNpc[t][n]);
    NPC_SetFacingAngle(gTerNpc[t][n], gTerNpcPos[t][n][3]);
}

static ShowTeritorij(playerid)
{
    new t = gTerOdabran[playerid], title[96], oruzje[64];
    if (gTer[t][terOruzje] == -1) strcat(oruzje, T(playerid, "nema"));
    else format(oruzje, sizeof oruzje, T(playerid, "%s, %d metaka"), gItems[gTer[t][terOruzje]][ITEM_NAME], gTer[t][terMunicija]);
    format(title, sizeof title, T(playerid, "%s | zaliha %d | $%d | profit $%d"), gTer[t][terIme], gTer[t][terZaliha], gTer[t][terCijena], gTer[t][terProfit]);
    new list[256];
    format(list, sizeof list, T(playerid, "%s\nPostavi cijenu\nUzmi profit\nDaj oruzje dilerima\nUzmi oruzje od dilera (%s)"),
        gTer[t][terTip] == TER_CRACKHOUSE ? T(playerid, "Daj kokain dilerima") : T(playerid, "Daj sjemenke za uzgoj"), oruzje);
    return PrikaziDialog(playerid, DLG_TERITORIJ, DIALOG_STYLE_LIST, title, list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

ShowTerOrg(playerid)
{
    new list[1024], row[96], len = 0, count = 0;
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb] || !gOrg[playerid] || gTer[t][terOrg] != gOrg[playerid]) continue;
        format(row, sizeof row, T(playerid, "%s (%s)%s"), gTer[t][terIme], T(playerid, gTerTip[gTer[t][terTip]]), T(playerid, gTer[t][terNapadac] ? (" {FF6347}- pod napadom") : ("")));
        if (DodajStupac(list, len, row)) gTerAdminPopis[playerid][count++] = t;
    }
    if (!count)
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vasa organizacija nema teritorija."));
        return ShowOrgPanel(playerid);
    }
    return PrikaziDialog(playerid, DLG_TER_ORG, DIALOG_STYLE_LIST, T(playerid, "Teritoriji organizacije"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:ter_org(playerid, response, listitem)
{
    if (!response) return ShowOrgPanel(playerid);
    if (listitem < 0 || listitem >= MAX_TER) return 1;
    gTerOdabran[playerid] = gTerAdminPopis[playerid][listitem];
    if (!CanManage(playerid, gTerOdabran[playerid])) return ShowTerOrg(playerid);
    return ShowTeritorij(playerid);
}

static CanManage(playerid, t)
{
    if (!gTer[t][terDb] || gTer[t][terOrg] != gOrg[playerid] || gTer[t][terNapadac])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vise ne mozete upravljati ovim teritorijem."));
        return 0;
    }
    return 1;
}

DLG:teritorij(playerid, response, listitem)
{
    new t = gTerOdabran[playerid];
    if (!response) return ShowTerOrg(playerid);
    if (!CanManage(playerid, t)) return 1;
    switch (listitem)
    {
        case 0: return PrikaziDialog(playerid, DLG_TER_DROGA, DIALOG_STYLE_INPUT, T(playerid, "Zaliha"), T(playerid, "Unesite kolicinu:"), T(playerid, "Daj"), T(playerid, "Natrag"));
        case 1:
        {
            if (!ZaduzenjeProvjera(playerid, PERM_SAFE)) return ShowTeritorij(playerid);
            return PrikaziDialog(playerid, DLG_TER_CIJENA, DIALOG_STYLE_INPUT, T(playerid, "Cijena"), T(playerid, "Unesite cijenu po jedinici ($):"), T(playerid, "Postavi"), T(playerid, "Natrag"));
        }
        case 2:
        {
            if (!ZaduzenjeProvjera(playerid, PERM_SAFE)) return ShowTeritorij(playerid);
            new p = gTer[t][terProfit];
            if (!p || !DajNovac(playerid, p)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema profita ili ne mozete primiti toliko novca."));
            else
            {
                gTer[t][terProfit] = 0;
                SaveTeritorij(t);
                SendClientMessage(playerid, COLOR_ORG, T(playerid, "Uzeli ste $%d profita."), p);
            }
            return ShowTeritorij(playerid);
        }
        case 3:
        {
            new list[512], row[64], len = 0, count = 0;
            for (new i = 0; i < BROJ_ITEMA; i++)
            {
                if (gItems[i][ITEM_AMMO] == -1 || !gInv[playerid][i]) continue;
                format(row, sizeof row, T(playerid, "%s (%d metaka)"), T(playerid, gItems[i][ITEM_NAME]), gInv[playerid][gItems[i][ITEM_AMMO]]);
                if (DodajStupac(list, len, row)) gTerPopis[playerid][count++] = i;
            }
            if (!count)
            {
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate vatreno oruzje u inventaru."));
                return ShowTeritorij(playerid);
            }
            return PrikaziDialog(playerid, DLG_TER_ORUZJE, DIALOG_STYLE_LIST, T(playerid, "Oruzje za dilere"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
        }
        case 4:
        {
            new item = gTer[t][terOruzje];
            if (!ZaduzenjeProvjera(playerid, PERM_SAFE)) return ShowTeritorij(playerid);
            if (item == -1) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dileri nemaju oruzje."));
            else if (gInv[playerid][gItems[item][ITEM_AMMO]] + gTer[t][terMunicija] > gItems[gItems[item][ITEM_AMMO]][ITEM_MAX])
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate mjesta za svu municiju (%d)."), gTer[t][terMunicija]);
            else if (!GiveItem(playerid, item, 1)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi jos oruzja."));
            else
            {
                GiveItem(playerid, gItems[item][ITEM_AMMO], gTer[t][terMunicija]);
                gTer[t][terMunicija] = 0;
                gTer[t][terOruzje] = -1;
                SaveTeritorij(t);
                SendClientMessage(playerid, COLOR_ORG, T(playerid, "Uzeli ste oruzje i municiju od dilera."));
            }
            return ShowTeritorij(playerid);
        }
    }
    return 1;
}

DLG:ter_droga(playerid, response, const inputtext[])
{
    new t = gTerOdabran[playerid];
    if (!response) return ShowTeritorij(playerid);
    if (!CanManage(playerid, t)) return 1;
    new item = gTer[t][terTip] == TER_CRACKHOUSE ? ITEM_KOKAIN : ITEM_SJEME, amount = strval(inputtext);
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 5 || !amount) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna kolicina."));
    else if (item == ITEM_KOKAIN && gTer[t][terZaliha] + amount > TER_MAX_ZALIHA) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dileri mogu imati najvise %d."), TER_MAX_ZALIHA);
    else if (!GiveItem(playerid, item, -amount)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko."));
    else
    {
        if (item == ITEM_KOKAIN) gTer[t][terZaliha] += amount;
        else gTer[t][terSjemenke] += amount;
        SaveTeritorij(t);
    }
    return ShowTeritorij(playerid);
}

DLG:ter_cijena(playerid, response, const inputtext[])
{
    new t = gTerOdabran[playerid];
    if (!response) return ShowTeritorij(playerid);
    if (!CanManage(playerid, t)) return 1;
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 6 || !strval(inputtext) || strval(inputtext) > TER_MAX_CIJENA)
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Cijena mora biti $1-%d."), TER_MAX_CIJENA);
    else
    {
        gTer[t][terCijena] = strval(inputtext);
        SaveTeritorij(t);
    }
    return ShowTeritorij(playerid);
}

DLG:ter_oruzje(playerid, response, listitem)
{
    new t = gTerOdabran[playerid];
    if (!response) return ShowTeritorij(playerid);
    if (!CanManage(playerid, t) || listitem < 0 || listitem >= BROJ_ITEMA) return 1;
    new item = gTerPopis[playerid][listitem];
    if (gTer[t][terOruzje] != -1 && gTer[t][terOruzje] != item)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dileri vec imaju %s. Prvo ga uzmite."), gItems[gTer[t][terOruzje]][ITEM_NAME]);
        return ShowTeritorij(playerid);
    }
    gTerPopis[playerid][0] = item;
    return PrikaziDialog(playerid, DLG_TER_MUNICIJA, DIALOG_STYLE_INPUT, T(playerid, gItems[item][ITEM_NAME]), T(playerid, "Koliko municije dajete dilerima?"), T(playerid, "Daj"), T(playerid, "Natrag"));
}

DLG:ter_municija(playerid, response, const inputtext[])
{
    new t = gTerOdabran[playerid], item = gTerPopis[playerid][0];
    if (!response) return ShowTeritorij(playerid);
    if (!CanManage(playerid, t)) return 1;
    new ammo = gItems[item][ITEM_AMMO], amount = strval(inputtext);
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 5 || !amount || amount > gInv[playerid][ammo] || !gInv[playerid][item])
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko municije ili oruzja."));
    else
    {
        if (gTer[t][terOruzje] == -1)
        {
            GiveItem(playerid, item, -1);
            gTer[t][terOruzje] = item;
        }
        GiveItem(playerid, ammo, -amount);
        gTer[t][terMunicija] += amount;
        SaveTeritorij(t);
        SendClientMessage(playerid, COLOR_ORG, T(playerid, "Dileri sada imaju %s s %d metaka."), T(playerid, gItems[item][ITEM_NAME]), gTer[t][terMunicija]);
    }
    return ShowTeritorij(playerid);
}

CreateTer(playerid, const params[])
{
    new idx = 0, p1[8], p2[8], name[32];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1) || strval(p1) > 1 || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || strval(p2) < 20 || strval(p2) > 300)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [tip: 0 crackhouse, 1 plantaza] [velicina 20-300 m] [ime]"));
    RestParam(params, idx, name);
    RemoveBoje(name);
    if (!name[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite ime teritorija."));
    new t;
    for (t = 0; t < MAX_TER && gTer[t][terDb]; t++) {}
    if (t == MAX_TER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najvise %d teritorija."), MAX_TER);
    new Float:x, Float:y, Float:z, Float:h = float(strval(p2)) / 2.0;
    GetPlayerPos(playerid, x, y, z);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO territories (name, type, minx, miny, maxx, maxy) VALUES ('%q', %d, %f, %f, %f, %f)",
        name, strval(p1), x - h, y - h, x + h, y + h));
    gTer[t][terDb] = ZadnjiId();
    strcopy(gTer[t][terIme], name, 32);
    gTer[t][terTip] = strval(p1);
    gTer[t][terMinX] = x - h; gTer[t][terMinY] = y - h; gTer[t][terMaxX] = x + h; gTer[t][terMaxY] = y + h;
    gTer[t][terOrg] = 0; gTer[t][terZaliha] = 0; gTer[t][terCijena] = 50; gTer[t][terProfit] = 0;
    gTer[t][terSjemenke] = 0; gTer[t][terOruzje] = -1; gTer[t][terMunicija] = 0; gTer[t][terNapadac] = 0;
    for (new n = 0; n < MAX_TER_NPC; n++) { gTerNpcDb[t][n] = 0; gTerNpc[t][n] = INVALID_PLAYER_ID; }
    gTer[t][terZona] = GangZoneCreate(x - h, y - h, x + h, y + h);
    ShowZonu(t);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Teritorij '%s' stvoren. Dilere dodajte u izborniku teritorija."), name);
}

TerDodajDilera(playerid, const params[])
{
    new idx = 0, p1[8], p2[8];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || strval(p2) > 311)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Skin mora biti 0-311."));
    new t = TerPoDb(strval(p1)), n;
    if (t == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Teritorij ne postoji."));
    for (n = 0; n < MAX_TER_NPC && gTerNpcDb[t][n]; n++) {}
    if (n == MAX_TER_NPC) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Teritorij vec ima %d dilera."), MAX_TER_NPC);
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO territory_npcs (territory, x, y, z, a, skin) VALUES (%d, %f, %f, %f, %f, %d)", gTer[t][terDb], x, y, z, a, strval(p2)));
    gTerNpcDb[t][n] = ZadnjiId();
    gTerNpcPos[t][n][0] = x; gTerNpcPos[t][n][1] = y; gTerNpcPos[t][n][2] = z; gTerNpcPos[t][n][3] = a;
    gTerNpcSkin[t][n] = strval(p2);
    KreirajNPCa(t, n);
    SetPlayerPos(playerid, x + 1.5, y, z);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Diler dodan teritoriju %s."), gTer[t][terIme]);
}

TerUkloniDilera(playerid)
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb]) continue;
        for (new n = 0; n < MAX_TER_NPC; n++)
        {
            if (!gTerNpcDb[t][n] || !IsPlayerInRangeOfPoint(playerid, TER_NPC_RANGE, gTerNpcPos[t][n][0], gTerNpcPos[t][n][1], gTerNpcPos[t][n][2])) continue;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM territory_npcs WHERE id = %d", gTerNpcDb[t][n]));
            MakniDilera(t, n);
            return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Diler uklonjen."));
        }
    }
    return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod dilera."));
}

TerVlasnik(playerid, const params[])
{
    new idx = 0, p1[8], p2[8], name[MAX_ORG_NAME];
    strcat(name, T(playerid, "nitko"));
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite ID organizacije (0 = nitko)."));
    new t = TerPoDb(strval(p1)), org = strval(p2);
    if (t == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Teritorij ne postoji."));
    if (org && !GetOrgIme(org, name)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Organizacija ne postoji."));
    if (gTer[t][terNapadac]) FinishNapad(t, false);
    gTer[t][terOrg] = org;
    ShowZonu(t);
    SaveTeritorij(t);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vlasnik teritorija %s: %s."), gTer[t][terIme], name);
}

DeleteTer(playerid, const params[])
{
    new idx = 0, p1[8];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Teritorij ne postoji."));
    new t = TerPoDb(strval(p1));
    if (t == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Teritorij ne postoji."));
    for (new n = 0; n < MAX_TER_NPC; n++) if (gTerNpcDb[t][n]) MakniDilera(t, n);
    GangZoneDestroy(gTer[t][terZona]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM territory_npcs WHERE territory = %d", gTer[t][terDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM territories WHERE id = %d", gTer[t][terDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Teritorij %s obrisan."), gTer[t][terIme]);
    gTer[t][terDb] = 0;
    gTer[t][terNapadac] = 0;
    return 1;
}

TerOdabraniDb(playerid)
{
    return gTer[gTerOdabran[playerid]][terDb];
}

ShowPopisTer(playerid)
{
    new list[2048], row[96], name[MAX_ORG_NAME], count = 0;
    strcat(list, T(playerid, "{8AB4F8}+ Novi teritorij ovdje\n{8AB4F8}Ukloni dilera kod mene\n"));
    new len = strlen(list);
    for (new t = 0; t < MAX_TER; t++)
    {
        if (!gTer[t][terDb]) continue;
        if (!GetOrgIme(gTer[t][terOrg], name)) { name[0] = EOS; strcat(name, T(playerid, "nitko")); }
        format(row, sizeof row, T(playerid, "[%d] %s - %s, vlasnik: %s, dilera: %d"), gTer[t][terDb], gTer[t][terIme], T(playerid, gTerTip[gTer[t][terTip]]), name, ZiviDileri(t));
        if (DodajStupac(list, len, row)) gTerAdminPopis[playerid][count++] = t;
    }
    return PrikaziDialog(playerid, DLG_APANEL_TER, DIALOG_STYLE_LIST, T(playerid, "Admin panel - Teritoriji"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

ShowTerMeni(playerid)
{
    new t = gTerOdabran[playerid];
    if (!gTer[t][terDb]) return ShowPopisTer(playerid);
    return PrikaziDialog(playerid, DLG_APANEL_TER_MENI, DIALOG_STYLE_LIST, gTer[t][terIme], T(playerid, "Postavi vlasnika\nDodaj dilera ovdje\nObrisi teritorij"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_ter(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (listitem == 0) return ShowUnos(playerid, AP_TER_KREIRAJ, T(playerid, "Centar teritorija je vasa pozicija.\nUnesite: [tip: 0 crackhouse, 1 plantaza] [velicina 20-300 m] [ime]"));
    if (listitem == 1)
    {
        TerUkloniDilera(playerid);
        return ShowPopisTer(playerid);
    }
    new i = listitem - 2;
    if (i < 0 || i >= MAX_TER) return 1;
    gTerOdabran[playerid] = gTerAdminPopis[playerid][i];
    return ShowTerMeni(playerid);
}

DLG:apanel_ter_meni(playerid, response, listitem)
{
    if (!response) return ShowPopisTer(playerid);
    switch (listitem)
    {
        case 0: return ShowUnos(playerid, AP_TER_VLASNIK, T(playerid, "Unesite ID organizacije (0 = nitko):"));
        case 1: return ShowUnos(playerid, AP_TER_NPC, T(playerid, "Diler se stvara na vasem mjestu.\nUnesite skin dilera (0-311):"));
        case 2:
        {
            new body[128];
            format(body, sizeof body, T(playerid, "Obrisati teritorij '%s' i sve njegove dilere?"), gTer[gTerOdabran[playerid]][terIme]);
            return PrikaziDialog(playerid, DLG_APANEL_BRISANJE_TER, DIALOG_STYLE_MSGBOX, T(playerid, "Brisanje teritorija"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
        }
    }
    return ShowTerMeni(playerid);
}

DLG:apanel_brisanje_ter(playerid, response)
{
    new p[16];
    if (!response) return ShowTerMeni(playerid);
    format(p, sizeof p, "%d", TerOdabraniDb(playerid));
    DeleteTer(playerid, p);
    return ShowPopisTer(playerid);
}
