#define MOBITEL_TD          32
#define MOBITEL_X           490.0
#define MOBITEL_Y           276.0
#define MOBITEL_STAVKA_Y    46.0
#define MOBITEL_STAVKA_H    9.5
#define MOBITEL_REDOVA      6
#define MOBITEL_KONTAKATA   20
#define MOBITEL_IME         21
#define MOBITEL_BROJ        13
#define MOBITEL_TAKSI_BROJ  "555"
#define MOBITEL_SLOT        9
#define MOBITEL_MODEL       18868
#define MOBITEL_ULAZ_MS     3000

#define MOBITEL_IZBORNIK    -1
#define MOBITEL_NAZOVI      0
#define MOBITEL_IMENIK      1
#define MOBITEL_NOVI_BROJ   6
#define MOBITEL_NOVO_IME    7

static const gMobitelIzbornik[MOBITEL_REDOVA][] = { "Nazovi", "Imenik", "Poruke", "Kamera", "Internet", "Postavke" };
static const gMobitelTipke[][] = { "1", "2", "3", "4", "5", "6" };

static PlayerText:gMobitelTd[MAX_PLAYERS][MOBITEL_TD], gMobitelBroj[MAX_PLAYERS];
static PlayerText:gMobitelNaslov[MAX_PLAYERS], PlayerText:gMobitelOznaka[MAX_PLAYERS], PlayerText:gMobitelStavke[MAX_PLAYERS][MOBITEL_REDOVA];
static bool:gMobitelOtvoren[MAX_PLAYERS], bool:gMobitelURuci[MAX_PLAYERS], gMobitelEkran[MAX_PLAYERS], gMobitelOdabir[MAX_PLAYERS], gMobitelUlaz[MAX_PLAYERS], KEY:gMobitelStareTipke[MAX_PLAYERS];
static gMobitelUnos[MAX_PLAYERS][MOBITEL_IME], gMobitelNoviBroj[MAX_PLAYERS][MOBITEL_BROJ];
static gMobitelKontakata[MAX_PLAYERS], gMobitelKontaktIme[MAX_PLAYERS][MOBITEL_KONTAKATA][MOBITEL_IME], gMobitelKontaktBroj[MAX_PLAYERS][MOBITEL_KONTAKATA][MOBITEL_BROJ];

InitMobitel()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS phone_contacts (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, number TEXT NOT NULL, name TEXT NOT NULL)"));
}

static MobitelUcitaj(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT number, name FROM phone_contacts WHERE account = %d ORDER BY id LIMIT %d", gAccountId[playerid], MOBITEL_KONTAKATA);
    gMobitelKontakata[playerid] = 0;
    for (new n = DB_GetRowCount(res); gMobitelKontakata[playerid] < n; DB_SelectNextRow(res))
    {
        new k = gMobitelKontakata[playerid]++;
        DB_GetFieldString(res, 0, gMobitelKontaktBroj[playerid][k], MOBITEL_BROJ);
        DB_GetFieldString(res, 1, gMobitelKontaktIme[playerid][k], MOBITEL_IME);
    }
    DB_FreeResultSet(res);
}

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

static bool:MobitelUnosEkran(ekran)
{
    return ekran == MOBITEL_NAZOVI || ekran == MOBITEL_NOVI_BROJ || ekran == MOBITEL_NOVO_IME;
}

static MobitelBrojStavki(playerid)
{
    if (gMobitelEkran[playerid] == MOBITEL_IZBORNIK) return MOBITEL_REDOVA;
    if (gMobitelEkran[playerid] == MOBITEL_IMENIK) return 2 + gMobitelKontakata[playerid];
    return 0;
}

static MobitelPrikazi(playerid)
{
    new ekran = gMobitelEkran[playerid], broj = MobitelBrojStavki(playerid), odabir = gMobitelOdabir[playerid];
    new pocetak = odabir >= MOBITEL_REDOVA ? odabir - MOBITEL_REDOVA + 1 : 0;
    if (ekran == MOBITEL_IZBORNIK) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "IZBORNIK"));
    else if (ekran == MOBITEL_NOVI_BROJ) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Novi broj"));
    else if (ekran == MOBITEL_NOVO_IME) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Ime"));
    else PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, gMobitelIzbornik[ekran]));
    PlayerTextDrawSetPos(playerid, gMobitelOznaka[playerid], MOBITEL_X + 15.0, MOBITEL_Y + MOBITEL_STAVKA_Y + float(odabir - pocetak) * MOBITEL_STAVKA_H);
    if (broj) PlayerTextDrawShow(playerid, gMobitelOznaka[playerid]);
    else PlayerTextDrawHide(playerid, gMobitelOznaka[playerid]);
    for (new i = 0; i < MOBITEL_REDOVA; i++)
    {
        new PlayerText:td = gMobitelStavke[playerid][i], s = pocetak + i;
        if (MobitelUnosEkran(ekran) && !i) PlayerTextDrawSetString(playerid, td, gMobitelUnos[playerid][0] ? gMobitelUnos[playerid] : "-");
        else if (s >= broj)
        {
            PlayerTextDrawHide(playerid, td);
            continue;
        }
        else if (ekran == MOBITEL_IZBORNIK) PlayerTextDrawSetString(playerid, td, T(playerid, gMobitelIzbornik[s]));
        else if (s == 0) PlayerTextDrawSetString(playerid, td, T(playerid, "Dodaj broj"));
        else if (s == 1) PlayerTextDrawSetString(playerid, td, T(playerid, "Taksi"));
        else PlayerTextDrawSetString(playerid, td, gMobitelKontaktIme[playerid][s - 2]);
        PlayerTextDrawColour(playerid, td, broj && s == odabir ? 0xFFFFFFFF : 0x1A1A1AFF);
        PlayerTextDrawShow(playerid, td);
    }
}

