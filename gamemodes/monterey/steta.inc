// HP i armor, serverside, koristi syncfix plugin
#define STETA_BAZA          8000000.0
#define STETA_SYNC_MS       2000
#define STETA_BLOK_MS       1000
#define STETA_ORUZJA        39
#define STETA_NOZ_KRADOM    1000.0
#define STETA_NOZ_MS        3000
#define STETA_VOZILO_GORI   250.0
#define STETA_VOZILO_MIN    300.0
#define STETA_DIO_POSTO     35
#define STETA_GUMA_DOMET    0.6
#define STETA_TD_X          546.0
#define STETA_TD_Y          66.7
#define STETA_TD_W          61.7
#define STETA_TD_H          8.4

enum E_STETA { Float:stDmg, Float:stDomet, stRazmak, bool:stKlijent }
static const gSteta[STETA_ORUZJA][E_STETA] = {
    { 5.0, 3.0, 150, true },    // fist
    { 5.0, 3.0, 150, true },    // brass knuckles
    { 8.0, 3.0, 150, true },    // golf club
    { 8.0, 3.0, 150, true },    // nightstick
    { 8.0, 3.0, 150, true },    // knife
    { 8.0, 3.0, 150, true },    // bat
    { 8.0, 3.0, 150, true },    // shovel
    { 8.0, 3.0, 150, true },    // pool cue
    { 10.0, 3.0, 150, true },   // katana
    { 27.0, 3.0, 50, true },    // chainsaw
    { 5.0, 3.0, 150, true }, { 5.0, 3.0, 150, true }, { 5.0, 3.0, 150, true }, { 5.0, 3.0, 150, true }, { 5.0, 3.0, 150, true }, { 5.0, 3.0, 150, true }, // dildos, flowers, cane
    { 0.0, 0.0, 0, false }, { 0.0, 0.0, 0, false }, { 0.0, 0.0, 0, false }, { 0.0, 0.0, 0, false }, { 0.0, 0.0, 0, false }, { 0.0, 0.0, 0, false }, // explosives, fire, unused
    { 8.25, 35.0, 120, false },  // colt 45
    { 13.2, 35.0, 120, false },  // silenced
    { 46.2, 35.0, 250, false },  // desert eagle
    { 49.5, 40.0, 500, true },   // shotgun (15 pellets x 3.3)
    { 49.5, 35.0, 120, true },   // sawn-off
    { 39.6, 40.0, 150, true },   // combat shotgun (8 x 4.95)
    { 6.6, 35.0, 40, false },    // uzi
    { 8.25, 45.0, 50, false },   // mp5
    { 9.9, 70.0, 80, false },    // ak-47
    { 9.9, 90.0, 80, false },    // m4
    { 6.6, 35.0, 40, false },    // tec-9
    { 24.75, 100.0, 500, false }, // rifle
    { 41.25, 300.0, 500, false }, // sniper
    { 0.0, 0.0, 0, false }, { 0.0, 0.0, 0, false }, { 0.0, 0.0, 0, false }, // rpg, heat seeker, flamethrower
    { 46.2, 75.0, 20, false }    // minigun
};

static Float:gZivot[MAX_PLAYERS], Float:gOklop[MAX_PLAYERS], gStetaPrikaz[MAX_PLAYERS], gStetaPostavljeno[MAX_PLAYERS], gStetaBlok[MAX_PLAYERS];
static gStetaZadnji[MAX_PLAYERS][MAX_PLAYERS];
static PlayerText:gStetaTd[MAX_PLAYERS][3], bool:gStetaTdVidljiv[MAX_PLAYERS];
static gStetaNoz[MAX_PLAYERS], gStetaVozilo[MAX_PLAYERS];

