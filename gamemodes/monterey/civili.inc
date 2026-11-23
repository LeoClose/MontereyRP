#define CIVIL_RANJEN_HP     30.0
#define CIVIL_NOVAC_SANSA   45
#define CIVIL_NOVAC_MIN     5
#define CIVIL_NOVAC_MAX     60
#define CIVIL_RANJENIH      48
#define CIVIL_USTANI_MS     45000

static gCivilNapadac[LIK_PACIJENT], bool:gCivilUnutra[LIK_PACIJENT], gCivilRanjen[CIVIL_RANJENIH] = { INVALID_LIK, ... };

CivilPostavi(lik, bool:unutra = false)
{
    if (lik < 0 || lik >= LIK_PACIJENT) return;
    gCivilNapadac[lik] = 0;
    gCivilUnutra[lik] = unutra;
    Lik_Ponasanje(lik, LIK_REAGIRA | LIK_IZBJEGAVA | LIK_RANJIV, CIVIL_RANJEN_HP);
}

static CivilNovac(lik)
{
    if (random(100) >= CIVIL_NOVAC_SANSA) return;
    new Float:x, Float:y, Float:z;
    if (!Lik_Pozicija(lik, x, y, z)) return;
    BaciNovac(x + float(random(11) - 5) / 10.0, y + float(random(11) - 5) / 10.0, z, 0, 0, CIVIL_NOVAC_MIN + random(CIVIL_NOVAC_MAX - CIVIL_NOVAC_MIN + 1));
}

static CivilMakniRanjenog(lik)
{
    for (new i = 0; i < CIVIL_RANJENIH; i++) if (gCivilRanjen[i] == lik) gCivilRanjen[i] = INVALID_LIK;
}

CivilPogoden(lik, playerid)
{
    if (lik < 0 || lik >= LIK_PACIJENT || IsPlayerNPC(playerid)) return 1;
    if (gCivilNapadac[lik] != gAccountId[playerid] + 1)
    {
        gCivilNapadac[lik] = gAccountId[playerid] + 1;
        DodajWanted(playerid);
    }
    return 1;
}

CivilRanjen(lik)
{
    if (lik < 0 || lik >= LIK_PACIJENT) return;
    CivilNovac(lik);
    if (gCivilUnutra[lik])
    {
        SetTimerEx("CivilUstani", CIVIL_USTANI_MS, false, "i", lik);
        return;
    }
    new Float:x, Float:y, Float:z;
    Lik_Pozicija(lik, x, y, z);
    CivilMakniRanjenog(lik);
    for (new i = 0; i < CIVIL_RANJENIH; i++)
    {
        if (gCivilRanjen[i] != INVALID_LIK) continue;
        gCivilRanjen[i] = lik;
        break;
    }
    HitnaZaNpc(LIK_PACIJENT + lik, x, y, z);
}

forward CivilUstani(lik);
public CivilUstani(lik)
{
    if (Lik_Ranjen(lik) && !Lik_Mrtav(lik)) Lik_Pridigni(lik);
}

CivilUmro(lik, ubojica)
{
    if (lik < 0 || lik >= LIK_PACIJENT) return;
    if (ubojica != INVALID_PLAYER_ID && IsPlayerConnected(ubojica) && !IsPlayerNPC(ubojica))
    {
        DodajWanted(ubojica);
        DodajWanted(ubojica);
    }
    CivilNovac(lik);
    gCivilNapadac[lik] = 0;
    CivilMakniRanjenog(lik);
    HitnaNpcUmro(LIK_PACIJENT + lik);
}

static bool:CivilJeRanjen(pacijent)
{
    if (pacijent < LIK_PACIJENT) return false;
    new lik = pacijent - LIK_PACIJENT;
    for (new i = 0; i < CIVIL_RANJENIH; i++) if (gCivilRanjen[i] == lik) return Lik_Ranjen(lik) && !Lik_Mrtav(lik);
    return false;
}

forward CivilUmri(lik);
public CivilUmri(lik)
{
    if (!Lik_Mrtav(lik)) Lik_Zdravlje(lik, 0.0);
}

NpcJeRanjen(pacijent)
{
    return KupacJeRanjen(pacijent) || CivilJeRanjen(pacijent) ? 1 : 0;
}

NpcOzivi(pacijent, bool:spasen)
{
    if (KupacJeRanjen(pacijent))
    {
        KupacOzivi(pacijent, spasen);
        return;
    }
    if (!CivilJeRanjen(pacijent)) return;
    new lik = pacijent - LIK_PACIJENT;
    CivilMakniRanjenog(lik);
    if (spasen) Lik_Pridigni(lik);
    else SetTimerEx("CivilUmri", 100, false, "i", lik);
}

NpcNajbliziRanjen(playerid, Float:domet)
{
    new best = KupacNajbliziRanjen(playerid, domet), Float:bd = domet, Float:x, Float:y, Float:z;
    if (best != INVALID_PLAYER_ID && Lik_Pozicija(best - LIK_PACIJENT, x, y, z)) bd = GetPlayerDistanceFromPoint(playerid, x, y, z);
    for (new i = 0; i < CIVIL_RANJENIH; i++)
    {
        new lik = gCivilRanjen[i];
        if (lik == INVALID_LIK || !CivilJeRanjen(LIK_PACIJENT + lik) || !Lik_Pozicija(lik, x, y, z)) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, x, y, z);
        if (d < bd)
        {
            bd = d;
            best = LIK_PACIJENT + lik;
        }
    }
    return best;
}
