#define GEPEK_MJESTA        6
#define VLASNIK_PIJACA      (-1)
#define GEPEK_DOMET         2.5
#define GEPEK_PRETRES_MS    5000

enum { GEPEK_OK, GEPEK_NEMA, GEPEK_PUN }

static gGepek[MAX_VEHICLES][BROJ_ITEMA];
static gGepekV[MAX_PLAYERS], gGepekItem[MAX_PLAYERS], gGepekRed[MAX_PLAYERS][BROJ_ITEMA], gGepekBroj[MAX_PLAYERS], bool:gGepekPut[MAX_PLAYERS];

static gGepekPlayer[MAX_VEHICLES], gInGepek[MAX_PLAYERS], gGepekGleda[MAX_VEHICLES];
static bool:gGepekLeaving[MAX_PLAYERS], Float:gGepekIzlaz[MAX_PLAYERS][3], Float:gGepekZdravlje[MAX_PLAYERS][2];

InitGepek()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS vehicle_trunks (\
        veh INTEGER NOT NULL, item INTEGER NOT NULL, amount INTEGER NOT NULL, PRIMARY KEY (veh, item))"));
    GepekCheck();
    for (new i = 0; i < gVozilaCount; i++)
    {
        new v = gVozila[i];
        if (!gOrgVehDb[v]) continue;
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT item, amount FROM vehicle_trunks WHERE veh = %d", gOrgVehDb[v]);
        if (DB_GetRowCount(res)) do
        {
            new item = DB_GetFieldInt(res, 0);
            gGepek[v][item] = DB_GetFieldInt(res, 1);
        } while (DB_SelectNextRow(res));
        DB_FreeResultSet(res);
    }
}

ClearGepek(v)
{
    KickFromGepek(v);
    gGepekGleda[v] = 0;
    for (new i = 0; i < BROJ_ITEMA; i++) gGepek[v][i] = 0;
}

static SaveGepek(v, item)
{
    if (!gOrgVehDb[v]) return;
    if (gGepek[v][item]) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO vehicle_trunks (veh, item, amount) VALUES (%d, %d, %d)", gOrgVehDb[v], item, gGepek[v][item]));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM vehicle_trunks WHERE veh = %d AND item = %d", gOrgVehDb[v], item));
}

static GepekMjesta(v)
{
    new n = 0;
    for (new i = 0; i < BROJ_ITEMA; i++) if (gGepek[v][i]) n++;
    return n;
}

static PutInGepek(playerid, v, item, n)
{
    if (n <= 0 || n > gInv[playerid][item]) return GEPEK_NEMA;
    if ((!gGepek[v][item] && GepekMjesta(v) >= GEPEK_MJESTA) || gGepek[v][item] + n > gItems[item][ITEM_MAX]) return GEPEK_PUN;
    GiveItem(playerid, item, -n);
    gGepek[v][item] += n;
    SaveGepek(v, item);
    return GEPEK_OK;
}

static TakeFromGepek(playerid, v, item, n)
{
    if (n <= 0 || n > gGepek[v][item]) return GEPEK_NEMA;
    if (!GiveItem(playerid, item, n)) return GEPEK_PUN;
    gGepek[v][item] -= n;
    SaveGepek(v, item);
    return GEPEK_OK;
}

static GepekCheck()
{
    new t = MAX_PLAYERS - 1, v = MAX_VEHICLES - 1;
    assert(GiveItem(t, ITEM_KOKAIN, 5));
    assert(PutInGepek(t, v, ITEM_KOKAIN, 3) == GEPEK_OK && gInv[t][ITEM_KOKAIN] == 2 && gGepek[v][ITEM_KOKAIN] == 3);
    assert(PutInGepek(t, v, ITEM_KOKAIN, 3) == GEPEK_NEMA && TakeFromGepek(t, v, ITEM_KOKAIN, 4) == GEPEK_NEMA);
    for (new i = 0; i < GEPEK_MJESTA; i++) gGepek[v][i] = 1;
    assert(GiveItem(t, ITEM_MUN_PUSKA, 1) && PutInGepek(t, v, ITEM_MUN_PUSKA, 1) == GEPEK_PUN);
    assert(GiveItem(t, ITEM_MUN_PUSKA, -1));
    ClearGepek(v);
    assert(GiveItem(t, ITEM_KOKAIN, -2) && gInv[t][ITEM_KOKAIN] == 0);
}

static bool:Leti(model)
{
    switch (model)
    {
        case 417, 425, 447, 460, 469, 476, 487, 488, 497, 511, 512, 513, 519, 520, 548, 553, 563, 577, 592, 593: return true;
    }
    return false;
}

static bool:HasGepek(v)
{
    new model = GetVehicleModel(v);
    if (BezKrova(model) || Leti(model)) return false;
    switch (model)
    {
        case 430, 446, 452, 453, 454, 472, 473, 484, 493, 595, 611: return false;
    }
    return true;
}

static bool:BlizuKraja(playerid, v, Float:smjer)
{
    new Float:x, Float:y, Float:z, Float:a, Float:sx, Float:sy, Float:sz;
    if (GetVehicleVirtualWorld(v) != GetPlayerVirtualWorld(playerid) || !GetVehiclePos(v, x, y, z)) return false;
    GetVehicleZAngle(v, a);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    return IsPlayerInRangeOfPoint(playerid, GEPEK_DOMET, x - smjer * sy / 2.0 * floatsin(a, degrees), y + smjer * sy / 2.0 * floatcos(a, degrees), z);
}

static GepekVozilo(playerid, Float:smjer)
{
    if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) return GetPlayerVehicleID(playerid);
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    for (new i = 0; i < gVozilaCount; i++) if (BlizuKraja(playerid, gVozila[i], smjer)) return gVozila[i];
    return 0;
}

static bool:GepekOtvoren(v)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    return bt == 1;
}

static bool:CanUseGepek(playerid, v)
{
    if (!HasGepek(v))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema prtljaznik."));
        return false;
    }
    if (GepekOtvoren(v) || ImaKljuc(playerid, v)) return true;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate kljuceve ovog vozila."));
    return false;
}

static bool:GepekZauzet(playerid, v)
{
    new g = gGepekGleda[v] - 1;
    return g >= 0 && g != playerid && gUlogiran[g] && gGepekV[g] == v && GetPlayerState(g) == PLAYER_STATE_ONFOOT && BlizuKraja(g, v, -1.0);
}

static GepekPusti(playerid)
{
    new v = gGepekV[playerid];
    if (gGepekGleda[v] == playerid + 1) gGepekGleda[v] = 0;
}

static bool:Poklopac(v, bool:hauba)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    if (hauba) bn = bn == 1 ? 0 : 1;
    else bt = bt == 1 ? 0 : 1;
    SetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    return (hauba ? bn : bt) == 1;
}

