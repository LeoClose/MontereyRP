#define LOGIN_TD          8
#define LOGIN_SIRINA      150.0
#define LOGIN_X           225.0
#define LOGIN_Y           110.0
#define LOGIN_KUTIJA_H    100.0

enum { LTD_NASLOV, LTD_STATISTIKA }

static PlayerText:gLoginTd[MAX_PLAYERS][LOGIN_TD], gLoginBroj[MAX_PLAYERS], PlayerText:gLogin[MAX_PLAYERS][2], bool:gLoginIma[MAX_PLAYERS];

static PlayerText:LoginKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gLoginTd[playerid][gLoginBroj[playerid]++] = td;
    return td;
}

static PlayerText:LoginTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, font);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
    gLoginTd[playerid][gLoginBroj[playerid]++] = td;
    return td;
}

LoginTd(playerid)
{
    LoginEkran(playerid, "LOGIN");
}

LoginEkran(playerid, const naslov[])
{
    new Float:sredina = LOGIN_X + LOGIN_SIRINA / 2.0, ime[MAX_PLAYER_NAME + 1], statistika[160];
    if (!gLoginIma[playerid])
    {
        gLoginIma[playerid] = true;
        gLoginBroj[playerid] = 0;
        GetPlayerName(playerid, ime, sizeof ime);
        LoginKutija(playerid, LOGIN_X, LOGIN_Y, LOGIN_SIRINA, 40.0, 0x1E5AA8F0);
        gLogin[playerid][LTD_NASLOV] = LoginTekst(playerid, sredina, LOGIN_Y + 6.0, "_", 0xFFFFFFFF, 0.7, 2.8, TEXT_DRAW_FONT_3);
        LoginKutija(playerid, LOGIN_X, LOGIN_Y + 40.0, LOGIN_SIRINA, 14.0, 0x164078);
        LoginTekst(playerid, sredina, LOGIN_Y + 42.0, ime, 0xFFFFFFFF, 0.2, 1.0, TEXT_DRAW_FONT_2);
        LoginKutija(playerid, LOGIN_X, LOGIN_Y + 54.0, LOGIN_SIRINA, LOGIN_KUTIJA_H, 0x000000B4);
        LoginKutija(playerid, LOGIN_X, LOGIN_Y + 54.0 + LOGIN_KUTIJA_H, LOGIN_SIRINA, 34.0, 0x164078);
        gLogin[playerid][LTD_STATISTIKA] = LoginTekst(playerid, sredina, LOGIN_Y + 58.0 + LOGIN_KUTIJA_H, "_", 0xFFFFFFFF, 0.18, 0.9, TEXT_DRAW_FONT_1);
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
    format(statistika, sizeof statistika, T(playerid, "Registriranih igraca: ~w~~h~%d~n~~w~Admina online: ~w~~h~%d~n~~w~Igraca online: ~w~~h~%d"), registrirano, admina, igraca);
    PlayerTextDrawSetString(playerid, gLogin[playerid][LTD_NASLOV], naslov);
    PlayerTextDrawSetString(playerid, gLogin[playerid][LTD_STATISTIKA], statistika);
    for (new i = 0; i < gLoginBroj[playerid]; i++) PlayerTextDrawShow(playerid, gLoginTd[playerid][i]);
}

LoginTdSakrij(playerid)
{
    if (!gLoginIma[playerid]) return;
    gLoginIma[playerid] = false;
    for (new i = 0; i < gLoginBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gLoginTd[playerid][i]);
    gLoginBroj[playerid] = 0;
}

LoginDisconnect(playerid)
{
    gLoginIma[playerid] = false;
    gLoginBroj[playerid] = 0;
}
