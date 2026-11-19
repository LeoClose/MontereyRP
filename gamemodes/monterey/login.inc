#define LOGIN_TD          12
#define LOGIN_PANEL       180.0
#define LOGIN_X           442.0
#define LOGIN_Y           316.0
#define LOGIN_VISINA      106.0
#define LOGIN_SREDINA     532.0
#define LOGIN_KADAR_MS    14000
#define LOGIN_KADROVA     4

enum { LTD_NASLOV, LTD_PODNASLOV, LTD_STATISTIKA }

#define LOGIN_ORBITA_KORAKA 12
#define LOGIN_ORBITA_MS     (LOGIN_KADAR_MS / LOGIN_ORBITA_KORAKA)
static const Float:gLoginOrbita[][6] = {
    { 484.0, -1499.5, 22.0, 45.0, 45.0, 0.0 },
    { 1193.4, -2054.9, 110.0, 260.0, 170.0, 0.0 }
};
static const Float:gLoginKadar[LOGIN_KADROVA][9] = {
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
    { 2440.0, -1610.0, 40.0, 2540.0, -1610.0, 40.0, 2495.0, -1680.0, 15.0 },
    { 280.0, -1880.0, 45.0, 440.0, -1930.0, 45.0, 370.0, -2050.0, 10.0 }
};

static const gLoginMuzika[] = "";

static bool:gLoginFade[MAX_PLAYERS], PlayerText:gLoginTd[MAX_PLAYERS][LOGIN_TD], gLoginBroj[MAX_PLAYERS], PlayerText:gLogin[MAX_PLAYERS][3], bool:gLoginIma[MAX_PLAYERS], gLoginToken[MAX_PLAYERS], gLoginKadarBr[MAX_PLAYERS];

static PlayerText:LoginKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    return gLoginTd[playerid][gLoginBroj[playerid]++] = TdSlika(playerid, x, y, w, h, boja);
}

static PlayerText:LoginTekst(playerid, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font)
{
    new PlayerText:td = TdTekst(playerid, LOGIN_SREDINA, y, tekst, boja, sirina, visina, font, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetProportional(playerid, td, true);
    return gLoginTd[playerid][gLoginBroj[playerid]++] = td;
}

LoginTd(playerid)
{
    LoginEkran(playerid, T(playerid, "Dobrodosli nazad"), T(playerid, "Unesite svoju lozinku za nastavak."));
}

LoginEkran(playerid, const naslov[], const podnaslov[])
{
    new statistika[160];
    if (!gLoginIma[playerid])
    {
        new ime[MAX_PLAYER_NAME + 1];
        GetPlayerName(playerid, ime, sizeof ime);
        for (new i = 0; ime[i]; i++) if (ime[i] == '_') ime[i] = ' ';
        gLoginIma[playerid] = true;
        gLoginBroj[playerid] = 0;
        LoginKutija(playerid, LOGIN_X, LOGIN_Y, LOGIN_PANEL, LOGIN_VISINA, 0x0B0B12D8);
        LoginKutija(playerid, LOGIN_X, LOGIN_Y, 2.0, LOGIN_VISINA, 0x1E5AA8FF);
        new PlayerText:logo = LoginTekst(playerid, LOGIN_Y + 4.0, "Monterey", 0x1E5AA8FF, 0.5, 2.0, TEXT_DRAW_FONT_3);
        PlayerTextDrawSetOutline(playerid, logo, 1);
        PlayerTextDrawBackgroundColour(playerid, logo, 0x000000FF);
        logo = LoginTekst(playerid, LOGIN_Y + 22.0, "RolePlay", 0xFFFFFFFF, 0.34, 1.35, TEXT_DRAW_FONT_3);
        PlayerTextDrawSetOutline(playerid, logo, 1);
        PlayerTextDrawBackgroundColour(playerid, logo, 0x000000FF);
        PlayerTextDrawAlignment(playerid, logo, TEXT_DRAW_ALIGN_RIGHT);
        PlayerTextDrawSetPos(playerid, logo, LOGIN_SREDINA + 44.0, LOGIN_Y + 22.0);
        LoginKutija(playerid, LOGIN_X + 14.0, LOGIN_Y + 42.0, LOGIN_PANEL - 28.0, 1.0, 0xFFFFFF30);
        gLogin[playerid][LTD_NASLOV] = LoginTekst(playerid, LOGIN_Y + 47.0, "_", 0xFFFFFFFF, 0.19, 0.9, TEXT_DRAW_FONT_2);
        LoginTekst(playerid, LOGIN_Y + 57.0, ime, 0x5B9BE6FF, 0.28, 1.25, TEXT_DRAW_FONT_1);
        gLogin[playerid][LTD_PODNASLOV] = LoginTekst(playerid, LOGIN_Y + 74.0, "_", 0xBBBBBBFF, 0.17, 0.8, TEXT_DRAW_FONT_1);
        gLogin[playerid][LTD_STATISTIKA] = LoginTekst(playerid, LOGIN_Y + 89.0, "_", 0x8899AAFF, 0.16, 0.75, TEXT_DRAW_FONT_1);
        gLoginKadarBr[playerid] = random(LOGIN_KADROVA);
        if (gLoginMuzika[0]) PlayAudioStreamForPlayer(playerid, gLoginMuzika);
        SetTimerEx("LoginKamera", 300, false, "ii", playerid, ++gLoginToken[playerid]);
    }
    new registrirano, admina = 0, igraca = 0, DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM accounts");
    registrirano = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!IsPlayerConnected(i) || IsPlayerNPC(i)) continue;
        igraca++;
        if (gUlogiran[i] && gAdmin[i] > 0) admina++;
    }
    format(statistika, sizeof statistika, T(playerid, "Online: %d  |  Admina: %d  |  Racuna: %d"), igraca, admina, registrirano);
    PlayerTextDrawSetString(playerid, gLogin[playerid][LTD_NASLOV], naslov);
    PlayerTextDrawSetString(playerid, gLogin[playerid][LTD_PODNASLOV], podnaslov);
    PlayerTextDrawSetString(playerid, gLogin[playerid][LTD_STATISTIKA], statistika);
    for (new i = 0; i < gLoginBroj[playerid]; i++) PlayerTextDrawShow(playerid, gLoginTd[playerid][i]);
}

