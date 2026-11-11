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
#define MOBITEL_ANIM_MS     1000
#define MOBITEL_POZIV_POMAK 45.0

#define MOBITEL_IZBORNIK    -1
#define MOBITEL_NAZOVI      0
#define MOBITEL_IMENIK      1
#define MOBITEL_NOVI_BROJ   6
#define MOBITEL_NOVO_IME    7
#define MOBITEL_POSTAVKE    5
#define MOBITEL_DOLAZNI     8
#define MOBITEL_PORUKA      9
#define MOBITEL_ZVONI_MS    10000
#define MOBITEL_ZVONO       3600
#define MOBITEL_ZVONO_MS    2000
#define MOBITEL_BAZA        100000
#define MOBITEL_PORUKA_LEN  144

static const gMobitelIzbornik[MOBITEL_REDOVA][] = { "Nazovi", "Imenik", "Poruke", "Kamera", "Internet", "Postavke" };
static const gMobitelTipke[][] = { "1", "2", "3", "4", "5", "6" };

static PlayerText:gMobitelTd[MAX_PLAYERS][MOBITEL_TD], gMobitelBroj[MAX_PLAYERS];
static PlayerText:gMobitelNaslov[MAX_PLAYERS], PlayerText:gMobitelOznaka[MAX_PLAYERS], PlayerText:gMobitelStavke[MAX_PLAYERS][MOBITEL_REDOVA];
static bool:gMobitelOtvoren[MAX_PLAYERS], bool:gMobitelURuci[MAX_PLAYERS], gMobitelEkran[MAX_PLAYERS], gMobitelOdabir[MAX_PLAYERS], gMobitelUlaz[MAX_PLAYERS], KEY:gMobitelStareTipke[MAX_PLAYERS], gMobitelStaroGore[MAX_PLAYERS], gMobitelStaraStrana[MAX_PLAYERS], gMobitelAnim[MAX_PLAYERS], gMobitelAnimOd[MAX_PLAYERS], bool:gMobitelPoziv[MAX_PLAYERS], gMobitelSugovornik[MAX_PLAYERS][MOBITEL_IME];
static gMobitelDrugi[MAX_PLAYERS], bool:gMobitelSpojen[MAX_PLAYERS], gMobitelObavijest[MAX_PLAYERS], gMobitelPoruka[MAX_PLAYERS][MOBITEL_PORUKA_LEN];
static gMobitelUnos[MAX_PLAYERS][MOBITEL_IME], gMobitelNoviBroj[MAX_PLAYERS][MOBITEL_BROJ];
static gMobitelKontakata[MAX_PLAYERS], gMobitelKontaktIme[MAX_PLAYERS][MOBITEL_KONTAKATA][MOBITEL_IME], gMobitelKontaktBroj[MAX_PLAYERS][MOBITEL_KONTAKATA][MOBITEL_BROJ];

InitMobitel()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS phone_contacts (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, number TEXT NOT NULL, name TEXT NOT NULL)"));
    for (new i = 0; i < MAX_PLAYERS; i++) gMobitelDrugi[i] = INVALID_PLAYER_ID;
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

static bool:MobitelSkocni(ekran)
{
    return ekran == MOBITEL_DOLAZNI || ekran == MOBITEL_PORUKA;
}

static MobitelMojBroj(playerid, broj[], velicina = sizeof broj)
{
    format(broj, velicina, "%d", MOBITEL_BAZA + gAccountId[playerid]);
}

static MobitelIgrac(const broj[])
{
    new moj[MOBITEL_BROJ];
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || IsPlayerNPC(i) || !gUlogiran[i]) continue;
        MobitelMojBroj(i, moj);
        if (!strcmp(moj, broj)) return i;
    }
    return INVALID_PLAYER_ID;
}

static MobitelImeBroja(playerid, const broj[], ime[], velicina = sizeof ime)
{
    MobitelUcitaj(playerid);
    for (new k = 0; k < gMobitelKontakata[playerid]; k++)
    {
        if (strcmp(gMobitelKontaktBroj[playerid][k], broj)) continue;
        strcopy(ime, gMobitelKontaktIme[playerid][k], velicina);
        return;
    }
    strcopy(ime, broj, velicina);
}

