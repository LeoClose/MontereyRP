#define LOGIN_TD          12
#define LOGIN_PANEL       170.0
#define LOGIN_SREDINA     85.0
#define LOGIN_KADAR_MS    14000
#define LOGIN_KADROVA     4

enum { LTD_NASLOV, LTD_PODNASLOV, LTD_STATISTIKA }

static const Float:gLoginKadar[LOGIN_KADROVA][9] = {
    { 1340.0, -960.0, 115.0, 1490.0, -960.0, 115.0, 1415.0, -805.0, 90.0 },
    { 1250.0, -1150.0, 190.0, 1650.0, -1100.0, 190.0, 1530.0, -1350.0, 90.0 },
    { 2440.0, -1610.0, 40.0, 2540.0, -1610.0, 40.0, 2495.0, -1680.0, 15.0 },
    { 280.0, -1880.0, 45.0, 440.0, -1930.0, 45.0, 370.0, -2050.0, 10.0 }
};

static PlayerText:gLoginTd[MAX_PLAYERS][LOGIN_TD], gLoginBroj[MAX_PLAYERS], PlayerText:gLogin[MAX_PLAYERS][3], bool:gLoginIma[MAX_PLAYERS], gLoginToken[MAX_PLAYERS], gLoginKadarBr[MAX_PLAYERS];

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
        LoginKutija(playerid, 0.0, 0.0, LOGIN_PANEL, 448.0, 0x000000C8);
        LoginKutija(playerid, LOGIN_PANEL, 0.0, 2.0, 448.0, 0x1E5AA8FF);
        LoginTekst(playerid, 70.0, "MONTEREY", 0xFFFFFFFF, 0.55, 2.4, TEXT_DRAW_FONT_3);
        LoginTekst(playerid, 96.0, "ROLEPLAY", 0x1E5AA8FF, 0.3, 1.3, TEXT_DRAW_FONT_2);
        LoginKutija(playerid, 45.0, 114.0, 80.0, 1.5, 0x1E5AA8FF);
        gLogin[playerid][LTD_NASLOV] = LoginTekst(playerid, 160.0, "_", 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2);
        LoginTekst(playerid, 176.0, ime, 0x5B9BE6FF, 0.3, 1.4, TEXT_DRAW_FONT_1);
        gLogin[playerid][LTD_PODNASLOV] = LoginTekst(playerid, 200.0, "_", 0xBBBBBBFF, 0.18, 0.85, TEXT_DRAW_FONT_1);
        LoginKutija(playerid, 30.0, 372.0, 110.0, 1.0, 0xFFFFFF40);
        gLogin[playerid][LTD_STATISTIKA] = LoginTekst(playerid, 380.0, "_", 0xCCCCCCFF, 0.18, 0.9, TEXT_DRAW_FONT_1);
        gLoginKadarBr[playerid] = random(LOGIN_KADROVA);
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
    format(statistika, sizeof statistika, T(playerid, "Registriranih igraca: %d~n~Admina online: %d~n~Igraca online: %d"), registrirano, admina, igraca);
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
    gLoginIma[playerid] = false;
    gLoginToken[playerid]++;
    gLoginBroj[playerid] = 0;
}
