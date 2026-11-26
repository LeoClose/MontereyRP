public OnLikStigao(lik)
{
    if (PlaninaStigao(lik)) return 1;
    if (FirmaStigao(lik)) return 1;
    if (RestoranStigao(lik)) return 1;
    if (SkladisteStigao(lik)) return 1;
    if (KupacStigao(lik)) return 1;
    if (SlucajNpcStigao(lik)) return 1;
    if (KurvaPomak(lik)) return 1;
    if (TaksiLikStigao(lik)) return 1;
    if (BankaLikStigao(lik)) return 1;
    return 1;
}

public OnLikPogoden(lik, playerid, Float:steta, oruzje, dio)
{
    if (!gUlogiran[playerid] || IsShootingBlocked(playerid)) return 0;
    new r = KupacPogoden(lik, playerid, steta, WEAPON:oruzje);
    if (r != -1) return r;
    r = DilerPogodak(playerid, lik);
    if (r != -1) return r;
    return CivilPogoden(lik, playerid);
}

public OnLikPogodio(lik, playerid, Float:steta, oruzje, dio)
{
    StetaOdLika(playerid, WEAPON:oruzje, dio);
    return 1;
}

public OnLikSmiren(lik)
{
    KupacSmiren(lik);
    return 1;
}

public OnLikRanjen(lik, napadac)
{
    #pragma unused napadac
    if (!KupacRanjen(lik)) CivilRanjen(lik);
    return 1;
}

public OnLikSnimkaKraj(lik)
{
    if (!HeliSnimkaKraj(lik) && !AvionPlaybackEnd(lik)) VlakPlaybackEnd(lik);
    return 1;
}

public OnLikPutnik(lik, playerid)
{
    #pragma unused playerid
    return TaksiJeLik(lik);
}

public OnLikIzbacen(lik, playerid)
{
    return 0;
}

public OnLikUmro(lik, ubojica, oruzje)
{
    if (KupacSmrt(lik, ubojica)) return 1;
    if (DilerUmro(lik)) return 1;
    CivilUmro(lik, ubojica);
    return 1;
}
