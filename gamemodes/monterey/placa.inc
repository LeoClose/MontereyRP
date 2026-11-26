#define PLACA_DRZAVNA       280
#define PLACA_PRIVATNA      160
#define PLACA_POMOC         100
#define PLACA_POMOC_LEVEL   5
#define PLACA_POREZ         15
#define PLACA_IMOVINA       40000
#define POSAO_NEMA          0
#define POSAO_DOSTAVLJAC    1
#define POSAO_KONTROLOR     2
#define POSAO_RIBAR         3
#define POSAO_RESTORAN         4
#define POSAO_SKLADISTAR    5
#define POSAO_PRAONICA      6
#define ATC_ODOBRENJE       40
#define ATC_DOMET           80.0
#define DOSTAVA_VOZILO      499
#define DOSTAVA_BAZA        30
#define DOSTAVA_PO_KM       25
#define DOSTAVA_DEPO_DOMET  20.0
#define DOSTAVA_MIN_M       150.0
#define HRANA_BAZA          20
#define HRANA_PO_KM         20
#define HRANA_MIN_M         200.0
#define HRANA_DOMET         4.0
#define HRANA_CIJENA        15

static const Float:gToranj[3] = { 1610.0, -2285.0, 13.5 };
static gPosao[MAX_PLAYERS], gPlacaDuty[MAX_PLAYERS], gDostavaVozilo[MAX_PLAYERS], bool:gDostavaCilj[MAX_PLAYERS], Float:gDostavaOd[MAX_PLAYERS][2];
static const Float:gRestoran[][3] = {
    { 811.98, -1616.02, 13.62 },
    { 1199.13, -918.07, 43.32 },
    { 928.53, -1352.77, 13.43 },
    { 2105.32, -1806.49, 13.69 },
    { 2397.83, -1898.65, 13.71 },
    { 2419.95, -1509.8, 24.16 }
};
static gHrana[MAX_PLAYERS], Float:gHranaCilj[MAX_PLAYERS][3], Float:gHranaOd[MAX_PLAYERS][2], gHranaStand[MAX_PLAYERS];

RestoranBroj()
{
    return sizeof gRestoran;
}

RestoranPozicija(i, &Float:x, &Float:y, &Float:z)
{
    if (i < 0 || i >= sizeof gRestoran) return 0;
    x = gRestoran[i][0];
    y = gRestoran[i][1];
    z = gRestoran[i][2];
    return 1;
}
static Float:gDepo[4], bool:gImaDepo, gDepoIkona, STREAMER_TAG_3D_TEXT_LABEL:gDepoTekst;

InitPlaca()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_depot (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z, a FROM job_depot WHERE id = 1");
    if (DB_GetRowCount(res))
    {
        for (new k = 0; k < 4; k++) gDepo[k] = DB_GetFieldFloat(res, k);
        DepoStvori();
    }
    DB_FreeResultSet(res);
}

static DepoStvori()
{
    if (gImaDepo)
    {
        MapaIkonaMakni(gDepoIkona);
        DestroyDynamic3DTextLabel(gDepoTekst);
    }
    gImaDepo = true;
    gDepoIkona = MapaIkona(gDepo[0], gDepo[1], gDepo[2], 51);
    gDepoTekst = CreateDynamic3DTextLabel("Depo\n/posao", COLOR_ORG, gDepo[0], gDepo[1], gDepo[2] + 0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
}

PlacaUcitaj(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT job FROM accounts WHERE id = %d", gAccountId[playerid]);
    gPosao[playerid] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : POSAO_NEMA;
    DB_FreeResultSet(res);
    gPlacaDuty[playerid] = 0;
}

PlacaLokacija(i, &Float:x, &Float:y, &Float:z, &Float:a)
{
    if (i == 1)
    {
        x = gToranj[0];
        y = gToranj[1];
        z = gToranj[2];
        return 1;
    }
    if (!gImaDepo) return 0;
    x = gDepo[0];
    y = gDepo[1];
    z = gDepo[2];
    a = gDepo[3];
    return 1;
}

PosaoIgraca(playerid)
{
    return gPosao[playerid];
}

PosaoPostavi(playerid, posao)
{
    gPosao[playerid] = posao;
    DostavaKraj(playerid);
    AtcKraj(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET job = %d WHERE id = %d", posao, gAccountId[playerid]));
}

PlacaTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && NaDuznosti(i) && gPlacaDuty[i] < 3600) gPlacaDuty[i]++;
}