InitSteta()
{
    new Float:z = 100.0, Float:o = 10.0;
    StetaRacun(z, o, 25.0, true);
    assert(z == 85.0 && o == 0.0);
    z = 100.0, o = 50.0;
    StetaRacun(z, o, 25.0, false);
    assert(z == 75.0 && o == 50.0);
    assert(StetaPrikazZa(150.0) == 100 && StetaPrikazZa(42.3) == 43 && StetaPrikazZa(-5.0) == 0 && StetaPrikazZa(FLOAT_INFINITY) == 100);
    assert(gSteta[_:WEAPON_DEAGLE][stDmg] == 46.2 && gSteta[_:WEAPON_MINIGUN][stDomet] == 75.0 && gSteta[_:WEAPON_GRENADE][stDmg] == 0.0);
    assert(STETA_BAZA + 100.0 - STETA_BAZA == 100.0);
    new p = 0x00300000;
    StetaRazina(p, 5);
    assert(p == 0x00300000);
    StetaRazina(p, 4);
    StetaRazina(p, 4);
    assert(p == 0x00320000);
}

static StetaPrikazZa(Float:z)
{
    if (z <= 0.0) return 0;
    if (z >= 100.0) return 100;
    return floatround(z, floatround_ceil);
}

// HP

static PlayerText:StetaKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    return td;
}

static StetaTraka(playerid)
{
    if (gStetaTd[playerid][0] == PlayerText:INVALID_TEXT_DRAW)
    {
        gStetaTd[playerid][0] = StetaKutija(playerid, STETA_TD_X, STETA_TD_Y, STETA_TD_W, STETA_TD_H, 0x000000FF);
        gStetaTd[playerid][1] = StetaKutija(playerid, STETA_TD_X + 2.0, STETA_TD_Y + 1.9, STETA_TD_W - 3.7, STETA_TD_H - 3.5, 0x5A0C0EFF);
        gStetaTd[playerid][2] = StetaKutija(playerid, STETA_TD_X + 2.0, STETA_TD_Y + 1.9, STETA_TD_W - 3.7, STETA_TD_H - 3.5, 0xB4191DFF);
        gStetaTdVidljiv[playerid] = false;
    }
    PlayerTextDrawTextSize(playerid, gStetaTd[playerid][2], (STETA_TD_W - 3.7) * float(gStetaPrikaz[playerid]) / 100.0, STETA_TD_H - 3.5);
    if (gStetaTdVidljiv[playerid]) PlayerTextDrawShow(playerid, gStetaTd[playerid][2]);
}

static StetaTrakaVidljiva(playerid, bool:vidljiva)
{
    if (gStetaTd[playerid][0] == PlayerText:INVALID_TEXT_DRAW || gStetaTdVidljiv[playerid] == vidljiva) return;
    gStetaTdVidljiv[playerid] = vidljiva;
    for (new i = 0; i < 3; i++)
    {
        if (vidljiva) PlayerTextDrawShow(playerid, gStetaTd[playerid][i]);
        else PlayerTextDrawHide(playerid, gStetaTd[playerid][i]);
    }
}

// HP i armor

bool:SetHP(playerid, Float:z)
{
    if (IsPlayerNPC(playerid)) return SetPlayerHealth(playerid, z);
    gZivot[playerid] = z;
    gStetaPrikaz[playerid] = IsRanjen(playerid) ? 0 : StetaPrikazZa(z);
    gStetaPostavljeno[playerid] = GetTickCount();
    StetaTraka(playerid);
    SetPVarInt(playerid, "steta_hp", gStetaPrikaz[playerid]);
    return SetPlayerHealth(playerid, STETA_BAZA + float(gStetaPrikaz[playerid]));
}

bool:SetArmor(playerid, Float:o)
{
    if (!IsPlayerNPC(playerid))
    {
        gOklop[playerid] = o;
        gStetaPostavljeno[playerid] = GetTickCount();
    }
    return SetPlayerArmour(playerid, o);
}

bool:GetHP(playerid, &Float:z)
{
    if (IsPlayerNPC(playerid)) return GetPlayerHealth(playerid, z);
    z = gZivot[playerid];
    return true;
}

