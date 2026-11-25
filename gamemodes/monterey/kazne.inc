#define KAZNA_SVIJET        15000
#define KAZNA_PLIVANJE      1
#define KAZNA_METENJE       2
#define KAZNA_MIN_METARA    50
#define KAZNA_MAX_METARA    3000
#define KAZNA_MAX_MARKERA   100
#define KAZNA_PLAZA_X       250.0
#define KAZNA_PLAZA_Y       -1888.0
#define KAZNA_TRG_X         1479.6
#define KAZNA_TRG_Y         -1675.0
#define KAZNA_TRG_Z         14.0
#define KAZNA_TRG_R         25.0
#define KAZNA_METLA         19622
#define KAZNA_METLA_SLOT    1
#define KAZNA_METENJE_MS    4000
#define KAZNA_KISA          16
#define KAZNA_TLO           0.4
#define KAZNA_STEPENICA     0.35
#define POVRATAK_S          600

static gKazna[MAX_PLAYERS], gKaznaOstalo[MAX_PLAYERS], bool:gKaznaMete[MAX_PLAYERS], Float:gKaznaCp[MAX_PLAYERS][3], bool:gPovratakProvjeren[MAX_PLAYERS], gKaznaRazlog[MAX_PLAYERS][96];

KaznaAktivna(playerid)
{
    return gKazna[playerid] != 0;
}

InitKazne()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS admin_punish (account INTEGER PRIMARY KEY, type INTEGER NOT NULL, amount INTEGER NOT NULL, reason TEXT NOT NULL DEFAULT '')"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS player_lastpos (account INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL, time INTEGER NOT NULL)"));
}

KaznaKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gKazna[playerid]) return 0;
    if (gKazna[playerid] == KAZNA_METENJE && !gKaznaMete[playerid] && (newkeys & KEY_JUMP) && !(oldkeys & KEY_JUMP) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) ClearAnimations(playerid, SYNC_ALL);
    return 1;
}

KaznaVozilo(playerid)
{
    if (!gKazna[playerid]) return 0;
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    SetPlayerPos(playerid, x, y, z);
    ObavijestGreska(playerid, T(playerid, "Dok ste kaznjeni ne mozete uci u vozilo."));
    return 1;
}

KaznaStanje(playerid, PLAYER_STATE:newstate)
{
    if (gKazna[playerid] && (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)) RemovePlayerFromVehicle(playerid);
}

KaznaBlokira(playerid)
{
    if (!gKazna[playerid]) return 0;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dok ste kaznjeni ne mozete koristiti komande ni chat."));
    return 1;
}

static KaznaSpremi(playerid)
{
    if (gKazna[playerid]) DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE admin_punish SET type = %d, amount = %d WHERE account = %d", gKazna[playerid], gKaznaOstalo[playerid], gAccountId[playerid]));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM admin_punish WHERE account = %d", gAccountId[playerid]));
}

static KaznaObavijest(playerid, tekst[], size = sizeof tekst)
{
    if (gKaznaRazlog[playerid][0])
    {
        strcat(tekst, "~n~", size);
        strcat(tekst, T(playerid, "Razlog:"), size);
        strcat(tekst, " ", size);
        strcat(tekst, gKaznaRazlog[playerid], size);
    }
    IgraObavijestPanel(playerid, T(playerid, "Kazna"), 0xC0392BE6, tekst);
}

static KaznaRazlogPostavi(playerid, const razlog[])
{
    gKaznaRazlog[playerid][0] = EOS;
    strcat(gKaznaRazlog[playerid], razlog, sizeof gKaznaRazlog[]);
    for (new i = 0; gKaznaRazlog[playerid][i]; i++) if (gKaznaRazlog[playerid][i] == '~') gKaznaRazlog[playerid][i] = '-';
}

static KaznaSljedeci(playerid)
{
    new Float:hx, Float:hy, Float:hz, Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    new Float:tlo = Kolizija_Zraka(px, py, pz, px, py, pz - 3.0, hx, hy, hz) ? hz : pz - 1.0;
    for (new p = 0; p < 30; p++)
    {
        new Float:a = float(random(360)), Float:r = 5.0 + float(random(floatround(KAZNA_TRG_R) - 5));
        gKaznaCp[playerid][0] = KAZNA_TRG_X + r * floatcos(a, degrees);
        gKaznaCp[playerid][1] = KAZNA_TRG_Y + r * floatsin(a, degrees);
        gKaznaCp[playerid][2] = tlo;
        if (Kolizija_Zraka(gKaznaCp[playerid][0], gKaznaCp[playerid][1], tlo + 2.5, gKaznaCp[playerid][0], gKaznaCp[playerid][1], tlo - 1.0, hx, hy, hz) && floatabs(hz - tlo) <= KAZNA_STEPENICA && !Kolizija_Zraka(px, py, tlo + 0.6, gKaznaCp[playerid][0], gKaznaCp[playerid][1], hz + 0.6))
        {
            gKaznaCp[playerid][2] = hz;
            break;
        }
    }
    SetPlayerCheckpoint(playerid, gKaznaCp[playerid][0], gKaznaCp[playerid][1], gKaznaCp[playerid][2], 1.5);
    new tekst[200];
    format(tekst, sizeof tekst, T(playerid, "Pometite mjesto oznaceno markerom. Preostalo: %d"), gKaznaOstalo[playerid]);
    KaznaObavijest(playerid, tekst);
}