static MobitelOtvori(playerid)
{
    new sat, minuta, vrijeme[8];
    gettime(sat, minuta);
    format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
    gMobitelOtvoren[playerid] = true;
    gMobitelURuci[playerid] = false;
    gMobitelEkran[playerid] = MOBITEL_IZBORNIK;
    gMobitelOdabir[playerid] = 0;
    gMobitelUlaz[playerid] = GetTickCount() - MOBITEL_ULAZ_MS;
    gMobitelStareTipke[playerid] = KEY:0;
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
    for (new i = 0; i < MOBITEL_REDOVA; i++) gMobitelStavke[playerid][i] = MobitelTekst(playerid, 19.0, MOBITEL_STAVKA_Y + float(i) * MOBITEL_STAVKA_H, "_", 0x1A1A1AFF, 0.2, 0.9);
    MobitelTekst(playerid, 16.0, 108.0, T(playerid, "Natrag"), 0x2E4A68FF, 0.17, 0.8);
    MobitelTekst(playerid, 104.0, 108.0, T(playerid, "Odaberi"), 0x2E4A68FF, 0.17, 0.8, TEXT_DRAW_ALIGN_RIGHT);
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

MobitelZatvori(playerid)
{
    if (!gMobitelOtvoren[playerid]) return;
    gMobitelOtvoren[playerid] = false;
    for (new i = 0; i < gMobitelBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gMobitelTd[playerid][i]);
    gMobitelBroj[playerid] = 0;
    if (gMobitelURuci[playerid]) RemovePlayerAttachedObject(playerid, MOBITEL_SLOT);
    gMobitelURuci[playerid] = false;
}

static MobitelNazovi(playerid, const broj[])
{
    if (!strcmp(broj, MOBITEL_TAKSI_BROJ)) TaksiPozovi(playerid);
    else SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Broj nije u upotrebi."));
}

static MobitelUnos(playerid, ekran, const pomoc[])
{
    gMobitelEkran[playerid] = ekran;
    gMobitelOdabir[playerid] = 0;
    gMobitelUnos[playerid][0] = EOS;
    SendClientMessage(playerid, COLOR_POMOC, T(playerid, pomoc));
}

static MobitelOdaberi(playerid)
{
    new ekran = gMobitelEkran[playerid], odabir = gMobitelOdabir[playerid];
    if (ekran == MOBITEL_IZBORNIK)
    {
        if (odabir == MOBITEL_NAZOVI) MobitelUnos(playerid, MOBITEL_NAZOVI, "Pomoc: Upisite broj u chat (T), pa pritisnite Num6 (Odaberi).");
        else
        {
            if (odabir == MOBITEL_IMENIK) MobitelUcitaj(playerid);
            gMobitelEkran[playerid] = odabir;
            gMobitelOdabir[playerid] = 0;
        }
    }
    else if (ekran == MOBITEL_NAZOVI && gMobitelUnos[playerid][0]) MobitelNazovi(playerid, gMobitelUnos[playerid]);
    else if (ekran == MOBITEL_IMENIK)
    {
        if (odabir == 1) TaksiPozovi(playerid);
        else if (odabir > 1) MobitelNazovi(playerid, gMobitelKontaktBroj[playerid][odabir - 2]);
        else if (gMobitelKontakata[playerid] >= MOBITEL_KONTAKATA) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Imenik je pun."));
        else MobitelUnos(playerid, MOBITEL_NOVI_BROJ, "Pomoc: Upisite broj u chat (T), pa pritisnite Num6 (Odaberi).");
    }
    else if (ekran == MOBITEL_NOVI_BROJ && gMobitelUnos[playerid][0])
    {
        strcopy(gMobitelNoviBroj[playerid], gMobitelUnos[playerid]);
        MobitelUnos(playerid, MOBITEL_NOVO_IME, "Pomoc: Upisite ime u chat (T), pa pritisnite Num6 (Odaberi).");
    }
    else if (ekran == MOBITEL_NOVO_IME && gMobitelUnos[playerid][0])
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO phone_contacts (account, number, name) VALUES (%d, '%q', '%q')", gAccountId[playerid], gMobitelNoviBroj[playerid], gMobitelUnos[playerid]));
        MobitelUcitaj(playerid);
        gMobitelEkran[playerid] = MOBITEL_IMENIK;
        gMobitelOdabir[playerid] = 1 + gMobitelKontakata[playerid];
    }
}