bool:GetArmor(playerid, &Float:o)
{
    if (IsPlayerNPC(playerid)) return GetPlayerArmour(playerid, o);
    o = gOklop[playerid];
    return true;
}

StetaConnect(playerid)
{
    for (new i = 0; i < 3; i++) gStetaTd[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
    gStetaTdVidljiv[playerid] = false;
    gStetaPrikaz[playerid] = 0;
    gStetaNoz[playerid] = 0;
}

StetaSpawn(playerid)
{
    SetHP(playerid, 100.0);
    SetArmor(playerid, 0.0);
    StetaTrakaVidljiva(playerid, true);
}

StetaBlokiraj(playerid)
{
    gStetaBlok[playerid] = GetTickCount();
}

// DMG

static StetaRacun(&Float:zivot, &Float:oklop, Float:dmg, bool:krozOklop)
{
    if (krozOklop && oklop > 0.0)
    {
        new Float:upio = dmg < oklop ? dmg : oklop;
        oklop -= upio;
        dmg -= upio;
    }
    zivot -= dmg;
}

static bool:StetaUNozu(playerid)
{
    return gStetaNoz[playerid] && GetTickCount() - gStetaNoz[playerid] < STETA_NOZ_MS + 500;
}

static StetaPrimijeni(playerid, Float:dmg, WEAPON:w)
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || ZatvorProvjera(playerid) || StetaUNozu(playerid) || dmg <= 0.0) return;
    StetaRacun(gZivot[playerid], gOklop[playerid], dmg, w != WEAPON_COLLISION && w != WEAPON_DROWN);
    SetArmor(playerid, gOklop[playerid]);
    if (RanjenProvjeri(playerid, gZivot[playerid])) return;
    SetHP(playerid, gZivot[playerid]);
}

// stealth kill
static StetaNoz(zrtva, napadac)
{
    if (!gUlogiran[zrtva] || IsRanjen(zrtva) || ZatvorProvjera(zrtva) || StetaUNozu(zrtva)) return;
    gStetaNoz[zrtva] = GetTickCount();
    SetPlayerHealth(zrtva, Float:0x7F7FFFFF);
    new Float:a;
    GetPlayerFacingAngle(zrtva, a);
    SetPlayerFacingAngle(napadac, a);
    SetPlayerVelocity(zrtva, 0.0, 0.0, 0.0);
    SetPlayerVelocity(napadac, 0.0, 0.0, 0.0);
    ApplyAnimation(zrtva, "KNIFE", "KILL_Knife_Ped_Die", 4.1, false, true, true, true, 0, SYNC_ALL);
    ApplyAnimation(napadac, "KNIFE", "KILL_Knife_Player", 4.1, false, true, true, false, 1800, SYNC_ALL);
    SetTimerEx("StetaNozKraj", STETA_NOZ_MS, false, "ii", zrtva, gAccountId[zrtva]);
}

forward StetaNozKraj(playerid, racun);
public StetaNozKraj(playerid, racun)
{
    if (gAccountId[playerid] != racun || !gUlogiran[playerid]) return;
    gStetaNoz[playerid] = 0;
    gZivot[playerid] = 0.0;
    RanjenProvjeri(playerid, 0.0);
}

