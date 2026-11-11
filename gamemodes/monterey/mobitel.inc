#define MOBITEL_TD          30
#define MOBITEL_X           490.0
#define MOBITEL_Y           210.0

static Text:gMobitelTd[MOBITEL_TD], Text:gMobitelSat, gMobitelBroj, bool:gMobitelGotov, bool:gMobitelOtvoren[MAX_PLAYERS];
static const gMobitelTipke[][] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#" };

static Text:MobitelKutija(Float:x, Float:y, Float:w, Float:h, boja)
{
    new Text:td = TextDrawCreate(MOBITEL_X + x, MOBITEL_Y + y, "LD_SPAC:white");
    TextDrawFont(td, TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawTextSize(td, w, h);
    TextDrawColour(td, boja);
    gMobitelTd[gMobitelBroj++] = td;
    return td;
}

static Text:MobitelTekst(Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new Text:td = TextDrawCreate(MOBITEL_X + x, MOBITEL_Y + y, tekst);
    TextDrawFont(td, TEXT_DRAW_FONT_2);
    TextDrawLetterSize(td, sirina, visina);
    TextDrawColour(td, boja);
    TextDrawSetShadow(td, 0);
    TextDrawAlignment(td, poravnanje);
    gMobitelTd[gMobitelBroj++] = td;
    return td;
}

static MobitelNapravi()
{
    if (gMobitelGotov) return;
    gMobitelGotov = true;
    MobitelKutija(0.0, 0.0, 120.0, 232.0, 0x101010FF);
    MobitelKutija(3.0, 3.0, 114.0, 226.0, 0x2A2A2AFF);
    MobitelKutija(45.0, 9.0, 30.0, 3.0, 0x050505FF);
    MobitelKutija(9.0, 17.0, 102.0, 106.0, 0x000000FF);
    MobitelKutija(12.0, 20.0, 96.0, 100.0, 0xDCE4ECFF);
    MobitelKutija(12.0, 20.0, 96.0, 11.0, 0x2E4A68FF);
    gMobitelSat = MobitelTekst(105.0, 21.0, "_", 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_ALIGN_RIGHT);
    MobitelTekst(15.0, 21.0, "MONTEREY", 0xFFFFFFFF, 0.18, 0.85);
    MobitelTekst(60.0, 34.0, "IZBORNIK", 0x2E4A68FF, 0.24, 1.1, TEXT_DRAW_ALIGN_CENTER);
    MobitelKutija(15.0, 48.0, 90.0, 10.0, 0x5B8DBFFF);
    MobitelTekst(19.0, 48.0, "Imenik~n~Poruke~n~Kamera~n~Organizator~n~Internet~n~Postavke", 0x1A1A1AFF, 0.2, 0.95);
    MobitelTekst(16.0, 108.0, "Odaberi", 0x2E4A68FF, 0.17, 0.8);
    MobitelTekst(104.0, 108.0, "Natrag", 0x2E4A68FF, 0.17, 0.8, TEXT_DRAW_ALIGN_RIGHT);
    MobitelKutija(12.0, 128.0, 28.0, 8.0, 0x444444FF);
    MobitelKutija(80.0, 128.0, 28.0, 8.0, 0x444444FF);
    MobitelKutija(47.0, 126.0, 26.0, 14.0, 0x3C3C3CFF);
    MobitelKutija(55.0, 130.0, 10.0, 6.0, 0x5A5A5AFF);
    for (new i = 0; i < sizeof gMobitelTipke; i++)
    {
        new Text:td = MobitelTekst(26.0 + float(i % 3) * 34.0, 146.0 + float(i / 3) * 20.0, gMobitelTipke[i], 0xE0E0E0FF, 0.28, 1.3, TEXT_DRAW_ALIGN_CENTER);
        TextDrawUseBox(td, true);
        TextDrawBoxColour(td, 0x3A3A3AFF);
        TextDrawTextSize(td, 10.0, 28.0);
    }
    assert(gMobitelBroj <= MOBITEL_TD);
}

MobitelDisconnect(playerid)
{
    gMobitelOtvoren[playerid] = false;
}

CMD:mobitel(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    MobitelNapravi();
    gMobitelOtvoren[playerid] = !gMobitelOtvoren[playerid];
    if (gMobitelOtvoren[playerid])
    {
        new sat, minuta, vrijeme[8];
        gettime(sat, minuta);
        format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
        TextDrawSetString(gMobitelSat, vrijeme);
        for (new i = 0; i < gMobitelBroj; i++) TextDrawShowForPlayer(playerid, gMobitelTd[i]);
        if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
    }
    else
    {
        for (new i = 0; i < gMobitelBroj; i++) TextDrawHideForPlayer(playerid, gMobitelTd[i]);
        if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USECELLPHONE) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
    }
    return 1;
}

CMD:phone(playerid, params[]) return cmd_mobitel(playerid, params);
CMD:telefon(playerid, params[]) return cmd_mobitel(playerid, params);
