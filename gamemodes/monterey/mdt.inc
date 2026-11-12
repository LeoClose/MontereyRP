#define MDT_TD              40
#define MDT_TEKST           1024
#define MDT_X               120.0
#define MDT_Y               95.0
#define MDT_TAB_Y           57.0
#define MDT_TAB_H           16.0

enum { MDT_POCETNA, MDT_OSOBA, MDT_VOZILO, MDT_POTJERNICE }
enum { MTD_SAT, MTD_OZNAKA, MTD_NASLOV, MTD_FOTO_OKVIR, MTD_FOTO, MTD_NAZIVI, MTD_VRIJEDNOSTI, MTD_ZNACKA, MTD_KARTICA, MTD_ZAPISI, MTD_IZADI, MTD_LOCIRAJ, MTD_TAB, MTD_IMENOVANI }

static PlayerText:gMdtTd[MAX_PLAYERS][MDT_TD], gMdtBroj[MAX_PLAYERS], PlayerText:gMdt[MAX_PLAYERS][MTD_IMENOVANI + 3], bool:gMdtOtvoren[MAX_PLAYERS], gMdtEkran[MAX_PLAYERS], gMdtMeta[MAX_PLAYERS];
static const gMdtTabovi[][] = { "Pocetna", "Osoba", "Vozilo", "Potjernice" };

InitMdt()
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT vozacka FROM accounts LIMIT 1");
    if (res) DB_FreeResultSet(res);
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE accounts ADD COLUMN vozacka INTEGER NOT NULL DEFAULT 0"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS police_records (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, type INTEGER NOT NULL, reason TEXT NOT NULL, officer TEXT NOT NULL, created INTEGER NOT NULL)"));
}

MdtZapis(racun, tip, const razlog[], policajac)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO police_records (account, type, reason, officer, created) VALUES (%d, %d, '%q', '%q', %d)", racun, tip, razlog, GetRPIme(policajac), gettime()));
}

static MdtCisti(tekst[])
{
    for (new i = 0; tekst[i]; i++) if (tekst[i] == '~') tekst[i] = '-';
}

static PlayerText:MdtKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, MDT_X + x, MDT_Y + y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gMdtTd[playerid][gMdtBroj[playerid]++] = td;
    return td;
}

static PlayerText:MdtTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font = TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, MDT_X + x, MDT_Y + y, tekst);
    PlayerTextDrawFont(playerid, td, font);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    gMdtTd[playerid][gMdtBroj[playerid]++] = td;
    return td;
}

static MdtPostavi(playerid, dio, const tekst[])
{
    PlayerTextDrawSetString(playerid, gMdt[playerid][dio], tekst[0] ? tekst : "_");
    return 1;
}

static MdtZnacka(playerid, const tekst[], boja)
{
    if (!tekst[0])
    {
        PlayerTextDrawHide(playerid, gMdt[playerid][MTD_ZNACKA]);
        return;
    }
    PlayerTextDrawSetString(playerid, gMdt[playerid][MTD_ZNACKA], tekst);
    PlayerTextDrawBoxColour(playerid, gMdt[playerid][MTD_ZNACKA], boja);
    PlayerTextDrawShow(playerid, gMdt[playerid][MTD_ZNACKA]);
}

static MdtFoto(playerid, model, bool:vozilo = false)
{
    if (model < 0)
    {
        PlayerTextDrawHide(playerid, gMdt[playerid][MTD_FOTO]);
        PlayerTextDrawHide(playerid, gMdt[playerid][MTD_FOTO_OKVIR]);
        return;
    }
    PlayerTextDrawSetPreviewModel(playerid, gMdt[playerid][MTD_FOTO], model);
    if (vozilo) PlayerTextDrawSetPreviewRot(playerid, gMdt[playerid][MTD_FOTO], -15.0, 0.0, -35.0, 1.0);
    else PlayerTextDrawSetPreviewRot(playerid, gMdt[playerid][MTD_FOTO], 0.0, 0.0, 0.0, 1.0);
    PlayerTextDrawShow(playerid, gMdt[playerid][MTD_FOTO_OKVIR]);
    PlayerTextDrawShow(playerid, gMdt[playerid][MTD_FOTO]);
}

