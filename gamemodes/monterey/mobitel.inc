#define MOBITEL_TD          32
#define MOBITEL_X           490.0
#define MOBITEL_Y           276.0
#define MOBITEL_STAVKA_Y    46.0
#define MOBITEL_STAVKA_H    9.5
#define MOBITEL_REDOVA      6
#define MOBITEL_KONTAKATA   15
#define MOBITEL_IME         21
#define MOBITEL_BROJ        13
#define MOBITEL_TAKSI_BROJ  "555"
#define MOBITEL_MEHANICAR_BROJ "556"
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
#define MOBITEL_ZAUZET      10
#define MOBITEL_ZVONI_MS    10000
#define MOBITEL_ZVONO       3600
#define MOBITEL_ZVONO_MS    2000
#define MOBITEL_ZNAMENKI     7
#define MOBITEL_POKUSAJA    50
#define MOBITEL_PORUKA_LEN  144
#define MOBITEL_PORUKE      2
#define MOBITEL_KONTAKT     11
#define MOBITEL_PREIMENUJ   12
#define MOBITEL_BRISI       13
#define MOBITEL_SMS_BROJ    14
#define MOBITEL_SMS_UNOS    15
#define MOBITEL_CITAJ       16
#define MOBITEL_PORUKA_MAX  10
#define MOBITEL_SMS_ZVUK    1057
#define MOBITEL_JEZIK       17
#define MOBITEL_KAMERA      3
#define MOBITEL_BIRANJE     18
#define MOBITEL_BIRANJE_MS  3000
#define MOBITEL_FOTO        1
#define MOBITEL_SELFIE      2
#define MOBITEL_KAMERA_ZVUK 1132
#define MOBITEL_BLJESAK_MS  150
#define MOBITEL_OKRET       1.5
#define MOBITEL_NAGIB_MAX   60.0

static const gMobitelIzbornik[MOBITEL_REDOVA][] = { "Nazovi", "Imenik", "Poruke", "Kamera", "Internet", "Postavke" };
static const gMobitelTipke[][] = { "1", "2", "3", "4", "5", "6" };
static const gMobitelOpcije[][] = { "Nazovi", "Posalji poruku", "Preimenuj", "Obrisi" };
static const gMobitelJezici[BROJ_JEZIKA][] = { "Hrvatski", "English", "Russkiy", "Espanol", "Turkce" };

static PlayerText:gMobitelTd[MAX_PLAYERS][MOBITEL_TD], gMobitelBroj[MAX_PLAYERS];
static PlayerText:gMobitelNaslov[MAX_PLAYERS], PlayerText:gMobitelOznaka[MAX_PLAYERS], PlayerText:gMobitelStavke[MAX_PLAYERS][MOBITEL_REDOVA];
static bool:gMobitelOtvoren[MAX_PLAYERS], bool:gMobitelURuci[MAX_PLAYERS], gMobitelEkran[MAX_PLAYERS], gMobitelOdabir[MAX_PLAYERS], gMobitelUlaz[MAX_PLAYERS], KEY:gMobitelStareTipke[MAX_PLAYERS], gMobitelStaroGore[MAX_PLAYERS], gMobitelStaraStrana[MAX_PLAYERS], gMobitelAnim[MAX_PLAYERS], gMobitelAnimOd[MAX_PLAYERS], bool:gMobitelPoziv[MAX_PLAYERS], gMobitelSugovornik[MAX_PLAYERS][MOBITEL_IME];
static PlayerText:gMobitelLijevo[MAX_PLAYERS], PlayerText:gMobitelDesno[MAX_PLAYERS], bool:gMobitelDolje[MAX_PLAYERS];
static gMobitelMoj[MAX_PLAYERS][MOBITEL_BROJ], gMobitelKamera[MAX_PLAYERS], Text:gMobitelBljesak, Float:gMobitelKut[MAX_PLAYERS], Float:gMobitelNagib[MAX_PLAYERS];
static gMobitelDrugi[MAX_PLAYERS], bool:gMobitelSpojen[MAX_PLAYERS], bool:gMobitelZvucnik[MAX_PLAYERS], gMobitelObavijest[MAX_PLAYERS], gMobitelPoruka[MAX_PLAYERS][MOBITEL_PORUKA_LEN];
static gMobitelUnos[MAX_PLAYERS][MOBITEL_PORUKA_LEN], gMobitelNoviBroj[MAX_PLAYERS][MOBITEL_BROJ], gMobitelPrimatelj[MAX_PLAYERS][MOBITEL_BROJ];
static gMobitelKontakata[MAX_PLAYERS], gMobitelKontaktIme[MAX_PLAYERS][MOBITEL_KONTAKATA][MOBITEL_IME], gMobitelKontaktBroj[MAX_PLAYERS][MOBITEL_KONTAKATA][MOBITEL_BROJ];
static gMobitelKontaktId[MAX_PLAYERS][MOBITEL_KONTAKATA], gMobitelKontakt[MAX_PLAYERS];
static gMobitelPrimljenih[MAX_PLAYERS], gMobitelCitam[MAX_PLAYERS], gMobitelPorukaId[MAX_PLAYERS][MOBITEL_PORUKA_MAX], gMobitelPorukaBroj[MAX_PLAYERS][MOBITEL_PORUKA_MAX][MOBITEL_BROJ], gMobitelPorukaIme[MAX_PLAYERS][MOBITEL_PORUKA_MAX][MOBITEL_IME], bool:gMobitelPorukaNova[MAX_PLAYERS][MOBITEL_PORUKA_MAX];

