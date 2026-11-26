#define FPS_MODEL           19300
#define FPS_MS              100
#define FPS_VOZILO_KUT      -0.42

enum { FM_NEMA = -1, FM_STOJI, FM_TRCI, FM_CUCI, FM_VOZILO }

static gFpsObj[MAX_PLAYERS] = { INVALID_OBJECT_ID, ... }, bool:gFps[MAX_PLAYERS], bool:gFpsPrisilno[MAX_PLAYERS], bool:gFpsPrije[MAX_PLAYERS];
static gFpsNacin[MAX_PLAYERS] = { FM_NEMA, ... }, gFpsVozilo[MAX_PLAYERS], gFpsSjedalo[MAX_PLAYERS], bool:gFpsTimer, bool:gFpsNisani[MAX_PLAYERS];

static bool:FpsMotor(model)
{
    switch (model)
    {
        case 448, 461, 462, 463, 468, 471, 481, 509, 510, 521, 522, 523, 581, 586: return true;
    }
    return false;
}

static FpsNacin(playerid, &vozilo, &sjedalo)
{
    vozilo = 0;
    sjedalo = -1;
    new PLAYER_STATE:st = GetPlayerState(playerid);
    if (st == PLAYER_STATE_DRIVER || st == PLAYER_STATE_PASSENGER)
    {
        vozilo = GetPlayerVehicleID(playerid);
        sjedalo = GetPlayerVehicleSeat(playerid);
        return FM_VOZILO;
    }
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DUCK) return FM_CUCI;
    new Float:vx, Float:vy, Float:vz;
    GetPlayerVelocity(playerid, vx, vy, vz);
    return vx * vx + vy * vy > 0.01 ? FM_TRCI : FM_STOJI;
}

static FpsMakni(playerid)
{
    if (gFpsObj[playerid] == INVALID_OBJECT_ID) return;
    DestroyPlayerObject(playerid, gFpsObj[playerid]);
    gFpsObj[playerid] = INVALID_OBJECT_ID;
}

static FpsPrimijeni(playerid)
{
    FpsMakni(playerid);
    gFpsNacin[playerid] = FM_NEMA;
    if (!gFps[playerid])
    {
        Lik_PrikaceniSebi(playerid, true);
        SetCameraBehindPlayer(playerid);
        return;
    }
    Lik_PrikaceniSebi(playerid, false);
    new vozilo, sjedalo, nacin = FpsNacin(playerid, vozilo, sjedalo), Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    gFpsObj[playerid] = CreatePlayerObject(playerid, FPS_MODEL, x, y, z, 0.0, 0.0, 0.0);
    if (nacin == FM_VOZILO && vozilo)
    {
        new Float:sx, Float:sy, Float:sz, model = GetVehicleModel(vozilo);
        GetVehicleModelInfo(model, sjedalo >= 2 ? VEHICLE_MODEL_INFO_REARSEAT : VEHICLE_MODEL_INFO_FRONTSEAT, sx, sy, sz);
        sx = floatabs(sx);
        if (sjedalo == 0 || sjedalo == 2) sx = -sx;
        if (FpsMotor(model)) sx = 0.0;
        AttachPlayerObjectToVehicle(playerid, gFpsObj[playerid], vozilo, sx, sy - 0.18, sz + 0.74, 0.0, 0.0, 0.0);
    }
    else
    {
        new Float:naprijed = nacin == FM_TRCI ? 0.32 : nacin == FM_CUCI ? 0.30 : 0.18, Float:visina = nacin == FM_CUCI ? 0.16 : nacin == FM_TRCI ? 0.64 : 0.70;
        AttachPlayerObjectToPlayer(playerid, gFpsObj[playerid], playerid, 0.0, naprijed, visina, 0.0, 0.0, 0.0);
    }
    gFpsNisani[playerid] = false;
    AttachCameraToPlayerObject(playerid, gFpsObj[playerid]);
    gFpsNacin[playerid] = nacin;
    gFpsVozilo[playerid] = vozilo;
    gFpsSjedalo[playerid] = sjedalo;
}

forward FpsTick();
public FpsTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gFps[i] || !gUlogiran[i]) continue;
        new PLAYER_STATE:st = GetPlayerState(i);
        if (st != PLAYER_STATE_ONFOOT && st != PLAYER_STATE_DRIVER && st != PLAYER_STATE_PASSENGER) continue;
        new vozilo, sjedalo, nacin = FpsNacin(i, vozilo, sjedalo);
        if (nacin != gFpsNacin[i] || vozilo != gFpsVozilo[i] || sjedalo != gFpsSjedalo[i])
        {
            FpsPrimijeni(i);
            continue;
        }
        if (gFpsObj[i] == INVALID_OBJECT_ID) continue;
        if (nacin == FM_VOZILO && vozilo)
        {
            new Float:fx, Float:fy, Float:fz, Float:kut;
            GetPlayerCameraFrontVector(i, fx, fy, fz);
            GetVehicleZAngle(vozilo, kut);
            new Float:d = floatsqroot(fx * fx + fy * fy);
            if (d > 0.1 && (fx * -floatsin(kut, degrees) + fy * floatcos(kut, degrees)) / d < FPS_VOZILO_KUT) AttachCameraToPlayerObject(i, gFpsObj[i]);
            continue;
        }
        new KEY:tipke, gore, strana;
        GetPlayerKeys(i, tipke, gore, strana);
        new bool:nisani = (tipke & KEY_HANDBRAKE) != KEY:0 && GetPlayerWeapon(i) >= WEAPON_COLT45;
        if (nisani == gFpsNisani[i]) continue;
        gFpsNisani[i] = nisani;
        if (nisani) SetCameraBehindPlayer(i);
        else AttachCameraToPlayerObject(i, gFpsObj[i]);
    }
}

static FpsPokreniTimer()
{
    if (gFpsTimer) return;
    gFpsTimer = true;
    SetTimer("FpsTick", FPS_MS, true);
}

FpsPrisili(playerid, bool:ukljuci)
{
    if (ukljuci == gFpsPrisilno[playerid]) return;
    gFpsPrisilno[playerid] = ukljuci;
    if (ukljuci) gFpsPrije[playerid] = gFps[playerid];
    gFps[playerid] = ukljuci ? true : gFpsPrije[playerid];
    FpsPokreniTimer();
    FpsPrimijeni(playerid);
}

FpsSpawn(playerid)
{
    if (gFps[playerid]) FpsPrimijeni(playerid);
}

FpsDisconnect(playerid)
{
    gFpsObj[playerid] = INVALID_OBJECT_ID;
    gFps[playerid] = false;
    gFpsPrisilno[playerid] = false;
    gFpsPrije[playerid] = false;
    gFpsNacin[playerid] = FM_NEMA;
}

CMD:fps(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (gFpsPrisilno[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dok radite u kombiju kamera je u prvom licu."));
    gFps[playerid] = !gFps[playerid];
    FpsPokreniTimer();
    FpsPrimijeni(playerid);
    return SendClientMessage(playerid, COLOR_INFO, gFps[playerid] ? T(playerid, "Kamera u prvom licu je ukljucena (/fps za iskljucivanje).") : T(playerid, "Kamera u prvom licu je iskljucena."));
}