static MdtLocirajGumb(playerid)
{
    new prati = PolicijaLocira(playerid), t = gMdtMeta[playerid];
    if (t == MAX_PLAYERS || (prati == INVALID_PLAYER_ID && t == INVALID_PLAYER_ID))
    {
        PlayerTextDrawHide(playerid, gMdt[playerid][MTD_LOCIRAJ]);
        return;
    }
    PlayerTextDrawSetString(playerid, gMdt[playerid][MTD_LOCIRAJ], prati != INVALID_PLAYER_ID && (t == INVALID_PLAYER_ID || t == prati) ? T(playerid, "PREKINI PRACENJE") : T(playerid, "LOCIRAJ"));
    PlayerTextDrawShow(playerid, gMdt[playerid][MTD_LOCIRAJ]);
}

static MdtEkran(playerid, ekran, const naslov[])
{
    gMdtEkran[playerid] = ekran;
    gMdtMeta[playerid] = MAX_PLAYERS;
    PlayerTextDrawHide(playerid, gMdt[playerid][MTD_LOCIRAJ]);
    PlayerTextDrawSetPos(playerid, gMdt[playerid][MTD_OZNAKA], MDT_X + 12.0, MDT_Y + MDT_TAB_Y - 2.0 + float(ekran) * MDT_TAB_H);
    PlayerTextDrawShow(playerid, gMdt[playerid][MTD_OZNAKA]);
    for (new i = 0; i < sizeof gMdtTabovi; i++)
    {
        PlayerTextDrawColour(playerid, gMdt[playerid][MTD_TAB + i], i == ekran ? 0xFFFFFFFF : 0x8AA0B6FF);
        PlayerTextDrawShow(playerid, gMdt[playerid][MTD_TAB + i]);
    }
    MdtPostavi(playerid, MTD_NASLOV, naslov);
    MdtFoto(playerid, -1);
    MdtZnacka(playerid, "", 0);
}

static MdtDosje(playerid, racun, tekst[], size = sizeof (tekst))
{
    new red[128], DBResult:res = DB_ExecuteQuery(gDB, "SELECT strftime('%%d.%%m.%%Y', created, 'unixepoch'), type, substr(reason, 1, 38), officer, paid FROM (SELECT created, type, reason, officer, 1 AS paid FROM police_records WHERE account = %d UNION ALL SELECT created, 2, reason || ' ($' || amount || ')', officer, paid FROM fines WHERE account = %d) ORDER BY created DESC LIMIT 7", racun, racun);
    new n = DB_GetRowCount(res);
    for (new i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        new kada[12], razlog[72], policajac[MAX_PLAYER_NAME + 1], tip = DB_GetFieldInt(res, 1);
        DB_GetFieldString(res, 0, kada, sizeof kada);
        DB_GetFieldString(res, 2, razlog, sizeof razlog);
        DB_GetFieldString(res, 3, policajac, sizeof policajac);
        MdtCisti(razlog);
        if (!DB_GetFieldInt(res, 4)) strcat(razlog, T(playerid, " ~r~neplaceno"));
        format(red, sizeof red, "~w~%s  %s%s  ~w~%s  ~b~~h~~h~(%s)~n~", kada, tip == ZAPIS_POTJERNICA ? ("~r~") : tip == ZAPIS_UHICENJE ? ("~y~") : ("~b~~h~~h~"),
            T(playerid, tip == ZAPIS_POTJERNICA ? ("Potjernica") : tip == ZAPIS_UHICENJE ? ("Uhicenje") : ("Kazna")), razlog, policajac);
        if (strlen(tekst) + strlen(red) >= size) break;
        strcat(tekst, red, size);
    }
    DB_FreeResultSet(res);
    if (!n) strcat(tekst, T(playerid, "Nema zapisa."), size);
}