static MobitelEkranPoruke(playerid, const naslov[], const tekst[], const pomoc[])
{
    PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], naslov);
    PlayerTextDrawHide(playerid, gMobitelOznaka[playerid]);
    for (new i = 0; i < MOBITEL_REDOVA; i++)
    {
        new PlayerText:td = gMobitelStavke[playerid][i];
        if (i && i < MOBITEL_REDOVA - 1)
        {
            PlayerTextDrawHide(playerid, td);
            continue;
        }
        PlayerTextDrawSetString(playerid, td, i ? pomoc : tekst);
        PlayerTextDrawColour(playerid, td, i ? 0x2E4A68FF : 0x1A1A1AFF);
        PlayerTextDrawShow(playerid, td);
    }
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
    if (gMobitelPoziv[playerid])
    {
        MobitelEkranPoruke(playerid, gMobitelSpojen[playerid] ? T(playerid, "U pozivu") : T(playerid, "Pozivam..."), gMobitelSugovornik[playerid], T(playerid, "N: prekini"));
        return;
    }
    if (ekran == MOBITEL_DOLAZNI)
    {
        MobitelEkranPoruke(playerid, T(playerid, "Dolazni poziv"), gMobitelSugovornik[playerid], T(playerid, "Y: javi se, N: odbij"));
        return;
    }
    if (ekran == MOBITEL_PORUKA)
    {
        MobitelEkranPoruke(playerid, gMobitelSugovornik[playerid], gMobitelPoruka[playerid], T(playerid, "Y: otvori, N: zatvori"));
        return;
    }
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
        else if (ekran == MOBITEL_POSTAVKE && !i)
        {
            new tekst[48];
            format(tekst, sizeof tekst, T(playerid, "Moj broj: %d"), MOBITEL_BAZA + gAccountId[playerid]);
            PlayerTextDrawSetString(playerid, td, tekst);
        }
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
    gMobitelPoziv[playerid] = false;
    gMobitelSpojen[playerid] = false;
    gMobitelDrugi[playerid] = INVALID_PLAYER_ID;
    gMobitelStaroGore[playerid] = 0;
    gMobitelStaraStrana[playerid] = 0;
    gMobitelAnim[playerid] = -1;
    gMobitelAnimOd[playerid] = GetTickCount() - MOBITEL_ANIM_MS;
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
    for (new i = 0; i < MOBITEL_REDOVA; i++)
    {
        gMobitelStavke[playerid][i] = MobitelTekst(playerid, 19.0, MOBITEL_STAVKA_Y + float(i) * MOBITEL_STAVKA_H, "_", 0x1A1A1AFF, 0.2, 0.9);
        PlayerTextDrawTextSize(playerid, gMobitelStavke[playerid][i], MOBITEL_X + 105.0, 0.0);
    }
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
    new drugi = gMobitelDrugi[playerid];
    if (gMobitelEkran[playerid] == MOBITEL_DOLAZNI) PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    gMobitelOtvoren[playerid] = false;
    gMobitelDrugi[playerid] = INVALID_PLAYER_ID;
    gMobitelSpojen[playerid] = false;
    gMobitelObavijest[playerid]++;
    if (gMobitelPoziv[playerid] && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USECELLPHONE) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
    gMobitelPoziv[playerid] = false;
    for (new i = 0; i < gMobitelBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gMobitelTd[playerid][i]);
    gMobitelBroj[playerid] = 0;
    if (gMobitelURuci[playerid])
    {
        RemovePlayerAttachedObject(playerid, MOBITEL_SLOT);
        ClearAnimations(playerid);
    }
    gMobitelURuci[playerid] = false;
    if (drugi == INVALID_PLAYER_ID || gMobitelDrugi[drugi] != playerid) return;
    SendClientMessage(drugi, COLOR_SMS, T(drugi, "Poziv je zavrsen."));
    MobitelZatvori(drugi);
}

static bool:MobitelDostupan(playerid)
{
    return gUlogiran[playerid] && !IsRanjen(playerid) && !ImaLisice(playerid) && gMobitelDrugi[playerid] == INVALID_PLAYER_ID && !gMobitelPoziv[playerid];
}

static MobitelDolazni(playerid, pozivatelj)
{
    new broj[MOBITEL_BROJ];
    if (!gMobitelOtvoren[playerid]) MobitelOtvori(playerid);
    gMobitelDrugi[playerid] = pozivatelj;
    MobitelMojBroj(pozivatelj, broj);
    MobitelImeBroja(playerid, broj, gMobitelSugovornik[playerid]);
    gMobitelEkran[playerid] = MOBITEL_DOLAZNI;
    MobitelPrikazi(playerid);
    SetTimerEx("MobitelIstek", MOBITEL_ZVONI_MS, false, "ii", playerid, ++gMobitelObavijest[playerid]);
    MobitelZvono(playerid, gMobitelObavijest[playerid]);
}