InitMobitel()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS phone_contacts (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, number TEXT NOT NULL, name TEXT NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS phone_numbers (account INTEGER PRIMARY KEY, number TEXT NOT NULL UNIQUE)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS phone_defaults (account INTEGER PRIMARY KEY)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS phone_given (account INTEGER PRIMARY KEY)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS phone_messages (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, sender TEXT NOT NULL, text TEXT NOT NULL, sent INTEGER NOT NULL, unread INTEGER NOT NULL DEFAULT 1)"));
    for (new i = 0; i < MAX_PLAYERS; i++) gMobitelDrugi[i] = INVALID_PLAYER_ID;
    gMobitelBljesak = TextDrawCreate(-20.0, -20.0, "_");
    TextDrawLetterSize(gMobitelBljesak, 0.0, 60.0);
    TextDrawTextSize(gMobitelBljesak, 660.0, 0.0);
    TextDrawUseBox(gMobitelBljesak, true);
    TextDrawBoxColour(gMobitelBljesak, 0xFFFFFFCC);
    new broj[MOBITEL_BROJ];
    MobitelGenerirajBroj(broj);
    assert(strlen(broj) == MOBITEL_ZNAMENKI && ProvjeriBroj(broj) && broj[0] >= '2');
}

static bool:MobitelBrojZauzet(const broj[], racun = 0)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM phone_numbers WHERE number = '%q' AND account != %d", broj, racun);
    new bool:zauzet = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    return zauzet || !strcmp(broj, MOBITEL_TAKSI_BROJ) || !strcmp(broj, MOBITEL_MEHANICAR_BROJ);
}

static MobitelGenerirajBroj(broj[], velicina = sizeof broj)
{
    for (new i = 0; i < MOBITEL_POKUSAJA; i++)
    {
        format(broj, velicina, "%d", 2000000 + random(8000000));
        if (!MobitelBrojZauzet(broj)) return;
    }
}

MobitelPrijava(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT number FROM phone_numbers WHERE account = %d", gAccountId[playerid]);
    new bool:ima = DB_GetRowCount(res) > 0;
    if (ima) DB_GetFieldString(res, 0, gMobitelMoj[playerid], MOBITEL_BROJ);
    DB_FreeResultSet(res);
    if (!ima)
    {
        MobitelGenerirajBroj(gMobitelMoj[playerid]);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO phone_numbers (account, number) VALUES (%d, '%q')", gAccountId[playerid], gMobitelMoj[playerid]));
    }
    res = DB_ExecuteQuery(gDB, "SELECT 1 FROM phone_defaults WHERE account = %d", gAccountId[playerid]);
    new bool:dodano = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    if (!dodano)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO phone_contacts (account, number, name) VALUES (%d, '%q', 'Taksi')", gAccountId[playerid], MOBITEL_TAKSI_BROJ));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO phone_contacts (account, number, name) VALUES (%d, '%q', 'Mehanicar')", gAccountId[playerid], MOBITEL_MEHANICAR_BROJ));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO phone_defaults (account) VALUES (%d)", gAccountId[playerid]));
    }
    res = DB_ExecuteQuery(gDB, "SELECT 1 FROM phone_given WHERE account = %d", gAccountId[playerid]);
    dodano = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    if (!dodano)
    {
        GiveItem(playerid, ITEM_MOBITEL, 1);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO phone_given (account) VALUES (%d)", gAccountId[playerid]));
    }
    res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM phone_messages WHERE account = %d AND unread = 1", gAccountId[playerid]);
    new novih = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    if (novih) SendClientMessage(playerid, COLOR_SMS, T(playerid, "Imate %d novih poruka. Otvorite mobitel (/phone, Poruke)."), novih);
}

MobitelPostaviBroj(playerid, const params[])
{
    new idx = 0, p1[32], broj[32];
    if (!ProvjeriOvlasti(playerid, ALVL_MONEY)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, broj))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [novi broj, 7 znamenki]"));
    if (strlen(broj) != MOBITEL_ZNAMENKI || !ProvjeriBroj(broj)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Broj mora imati 7 znamenki."));
    new name[MAX_PLAYER_NAME + 1], target = FindPlayer(p1), racun = 0;
    if (target != INVALID_PLAYER_ID && gUlogiran[target])
    {
        GetPlayerName(target, name, sizeof name);
        racun = gAccountId[target];
    }
    else
    {
        target = INVALID_PLAYER_ID;
        strcat(name, p1);
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q'", name);
        if (DB_GetRowCount(res)) racun = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
    }
    if (!racun) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (MobitelBrojZauzet(broj, racun)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Taj broj vec postoji."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO phone_numbers (account, number) VALUES (%d, '%q')", racun, broj));
    if (target != INVALID_PLAYER_ID) strcopy(gMobitelMoj[target], broj);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s sada ima broj %s."), name, broj);
    return 1;
}

static MobitelUcitaj(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, number, name FROM phone_contacts WHERE account = %d ORDER BY id LIMIT %d", gAccountId[playerid], MOBITEL_KONTAKATA);
    gMobitelKontakata[playerid] = 0;
    for (new n = DB_GetRowCount(res); gMobitelKontakata[playerid] < n; DB_SelectNextRow(res))
    {
        new k = gMobitelKontakata[playerid]++;
        gMobitelKontaktId[playerid][k] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, gMobitelKontaktBroj[playerid][k], MOBITEL_BROJ);
        DB_GetFieldString(res, 2, gMobitelKontaktIme[playerid][k], MOBITEL_IME);
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
    return ekran == MOBITEL_NAZOVI || ekran == MOBITEL_NOVI_BROJ || ekran == MOBITEL_NOVO_IME || ekran == MOBITEL_PREIMENUJ || ekran == MOBITEL_SMS_BROJ || ekran == MOBITEL_SMS_UNOS;
}