static MdtPocetna(playerid)
{
    new tekst[256], d, m, g, sat, minuta, ime[MAX_ORG_NAME], rang[ORG_RANK_LEN] = "-";
    getdate(g, m, d);
    gettime(sat, minuta);
    GetOrgIme(gOrg[playerid], ime);
    if (gOrgRank[playerid][0]) strcopy(rang, gOrgRank[playerid]);
    MdtCisti(rang);
    MdtCisti(ime);
    MdtEkran(playerid, MDT_POCETNA, T(playerid, "POCETNA"));
    format(tekst, sizeof tekst, "%s~n~%s~n~%s~n~%s", T(playerid, "Policajac"), T(playerid, "Rang"), T(playerid, "Jedinica"), T(playerid, "Datum"));
    MdtPostavi(playerid, MTD_NAZIVI, tekst);
    format(tekst, sizeof tekst, "%s~n~%s~n~%s~n~%02d.%02d.%d %02d:%02d", GetRPIme(playerid), rang, ime, d, m, g, sat, minuta);
    MdtPostavi(playerid, MTD_VRIJEDNOSTI, tekst);
    MdtFoto(playerid, GetPlayerSkin(playerid));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM accounts WHERE wanted > 0");
    format(tekst, sizeof tekst, T(playerid, "~w~Aktivnih potjernica: ~r~%d~n~~n~~w~Odaberite pretragu u izborniku lijevo."), DB_GetFieldInt(res, 0));
    DB_FreeResultSet(res);
    MdtPostavi(playerid, MTD_ZAPISI, tekst);
    MdtZnacka(playerid, T(playerid, "NA DUZNOSTI"), 0x2E7D32FF);
    gMdtMeta[playerid] = INVALID_PLAYER_ID;
    MdtLocirajGumb(playerid);
}

static MdtPotjernice(playerid)
{
    new tekst[MDT_TEKST], red[160], DBResult:res = DB_ExecuteQuery(gDB, "SELECT replace(a.name, '_', ' '), a.wanted, IFNULL((SELECT substr(reason, 1, 34) || ' (' || officer || ')' FROM police_records WHERE account = a.id AND type = %d ORDER BY id DESC LIMIT 1), '-') FROM accounts a WHERE a.wanted > 0 ORDER BY a.wanted DESC, a.name LIMIT 9", ZAPIS_POTJERNICA);
    new n = DB_GetRowCount(res);
    MdtEkran(playerid, MDT_POTJERNICE, T(playerid, "POTJERNICE"));
    for (new i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        new ime[MAX_PLAYER_NAME + 1], razlog[64];
        DB_GetFieldString(res, 0, ime, sizeof ime);
        DB_GetFieldString(res, 2, razlog, sizeof razlog);
        MdtCisti(razlog);
        format(red, sizeof red, "~r~%d  ~w~%s  ~b~~h~~h~%s~n~", DB_GetFieldInt(res, 1), ime, razlog);
        strcat(tekst, red);
    }
    DB_FreeResultSet(res);
    if (!n) strcat(tekst, T(playerid, "Nitko nije trazen."));
    MdtPostavi(playerid, MTD_ZAPISI, tekst);
    res = DB_ExecuteQuery(gDB, "SELECT COUNT(*), IFNULL(MAX(wanted), 0) FROM accounts WHERE wanted > 0");
    format(tekst, sizeof tekst, "%s~n~%s", T(playerid, "Trazenih"), T(playerid, "Najvisa razina"));
    MdtPostavi(playerid, MTD_NAZIVI, tekst);
    format(tekst, sizeof tekst, "%d~n~%d", DB_GetFieldInt(res, 0), DB_GetFieldInt(res, 1));
    MdtPostavi(playerid, MTD_VRIJEDNOSTI, tekst);
    DB_FreeResultSet(res);
    if (n) MdtZnacka(playerid, T(playerid, "AKTIVNO"), 0xB71C1CFF);
}

