forward bool:TuningTrubaKljuc(playerid, v, bool:pritisnuta);

const SYNC_VOZAC = 200, SYNC_NISAN = 203, SYNC_PJESKE = 207, SYNC_PUTNIK = 211, SYNC_GLEDA = 212;

#define SYNC_PJESKE_BRZINA  5.0
#define SYNC_VOZILO_BRZINA  3.0
#define SYNC_TRUBA          2
#define SYNC_SPACE          8

static gSyncOruzje[MAX_PLAYERS], gSyncNaocale[MAX_PLAYERS], gSyncNaocaleMs[MAX_PLAYERS], gSyncGledaTipke[MAX_PLAYERS];

SyncConnect(playerid)
{
    gSyncOruzje[playerid] = gSyncNaocale[playerid] = gSyncGledaTipke[playerid] = 0;
}

static bool:SyncBezPucanja(a)
{
    return (1222 <= a <= 1236) || a == 1249 || (1275 <= a <= 1287) || a == 459 || (908 <= a <= 909) || a == 1274 || a == 1266 || (1241 <= a <= 1242)
        || (17 <= a <= 27) || (745 <= a <= 760) || (1545 <= a <= 1554) || (471 <= a <= 507) || (1135 <= a <= 1151);
}

static SyncNePuca(k)
{
    if (k & _:KEY_HANDBRAKE) k &= ~_:KEY_ACTION;
    return k & ~(_:KEY_FIRE | _:KEY_HANDBRAKE);
}

static SyncTipke(playerid, k, w, a, sad)
{
    gSyncOruzje[playerid] = w;
    if (w == _:WEAPON_BOMB) k &= ~_:KEY_HANDBRAKE;
    if ((_:WEAPON_COLT45 <= w <= _:WEAPON_SNIPER) || w == _:WEAPON_MINIGUN)
    {
        if (SyncBezPucanja(a)) k = SyncNePuca(k);
    }
    else if (w == _:WEAPON_SPRAYCAN || w == _:WEAPON_FIREEXTINGUISHER || w == _:WEAPON_FLAMETHROWER)
    {
        if (!(1160 <= a <= 1167)) k = SyncNePuca(k);
    }
    else if (w == _:WEAPON_GRENADE && !(644 <= a <= 646)) k &= ~_:KEY_ACTION;
    if (w == _:WEAPON_NIGHT_VISION_GOGGLES || w == _:WEAPON_THERMAL_GOGGLES)
    {
        k &= ~_:KEY_FIRE;
        gSyncNaocale[playerid] = 1;
        gSyncNaocaleMs[playerid] = sad;
    }
    else if (gSyncNaocale[playerid])
    {
        if (gSyncNaocale[playerid] == 2 && sad - gSyncNaocaleMs[playerid] > 40) gSyncNaocale[playerid] = 0;
        else
        {
            k &= ~_:KEY_FIRE;
            gSyncNaocale[playerid] = 2;
            gSyncNaocaleMs[playerid] = sad;
        }
    }
    return k;
}

static bool:SyncPrvoLice(w)
{
    return (_:WEAPON_SNIPER <= w <= _:WEAPON_HEATSEEKER) || w == _:WEAPON_CAMERA;
}

static bool:SyncBrzinaOk(Float:x, Float:y, Float:z, Float:najvise)
{
    new Float:v = VectorSize(x, y, z);
    return v == v && v <= najvise;
}