static bool:MobitelSkocni(ekran)
{
    return ekran == MOBITEL_DOLAZNI || ekran == MOBITEL_PORUKA;
}

static MobitelMojBroj(playerid, broj[], velicina = sizeof broj)
{
    strcopy(broj, gMobitelMoj[playerid], velicina);
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
    MobitelImeIzImenika(playerid, broj, ime, velicina);
}

static MobitelImeIzImenika(playerid, const broj[], ime[], velicina)
{
    for (new k = 0; k < gMobitelKontakata[playerid]; k++)
    {
        if (strcmp(gMobitelKontaktBroj[playerid][k], broj)) continue;
        strcopy(ime, gMobitelKontaktIme[playerid][k], velicina);
        return;
    }
    if (!strcmp(broj, MOBITEL_TAKSI_BROJ)) strcopy(ime, T(playerid, "Taksi sluzba"), velicina);
    else if (!strcmp(broj, MOBITEL_MEHANICAR_BROJ)) strcopy(ime, T(playerid, "Mehanicar"), velicina);
    else strcopy(ime, broj, velicina);
}

static MobitelSpusti(playerid, bool:dolje)
{
    if (gMobitelDolje[playerid] == dolje) return;
    gMobitelDolje[playerid] = dolje;
    for (new i = 0; i < gMobitelBroj[playerid]; i++)
    {
        new Float:x, Float:y;
        PlayerTextDrawGetPos(playerid, gMobitelTd[playerid][i], x, y);
        PlayerTextDrawSetPos(playerid, gMobitelTd[playerid][i], x, dolje ? y + MOBITEL_POZIV_POMAK : y - MOBITEL_POZIV_POMAK);
        PlayerTextDrawShow(playerid, gMobitelTd[playerid][i]);
    }
}

static MobitelTipke(playerid, const lijevo[], const desno[])
{
    PlayerTextDrawSetString(playerid, gMobitelLijevo[playerid], lijevo);
    PlayerTextDrawSetString(playerid, gMobitelDesno[playerid], desno);
}

static MobitelEkranPoruke(playerid, const naslov[], const tekst[], const lijevo[], const desno[])
{
    PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], naslov);
    PlayerTextDrawHide(playerid, gMobitelOznaka[playerid]);
    PlayerTextDrawSetString(playerid, gMobitelStavke[playerid][0], tekst);
    PlayerTextDrawColour(playerid, gMobitelStavke[playerid][0], 0x1A1A1AFF);
    PlayerTextDrawShow(playerid, gMobitelStavke[playerid][0]);
    for (new i = 1; i < MOBITEL_REDOVA; i++) PlayerTextDrawHide(playerid, gMobitelStavke[playerid][i]);
    MobitelTipke(playerid, lijevo, desno);
}

static bool:MobitelZvoni(playerid)
{
    return gMobitelOtvoren[playerid] && (gMobitelEkran[playerid] == MOBITEL_DOLAZNI || (gMobitelPoziv[playerid] && !gMobitelSpojen[playerid] && gMobitelDrugi[playerid] != INVALID_PLAYER_ID));
}

static MobitelUcitajPoruke(playerid)
{
    MobitelUcitaj(playerid);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, sender, unread FROM phone_messages WHERE account = %d ORDER BY id DESC LIMIT %d", gAccountId[playerid], MOBITEL_PORUKA_MAX);
    gMobitelPrimljenih[playerid] = 0;
    for (new n = DB_GetRowCount(res); gMobitelPrimljenih[playerid] < n; DB_SelectNextRow(res))
    {
        new k = gMobitelPrimljenih[playerid]++;
        gMobitelPorukaId[playerid][k] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, gMobitelPorukaBroj[playerid][k], MOBITEL_BROJ);
        gMobitelPorukaNova[playerid][k] = DB_GetFieldInt(res, 2) != 0;
        MobitelImeIzImenika(playerid, gMobitelPorukaBroj[playerid][k], gMobitelPorukaIme[playerid][k], MOBITEL_IME);
    }
    DB_FreeResultSet(res);
}

static MobitelBrojStavki(playerid)
{
    if (gMobitelEkran[playerid] == MOBITEL_IZBORNIK) return MOBITEL_REDOVA;
    if (gMobitelEkran[playerid] == MOBITEL_IMENIK) return 1 + gMobitelKontakata[playerid];
    if (gMobitelEkran[playerid] == MOBITEL_KONTAKT) return sizeof gMobitelOpcije;
    if (gMobitelEkran[playerid] == MOBITEL_PORUKE) return 1 + gMobitelPrimljenih[playerid];
    if (gMobitelEkran[playerid] == MOBITEL_POSTAVKE) return 1;
    if (gMobitelEkran[playerid] == MOBITEL_JEZIK) return BROJ_JEZIKA;
    if (gMobitelEkran[playerid] == MOBITEL_KAMERA) return 2;
    return 0;
}

