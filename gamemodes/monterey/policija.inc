#define KAZNA_MAX           2000
#define PD_BLIZU            4.0
#define PD_STOP_DOMET       30.0
#define PD_STOP_SIRINA      6.0
#define WANTED_MAX          6
#define VUCA_RAZMAK         1.0
#define VUCA_MS             250
#define TASER_DOMET         15.0
#define TASER_MS            7000
#define TASER_METAK         100
#define LSPD_INT            6
#define DUTY_POS            240.2088, 78.1646, 1005.0391
#define LOCKER_POS          255.0175, 77.3816, 1003.6406
#define ZATVOR_POS          1798.2422, -1578.3702, 14.0872
#define ZATVOR_A            303.3253
#define ZATVOR_INT          0
#define ZATVOR_SVIJET       900
#define ZATVOR_DOMET        4.0
#define ZATVOR_ULAZ_DOMET   5.0
#define ZATVOR_MAX_MIN      60
#define WANTED_PAD_S        900
#define LOCATE_LS_MS        5000
#define LOCATE_VAN_MS       10000
#define LOCATE_IKONA        99

static bool:gOnDuty[MAX_PLAYERS], gWanted[MAX_PLAYERS], gJailTime[MAX_PLAYERS];
static bool:gCuffed[MAX_PLAYERS], bool:gTased[MAX_PLAYERS], bool:gTaser[MAX_PLAYERS];
static gDragBy[MAX_PLAYERS], gDragging[MAX_PLAYERS], gDragCount;
static gLockerUzeo[MAX_PLAYERS];
static gWantedSat[MAX_PLAYERS], gLocate[MAX_PLAYERS], gLocateRacun[MAX_PLAYERS], gLocateGen[MAX_PLAYERS];

enum { ORMAR_PREDMET, ORMAR_METCI, ORMAR_LIMIT }
static const gLocker[][3] = { { ITEM_PENDREK, 0, 12 }, { ITEM_DEAGLE, 35, 10 }, { ITEM_SACMARICA, 30, 6 }, { ITEM_MP5, 120, 4 }, { ITEM_M4, 150, 3 } };
static gLockerVani[sizeof gLocker];

InitPolicija()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS locker_log (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, org INTEGER NOT NULL, vrijeme INTEGER NOT NULL, ime TEXT NOT NULL, item INTEGER NOT NULL, kolicina INTEGER NOT NULL, uzeo INTEGER NOT NULL)"));
    AddPdIcon("Duty\n/duty", DUTY_POS, LSPD_INT);
    AddPdIcon("Locker\n/locker", LOCKER_POS, LSPD_INT);
    AddPdIcon("Jail", ZATVOR_POS, ZATVOR_INT);
    SetTimer("DragTick", VUCA_MS, true);
}

static AddPdIcon(const natpis[], Float:x, Float:y, Float:z, unutra)
{
    CreateDynamicPickup(HQ_PICKUP, 1, x, y, z, -1, unutra);
    CreateDynamic3DTextLabel(natpis, COLOR_ORG, x, y, z + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, unutra);
}

PolicijaPrijava(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT wanted, zatvor FROM accounts WHERE id = %d", gAccountId[playerid]);
    gWanted[playerid] = DB_GetFieldInt(res, 0);
    gJailTime[playerid] = DB_GetFieldInt(res, 1);
    DB_FreeResultSet(res);
    gWantedSat[playerid] = 0;
}

GetDragged(playerid)
{
    return gDragging[playerid] ? gDragging[playerid] - 1 : INVALID_PLAYER_ID;
}

GetWanted(playerid)
{
    return gWanted[playerid];
}

ZatvorProvjera(playerid)
{
    return gJailTime[playerid];
}

ImaLisice(playerid)
{
    return gCuffed[playerid];
}

VratiLisice(playerid)
{
    if (gCuffed[playerid]) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CUFFED);
}

static SetWanted(playerid, razina)
{
    gWanted[playerid] = razina;
    gWantedSat[playerid] = 0;
    SetPlayerWantedLevel(playerid, razina);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET wanted = %d WHERE id = %d", razina, gAccountId[playerid]));
}

bool:PolicajacNaDuznosti(playerid)
{
    return gOnDuty[playerid] && GetOrgTip(gOrg[playerid]) == ORG_LSPD;
}