static KaznaPocni(playerid)
{
    SetPlayerHealth(playerid, FLOAT_INFINITY);
    MobitelZatvori(playerid);
    if (gKazna[playerid] == KAZNA_PLIVANJE)
    {
        new Float:x = KAZNA_PLAZA_X + float(random(201) - 100);
        PrebaciIgraca(playerid, 0, KAZNA_SVIJET + 1 + playerid, x, KAZNA_PLAZA_Y - float(gKaznaOstalo[playerid]), 1.0, 0.0);
        new tekst[200];
        format(tekst, sizeof tekst, T(playerid, "Doplivajte do obale (%d m sjeverno)."), gKaznaOstalo[playerid]);
        KaznaObavijest(playerid, tekst);
        return;
    }
    PrebaciIgraca(playerid, 0, KAZNA_SVIJET, KAZNA_TRG_X, KAZNA_TRG_Y, KAZNA_TRG_Z, 0.0);
    SetPlayerWeather(playerid, KAZNA_KISA);
    SetPlayerAttachedObject(playerid, KAZNA_METLA_SLOT, KAZNA_METLA, 6, 0.05, 0.03, -0.25, 0.0, 0.0, 0.0);
    KaznaSljedeci(playerid);
}

static KaznaKraj(playerid)
{
    new tip = gKazna[playerid];
    gKazna[playerid] = 0;
    gKaznaOstalo[playerid] = 0;
    gKaznaMete[playerid] = false;
    KaznaSpremi(playerid);
    IgraObavijestUkloni(playerid);
    SetPlayerHealth(playerid, 100.0);
    if (tip == KAZNA_PLIVANJE)
    {
        SetPlayerVirtualWorld(playerid, 0);
        ObavijestInfo(playerid, T(playerid, "Kazna"), T(playerid, "Kazna je gotova. Mozete nastaviti igrati."));
        return;
    }
    DisablePlayerCheckpoint(playerid);
    RemovePlayerAttachedObject(playerid, KAZNA_METLA_SLOT);
    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);
    SetPlayerWeather(playerid, GetWeather());
    PrebaciIgraca(playerid, 0, 0, SPAWN_X, SPAWN_Y, SPAWN_Z, SPAWN_A);
    KucaSpawn(playerid);
    ObavijestInfo(playerid, T(playerid, "Kazna"), T(playerid, "Kazna je gotova. Mozete nastaviti igrati."));
}

KaznaCheckpoint(playerid)
{
    if (gKazna[playerid] != KAZNA_METENJE || gKaznaMete[playerid]) return 0;
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    if (VectorSize(x - gKaznaCp[playerid][0], y - gKaznaCp[playerid][1], 0.0) > 3.0) return 1;
    DisablePlayerCheckpoint(playerid);
    gKaznaMete[playerid] = true;
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "INT_HOUSE", "wash_up", 4.1, true, false, false, false, 0, SYNC_ALL);
    SetTimerEx("KaznaPometeno", KAZNA_METENJE_MS, false, "ii", playerid, gAccountId[playerid]);
    return 1;
}

forward KaznaPometeno(playerid, racun);
public KaznaPometeno(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gKazna[playerid] != KAZNA_METENJE) return;
    gKaznaMete[playerid] = false;
    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);
    if (--gKaznaOstalo[playerid] <= 0)
    {
        KaznaKraj(playerid);
        return;
    }
    KaznaSpremi(playerid);
    KaznaSljedeci(playerid);
}

KaznaTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gKazna[i] != KAZNA_PLIVANJE) continue;
        new Float:x, Float:y, Float:z, Float:hx, Float:hy, Float:hz;
        GetPlayerPos(i, x, y, z);
        if (Kolizija_Zraka(x, y, z + 1.0, x, y, z - 2.5, hx, hy, hz) && hz >= KAZNA_TLO) KaznaKraj(i);
    }
}

KaznaSpawn(playerid)
{
    if (!gKazna[playerid])
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT type, amount, reason FROM admin_punish WHERE account = %d", gAccountId[playerid]);
        if (DB_GetRowCount(res))
        {
            gKazna[playerid] = DB_GetFieldInt(res, 0);
            gKaznaOstalo[playerid] = DB_GetFieldInt(res, 1);
            new razlog[96];
            DB_GetFieldString(res, 2, razlog, sizeof razlog);
            KaznaRazlogPostavi(playerid, razlog);
        }
        DB_FreeResultSet(res);
    }
    if (!gKazna[playerid]) return 0;
    gKaznaMete[playerid] = false;
    KaznaPocni(playerid);
    return 1;
}