forward MobitelZvono(playerid, obavijest);
public MobitelZvono(playerid, obavijest)
{
    if (!gMobitelOtvoren[playerid] || gMobitelObavijest[playerid] != obavijest || gMobitelEkran[playerid] != MOBITEL_DOLAZNI) return;
    PlayerPlaySound(playerid, MOBITEL_ZVONO, 0.0, 0.0, 0.0);
    SetTimerEx("MobitelZvono", MOBITEL_ZVONO_MS, false, "ii", playerid, obavijest);
}

static MobitelNazovi(playerid, const broj[])
{
    if (!strcmp(broj, MOBITEL_TAKSI_BROJ)) return TaksiPozovi(playerid);
    new drugi = MobitelIgrac(broj), ime[MOBITEL_IME];
    if (drugi == INVALID_PLAYER_ID || drugi == playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Broj nije u upotrebi."));
    if (!MobitelDostupan(drugi)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pretplatnik trenutno nije dostupan."));
    MobitelImeBroja(playerid, broj, ime);
    MobitelPoziv(playerid, ime, drugi);
    MobitelDolazni(drugi, playerid);
    return 1;
}

static MobitelOdgovori(playerid)
{
    new drugi = gMobitelDrugi[playerid], ime[MOBITEL_IME];
    if (drugi == INVALID_PLAYER_ID || gMobitelDrugi[drugi] != playerid)
    {
        MobitelZatvori(playerid);
        return;
    }
    strcopy(ime, gMobitelSugovornik[playerid]);
    PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    gMobitelEkran[playerid] = MOBITEL_IZBORNIK;
    MobitelPoziv(playerid, ime, drugi);
    gMobitelSpojen[playerid] = true;
    gMobitelSpojen[drugi] = true;
    MobitelPrikazi(playerid);
    MobitelPrikazi(drugi);
}

forward MobitelIstek(playerid, obavijest);
public MobitelIstek(playerid, obavijest)
{
    if (gMobitelOtvoren[playerid] && gMobitelObavijest[playerid] == obavijest && MobitelSkocni(gMobitelEkran[playerid])) MobitelZatvori(playerid);
}

MobitelSms(playerid, const posiljatelj[], const tekst[])
{
    if (!gUlogiran[playerid]) return;
    if (!MobitelDostupan(playerid) || (gMobitelOtvoren[playerid] && MobitelUnosEkran(gMobitelEkran[playerid])))
    {
        SendClientMessage(playerid, COLOR_SMS, T(playerid, "SMS od %s: %s"), posiljatelj, tekst);
        return;
    }
    if (!gMobitelOtvoren[playerid]) MobitelOtvori(playerid);
    strcopy(gMobitelSugovornik[playerid], posiljatelj);
    strcopy(gMobitelPoruka[playerid], tekst);
    gMobitelEkran[playerid] = MOBITEL_PORUKA;
    MobitelPrikazi(playerid);
    SetTimerEx("MobitelIstek", MOBITEL_ZVONI_MS, false, "ii", playerid, ++gMobitelObavijest[playerid]);
}

MobitelKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gMobitelOtvoren[playerid]) return 0;
    new KEY:pritisnuto = newkeys & ~oldkeys, ekran = gMobitelEkran[playerid];
    if (ekran == MOBITEL_DOLAZNI && (pritisnuto & KEY_YES)) MobitelOdgovori(playerid);
    else if (ekran == MOBITEL_PORUKA && (pritisnuto & KEY_YES))
    {
        gMobitelEkran[playerid] = MOBITEL_IZBORNIK;
        gMobitelOdabir[playerid] = 0;
        gMobitelObavijest[playerid]++;
        MobitelPrikazi(playerid);
    }
    else if ((MobitelSkocni(ekran) || gMobitelPoziv[playerid]) && (pritisnuto & KEY_NO)) MobitelZatvori(playerid);
    else return 0;
    return 1;
}