static PlacaImovina(racun)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT (SELECT COALESCE(SUM(price), 0) FROM houses WHERE owner = %d) + (SELECT COALESCE(SUM(price), 0) FROM garages WHERE owner = %d) + (SELECT COALESCE(SUM(price), 0) FROM player_vehicles WHERE owner = %d)", racun, racun, racun);
    new ukupno = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    return ukupno / PLACA_IMOVINA;
}

Placa(playerid)
{
    new racun = gAccountId[playerid], tip = gOrg[playerid] ? GetOrgTip(gOrg[playerid]) : -1, bruto = 0, zarada;
    switch (tip)
    {
        case ORG_LSPD, ORG_MEDICINSKA, ORG_VATROGASCI: bruto = PLACA_DRZAVNA * gPlacaDuty[playerid] / 3600;
        case ORG_TAKSI, ORG_MEHANICAR: bruto = PLACA_PRIVATNA * gPlacaDuty[playerid] / 3600;
    }
    gPlacaDuty[playerid] = 0;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT payday_earn FROM accounts WHERE id = %d", racun);
    zarada = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET payday_earn = 0 WHERE id = %d", racun));
    bruto += zarada + RestoranPlaca(playerid);
    if (!bruto && GetPlayerScore(playerid) <= PLACA_POMOC_LEVEL) bruto = PLACA_POMOC;
    bruto = BlagajnaIsplati(bruto);
    new porez = bruto * PLACA_POREZ / 100, neto = bruto - porez, imovina = PlacaImovina(racun), poruka[144];
    BlagajnaDodaj(porez);
    if (neto && !BankaUplati(racun, neto, "Placa") && !DajNovac(playerid, neto))
    {
        BlagajnaDodaj(neto);
        neto = 0;
    }
    if (imovina)
    {
        if (BankaUplati(racun, -imovina, "Porez na imovinu") || DajNovac(playerid, -imovina)) BlagajnaDodaj(imovina);
        else DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO fines (account, amount, reason, officer, created) VALUES (%d, %d, 'Porez na imovinu', 'Opstina', %d)", racun, imovina, gettime()));
    }
    format(poruka, sizeof poruka, T(playerid, "Placa: $%d (porez $%d). Porez na imovinu: $%d."), neto, porez, imovina);
    MobitelPrimiPoruku(gAccountId[playerid], MOBITEL_BANKA_BROJ, poruka);
}

static DostavaCilj(playerid)
{
    new Float:x, Float:y, Float:z, Float:a, Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    for (new pokusaj = 0; pokusaj < 40; pokusaj++)
    {
        new kat = 1 + random(10), n = 0;
        while (LokacijaUzmi(kat, n, x, y, z, a) != -1) n++;
        if (!n || LokacijaUzmi(kat, random(n), x, y, z, a) <= 0 || VectorSize(x - px, y - py, 0.0) < DOSTAVA_MIN_M) continue;
        gDostavaOd[playerid][0] = px;
        gDostavaOd[playerid][1] = py;
        gDostavaCilj[playerid] = true;
        SetPlayerCheckpoint(playerid, x, y, z, 6.0);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Dostavite robu na oznaceno mjesto (%s)."), CestaZona(x, y));
    }
    gDostavaCilj[playerid] = false;
    return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trenutno nema narudzbi."));
}

static DostavaKraj(playerid)
{
    if (gDostavaVozilo[playerid]) DestroyVehicle(gDostavaVozilo[playerid]);
    gDostavaVozilo[playerid] = 0;
    if (gDostavaCilj[playerid]) DisablePlayerCheckpoint(playerid);
    gDostavaCilj[playerid] = false;
}

AtcZarada(playerid)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET payday_earn = payday_earn + %d WHERE id = %d", ATC_ODOBRENJE, gAccountId[playerid]));
}

DostavaDisconnect(playerid)
{
    AtcKraj(playerid);
    DostavaKraj(playerid);
    gHrana[playerid] = 0;
}

HranaAktivna(playerid)
{
    return gHrana[playerid] ? 1 : 0;
}

NaPoslu(playerid)
{
    return gHrana[playerid] || gDostavaVozilo[playerid] || AtcNaSmjeni(playerid) ? 1 : 0;
}

static HranaOtkazi(playerid)
{
    gHrana[playerid] = 0;
    DisablePlayerCheckpoint(playerid);
}

