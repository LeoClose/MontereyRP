#define MDT_TD              8
#define MDT_TEKST           1024

enum { MTD_OKVIR, MTD_EKRAN, MTD_NASLOV, MTD_OSOBA, MTD_VOZILO, MTD_POTJERNICE, MTD_IZADI, MTD_SADRZAJ }

static PlayerText:gMdtTd[MAX_PLAYERS][MDT_TD], bool:gMdtOtvoren[MAX_PLAYERS];

InitMdt()
{
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

static PlayerText:MdtKutija(playerid, Float:x, Float:y, Float:sirina, Float:visina, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawShow(playerid, td);
    return td;
}

static PlayerText:MdtGumb(playerid, Float:x, const tekst[], boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, 138.0, tekst);
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, td, 0.22, 1.0);
    PlayerTextDrawColour(playerid, td, 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 12.0, 80.0);
    PlayerTextDrawSetSelectable(playerid, td, true);
    PlayerTextDrawShow(playerid, td);
    return td;
}

static MdtPrikazi(playerid, const tekst[])
{
    PlayerTextDrawSetString(playerid, gMdtTd[playerid][MTD_SADRZAJ], tekst[0] ? tekst : "_");
    return 1;
}

static MdtOtvori(playerid)
{
    gMdtOtvoren[playerid] = true;
    gMdtTd[playerid][MTD_OKVIR] = MdtKutija(playerid, 110.0, 100.0, 420.0, 290.0, 0x1B1F24F5);
    gMdtTd[playerid][MTD_EKRAN] = MdtKutija(playerid, 118.0, 130.0, 404.0, 252.0, 0x0B1A2AF5);
    new PlayerText:td = CreatePlayerTextDraw(playerid, 320.0, 106.0, T(playerid, "LSPD - MOBILNI TERMINAL"));
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, td, 0.3, 1.3);
    PlayerTextDrawColour(playerid, td, 0x5B8DBFFF);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawShow(playerid, td);
    gMdtTd[playerid][MTD_NASLOV] = td;
    gMdtTd[playerid][MTD_OSOBA] = MdtGumb(playerid, 175.0, T(playerid, "OSOBA"), 0x2E4A68CC);
    gMdtTd[playerid][MTD_VOZILO] = MdtGumb(playerid, 270.0, T(playerid, "VOZILO"), 0x2E4A68CC);
    gMdtTd[playerid][MTD_POTJERNICE] = MdtGumb(playerid, 365.0, T(playerid, "POTJERNICE"), 0x2E4A68CC);
    gMdtTd[playerid][MTD_IZADI] = MdtGumb(playerid, 465.0, T(playerid, "IZADI"), 0x1E5AA8CC);
    td = CreatePlayerTextDraw(playerid, 126.0, 160.0, "_");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, td, 0.2, 0.95);
    PlayerTextDrawColour(playerid, td, 0xDCE4ECFF);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawShow(playerid, td);
    gMdtTd[playerid][MTD_SADRZAJ] = td;
    MdtPrikazi(playerid, T(playerid, "Odaberite pretragu: OSOBA, VOZILO ili POTJERNICE."));
    SelectTextDraw(playerid, 0x5B8DBFFF);
}

MdtZatvori(playerid)
{
    if (!gMdtOtvoren[playerid]) return;
    gMdtOtvoren[playerid] = false;
    for (new i = 0; i < MDT_TD; i++) PlayerTextDrawDestroy(playerid, gMdtTd[playerid][i]);
    CancelSelectTextDraw(playerid);
}

MdtDisconnect(playerid)
{
    gMdtOtvoren[playerid] = false;
}