static MdtOsoba(playerid, const unos[])
{
    new racun = 0, trazi[MAX_PLAYER_NAME + 1], target = ProvjeriBroj(unos) ? FindPlayer(unos) : INVALID_PLAYER_ID;
    if (target != INVALID_PLAYER_ID && gUlogiran[target]) racun = gAccountId[target];
    else
    {
        strcopy(trazi, unos);
        for (new i = 0; trazi[i]; i++) if (trazi[i] == ' ') trazi[i] = '_';
        new DBResult:r = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q' COLLATE NOCASE", trazi);
        if (DB_GetRowCount(r)) racun = DB_GetFieldInt(r, 0);
        DB_FreeResultSet(r);
        target = INVALID_PLAYER_ID;
        for (new i = 0; i < MAX_PLAYERS; i++) if (racun && gUlogiran[i] && gAccountId[i] == racun) target = i;
    }
    MdtEkran(playerid, MDT_OSOBA, T(playerid, "OSOBA"));
    if (!racun)
    {
        MdtPostavi(playerid, MTD_NAZIVI, "_");
        MdtPostavi(playerid, MTD_VRIJEDNOSTI, "_");
        return MdtPostavi(playerid, MTD_ZAPISI, T(playerid, "Osoba nije pronadena u bazi."));
    }
    new tekst[MDT_TEKST], red[96], ime[MAX_PLAYER_NAME + 1], broj[16], DBResult:res = DB_ExecuteQuery(gDB, "SELECT replace(a.name, '_', ' '), a.wanted, IFNULL((SELECT number FROM phone_numbers WHERE account = a.id), '-'), a.level, a.zatvor, IFNULL((SELECT skin FROM player_style WHERE account = a.id), -1), a.vozacka FROM accounts a WHERE a.id = %d", racun);
    DB_GetFieldString(res, 0, ime, sizeof ime);
    DB_GetFieldString(res, 2, broj, sizeof broj);
    new wanted = DB_GetFieldInt(res, 1), level = DB_GetFieldInt(res, 3), zatvor = DB_GetFieldInt(res, 4), skin = DB_GetFieldInt(res, 5), vozacka = DB_GetFieldInt(res, 6);
    DB_FreeResultSet(res);
    MdtCisti(ime);
    MdtCisti(broj);
    MdtFoto(playerid, target != INVALID_PLAYER_ID ? GetPlayerSkin(target) : skin < 0 ? 0 : skin);
    format(tekst, sizeof tekst, "%s~n~%s~n~%s~n~%s~n~%s~n~%s", T(playerid, "Ime"), T(playerid, "Telefon"), T(playerid, "Vozacka dozvola"), T(playerid, "Level"), T(playerid, "Vozila"), T(playerid, "Kuca"));
    MdtPostavi(playerid, MTD_NAZIVI, tekst);
    format(tekst, sizeof tekst, "%s~n~%s~n~%s~n~%d~n~", ime, broj, vozacka ? T(playerid, "~g~Ima") : T(playerid, "~r~Nema"), level);
    res = DB_ExecuteQuery(gDB, "SELECT id, model FROM player_vehicles WHERE owner = %d LIMIT 2", racun);
    new n = DB_GetRowCount(res);
    for (new i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        new v = VlasnikPoDb(DB_GetFieldInt(res, 0)), tablica[12] = "-";
        if (v) VoziloTablica(v, tablica);
        format(red, sizeof red, "%s%s (%s)", i ? (", ") : (""), gImenaVozila[DB_GetFieldInt(res, 1) - 400], tablica);
        strcat(tekst, red);
    }
    DB_FreeResultSet(res);
    if (!n) strcat(tekst, "-");
    strcat(tekst, "~n~");
    res = DB_ExecuteQuery(gDB, "SELECT x, y FROM houses WHERE owner = %d LIMIT 1", racun);
    if (DB_GetRowCount(res)) strcat(tekst, CestaZona(DB_GetFieldFloat(res, 0), DB_GetFieldFloat(res, 1)));
    else strcat(tekst, "-");
    DB_FreeResultSet(res);
    MdtPostavi(playerid, MTD_VRIJEDNOSTI, tekst);
    tekst[0] = EOS;
    MdtDosje(playerid, racun, tekst);
    MdtPostavi(playerid, MTD_ZAPISI, tekst);
    if (zatvor) MdtZnacka(playerid, T(playerid, "U ZATVORU"), 0xE65100FF);
    else if (wanted)
    {
        format(red, sizeof red, T(playerid, "TRAZEN (%d)"), wanted);
        MdtZnacka(playerid, red, 0xB71C1CFF);
    }
    else MdtZnacka(playerid, T(playerid, "CIST"), 0x2E7D32FF);
    if (target != INVALID_PLAYER_ID && target != playerid)
    {
        gMdtMeta[playerid] = target;
        MdtLocirajGumb(playerid);
    }
    return 1;
}