HranaNarudzba(playerid)
{
    if (gHrana[playerid])
    {
        new bool:preuzeto = gHrana[playerid] == 2;
        HranaOtkazi(playerid);
        if (!preuzeto) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Otkazali ste dostavu hrane."));
        if (!BankaUplati(gAccountId[playerid], -HRANA_CIJENA, "Narudzba hrane")) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO fines (account, amount, reason, officer, created) VALUES (%d, %d, 'Neisporucena narudzba', 'Opstina', %d)", gAccountId[playerid], HRANA_CIJENA, gettime()));
        else BlagajnaDodaj(HRANA_CIJENA);
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Otkazali ste narudzbu. Hrana od $%d naplacena je s vaseg racuna."), HRANA_CIJENA);
    }
    if (gDostavaVozilo[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Prvo zavrsite smjenu dostave robe."));
    if (!BankaUplati(gAccountId[playerid], 0)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Za dostavu hrane potreban vam je bankovni racun."));
    new Float:x, Float:y, Float:z, r = 0, Float:najblize = 1.0e9;
    GetPlayerPos(playerid, x, y, z);
    for (new k = 0; k < sizeof gRestoran; k++)
    {
        new Float:d = VectorSize(gRestoran[k][0] - x, gRestoran[k][1] - y, 0.0);
        if (d < najblize) najblize = d, r = k;
    }
    gHrana[playerid] = 1;
    gHranaStand[playerid] = 0;
    for (new k = 0; k < 3; k++) gHranaCilj[playerid][k] = gRestoran[r][k];
    for (new s = 0, sMax = DostavaStandBroj(); s < sMax; s++)
    {
        new Float:sx, Float:sy, Float:sz;
        if (!DostavaStand(s, sx, sy, sz)) continue;
        new Float:d = VectorSize(sx - x, sy - y, 0.0);
        if (d >= najblize) continue;
        najblize = d;
        gHranaStand[playerid] = s + 1;
        gHranaCilj[playerid][0] = sx;
        gHranaCilj[playerid][1] = sy;
        gHranaCilj[playerid][2] = sz;
    }
    SetPlayerCheckpoint(playerid, gHranaCilj[playerid][0], gHranaCilj[playerid][1], gHranaCilj[playerid][2], HRANA_DOMET);
    if (gHranaStand[playerid]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prihvatili ste narudzbu. Preuzmite hranu na standu (%s)."), CestaZona(gHranaCilj[playerid][0], gHranaCilj[playerid][1]));
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prihvatili ste narudzbu. Preuzmite hranu u restoranu (%s)."), CestaZona(gRestoran[r][0], gRestoran[r][1]));
}

static HranaKuca(playerid)
{
    new Float:x, Float:y, Float:z, Float:a, n = 0;
    while (LokacijaUzmi(11, n, x, y, z, a) != -1) n++;
    for (new pokusaj = 0; n && pokusaj < 60; pokusaj++)
    {
        if (LokacijaUzmi(11, random(n), x, y, z, a) <= 0 || VectorSize(x - gHranaCilj[playerid][0], y - gHranaCilj[playerid][1], 0.0) < HRANA_MIN_M) continue;
        gHranaOd[playerid][0] = gHranaCilj[playerid][0];
        gHranaOd[playerid][1] = gHranaCilj[playerid][1];
        gHranaCilj[playerid][0] = x;
        gHranaCilj[playerid][1] = y;
        gHranaCilj[playerid][2] = z;
        gHrana[playerid] = 2;
        SetPlayerCheckpoint(playerid, x, y, z, HRANA_DOMET);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Preuzeli ste hranu. Dostavite je na adresu (%s)."), CestaZona(x, y));
    }
    HranaOtkazi(playerid);
    return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trenutno nema narudzbi."));
}

static HranaCheckpoint(playerid)
{
    if (!gHrana[playerid] || !IsPlayerInRangeOfPoint(playerid, HRANA_DOMET + 2.0, gHranaCilj[playerid][0], gHranaCilj[playerid][1], gHranaCilj[playerid][2])) return 0;
    DisablePlayerCheckpoint(playerid);
    if (gHrana[playerid] == 1)
    {
        if (gHranaStand[playerid] && !DostavaStandPreuzmi(gHranaStand[playerid] - 1))
        {
            HranaOtkazi(playerid);
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Stand je ostao bez robe, narudzba je otkazana."));
            return 1;
        }
        return HranaKuca(playerid), 1;
    }
    gHrana[playerid] = 0;
    new iznos = HRANA_BAZA + floatround(VectorSize(gHranaCilj[playerid][0] - gHranaOd[playerid][0], gHranaCilj[playerid][1] - gHranaOd[playerid][1], 0.0) * HRANA_PO_KM / 1000.0);
    new poruka[144];
    iznos = BlagajnaIsplati(iznos);
    if (!BankaUplati(gAccountId[playerid], iznos, "Dostava hrane")) BlagajnaDodaj(iznos), iznos = 0;
    format(poruka, sizeof poruka, T(playerid, "Uplata za dostavu hrane: $%d."), iznos);
    MobitelPrimiPoruku(gAccountId[playerid], MOBITEL_BANKA_BROJ, poruka);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Hrana je dostavljena. $%d je uplaceno na vas racun."), iznos), 1;
}

DostavaCheckpoint(playerid)
{
    if (HranaCheckpoint(playerid)) return 1;
    if (!gDostavaCilj[playerid]) return 0;
    if (!gDostavaVozilo[playerid] || GetPlayerVehicleID(playerid) != gDostavaVozilo[playerid] || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Robu morate dovesti u dostavnom kamionu."));
        return 1;
    }
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new iznos = DOSTAVA_BAZA + floatround(VectorSize(x - gDostavaOd[playerid][0], y - gDostavaOd[playerid][1], 0.0) * DOSTAVA_PO_KM / 1000.0);
    DisablePlayerCheckpoint(playerid);
    gDostavaCilj[playerid] = false;
    iznos = BlagajnaIsplati(iznos);
    if (!DajNovac(playerid, iznos)) BlagajnaDodaj(iznos), iznos = 0;
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Dostava zavrsena. Dobili ste $%d na ruke."), iznos);
    DostavaCilj(playerid);
    return 1;
}

CMD:posao(playerid, params[])
{
    if (!gUlogiran[playerid] || RestoranPosao(playerid) || RibarPosao(playerid) || SkladistePosao(playerid) || PraonicaPosao(playerid)) return 1;
    if (gPosao[playerid] == POSAO_KONTROLOR)
    {
        if (AtcNaSmjeni(playerid))
        {
            AtcKraj(playerid);
            return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zavrsili ste smjenu."));
        }
        if (!IsPlayerInRangeOfPoint(playerid, ATC_DOMET, gToranj[0], gToranj[1], gToranj[2])) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Smjenu zapocinjete u kontrolnom tornju aerodroma."));
        if (IsPlayerInAnyVehicle(playerid) || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Smjenu zapocinjete u kontrolnom tornju aerodroma."));
        if (!AtcPocni(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Svi kontrolni tornjevi su trenutno zauzeti."));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Smjena u tornju je pocela. Zahtjeve odobravate s /toranj, kraj smjene s /posao."));
    }
    if (gPosao[playerid] != POSAO_DOSTAVLJAC) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate posao. Zaposlite se u opstini."));
    if (gDostavaVozilo[playerid])
    {
        DostavaKraj(playerid);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zavrsili ste smjenu."));
    }
    if (gHrana[playerid]) HranaOtkazi(playerid);
    if (!gImaDepo || !IsPlayerInRangeOfPoint(playerid, DOSTAVA_DEPO_DOMET, gDepo[0], gDepo[1], gDepo[2]) || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Smjenu zapocinjete u depou (oznacen na karti)."));
    new v = CreateVehicle(DOSTAVA_VOZILO, gDepo[0], gDepo[1], gDepo[2] + 1.0, gDepo[3], 1, 1, -1);
    if (v == INVALID_VEHICLE_ID) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo trenutno nije dostupno."));
    gDostavaVozilo[playerid] = v;
    PutPlayerInVehicle(playerid, v, 0);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Smjena je pocela. /posao za kraj smjene."));
    DostavaCilj(playerid);
    return 1;
}

CMD:job(playerid, params[]) return cmd_posao(playerid, params);

CMD:depo(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_MANAGE)) return 1;
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Depo se postavlja samo izvana."));
    GetPlayerPos(playerid, gDepo[0], gDepo[1], gDepo[2]);
    GetPlayerFacingAngle(playerid, gDepo[3]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO job_depot (id, x, y, z, a) VALUES (1, %f, %f, %f, %f)", gDepo[0], gDepo[1], gDepo[2], gDepo[3]));
    DepoStvori();
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Depo je postavljen."));
}

PlacaPoslovi(playerid)
{
    return PrikaziDialog(playerid, DLG_OPSTINA_POSAO, DIALOG_STYLE_LIST, T(playerid, "Zaposlenje"), T(playerid, "Dostavljac robe\nKontrolor leta\nDaj otkaz"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:opstina_posao(playerid, response, listitem)
{
    if (!response) return ShowUprava(playerid);
    RibarKraj(playerid);
    PosaoPostavi(playerid, listitem == 0 ? POSAO_DOSTAVLJAC : listitem == 1 ? POSAO_KONTROLOR : POSAO_NEMA);
    if (gPosao[playerid] == POSAO_DOSTAVLJAC) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zaposleni ste kao dostavljac. Smjenu zapocnite s /posao u depou."));
    if (gPosao[playerid] == POSAO_KONTROLOR) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zaposleni ste kao kontrolor leta. Smjenu zapocnite s /posao u tornju aerodroma."));
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Dali ste otkaz."));
}