InitSync()
{
    new t = MAX_PLAYERS - 1, k = _:KEY_FIRE | _:KEY_HANDBRAKE | _:KEY_ACTION;
    assert(SyncTipke(t, k, 31, 1231, 0) == 0 && gSyncOruzje[t] == 31);
    assert(SyncTipke(t, _:KEY_FIRE | _:KEY_HANDBRAKE, 31, 1189, 0) == (_:KEY_FIRE | _:KEY_HANDBRAKE));
    assert(SyncTipke(t, _:KEY_HANDBRAKE | _:KEY_FIRE, _:WEAPON_BOMB, 1189, 0) == _:KEY_FIRE);
    assert(SyncTipke(t, _:KEY_FIRE, _:WEAPON_SPRAYCAN, 1189, 0) == 0 && SyncTipke(t, _:KEY_FIRE, _:WEAPON_SPRAYCAN, 1161, 0) == _:KEY_FIRE);
    assert(SyncTipke(t, _:KEY_ACTION, _:WEAPON_GRENADE, 1189, 0) == 0);
    assert(SyncTipke(t, _:KEY_FIRE, _:WEAPON_NIGHT_VISION_GOGGLES, 0, 100) == 0 && gSyncNaocale[t] == 1);
    assert(SyncTipke(t, _:KEY_FIRE, 0, 0, 110) == 0 && gSyncNaocale[t] == 2);
    assert(SyncTipke(t, _:KEY_FIRE, 0, 0, 200) == _:KEY_FIRE && gSyncNaocale[t] == 0);
    assert(SyncPrvoLice(34) && SyncPrvoLice(43) && !SyncPrvoLice(31));
    assert(SyncBrzinaOk(1.0, 1.0, 1.0, SYNC_VOZILO_BRZINA) && !SyncBrzinaOk(Float:0x7FC00000, 0.0, 0.0, SYNC_VOZILO_BRZINA) && !SyncBrzinaOk(3.0, 0.0, 0.5, SYNC_VOZILO_BRZINA));
    SyncConnect(t);
}

IPacket:SYNC_PJESKE(playerid, BitStream:bs)
{
    new d[PR_OnFootSync];
    BS_IgnoreBits(bs, 8);
    BS_ReadOnFootSync(bs, d);
    if (!SyncBrzinaOk(d[PR_velocity][0], d[PR_velocity][1], d[PR_velocity][2], SYNC_PJESKE_BRZINA))
    {
        OnCheatDetected(playerid, "", 0, AC_RVANKA);
        return 0;
    }
    d[PR_keys] = SyncTipke(playerid, d[PR_keys], d[PR_weaponId], d[PR_animationId], GetTickCount());
    d[PR_health] = StetaPrikaz(playerid);
    BS_SetWriteOffset(bs, 8);
    BS_WriteOnFootSync(bs, d);
    return 1;
}

IPacket:SYNC_VOZAC(playerid, BitStream:bs)
{
    new d[PR_InCarSync];
    BS_IgnoreBits(bs, 8);
    BS_ReadInCarSync(bs, d);
    SkladisteSyncVilice(playerid, d[PR_vehicleId], _:d[PR_trainSpeed]);
    if (!SyncBrzinaOk(d[PR_velocity][0], d[PR_velocity][1], d[PR_velocity][2], SYNC_VOZILO_BRZINA))
    {
        OnCheatDetected(playerid, "", 0, AC_RVANKA);
        return 0;
    }
    d[PR_playerHealth] = StetaPrikaz(playerid);
    if (TuningTrubaKljuc(playerid, d[PR_vehicleId], (d[PR_keys] & SYNC_TRUBA) != 0)) d[PR_keys] &= ~SYNC_TRUBA;
    BS_SetWriteOffset(bs, 8);
    BS_WriteInCarSync(bs, d);
    return 1;
}

IPacket:SYNC_PUTNIK(playerid, BitStream:bs)
{
    new d[PR_PassengerSync];
    BS_IgnoreBits(bs, 8);
    BS_ReadPassengerSync(bs, d);
    d[PR_playerHealth] = StetaPrikaz(playerid);
    BS_SetWriteOffset(bs, 8);
    BS_WritePassengerSync(bs, d);
    return 1;
}

IPacket:SYNC_NISAN(playerid, BitStream:bs)
{
    if (!SyncPrvoLice(gSyncOruzje[playerid])) return 1;
    new d[PR_AimSync];
    BS_IgnoreBits(bs, 8);
    BS_ReadAimSync(bs, d);
    new Float:z = -d[PR_camFrontVec][2];
    d[PR_aimZ] = z > 1.0 ? 1.0 : z < -1.0 ? -1.0 : z;
    BS_SetWriteOffset(bs, 8);
    BS_WriteAimSync(bs, d);
    return 1;
}

IPacket:SYNC_GLEDA(playerid, BitStream:bs)
{
    new d[PR_SpectatingSync];
    BS_IgnoreBits(bs, 8);
    BS_ReadSpectatingSync(bs, d);
    if ((d[PR_keys] & SYNC_SPACE) && !(gSyncGledaTipke[playerid] & SYNC_SPACE)) SpecProvjera(playerid);
    gSyncGledaTipke[playerid] = d[PR_keys];
    return 1;
}
