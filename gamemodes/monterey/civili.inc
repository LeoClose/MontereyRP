#define CIVIL_RANJEN_HP     30.0
#define CIVIL_NOVAC_SANSA   45
#define CIVIL_NOVAC_MIN     5
#define CIVIL_NOVAC_MAX     60
#define CIVIL_RANJENIH      48
#define CIVIL_USTANI_MS     45000
#define CIVIL_VOZILA        64
#define CIVIL_VOZILO_S      600
#define CIVIL_VOZILO_DOMET  150.0

static gCivilVozilo[CIVIL_VOZILA] = { INVALID_VEHICLE_ID, ... }, gCivilVoziloOd[CIVIL_VOZILA], gCivilNapadac[LIK_PACIJENT], bool:gCivilUnutra[LIK_PACIJENT], gCivilRanjen[CIVIL_RANJENIH] = { INVALID_LIK, ... };

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

public OnPrometVozilo(playerid, model, Float:x, Float:y, Float:z, Float:a, c1, c2, Float:zdravlje, paneli, vrata, svjetla, gume)
{
    new k = 0;
    while (k < CIVIL_VOZILA && gCivilVozilo[k] != INVALID_VEHICLE_ID) k++;
    if (k == CIVIL_VOZILA) return INVALID_VEHICLE_ID;
    new v = CreateVozilo(model, x, y, z, a, c1, c2, -1);
    if (v == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
    gCivilVozilo[k] = v;
    gCivilVoziloOd[k] = gettime();
    SetVehicleHealth(v, zdravlje);
    UpdateVehicleDamageStatus(v, VEHICLE_PANEL_STATUS:paneli, VEHICLE_DOOR_STATUS:vrata, VEHICLE_LIGHT_STATUS:svjetla, VEHICLE_TIRE_STATUS:gume);
    VoziloSet(v, VP_MOTOR, playerid == INVALID_PLAYER_ID ? 0 : 1);
    if (playerid != INVALID_PLAYER_ID && IsPlayerConnected(playerid))
    {
        PutPlayerInVehicle(playerid, v, 0);
        DodajWanted(playerid);
    }
    return v;
}

static bool:CivilVoziloZauzeto(v)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerInVehicle(i, v)) return true;
    return false;
}

CivilVozilaTick()
{
    new sad = gettime(), Float:x, Float:y, Float:z;
    for (new k = 0; k < CIVIL_VOZILA; k++)
    {
        new v = gCivilVozilo[k];
        if (v == INVALID_VEHICLE_ID) continue;
        if (!GetVehicleModel(v))
        {
            gCivilVozilo[k] = INVALID_VEHICLE_ID;
            continue;
        }
        if (sad - gCivilVoziloOd[k] < CIVIL_VOZILO_S || CivilVoziloZauzeto(v)) continue;
        GetVehiclePos(v, x, y, z);
        if (!Lik_Skriveno(x, y, z, CIVIL_VOZILO_DOMET, CIVIL_VOZILO_DOMET)) continue;
        DestroyVozilo(v);
        gCivilVozilo[k] = INVALID_VEHICLE_ID;
    }
}