static MobitelPrikazi(playerid)
{
    new ekran = gMobitelEkran[playerid], broj = MobitelBrojStavki(playerid), odabir = gMobitelOdabir[playerid];
    new pocetak = odabir >= MOBITEL_REDOVA ? odabir - MOBITEL_REDOVA + 1 : 0;
    MobitelSpusti(playerid, gMobitelPoziv[playerid] || MobitelSkocni(ekran));
    if (gMobitelPoziv[playerid])
    {
        new bool:razgovor = gMobitelSpojen[playerid] && gMobitelDrugi[playerid] != INVALID_PLAYER_ID;
        MobitelEkranPoruke(playerid, gMobitelSpojen[playerid] ? T(playerid, "U pozivu") : T(playerid, "Pozivam..."), gMobitelSugovornik[playerid], T(playerid, "N: prekini"), T(playerid, !razgovor ? ("_") : gMobitelZvucnik[playerid] ? ("Y: slusalica") : ("Y: zvucnik")));
        return;
    }
    if (ekran == MOBITEL_DOLAZNI)
    {
        MobitelEkranPoruke(playerid, T(playerid, "Dolazni poziv"), gMobitelSugovornik[playerid], T(playerid, "N: odbij"), T(playerid, "Y: javi se"));
        return;
    }
    if (ekran == MOBITEL_PORUKA)
    {
        MobitelEkranPoruke(playerid, gMobitelSugovornik[playerid], gMobitelPoruka[playerid], T(playerid, "N: zatvori"), T(playerid, "Y: otvori"));
        return;
    }
    if (ekran == MOBITEL_ZAUZET)
    {
        MobitelEkranPoruke(playerid, gMobitelSugovornik[playerid], gMobitelPoruka[playerid], T(playerid, "Natrag"), "_");
        return;
    }
    if (ekran == MOBITEL_BIRANJE)
    {
        MobitelEkranPoruke(playerid, T(playerid, "Pozivam..."), gMobitelSugovornik[playerid], T(playerid, "Natrag"), "_");
        return;
    }
    if (ekran == MOBITEL_CITAJ)
    {
        MobitelEkranPoruke(playerid, gMobitelPorukaIme[playerid][gMobitelCitam[playerid]], gMobitelPoruka[playerid], T(playerid, "Natrag"), T(playerid, "Odgovori"));
        return;
    }
    if (ekran == MOBITEL_BRISI)
    {
        MobitelEkranPoruke(playerid, gMobitelKontaktIme[playerid][gMobitelKontakt[playerid]], T(playerid, "Obrisati kontakt?"), T(playerid, "Natrag"), T(playerid, "Obrisi"));
        return;
    }
    MobitelTipke(playerid, T(playerid, "Natrag"), T(playerid, "Odaberi"));
    if (ekran == MOBITEL_IZBORNIK) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "IZBORNIK"));
    else if (ekran == MOBITEL_NOVI_BROJ) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Novi broj"));
    else if (ekran == MOBITEL_NOVO_IME) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Ime"));
    else if (ekran == MOBITEL_PREIMENUJ) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Novo ime"));
    else if (ekran == MOBITEL_SMS_BROJ) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Primatelj"));
    else if (ekran == MOBITEL_SMS_UNOS) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Nova poruka"));
    else if (ekran == MOBITEL_KONTAKT) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], gMobitelKontaktIme[playerid][gMobitelKontakt[playerid]]);
    else if (ekran == MOBITEL_JEZIK) PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, "Jezik"));
    else PlayerTextDrawSetString(playerid, gMobitelNaslov[playerid], T(playerid, gMobitelIzbornik[ekran]));
    PlayerTextDrawSetPos(playerid, gMobitelOznaka[playerid], MOBITEL_X + 15.0, MOBITEL_Y + MOBITEL_STAVKA_Y + float(odabir - pocetak) * MOBITEL_STAVKA_H);
    if (broj) PlayerTextDrawShow(playerid, gMobitelOznaka[playerid]);
    else PlayerTextDrawHide(playerid, gMobitelOznaka[playerid]);
    for (new i = 0; i < MOBITEL_REDOVA; i++)
    {
        new PlayerText:td = gMobitelStavke[playerid][i], s = pocetak + i;
        if (MobitelUnosEkran(ekran) && !i) PlayerTextDrawSetString(playerid, td, gMobitelUnos[playerid][0] ? gMobitelUnos[playerid] : "-");
        else if (ekran == MOBITEL_POSTAVKE && i == 1)
        {
            new tekst[48];
            format(tekst, sizeof tekst, T(playerid, "Moj broj: %s"), gMobitelMoj[playerid]);
            PlayerTextDrawSetString(playerid, td, tekst);
        }
        else if (s >= broj)
        {
            PlayerTextDrawHide(playerid, td);
            continue;
        }
        else if (ekran == MOBITEL_IZBORNIK) PlayerTextDrawSetString(playerid, td, T(playerid, gMobitelIzbornik[s]));
        else if (ekran == MOBITEL_KONTAKT) PlayerTextDrawSetString(playerid, td, T(playerid, gMobitelOpcije[s]));
        else if (ekran == MOBITEL_POSTAVKE) PlayerTextDrawSetString(playerid, td, T(playerid, "Jezik"));
        else if (ekran == MOBITEL_JEZIK) PlayerTextDrawSetString(playerid, td, gMobitelJezici[s]);
        else if (ekran == MOBITEL_KAMERA) PlayerTextDrawSetString(playerid, td, T(playerid, s ? ("Selfie") : ("Fotoaparat")));
        else if (ekran == MOBITEL_PORUKE && s == 0) PlayerTextDrawSetString(playerid, td, T(playerid, "Nova poruka"));
        else if (ekran == MOBITEL_PORUKE)
        {
            new red[MOBITEL_IME + 2];
            if (gMobitelPorukaNova[playerid][s - 1]) strcat(red, "! ");
            strcat(red, gMobitelPorukaIme[playerid][s - 1]);
            PlayerTextDrawSetString(playerid, td, red);
        }
        else if (s == 0) PlayerTextDrawSetString(playerid, td, T(playerid, "Dodaj broj"));
        else PlayerTextDrawSetString(playerid, td, gMobitelKontaktIme[playerid][s - 1]);
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
    gMobitelDolje[playerid] = false;
    gMobitelSpojen[playerid] = false;
    gMobitelZvucnik[playerid] = false;
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
    gMobitelLijevo[playerid] = MobitelTekst(playerid, 16.0, 108.0, T(playerid, "Natrag"), 0x2E4A68FF, 0.17, 0.8);
    gMobitelDesno[playerid] = MobitelTekst(playerid, 104.0, 108.0, T(playerid, "Odaberi"), 0x2E4A68FF, 0.17, 0.8, TEXT_DRAW_ALIGN_RIGHT);
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
    MobitelKameraKraj(playerid);
    if (!gMobitelOtvoren[playerid]) return;
    new drugi = gMobitelDrugi[playerid];
    if (MobitelZvoni(playerid)) PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    gMobitelOtvoren[playerid] = false;
    gMobitelDrugi[playerid] = INVALID_PLAYER_ID;
    gMobitelSpojen[playerid] = false;
    gMobitelZvucnik[playerid] = false;
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
    return gUlogiran[playerid] && gInv[playerid][ITEM_MOBITEL] > 0 && !IsRanjen(playerid) && !ImaLisice(playerid) && gMobitelDrugi[playerid] == INVALID_PLAYER_ID && !gMobitelPoziv[playerid];
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
    if (gMobitelObavijest[playerid] != obavijest || !MobitelZvoni(playerid)) return;
    PlayerPlaySound(playerid, MOBITEL_ZVONO, 0.0, 0.0, 0.0);
    SetTimerEx("MobitelZvono", MOBITEL_ZVONO_MS, false, "ii", playerid, obavijest);
}