static MobitelTipka(playerid, KEY:pritisnuto)
{
    new broj = MobitelBrojStavki(playerid), ekran = gMobitelEkran[playerid];
    if (pritisnuto & KEY_ANALOG_LEFT)
    {
        if (ekran == MOBITEL_IZBORNIK)
        {
            MobitelZatvori(playerid);
            return;
        }
        gMobitelEkran[playerid] = ekran >= MOBITEL_REDOVA ? MOBITEL_IMENIK : MOBITEL_IZBORNIK;
        gMobitelOdabir[playerid] = ekran >= MOBITEL_REDOVA ? 0 : ekran;
    }
    else if (pritisnuto & KEY_ANALOG_RIGHT) MobitelOdaberi(playerid);
    else if (!broj) return;
    else if (pritisnuto & KEY_ANALOG_UP) gMobitelOdabir[playerid] = (gMobitelOdabir[playerid] + broj - 1) % broj;
    else if (pritisnuto & KEY_ANALOG_DOWN) gMobitelOdabir[playerid] = (gMobitelOdabir[playerid] + 1) % broj;
    else return;
    if (gMobitelOtvoren[playerid]) MobitelPrikazi(playerid);
}

MobitelText(playerid, const text[])
{
    if (!gMobitelOtvoren[playerid] || !MobitelUnosEkran(gMobitelEkran[playerid])) return 0;
    if (gMobitelEkran[playerid] == MOBITEL_NOVO_IME)
    {
        if (strfind(text, "~") != -1)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ime ne smije sadrzavati znak ~."));
            return 1;
        }
        strcat(gMobitelUnos[playerid], text, MOBITEL_IME);
    }
    else
    {
        for (new i = 0; text[i]; i++)
        {
            if (text[i] >= '0' && text[i] <= '9') continue;
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Broj smije sadrzavati samo znamenke."));
            return 1;
        }
        strcat(gMobitelUnos[playerid], text, MOBITEL_BROJ);
    }
    MobitelPrikazi(playerid);
    return 1;
}

MobitelUpdate(playerid)
{
    if (!gMobitelOtvoren[playerid]) return;
    if (IsRanjen(playerid) || ImaLisice(playerid))
    {
        MobitelZatvori(playerid);
        return;
    }
    new KEY:tipke, gore, strana;
    GetPlayerKeys(playerid, tipke, gore, strana);
    new KEY:pritisnuto = tipke & ~gMobitelStareTipke[playerid];
    if (tipke != gMobitelStareTipke[playerid] || gore || strana) SendClientMessage(playerid, -1, "tipke %d gore %d strana %d", _:tipke, gore, strana);
    gMobitelStareTipke[playerid] = tipke;
    if (pritisnuto & (KEY_ANALOG_UP | KEY_ANALOG_DOWN | KEY_ANALOG_LEFT | KEY_ANALOG_RIGHT)) MobitelTipka(playerid, pritisnuto);
    if (!gMobitelOtvoren[playerid]) return;
    new bool:ruka = GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_USECELLPHONE && GetTickCount() - gMobitelUlaz[playerid] >= MOBITEL_ULAZ_MS;
    if (ruka == gMobitelURuci[playerid]) return;
    gMobitelURuci[playerid] = ruka;
    if (ruka) SetPlayerAttachedObject(playerid, MOBITEL_SLOT, MOBITEL_MODEL, 6, 0.08, 0.03, -0.02, 180.0, 165.0, -5.0);
    else RemovePlayerAttachedObject(playerid, MOBITEL_SLOT);
}

MobitelEnterVehicle(playerid)
{
    gMobitelUlaz[playerid] = GetTickCount();
}

MobitelDisconnect(playerid)
{
    gMobitelOtvoren[playerid] = false;
    gMobitelURuci[playerid] = false;
    gMobitelBroj[playerid] = 0;
}

CMD:mobitel(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    if (gMobitelOtvoren[playerid]) MobitelZatvori(playerid);
    else MobitelOtvori(playerid);
    return 1;
}

CMD:phone(playerid, params[]) return cmd_mobitel(playerid, params);
CMD:telefon(playerid, params[]) return cmd_mobitel(playerid, params);