DodajWanted(playerid)
{
    if (gWanted[playerid] < WANTED_MAX) SetWanted(playerid, gWanted[playerid] + 1);
    return gWanted[playerid];
}

static SaveJail(playerid)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET zatvor = %d WHERE id = %d", gJailTime[playerid], gAccountId[playerid]));
}

static IgracUCeliju(playerid)
{
    SetPlayerInterior(playerid, ZATVOR_INT);
    SetPlayerVirtualWorld(playerid, ZATVOR_SVIJET);
    SetPlayerPos(playerid, ZATVOR_POS);
    SetPlayerFacingAngle(playerid, ZATVOR_A);
}

static OslobodiIzZatvora(playerid)
{
    gJailTime[playerid] = 0;
    SaveJail(playerid);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, ZATVOR_POS);
    SetPlayerFacingAngle(playerid, ZATVOR_A);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pusteni ste iz zatvora."));
}

PolicijaSpawn(playerid)
{
    ApplyAnimation(playerid, "POLICE", "null", 4.1, false, false, false, false, 0);
    SetPlayerWantedLevel(playerid, gWanted[playerid]);
    if (gJailTime[playerid]) IgracUCeliju(playerid);
}

PolicijaTick()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (gUlogiran[i] && gWanted[i] && !gJailTime[i] && ++gWantedSat[i] >= WANTED_PAD_S)
            SetWanted(i, gWanted[i] - 1);
        if (!gUlogiran[i] || !gJailTime[i]) continue;
        if (--gJailTime[i] <= 0)
        {
            OslobodiIzZatvora(i);
            continue;
        }
        if (GetPlayerState(i) == PLAYER_STATE_ONFOOT && (GetPlayerInterior(i) != ZATVOR_INT || !IsPlayerInRangeOfPoint(i, ZATVOR_DOMET, ZATVOR_POS))) IgracUCeliju(i);
        if (gJailTime[i] % 60 == 0)
        {
            SaveJail(i);
            SendClientMessage(i, COLOR_INFO, T(i, "Preostalo u zatvoru: %d min."), gJailTime[i] / 60);
        }
    }
}

static bool:SluzbaProvjera(playerid, tip)
{
    if (gUlogiran[playerid] && gOrg[playerid] && GetOrgTip(gOrg[playerid]) == tip) return true;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate ovlasti za ovu naredbu."));
    return false;
}

static bool:DutyProvjera(playerid, tip)
{
    if (!SluzbaProvjera(playerid, tip)) return false;
    if (gOnDuty[playerid]) return true;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste na duznosti."));
    return false;
}

bool:PDNaDuznosti(playerid)
{
    return DutyProvjera(playerid, ORG_LSPD);
}

static SluzbaTarget(playerid, const params[])
{
    new idx = 0, p1[32], Float:x, Float:y, Float:z;
    if (!SljedeciParam(params, idx, p1))
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime]"));
        return INVALID_PLAYER_ID;
    }
    new target = ProvjetiBlizinuIgraca(playerid, p1);
    if (target == INVALID_PLAYER_ID) return target;
    GetPlayerPos(target, x, y, z);
    if (IsPlayerInRangeOfPoint(playerid, PD_BLIZU, x, y, z)) return target;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije dovoljno blizu."));
    return INVALID_PLAYER_ID;
}

static bool:HqProvjera(playerid)
{
    if (GetPlayerVirtualWorld(playerid) == HQ_SVIJET + gOrg[playerid]) return true;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar HQ-a organizacije."));
    return false;
}

static VratiOpremu(playerid, bool:skini)
{
    if (!gTaser[playerid]) return;
    gTaser[playerid] = false;
    if (skini) RemovePlayerWeapon(playerid, WEAPON_SILENCED);
}

StopDrag(target)
{
    new p = gDragBy[target] - 1;
    if (p < 0) return;
    gDragBy[target] = 0;
    gDragging[p] = 0;
    gDragCount--;
    if (!gTased[target]) TogglePlayerControllable(target, true);
}

static SkiniLisice(target)
{
    StopDrag(target);
    gCuffed[target] = false;
    SetPlayerSpecialAction(target, SPECIAL_ACTION_NONE);
}