KaznaDisconnect(playerid)
{
    gKazna[playerid] = 0;
    gKaznaOstalo[playerid] = 0;
    gKaznaMete[playerid] = false;
    gPovratakProvjeren[playerid] = false;
}

PovratakSpremi(playerid)
{
    if (!gUlogiran[playerid] || gKazna[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING || GetPlayerState(playerid) == PLAYER_STATE_NONE) return;
    new svijet = GetPlayerVirtualWorld(playerid), interijer = GetPlayerInterior(playerid);
    new Float:x, Float:y, Float:z, Float:a, v = GetPlayerVehicleID(playerid);
    if (v)
    {
        GetVehiclePos(v, x, y, z);
        GetVehicleZAngle(v, a);
        z += 1.0;
    }
    else
    {
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO player_lastpos (account, x, y, z, a, interior, world, time) VALUES (%d, %f, %f, %f, %f, %d, %d, %d)", gAccountId[playerid], x, y, z, a, interijer, svijet, gettime()));
}

PovratakSpawn(playerid)
{
    if (gPovratakProvjeren[playerid]) return;
    gPovratakProvjeren[playerid] = true;
    if (ZatvorProvjera(playerid))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_lastpos WHERE account = %d", gAccountId[playerid]));
        return;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z, a, interior, world, time FROM player_lastpos WHERE account = %d", gAccountId[playerid]);
    if (DB_GetRowCount(res) && gettime() - DB_GetFieldInt(res, 6) <= POVRATAK_S)
    {
        PrebaciIgraca(playerid, DB_GetFieldInt(res, 4), DB_GetFieldInt(res, 5), DB_GetFieldFloat(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3));
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vraceni ste na mjesto gdje ste izasli iz igre."));
    }
    DB_FreeResultSet(res);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_lastpos WHERE account = %d", gAccountId[playerid]));
}

CMD:kazni(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    new idx = 0, p1[32], p2[8], p3[8], razlog[96];
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !SljedeciParam(params, idx, p3) || !ProvjeriBroj(p2) || !ProvjeriBroj(p3))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: /kazni [id/ime] [1 = plivanje, 2 = metenje] [metara / markera] [razlog]"));
    while (params[idx] == ' ') idx++;
    strcat(razlog, params[idx]);
    if (!razlog[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: /kazni [id/ime] [1 = plivanje, 2 = metenje] [metara / markera] [razlog]"));
    new target = FindPlayer(p1), tip = strval(p2), kolicina = strval(p3);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (!MozeKazniti(playerid, target)) return 1;
    if (tip != KAZNA_PLIVANJE && tip != KAZNA_METENJE) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vrsta kazne mora biti 1 (plivanje) ili 2 (metenje)."));
    if (tip == KAZNA_PLIVANJE && (kolicina < KAZNA_MIN_METARA || kolicina > KAZNA_MAX_METARA)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Udaljenost mora biti od %d do %d m."), KAZNA_MIN_METARA, KAZNA_MAX_METARA);
    if (tip == KAZNA_METENJE && (kolicina < 1 || kolicina > KAZNA_MAX_MARKERA)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Broj markera mora biti od 1 do %d."), KAZNA_MAX_MARKERA);
    if (gKazna[target]) KaznaKraj(target);
    if (IsPlayerInAnyVehicle(target)) RemovePlayerFromVehicle(target);
    gKazna[target] = tip;
    gKaznaOstalo[target] = kolicina;
    KaznaRazlogPostavi(target, razlog);
    gKaznaMete[target] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO admin_punish (account, type, amount, reason) VALUES (%d, %d, %d, '%q')", gAccountId[target], tip, kolicina, razlog));
    KaznaPocni(target);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i]) continue;
        if (tip == KAZNA_PLIVANJE) SendClientMessage(i, COLOR_ADMIN, T(i, "Admin %s je kaznio %s plivanjem (%d m). Razlog: %s"), GetRPIme(playerid), GetRPIme(target), kolicina, razlog);
        else SendClientMessage(i, COLOR_ADMIN, T(i, "Admin %s je kaznio %s metenjem (%d markera). Razlog: %s"), GetRPIme(playerid), GetRPIme(target), kolicina, razlog);
    }
    return 1;
}

CMD:oslobodi(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    new target = FindPlayer(params);
    if (target == INVALID_PLAYER_ID || !gKazna[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Taj igrac nije kaznjen."));
    KaznaKraj(target);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Oslobodili ste %s kazne."), GetRPIme(target));
}