static MdtPotjernice(playerid)
{
    new tekst[MDT_TEKST], red[160], DBResult:res = DB_ExecuteQuery(gDB, "SELECT replace(a.name, '_', ' '), a.wanted, IFNULL((SELECT reason || ' (' || officer || ')' FROM police_records WHERE account = a.id AND type = %d ORDER BY id DESC LIMIT 1), '-') FROM accounts a WHERE a.wanted > 0 ORDER BY a.wanted DESC, a.name LIMIT 14", ZAPIS_POTJERNICA);
    format(tekst, sizeof tekst, "~b~%s~n~~w~", T(playerid, "AKTIVNE POTJERNICE"));
    new n = DB_GetRowCount(res);
    for (new i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        new ime[MAX_PLAYER_NAME + 1], razlog[96];
        DB_GetFieldString(res, 0, ime, sizeof ime);
        DB_GetFieldString(res, 2, razlog, sizeof razlog);
        MdtCisti(razlog);
        format(red, sizeof red, "~r~%d ~w~%s - %s~n~", DB_GetFieldInt(res, 1), ime, razlog);
        strcat(tekst, red);
    }
    DB_FreeResultSet(res);
    if (!n) strcat(tekst, T(playerid, "Nitko nije trazen."));
    MdtPrikazi(playerid, tekst);
}

static MdtOsoba(playerid, const unos[])
{
    new racun = 0, ime[MAX_PLAYER_NAME + 1], trazi[MAX_PLAYER_NAME + 1], target = ProvjeriBroj(unos) ? FindPlayer(unos) : INVALID_PLAYER_ID;
    if (target != INVALID_PLAYER_ID && gUlogiran[target]) racun = gAccountId[target];
    else
    {
        strcopy(trazi, unos);
        for (new i = 0; trazi[i]; i++) if (trazi[i] == ' ') trazi[i] = '_';
        new DBResult:r = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q' COLLATE NOCASE", trazi);
        if (DB_GetRowCount(r)) racun = DB_GetFieldInt(r, 0);
        DB_FreeResultSet(r);
    }
    if (!racun) return MdtPrikazi(playerid, T(playerid, "Osoba nije pronadena u bazi."));
    new tekst[MDT_TEKST], red[160], DBResult:res = DB_ExecuteQuery(gDB, "SELECT replace(a.name, '_', ' '), a.wanted, IFNULL((SELECT number FROM phone_numbers WHERE account = a.id), '-'), (SELECT COUNT(*) FROM fines WHERE account = a.id AND paid = 0), (SELECT IFNULL(SUM(amount), 0) FROM fines WHERE account = a.id AND paid = 0) FROM accounts a WHERE a.id = %d", racun);
    DB_GetFieldString(res, 0, ime, sizeof ime);
    new broj[16], wanted = DB_GetFieldInt(res, 1);
    DB_GetFieldString(res, 2, broj, sizeof broj);
    format(tekst, sizeof tekst, "~b~%s~n~~w~%s: %s~n~%s: ", ime, T(playerid, "Telefon"), broj, T(playerid, "Potjernica"));
    if (wanted) format(red, sizeof red, "~r~%d~w~~n~", wanted);
    else format(red, sizeof red, "%s~n~", T(playerid, "Nema"));
    strcat(tekst, red);
    format(red, sizeof red, T(playerid, "Neplacene kazne: %d ($%d)~n~"), DB_GetFieldInt(res, 3), DB_GetFieldInt(res, 4));
    strcat(tekst, red);
    DB_FreeResultSet(res);
    strcat(tekst, T(playerid, "Vozila: "));
    res = DB_ExecuteQuery(gDB, "SELECT id, model FROM player_vehicles WHERE owner = %d LIMIT 5", racun);
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
    strcat(tekst, T(playerid, "~n~Kuca: "));
    res = DB_ExecuteQuery(gDB, "SELECT x, y FROM houses WHERE owner = %d LIMIT 1", racun);
    if (DB_GetRowCount(res)) strcat(tekst, CestaZona(DB_GetFieldFloat(res, 0), DB_GetFieldFloat(res, 1)));
    else strcat(tekst, "-");
    DB_FreeResultSet(res);
    format(red, sizeof red, "~n~~n~~b~%s~n~~w~", T(playerid, "DOSJE"));
    strcat(tekst, red);
    res = DB_ExecuteQuery(gDB, "SELECT strftime('%%d.%%m.%%Y', created, 'unixepoch'), type, reason, officer FROM (SELECT created, type, reason, officer FROM police_records WHERE account = %d UNION ALL SELECT created, 2, reason || ' ($' || amount || ')', officer FROM fines WHERE account = %d) ORDER BY created DESC LIMIT 8", racun, racun);
    n = DB_GetRowCount(res);
    for (new i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        new kada[12], razlog[80], policajac[MAX_PLAYER_NAME + 1], tip = DB_GetFieldInt(res, 1);
        DB_GetFieldString(res, 0, kada, sizeof kada);
        DB_GetFieldString(res, 2, razlog, sizeof razlog);
        DB_GetFieldString(res, 3, policajac, sizeof policajac);
        MdtCisti(razlog);
        format(red, sizeof red, "%s %s: %s (%s)~n~", kada, T(playerid, tip == ZAPIS_POTJERNICA ? ("Potjernica") : tip == ZAPIS_UHICENJE ? ("Uhicenje") : ("Kazna")), razlog, policajac);
        if (strlen(tekst) + strlen(red) >= sizeof tekst) break;
        strcat(tekst, red);
    }
    DB_FreeResultSet(res);
    if (!n) strcat(tekst, T(playerid, "Nema zapisa."));
    MdtPrikazi(playerid, tekst);
    return 1;
}