PolicijaDisconnect(playerid, bool:izlaz)
{
    if (gTased[playerid]) TogglePlayerControllable(playerid, true);
    gTased[playerid] = false;
    if (gDragging[playerid]) StopDrag(gDragging[playerid] - 1);
    StopDrag(playerid);
    gCuffed[playerid] = false;
    VratiOpremu(playerid, false);
    if (!izlaz) return;
    LocateStop(playerid);
    gOnDuty[playerid] = false;
    gLockerUzeo[playerid] = 0;
    if (gJailTime[playerid]) SaveJail(playerid);
    gWanted[playerid] = 0;
    gJailTime[playerid] = 0;
}

forward DragTick();
public DragTick()
{
    if (!gDragCount) return;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gDragging[i]) continue;
        new t = gDragging[i] - 1, Float:x, Float:y, Float:z, Float:a;
        GetPlayerPos(i, x, y, z);
        GetPlayerFacingAngle(i, a);
        x += VUCA_RAZMAK * floatsin(a, degrees);
        y -= VUCA_RAZMAK * floatcos(a, degrees);
        if (GetPlayerInterior(t) != GetPlayerInterior(i)) SetPlayerInterior(t, GetPlayerInterior(i));
        if (GetPlayerVirtualWorld(t) != GetPlayerVirtualWorld(i)) SetPlayerVirtualWorld(t, GetPlayerVirtualWorld(i));
        if (IsPlayerInRangeOfPoint(t, 0.3, x, y, z)) continue;
        SetPlayerPos(t, x, y, z);
        SetPlayerFacingAngle(t, a);
    }
}

static bool:SjedaloSlobodno(v, sjedalo)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && IsPlayerInVehicle(i, v) && GetPlayerVehicleSeat(i) == sjedalo) return false;
    return true;
}

PolicijaStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate == PLAYER_STATE_ONFOOT) VratiLisice(playerid);
    if ((newstate != PLAYER_STATE_DRIVER && newstate != PLAYER_STATE_PASSENGER) || !gDragging[playerid]) return;
    new t = gDragging[playerid] - 1, v = GetPlayerVehicleID(playerid);
    StopDrag(t);
    for (new s = GetVehicleSeats(v) - 1; s >= 1; s--)
    {
        if (!SjedaloSlobodno(v, s)) continue;
        PutPlayerInVehicle(t, v, s);
        return;
    }
}

bool:TaserPogodak(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid)
{
    if (!gTaser[playerid] || weaponid != WEAPON_SILENCED) return false;
    if (hittype != BULLET_HIT_TYPE_PLAYER || !gUlogiran[hitid] || gTased[hitid] || IsPlayerInAnyVehicle(hitid) || IsRanjen(hitid)) return true;
    new Float:x, Float:y, Float:z;
    GetPlayerPos(hitid, x, y, z);
    if (!IsPlayerInRangeOfPoint(playerid, TASER_DOMET, x, y, z)) return true;
    gTased[hitid] = true;
    TogglePlayerControllable(hitid, false);
    ApplyAnimation(hitid, "PED", "FLOOR_hit_f", 4.1, false, false, false, true, 0);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pogada %s taserom.", GetRPIme(playerid), GetRPIme(hitid), 1);
    SetTimerEx("TaserEnd", TASER_MS, false, "ii", hitid, gAccountId[hitid]);
    return true;
}

forward TaserEnd(playerid, racun);
public TaserEnd(playerid, racun)
{
    if (gAccountId[playerid] != racun || !gTased[playerid]) return;
    gTased[playerid] = false;
    ClearAnimations(playerid);
    if (!gDragBy[playerid]) TogglePlayerControllable(playerid, true);
    VratiLisice(playerid);
}

static bool:LockerProvjera(playerid)
{
    if (!HqProvjera(playerid)) return false;
    if (IsPlayerInRangeOfPoint(playerid, HQ_DOMET, LOCKER_POS)) return true;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod ormara."));
    return false;
}

