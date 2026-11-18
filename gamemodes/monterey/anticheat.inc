#define AC_KODOVA           53
#define AC_RAZMAK_MS        15000
#define AC_BLOK_MS          600000
#define AC_LOG_DANA         30
#define AC_LOG_PRIKAZ       25

enum { ACM_ISKLJUCENO, ACM_UPOZORI, ACM_IZBACI }

static const gAcIme[AC_KODOVA][] = {
    "AirBreak (onfoot)", "AirBreak (vehicle)", "Teleport (onfoot)", "Teleport (vehicle)", "Teleport (into vehicle)", "Teleport (vehicle to player)", "Teleport (pickups)", "FlyHack (onfoot)", "FlyHack (vehicle)",
    "SpeedHack (onfoot)", "", "Health hack (vehicle)", "", "", "", "Weapon hack", "Ammo hack (add)", "Ammo hack (infinite)",
    "Special action hack", "", "", "Invisible hack", "Lagcomp spoof", "Tuning hack", "", "", "Rapid fire",
    "Fake spawn", "Fake kill", "Pro Aim", "", "CarShot", "CarJack", "", "AFK Ghost", "",
    "", "", "High ping", "Dialog hack", "Sandbox", "Invalid version", "Rcon hack", "", "",
    "", "", "", "Connection flood", "", "Seat change flood", "DoS", "NOP"
};

static const gAcZadano[AC_KODOVA] = {
    ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI,
    ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_ISKLJUCENO, ACM_UPOZORI, ACM_ISKLJUCENO, ACM_UPOZORI, ACM_UPOZORI,
    ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_IZBACI, ACM_ISKLJUCENO, ACM_ISKLJUCENO, ACM_UPOZORI, ACM_UPOZORI,
    ACM_IZBACI, ACM_ISKLJUCENO, ACM_ISKLJUCENO, ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_UPOZORI, ACM_ISKLJUCENO, ACM_UPOZORI, ACM_UPOZORI, ACM_ISKLJUCENO, ACM_UPOZORI,
    ACM_ISKLJUCENO, ACM_ISKLJUCENO, ACM_ISKLJUCENO, ACM_UPOZORI, ACM_UPOZORI, ACM_IZBACI, ACM_IZBACI, ACM_IZBACI, ACM_ISKLJUCENO, ACM_ISKLJUCENO,
    ACM_ISKLJUCENO, ACM_ISKLJUCENO, ACM_ISKLJUCENO, ACM_IZBACI, ACM_ISKLJUCENO, ACM_IZBACI, ACM_IZBACI, ACM_UPOZORI
};

static const gAcNacinIme[3][] = { "{777777}Iskljuceno", "{F1C40F}Upozorenje", "{E74C3C}Izbacivanje" };

static gAcNacin[AC_KODOVA], gAcZadnje[MAX_PLAYERS][AC_KODOVA], bool:gAcIzbacen[MAX_PLAYERS];
static const gAcAdminKodovi[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 21, 34 };
static gAcRed[MAX_PLAYERS][AC_KODOVA];

InitAntiCheat()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ac_settings (code INTEGER PRIMARY KEY, mode INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ac_log (id INTEGER PRIMARY KEY AUTOINCREMENT, ts INTEGER NOT NULL, account INTEGER NOT NULL, name TEXT NOT NULL, ip TEXT NOT NULL, code INTEGER NOT NULL, mode INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM ac_log WHERE ts < %d", gettime() - AC_LOG_DANA * 86400));
    for (new c = 0; c < AC_KODOVA; c++) gAcNacin[c] = gAcZadano[c];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT code, mode FROM ac_settings");
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        new c = DB_GetFieldInt(res, 0), m = DB_GetFieldInt(res, 1);
        if (c >= 0 && c < AC_KODOVA && gAcIme[c][0] && m >= ACM_ISKLJUCENO && m <= ACM_IZBACI) gAcNacin[c] = m;
    }
    DB_FreeResultSet(res);
    for (new c = 0; c < AC_KODOVA; c++)
    {
        if (gAcIme[c][0]) EnableAntiCheat(c, gAcNacin[c] != ACM_ISKLJUCENO);
        else DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM ac_settings WHERE code = %d", c));
    }
    EnableAntiNOP(2, false);
    EnableAntiNOP(3, false);
    EnableAntiNOP(5, false);
    EnableAntiNOP(10, false);
}

AntiCheatAdmin(playerid)
{
    new bool:admin = gAdmin[playerid] > 0;
    for (new k = 0; k < sizeof gAcAdminKodovi; k++)
    {
        new c = gAcAdminKodovi[k];
        EnableAntiCheatForPlayer(playerid, c, !admin && gAcNacin[c] != ACM_ISKLJUCENO);
    }
}

AntiCheatConnect(playerid)
{
    gAcIzbacen[playerid] = false;
    for (new c = 0; c < AC_KODOVA; c++) gAcZadnje[playerid][c] = 0;
}

static AcZapisi(playerid, const ip[], code, nacin)
{
    new ime[MAX_PLAYER_NAME + 1] = "-", adresa[24], bool:igrac = playerid != INVALID_PLAYER_ID && IsPlayerConnected(playerid);
    strcat(adresa, ip);
    if (igrac)
    {
        GetPlayerName(playerid, ime, sizeof ime);
        if (!adresa[0]) GetPlayerIp(playerid, adresa, sizeof adresa);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO ac_log (ts, account, name, ip, code, mode) VALUES (%d, %d, '%q', '%q', %d, %d)",
        gettime(), igrac ? gAccountId[playerid] : 0, ime, adresa, code, nacin));
}