static MdtVozilo(playerid, const unos[])
{
    new v = 0;
    for (new i = 0; unos[i]; i++) if ('0' <= unos[i] <= '9') v = v * 10 + unos[i] - '0';
    if (v <= 0 || v >= MAX_VEHICLES || !GetVehicleModel(v)) return MdtPrikazi(playerid, T(playerid, "Vozilo s tim tablicama nije pronadeno."));
    new tekst[MDT_TEKST], vlasnik[MAX_ORG_NAME], tablica[12], racun = VlasnikVozila(v);
    if (!VlasnikIme(v, vlasnik) && !GetOrgIme(gOrgVehOrg[v], vlasnik)) strcat(vlasnik, T(playerid, "Grad Los Santos"));
    VoziloTablica(v, tablica);
    MdtCisti(vlasnik);
    format(tekst, sizeof tekst, "~b~%s~n~~w~%s: %s~n~%s: %s~n~", tablica, T(playerid, "Vozilo"), gImenaVozila[GetVehicleModel(v) - 400], T(playerid, "Vlasnik"), vlasnik);
    if (racun > 0 && racun != VLASNIK_PIJACA)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT wanted FROM accounts WHERE id = %d", racun);
        if (DB_GetRowCount(res) && DB_GetFieldInt(res, 0)) strcat(tekst, T(playerid, "~r~Vlasnik je trazen!"));
        DB_FreeResultSet(res);
    }
    MdtPrikazi(playerid, tekst);
    return 1;
}

MdtKlik(playerid, PlayerText:td)
{
    if (!gMdtOtvoren[playerid]) return 0;
    if (td == gMdtTd[playerid][MTD_IZADI]) MdtZatvori(playerid);
    else if (td == gMdtTd[playerid][MTD_POTJERNICE]) MdtPotjernice(playerid);
    else if (td == gMdtTd[playerid][MTD_OSOBA]) PrikaziDialog(playerid, DLG_MDT_OSOBA, DIALOG_STYLE_INPUT, T(playerid, "MDT - Osoba"), T(playerid, "Unesite ime i prezime ili ID igraca:"), T(playerid, "Trazi"), T(playerid, "Natrag"));
    else if (td == gMdtTd[playerid][MTD_VOZILO]) PrikaziDialog(playerid, DLG_MDT_VOZILO, DIALOG_STYLE_INPUT, T(playerid, "MDT - Vozilo"), T(playerid, "Unesite tablice (npr. LS-0042):"), T(playerid, "Trazi"), T(playerid, "Natrag"));
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