static MdtVozilo(playerid, const unos[])
{
    new v = 0;
    for (new i = 0; unos[i] && v < MAX_VEHICLES; i++) if ('0' <= unos[i] <= '9') v = v * 10 + unos[i] - '0';
    MdtEkran(playerid, MDT_VOZILO, T(playerid, "VOZILO"));
    if (v <= 0 || v >= MAX_VEHICLES || !GetVehicleModel(v))
    {
        MdtPostavi(playerid, MTD_NAZIVI, "_");
        MdtPostavi(playerid, MTD_VRIJEDNOSTI, "_");
        return MdtPostavi(playerid, MTD_ZAPISI, T(playerid, "Vozilo s tim tablicama nije pronadeno."));
    }
    new tekst[MDT_TEKST], vlasnik[MAX_ORG_NAME], tablica[12], racun = VlasnikVozila(v), model = GetVehicleModel(v), wanted = 0;
    if (!VlasnikIme(v, vlasnik) && !GetOrgIme(gOrgVehOrg[v], vlasnik)) strcat(vlasnik, T(playerid, "Grad Los Santos"));
    VoziloTablica(v, tablica);
    MdtCisti(vlasnik);
    MdtFoto(playerid, model, true);
    format(tekst, sizeof tekst, "%s~n~%s~n~%s~n~%s", T(playerid, "Tablice"), T(playerid, "Model"), T(playerid, "Vlasnik"), T(playerid, "Registrirano"));
    MdtPostavi(playerid, MTD_NAZIVI, tekst);
    new bool:registrirano = (racun > 0 && racun != VLASNIK_PIJACA) || gOrgVehOrg[v] > 0;
    format(tekst, sizeof tekst, "%s~n~%s~n~%s~n~%s", tablica, gImenaVozila[model - 400], vlasnik, registrirano ? T(playerid, "~g~Da") : T(playerid, "~r~Ne"));
    MdtPostavi(playerid, MTD_VRIJEDNOSTI, tekst);
    tekst[0] = EOS;
    if (racun > 0 && racun != VLASNIK_PIJACA)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT wanted FROM accounts WHERE id = %d", racun);
        if (DB_GetRowCount(res)) wanted = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        MdtDosje(playerid, racun, tekst);
    }
    else strcat(tekst, T(playerid, "Nema zapisa."));
    MdtPostavi(playerid, MTD_ZAPISI, tekst);
    if (wanted) MdtZnacka(playerid, T(playerid, "VLASNIK TRAZEN"), 0xB71C1CFF);
    else MdtZnacka(playerid, T(playerid, "UREDNO"), 0x2E7D32FF);
    return 1;
}

