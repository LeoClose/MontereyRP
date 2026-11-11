// Granica mape - ne dopusta igracima da se nalaze izvan granica LS-a i okolnih sela (SF, LV i sve ostalo tamo gore je zabranjeno)

#define GRANICA_SF_X        -1213.9
#define GRANICA_SF_Y        -1115.5
#define GRANICA_SJEVER_Y    596.3

static bool:gGranicaIma[MAX_PLAYERS], bool:gGranicaVraca[MAX_PLAYERS], Float:gGranicaZadnja[MAX_PLAYERS][3];

InitGranica()
{
    assert(GranicaZabranjeno(-2000.0, 500.0) && GranicaZabranjeno(2000.0, 1500.0) && GranicaZabranjeno(0.0, 1000.0));
    assert(!GranicaZabranjeno(1500.0, -1500.0) && !GranicaZabranjeno(-2300.0, -1600.0) && !GranicaZabranjeno(1300.0, 300.0) && !GranicaZabranjeno(-500.0, -2000.0));
}

GranicaZabranjeno(Float:x, Float:y)
{
    return y > GRANICA_SJEVER_Y || (x < GRANICA_SF_X && y > GRANICA_SF_Y);
}

GranicaConnect(playerid)
{
    gGranicaIma[playerid] = false;
    gGranicaVraca[playerid] = false;
}

static GranicaPutnici(v, korak)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && IsPlayerInVehicle(i, v)) Fade(i, korak);
}

static GranicaVrati(playerid)
{
    new v = GetPlayerVehicleID(playerid);
    if (!v)
    {
        GranicaPostavi(playerid, 0);
        return;
    }
    gGranicaVraca[playerid] = true;
    GranicaPutnici(v, 0);
    SetTimerEx("GranicaVozilo", FADE_SLOJEVA * FADE_KORAK_MS, false, "iii", playerid, gAccountId[playerid], v);
}

forward GranicaVozilo(playerid, racun, v);
public GranicaVozilo(playerid, racun, v)
{
    if (gAccountId[playerid] == racun) gGranicaVraca[playerid] = false;
    if (!GetVehicleModel(v)) return;
    if (gAccountId[playerid] == racun && GetPlayerVehicleID(playerid) == v) GranicaPostavi(playerid, v);
    GranicaPutnici(v, FADE_SLOJEVA + 1);
}

static GranicaPostavi(playerid, v)
{
    new Float:x, Float:y, Float:z;
    if (gGranicaIma[playerid])
    {
        x = gGranicaZadnja[playerid][0];
        y = gGranicaZadnja[playerid][1];
        z = gGranicaZadnja[playerid][2];
    }
    else
    {
        GetPlayerPos(playerid, x, y, z);
        new Float:d, c = CestaNajbliza(x, y, z, d, true);
        x = float(gCestaTocka[c][0]) / 10.0;
        y = float(gCestaTocka[c][1]) / 10.0;
        z = float(gCestaTocka[c][2]) / 10.0 + 1.0;
    }
    if (v)
    {
        new Float:a, Float:vx, Float:vy, Float:vz;
        GetVehicleZAngle(v, a);
        GetVehicleVelocity(v, vx, vy, vz);
        SetVehiclePos(v, x, y, z);
        SetVehicleZAngle(v, a + 180.0);
        SetVehicleVelocity(v, -vx * 0.5, -vy * 0.5, vz);
    }
    else
    {
        new Float:a;
        GetPlayerFacingAngle(playerid, a);
        SetPlayerPos(playerid, x, y, z);
        SetPlayerFacingAngle(playerid, a + 180.0);
        SetCameraBehindPlayer(playerid);
    }
}

GranicaTick()
{
    new Float:x, Float:y, Float:z;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i] || gGranicaVraca[i] || IsRanjen(i) || GetPlayerInterior(i) || GetPlayerVirtualWorld(i)) continue;
        new PLAYER_STATE:st = GetPlayerState(i);
        if (st != PLAYER_STATE_ONFOOT && st != PLAYER_STATE_DRIVER) continue;
        GetPlayerPos(i, x, y, z);
        if (GranicaZabranjeno(x, y)) GranicaVrati(i);
        else
        {
            gGranicaIma[i] = true;
            gGranicaZadnja[i][0] = x;
            gGranicaZadnja[i][1] = y;
            gGranicaZadnja[i][2] = z;
        }
    }
}