static ShowGepek(playerid)
{
    new v = gGepekV[playerid], list[640], row[64], title[96], count = 0;
    gGepekGleda[v] = playerid + 1;
    format(list, sizeof list, "%s\t%s\n{8AB4F8}%s\t\n{8AB4F8}%s\t\n", T(playerid, "Predmet"), T(playerid, "Kolicina"), T(playerid, "Stavi predmet"), T(playerid, "Zatvori prtljaznik"));
    new len = strlen(list);
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gGepek[v][i]) continue;
        format(row, sizeof row, "%s\t%d %s", T(playerid, gItems[i][ITEM_NAME]), gGepek[v][i], T(playerid, gItems[i][ITEM_UNIT]));
        if (DodajStupac(list, len, row)) gGepekRed[playerid][count++] = i;
    }
    gGepekBroj[playerid] = count;
    if (gGepekPlayer[v])
    {
        format(row, sizeof row, "{FFD700}%s\t%s", GetRPIme(gGepekPlayer[v] - 1), T(playerid, "Osoba"));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, T(playerid, "Prtljaznik (%d/%d)"), count, GEPEK_MJESTA);
    return PrikaziDialog(playerid, DLG_GEPEK, DIALOG_STYLE_TABLIST_HEADERS, title, list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static bool:IsGepekOpen(playerid)
{
    new v = gGepekV[playerid];
    if (GetVehicleModel(v) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && BlizuKraja(playerid, v, -1.0) && GepekOtvoren(v)) return true;
    GepekPusti(playerid);
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod otvorenog prtljaznika."));
    return false;
}

static GepekKolicina(playerid, item, bool:stavlja)
{
    gGepekItem[playerid] = item;
    gGepekPut[playerid] = stavlja;
    return PrikaziDialog(playerid, DLG_GEPEK_KOLICINA, DIALOG_STYLE_INPUT, T(playerid, gItems[item][ITEM_NAME]), T(playerid, "Unesite kolicinu:"), T(playerid, "U redu"), T(playerid, "Natrag"));
}

DLG:gepek(playerid, response, listitem)
{
    if (!response)
    {
        GepekPusti(playerid);
        return 1;
    }
    if (!IsGepekOpen(playerid)) return 1;
    if (listitem == 1)
    {
        GepekPusti(playerid);
        Poklopac(gGepekV[playerid], false);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s zatvara prtljaznik.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (listitem == 0)
    {
        new list[512], row[64], len = 0, count = 0;
        for (new i = 0; i < BROJ_ITEMA; i++)
        {
            if (!gInv[playerid][i]) continue;
            format(row, sizeof row, "%s (%d %s)", T(playerid, gItems[i][ITEM_NAME]), gInv[playerid][i], T(playerid, gItems[i][ITEM_UNIT]));
            if (DodajStupac(list, len, row)) gGepekRed[playerid][count++] = i;
        }
        gGepekBroj[playerid] = count;
        if (!count)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
            return ShowGepek(playerid);
        }
        return PrikaziDialog(playerid, DLG_GEPEK_STAVI, DIALOG_STYLE_LIST, T(playerid, "Stavi predmet"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    listitem -= 2;
    if (listitem < 0 || listitem >= gGepekBroj[playerid]) return ShowGepek(playerid);
    return GepekKolicina(playerid, gGepekRed[playerid][listitem], false);
}

DLG:gepek_stavi(playerid, response, listitem)
{
    if (!IsGepekOpen(playerid)) return 1;
    if (!response || listitem < 0 || listitem >= gGepekBroj[playerid]) return ShowGepek(playerid);
    return GepekKolicina(playerid, gGepekRed[playerid][listitem], true);
}

DLG:gepek_kolicina(playerid, response, const inputtext[])
{
    if (!IsGepekOpen(playerid)) return 1;
    if (!response) return ShowGepek(playerid);
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 9)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna kolicina."));
        return ShowGepek(playerid);
    }
    new v = gGepekV[playerid], item = gGepekItem[playerid], n = strval(inputtext);
    new rez = gGepekPut[playerid] ? PutInGepek(playerid, v, item, n) : TakeFromGepek(playerid, v, item, n);
    if (rez == GEPEK_NEMA) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko."));
    else if (rez == GEPEK_PUN) SendClientMessage(playerid, COLOR_ERROR, gGepekPut[playerid] ? T(playerid, "Prtljaznik je pun.") : T(playerid, "Ne mozete nositi toliko."));
    else PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, gGepekPut[playerid] ? ("* %s nesto stavlja u prtljaznik.") : ("* %s nesto uzima iz prtljaznika."), GetRPIme(playerid), "", 1);
    return ShowGepek(playerid);
}

static GepekIza(v, &Float:x, &Float:y, &Float:z)
{
    new Float:a, Float:sx, Float:sy, Float:sz;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    x += (sy / 2.0 + 1.0) * floatsin(a, degrees);
    y -= (sy / 2.0 + 1.0) * floatcos(a, degrees);
    z += 0.3;
}

static SplitGepek(playerid)
{
    new v = gInGepek[playerid];
    gInGepek[playerid] = 0;
    gGepekPlayer[v] = 0;
}

KickFromGepek(v)
{
    new t = gGepekPlayer[v] - 1;
    if (t < 0) return;
    SplitGepek(t);
    GepekIza(v, gGepekIzlaz[t][0], gGepekIzlaz[t][1], gGepekIzlaz[t][2]);
    gGepekLeaving[t] = true;
    TogglePlayerSpectating(t, false);
}

GepekSpawn(playerid)
{
    if (!gGepekLeaving[playerid]) return;
    gGepekLeaving[playerid] = false;
    SetPlayerPos(playerid, gGepekIzlaz[playerid][0], gGepekIzlaz[playerid][1], gGepekIzlaz[playerid][2]);
    SetPlayerHealth(playerid, gGepekZdravlje[playerid][0]);
    SetPlayerArmour(playerid, gGepekZdravlje[playerid][1]);
    VratiLisice(playerid);
    BlackScreen(playerid, false);
}

GepekDisconnect(playerid)
{
    if (gInGepek[playerid]) SplitGepek(playerid);
    gGepekLeaving[playerid] = false;
    GepekPusti(playerid);
    gGepekV[playerid] = 0;
}

static OpenGepek(v)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    if (bt == 1) return false;
    Poklopac(v, false);
    return true;
}

CMD:frisktrunk(playerid, params[])
{
    if (!PDNaDuznosti(playerid)) return 1;
    new v = GetPlayerState(playerid) == PLAYER_STATE_ONFOOT ? GepekVozilo(playerid, -1.0) : 0;
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (!HasGepek(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema prtljaznik."));
    new otvorio = OpenGepek(v);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pretresa prtljaznik.", GetRPIme(playerid), "", 1);
    ApplyAnimation(playerid, "POLICE", "plc_drgbst_01", 4.1, true, false, false, false, 0);
    SetTimerEx("GepekPretresEnd", GEPEK_PRETRES_MS, false, "iiii", playerid, gAccountId[playerid], v, otvorio);
    return 1;
}

forward GepekPretresEnd(playerid, racun, v, otvorio);
public GepekPretresEnd(playerid, racun, v, otvorio)
{
    if (!GetVehicleModel(v)) return;
    if (otvorio) Poklopac(v, false);
    if (!gUlogiran[playerid] || gAccountId[playerid] != racun) return;
    ClearAnimations(playerid);
    if (!BlizuKraja(playerid, v, -1.0)) return;
    new list[640], row[64], len = 0;
    if (gGepekPlayer[v])
    {
        format(row, sizeof row, T(playerid, "Osoba u prtljazniku: %s"), GetRPIme(gGepekPlayer[v] - 1));
        DodajStupac(list, len, row);
    }
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gGepek[v][i]) continue;
        format(row, sizeof row, "%s - %d %s", T(playerid, gItems[i][ITEM_NAME]), gGepek[v][i], T(playerid, gItems[i][ITEM_UNIT]));
        DodajStupac(list, len, row);
    }
    PrikaziDialog(playerid, DLG_NONE, DIALOG_STYLE_MSGBOX, T(playerid, "Pretres prtljaznika"), len ? list : T(playerid, "Nista nije pronadeno."), T(playerid, "Zatvori"));
}