CMD:duty(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new tip = gOrg[playerid] ? GetOrgTip(gOrg[playerid]) : -1;
    if (tip != ORG_LSPD && tip != ORG_MEDICINSKA && tip != ORG_VATROGASCI) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate ovlasti za ovu naredbu."));
    if (!HqProvjera(playerid)) return 1;
    if (tip == ORG_LSPD && !IsPlayerInRangeOfPoint(playerid, HQ_DOMET, DUTY_POS)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod mjesta za duznost."));
    gOnDuty[playerid] = !gOnDuty[playerid];
    if (!gOnDuty[playerid]) VratiOpremu(playerid, true);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, gOnDuty[playerid] ? ("* %s dolazi na duznost.") : ("* %s odlazi s duznosti."), GetRPIme(playerid), "", 1);
    return 1;
}

CMD:wanted(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
    if (!params[0])
    {
        new list[1024], row[64], len = 0;
        for (new i = 0; i < MAX_PLAYERS; i++)
        {
            if (!gUlogiran[i] || !gWanted[i]) continue;
            format(row, sizeof row, "%s - %d", GetRPIme(i), gWanted[i]);
            DodajStupac(list, len, row);
        }
        return PrikaziDialog(playerid, DLG_NONE, DIALOG_STYLE_MSGBOX, T(playerid, "Trazeni"), len ? list : T(playerid, "Nitko nije trazen."), T(playerid, "Zatvori"));
    }
    new idx = 0, p1[32], p2[8];
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /wanted [id/ime] [razina 1-6]"));
    new target = FindPlayer(p1), razina = strval(p2);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (razina < 1 || razina > WANTED_MAX) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /wanted [id/ime] [razina 1-6]"));
    if (razina <= gWanted[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Razina mora biti veca od trenutne (%d)."), gWanted[target]);
    SetWanted(target, razina);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "%s je sada trazen, razina %d."), GetRPIme(target), razina);
    SendClientMessage(target, COLOR_ERROR, T(target, "Policija vas trazi, razina %d."), razina);
    return 1;
}

CMD:unwanted(playerid, params[])
{
    if (!gAdmin[playerid])
    {
        if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
        if (!(gOrgPerms[playerid] & PERM_LEADER)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Potjernicu moze skinuti samo lider."));
    }
    new idx = 0, p1[32];
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime]"));
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    SetWanted(target, 0);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "%s vise nije trazen."), GetRPIme(target));
    return 1;
}

