#define MOBITEL_TD          32
#define MOBITEL_X           490.0
#define MOBITEL_Y           276.0
#define MOBITEL_STAVKA_Y    46.0
#define MOBITEL_STAVKA_H    9.5

static const gMobitelIzbornik[][] = { "Imenik", "Poruke", "Kamera", "Organizator", "Internet", "Postavke" };
static const gMobitelKontakti[][] = { "Taksi" };
static const gMobitelTipke[][] = { "1", "2", "3", "4", "5", "6" };

static PlayerText:gMobitelTd[MAX_PLAYERS][MOBITEL_TD], gMobitelBroj[MAX_PLAYERS];
static PlayerText:gMobitelNaslov[MAX_PLAYERS], PlayerText:gMobitelOznaka[MAX_PLAYERS], PlayerText:gMobitelStavke[MAX_PLAYERS][sizeof gMobitelIzbornik];
static bool:gMobitelOtvoren[MAX_PLAYERS], gMobitelEkran[MAX_PLAYERS], gMobitelOdabir[MAX_PLAYERS];

static PlayerText:MobitelKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, MOBITEL_X + x, MOBITEL_Y + y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gMobitelTd[playerid][gMobitelBroj[playerid]++] = td;
    return td;
}

static PlayerText:MobitelTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, MOBITEL_X + x, MOBITEL_Y + y, tekst);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    gMobitelTd[playerid][gMobitelBroj[playerid]++] = td;
    return td;
}

static MobitelBrojStavki(playerid)
{
    if (gMobitelEkran[playerid] == -1) return sizeof gMobitelIzbornik;
    if (gMobitelEkran[playerid] == 0) return sizeof gMobitelKontakti;
    return 0;
}

static MobitelPrikazi(playerid)
{
    new ekran = gMobitelEkran[playerid], broj = MobitelBrojStavki(playerid);
    PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], ekran == -1 ? T(playerid, "IZBORNIK") : T(playerid, gMobitelIzbornik[ekran]));
    PlayerTextDrawSetPos(playerid, gMobitelOznaka[playerid], MOBITEL_X + 15.0, MOBITEL_Y + MOBITEL_STAVKA_Y + float(gMobitelOdabir[playerid]) * MOBITEL_STAVKA_H);
    if (broj) PlayerTextDrawShow(playerid, gMobitelOznaka[playerid]);
    else PlayerTextDrawHide(playerid, gMobitelOznaka[playerid]);
    for (new i = 0; i < sizeof gMobitelIzbornik; i++)
    {
        if (i >= broj)
        {
            PlayerTextDrawHide(playerid, gMobitelStavke[playerid][i]);
            continue;
        }
        if (ekran == -1) PlayerTextDrawSetString(playerid, gMobitelStavke[playerid][i], T(playerid, gMobitelIzbornik[i]));
        else PlayerTextDrawSetString(playerid, gMobitelStavke[playerid][i], T(playerid, gMobitelKontakti[i]));
        PlayerTextDrawColour(playerid, gMobitelStavke[playerid][i], i == gMobitelOdabir[playerid] ? 0xFFFFFFFF : 0x1A1A1AFF);
        PlayerTextDrawShow(playerid, gMobitelStavke[playerid][i]);
    }
}