static MdtOtvori(playerid)
{
    new sat, minuta, vrijeme[8];
    gettime(sat, minuta);
    format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
    gMdtOtvoren[playerid] = true;
    gMdtBroj[playerid] = 0;
    MdtKutija(playerid, -14.0, 268.0, 428.0, 12.0, 0x1A1A1AFF);
    MdtKutija(playerid, 170.0, 276.0, 60.0, 3.0, 0x0A0A0AFF);
    MdtKutija(playerid, 0.0, 0.0, 400.0, 270.0, 0x0A0A0AFF);
    MdtKutija(playerid, 198.0, 1.0, 4.0, 2.0, 0x3A3A3AFF);
    MdtKutija(playerid, 4.0, 4.0, 392.0, 262.0, 0x0F1A27FF);
    MdtKutija(playerid, 4.0, 4.0, 392.0, 20.0, 0x1F3A5FFF);
    MdtTekst(playerid, 15.0, 12.0, "LSPD", 0xFFFFFFFF, 0.22, 1.0);
    MdtTekst(playerid, 200.0, 12.0, T(playerid, "MOBILNI PODATKOVNI TERMINAL"), 0xDCE4ECFF, 0.18, 0.95, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    gMdt[playerid][MTD_SAT] = MdtTekst(playerid, 385.0, 12.0, vrijeme, 0xFFFFFFFF, 0.2, 0.95, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_RIGHT);
    MdtKutija(playerid, 4.0, 24.0, 96.0, 242.0, 0x15212FFF);
    MdtTekst(playerid, 15.0, 29.0, T(playerid, "Policajac"), 0x8AA0B6FF, 0.16, 0.8);
    MdtTekst(playerid, 15.0, 38.0, GetRPIme(playerid), 0xFFFFFFFF, 0.17, 0.85);
    MdtKutija(playerid, 14.0, 51.0, 82.0, 1.0, 0x2E4A68FF);
    gMdt[playerid][MTD_OZNAKA] = MdtKutija(playerid, 12.0, MDT_TAB_Y - 2.0, 86.0, MDT_TAB_H - 2.0, 0x5B8DBFFF);
    for (new i = 0; i < sizeof gMdtTabovi; i++)
    {
        new PlayerText:td = MdtTekst(playerid, 18.0, MDT_TAB_Y + float(i) * MDT_TAB_H, T(playerid, gMdtTabovi[i]), 0x8AA0B6FF, 0.2, 0.95);
        PlayerTextDrawTextSize(playerid, td, MDT_X + 96.0, 10.0);
        PlayerTextDrawSetSelectable(playerid, td, true);
        gMdt[playerid][MTD_TAB + i] = td;
    }
    gMdt[playerid][MTD_IZADI] = MdtTekst(playerid, 55.0, 243.0, T(playerid, "IZADI"), 0xFFFFFFFF, 0.2, 0.95, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gMdt[playerid][MTD_IZADI], true);
    PlayerTextDrawBoxColour(playerid, gMdt[playerid][MTD_IZADI], 0x1E5AA8FF);
    PlayerTextDrawTextSize(playerid, gMdt[playerid][MTD_IZADI], 12.0, 78.0);
    PlayerTextDrawSetSelectable(playerid, gMdt[playerid][MTD_IZADI], true);
    gMdt[playerid][MTD_LOCIRAJ] = MdtTekst(playerid, 346.0, 118.0, "_", 0xFFFFFFFF, 0.16, 0.8, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gMdt[playerid][MTD_LOCIRAJ], true);
    PlayerTextDrawBoxColour(playerid, gMdt[playerid][MTD_LOCIRAJ], 0x5B8DBFFF);
    PlayerTextDrawTextSize(playerid, gMdt[playerid][MTD_LOCIRAJ], 10.0, 72.0);
    PlayerTextDrawSetSelectable(playerid, gMdt[playerid][MTD_LOCIRAJ], true);
    gMdt[playerid][MTD_NASLOV] = MdtTekst(playerid, 108.0, 29.0, "_", 0x5B8DBFFF, 0.26, 1.2);
    MdtKutija(playerid, 108.0, 44.0, 276.0, 1.0, 0x2E4A68FF);
    gMdt[playerid][MTD_FOTO_OKVIR] = MdtKutija(playerid, 108.0, 50.0, 66.0, 80.0, 0x1A2B3DFF);
    new PlayerText:foto = CreatePlayerTextDraw(playerid, MDT_X + 110.0, MDT_Y + 52.0, "_");
    PlayerTextDrawFont(playerid, foto, TEXT_DRAW_FONT_MODEL_PREVIEW);
    PlayerTextDrawTextSize(playerid, foto, 62.0, 76.0);
    PlayerTextDrawBackgroundColour(playerid, foto, 0x00000000);
    PlayerTextDrawSetPreviewModel(playerid, foto, 0);
    gMdtTd[playerid][gMdtBroj[playerid]++] = foto;
    gMdt[playerid][MTD_FOTO] = foto;
    gMdt[playerid][MTD_NAZIVI] = MdtTekst(playerid, 182.0, 52.0, "_", 0x8AA0B6FF, 0.18, 0.95, TEXT_DRAW_FONT_1);
    gMdt[playerid][MTD_VRIJEDNOSTI] = MdtTekst(playerid, 252.0, 52.0, "_", 0xFFFFFFFF, 0.18, 0.95, TEXT_DRAW_FONT_1);
    gMdt[playerid][MTD_ZNACKA] = MdtTekst(playerid, 346.0, 30.0, "_", 0xFFFFFFFF, 0.16, 0.8, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gMdt[playerid][MTD_ZNACKA], true);
    PlayerTextDrawTextSize(playerid, gMdt[playerid][MTD_ZNACKA], 10.0, 72.0);
    gMdt[playerid][MTD_KARTICA] = MdtKutija(playerid, 108.0, 138.0, 276.0, 116.0, 0x132233FF);
    MdtTekst(playerid, 113.0, 141.0, T(playerid, "DOSJE"), 0x5B8DBFFF, 0.18, 0.85);
    gMdt[playerid][MTD_ZAPISI] = MdtTekst(playerid, 113.0, 153.0, "_", 0xDCE4ECFF, 0.16, 0.85, TEXT_DRAW_FONT_1);
    for (new i = 0; i < gMdtBroj[playerid]; i++) PlayerTextDrawShow(playerid, gMdtTd[playerid][i]);
    MdtPocetna(playerid);
    SelectTextDraw(playerid, 0x5B8DBFFF);
}

MdtZatvori(playerid)
{
    if (!gMdtOtvoren[playerid]) return;
    gMdtOtvoren[playerid] = false;
    for (new i = 0; i < gMdtBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gMdtTd[playerid][i]);
    gMdtBroj[playerid] = 0;
    CancelSelectTextDraw(playerid);
}

MdtKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gMdtOtvoren[playerid] || !(newkeys & KEY_NO) || (oldkeys & KEY_NO)) return 0;
    MdtZatvori(playerid);
    return 1;
}