CMD:trunkput(playerid, params[])
{
    if (!PDNaDuznosti(playerid)) return 1;
    new t = GetDragged(playerid);
    if (t == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nikoga ne vucete."));
    new v = GetPlayerState(playerid) == PLAYER_STATE_ONFOOT ? GepekVozilo(playerid, -1.0) : 0;
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (!CanUseGepek(playerid, v)) return 1;
    if (gGepekPlayer[v]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U prtljazniku je vec netko."));
    OpenGepek(v);
    StopDrag(t);
    SpremiSvoOruzje(t);
    GetPlayerHealth(t, gGepekZdravlje[t][0]);
    GetPlayerArmour(t, gGepekZdravlje[t][1]);
    gGepekPlayer[v] = t + 1;
    gInGepek[t] = v;
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s stavlja %s u prtljaznik.", GetRPIme(playerid), GetRPIme(t), 1);
    BlackScreen(t, true);
    TogglePlayerSpectating(t, true);
    PlayerSpectateVehicle(t, v, SPECTATE_MODE_NORMAL);
    return 1;
}

CMD:trunkout(playerid, params[])
{
    if (!gUlogiran[playerid] || ImaLisice(playerid)) return 1;
    new v = GetPlayerState(playerid) == PLAYER_STATE_ONFOOT ? GepekVozilo(playerid, -1.0) : 0;
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (!CanUseGepek(playerid, v)) return 1;
    if (!gGepekPlayer[v]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U prtljazniku nema nikoga."));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s vadi %s iz prtljaznika.", GetRPIme(playerid), GetRPIme(gGepekPlayer[v] - 1), 1);
    OpenGepek(v);
    KickFromGepek(v);
    return 1;
}

CMD:trunk(playerid, params[])
{
    if (IsRanjen(playerid)) return 1;
    if (!gUlogiran[playerid]) return 1;
    new v = GepekVozilo(playerid, -1.0);
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
    {
        if (!HasGepek(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema prtljaznik."));
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, Poklopac(v, false) ? ("* %s otvara prtljaznik.") : ("* %s zatvara prtljaznik."), GetRPIme(playerid), "", 1);
        return 1;
    }
    if (!CanUseGepek(playerid, v)) return 1;
    if (GepekZauzet(playerid, v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Netko vec pretrazuje prtljaznik."));
    if (!GepekOtvoren(v))
    {
        Poklopac(v, false);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s otvara prtljaznik.", GetRPIme(playerid), "", 1);
    }
    gGepekV[playerid] = v;
    return ShowGepek(playerid);
}

CMD:hood(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    new v = GetPlayerVehicleID(playerid);
    if (!HasGepek(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema haubu."));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, Poklopac(v, true) ? ("* %s otvara haubu.") : ("* %s zatvara haubu."), GetRPIme(playerid), "", 1);
    return 1;
}

#define MARICA_MODEL        427
#define HITNA_MODEL         416
#define MARICA_SJEDALO      2
#define MARICA_SVIJET       5000
#define MARICA_INT          4
#define HITNA_INT           1
#define PRIKOLICA_INT       2
#define PRIKOLICA_KUKA      8.0
#define PRIKOLICA_OBJEKT    3174
#define PRIKOLICA_OBJ_Y     -1.0
#define PRIKOLICA_OBJ_Z     -0.8
#define PRIKOLICA_DOMET     2.5
#define PRIKOLICA_STRANA    -1.0
#define PRIKOLICA_VRATA     2.2
#define PRIKOLICA_RAZMAK    3.75
#define PRIKOLICA_VUCA_Z    -0.6
#define PRIKOLICA_SKRIVENO  6000

static STREAMER_TAG_OBJECT:gPrikolicaObj[MAX_VEHICLES];
static gPrikolicaVuce[MAX_VEHICLES], gPrikolicaZakacena[MAX_VEHICLES];

static gMaricaU[MAX_PLAYERS], Float:gMaricaIzlaz[MAX_PLAYERS][3];

InitMarica()
{
    CreateDynamicObject(19321, 225.77921, 1778.15930, 300.00000, 0.00000, 0.00000, 0.00000, -1, 3);
    CreateDynamicObject(19321, 225.77921, 1771.00867, 300.00000, 0.00000, 0.00000, 0.00000, -1, 3);

    CreateVehicle(MARICA_MODEL, -1712.4635, 13.0354, 3.7423, 0.0000, -1, -1, 100);
    CreateVehicle(HITNA_MODEL, -1708.0593, 11.0412, 3.7423, 0.0000, -1, -1, 100);
    CreateVehicle(MARICA_MODEL, -1703.7712, 9.2236, 3.7423, 0.0000, -1, -1, 100);
}

static bool:PrikolicaVrata(v, &Float:x, &Float:y, &Float:z)
{
    new Float:a, c = gPrikolicaVuce[v];
    if (c)
    {
        new Float:sx, Float:sy, Float:sz;
        if (!GetVehiclePos(c, x, y, z)) return false;
        GetVehicleZAngle(c, a);
        GetVehicleModelInfo(GetVehicleModel(c), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
        new Float:l = sy / 2.0 + PRIKOLICA_RAZMAK;
        x += floatsin(a, degrees) * l + floatcos(a, degrees) * PRIKOLICA_STRANA * PRIKOLICA_VRATA;
        y += floatsin(a, degrees) * PRIKOLICA_STRANA * PRIKOLICA_VRATA - floatcos(a, degrees) * l;
        return true;
    }
    if (!GetVehiclePos(v, x, y, z)) return false;
    GetVehicleZAngle(v, a);
    x += floatcos(a, degrees) * PRIKOLICA_STRANA * PRIKOLICA_VRATA - floatsin(a, degrees) * PRIKOLICA_OBJ_Y;
    y += floatsin(a, degrees) * PRIKOLICA_STRANA * PRIKOLICA_VRATA + floatcos(a, degrees) * PRIKOLICA_OBJ_Y;
    return true;
}

static bool:MaricaBehind(v, &Float:x, &Float:y, &Float:z)
{
    if (GetVehicleModel(v) == PRIKOLICA_MODEL) return PrikolicaVrata(v, x, y, z);
    new Float:a, Float:sirina, Float:duzina, Float:visina;
    if (!GetVehiclePos(v, x, y, z)) return false;
    GetVehicleZAngle(v, a);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sirina, duzina, visina);
    x += (duzina / 2.0 + 1.0) * floatsin(-a + 180.0, degrees);
    y += (duzina / 2.0 + 1.0) * floatcos(-a + 180.0, degrees);
    return true;
}

MaricaReset(playerid)
{
    if (!gMaricaU[playerid]) return;
    gMaricaU[playerid] = 0;
    SetPlayerTime(playerid, GetWorldTime(), 0);
}

static MaricaExit(playerid)
{
    new Float:x, Float:y, Float:z;
    if (MaricaBehind(gMaricaU[playerid], x, y, z)) z += 0.3;
    else
    {
        x = gMaricaIzlaz[playerid][0];
        y = gMaricaIzlaz[playerid][1];
        z = gMaricaIzlaz[playerid][2];
    }
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, x, y, z);
    new Float:a;
    GetVehicleZAngle(gPrikolicaVuce[gMaricaU[playerid]] ? gPrikolicaVuce[gMaricaU[playerid]] : gMaricaU[playerid], a);
    SetPlayerFacingAngle(playerid, GetVehicleModel(gMaricaU[playerid]) == PRIKOLICA_MODEL ? a - 90.0 * PRIKOLICA_STRANA : a + 180.0);
    MaricaReset(playerid);
    if (!ImaLisice(playerid)) ApplyAnimation(playerid, "PED", "CAR_getout_LHS", 4.1, false, false, false, false, 0);
}

static UdjiUMaricu(playerid, v)
{
    new model = GetVehicleModel(v), unutra = MARICA_INT, svijet = MARICA_SVIJET + v, Float:x = 1462.27, Float:y = 1182.16, Float:z = -6.18;
    if (model == HITNA_MODEL)
    {
        unutra = HITNA_INT;
        x = 1435.64;
        y = 1229.4;
        z = -0.2;
    }
    else if (model == PRIKOLICA_MODEL)
    {
        unutra = PRIKOLICA_INT;
        x = 0.3440;
        y = -0.5140;
        z = 1000.5490;
    }
    GetPlayerPos(playerid, gMaricaIzlaz[playerid][0], gMaricaIzlaz[playerid][1], gMaricaIzlaz[playerid][2]);
    gMaricaU[playerid] = v;
    SetPlayerInterior(playerid, unutra);
    SetPlayerVirtualWorld(playerid, svijet);
    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, 180.0);
    SetPlayerTime(playerid, 0, 0);
    SetCameraBehindPlayer(playerid);
    Streamer_UpdateEx(playerid, x, y, z, svijet, unutra, STREAMER_TYPE_OBJECT, 1000);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Stisni Enter da izadjes iz vozila."));
}

MaricaEnter(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_PASSENGER || !gUlogiran[playerid] || GetPlayerVehicleSeat(playerid) < MARICA_SJEDALO) return;
    new v = GetPlayerVehicleID(playerid), model = GetVehicleModel(v);
    if (model == MARICA_MODEL || model == HITNA_MODEL) UdjiUMaricu(playerid, v);
}

PrikolicaDodaj(v)
{
    if (GetVehicleModel(v) != PRIKOLICA_MODEL) return;
    gPrikolicaObj[v] = CreateDynamicObject(PRIKOLICA_OBJEKT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    AttachDynamicObjectToVehicle(gPrikolicaObj[v], v, 0.0, PRIKOLICA_OBJ_Y, PRIKOLICA_OBJ_Z, 0.0, 0.0, 0.0);
}

PrikolicaOtkaci(c)
{
    new t = gPrikolicaZakacena[c], Float:x, Float:y, Float:z, Float:a, Float:sx, Float:sy, Float:sz;
    if (!t) return;
    GetVehiclePos(c, x, y, z);
    GetVehicleZAngle(c, a);
    GetVehicleModelInfo(GetVehicleModel(c), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    new Float:l = sy / 2.0 + PRIKOLICA_RAZMAK + PRIKOLICA_OBJ_Y;
    gPrikolicaZakacena[c] = 0;
    gPrikolicaVuce[t] = 0;
    SetVehiclePos(t, x + floatsin(a, degrees) * l, y - floatcos(a, degrees) * l, z + 0.5);
    SetVehicleZAngle(t, a);
    SetVehicleVirtualWorld(t, 0);
    AttachDynamicObjectToVehicle(gPrikolicaObj[t], t, 0.0, PRIKOLICA_OBJ_Y, PRIKOLICA_OBJ_Z, 0.0, 0.0, 0.0);
}

static PrikolicaZakaci(t, c)
{
    new Float:sx, Float:sy, Float:sz, Float:wx, Float:wy, Float:wz, model = GetVehicleModel(c);
    GetVehicleModelInfo(model, VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    GetVehicleModelInfo(model, VEHICLE_MODEL_INFO_WHEELSREAR, wx, wy, wz);
    gPrikolicaVuce[t] = c;
    gPrikolicaZakacena[c] = t;
    AttachDynamicObjectToVehicle(gPrikolicaObj[t], c, 0.0, -(sy / 2.0 + PRIKOLICA_RAZMAK), wz + PRIKOLICA_VUCA_Z, 0.0, 0.0, 0.0);
    SetVehicleVirtualWorld(t, PRIKOLICA_SKRIVENO);
    SetVehiclePos(t, 0.0, 0.0, -100.0);
}

PrikolicaMakni(v)
{
    PrikolicaOtkaci(v);
    if (GetVehicleModel(v) != PRIKOLICA_MODEL) return;
    gPrikolicaZakacena[gPrikolicaVuce[v]] = 0;
    gPrikolicaVuce[v] = 0;
    DestroyDynamicObject(gPrikolicaObj[v]);
}

PrikolicaUlaz(playerid)
{
    if (GetPlayerVirtualWorld(playerid) || GetPlayerInterior(playerid) || ImaLisice(playerid)) return 0;
    new Float:vx, Float:vy, Float:vz;
    for (new i = 0; i < gVozilaCount; i++)
    {
        if (GetVehicleModel(gVozila[i]) != PRIKOLICA_MODEL || (!gPrikolicaVuce[gVozila[i]] && GetVehicleVirtualWorld(gVozila[i])) || !PrikolicaVrata(gVozila[i], vx, vy, vz) || !IsPlayerInRangeOfPoint(playerid, PRIKOLICA_DOMET, vx, vy, vz)) continue;
        UdjiUMaricu(playerid, gVozila[i]);
        return 1;
    }
    return 0;
}

CMD:zakaci(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    new v = GetPlayerVehicleID(playerid);
    if (gPrikolicaZakacena[v])
    {
        PrikolicaOtkaci(v);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s otkacuje prikolicu.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (!HasGepek(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo ne moze vuci prikolicu."));
    new Float:x, Float:y, Float:z;
    GepekIza(v, x, y, z);
    for (new i = 0; i < gVozilaCount; i++)
    {
        new t = gVozila[i];
        if (GetVehicleModel(t) != PRIKOLICA_MODEL || gPrikolicaVuce[t] || GetVehicleVirtualWorld(t) || GetVehicleDistanceFromPoint(t, x, y, z) > PRIKOLICA_KUKA) continue;
        PrikolicaZakaci(t, v);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s kaci prikolicu.", GetRPIme(playerid), "", 1);
        return 1;
    }
    return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema slobodne prikolice iza vozila."));
}

MaricaKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gUlogiran[playerid] || !gMaricaU[playerid]) return;
    if (GetPlayerVirtualWorld(playerid) != MARICA_SVIJET + gMaricaU[playerid]) MaricaReset(playerid);
    else if ((newkeys & KEY_SECONDARY_ATTACK) && !(oldkeys & KEY_SECONDARY_ATTACK))
    {
        new Float:vx, Float:vy, Float:vz;
        GetVehicleVelocity(gMaricaU[playerid], vx, vy, vz);
        if (ImaLisice(playerid) && VectorSize(vx, vy, vz) > 0.02) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo se krece."));
        else MaricaExit(playerid);
    }
}

CMD:maricatest(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    MaricaReset(playerid);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, -1704.5363, 18.5412, 3.5547);
    return 1;
}

#define HELI_LSPD           7
#define HELI_NEWS           6
#define HELI_DOMET          200.0
#define HELI_ZAPIS_MS       60000
#define HELI_VIJEST_MS      30000
#define HELI_BROJ           (HELI_LSPD + HELI_NEWS)
#define HELI_MODEL_LSPD     497
#define HELI_MODEL_NEWS     488
#define HELI_SKIN_LSPD      280
#define HELI_SKIN_NEWS      170
#define HELI_START_MS       3000
#define HELI_RAZMAK_MS      200
#define HELI_RESPAWN_MS     60000

static const Float:gHeliStart[HELI_BROJ][4] = {
    { 400.0, -1800.0, 65.0, 38.55 },
    { 1072.44, -1048.86, 162.64, 290.35 },
    { 2144.03, -1199.7, 110.29, 267.86 },
    { 2683.07, -1775.47, 81.69, 121.71 },
    { 2421.4, -2285.23, 89.02, 64.8 },
    { 1325.43, -2107.44, 77.29, 79.28 },
    { 720.0, -1400.0, 115.0, 325.84 },
    { 1410.0, -830.0, 190.0, 87.31 },
    { 681.44, -1911.75, 62.91, 291.34 },
    { 2769.27, -1621.44, 82.22, 229.99 },
    { 1450.0, -2300.0, 85.0, 143.6 },
    { 2240.36, -2685.81, 94.19, 276.8 },
    { 2583.38, -2197.27, 92.83, 87.96 }
};

static gHeliNpc[HELI_BROJ], gHeliVozilo[HELI_BROJ], gNpcHeli[MAX_PLAYERS], gHeliZapis[MAX_PLAYERS], gHeliVijest;

InitHeli()
{
    for (new i = 0; i < HELI_BROJ; i++) SetTimerEx("HeliStvori", HELI_START_MS + i * HELI_RAZMAK_MS, false, "i", i);
}

forward HeliStvori(i);
public HeliStvori(i)
{
    new name[MAX_PLAYER_NAME + 1];
    format(name, sizeof name, "Pilot_%d", i);
    new npc = NPC_Create(name);
    if (npc == INVALID_PLAYER_ID) return;
    new v = CreateVehicle(i < HELI_LSPD ? HELI_MODEL_LSPD : HELI_MODEL_NEWS, gHeliStart[i][0], gHeliStart[i][1], gHeliStart[i][2], gHeliStart[i][3], -1, -1, -1);
    if (v == INVALID_VEHICLE_ID)
    {
        NPC_Destroy(npc);
        return;
    }
    gHeliNpc[i] = npc;
    gHeliVozilo[i] = v;
    gNpcHeli[npc] = i + 1;
    NPC_Spawn(npc);
}

HeliSpawn(npcid)
{
    new i = gNpcHeli[npcid] - 1;
    if (i < 0) return;
    NPC_SetSkin(npcid, i < HELI_LSPD ? HELI_SKIN_LSPD : HELI_SKIN_NEWS);
    NPC_SetInvulnerable(npcid, true);
    SetVehicleParamsEx(gHeliVozilo[i], 1, 1, 0, 0, 0, 0, 0);
    NPC_PutInVehicle(npcid, gHeliVozilo[i], 0);
    SetTimerEx("HeliLeti", 1000, false, "i", i);
}

forward HeliLeti(i);
public HeliLeti(i)
{
    if (!gHeliVozilo[i]) return;
    new ime[16];
    format(ime, sizeof ime, i < HELI_LSPD ? ("heli_lspd_%d") : ("heli_news_%d"), i < HELI_LSPD ? i + 1 : i - HELI_LSPD + 1);
    NPC_StartPlayback(gHeliNpc[i], ime);
}

public OnNPCPlaybackEnd(npcid, recordid)
{
    if (gNpcHeli[npcid]) HeliLeti(gNpcHeli[npcid] - 1);
    else VlakPlaybackEnd(npcid);
    return 1;
}

static bool:HeliVidi(playerid, &bool:vijest)
{
    if (!gUlogiran[playerid] || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || PolicajacNaDuznosti(playerid)) return false;
    new Float:x, Float:y, Float:z, Float:hx, Float:hy, Float:hz, bool:vidi = false;
    GetPlayerPos(playerid, x, y, z);
    for (new i = 0; i < HELI_BROJ; i++)
    {
        if (!gHeliVozilo[i]) continue;
        NPC_GetPos(gHeliNpc[i], hx, hy, hz);
        if (VectorSize(hx - x, hy - y, hz - z) > HELI_DOMET || Kolizija_Zraka(hx, hy, hz - 2.0, x, y, z + 0.5)) continue;
        vidi = true;
        if (i >= HELI_LSPD) vijest = true;
    }
    return vidi;
}

HeliVidiIgraca(playerid)
{
    new bool:vijest;
    return HeliVidi(playerid, vijest) ? 1 : 0;
}

static HeliPrijava(playerid)
{
    new sad = GetTickCount();
    if (sad - gHeliZapis[playerid] < HELI_ZAPIS_MS) return;
    gHeliZapis[playerid] = sad;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Helikopter vas je uocio. Razina potjernice: %d."), DodajWanted(playerid));
}

HeliPucnjava(playerid, BULLET_HIT_TYPE:hittype)
{
    new bool:vijest = false;
    if (hittype != BULLET_HIT_TYPE_PLAYER || !HeliVidi(playerid, vijest)) return;
    HeliPrijava(playerid);
    new sad = GetTickCount();
    if (vijest && sad - gHeliVijest >= HELI_VIJEST_MS)
    {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        gHeliVijest = sad;
        PosaljiSvima(COLOR_ADMIN, "LS News: Prijavljena pucnjava u podrucju %s.", CestaZona(x, y));
    }
}

static HeliKradja(playerid)
{
    new bool:vijest = false;
    if (HeliVidi(playerid, vijest)) HeliPrijava(playerid);
}

HeliMrtav(v)
{
    for (new i = 0; i < HELI_BROJ; i++)
    {
        if (gHeliVozilo[i] != v) continue;
        gNpcHeli[gHeliNpc[i]] = 0;
        NPC_Destroy(gHeliNpc[i]);
        DestroyVehicle(v);
        gHeliVozilo[i] = 0;
        SetTimerEx("HeliStvori", HELI_RESPAWN_MS, false, "i", i);
        return 1;
    }
    return 0;
}

static const gVoziloBrzina[212] = {
    160, 160, 200, 120, 150, 165, 110, 170, 110, 180, 160, 240, 160, 160, 140, 230, 155, 200,
    150, 160, 180, 180, 165, 145, 170, 200, 200, 170, 170, 200, 190, 130, 80, 180, 200, 120,
    160, 160, 160, 160, 160, 75, 150, 150, 110, 165, 190, 200, 190, 150, 120, 240, 190, 190,
    190, 140, 160, 160, 165, 160, 190, 160, 190, 140, 200, 75, 160, 160, 190, 200, 170, 190,
    190, 190, 160, 160, 150, 200, 150, 165, 200, 160, 150, 120, 190, 160, 100, 200, 200, 170,
    170, 160, 160, 190, 220, 170, 200, 200, 140, 140, 160, 75, 220, 220, 160, 170, 160, 230,
    165, 120, 120, 200, 200, 200, 140, 120, 120, 165, 165, 200, 200, 190, 190, 190, 160, 110,
    160, 160, 160, 170, 160, 60, 70, 140, 200, 160, 160, 160, 110, 200, 110, 160, 230, 160,
    170, 160, 160, 160, 200, 160, 160, 160, 165, 200, 160, 170, 180, 110, 110, 200, 200, 200,
    200, 200, 200, 75, 200, 160, 160, 170, 110, 110, 90, 60, 110, 60, 160, 200, 160, 110,
    160, 190, 165, 160, 170, 120, 190, 110, 200, 140, 170, 200, 200, 200, 120, 190, 60, 200,
    200, 200, 160, 165, 110, 200, 160, 160, 200, 160, 160, 160, 140, 160
};

#define VOZILO_TANK             100.0
#define VOZILO_REZERVA          10.0
#define VOZILO_POTROSNJA        0.014
#define VOZILO_POTROSNJA_BRZINA 0.07
#define VOZILO_POTROSNJA_DIZEL  0.75
#define VOZILO_KVAR_ZDRAVLJE    450.0
#define VOZILO_KVAR_POSTO       8
#define VOZILO_KVAR_PALJENJE    50
#define VOZILO_SUDAR_PAD        0.55
#define VOZILO_SUDAR_MIN        15.0
#define VOZILO_SUDAR_STETA      150.0
#define VOZILO_SUDAR_POJAS      0.4
#define VOZILO_KRADJA_MS        6000
#define VOZILO_BRZINA_PUTA      1.3
#define VOZILO_BRZINA_VISE      20.0
#define VOZILO_MOTOR_MS         200
#define VOZILO_DOMET            4.0
#define BENZIN_CIJENA           3
#define DIZEL_CIJENA            2
#define PUMPA_MAX               48
#define PUMPA_DOMET             2.0
#define VOZILO_KRIVO_LITARA     5.0
#define VOZILO_KRIVO_S          180
#define VOZILO_KRADJA_ORG_POSTO 10
#define PUMPA_PICKUP            1650

enum { VP_MOTOR, VP_SVJETLA, VP_ALARM, VP_VRATA }
enum { PUMPA_NEMA, PUMPA_KRIVA, PUMPA_OK }
enum E_PUMPA { bool:pmIma, pmDb, pmDizel, Float:pmX, Float:pmY, Float:pmZ, STREAMER_TAG_PICKUP:pmPickup, STREAMER_TAG_3D_TEXT_LABEL:pmTekst }

static gPumpa[PUMPA_MAX][E_PUMPA];
static gApPumpaDb[MAX_PLAYERS];

static Float:gKrivoGorivo[MAX_VEHICLES], gKrivoVoznja[MAX_VEHICLES];
static Float:gGorivo[MAX_VEHICLES],Float:gVoziloZadnja[MAX_VEHICLES], gVoziloUkradeno[MAX_VEHICLES], gVoziloKradjac[MAX_VEHICLES], gVoziloMotorMs[MAX_VEHICLES];
static gPojas[MAX_PLAYERS], gKradja[MAX_PLAYERS], gKradjaKraj[MAX_PLAYERS];
static gGorivoNacin[MAX_PLAYERS], gGorivoItem[MAX_PLAYERS];
enum { GORIVO_PUMPA = 1, GORIVO_PUNI, GORIVO_TOCI }

static Float:VoziloSteta(Float:pad, pojas)
{
    new Float:steta = VOZILO_SUDAR_MIN + (pad - VOZILO_SUDAR_PAD) * VOZILO_SUDAR_STETA;
    return pojas ? steta * VOZILO_SUDAR_POJAS : steta;
}

InitVozilo()
{
    assert(JeDizel(431) && !JeDizel(411) && gVoziloBrzina[411 - 400] == 240);
    assert(VoziloSteta(1.0, 1) < VoziloSteta(1.0, 0));
    ManualVehicleEngineAndLights();
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS gas_pumps (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, dizel INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, dizel FROM gas_pumps ORDER BY id LIMIT %d", PUMPA_MAX);
    for (new p = 0, n = DB_GetRowCount(res); p < n; p++)
    {
        gPumpa[p][pmDb] = DB_GetFieldInt(res, 0);
        PumpaStvori(p, DB_GetFieldInt(res, 4), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3));
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static PumpaStvori(p, dizel, Float:x, Float:y, Float:z)
{
    gPumpa[p][pmIma] = true;
    gPumpa[p][pmDizel] = dizel;
    gPumpa[p][pmX] = x;
    gPumpa[p][pmY] = y;
    gPumpa[p][pmZ] = z;
    gPumpa[p][pmPickup] = CreateDynamicPickup(PUMPA_PICKUP, 1, x, y, z, 0, 0);
    gPumpa[p][pmTekst] = CreateDynamic3DTextLabel(dizel ? ("Dizel\n/tankaj") : ("Benzin\n/tankaj"), COLOR_ORG, x, y, z + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
}

static PumpaBlizu(playerid, dizel)
{
    new rez = PUMPA_NEMA;
    for (new p = 0; p < PUMPA_MAX; p++)
    {
        if (!gPumpa[p][pmIma] || !IsPlayerInRangeOfPoint(playerid, PUMPA_DOMET, gPumpa[p][pmX], gPumpa[p][pmY], gPumpa[p][pmZ])) continue;
        if (gPumpa[p][pmDizel] == dizel) return PUMPA_OK;
        rez = PUMPA_KRIVA;
    }
    return rez;
}

ShowPumpe(playerid)
{
    new list[2048], row[64], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova pumpa za benzin na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova pumpa za dizel na mojoj poziciji"));
    for (new p = 0; p < PUMPA_MAX; p++)
    {
        if (!gPumpa[p][pmIma]) continue;
        format(row, sizeof row, "[%d] %s - %s", gPumpa[p][pmDb], CestaZona(gPumpa[p][pmX], gPumpa[p][pmY]), gPumpa[p][pmDizel] ? T(playerid, "Dizel") : T(playerid, "Benzin"));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_PUMPA, DIALOG_STYLE_LIST, T(playerid, "Pumpe goriva"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static PumpaNova(playerid, dizel)
{
    new Float:x, Float:y, Float:z, p = 0;
    GetPlayerPos(playerid, x, y, z);
    while (p < PUMPA_MAX && gPumpa[p][pmIma]) p++;
    if (p == PUMPA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pumpa se ovdje ne moze postaviti."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO gas_pumps (x, y, z, dizel) VALUES (%f, %f, %f, %d)", x, y, z, dizel));
    gPumpa[p][pmDb] = ZadnjiId();
    PumpaStvori(p, dizel, x, y, z);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pumpa dodana: %s."), CestaZona(x, y));
}

DLG:apanel_pumpa(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem < 2)
    {
        PumpaNova(playerid, listitem);
        return ShowPumpe(playerid);
    }
    new n = listitem - 1;
    for (new p = 0; p < PUMPA_MAX; p++)
    {
        if (!gPumpa[p][pmIma] || --n) continue;
        new body[96];
        gApPumpaDb[playerid] = gPumpa[p][pmDb];
        format(body, sizeof body, T(playerid, "Obrisati pumpu: %s?"), CestaZona(gPumpa[p][pmX], gPumpa[p][pmY]));
        return PrikaziDialog(playerid, DLG_APANEL_PUMPA_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Pumpe goriva"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowPumpe(playerid);
}

DLG:apanel_pumpa_brisi(playerid, response)
{
    if (!response) return ShowPumpe(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new p = 0;
    while (p < PUMPA_MAX && (!gPumpa[p][pmIma] || gPumpa[p][pmDb] != gApPumpaDb[playerid])) p++;
    if (p == PUMPA_MAX) return ShowPumpe(playerid);
    DestroyDynamicPickup(gPumpa[p][pmPickup]);
    DestroyDynamic3DTextLabel(gPumpa[p][pmTekst]);
    gPumpa[p][pmIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM gas_pumps WHERE id = %d", gApPumpaDb[playerid]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pumpa obrisana."));
    return ShowPumpe(playerid);
}

BezKrova(model)
{
    switch (model)
    {
        case 448, 461, 462, 463, 468, 471, 481, 509, 510, 521, 522, 523, 581, 586: return 1;
    }
    return 0;
}

BezMotora(model)
{
    return model == 481 || model == 509 || model == 510;
}

static JeDizel(model)
{
    switch (model)
    {
        case 400, 403, 406, 407, 408, 413, 414, 416, 418, 422, 423, 427, 428, 431, 432, 433, 435, 437, 443, 450, 455, 456, 459, 478, 486, 498, 499, 500, 508, 514, 515, 516, 524, 525, 526, 529, 532, 533, 538, 540, 544, 545, 554, 570, 571, 573, 578, 579, 583, 585, 587, 589, 594, 600, 601, 602, 609, 610: return 1;
    }
    return 0;
}

static VoziloGet(v, param)
{
    new p[7];
    GetVehicleParamsEx(v, p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
    return p[param];
}

static VoziloSet(v, param, stanje)
{
    new p[7];
    GetVehicleParamsEx(v, p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
    p[param] = param == VP_MOTOR && BezMotora(GetVehicleModel(v)) ? 1 : stanje;
    SetVehicleParamsEx(v, p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
}

VoziloZakljucaj(v, bool:zakljucano)
{
    VoziloSet(v, VP_VRATA, zakljucano ? 1 : 0);
}

VoziloImaGoriva(v)
{
    return gGorivo[v] > 0.0 ? 1 : 0;
}

Float:VoziloTank(v)
{
    return VOZILO_TANK * (1.0 + 0.5 * float(TuningRezervoar(v)));
}

VoziloNapuni(v)
{
    gGorivo[v] = VoziloTank(v);
    gKrivoGorivo[v] = 0.0;
    gKrivoVoznja[v] = 0;
}

static VoziloReset(v)
{
    gGorivo[v] = VoziloTank(v);
    gVoziloZadnja[v] = 0.0;
    gVoziloUkradeno[v] = 0;
    gKrivoGorivo[v] = 0.0;
    gKrivoVoznja[v] = 0;
    SetVehicleParamsEx(v, BezMotora(GetVehicleModel(v)), 0, 0, 0, 0, 0, 0);
}

VoziloTablica(v, tablica[], size = sizeof (tablica))
{
    format(tablica, size, "LS-%04d", v);
}

VoziloDodaj(v)
{
    new tablica[12];
    VoziloTablica(v, tablica);
    SetVehicleNumberPlate(v, tablica);
    VoziloReset(v);
}

public OnVehicleSpawn(vehicleid)
{
    VoziloReset(vehicleid);
    VlasnickoSpawn(vehicleid);
    return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
    if (HeliMrtav(vehicleid)) return 1;
    new s = TaksiPoVozilu(vehicleid);
    KickFromGepek(vehicleid);
    if (s != -1) TaksiPrekini(s);
    PrikolicaOtkaci(vehicleid);
    SlucajEksplozija(vehicleid);
    VlasnikUnisten(vehicleid);
    return 1;
}

ImaPojas(playerid)
{
    return gPojas[playerid];
}

VoziloStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate == PLAYER_STATE_DRIVER) gVoziloZadnja[GetPlayerVehicleID(playerid)] = 0.0;
    else if (newstate != PLAYER_STATE_PASSENGER) gPojas[playerid] = 0;
}

// anti speed hack
static bool:VoziloPrebrzo(playerid, v, Float:vx, Float:vy)
{
    new model = GetVehicleModel(v);
    if (model < 400 || model > 611 || Leti(model)) return false;
    new kmh = floatround(VectorSize(vx, vy, 0.0) * 180.0), najvise = gVoziloBrzina[model - 400] * (100 + 8 * TuningMotor(v)) / 100;
    if (float(kmh) <= float(najvise) * VOZILO_BRZINA_PUTA + VOZILO_BRZINA_VISE) return false;
    VlasnikStetaSpremi(v);
    SetVehicleToRespawn(v);
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo je vraceno: islo je brze nego sto moze."));
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (IsPlayerConnected(i) && gAdmin[i] > 0) SendClientMessage(i, COLOR_ADMIN, T(i, "[AC] %s vozi %s brzinom %d km/h (najvise %d). Vozilo je vraceno."), GetRPIme(playerid), gImenaVozila[model - 400], kmh, najvise);
    return true;
}

VoziloUpdate(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return;
    new v = GetPlayerVehicleID(playerid), Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(v, vx, vy, vz);
    if (VoziloPrebrzo(playerid, v, vx, vy)) return;
    new razina = TuningMotor(v), model = GetVehicleModel(v);
    TuningVozac(v);
    TuningTrubaSync(playerid, v);
    if (razina && model >= 400 && model <= 611 && GetTickCount() - gVoziloMotorMs[v] >= VOZILO_MOTOR_MS)
    {
        new KEY:tipke, gore, strana, Float:a, Float:ravno = VectorSize(vx, vy, 0.0), Float:kmh = ravno * 180.0;
        GetPlayerKeys(playerid, tipke, gore, strana);
        GetVehicleZAngle(v, a);
        new Float:naprijed = -vx * floatsin(a, degrees) + vy * floatcos(a, degrees);
        if ((tipke & KEY_SPRINT) && !(tipke & (KEY_JUMP | KEY_HANDBRAKE)) && kmh > 15.0 && naprijed > 0.95 * ravno && floatabs(vz) < 0.15
            && kmh < float(gVoziloBrzina[model - 400]) * (1.0 + 0.08 * float(razina)))
        {
            gVoziloMotorMs[v] = GetTickCount();
            new Float:pojacanje = 1.0 + 0.008 * float(razina);
            SetVehicleVelocity(v, vx * pojacanje, vy * pojacanje, vz);
        }
    }
    new Float:brzina = VectorSize(vx, vy, vz), Float:pad = gVoziloZadnja[v] - brzina;
    gVoziloZadnja[v] = brzina;
    if (pad < VOZILO_SUDAR_PAD) return;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i] || !IsPlayerInVehicle(i, v) || IsRanjen(i)) continue;
        new Float:zivot;
        GetPlayerHealth(i, zivot);
        zivot -= VoziloSteta(pad, gPojas[i]);
        if (zivot <= RANJEN_PRAG) RanjenOdmah(i);
        else SetPlayerHealth(i, zivot);
    }
}

static VoziloPotrosnja(playerid, v)
{
    new model = GetVehicleModel(v);
    if (BezMotora(model) || VoziloGet(v, VP_MOTOR) != 1) return;
    new Float:vx, Float:vy, Float:vz, Float:zdravlje, Float:staro = gGorivo[v];
    GetVehicleVelocity(v, vx, vy, vz);
    gGorivo[v] -= (VOZILO_POTROSNJA + VectorSize(vx, vy, vz) * VOZILO_POTROSNJA_BRZINA) * (JeDizel(model) ? VOZILO_POTROSNJA_DIZEL : 1.0);
    if (gGorivo[v] <= 0.0)
    {
        gGorivo[v] = gKrivoGorivo[v] = 0.0;
        VoziloSet(v, VP_MOTOR, 0);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilu je nestalo goriva."));
        return;
    }
    gKrivoGorivo[v] *= gGorivo[v] / staro;
    if (staro > VOZILO_REZERVA && gGorivo[v] <= VOZILO_REZERVA) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Malo goriva u spremniku."));
    if (gKrivoGorivo[v] >= VOZILO_KRIVO_LITARA && ++gKrivoVoznja[v] >= VOZILO_KRIVO_S)
    {
        VoziloSet(v, VP_MOTOR, 0);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Motor ne radi zbog krivog goriva."));
        return;
    }
    GetVehicleHealth(v, zdravlje);
    if (zdravlje >= VOZILO_KVAR_ZDRAVLJE || random(100) >= VOZILO_KVAR_POSTO) return;
    VoziloSet(v, VP_MOTOR, 0);
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Motor se ugasio zbog kvara."));
}

static KradjaKraj(playerid, uspjeh)
{
    new v = gKradja[playerid];
    gKradja[playerid] = 0;
    gVoziloKradjac[v] = 0;
    VoziloSet(v, VP_ALARM, 0);
    VoziloSet(v, VP_SVJETLA, 0);
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) ClearAnimations(playerid);
    if (!uspjeh) return;
    gVoziloUkradeno[v] = 1;
    VoziloSet(v, VP_VRATA, 0);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Uspjesno ste obili vozilo."));
}

static KradjaKorak(playerid)
{
    new v = gKradja[playerid], Float:x, Float:y, Float:z, PLAYER_STATE:stanje = GetPlayerState(playerid);
    GetVehiclePos(v, x, y, z);
    if (!GetVehicleModel(v) || (stanje != PLAYER_STATE_ONFOOT && (stanje != PLAYER_STATE_DRIVER || GetPlayerVehicleID(playerid) != v)) || IsRanjen(playerid) || ImaLisice(playerid) || !IsPlayerInRangeOfPoint(playerid, VOZILO_DOMET, x, y, z))
    {
        KradjaKraj(playerid, 0);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kradja je prekinuta."));
        return;
    }
    VoziloSet(v, VP_SVJETLA, VoziloGet(v, VP_SVJETLA) == 1 ? 0 : 1);
    HeliKradja(playerid);
    if (GetTickCount() - gKradjaKraj[playerid] < 0) return;
    if (!gOrgVehOrg[v] || random(100) < VOZILO_KRADJA_ORG_POSTO) KradjaKraj(playerid, 1);
    else
    {
        KradjaKraj(playerid, 0);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste uspjeli obiti vozilo."));
    }
}

VoziloTick()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i]) continue;
        if (gKradja[i]) KradjaKorak(i);
        if (GetPlayerState(i) == PLAYER_STATE_DRIVER) VoziloPotrosnja(i, GetPlayerVehicleID(i));
    }
}

VoziloDisconnect(playerid)
{
    if (gKradja[playerid]) KradjaKraj(playerid, 0);
    gPojas[playerid] = 0;
}

static VoziloBlizu(playerid)
{
    new Float:x, Float:y, Float:z;
    for (new i = 0; i < gVozilaCount; i++)
    {
        new v = gVozila[i];
        if (GetVehicleVirtualWorld(v) != GetPlayerVirtualWorld(playerid)) continue;
        GetVehiclePos(v, x, y, z);
        if (IsPlayerInRangeOfPoint(playerid, VOZILO_DOMET, x, y, z)) return v;
    }
    return 0;
}

static bool:MozeKljuc(v)
{
    new model = GetVehicleModel(v);
    return VlasnikVozila(v) != VLASNIK_PIJACA && !BezKrova(model) && model != MARICA_MODEL && model != HITNA_MODEL && model != PRIKOLICA_MODEL && TaksiPoVozilu(v) == -1 && !JeKamion(v);
}

ImaKljuc(playerid, v)
{
    new vlasnik = VlasnikVozila(v);
    if (vlasnik == VLASNIK_PIJACA) return gAdmin[playerid] >= ALVL_ORG;
    if (vlasnik) return vlasnik == gAccountId[playerid] || gVoziloUkradeno[v] || gAdmin[playerid] >= ALVL_ORG;
    return !gOrgVehOrg[v] || gOrgVehOrg[v] == gOrg[playerid] || gVoziloUkradeno[v] || gAdmin[playerid] >= ALVL_ORG;
}

VoziloVozac(playerid)
{
    new v = GetPlayerVehicleID(playerid);
    if (!gUlogiran[playerid]) return;
    if (ImaKljuc(playerid, v))
    {
        if (!BezMotora(GetVehicleModel(v)) && VoziloGet(v, VP_MOTOR) != 1) SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Pomoc: Upalite motor tipkom 2 ili naredbom /motor."));
        return;
    }
    if (!MozeKljuc(v))
    {
        RemovePlayerFromVehicle(playerid);
        SendClientMessage(playerid, COLOR_ERROR, VlasnikVozila(v) == VLASNIK_PIJACA ? T(playerid, "Ovo vozilo je na prodaji na auto pijaci.") : T(playerid, "Ovo vozilo pripada organizaciji."));
        return;
    }
    VoziloSet(v, VP_MOTOR, 0);
    SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Nemate kljuceve. Spojite zice naredbom /ukradi."));
}

static VoziloMotor(playerid)
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || TuningAktivan(playerid)) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    new v = GetPlayerVehicleID(playerid), Float:zdravlje;
    if (BezMotora(GetVehicleModel(v))) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema motor."));
    if (VoziloGet(v, VP_MOTOR) == 1)
    {
        VoziloSet(v, VP_MOTOR, 0);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s gasi motor.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (!ImaKljuc(playerid, v)) return SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Nemate kljuceve. Spojite zice naredbom /ukradi."));
    if (gGorivo[v] <= 0.0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema goriva."));
    if (gKrivoVoznja[v] >= VOZILO_KRIVO_S) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Motor ne radi zbog krivog goriva."));
    GetVehicleHealth(v, zdravlje);
    if (zdravlje < VOZILO_KVAR_ZDRAVLJE && random(100) < VOZILO_KVAR_PALJENJE) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Motor se ne pali."));
    VoziloSet(v, VP_MOTOR, 1);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pali motor.", GetRPIme(playerid), "", 1);
    return 1;
}

VoziloKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if ((newkeys & KEY_SUBMISSION) && !(oldkeys & KEY_SUBMISSION) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) VoziloMotor(playerid);
    if ((newkeys & KEY_CROUCH) && !(oldkeys & KEY_CROUCH) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) TuningTrubi(playerid);
}

enum { VM_MOTOR, VM_SVJETLA, VM_HAUBA, VM_PRTLJAZNIK, VM_ZAKLJUCAJ, VM_PARKIRAJ, VM_PROZOR, VM_POJAS, VM_PROMETNA }
static gVoziloMeni[MAX_PLAYERS][9], gVoziloMeniBroj[MAX_PLAYERS], gVoziloMeniV[MAX_PLAYERS];

static VoziloMojeBlizu(playerid)
{
    new najblize = 0, Float:naj = VOZILO_DOMET, Float:x, Float:y, Float:z;
    for (new i = 0; i < gVozilaCount; i++)
    {
        new v = gVozila[i];
        if (VlasnikVozila(v) != gAccountId[playerid] || GetVehicleVirtualWorld(v) != GetPlayerVirtualWorld(playerid)) continue;
        GetVehiclePos(v, x, y, z);
        new Float:d = GetPlayerDistanceFromPoint(playerid, x, y, z);
        if (d > naj) continue;
        naj = d;
        najblize = v;
    }
    return najblize;
}

static bool:VoziloMeniVrijedi(playerid, v)
{
    if (!GetVehicleModel(v)) return false;
    if (IsPlayerInAnyVehicle(playerid)) return GetPlayerVehicleID(playerid) == v;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || VlasnikVozila(v) != gAccountId[playerid]) return false;
    new Float:x, Float:y, Float:z;
    GetVehiclePos(v, x, y, z);
    return IsPlayerInRangeOfPoint(playerid, VOZILO_DOMET, x, y, z);
}

VoziloMeni(playerid)
{
    new v = GetPlayerVehicleID(playerid), bool:pjesice = false;
    if (!v && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
    {
        v = VoziloMojeBlizu(playerid);
        pjesice = true;
    }
    if (!v) return 0;
    new model = GetVehicleModel(v), bool:vozac = GetPlayerState(playerid) == PLAYER_STATE_DRIVER, n = 0, list[512], row[64], len = 0;
    gVoziloMeniV[playerid] = v;
    if (pjesice)
    {
        gVoziloMeni[playerid][n++] = VM_ZAKLJUCAJ;
        if (HasGepek(v))
        {
            gVoziloMeni[playerid][n++] = VM_HAUBA;
            gVoziloMeni[playerid][n++] = VM_PRTLJAZNIK;
        }
    }
    if (vozac && !BezMotora(model))
    {
        gVoziloMeni[playerid][n++] = VM_MOTOR;
        gVoziloMeni[playerid][n++] = VM_SVJETLA;
    }
    if (vozac && HasGepek(v))
    {
        gVoziloMeni[playerid][n++] = VM_HAUBA;
        gVoziloMeni[playerid][n++] = VM_PRTLJAZNIK;
    }
    if (vozac) gVoziloMeni[playerid][n++] = VM_ZAKLJUCAJ;
    if (vozac && VlasnikVozila(v) == gAccountId[playerid]) gVoziloMeni[playerid][n++] = VM_PARKIRAJ;
    if (!pjesice && !BezKrova(model))
    {
        gVoziloMeni[playerid][n++] = VM_PROZOR;
        gVoziloMeni[playerid][n++] = VM_POJAS;
    }
    gVoziloMeni[playerid][n++] = VM_PROMETNA;
    gVoziloMeniBroj[playerid] = n;
    for (new i = 0; i < n; i++)
    {
        switch (gVoziloMeni[playerid][i])
        {
            case VM_MOTOR: format(row, sizeof row, "%s", VoziloGet(v, VP_MOTOR) == 1 ? T(playerid, "Ugasi motor") : T(playerid, "Upali motor"));
            case VM_SVJETLA: format(row, sizeof row, "%s", VoziloGet(v, VP_SVJETLA) == 1 ? T(playerid, "Ugasi svjetla") : T(playerid, "Upali svjetla"));
            case VM_HAUBA: format(row, sizeof row, "%s", T(playerid, "Hauba"));
            case VM_PRTLJAZNIK: format(row, sizeof row, "%s", T(playerid, "Prtljaznik"));
            case VM_ZAKLJUCAJ: format(row, sizeof row, "%s", VoziloGet(v, VP_VRATA) == 1 ? T(playerid, "Otkljucaj") : T(playerid, "Zakljucaj"));
            case VM_PARKIRAJ: format(row, sizeof row, "%s", T(playerid, "Parkiraj ovdje"));
            case VM_PROZOR: format(row, sizeof row, "%s", T(playerid, "Prozor"));
            case VM_POJAS: format(row, sizeof row, "%s", gPojas[playerid] ? T(playerid, "Skini pojas") : T(playerid, "Stavi pojas"));
            case VM_PROMETNA: format(row, sizeof row, "%s", T(playerid, "Prometna dozvola"));
        }
        DodajStupac(list, len, row);
    }
    PrikaziDialog(playerid, DLG_VOZILO_MENI, DIALOG_STYLE_LIST, gImenaVozila[model - 400], list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
    return 1;
}

DLG:vozilo_meni(playerid, response, listitem)
{
    new v = gVoziloMeniV[playerid];
    if (!response || listitem < 0 || listitem >= gVoziloMeniBroj[playerid] || !VoziloMeniVrijedi(playerid, v)) return 1;
    new prazno[1];
    if (!IsPlayerInAnyVehicle(playerid))
    {
        switch (gVoziloMeni[playerid][listitem])
        {
            case VM_ZAKLJUCAJ: VoziloKljuc(playerid, v);
            case VM_HAUBA: PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, Poklopac(v, true) ? ("* %s otvara haubu.") : ("* %s zatvara haubu."), GetRPIme(playerid), "", 1);
            case VM_PRTLJAZNIK:
            {
                if (GepekVozilo(playerid, -1.0) == v) return cmd_trunk(playerid, prazno);
                PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, Poklopac(v, false) ? ("* %s otvara prtljaznik.") : ("* %s zatvara prtljaznik."), GetRPIme(playerid), "", 1);
            }
            case VM_PROMETNA: return PokaziPrometnu(playerid, v);
        }
        return VoziloMeni(playerid);
    }
    switch (gVoziloMeni[playerid][listitem])
    {
        case VM_MOTOR: cmd_motor(playerid, prazno);
        case VM_SVJETLA: cmd_svjetla(playerid, prazno);
        case VM_HAUBA: cmd_hood(playerid, prazno);
        case VM_PRTLJAZNIK: cmd_trunk(playerid, prazno);
        case VM_ZAKLJUCAJ: cmd_zakljucaj(playerid, prazno);
        case VM_PARKIRAJ: cmd_parkiraj(playerid, prazno);
        case VM_PROZOR: cmd_prozor(playerid, prazno);
        case VM_POJAS: cmd_pojas(playerid, prazno);
        case VM_PROMETNA: return cmd_prometna(playerid, prazno);
    }
    return VoziloMeni(playerid);
}

CMD:motor(playerid, params[])
{
    return VoziloMotor(playerid);
}

CMD:svjetla(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    new v = GetPlayerVehicleID(playerid), novo = VoziloGet(v, VP_SVJETLA) == 1 ? 0 : 1;
    if (BezMotora(GetVehicleModel(v))) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema motor."));
    VoziloSet(v, VP_SVJETLA, novo);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, novo ? ("* %s pali svjetla.") : ("* %s gasi svjetla."), GetRPIme(playerid), "", 1);
    return 1;
}

CMD:zakljucaj(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    if (!IsPlayerInAnyVehicle(playerid) && (KucaZakljucaj(playerid) || GarazaZakljucaj(playerid))) return 1;
    new v = GetPlayerVehicleID(playerid);
    if (!v) v = VoziloBlizu(playerid);
    return VoziloKljuc(playerid, v);
}

static VoziloKljuc(playerid, v)
{
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (!MozeKljuc(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nije moguce zakljucati."));
    if (!ImaKljuc(playerid, v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate kljuceve ovog vozila."));
    new novo = VoziloGet(v, VP_VRATA) == 1 ? 0 : 1;
    VoziloSet(v, VP_VRATA, novo);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, novo ? ("* %s zakljucava vozilo.") : ("* %s otkljucava vozilo."), GetRPIme(playerid), "", 1);
    return 1;
}

CMD:ukradi(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    new PLAYER_STATE:stanje = GetPlayerState(playerid);
    if (stanje != PLAYER_STATE_ONFOOT && stanje != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    if (gKradja[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vec obijate vozilo."));
    new v = stanje == PLAYER_STATE_DRIVER ? GetPlayerVehicleID(playerid) : VoziloBlizu(playerid);
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (!MozeKljuc(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nije moguce ukrasti."));
    if (ImaKljuc(playerid, v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo ne morate krasti."));
    new vozac = GetVehicleOccupant(v, 0);
    if (vozac != INVALID_PLAYER_ID && vozac != playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U vozilu netko sjedi."));
    if (gVoziloKradjac[v]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo vec netko obija."));
    gKradja[playerid] = v;
    gKradjaKraj[playerid] = GetTickCount() + VOZILO_KRADJA_MS;
    gVoziloKradjac[v] = playerid + 1;
    VoziloSet(v, VP_ALARM, 1);
    if (stanje == PLAYER_STATE_ONFOOT) ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, true, false, false, false, 0);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, stanje == PLAYER_STATE_ONFOOT ? ("* %s pokusava obiti vozilo.") : ("* %s spaja zice ispod volana."), GetRPIme(playerid), "", 1);
    return 1;
}

CMD:pojas(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    new v = GetPlayerVehicleID(playerid);
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste u vozilu."));
    if (BezKrova(GetVehicleModel(v))) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema pojas."));
    gPojas[playerid] = !gPojas[playerid];
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, gPojas[playerid] ? ("* %s stavlja pojas.") : ("* %s skida pojas."), GetRPIme(playerid), "", 1);
    return 1;
}

CMD:prozor(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    new v = GetPlayerVehicleID(playerid), s = GetPlayerVehicleSeat(playerid), p[4];
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste u vozilu."));
    if (BezKrova(GetVehicleModel(v)) || s < 0 || s >= sizeof p) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema prozore."));
    GetVehicleParamsCarWindows(v, p[0], p[1], p[2], p[3]);
    p[s] = p[s] == 0 ? 1 : 0;
    SetVehicleParamsCarWindows(v, p[0], p[1], p[2], p[3]);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, p[s] == 0 ? ("* %s spusta prozor.") : ("* %s podize prozor."), GetRPIme(playerid), "", 1);
    return 1;
}

static Spremnik(t, v)
{
    new ret[16], Float:pravo = gGorivo[v] - gKrivoGorivo[v], bool:dizel = JeDizel(GetVehicleModel(v)) != 0;
    if (gGorivo[v] < 0.5) strcat(ret, T(t, "Prazan"));
    else if (gKrivoGorivo[v] < 0.5) strcat(ret, dizel ? T(t, "Dizel") : T(t, "Benzin"));
    else if (pravo < 0.5) strcat(ret, dizel ? T(t, "Benzin") : T(t, "Dizel"));
    else strcat(ret, T(t, "Mjesavina"));
    return ret;
}

static PokaziPrometnu(t, v)
{
    new body[512], vlasnik[MAX_ORG_NAME], tablica[12], model = GetVehicleModel(v), Float:zdravlje;
    if (!VlasnikIme(v, vlasnik) && !GetOrgIme(gOrgVehOrg[v], vlasnik)) strcat(vlasnik, T(t, "Grad Los Santos"));
    VoziloTablica(v, tablica);
    GetVehicleHealth(v, zdravlje);
    format(body, sizeof body, T(t, "Vozilo: %s\nTablice: %s\nVlasnik: %s\nGodina: %d\nGorivo: %s\nSpremnik: %s (%d/%d L)\nNajveca brzina: %d km/h\nSjedala: %d\nStanje: %d%%"),
        gImenaVozila[model - 400], tablica, vlasnik, 1990 + v * 7 % 32, BezMotora(model) ? T(t, "Nema") : JeDizel(model) ? T(t, "Dizel") : T(t, "Benzin"),
        BezMotora(model) ? T(t, "Nema") : Spremnik(t, v), BezMotora(model) ? 0 : floatround(gGorivo[v]), floatround(VoziloTank(v)), gVoziloBrzina[model - 400], GetVehicleSeats(v), floatround(zdravlje / 10.0));
    return PrikaziDialog(t, DLG_NONE, DIALOG_STYLE_MSGBOX, T(t, "Prometna dozvola"), body, T(t, "Zatvori"));
}

CMD:prometna(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new v = GetPlayerVehicleID(playerid), idx = 0, p1[32];
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste u vozilu."));
    if (!SljedeciParam(params, idx, p1)) return PokaziPrometnu(playerid, v);
    new target = ProvjetiBlizinuIgraca(playerid, p1);
    if (target == INVALID_PLAYER_ID) return 1;
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pokazuje prometnu dozvolu %s.", GetRPIme(playerid), GetRPIme(target), 1);
    return PokaziPrometnu(target, v);
}

static PumpaTip(playerid)
{
    for (new p = 0; p < PUMPA_MAX; p++) if (gPumpa[p][pmIma] && IsPlayerInRangeOfPoint(playerid, PUMPA_DOMET, gPumpa[p][pmX], gPumpa[p][pmY], gPumpa[p][pmZ])) return gPumpa[p][pmDizel];
    return -1;
}

static GorivoPitaj(playerid, nacin, item, maks, const upozorenje[] = "")
{
    new body[192];
    gGorivoNacin[playerid] = nacin;
    gGorivoItem[playerid] = item;
    format(body, sizeof body, T(playerid, "%sUnesite kolicinu u litrama (najvise %d):"), upozorenje, maks);
    return PrikaziDialog(playerid, DLG_GORIVO, DIALOG_STYLE_INPUT, T(playerid, "Gorivo"), body, T(playerid, "Natoci"), T(playerid, "Odustani"));
}

static VoziloProstor(v)
{
    return floatround(VoziloTank(v) - gGorivo[v], floatround_floor);
}

static KanisterNapuni(playerid, litara)
{
    if (IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    new dizel = PumpaTip(playerid);
    if (dizel == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod pumpe."));
    new item = dizel ? ITEM_KANISTER_DIZEL : ITEM_KANISTER_BENZIN, prostor = gItems[item][ITEM_MAX] - gInv[playerid][item];
    if (!gInv[playerid][item] && !gInv[playerid][ITEM_KANISTER]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate prazan kanister."));
    if (prostor < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kanister je pun."));
    new cijena = dizel ? DIZEL_CIJENA : BENZIN_CIJENA, maks = min(prostor, gNovac[playerid] / cijena);
    if (maks < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    if (litara < 0) return GorivoPitaj(playerid, GORIVO_PUNI, item, maks);
    if (litara > maks) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najvise mozete %d L."), maks);
    if (!gInv[playerid][item]) GiveItem(playerid, ITEM_KANISTER, -1);
    GiveItem(playerid, item, litara);
    DajNovac(playerid, -litara * cijena);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s puni kanister gorivom.", GetRPIme(playerid), "", 1);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Natocili ste %d L za $%d."), litara, litara * cijena);
}

VoziloKanister(playerid, item, litara)
{
    if (IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    if (!gInv[playerid][item]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti pjeske."));
    new v = VoziloBlizu(playerid), model = GetVehicleModel(v);
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (BezMotora(model)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema motor."));
    if (!ImaKljuc(playerid, v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate kljuceve ovog vozila."));
    new prostor = VoziloProstor(v);
    if (prostor < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Spremnik je pun."));
    new maks = min(gInv[playerid][item], prostor);
    if (litara < 0) return GorivoPitaj(playerid, GORIVO_TOCI, item, maks);
    if (litara > maks) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najvise mozete %d L."), maks);
    if (litara == gInv[playerid][item] && !GiveItem(playerid, ITEM_KANISTER, 1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
    GiveItem(playerid, item, -litara);
    gGorivo[v] += float(litara);
    if ((item == ITEM_KANISTER_DIZEL) != (JeDizel(model) != 0)) gKrivoGorivo[v] += float(litara);
    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, false, false, false, false, 0);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s toci gorivo iz kanistera.", GetRPIme(playerid), "", 1);
    return 1;
}

static VoziloTankaj(playerid, litara)
{
    if (!gUlogiran[playerid]) return 1;
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) return KanisterNapuni(playerid, litara);
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    new v = GetPlayerVehicleID(playerid), model = GetVehicleModel(v), dizel = JeDizel(model);
    if (BezMotora(model)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema motor."));
    if (!ImaKljuc(playerid, v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate kljuceve ovog vozila."));
    new pumpa = PumpaBlizu(playerid, dizel);
    if (pumpa == PUMPA_NEMA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod pumpe."));
    if (VoziloGet(v, VP_MOTOR) == 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ugasite motor prije tankanja."));
    new prostor = VoziloProstor(v);
    if (prostor < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Spremnik je pun."));
    new cijena = (pumpa == PUMPA_OK) == (dizel != 0) ? DIZEL_CIJENA : BENZIN_CIJENA, maks = min(prostor, gNovac[playerid] / cijena);
    if (maks < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    if (litara < 0)
    {
        new upozorenje[128];
        if (pumpa == PUMPA_KRIVA) format(upozorenje, sizeof upozorenje, T(playerid, "{FF6347}Ova pumpa toci %s, a vozilo koristi %s.\n\n{FFFFFF}"), dizel ? T(playerid, "Benzin") : T(playerid, "Dizel"), dizel ? T(playerid, "Dizel") : T(playerid, "Benzin"));
        return GorivoPitaj(playerid, GORIVO_PUMPA, 0, maks, upozorenje);
    }
    if (litara > maks) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najvise mozete %d L."), maks);
    DajNovac(playerid, -litara * cijena);
    gGorivo[v] += float(litara);
    if (pumpa == PUMPA_KRIVA) gKrivoGorivo[v] += float(litara);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s toci gorivo.", GetRPIme(playerid), "", 1);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Natocili ste %d L za $%d."), litara, litara * cijena);
}

CMD:tankaj(playerid, params[])
{
    return VoziloTankaj(playerid, -1);
}

DLG:gorivo(playerid, response, const inputtext[])
{
    if (!response) return 1;
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 4 || !strval(inputtext)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna kolicina."));
    new n = strval(inputtext);
    switch (gGorivoNacin[playerid])
    {
        case GORIVO_PUMPA: if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) VoziloTankaj(playerid, n);
        case GORIVO_PUNI: KanisterNapuni(playerid, n);
        case GORIVO_TOCI: VoziloKanister(playerid, gGorivoItem[playerid], n);
    }
    return 1;
}

VoziloIsprazni(playerid)
{
    if (IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti pjeske."));
    new v = VoziloBlizu(playerid);
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod vozila."));
    if (BezMotora(GetVehicleModel(v))) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo vozilo nema motor."));
    if (!ImaKljuc(playerid, v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate kljuceve ovog vozila."));
    if (gGorivo[v] <= 0.0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Spremnik je prazan."));
    gGorivo[v] = 0.0;
    gKrivoGorivo[v] = 0.0;
    gKrivoVoznja[v] = 0;
    VoziloSet(v, VP_MOTOR, 0);
    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, false, false, false, false, 0);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pretace gorivo iz vozila.", GetRPIme(playerid), "", 1);
    return 1;
}

CMD:engine(playerid, params[]) return cmd_motor(playerid, params);
CMD:lights(playerid, params[]) return cmd_svjetla(playerid, params);
CMD:lock(playerid, params[]) return cmd_zakljucaj(playerid, params);
CMD:steal(playerid, params[]) return cmd_ukradi(playerid, params);
CMD:belt(playerid, params[]) return cmd_pojas(playerid, params);
CMD:window(playerid, params[]) return cmd_prozor(playerid, params);
CMD:vinfo(playerid, params[]) return cmd_prometna(playerid, params);
CMD:refuel(playerid, params[]) return cmd_tankaj(playerid, params);