static bool:NapadValidanProvjera(playerid, damagedid, WEAPON:weaponid)
{
    new i = _:weaponid;
    if (damagedid == playerid || !IsPlayerConnected(damagedid) || !gUlogiran[damagedid] || !gUlogiran[playerid]) return false;
    if (i < 0 || i >= STETA_ORUZJA || gSteta[i][stDmg] == 0.0 || IsRanjen(playerid) || ImaLisice(playerid) || !OruzjeLegit(playerid, weaponid)) return false;
    if (GetTickCount() - gStetaBlok[playerid] < STETA_BLOK_MS || GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(damagedid)) return false;
    new Float:x, Float:y, Float:z;
    GetPlayerPos(damagedid, x, y, z);
    return IsPlayerInRangeOfPoint(playerid, gSteta[i][stDomet] + (gSteta[i][stDomet] <= 3.0 ? 2.0 : 10.0), x, y, z);
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, WEAPON:weaponid, bodypart)
{
    new i = _:weaponid, sad = GetTickCount();
    if (IsPlayerNPC(damagedid)) return DilerPogodak(playerid, damagedid);
    if (!NapadValidanProvjera(playerid, damagedid, weaponid)) return 1;
    if (weaponid == WEAPON_KNIFE && amount >= STETA_NOZ_KRADOM)
    {
        StetaNoz(damagedid, playerid);
        return 1;
    }
    if (sad - gStetaZadnji[playerid][damagedid] < gSteta[i][stRazmak]) return 1;
    gStetaZadnji[playerid][damagedid] = sad;
    new Float:dmg = gSteta[i][stDmg];
    if (gSteta[i][stKlijent] && amount < dmg) dmg = amount;
    StetaPrimijeni(damagedid, dmg, weaponid);
    return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
    MobitelZatvori(playerid);
    new i = _:weaponid;
    if (weaponid == WEAPON_KNIFE && amount == 0.0 && issuerid != INVALID_PLAYER_ID)
    {
        if (NapadValidanProvjera(issuerid, playerid, weaponid)) StetaNoz(playerid, issuerid);
        return 1;
    }
    if (issuerid != INVALID_PLAYER_ID && i >= 0 && i < STETA_ORUZJA && gSteta[i][stDmg] > 0.0) return 1;
    if (weaponid == WEAPON_COLLISION && 1061 <= GetPlayerAnimationIndex(playerid) <= 1067) return 1;
    StetaPrimijeni(playerid, amount, weaponid);
    return 1;
}

static StetaRazina(&panel, n)
{
    new s = n * 4, l = (panel >> s) & 0xF;
    if (l < 3) panel = (panel & ~(0xF << s)) | ((l + 1) << s);
}

static bool:StetaGuma(model, Float:fx, Float:fy, Float:fz, &gume)
{
    new Float:wx, Float:wy, Float:wz;
    for (new prednja = 0; prednja < 2; prednja++)
    {
        GetVehicleModelInfo(model, prednja ? VEHICLE_MODEL_INFO_WHEELSFRONT : VEHICLE_MODEL_INFO_WHEELSREAR, wx, wy, wz);
        if (VectorSize(floatabs(fx) - floatabs(wx), fy - wy, fz - wz) > STETA_GUMA_DOMET) continue;
        new bool:desno = fx > 0.0 || floatabs(wx) < 0.1;
        gume |= prednja ? (desno ? 2 : 8) : (desno ? 1 : 4);
        return true;
    }
    return false;
}