static MobitelNazovi(playerid, const broj[])
{
    if (!strcmp(broj, MOBITEL_TAKSI_BROJ)) return TaksiPozovi(playerid);
    if (!strcmp(broj, MOBITEL_MEHANICAR_BROJ)) return MehanicarPozovi(playerid);
    new drugi = MobitelIgrac(broj), ime[MOBITEL_IME];
    MobitelImeBroja(playerid, broj, ime);
    strcopy(gMobitelSugovornik[playerid], ime);
    if (drugi == playerid || (drugi == INVALID_PLAYER_ID && !MobitelBrojZauzet(broj)))
    {
        strcopy(gMobitelPoruka[playerid], T(playerid, "Broj nije u upotrebi."));
        gMobitelEkran[playerid] = MOBITEL_ZAUZET;
        MobitelPrikazi(playerid);
        return 1;
    }
    if (drugi == INVALID_PLAYER_ID || !MobitelDostupan(drugi))
    {
        gMobitelEkran[playerid] = MOBITEL_BIRANJE;
        MobitelPrikazi(playerid);
        PlayerPlaySound(playerid, MOBITEL_ZVONO, 0.0, 0.0, 0.0);
        SetTimerEx("MobitelNedostupan", MOBITEL_BIRANJE_MS, false, "ii", playerid, ++gMobitelObavijest[playerid]);
        return 1;
    }
    MobitelPoziv(playerid, ime, drugi);
    MobitelDolazni(drugi, playerid);
    MobitelZvono(playerid, ++gMobitelObavijest[playerid]);
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
    PlayerPlaySound(drugi, 0, 0.0, 0.0, 0.0);
    gMobitelEkran[playerid] = MOBITEL_IZBORNIK;
    MobitelPoziv(playerid, ime, drugi);
    gMobitelSpojen[playerid] = true;
    gMobitelSpojen[drugi] = true;
    MobitelPrikazi(playerid);
    MobitelPrikazi(drugi);
}

forward MobitelNedostupan(playerid, obavijest);
public MobitelNedostupan(playerid, obavijest)
{
    if (!gMobitelOtvoren[playerid] || gMobitelObavijest[playerid] != obavijest || gMobitelEkran[playerid] != MOBITEL_BIRANJE) return;
    PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    strcopy(gMobitelPoruka[playerid], T(playerid, "Korisnik nedostupan."));
    gMobitelEkran[playerid] = MOBITEL_ZAUZET;
    MobitelPrikazi(playerid);
}

static MobitelKamera(playerid, nacin)
{
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return;
    MobitelZatvori(playerid);
    gMobitelKamera[playerid] = nacin;
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    TogglePlayerControllable(playerid, false);
    SetPlayerAttachedObject(playerid, MOBITEL_SLOT, MOBITEL_MODEL, 6, 0.08, 0.03, -0.02, 180.0, 165.0, -5.0);
    ApplyAnimation(playerid, "PED", "gun_stand", 4.1, true, false, false, true, 0, SYNC_ALL);
    if (nacin == MOBITEL_FOTO)
    {
        gMobitelKut[playerid] = a;
        gMobitelNagib[playerid] = 0.0;
        MobitelPogled(playerid);
        SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Fotoaparat: strelicama okrecite pogled, lijevom tipkom misa slikajte. N za izlaz."));
        return;
    }
    SetPlayerCameraPos(playerid, x - floatsin(a, degrees) * 1.1, y + floatcos(a, degrees) * 1.1, z + 0.7);
    SetPlayerCameraLookAt(playerid, x, y, z + 0.6);
    SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Selfie: lijevom tipkom misa slikajte. N za izlaz."));
}

