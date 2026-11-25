#define VRIJEME_SIRINA      45.815
#define VRIJEME_DUZINA      15.982
#define VRIJEME_SUMRAK      40

static gVrijemeRucno = -1, gVrijemeSekundi = 59, gVrijemeSat, gVrijemeMinuta;

static VrijemeDani(g, m, d)
{
    g -= m <= 2 ? 1 : 0;
    new era = g / 400, yoe = g - era * 400, doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
    return era * 146097 + yoe * 365 + yoe / 4 - yoe / 100 + doy - 719468;
}

static VrijemeDatum(dani, &g, &m, &d)
{
    new z = dani + 719468, era = z / 146097, doe = z - era * 146097;
    new yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365, doy = doe - (365 * yoe + yoe / 4 - yoe / 100), mp = (5 * doy + 2) / 153;
    d = doy - (153 * mp + 2) / 5 + 1;
    m = mp < 10 ? mp + 3 : mp - 9;
    g = yoe + era * 400 + (m <= 2 ? 1 : 0);
}

static VrijemeZadnjaNedjelja(g, m)
{
    new zadnji = VrijemeDani(g, m, 31);
    return zadnji - (zadnji + 4) % 7;
}

static VrijemePomak(ts)
{
    new g, m, d;
    VrijemeDatum(ts / 86400, g, m, d);
    new bool:ljeto = ts >= VrijemeZadnjaNedjelja(g, 3) * 86400 + 3600 && ts < VrijemeZadnjaNedjelja(g, 10) * 86400 + 3600;
    return ljeto ? 7200 : 3600;
}

SatTick()
{
    static zadnja = -1;
    new g, m, d, sat, minuta, vrijeme[8], datum[12];
    VrijemeLokalno(g, m, d, sat, minuta);
    if (minuta == zadnja) return;
    zadnja = minuta;
    format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
    format(datum, sizeof datum, "%02d.%02d.%d", d, m, g);
    RacunaloSat(vrijeme, datum);
    MobitelSat(vrijeme);
    MdtSat(vrijeme);
}

VrijemeLokalno(&g, &m, &d, &sat, &minuta)
{
    VrijemeTs(gettime(), g, m, d, sat, minuta);
}

VrijemeTs(ts, &g, &m, &d, &sat, &minuta)
{
    new lokalno = ts + VrijemePomak(ts), sekundi = lokalno % 86400;
    VrijemeDatum(lokalno / 86400, g, m, d);
    sat = sekundi / 3600;
    minuta = sekundi / 60 % 60;
}

static VrijemeSunce(&izlazak, &zalazak)
{
    new ts = gettime(), lokalno = ts + VrijemePomak(ts), g, m, d;
    VrijemeDatum(lokalno / 86400, g, m, d);
    new Float:n = float(lokalno / 86400 - VrijemeDani(g, 1, 1) + 1), Float:b = 360.0 / 365.0 * (n - 81.0);
    new Float:dekl = 23.44 * floatsin(b, degrees), Float:eot = 9.87 * floatsin(2.0 * b, degrees) - 7.53 * floatcos(b, degrees) - 1.5 * floatsin(b, degrees);
    new Float:cosh = (floatsin(-0.833, degrees) - floatsin(VRIJEME_SIRINA, degrees) * floatsin(dekl, degrees)) / (floatcos(VRIJEME_SIRINA, degrees) * floatcos(dekl, degrees));
    new Float:h = acos(cosh < -1.0 ? -1.0 : cosh > 1.0 ? 1.0 : cosh), Float:podne = 720.0 - 4.0 * VRIJEME_DUZINA - eot + float(VrijemePomak(ts) / 60);
    izlazak = floatround(podne - 4.0 * h);
    zalazak = floatround(podne + 4.0 * h);
}

static VrijemeIgra(t, izlazak, zalazak)
{
    new tocke[4][2];
    tocke[0][0] = izlazak - VRIJEME_SUMRAK;
    tocke[0][1] = 5 * 60;
    tocke[1][0] = izlazak;
    tocke[1][1] = 6 * 60 + 30;
    tocke[2][0] = zalazak;
    tocke[2][1] = 20 * 60;
    tocke[3][0] = zalazak + VRIJEME_SUMRAK;
    tocke[3][1] = 22 * 60;
    if (t < tocke[0][0]) t += 1440;
    for (new k = 0; k < 4; k++)
    {
        new a0 = tocke[k][0], a1 = k < 3 ? tocke[k + 1][0] : tocke[0][0] + 1440, b0 = tocke[k][1], b1 = k < 3 ? tocke[k + 1][1] : tocke[0][1] + 1440;
        if (t >= a0 && t < a1) return (b0 + (b1 - b0) * (t - a0) / (a1 - a0)) % 1440;
    }
    return 12 * 60;
}

VrijemeIgrac(playerid)
{
    if (UMarici(playerid)) return;
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) SetPlayerTime(playerid, 12, 0);
    else SetPlayerTime(playerid, gVrijemeSat, gVrijemeMinuta);
}

static VrijemeOsvjezi()
{
    if (gVrijemeRucno >= 0)
    {
        gVrijemeSat = gVrijemeRucno;
        gVrijemeMinuta = 0;
    }
    else
    {
        new g, m, d, sat, minuta, izlazak, zalazak;
        VrijemeLokalno(g, m, d, sat, minuta);
        VrijemeSunce(izlazak, zalazak);
        new igra = VrijemeIgra(sat * 60 + minuta, izlazak, zalazak);
        gVrijemeSat = igra / 60;
        gVrijemeMinuta = igra % 60;
    }
    SetWorldTime(gVrijemeSat);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerConnected(i) && !IsPlayerNPC(i)) VrijemeIgrac(i);
}

VrijemeTick()
{
    if (++gVrijemeSekundi < 60) return;
    gVrijemeSekundi = 0;
    VrijemeOsvjezi();
}

VrijemeRucno(sat)
{
    gVrijemeRucno = sat;
    VrijemeOsvjezi();
}

InitVrijeme()
{
    assert(VrijemeDani(1970, 1, 1) == 0 && VrijemeDani(2026, 11, 14) == 20771);
    new g, m, d;
    VrijemeDatum(20771, g, m, d);
    assert(g == 2026 && m == 11 && d == 14);
    assert(VrijemePomak(VrijemeDani(2026, 7, 1) * 86400) == 7200 && VrijemePomak(VrijemeDani(2026, 11, 14) * 86400) == 3600);
    new izlazak, zalazak;
    VrijemeSunce(izlazak, zalazak);
    assert(izlazak > 4 * 60 && izlazak < 9 * 60 && zalazak > 15 * 60 && zalazak < 22 * 60);
    assert(VrijemeIgra(16 * 60 + 20, 7 * 60 + 10, 16 * 60 + 20) == 20 * 60 && VrijemeIgra(7 * 60 + 10, 7 * 60 + 10, 16 * 60 + 20) == 6 * 60 + 30);
    VrijemeOsvjezi();
}
