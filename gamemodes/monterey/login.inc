#define LOGIN_TD          12
#define LOGIN_PANEL       180.0
#define LOGIN_X           442.0
#define LOGIN_Y           316.0
#define LOGIN_VISINA      106.0
#define LOGIN_SREDINA     532.0
#define LOGIN_KADAR_MS    14000
#define LOGIN_KADROVA     4

enum { LTD_NASLOV, LTD_PODNASLOV, LTD_STATISTIKA }

static const Float:gLoginKadar[LOGIN_KADROVA][9] = {
    { 1340.0, -960.0, 115.0, 1490.0, -960.0, 115.0, 1415.0, -805.0, 90.0 },
    { 1250.0, -1150.0, 190.0, 1650.0, -1100.0, 190.0, 1530.0, -1350.0, 90.0 },
    { 2440.0, -1610.0, 40.0, 2540.0, -1610.0, 40.0, 2495.0, -1680.0, 15.0 },
    { 280.0, -1880.0, 45.0, 440.0, -1930.0, 45.0, 370.0, -2050.0, 10.0 }
};

static const gLoginMuzika[] = "";

static bool:gLoginFade[MAX_PLAYERS], PlayerText:gLoginTd[MAX_PLAYERS][LOGIN_TD], gLoginBroj[MAX_PLAYERS], PlayerText:gLogin[MAX_PLAYERS][3], bool:gLoginIma[MAX_PLAYERS], gLoginToken[MAX_PLAYERS], gLoginKadarBr[MAX_PLAYERS];

static PlayerText:LoginKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gLoginTd[playerid][gLoginBroj[playerid]++] = td;
    return td;
}

static PlayerText:LoginTekst(playerid, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, LOGIN_SREDINA, y, tekst);
    PlayerTextDrawFont(playerid, td, font);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawSetProportional(playerid, td, true);
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
    gLoginTd[playerid][gLoginBroj[playerid]++] = td;
    return td;
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
    for (new i = 0; i < MAX_PLAYERS; i++)
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
    Streamer_UpdateEx(playerid, gLoginKadar[k][6], gLoginKadar[k][7], gLoginKadar[k][8], 0, 0, STREAMER_TYPE_OBJECT, 1000);
    InterpolateCameraPos(playerid, gLoginKadar[k][0], gLoginKadar[k][1], gLoginKadar[k][2], gLoginKadar[k][3], gLoginKadar[k][4], gLoginKadar[k][5], LOGIN_KADAR_MS, CAMERA_CUT);
    InterpolateCameraLookAt(playerid, gLoginKadar[k][6], gLoginKadar[k][7], gLoginKadar[k][8], gLoginKadar[k][6], gLoginKadar[k][7], gLoginKadar[k][8], LOGIN_KADAR_MS, CAMERA_CUT);
    SetTimerEx("LoginKamera", LOGIN_KADAR_MS, false, "ii", playerid, token);
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