static MobitelPogled(playerid)
{
    new Float:x, Float:y, Float:z, Float:a = gMobitelKut[playerid], Float:n = gMobitelNagib[playerid];
    GetPlayerPos(playerid, x, y, z);
    new Float:fx = -floatsin(a, degrees), Float:fy = floatcos(a, degrees), Float:cx = x + fx * 0.35, Float:cy = y + fy * 0.35, Float:cz = z + 0.65;
    SetPlayerFacingAngle(playerid, a);
    SetPlayerCameraPos(playerid, cx, cy, cz);
    SetPlayerCameraLookAt(playerid, cx + fx * floatcos(n, degrees) * 10.0, cy + fy * floatcos(n, degrees) * 10.0, cz + floatsin(n, degrees) * 10.0);
}

MobitelKameraKraj(playerid)
{
    new nacin = gMobitelKamera[playerid];
    if (!nacin) return;
    gMobitelKamera[playerid] = 0;
    RemovePlayerAttachedObject(playerid, MOBITEL_SLOT);
    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
}

forward MobitelBljesakKraj(playerid);
public MobitelBljesakKraj(playerid)
{
    TextDrawHideForPlayer(playerid, gMobitelBljesak);
}

forward MobitelIstek(playerid, obavijest);
public MobitelIstek(playerid, obavijest)
{
    if (gMobitelOtvoren[playerid] && gMobitelObavijest[playerid] == obavijest && MobitelSkocni(gMobitelEkran[playerid])) MobitelZatvori(playerid);
}

static MobitelSms(playerid, const posiljatelj[], const tekst[])
{
    if (!gUlogiran[playerid] || !gInv[playerid][ITEM_MOBITEL]) return;
    PlayerPlaySound(playerid, MOBITEL_SMS_ZVUK, 0.0, 0.0, 0.0);
    if (!MobitelDostupan(playerid) || (gMobitelOtvoren[playerid] && MobitelUnosEkran(gMobitelEkran[playerid])))
    {
        SendClientMessage(playerid, COLOR_SMS, T(playerid, "Imate novu poruku od %s."), posiljatelj);
        return;
    }
    if (!gMobitelOtvoren[playerid]) MobitelOtvori(playerid);
    strcopy(gMobitelSugovornik[playerid], posiljatelj);
    strcopy(gMobitelPoruka[playerid], tekst);
    gMobitelEkran[playerid] = MOBITEL_PORUKA;
    MobitelPrikazi(playerid);
    SetTimerEx("MobitelIstek", MOBITEL_ZVONI_MS, false, "ii", playerid, ++gMobitelObavijest[playerid]);
}

MobitelPrimiPoruku(racun, const broj[], const tekst[])
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO phone_messages (account, sender, text, sent) VALUES (%d, '%q', '%q', %d)", racun, broj, tekst, gettime()));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM phone_messages WHERE account = %d AND id NOT IN (SELECT id FROM phone_messages WHERE account = %d ORDER BY id DESC LIMIT %d)", racun, racun, MOBITEL_PORUKA_MAX));
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || IsPlayerNPC(i) || !gUlogiran[i] || gAccountId[i] != racun) continue;
        new ime[MOBITEL_IME];
        MobitelImeBroja(i, broj, ime);
        MobitelSms(i, ime, tekst);
        return;
    }
}

MobitelTaksiSms(playerid, const tekst[])
{
    if (gUlogiran[playerid]) MobitelPrimiPoruku(gAccountId[playerid], MOBITEL_TAKSI_BROJ, tekst);
}

static bool:MobitelPosaljiPoruku(playerid, const broj[], const tekst[])
{
    if (!strcmp(broj, MOBITEL_TAKSI_BROJ) || !strcmp(broj, MOBITEL_MEHANICAR_BROJ))
    {
        SendClientMessage(playerid, COLOR_SMS, T(playerid, "Poruka je poslana."));
        return true;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT account FROM phone_numbers WHERE number = '%q'", broj);
    new racun = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    if (!racun)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Broj nije u upotrebi."));
        return false;
    }
    MobitelPrimiPoruku(racun, gMobitelMoj[playerid], tekst);
    SendClientMessage(playerid, COLOR_SMS, T(playerid, "Poruka je poslana."));
    return true;
}

static MobitelCitaj(playerid, k)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT text FROM phone_messages WHERE id = %d", gMobitelPorukaId[playerid][k]);
    gMobitelPoruka[playerid][0] = EOS;
    if (DB_GetRowCount(res)) DB_GetFieldString(res, 0, gMobitelPoruka[playerid], MOBITEL_PORUKA_LEN);
    DB_FreeResultSet(res);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE phone_messages SET unread = 0 WHERE id = %d", gMobitelPorukaId[playerid][k]));
    gMobitelPorukaNova[playerid][k] = false;
    gMobitelCitam[playerid] = k;
    gMobitelEkran[playerid] = MOBITEL_CITAJ;
}

MobitelKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    new KEY:pritisnuto = newkeys & ~oldkeys;
    if (gMobitelKamera[playerid])
    {
        if (pritisnuto & KEY_NO) MobitelKameraKraj(playerid);
        else if (pritisnuto & KEY_FIRE)
        {
            PlayerPlaySound(playerid, MOBITEL_KAMERA_ZVUK, 0.0, 0.0, 0.0);
            TextDrawShowForPlayer(playerid, gMobitelBljesak);
            SetTimerEx("MobitelBljesakKraj", MOBITEL_BLJESAK_MS, false, "i", playerid);
        }
        else return 0;
        return 1;
    }
    if (!gMobitelOtvoren[playerid]) return 0;
    new ekran = gMobitelEkran[playerid];
    if (ekran == MOBITEL_DOLAZNI && (pritisnuto & KEY_YES)) MobitelOdgovori(playerid);
    else if (ekran == MOBITEL_PORUKA && (pritisnuto & KEY_YES))
    {
        MobitelUcitajPoruke(playerid);
        gMobitelEkran[playerid] = MOBITEL_PORUKE;
        gMobitelOdabir[playerid] = 0;
        if (gMobitelPrimljenih[playerid]) MobitelCitaj(playerid, 0);
        gMobitelObavijest[playerid]++;
        MobitelPrikazi(playerid);
    }
    else if (gMobitelPoziv[playerid] && gMobitelSpojen[playerid] && gMobitelDrugi[playerid] != INVALID_PLAYER_ID && (pritisnuto & KEY_YES))
    {
        gMobitelZvucnik[playerid] = !gMobitelZvucnik[playerid];
        SendClientMessage(playerid, COLOR_INFO, T(playerid, gMobitelZvucnik[playerid] ? ("Zvucnik je ukljucen.") : ("Zvucnik je iskljucen.")));
        MobitelPrikazi(playerid);
    }
    else if ((MobitelSkocni(ekran) || gMobitelPoziv[playerid]) && (pritisnuto & KEY_NO)) MobitelZatvori(playerid);
    else return 0;
    return 1;
}