MobitelRazgovor(playerid, const text[])
{
    new drugi = gMobitelDrugi[playerid];
    if (!gMobitelSpojen[playerid] || drugi == INVALID_PLAYER_ID) return;
    SendClientMessage(drugi, COLOR_SMS, T(drugi, "%s (telefon): %s"), gMobitelSugovornik[drugi], text);
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
        if (odabir == MOBITEL_NAZOVI) MobitelUnos(playerid, MOBITEL_NAZOVI, "Pomoc: Upisite broj u chat (T), pa pritisnite desnu strelicu (Odaberi).");
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
        else MobitelUnos(playerid, MOBITEL_NOVI_BROJ, "Pomoc: Upisite broj u chat (T), pa pritisnite desnu strelicu (Odaberi).");
    }
    else if (ekran == MOBITEL_NOVI_BROJ && gMobitelUnos[playerid][0])
    {
        strcopy(gMobitelNoviBroj[playerid], gMobitelUnos[playerid]);
        MobitelUnos(playerid, MOBITEL_NOVO_IME, "Pomoc: Upisite ime u chat (T), pa pritisnite desnu strelicu (Odaberi).");
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

static MobitelStoji(playerid)
{
    new idx = GetPlayerAnimationIndex(playerid);
    if (idx == gMobitelAnim[playerid]) return;
    new lib[32], ime[32];
    GetAnimationName(idx, lib, sizeof lib, ime, sizeof ime);
    if (!strcmp(ime, "IDLE_stance", true))
    {
        gMobitelAnim[playerid] = idx;
        return;
    }
    new sad = GetTickCount();
    if (sad - gMobitelAnimOd[playerid] < MOBITEL_ANIM_MS) return;
    gMobitelAnimOd[playerid] = sad;
    ApplyAnimation(playerid, "PED", "IDLE_stance", 4.1, true, false, false, false, 0, SYNC_ALL);
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
    if (gore != gMobitelStaroGore[playerid] && gore == KEY_UP) pritisnuto |= KEY_ANALOG_UP;
    if (gore != gMobitelStaroGore[playerid] && gore == KEY_DOWN) pritisnuto |= KEY_ANALOG_DOWN;
    if (strana != gMobitelStaraStrana[playerid] && strana == KEY_LEFT) pritisnuto |= KEY_ANALOG_LEFT;
    if (strana != gMobitelStaraStrana[playerid] && strana == KEY_RIGHT) pritisnuto |= KEY_ANALOG_RIGHT;
    gMobitelStareTipke[playerid] = tipke;
    gMobitelStaroGore[playerid] = gore;
    gMobitelStaraStrana[playerid] = strana;
    new bool:slobodan = !gMobitelPoziv[playerid] && !MobitelSkocni(gMobitelEkran[playerid]);
    if (slobodan && (pritisnuto & (KEY_ANALOG_UP | KEY_ANALOG_DOWN | KEY_ANALOG_LEFT | KEY_ANALOG_RIGHT))) MobitelTipka(playerid, pritisnuto);
    if (!gMobitelOtvoren[playerid]) return;
    new bool:ruka = slobodan && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_USECELLPHONE && GetTickCount() - gMobitelUlaz[playerid] >= MOBITEL_ULAZ_MS;
    if (ruka) MobitelStoji(playerid);
    if (ruka == gMobitelURuci[playerid]) return;
    gMobitelURuci[playerid] = ruka;
    if (ruka) SetPlayerAttachedObject(playerid, MOBITEL_SLOT, MOBITEL_MODEL, 6, 0.08, 0.03, -0.02, 180.0, 165.0, -5.0);
    else RemovePlayerAttachedObject(playerid, MOBITEL_SLOT);
}

MobitelPoziv(playerid, const ime[], drugi = INVALID_PLAYER_ID)
{
    if (!gMobitelOtvoren[playerid] || gMobitelPoziv[playerid]) return;
    gMobitelPoziv[playerid] = true;
    gMobitelDrugi[playerid] = drugi;
    gMobitelSpojen[playerid] = drugi == INVALID_PLAYER_ID;
    strcopy(gMobitelSugovornik[playerid], ime);
    if (gMobitelURuci[playerid])
    {
        RemovePlayerAttachedObject(playerid, MOBITEL_SLOT);
        ClearAnimations(playerid);
        gMobitelURuci[playerid] = false;
    }
    for (new i = 0; i < gMobitelBroj[playerid]; i++)
    {
        new Float:x, Float:y;
        PlayerTextDrawGetPos(playerid, gMobitelTd[playerid][i], x, y);
        PlayerTextDrawSetPos(playerid, gMobitelTd[playerid][i], x, y + MOBITEL_POZIV_POMAK);
        PlayerTextDrawShow(playerid, gMobitelTd[playerid][i]);
    }
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
    MobitelPrikazi(playerid);
}

MobitelKrajPoziva(playerid)
{
    if (gMobitelPoziv[playerid] && gMobitelDrugi[playerid] == INVALID_PLAYER_ID) MobitelZatvori(playerid);
}

MobitelEnterVehicle(playerid)
{
    gMobitelUlaz[playerid] = GetTickCount();
}

MobitelDisconnect(playerid)
{
    MobitelZatvori(playerid);
    gMobitelDrugi[playerid] = INVALID_PLAYER_ID;
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