static MobitelOtvori(playerid)
{
    new sat, minuta, vrijeme[8];
    gettime(sat, minuta);
    format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
    gMobitelOtvoren[playerid] = true;
    gMobitelEkran[playerid] = -1;
    gMobitelOdabir[playerid] = 0;
    gMobitelBroj[playerid] = 0;
    MobitelKutija(playerid, 0.0, 0.0, 120.0, 172.0, 0x101010FF);
    MobitelKutija(playerid, 3.0, 3.0, 114.0, 169.0, 0x2A2A2AFF);
    MobitelKutija(playerid, 45.0, 9.0, 30.0, 3.0, 0x050505FF);
    MobitelKutija(playerid, 9.0, 17.0, 102.0, 106.0, 0x000000FF);
    MobitelKutija(playerid, 12.0, 20.0, 96.0, 100.0, 0xDCE4ECFF);
    MobitelKutija(playerid, 12.0, 20.0, 96.0, 11.0, 0x2E4A68FF);
    MobitelTekst(playerid, 105.0, 21.0, vrijeme, 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_ALIGN_RIGHT);
    MobitelTekst(playerid, 15.0, 21.0, "LS", 0xFFFFFFFF, 0.18, 0.85);
    gMobitelNaslov[playerid] = MobitelTekst(playerid, 60.0, 34.0, "_", 0x2E4A68FF, 0.24, 1.1, TEXT_DRAW_ALIGN_CENTER);
    gMobitelOznaka[playerid] = MobitelKutija(playerid, 15.0, MOBITEL_STAVKA_Y, 90.0, MOBITEL_STAVKA_H, 0x5B8DBFFF);
    for (new i = 0; i < sizeof gMobitelIzbornik; i++) gMobitelStavke[playerid][i] = MobitelTekst(playerid, 19.0, MOBITEL_STAVKA_Y + float(i) * MOBITEL_STAVKA_H, T(playerid, gMobitelIzbornik[i]), 0x1A1A1AFF, 0.2, 0.9);
    MobitelTekst(playerid, 16.0, 108.0, T(playerid, "Odaberi"), 0x2E4A68FF, 0.17, 0.8);
    MobitelTekst(playerid, 104.0, 108.0, T(playerid, "Natrag"), 0x2E4A68FF, 0.17, 0.8, TEXT_DRAW_ALIGN_RIGHT);
    MobitelKutija(playerid, 12.0, 128.0, 28.0, 8.0, 0x444444FF);
    MobitelKutija(playerid, 80.0, 128.0, 28.0, 8.0, 0x444444FF);
    MobitelKutija(playerid, 47.0, 126.0, 26.0, 14.0, 0x3C3C3CFF);
    MobitelKutija(playerid, 55.0, 130.0, 10.0, 6.0, 0x5A5A5AFF);
    for (new i = 0; i < sizeof gMobitelTipke; i++)
    {
        new PlayerText:td = MobitelTekst(playerid, 26.0 + float(i % 3) * 34.0, 146.0 + float(i / 3) * 20.0, gMobitelTipke[i], 0xE0E0E0FF, 0.28, 1.3, TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawUseBox(playerid, td, true);
        PlayerTextDrawBoxColour(playerid, td, 0x3A3A3AFF);
        PlayerTextDrawTextSize(playerid, td, 10.0, 28.0);
    }
    assert(gMobitelBroj[playerid] <= MOBITEL_TD);
    for (new i = 0; i < gMobitelBroj[playerid]; i++) PlayerTextDrawShow(playerid, gMobitelTd[playerid][i]);
    MobitelPrikazi(playerid);
}

static MobitelZatvori(playerid)
{
    if (!gMobitelOtvoren[playerid]) return;
    gMobitelOtvoren[playerid] = false;
    for (new i = 0; i < gMobitelBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gMobitelTd[playerid][i]);
    gMobitelBroj[playerid] = 0;
}

MobitelKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gMobitelOtvoren[playerid]) return;
    new KEY:pritisnuto = newkeys & ~oldkeys, broj = MobitelBrojStavki(playerid);
    if (pritisnuto & KEY_ANALOG_RIGHT)
    {
        if (gMobitelEkran[playerid] == -1)
        {
            MobitelZatvori(playerid);
            return;
        }
        gMobitelOdabir[playerid] = gMobitelEkran[playerid];
        gMobitelEkran[playerid] = -1;
    }
    else if (pritisnuto & KEY_ANALOG_LEFT)
    {
        if (gMobitelEkran[playerid] == 0)
        {
            TaksiPozovi(playerid);
            return;
        }
        if (gMobitelEkran[playerid] != -1) return;
        gMobitelEkran[playerid] = gMobitelOdabir[playerid];
        gMobitelOdabir[playerid] = 0;
    }
    else if (!broj) return;
    else if (pritisnuto & KEY_ANALOG_UP) gMobitelOdabir[playerid] = (gMobitelOdabir[playerid] + broj - 1) % broj;
    else if (pritisnuto & KEY_ANALOG_DOWN) gMobitelOdabir[playerid] = (gMobitelOdabir[playerid] + 1) % broj;
    else return;
    MobitelPrikazi(playerid);
}

MobitelDisconnect(playerid)
{
    gMobitelOtvoren[playerid] = false;
    gMobitelBroj[playerid] = 0;
}

CMD:mobitel(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (gMobitelOtvoren[playerid]) MobitelZatvori(playerid);
    else MobitelOtvori(playerid);
    return 1;
}

CMD:phone(playerid, params[]) return cmd_mobitel(playerid, params);
CMD:telefon(playerid, params[]) return cmd_mobitel(playerid, params);