static StetaVoziloDio(v, Float:fx, Float:fy, Float:fz)
{
    new VEHICLE_PANEL_STATUS:pan, VEHICLE_DOOR_STATUS:vrata, VEHICLE_LIGHT_STATUS:svj, VEHICLE_TYRE_STATUS:gum, model = GetVehicleModel(v), Float:sx, Float:sy, Float:sz;
    GetVehicleDamageStatus(v, pan, vrata, svj, gum);
    new p = _:pan, d = _:vrata, l = _:svj, t = _:gum;
    if (!StetaGuma(model, fx, fy, fz, t))
    {
        if (random(100) >= STETA_DIO_POSTO) return;
        GetVehicleModelInfo(model, VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
        new dio = random(3);
        if (fy > sy * 0.25)
        {
            if (fz > sz * 0.1 && dio == 0) StetaRazina(p, 4);   // soferka
            else if (dio == 1) d |= 0x00000002;                 // hauba
            else if (dio == 2) l |= fx < 0.0 ? 1 : 4;           // prednja svjetla
            else StetaRazina(p, 5);                             // prednji bumper
        }
        else if (fy < -sy * 0.25)
        {
            if (dio == 0) StetaRazina(p, 6);                    // zadnji bumper
            else if (dio == 1) d |= 0x00000200;                 // gepek
            else l |= 64;                                       // zadnja svjetla
        }
        else if (fx < 0.0)
        {
            if (dio == 0) d |= 0x00020000;                      // vrata vozaca
            else StetaRazina(p, fy > 0.0 ? 0 : 2);              // lijeva strana
        }
        else
        {
            if (dio == 0) d |= 0x02000000;                      // vrata suvozaca
            else StetaRazina(p, fy > 0.0 ? 1 : 3);              // desna strana
        }
    }
    UpdateVehicleDamageStatus(v, VEHICLE_PANEL_STATUS:p, VEHICLE_DOOR_STATUS:d, VEHICLE_LIGHT_STATUS:l, VEHICLE_TYRE_STATUS:t);
}

static bool:StetaImaVozaca(v)
{
    for (new p = 0; p < MAX_PLAYERS; p++) if (IsPlayerConnected(p) && GetPlayerState(p) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(p) == v) return true;
    return false;
}

StetaVozilo(playerid, WEAPON:w, BULLET_HIT_TYPE:hittype, v, Float:fx, Float:fy, Float:fz)
{
    new i = _:w, sad = GetTickCount(), Float:x, Float:y, Float:z, Float:h;
    if (hittype != BULLET_HIT_TYPE_VEHICLE || i < 0 || i >= STETA_ORUZJA || gSteta[i][stDomet] <= 3.0 || !gUlogiran[playerid]) return;
    if (IsRanjen(playerid) || ImaLisice(playerid) || !OruzjeLegit(playerid, w) || sad - gStetaVozilo[playerid] < gSteta[i][stRazmak]) return;
    if (StetaImaVozaca(v)) return;
    GetVehiclePos(v, x, y, z);
    if (GetVehicleVirtualWorld(v) != GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, gSteta[i][stDomet] + 10.0, x, y, z)) return;
    gStetaVozilo[playerid] = sad;
    GetVehicleHealth(v, h);
    h -= gSteta[i][stDmg];
    if (h < STETA_VOZILO_MIN) h = STETA_VOZILO_MIN;
    SetVehicleHealth(v, h);
    StetaVoziloDio(v, fx, fy, fz);
}

StetaTick()
{
    new sad = GetTickCount(), Float:o, Float:h;
    for (new n = 0; n < gVozilaCount; n++)
    {
        new v = gVozila[n];
        if (GetVehicleHealth(v, h) && h < STETA_VOZILO_MIN && !StetaImaVozaca(v)) SetVehicleHealth(v, STETA_VOZILO_MIN);
    }
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i]) continue;
        new PLAYER_STATE:st = GetPlayerState(i);
        new bool:ziv = st == PLAYER_STATE_ONFOOT || st == PLAYER_STATE_DRIVER || st == PLAYER_STATE_PASSENGER;
        StetaTrakaVidljiva(i, ziv);
        if ((st == PLAYER_STATE_DRIVER || st == PLAYER_STATE_PASSENGER) && !IsRanjen(i) && !ZatvorProvjera(i))
        {
            GetVehicleHealth(GetPlayerVehicleID(i), h);
            if (h < STETA_VOZILO_GORI)
            {
                gZivot[i] = 0.0;
                RanjenProvjeri(i, 0.0);
                continue;
            }
        }
        if (!ziv || sad - gStetaPostavljeno[i] < STETA_SYNC_MS) continue;
        GetPlayerArmour(i, o);
        if (o > gOklop[i] + 1.0) SetPlayerArmour(i, gOklop[i]);
    }
}

#define SetPlayerHealth SetHP
#define SetPlayerArmour SetArmor
#define GetPlayerHealth GetHP
#define GetPlayerArmour GetArmor