public OnCheatDetected(playerid, const ip_address[], type, code)
{
    if (code < 0 || code >= AC_KODOVA || !gAcIme[code][0] || gAcNacin[code] == ACM_ISKLJUCENO) return 1;
    if (type)
    {
        BlockIpAddress(ip_address, AC_BLOK_MS);
        AcZapisi(INVALID_PLAYER_ID, ip_address, code, ACM_IZBACI);
        printf("[AC] IP %s blokiran: %s (#%03d)", ip_address, gAcIme[code], code);
        for (new i = 0; i < MAX_PLAYERS; i++)
            if (IsPlayerConnected(i) && gAdmin[i] > 0) SendClientMessage(i, COLOR_ADMIN, T(i, "[AC] IP %s je blokiran na 10 minuta: %s (#%03d)."), ip_address, gAcIme[code], code);
        return 1;
    }
    if (!IsPlayerConnected(playerid) || gAcIzbacen[playerid]) return 1;
    new nacin = gAcNacin[code], sad = GetTickCount();
    if (nacin == ACM_UPOZORI && gAcZadnje[playerid][code] && sad - gAcZadnje[playerid][code] < AC_RAZMAK_MS) return 1;
    gAcZadnje[playerid][code] = sad;
    AcZapisi(playerid, ip_address, code, nacin);
    printf("[AC] %s (%d): %s (#%03d) - %s", GetRPIme(playerid), playerid, gAcIme[code], code, nacin == ACM_IZBACI ? ("izbacen") : ("upozorenje"));
    if (nacin == ACM_IZBACI)
    {
        gAcIzbacen[playerid] = true;
        if (code == 40) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Previse je igraca spojeno s vase IP adrese."));
        else if (code == 41) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ova verzija klijenta nije podrzana na serveru."));
        else SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Izbaceni ste zbog sumnje na varanje (#%03d)."), code);
        KickTimer(playerid);
    }
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || gAdmin[i] < 1) continue;
        if (nacin == ACM_IZBACI) SendClientMessage(i, COLOR_ADMIN, T(i, "[AC] %s (%d) je izbacen: %s (#%03d)."), GetRPIme(playerid), playerid, gAcIme[code], code);
        else SendClientMessage(i, COLOR_ADMIN, T(i, "[AC] Sumnja: %s (%d) - %s (#%03d)."), GetRPIme(playerid), playerid, gAcIme[code], code);
    }
    return 1;
}

ShowAntiCheat(playerid)
{
    new list[4096], red[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "+ Zadnje detekcije"));
    for (new c = 0; c < AC_KODOVA; c++)
    {
        if (!gAcIme[c][0]) continue;
        format(red, sizeof red, "{FFFFFF}#%02d  %s  -  %s", c, gAcIme[c], T(playerid, gAcNacinIme[gAcNacin[c]]));
        if (DodajStupac(list, len, red)) gAcRed[playerid][n++] = c;
    }
    for (new k = n; k < AC_KODOVA; k++) gAcRed[playerid][k] = -1;
    return PrikaziDialog(playerid, DLG_APANEL_AC, DIALOG_STYLE_LIST, T(playerid, "Anticheat"), list, T(playerid, "Promijeni"), T(playerid, "Natrag"));
}

static ShowAcLog(playerid)
{
    new list[4096], red[160], len = 0, vrijeme[24], ime[MAX_PLAYER_NAME + 1], ip[24];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT datetime(ts, 'unixepoch', 'localtime'), name, ip, code, mode FROM ac_log ORDER BY id DESC LIMIT %d", AC_LOG_PRIKAZ);
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        new c = DB_GetFieldInt(res, 3), m = DB_GetFieldInt(res, 4);
        if (c < 0 || c >= AC_KODOVA || !gAcIme[c][0] || m < ACM_ISKLJUCENO || m > ACM_IZBACI) continue;
        DB_GetFieldString(res, 0, vrijeme, sizeof vrijeme);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        DB_GetFieldString(res, 2, ip, sizeof ip);
        format(red, sizeof red, "{AAAAAA}%s  {FFFFFF}%s (%s)  #%02d %s  %s", vrijeme, ime, ip, c, gAcIme[c], T(playerid, gAcNacinIme[m]));
        DodajStupac(list, len, red);
    }
    DB_FreeResultSet(res);
    if (!len) DodajStupac(list, len, T(playerid, "Nema detekcija."));
    return PrikaziDialog(playerid, DLG_APANEL_AC_LOG, DIALOG_STYLE_LIST, T(playerid, "Zadnje detekcije"), list, T(playerid, "Natrag"));
}

DLG:apanel_ac(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_MANAGE)) return 1;
    if (listitem == 0) return ShowAcLog(playerid);
    new k = listitem - 1;
    if (k < 0 || k >= AC_KODOVA) return 1;
    new c = gAcRed[playerid][k];
    if (c < 0) return 1;
    gAcNacin[c] = (gAcNacin[c] + 1) % 3;
    EnableAntiCheat(c, gAcNacin[c] != ACM_ISKLJUCENO);
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && gAdmin[i] > 0) AntiCheatAdmin(i);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO ac_settings (code, mode) VALUES (%d, %d)", c, gAcNacin[c]));
    return ShowAntiCheat(playerid);
}

DLG:apanel_ac_log(playerid, response, listitem)
{
    #pragma unused response, listitem
    return ShowAntiCheat(playerid);
}