MdtStateChange(playerid, PLAYER_STATE:oldstate)
{
    if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER) MdtZatvori(playerid);
}

MdtDisconnect(playerid)
{
    gMdtOtvoren[playerid] = false;
    gMdtBroj[playerid] = 0;
}

MdtKlik(playerid, PlayerText:td)
{
    if (!gMdtOtvoren[playerid]) return 0;
    if (td == gMdt[playerid][MTD_IZADI]) MdtZatvori(playerid);
    else if (td == gMdt[playerid][MTD_LOCIRAJ])
    {
        new t = gMdtMeta[playerid], prati = PolicijaLocira(playerid);
        if (t == MAX_PLAYERS) return 1;
        if (prati != INVALID_PLAYER_ID && (t == INVALID_PLAYER_ID || t == prati)) PolicijaLocirajPrekini(playerid);
        else if (t != INVALID_PLAYER_ID && IsPlayerConnected(t) && gUlogiran[t]) PolicijaLociraj(playerid, t);
        MdtLocirajGumb(playerid);
    }
    else if (td == gMdt[playerid][MTD_TAB + MDT_POCETNA]) MdtPocetna(playerid);
    else if (td == gMdt[playerid][MTD_TAB + MDT_POTJERNICE]) MdtPotjernice(playerid);
    else if (td == gMdt[playerid][MTD_TAB + MDT_OSOBA]) PrikaziDialog(playerid, DLG_MDT_OSOBA, DIALOG_STYLE_INPUT, T(playerid, "MDT - Osoba"), T(playerid, "Unesite ime i prezime ili ID igraca:"), T(playerid, "Trazi"), T(playerid, "Natrag"));
    else if (td == gMdt[playerid][MTD_TAB + MDT_VOZILO]) PrikaziDialog(playerid, DLG_MDT_VOZILO, DIALOG_STYLE_INPUT, T(playerid, "MDT - Vozilo"), T(playerid, "Unesite tablice (npr. LS-0042):"), T(playerid, "Trazi"), T(playerid, "Natrag"));
    return 1;
}

DLG:mdt_osoba(playerid, response, const inputtext[])
{
    if (!gMdtOtvoren[playerid]) return 1;
    SelectTextDraw(playerid, 0x5B8DBFFF);
    if (response && inputtext[0] && strlen(inputtext) <= MAX_PLAYER_NAME) MdtOsoba(playerid, inputtext);
    return 1;
}

DLG:mdt_vozilo(playerid, response, const inputtext[])
{
    if (!gMdtOtvoren[playerid]) return 1;
    SelectTextDraw(playerid, 0x5B8DBFFF);
    if (response && inputtext[0]) MdtVozilo(playerid, inputtext);
    return 1;
}

CMD:mdt(playerid, params[])
{
    if (!PDNaDuznosti(playerid)) return 1;
    if (gMdtOtvoren[playerid])
    {
        MdtZatvori(playerid);
        return 1;
    }
    new v = GetPlayerVehicleID(playerid);
    if (!(v && GetOrgTip(gOrgVehOrg[v]) == ORG_LSPD) && GetPlayerVirtualWorld(playerid) != HQ_SVIJET + gOrg[playerid])
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "MDT je dostupan samo u policijskom vozilu ili HQ-u."));
    MdtOtvori(playerid);
    return 1;
}