CMD:cuff(playerid, params[])
{
    if (!SluzbaProvjera(playerid, ORG_LSPD)) return 1;
    new target = SluzbaTarget(playerid, params);
    if (target == INVALID_PLAYER_ID) return 1;
    if (IsRanjen(target)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac je ranjen."));
    if (target == playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to napraviti sami sebi."));
    if (GetPlayerState(target) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac mora biti pjeske."));
    gCuffed[target] = true;
    SetPlayerArmedWeapon(target, WEAPON_FIST);
    SetPlayerSpecialAction(target, SPECIAL_ACTION_CUFFED);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s stavlja lisice na %s.", GetRPIme(playerid), GetRPIme(target), 1);
    return 1;
}

CMD:uncuff(playerid, params[])
{
    if (!SluzbaProvjera(playerid, ORG_LSPD)) return 1;
    new target = SluzbaTarget(playerid, params);
    if (target == INVALID_PLAYER_ID) return 1;
    if (!gCuffed[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "%s nema lisice."), GetRPIme(target));
    SkiniLisice(target);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s skida lisice s %s.", GetRPIme(playerid), GetRPIme(target), 1);
    return 1;
}

CMD:drag(playerid, params[])
{
    if (!SluzbaProvjera(playerid, ORG_LSPD)) return 1;
    if (gDragging[playerid])
    {
        new t = gDragging[playerid] - 1;
        StopDrag(t);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pusta %s.", GetRPIme(playerid), GetRPIme(t), 1);
        return 1;
    }
    new target = SluzbaTarget(playerid, params);
    if (target == INVALID_PLAYER_ID) return 1;
    if (target == playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to napraviti sami sebi."));
    if (!gCuffed[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "%s nema lisice."), GetRPIme(target));
    if (gDragBy[target] || GetPlayerState(target) != PLAYER_STATE_ONFOOT || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac mora biti pjeske."));
    gDragBy[target] = playerid + 1;
    gDragging[playerid] = target + 1;
    gDragCount++;
    TogglePlayerControllable(target, false);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s vuce %s.", GetRPIme(playerid), GetRPIme(target), 1);
    return 1;
}

CMD:taser(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
    if (gTaser[playerid])
    {
        VratiOpremu(playerid, true);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s sprema taser.", GetRPIme(playerid), "", 1);
        return 1;
    }
    StoreUtor(playerid, 2);
    gTaser[playerid] = true;
    GivePlayerWeapon(playerid, WEAPON_SILENCED, TASER_METAK);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s vadi taser.", GetRPIme(playerid), "", 1);
    return 1;
}

CMD:locker(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD) || !LockerProvjera(playerid)) return 1;
    return PrikaziLocker(playerid);
}

static PrikaziLocker(playerid)
{
    new list[256], row[64], len = 0;
    for (new i = 0; i < sizeof gLocker; i++)
    {
        format(row, sizeof row, "%s%s\t%d/%d", (gLockerUzeo[playerid] & (1 << i)) ? ("* ") : (""), T(playerid, gItems[gLocker[i][ORMAR_PREDMET]][ITEM_NAME]), gLocker[i][ORMAR_LIMIT] - gLockerVani[i], gLocker[i][ORMAR_LIMIT]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_ORMAR, DIALOG_STYLE_TABLIST, T(playerid, "Ormar s oruzjem"), list, T(playerid, "Uzmi/vrati"), T(playerid, "Zatvori"));
}

static LogLocker(playerid, item, kolicina, uzeo)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO locker_log (org, vrijeme, ime, item, kolicina, uzeo) VALUES (%d, %d, '%q', %d, %d, %d)", gOrg[playerid], gettime(), GetRPIme(playerid), item, kolicina, uzeo));
}

DLG:ormar(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gLocker || !DutyProvjera(playerid, ORG_LSPD) || !LockerProvjera(playerid)) return 1;
    new item = gLocker[listitem][ORMAR_PREDMET], ammoItem = gItems[item][ITEM_AMMO], metci = gLocker[listitem][ORMAR_METCI];
    if (gLockerUzeo[playerid] & (1 << listitem))
    {
        gLockerUzeo[playerid] &= ~(1 << listitem);
        if (!GiveItem(playerid, item, -1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
        gLockerVani[listitem]--;
        LogLocker(playerid, item, 1, 0);
        new n = ammoItem == -1 ? 0 : min(metci, gInv[playerid][ammoItem]);
        if (n)
        {
            GiveItem(playerid, ammoItem, -n);
            LogLocker(playerid, ammoItem, n, 0);
        }
        return PrikaziLocker(playerid);
    }
    if (gLockerVani[listitem] >= gLocker[listitem][ORMAR_LIMIT]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Tog oruzja vise nema u ormaru."));
    if (!GiveItem(playerid, item, 1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
    gLockerUzeo[playerid] |= 1 << listitem;
    gLockerVani[listitem]++;
    LogLocker(playerid, item, 1, 1);
    new n = ammoItem == -1 ? 0 : min(metci, gItems[ammoItem][ITEM_MAX] - gInv[playerid][ammoItem]);
    if (n)
    {
        GiveItem(playerid, ammoItem, n);
        LogLocker(playerid, ammoItem, n, 1);
    }
    return PrikaziLocker(playerid);
}

CMD:lockerlog(playerid, params[])
{
    if (!gUlogiran[playerid] || !gOrg[playerid] || GetOrgTip(gOrg[playerid]) != ORG_LSPD || !(gOrgPerms[playerid] & PERM_LEADER))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate ovlasti za ovu naredbu."));
    new list[3072], row[128], vrijeme[16], ime[MAX_PLAYER_NAME + 1];
    format(list, sizeof list, "%s\t%s\t%s\t%s\n", T(playerid, "Vrijeme"), T(playerid, "Ime"), T(playerid, "Predmet"), T(playerid, "Kolicina"));
    new len = strlen(list), DBResult:res = DB_ExecuteQuery(gDB, "SELECT strftime('%%d.%%m. %%H:%%M', vrijeme, 'unixepoch'), ime, item, kolicina, uzeo FROM locker_log WHERE org = %d ORDER BY id DESC LIMIT 30", gOrg[playerid]);
    new bool:ima = DB_GetRowCount(res) > 0;
    if (ima) do
    {
        DB_GetFieldString(res, 0, vrijeme, sizeof vrijeme);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        format(row, sizeof row, "%s\t%s\t%s\t%s %d", vrijeme, ime, T(playerid, gItems[DB_GetFieldInt(res, 2)][ITEM_NAME]), DB_GetFieldInt(res, 4) ? T(playerid, "uzeto") : T(playerid, "vraceno"), DB_GetFieldInt(res, 3));
        DodajStupac(list, len, row);
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    if (!ima) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zapisnik ormara je prazan."));
    return PrikaziDialog(playerid, DLG_NONE, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Zapisnik ormara"), list, T(playerid, "Zatvori"));
}

static GetVozac(v)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && GetPlayerState(i) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(i) == v) return i;
    return INVALID_PLAYER_ID;
}

CMD:provjeripojas(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
    new target = SluzbaTarget(playerid, params);
    if (target == INVALID_PLAYER_ID) return 1;
    if (!IsPlayerInAnyVehicle(target)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije u vozilu."));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s provjerava je li %s vezan pojasom.", GetRPIme(playerid), GetRPIme(target), 1);
    return SendClientMessage(playerid, COLOR_INFO, ImaPojas(target) ? T(playerid, "%s je vezan pojasom.") : T(playerid, "%s nije vezan pojasom."), GetRPIme(target));
}

CMD:checkbelt(playerid, params[]) return cmd_provjeripojas(playerid, params);

CMD:stop(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    new v = GetPlayerVehicleID(playerid), Float:x, Float:y, Float:z, Float:fx, Float:fy, Float:naj = PD_STOP_DOMET, vozac = INVALID_PLAYER_ID, taksi = 0;
    VoziloSmjer(v, x, y, z, fx, fy);
    for (new i = 0; i < gVozilaCount; i++)
    {
        new w = gVozila[i], Float:vx, Float:vy, Float:vz;
        if (w == v) continue;
        GetVehiclePos(w, vx, vy, vz);
        vx -= x;
        vy -= y;
        new Float:uz = vx * fx + vy * fy;
        if (uz <= 0.0 || uz >= naj || floatabs(vx * fy - vy * fx) > PD_STOP_SIRINA || floatabs(vz - z) > 4.0) continue;
        new t = TaksiUVoznji(w), d = t ? INVALID_PLAYER_ID : GetVozac(w);
        if (!t && d == INVALID_PLAYER_ID) continue;
        naj = uz;
        taksi = t;
        vozac = d;
    }
    if (taksi) TaksiNaRub(taksi - 1);
    else if (vozac != INVALID_PLAYER_ID)
    {
        GameTextForPlayer(vozac, T(vozac, "~r~POLICIJA~n~~w~Zaustavite vozilo"), 4000, 3);
        SendClientMessage(vozac, COLOR_ERROR, T(vozac, "Policija vam nalaze da se zaustavite uz rub ceste."));
    }
    else return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ispred vas nema vozila."));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s signalizira vozacu da se zaustavi.", GetRPIme(playerid), "", 1);
    return 1;
}

CMD:arrest(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
    if (GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, ZATVOR_ULAZ_DOMET, ZATVOR_POS)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod ulaza u zatvor."));
    new idx = 0, p1[32], p2[8];
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || strval(p2) < 1 || strval(p2) > ZATVOR_MAX_MIN)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /arrest [id/ime] [minute 1-60]"));
    new target = SluzbaTarget(playerid, p1);
    if (target == INVALID_PLAYER_ID) return 1;
    if (!gCuffed[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "%s nema lisice."), GetRPIme(target));
    SkiniLisice(target);
    VratiOpremu(target, true);
    SpremiSvoOruzje(target);
    ResetPlayerWeapons(target);
    gOnDuty[target] = false;
    SetWanted(target, 0);
    gJailTime[target] = strval(p2) * 60;
    SaveJail(target);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s odvodi %s u celiju.", GetRPIme(playerid), GetRPIme(target), 1);
    IgracUCeliju(target);
    SendClientMessage(target, COLOR_ERROR, T(target, "Uhiceni ste na %d min."), strval(p2));
    return 1;
}

CMD:unjail(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    new idx = 0, p1[32];
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /unjail [id/ime]"));
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (!gJailTime[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "%s nije u zatvoru."), GetRPIme(target));
    OslobodiIzZatvora(target);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s je pusten iz zatvora."), GetRPIme(target));
    return 1;
}

CMD:heal(playerid, params[])
{
    if (IsRanjen(playerid) || !DutyProvjera(playerid, ORG_VATROGASCI)) return 1;
    new target = SluzbaTarget(playerid, params);
    if (target == INVALID_PLAYER_ID) return 1;
    if (IsRanjen(target))
    {
        RanjenPonudi(target, playerid);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Ranjenik odlucuje o pomoci."));
    }
    SetPlayerHealth(target, 100.0);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pruza prvu pomoc %s.", GetRPIme(playerid), GetRPIme(target), 1);
    return 1;
}

CMD:aparat(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_VATROGASCI)) return 1;
    GivePlayerWeapon(playerid, WEAPON_FIREEXTINGUISHER, 500);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s uzima aparat za gasenje.", GetRPIme(playerid), "", 1);
    return 1;
}

CMD:kazna(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
    new idx = 0, p1[32], p2[16], razlog[64];
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || strlen(p2) > 9)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /kazna [id/ime] [iznos] [razlog]"));
    RestParam(params, idx, razlog);
    RemoveBoje(razlog);
    if (!razlog[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /kazna [id/ime] [iznos] [razlog]"));
    new iznos = strval(p2);
    if (iznos < 1 || iznos > KAZNA_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kazna mora biti izmedu $1 i $%d."), KAZNA_MAX);
    new target = SluzbaTarget(playerid, params);
    if (target == INVALID_PLAYER_ID) return 1;
    KaznaDodaj(target, playerid, iznos, razlog);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s ispisuje kaznu za %s.", GetRPIme(playerid), GetRPIme(target), 1);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Izdali ste kaznu od $%d igracu %s."), iznos, GetRPIme(target));
    SendClientMessage(target, COLOR_ERROR, T(target, "Dobili ste kaznu od $%d: %s. Platite je u opstini."), iznos, razlog);
    return 1;
}

CMD:fine(playerid, params[]) return cmd_kazna(playerid, params);

static LocateStop(cop)
{
    if (!gLocate[cop]) return;
    gLocate[cop] = 0;
    gLocateGen[cop]++;
    RemovePlayerMapIcon(cop, LOCATE_IKONA);
}

forward LocateTick(cop, gen);
public LocateTick(cop, gen)
{
    if (gen != gLocateGen[cop] || !gLocate[cop]) return;
    new target = gLocate[cop] - 1;
    if (!gUlogiran[cop] || !gOnDuty[cop])
    {
        LocateStop(cop);
        return;
    }
    if (!IsPlayerConnected(target) || !gUlogiran[target] || gAccountId[target] != gLocateRacun[cop])
    {
        LocateStop(cop);
        SendClientMessage(cop, COLOR_ERROR, T(cop, "Signal je izgubljen, pracenje je prekinuto."));
        return;
    }
    new Float:x, Float:y, Float:z;
    GetPlayerPos(target, x, y, z);
    if (!GetPlayerInterior(target) && !GetPlayerVirtualWorld(target)) SetPlayerMapIcon(cop, LOCATE_IKONA, x, y, z, 0, 0xFF3030FF, MAPICON_GLOBAL);
    new bool:ls = x >= 44.6 && x <= 2997.1 && y >= -2893.0 && y <= -768.0;
    SetTimerEx("LocateTick", ls ? LOCATE_LS_MS : LOCATE_VAN_MS, false, "ii", cop, gen);
}

CMD:locate(playerid, params[])
{
    if (!DutyProvjera(playerid, ORG_LSPD)) return 1;
    new idx = 0, p1[32];
    if (!SljedeciParam(params, idx, p1))
    {
        if (!gLocate[playerid]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /locate [id/ime], /locate za prekid"));
        LocateStop(playerid);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pracenje je prekinuto."));
    }
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (target == playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to sami sebi."));
    LocateStop(playerid);
    gLocate[playerid] = target + 1;
    gLocateRacun[playerid] = gAccountId[target];
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pratite %s. Polozaj se osvjezava na karti (/locate za prekid)."), GetRPIme(target));
    LocateTick(playerid, gLocateGen[playerid]);
    return 1;
}

CMD:lociraj(playerid, params[]) return cmd_locate(playerid, params);