MobitelURazgovoru(playerid)
{
    return gMobitelOtvoren[playerid] && gMobitelPoziv[playerid] && gMobitelSpojen[playerid] ? 1 : 0;
}

static MobitelCuje(playerid, const poruka[], const a[], const b[] = "")
{
    if (gMobitelZvucnik[playerid]) PosaljiBlizu(playerid, CHAT_RANGE, COLOR_SMS, poruka, a, b, 1);
    else SendClientMessage(playerid, COLOR_SMS, T(playerid, poruka), a, b);
}

MobitelGovor(playerid, const text[], vrsta)
{
    new drugi = gMobitelDrugi[playerid];
    if (MobitelURazgovoru(playerid) && drugi != INVALID_PLAYER_ID) MobitelCuje(drugi, "%s (telefon): %s", gMobitelSugovornik[drugi], text);
    if (vrsta != CHAT_VIKANJE) return;
    new Float:x, Float:y, Float:z, vw = GetPlayerVirtualWorld(playerid), interior = GetPlayerInterior(playerid);
    GetPlayerPos(playerid, x, y, z);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (i == playerid || !IsPlayerConnected(i) || !MobitelURazgovoru(i)) continue;
        new r = gMobitelDrugi[i];
        if (r == INVALID_PLAYER_ID || r == playerid || GetPlayerVirtualWorld(i) != vw || GetPlayerInterior(i) != interior || GetPlayerDistanceFromPoint(i, x, y, z) > CHAT_RANGE) continue;
        MobitelCuje(r, "(telefon) Netko u pozadini vice: %s!", text);
    }
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
            if (odabir == MOBITEL_PORUKE) MobitelUcitajPoruke(playerid);
            gMobitelEkran[playerid] = odabir;
            gMobitelOdabir[playerid] = 0;
        }
    }
    else if (ekran == MOBITEL_NAZOVI && gMobitelUnos[playerid][0]) MobitelNazovi(playerid, gMobitelUnos[playerid]);
    else if (ekran == MOBITEL_KONTAKT)
    {
        new k = gMobitelKontakt[playerid];
        if (odabir == 0) MobitelNazovi(playerid, gMobitelKontaktBroj[playerid][k]);
        else if (odabir == 1)
        {
            strcopy(gMobitelPrimatelj[playerid], gMobitelKontaktBroj[playerid][k]);
            MobitelUnos(playerid, MOBITEL_SMS_UNOS, "Pomoc: Upisite poruku u chat (T), pa pritisnite desnu strelicu (Odaberi).");
        }
        else if (odabir == 2) MobitelUnos(playerid, MOBITEL_PREIMENUJ, "Pomoc: Upisite ime u chat (T), pa pritisnite desnu strelicu (Odaberi).");
        else gMobitelEkran[playerid] = MOBITEL_BRISI;
    }
    else if (ekran == MOBITEL_PREIMENUJ && gMobitelUnos[playerid][0])
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE phone_contacts SET name = '%q' WHERE id = %d AND account = %d", gMobitelUnos[playerid], gMobitelKontaktId[playerid][gMobitelKontakt[playerid]], gAccountId[playerid]));
        MobitelUcitaj(playerid);
        gMobitelEkran[playerid] = MOBITEL_KONTAKT;
        gMobitelOdabir[playerid] = 2;
    }
    else if (ekran == MOBITEL_BRISI)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM phone_contacts WHERE id = %d AND account = %d", gMobitelKontaktId[playerid][gMobitelKontakt[playerid]], gAccountId[playerid]));
        MobitelUcitaj(playerid);
        gMobitelEkran[playerid] = MOBITEL_IMENIK;
        gMobitelOdabir[playerid] = 0;
    }
    else if (ekran == MOBITEL_PORUKE)
    {
        if (odabir) MobitelCitaj(playerid, odabir - 1);
        else MobitelUnos(playerid, MOBITEL_SMS_BROJ, "Pomoc: Upisite broj u chat (T), pa pritisnite desnu strelicu (Odaberi).");
    }
    else if (ekran == MOBITEL_SMS_BROJ && gMobitelUnos[playerid][0])
    {
        strcopy(gMobitelPrimatelj[playerid], gMobitelUnos[playerid]);
        MobitelUnos(playerid, MOBITEL_SMS_UNOS, "Pomoc: Upisite poruku u chat (T), pa pritisnite desnu strelicu (Odaberi).");
    }
    else if (ekran == MOBITEL_SMS_UNOS && gMobitelUnos[playerid][0])
    {
        if (!MobitelPosaljiPoruku(playerid, gMobitelPrimatelj[playerid], gMobitelUnos[playerid])) return;
        MobitelUcitajPoruke(playerid);
        gMobitelEkran[playerid] = MOBITEL_PORUKE;
        gMobitelOdabir[playerid] = 0;
    }
    else if (ekran == MOBITEL_KAMERA) MobitelKamera(playerid, odabir ? MOBITEL_SELFIE : MOBITEL_FOTO);
    else if (ekran == MOBITEL_POSTAVKE)
    {
        gMobitelEkran[playerid] = MOBITEL_JEZIK;
        gMobitelOdabir[playerid] = gJezik[playerid];
    }
    else if (ekran == MOBITEL_JEZIK)
    {
        JezikPromijeni(playerid, odabir);
        MobitelZatvori(playerid);
        MobitelOtvori(playerid);
        gMobitelEkran[playerid] = MOBITEL_POSTAVKE;
        gMobitelOdabir[playerid] = 0;
    }
    else if (ekran == MOBITEL_CITAJ)
    {
        strcopy(gMobitelPrimatelj[playerid], gMobitelPorukaBroj[playerid][gMobitelCitam[playerid]]);
        MobitelUnos(playerid, MOBITEL_SMS_UNOS, "Pomoc: Upisite poruku u chat (T), pa pritisnite desnu strelicu (Odaberi).");
    }
    else if (ekran == MOBITEL_IMENIK)
    {
        if (odabir >= 1)
        {
            gMobitelKontakt[playerid] = odabir - 1;
            gMobitelEkran[playerid] = MOBITEL_KONTAKT;
            gMobitelOdabir[playerid] = 0;
        }
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
        gMobitelOdabir[playerid] = gMobitelKontakata[playerid];
    }
}

