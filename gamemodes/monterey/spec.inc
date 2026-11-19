static gSpecMeta[MAX_PLAYERS] = { INVALID_PLAYER_ID, ... }, gSpecVozilo[MAX_PLAYERS], bool:gSpecVraca[MAX_PLAYERS], gSpecOrg[MAX_PLAYERS];
static Float:gSpecPoz[MAX_PLAYERS][4], gSpecInt[MAX_PLAYERS], gSpecSvijet[MAX_PLAYERS], Float:gSpecZivot[MAX_PLAYERS][2];
static gSpecOrgIme[MAX_PLAYERS][MAX_ORG_NAME], gSpecAc[MAX_PLAYERS][96], gSpecModovi[MAX_PLAYERS][96], gSpecZadnjiUpdate[MAX_PLAYERS];

SpecUpdate(playerid)
{
    gSpecZadnjiUpdate[playerid] = GetTickCount();
}

static SpecOsvjeziAc(admin)
{
    new t = gSpecMeta[admin], sazetak[80];
    AcSazetak(admin, t, sazetak, sizeof sazetak);
    format(gSpecAc[admin], sizeof gSpecAc[], T(admin, "~w~AC: %s"), sazetak);
}

static SpecPrati(admin)
{
    new t = gSpecMeta[admin], v = GetPlayerVehicleID(t), interior = GetPlayerInterior(t), svijet = GetPlayerVirtualWorld(t);
    if (GetPlayerInterior(admin) != interior || GetPlayerVirtualWorld(admin) != svijet)
    {
        SetPlayerInterior(admin, interior);
        SetPlayerVirtualWorld(admin, svijet);
        gSpecVozilo[admin] = -1;
    }
    if (v == gSpecVozilo[admin] && (v || GetPlayerSpectateID(admin) == t)) return;
    gSpecVozilo[admin] = v;
    if (v) PlayerSpectateVehicle(admin, v);
    else PlayerSpectatePlayer(admin, t);
}

static SpecPanel(admin)
{
    new t = gSpecMeta[admin], tekst[640], red[96], naslov[48], Float:hp, Float:oklop, ime[32], WEAPON:w = GetPlayerWeapon(t), v = GetPlayerVehicleID(t);
    GetPlayerHealth(t, hp);
    GetPlayerArmour(t, oklop);
    format(tekst, sizeof tekst, T(admin, "~w~HP ~g~%.0f ~w~Oklop ~b~%.0f ~w~Ping %d (%.1f%%)"), hp, oklop, GetPlayerPing(t), NetStats_PacketLossPercent(t));
    new afk = (GetTickCount() - gSpecZadnjiUpdate[t]) / 1000;
    if (afk >= 3)
    {
        format(red, sizeof red, T(admin, "~r~ AFK %d s"), afk);
        strcat(tekst, red);
    }
    if (w == WEAPON_FIST || w == WEAPON:-1) format(red, sizeof red, "~n~%s", T(admin, "~w~Oruzje: nema"));
    else
    {
        GetWeaponName(w, ime, sizeof ime);
        new server = OruzjeServerMunicija(t, w);
        if (!OruzjeLegit(t, w)) format(red, sizeof red, T(admin, "~r~Oruzje: %s NELEGALNO"), ime);
        else if (server >= 0) format(red, sizeof red, T(admin, "~w~Oruzje: %s %d/%d"), ime, GetPlayerAmmo(t), server);
        else format(red, sizeof red, T(admin, "~w~Oruzje: %s"), ime);
        strins(red, "~n~", 0);
    }
    strcat(tekst, red);
    if (v)
    {
        new Float:vx, Float:vy, Float:vz, Float:vhp, model = GetVehicleModel(v);
        GetVehicleVelocity(v, vx, vy, vz);
        GetVehicleHealth(v, vhp);
        format(red, sizeof red, T(admin, "~w~%s %d/%d km/h HP %.0f"), gImenaVozila[model - 400], floatround(VectorSize(vx, vy, vz) * 180.0), VoziloNajvecaBrzina(v), vhp);
    }
    else format(red, sizeof red, "%s", T(admin, "~w~Pjeske"));
    strins(red, "~n~", 0);
    strcat(tekst, red);
    new Float:x, Float:y, Float:z;
    GetPlayerPos(t, x, y, z);
    format(red, sizeof red, T(admin, "~w~%s  Int %d VW %d"), CestaZona(x, y), GetPlayerInterior(t), GetPlayerVirtualWorld(t));
    strins(red, "~n~", 0);
    strcat(tekst, red);
    if (gSpecOrg[admin] != gOrg[t])
    {
        gSpecOrg[admin] = gOrg[t];
        gSpecOrgIme[admin][0] = EOS;
        if (!gOrg[t] || !GetOrgIme(gOrg[t], gSpecOrgIme[admin], sizeof gSpecOrgIme[])) strcat(gSpecOrgIme[admin], T(admin, "bez organizacije"), sizeof gSpecOrgIme[]);
    }
    format(red, sizeof red, T(admin, "~w~%s  $%d  Wanted %d"), gSpecOrgIme[admin], gNovac[t], GetWanted(t));
    strins(red, "~n~", 0);
    strcat(tekst, red);
    new verzija[24], serijski[48];
    GetPlayerVersion(t, verzija, sizeof verzija);
    gpci(t, serijski, sizeof serijski);
    format(red, sizeof red, T(admin, "~w~Klijent: %s%s"), verzija, !strcmp(serijski, AC_MOBITEL_GPCI) ? (" mobitel") : IsPlayerUsingOmp(t) ? (" open.mp") : (""));
    strins(red, "~n~", 0);
    strcat(tekst, red);
    format(red, sizeof red, "~n~%s~n~", gSpecAc[admin]);
    strcat(tekst, red);
    format(red, sizeof red, T(admin, "~w~Modovi: %s"), gSpecModovi[admin]);
    strcat(tekst, red);
    format(red, sizeof red, "~n~%s", T(admin, "~y~SPACE ~w~provjeri modove  ~y~/spec ~w~izlaz"));
    strcat(tekst, red);
    GetPlayerName(t, ime, sizeof ime);
    format(naslov, sizeof naslov, "SPEC: %s (%d)", ime, t);
    IgraObavijestPanel(admin, naslov, 0x2C3E50E6, tekst);
}