forward LoginKamera(playerid, token);
public LoginKamera(playerid, token)
{
    if (!IsPlayerConnected(playerid) || !gLoginIma[playerid] || gLoginToken[playerid] != token) return;
    new k = gLoginKadarBr[playerid];
    gLoginKadarBr[playerid] = (k + 1) % LOGIN_KADROVA;
    if (k < sizeof gLoginOrbita)
    {
        Streamer_UpdateEx(playerid, gLoginOrbita[k][0], gLoginOrbita[k][1], gLoginOrbita[k][2], 0, 0, STREAMER_TYPE_OBJECT, 1000);
        LoginOrbita(playerid, token, k, 0);
        return;
    }
    Streamer_UpdateEx(playerid, gLoginKadar[k][6], gLoginKadar[k][7], gLoginKadar[k][8], 0, 0, STREAMER_TYPE_OBJECT, 1000);
    InterpolateCameraPos(playerid, gLoginKadar[k][0], gLoginKadar[k][1], gLoginKadar[k][2], gLoginKadar[k][3], gLoginKadar[k][4], gLoginKadar[k][5], LOGIN_KADAR_MS, CAMERA_CUT);
    InterpolateCameraLookAt(playerid, gLoginKadar[k][6], gLoginKadar[k][7], gLoginKadar[k][8], gLoginKadar[k][6], gLoginKadar[k][7], gLoginKadar[k][8], LOGIN_KADAR_MS, CAMERA_CUT);
    SetTimerEx("LoginKamera", LOGIN_KADAR_MS, false, "ii", playerid, token);
}

forward LoginOrbita(playerid, token, o, korak);
public LoginOrbita(playerid, token, o, korak)
{
    if (!IsPlayerConnected(playerid) || !gLoginIma[playerid] || gLoginToken[playerid] != token) return;
    if (korak >= LOGIN_ORBITA_KORAKA)
    {
        LoginKamera(playerid, token);
        return;
    }
    new Float:a0 = float(korak) * 360.0 / float(LOGIN_ORBITA_KORAKA), Float:a1 = a0 + 360.0 / float(LOGIN_ORBITA_KORAKA), Float:r = gLoginOrbita[o][3];
    new Float:cx = gLoginOrbita[o][0], Float:cy = gLoginOrbita[o][1], Float:cz = gLoginOrbita[o][2], Float:z = gLoginOrbita[o][4];
    InterpolateCameraPos(playerid, cx + r * floatcos(a0, degrees), cy + r * floatsin(a0, degrees), z, cx + r * floatcos(a1, degrees), cy + r * floatsin(a1, degrees), z, LOGIN_ORBITA_MS, korak ? CAMERA_MOVE : CAMERA_CUT);
    InterpolateCameraLookAt(playerid, cx, cy, cz, cx, cy, cz, LOGIN_ORBITA_MS, korak ? CAMERA_MOVE : CAMERA_CUT);
    SetTimerEx("LoginOrbita", LOGIN_ORBITA_MS, false, "iiii", playerid, token, o, korak + 1);
}

LoginUlaz(playerid)
{
    LoginTdSakrij(playerid);
    if (gLoginMuzika[0]) StopAudioStreamForPlayer(playerid);
    gLoginFade[playerid] = true;
    Fade(playerid, 0);
    SetTimerEx("LoginUlazKraj", FADE_SLOJEVA * FADE_KORAK_MS + 300, false, "ii", playerid, gAccountId[playerid]);
}

forward LoginUlazKraj(playerid, racun);
public LoginUlazKraj(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    ZavrsiUlaz(playerid);
}

LoginSpawn(playerid)
{
    if (!gLoginFade[playerid]) return;
    gLoginFade[playerid] = false;
    Fade(playerid, FADE_SLOJEVA + 1);
}

LoginTdSakrij(playerid)
{
    if (!gLoginIma[playerid]) return;
    gLoginIma[playerid] = false;
    gLoginToken[playerid]++;
    for (new i = 0; i < gLoginBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gLoginTd[playerid][i]);
    gLoginBroj[playerid] = 0;
}

LoginDisconnect(playerid)
{
    gLoginFade[playerid] = false;
    gLoginIma[playerid] = false;
    gLoginToken[playerid]++;
    gLoginBroj[playerid] = 0;
}