static MobitelNatrag(playerid, ekran)
{
    switch (ekran)
    {
        case MOBITEL_NOVI_BROJ, MOBITEL_NOVO_IME, MOBITEL_KONTAKT:
        {
            gMobitelEkran[playerid] = MOBITEL_IMENIK;
            gMobitelOdabir[playerid] = ekran == MOBITEL_KONTAKT ? gMobitelKontakt[playerid] + 1 : 0;
        }
        case MOBITEL_PREIMENUJ, MOBITEL_BRISI:
        {
            gMobitelEkran[playerid] = MOBITEL_KONTAKT;
            gMobitelOdabir[playerid] = ekran == MOBITEL_PREIMENUJ ? 2 : 3;
        }
        case MOBITEL_JEZIK:
        {
            gMobitelEkran[playerid] = MOBITEL_POSTAVKE;
            gMobitelOdabir[playerid] = 0;
        }
        case MOBITEL_SMS_BROJ, MOBITEL_SMS_UNOS, MOBITEL_CITAJ:
        {
            MobitelUcitajPoruke(playerid);
            gMobitelEkran[playerid] = MOBITEL_PORUKE;
            gMobitelOdabir[playerid] = 0;
        }
        default:
        {
            gMobitelEkran[playerid] = MOBITEL_IZBORNIK;
            gMobitelOdabir[playerid] = ekran >= 0 && ekran < MOBITEL_REDOVA ? ekran : 0;
        }
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
        MobitelNatrag(playerid, ekran);
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
    new ekran = gMobitelEkran[playerid];
    if (ekran == MOBITEL_SMS_UNOS)
    {
        if (strfind(text, "~") != -1)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Poruka ne smije sadrzavati znak ~."));
            return 1;
        }
        strcat(gMobitelUnos[playerid], text, MOBITEL_PORUKA_LEN);
    }
    else if (ekran == MOBITEL_NOVO_IME || ekran == MOBITEL_PREIMENUJ)
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
    if (gMobitelKamera[playerid] == MOBITEL_FOTO)
    {
        new KEY:k, gore, strana;
        GetPlayerKeys(playerid, k, gore, strana);
        if (!gore && !strana) return;
        if (strana == KEY_LEFT) gMobitelKut[playerid] += MOBITEL_OKRET;
        else if (strana == KEY_RIGHT) gMobitelKut[playerid] -= MOBITEL_OKRET;
        if (gore == KEY_UP && gMobitelNagib[playerid] < MOBITEL_NAGIB_MAX) gMobitelNagib[playerid] += MOBITEL_OKRET;
        else if (gore == KEY_DOWN && gMobitelNagib[playerid] > -MOBITEL_NAGIB_MAX) gMobitelNagib[playerid] -= MOBITEL_OKRET;
        MobitelPogled(playerid);
        return;
    }
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
    else
    {
        RemovePlayerAttachedObject(playerid, MOBITEL_SLOT);
        if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) ClearAnimations(playerid);
    }
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
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_USECELLPHONE) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
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
    gMobitelKamera[playerid] = 0;
    gMobitelDrugi[playerid] = INVALID_PLAYER_ID;
    gMobitelMoj[playerid][0] = EOS;
    gMobitelOtvoren[playerid] = false;
    gMobitelURuci[playerid] = false;
    gMobitelBroj[playerid] = 0;
}

MobitelPrebaci(playerid)
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    if (gMobitelKamera[playerid]) MobitelKameraKraj(playerid);
    else if (gMobitelOtvoren[playerid]) MobitelZatvori(playerid);
    else if (!gInv[playerid][ITEM_MOBITEL]) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate mobitel."));
    else MobitelOtvori(playerid);
    return 1;
}

CMD:mobitel(playerid, params[]) return MobitelPrebaci(playerid);

CMD:phone(playerid, params[]) return cmd_mobitel(playerid, params);
CMD:telefon(playerid, params[]) return cmd_mobitel(playerid, params);