static SpecStop(admin)
{
    if (gSpecMeta[admin] == INVALID_PLAYER_ID) return;
    gSpecMeta[admin] = INVALID_PLAYER_ID;
    IgraObavijestUkloni(admin);
    gSpecVraca[admin] = true;
    TogglePlayerSpectating(admin, false);
}

SpecSpawn(playerid)
{
    if (!gSpecVraca[playerid]) return;
    gSpecVraca[playerid] = false;
    PrebaciIgraca(playerid, gSpecInt[playerid], gSpecSvijet[playerid], gSpecPoz[playerid][0], gSpecPoz[playerid][1], gSpecPoz[playerid][2], gSpecPoz[playerid][3]);
    SetPlayerHealth(playerid, gSpecZivot[playerid][0]);
    SetPlayerArmour(playerid, gSpecZivot[playerid][1]);
}

SpecDisconnect(playerid)
{
    gSpecMeta[playerid] = INVALID_PLAYER_ID;
    gSpecVraca[playerid] = false;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gSpecMeta[i] != playerid) continue;
        SpecStop(i);
        SendClientMessage(i, COLOR_ADMIN, T(i, "Igrac kojeg ste gledali je izasao, gledanje je zavrseno."));
    }
}

SpecTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gSpecMeta[i] == INVALID_PLAYER_ID) continue;
        SpecPrati(i);
        SpecPanel(i);
    }
}

SpecModoviRezultat(admin, playerid, const rezultat[])
{
    if (admin < 0 || admin >= MAX_PLAYERS || gSpecMeta[admin] != playerid) return;
    strcopy(gSpecModovi[admin], rezultat, sizeof gSpecModovi[]);
    SpecOsvjeziAc(admin);
    SpecPanel(admin);
}

SpecProvjera(playerid)
{
    if (gSpecMeta[playerid] == INVALID_PLAYER_ID) return 0;
    strcopy(gSpecModovi[playerid], T(playerid, "~y~provjera u tijeku..."), sizeof gSpecModovi[]);
    AntiCheatProvjeriZa(gSpecMeta[playerid], playerid);
    SpecPanel(playerid);
    return 1;
}

CMD:spec(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    new idx = 0, p1[32];
    if (!SljedeciParam(params, idx, p1))
    {
        if (gSpecMeta[playerid] == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /spec [id/ime] (bez parametra za izlaz)"));
        SpecStop(playerid);
        return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Gledanje je zavrseno."));
    }
    new t = FindPlayer(p1);
    if (t == INVALID_PLAYER_ID || !gUlogiran[t]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (t == playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete gledati sami sebe."));
    if (gSpecMeta[playerid] == INVALID_PLAYER_ID)
    {
        if (GetPlayerState(playerid) == PLAYER_STATE_SPECTATING || GetPlayerState(playerid) == PLAYER_STATE_WASTED || IsRanjen(playerid))
            return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trenutno ne mozete gledati igrace."));
        GetPlayerPos(playerid, gSpecPoz[playerid][0], gSpecPoz[playerid][1], gSpecPoz[playerid][2]);
        GetPlayerFacingAngle(playerid, gSpecPoz[playerid][3]);
        gSpecInt[playerid] = GetPlayerInterior(playerid);
        gSpecSvijet[playerid] = GetPlayerVirtualWorld(playerid);
        GetPlayerHealth(playerid, gSpecZivot[playerid][0]);
        GetPlayerArmour(playerid, gSpecZivot[playerid][1]);
        SpremiSvoOruzje(playerid);
        TogglePlayerSpectating(playerid, true);
    }
    gSpecMeta[playerid] = t;
    gSpecVozilo[playerid] = -1;
    gSpecOrg[playerid] = -1;
    strcopy(gSpecModovi[playerid], T(playerid, "~y~nije provjereno"), sizeof gSpecModovi[]);
    SpecOsvjeziAc(playerid);
    SpecPrati(playerid);
    SpecPanel(playerid);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Gledate igraca %s. SPACE - provjera modova, /spec - izlaz."), GetRPIme(t));
}
