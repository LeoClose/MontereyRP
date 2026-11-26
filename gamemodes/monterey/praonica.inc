#define PR_MJESTA           4
#define PR_DOMET            2.5
#define PR_SKIN             50
#define PR_BOJA             0x1ABC9CFF
#define PR_TIK_MS           250
#define PR_KORAK            8
#define PR_STRANA_DOMET     1.8
#define PR_STRANA_RAZMAK    0.7
#define PR_STRANA_CIJENA    10
#define PR_NAPOJNICA        16
#define PR_DOLAZAK_RAZMAK   15
#define PR_DOLAZI_S         150
#define PR_CEKA_S           300
#define PR_ODLAZI_S         40
#define PR_OZNAKA_MODEL     1318
#define PR_MUNICIJA         5000
#define PR_SMJENA_DOMET     150.0

enum { PRS_NEMA, PRS_DOLAZI, PRS_CEKA, PRS_ODLAZI }

static const Float:gPraonicaZadano[3] = { 2454.6, -1461.4, 24.0 };
static const gPraonicaModeli[] = { 400, 401, 404, 405, 410, 412, 419, 421, 426, 436, 445, 466, 467, 474, 479, 489, 491, 492, 496, 507, 516, 517, 518, 526, 527, 529, 540, 542, 546, 547, 549, 550, 551, 560, 562, 565, 566, 567, 575, 576, 579, 580, 585, 587, 589, 600, 602, 603 };
static const gPraonicaVozaci[] = { 7, 12, 19, 20, 40, 41, 56, 57, 59, 60, 69, 76, 98, 141, 147, 150, 170, 184, 185, 187, 188, 217, 223, 240, 250 };

enum E_PR_MJESTO { bool:pmIma, pmDb, Float:pmX, Float:pmY, Float:pmZ, Float:pmA, pmStanje, pmVozilo, pmLik, pmVrijeme, pmStrana[4], pmPrao[4], STREAMER_TAG_OBJECT:pmOznaka[4] }
static gPraonicaMjesto[PR_MJESTA][E_PR_MJESTO];
static Float:gPraonicaPosao[3], gPraonicaIkona, STREAMER_TAG_3D_TEXT_LABEL:gPraonicaTekst, bool:gPraonicaImaPosao, gPraonicaSljedeci;
static bool:gPraonicaSmjena[MAX_PLAYERS], gPraonicaTimer[MAX_PLAYERS], gPraonicaApPopis[MAX_PLAYERS][PR_MJESTA];

InitPraonica()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_carwash (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS carwash_bays (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    for (new k = 0; k < 3; k++) gPraonicaPosao[k] = gPraonicaZadano[k];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z FROM job_carwash WHERE id = 1");
    if (DB_GetRowCount(res)) for (new k = 0; k < 3; k++) gPraonicaPosao[k] = DB_GetFieldFloat(res, k);
    DB_FreeResultSet(res);
    PraonicaPosaoStvori();
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM carwash_bays ORDER BY id LIMIT %d", PR_MJESTA);
    for (new m = 0, n = DB_GetRowCount(res); m < n; m++, DB_SelectNextRow(res))
        PraonicaMjestoStvori(m, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
    DB_FreeResultSet(res);
}

static PraonicaPosaoStvori()
{
    if (gPraonicaImaPosao)
    {
        MapaIkonaMakni(gPraonicaIkona);
        DestroyDynamic3DTextLabel(gPraonicaTekst);
    }
    gPraonicaImaPosao = true;
    gPraonicaIkona = MapaIkona(gPraonicaPosao[0], gPraonicaPosao[1], gPraonicaPosao[2], 63);
    gPraonicaTekst = CreateDynamic3DTextLabel("Autopraonica\n/posao", PR_BOJA, gPraonicaPosao[0], gPraonicaPosao[1], gPraonicaPosao[2] + 0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gPraonicaTekst, PR_DOMET);
}

static PraonicaMjestoStvori(m, db, Float:x, Float:y, Float:z, Float:a)
{
    gPraonicaMjesto[m][pmIma] = true;
    gPraonicaMjesto[m][pmDb] = db;
    gPraonicaMjesto[m][pmX] = x;
    gPraonicaMjesto[m][pmY] = y;
    gPraonicaMjesto[m][pmZ] = z;
    gPraonicaMjesto[m][pmA] = a;
    gPraonicaMjesto[m][pmStanje] = PRS_NEMA;
}

PraonicaSmjena(playerid)
{
    return gPraonicaSmjena[playerid] ? 1 : 0;
}

PraonicaLokacija(&Float:x, &Float:y, &Float:z)
{
    x = gPraonicaPosao[0];
    y = gPraonicaPosao[1];
    z = gPraonicaPosao[2];
    return 1;
}

static bool:PraonicaKodPosla(playerid, Float:r = PR_DOMET)
{
    return !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, r, gPraonicaPosao[0], gPraonicaPosao[1], gPraonicaPosao[2]);
}

static PraonicaStrana(v, s, &Float:ox, &Float:oy)
{
    new Float:sx, Float:sy, Float:sz;
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    ox = s == 2 ? -(sx / 2.0 + PR_STRANA_RAZMAK) : s == 3 ? sx / 2.0 + PR_STRANA_RAZMAK : 0.0;
    oy = s == 0 ? sy / 2.0 + PR_STRANA_RAZMAK : s == 1 ? -(sy / 2.0 + PR_STRANA_RAZMAK) : 0.0;
}

static PraonicaOznakeMakni(m)
{
    for (new s = 0; s < 4; s++)
    {
        if (_:gPraonicaMjesto[m][pmOznaka][s]) DestroyDynamicObject(gPraonicaMjesto[m][pmOznaka][s]);
        gPraonicaMjesto[m][pmOznaka][s] = STREAMER_TAG_OBJECT:0;
    }
}

static PraonicaDolazakStvori(m)
{
    new Float:x, Float:y, Float:z, bool:nasao = false;
    for (new k = 0; k < 10 && !nasao; k++)
    {
        new Float:kut = float(random(360));
        Lik_Cesta(gPraonicaMjesto[m][pmX] + 250.0 * floatcos(kut, degrees), gPraonicaMjesto[m][pmY] + 250.0 * floatsin(kut, degrees), gPraonicaMjesto[m][pmZ], x, y, z);
        nasao = Lik_Skriveno(x, y, z);
    }
    if (!nasao) return;
    new Float:a = atan2(gPraonicaMjesto[m][pmX] - x, gPraonicaMjesto[m][pmY] - y);
    new v = CreateVehicle(gPraonicaModeli[random(sizeof gPraonicaModeli)], x, y, z + 0.5, -a, -1, -1, -1);
    if (v == INVALID_VEHICLE_ID) return;
    new lik = Lik_Stvori(gPraonicaVozaci[random(sizeof gPraonicaVozaci)], x, y, z, -a);
    if (lik == INVALID_LIK)
    {
        DestroyVehicle(v);
        return;
    }
    Lik_Neranjiv(lik, true);
    VoziloSet(v, VP_MOTOR, 1);
    VoziloSet(v, VP_VRATA, 1);
    Lik_UVozilo(lik, v, 0);
    gPraonicaMjesto[m][pmStanje] = PRS_DOLAZI;
    gPraonicaMjesto[m][pmVozilo] = v;
    gPraonicaMjesto[m][pmLik] = lik;
    gPraonicaMjesto[m][pmVrijeme] = gettime();
    SetTimerEx("PraonicaDolazakKreni", 1000, false, "ii", m, v);
}

forward PraonicaDolazakKreni(m, v);
public PraonicaDolazakKreni(m, v)
{
    if (gPraonicaMjesto[m][pmStanje] != PRS_DOLAZI || gPraonicaMjesto[m][pmVozilo] != v) return;
    if (!Lik_Vozi(gPraonicaMjesto[m][pmLik], gPraonicaMjesto[m][pmX], gPraonicaMjesto[m][pmY], gPraonicaMjesto[m][pmZ], 8.0)) PraonicaParkiraj(m);
}

static PraonicaParkiraj(m)
{
    SetVehiclePos(gPraonicaMjesto[m][pmVozilo], gPraonicaMjesto[m][pmX], gPraonicaMjesto[m][pmY], gPraonicaMjesto[m][pmZ] + 0.5);
    SetVehicleZAngle(gPraonicaMjesto[m][pmVozilo], gPraonicaMjesto[m][pmA]);
    PraonicaCeka(m);
}

static PraonicaCeka(m)
{
    new v = gPraonicaMjesto[m][pmVozilo], Float:ox, Float:oy;
    gPraonicaMjesto[m][pmStanje] = PRS_CEKA;
    gPraonicaMjesto[m][pmVrijeme] = gettime();
    VoziloSet(v, VP_MOTOR, 0);
    PraonicaOznakeMakni(m);
    for (new s = 0; s < 4; s++)
    {
        gPraonicaMjesto[m][pmStrana][s] = 0;
        gPraonicaMjesto[m][pmPrao][s] = INVALID_PLAYER_ID;
        PraonicaStrana(v, s, ox, oy);
        gPraonicaMjesto[m][pmOznaka][s] = CreateDynamicObject(PR_OZNAKA_MODEL, 0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0, 0);
        AttachDynamicObjectToVehicle(gPraonicaMjesto[m][pmOznaka][s], v, ox, oy, 1.0, 0.0, 0.0, 0.0);
    }
}

PraonicaStigao(lik)
{
    for (new m = 0; m < PR_MJESTA; m++)
    {
        if (!gPraonicaMjesto[m][pmIma] || gPraonicaMjesto[m][pmStanje] == PRS_NEMA || gPraonicaMjesto[m][pmLik] != lik) continue;
        if (gPraonicaMjesto[m][pmStanje] == PRS_DOLAZI) PraonicaCeka(m);
        return 1;
    }
    return 0;
}

static PraonicaOdlazi(m)
{
    PraonicaOznakeMakni(m);
    gPraonicaMjesto[m][pmStanje] = PRS_ODLAZI;
    gPraonicaMjesto[m][pmVrijeme] = gettime();
    VoziloSet(gPraonicaMjesto[m][pmVozilo], VP_MOTOR, 1);
    Lik_Kruzi(gPraonicaMjesto[m][pmLik], 11.0);
}

static PraonicaMakni(m)
{
    if (gPraonicaMjesto[m][pmStanje] == PRS_NEMA) return;
    PraonicaOznakeMakni(m);
    Lik_Ukloni(gPraonicaMjesto[m][pmLik]);
    DestroyVehicle(gPraonicaMjesto[m][pmVozilo]);
    gPraonicaMjesto[m][pmStanje] = PRS_NEMA;
}

static PraonicaGotovo(m, zadnji)
{
    new napojnica = random(PR_NAPOJNICA + 1), poruka[128];
    for (new s = 0; s < 4; s++)
    {
        new i = gPraonicaMjesto[m][pmPrao][s];
        if (i == INVALID_PLAYER_ID || !IsPlayerConnected(i)) continue;
        new iznos = 0;
        for (new t = s; t < 4; t++)
        {
            if (gPraonicaMjesto[m][pmPrao][t] != i) continue;
            iznos += PR_STRANA_CIJENA;
            gPraonicaMjesto[m][pmPrao][t] = INVALID_PLAYER_ID;
        }
        if (i == zadnji) iznos += napojnica;
        if (!DajNovac(i, iznos)) continue;
        format(poruka, sizeof poruka, T(i, "Auto je opran. Zaradili ste $%d."), iznos);
        IgraObavijest(i, T(i, "Autopraonica"), PR_BOJA, poruka);
    }
    PraonicaOdlazi(m);
}

forward PraonicaPranje(playerid);
public PraonicaPranje(playerid)
{
    if (!gPraonicaSmjena[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPlayerWeapon(playerid) != WEAPON_FIREEXTINGUISHER) return;
    new KEY:tipke, gd, lr;
    GetPlayerKeys(playerid, tipke, gd, lr);
    if (!(tipke & KEY_FIRE)) return;
    new Float:px, Float:py, Float:pz, Float:pa;
    GetPlayerPos(playerid, px, py, pz);
    GetPlayerFacingAngle(playerid, pa);
    for (new m = 0; m < PR_MJESTA; m++)
    {
        if (!gPraonicaMjesto[m][pmIma] || gPraonicaMjesto[m][pmStanje] != PRS_CEKA) continue;
        new v = gPraonicaMjesto[m][pmVozilo], Float:vx, Float:vy, Float:vz, Float:va;
        GetVehiclePos(v, vx, vy, vz);
        if (VectorSize(px - vx, py - vy, pz - vz) > 8.0) continue;
        GetVehicleZAngle(v, va);
        new Float:c = floatcos(va, degrees), Float:sn = floatsin(va, degrees), Float:dx = vx - px, Float:dy = vy - py, Float:d = VectorSize(dx, dy, 0.0);
        if (d < 0.1 || (-floatsin(pa, degrees) * dx + floatcos(pa, degrees) * dy) / d < 0.5) continue;
        for (new s = 0; s < 4; s++)
        {
            if (gPraonicaMjesto[m][pmStrana][s] >= 100) continue;
            new Float:ox, Float:oy;
            PraonicaStrana(v, s, ox, oy);
            if (VectorSize(px - (vx + ox * c - oy * sn), py - (vy + ox * sn + oy * c), 0.0) > PR_STRANA_DOMET) continue;
            gPraonicaMjesto[m][pmVrijeme] = gettime();
            gPraonicaMjesto[m][pmPrao][s] = playerid;
            gPraonicaMjesto[m][pmStrana][s] = min(100, gPraonicaMjesto[m][pmStrana][s] + PR_KORAK);
            new tekst[48];
            format(tekst, sizeof tekst, T(playerid, "~b~Pranje: ~w~%d%%"), gPraonicaMjesto[m][pmStrana][s]);
            GameTextForPlayer(playerid, tekst, 1000, 3);
            if (gPraonicaMjesto[m][pmStrana][s] < 100) return;
            if (_:gPraonicaMjesto[m][pmOznaka][s]) DestroyDynamicObject(gPraonicaMjesto[m][pmOznaka][s]);
            gPraonicaMjesto[m][pmOznaka][s] = STREAMER_TAG_OBJECT:0;
            PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
            for (new t = 0; t < 4; t++) if (gPraonicaMjesto[m][pmStrana][t] < 100) return;
            PraonicaGotovo(m, playerid);
            return;
        }
    }
}

static PraonicaSmjenaPocni(playerid)
{
    gPraonicaSmjena[playerid] = true;
    SetPlayerSkin(playerid, PR_SKIN);
    GivePlayerWeapon(playerid, WEAPON_FIREEXTINGUISHER, PR_MUNICIJA);
    if (gPraonicaTimer[playerid]) KillTimer(gPraonicaTimer[playerid]);
    gPraonicaTimer[playerid] = SetTimerEx("PraonicaPranje", PR_TIK_MS, true, "i", playerid);
    gPraonicaSljedeci = 0;
    ObavijestInfo(playerid, T(playerid, "Autopraonica"), T(playerid, "Smjena je pocela. Perite aute koji dolaze: stanite uz oznacenu stranu auta i prskajte aparatom."));
    Savjet(playerid, T(playerid, "~y~~k~~PED_FIREWEAPON~~w~ - prskaj aparatom~n~Stanite uz strelicu kraj auta~n~~y~/posao~w~ - zavrsi smjenu u praonici"), 8000);
    return 1;
}

PraonicaKraj(playerid)
{
    if (!gPraonicaSmjena[playerid]) return;
    gPraonicaSmjena[playerid] = false;
    if (gPraonicaTimer[playerid]) KillTimer(gPraonicaTimer[playerid]);
    gPraonicaTimer[playerid] = 0;
    if (!IsPlayerConnected(playerid)) return;
    if (!SluzbenoOruzje(playerid, WEAPON_FIREEXTINGUISHER)) RemovePlayerWeapon(playerid, WEAPON_FIREEXTINGUISHER);
    if (GetPlayerSkin(playerid) == PR_SKIN) SetPlayerSkin(playerid, OdjecaSkinIgraca(playerid));
}

static PraonicaIzbornik(playerid)
{
    if (PosaoIgraca(playerid) != POSAO_PRAONICA) return UpitDaNe(playerid, UPIT_PRAONICA, 0, 0, T(playerid, "Autopraonica"), T(playerid, "Zelite li se zaposliti u autopraonici?"));
    if (!gPraonicaSmjena[playerid]) return UpitDaNe(playerid, UPIT_PRAONICA, 0, 1, T(playerid, "Autopraonica"), T(playerid, "Zelite li zapoceti smjenu?"));
    return UpitDaNe(playerid, UPIT_PRAONICA, 0, 2, T(playerid, "Autopraonica"), T(playerid, "Zelite li zavrsiti smjenu?"));
}

PraonicaUpit(playerid, opcija, bool:da)
{
    if (!da || !PraonicaKodPosla(playerid, PR_DOMET + 2.0)) return 1;
    if (opcija == 0 && PosaoIgraca(playerid) != POSAO_PRAONICA)
    {
        RibarKraj(playerid);
        PosaoPostavi(playerid, POSAO_PRAONICA);
        return ObavijestInfo(playerid, T(playerid, "Autopraonica"), T(playerid, "Zaposleni ste u autopraonici. Pritisnite ALT ovdje (ili /posao) za pocetak smjene."));
    }
    if (PosaoIgraca(playerid) != POSAO_PRAONICA) return 1;
    if (opcija == 1 && !gPraonicaSmjena[playerid]) return PraonicaSmjenaPocni(playerid);
    if (opcija == 2 && gPraonicaSmjena[playerid])
    {
        PraonicaKraj(playerid);
        return ObavijestInfo(playerid, T(playerid, "Autopraonica"), T(playerid, "Smjena je zavrsena."));
    }
    return 1;
}

PraonicaPosao(playerid)
{
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && PraonicaKodPosla(playerid)) return PraonicaIzbornik(playerid), 1;
    if (PosaoIgraca(playerid) != POSAO_PRAONICA) return 0;
    if (!gPraonicaSmjena[playerid]) return ObavijestGreska(playerid, T(playerid, "Smjenu zapocinjete u autopraonici (oznaceno na karti).")), 1;
    return ObavijestGreska(playerid, T(playerid, "Smjenu zavrsite kod ulaza u autopraonicu.")), 1;
}

PraonicaAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || !PraonicaKodPosla(playerid)) return 0;
    return PraonicaIzbornik(playerid), 1;
}

PraonicaTick()
{
    new sad = gettime(), radnika = 0;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gPraonicaSmjena[i]) continue;
        if (PosaoIgraca(i) != POSAO_PRAONICA || IsRanjen(i) || !PraonicaKodPosla(i, PR_SMJENA_DOMET))
        {
            PraonicaKraj(i);
            ObavijestGreska(i, T(i, "Smjena u autopraonici je zavrsena jer ste otisli."));
            continue;
        }
        radnika++;
    }
    new slobodno = -1, dolazi = 0, cekaju = 0;
    for (new m = 0; m < PR_MJESTA; m++)
    {
        if (!gPraonicaMjesto[m][pmIma]) continue;
        switch (gPraonicaMjesto[m][pmStanje])
        {
            case PRS_NEMA: if (slobodno == -1 || !random(2)) slobodno = m;
            case PRS_DOLAZI:
            {
                dolazi++;
                if (sad - gPraonicaMjesto[m][pmVrijeme] > PR_DOLAZI_S) PraonicaParkiraj(m);
            }
            case PRS_CEKA:
            {
                cekaju++;
                if (sad - gPraonicaMjesto[m][pmVrijeme] <= PR_CEKA_S) continue;
                for (new s = 0; s < 4; s++)
                {
                    new i = gPraonicaMjesto[m][pmPrao][s];
                    if (i != INVALID_PLAYER_ID && IsPlayerConnected(i)) IgraObavijest(i, T(i, "Autopraonica"), OBAVIJEST_GRESKA, T(i, "Musterija se umorila od cekanja i otisla."));
                }
                PraonicaOdlazi(m);
            }
            case PRS_ODLAZI: if (sad - gPraonicaMjesto[m][pmVrijeme] > PR_ODLAZI_S) PraonicaMakni(m);
        }
    }
    if (!radnika || slobodno == -1 || dolazi || cekaju >= radnika || sad < gPraonicaSljedeci) return;
    gPraonicaSljedeci = sad + PR_DOLAZAK_RAZMAK;
    PraonicaDolazakStvori(slobodno);
}

ShowPraonicaAdmin(playerid)
{
    new list[1024], row[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto zaposlenja autopraonice na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto za pranje auta na mojoj poziciji"));
    for (new m = 0; m < PR_MJESTA; m++)
    {
        if (!gPraonicaMjesto[m][pmIma]) continue;
        format(row, sizeof row, T(playerid, "Mjesto za pranje [%d] %s - obrisi"), gPraonicaMjesto[m][pmDb], CestaZona(gPraonicaMjesto[m][pmX], gPraonicaMjesto[m][pmY]));
        DodajStupac(list, len, row);
        gPraonicaApPopis[playerid][n++] = m;
    }
    return PrikaziDialog(playerid, DLG_APANEL_PRAONICA, DIALOG_STYLE_LIST, T(playerid, "Autopraonice"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_praonica(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new Float:x, Float:y, Float:z, Float:a;
    if (listitem <= 1)
    {
        if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ovo se postavlja samo izvana.")), ShowPraonicaAdmin(playerid);
        new v = GetPlayerVehicleID(playerid);
        if (v) GetVehiclePos(v, x, y, z), GetVehicleZAngle(v, a);
        else GetPlayerPos(playerid, x, y, z), GetPlayerFacingAngle(playerid, a);
    }
    if (listitem == 0)
    {
        gPraonicaPosao[0] = x;
        gPraonicaPosao[1] = y;
        gPraonicaPosao[2] = z;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO job_carwash (id, x, y, z) VALUES (1, %f, %f, %f)", x, y, z));
        PraonicaPosaoStvori();
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto zaposlenja autopraonice postavljeno."));
        return ShowPraonicaAdmin(playerid);
    }
    if (listitem == 1)
    {
        new m = 0;
        while (m < PR_MJESTA && gPraonicaMjesto[m][pmIma]) m++;
        if (m == PR_MJESTA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowPraonicaAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO carwash_bays (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        PraonicaMjestoStvori(m, ZadnjiId(), x, y, z, a);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto za pranje postavljeno."));
        return ShowPraonicaAdmin(playerid);
    }
    new i = listitem - 2;
    if (i < 0 || i >= sizeof gPraonicaApPopis[]) return 1;
    new m = gPraonicaApPopis[playerid][i];
    if (!gPraonicaMjesto[m][pmIma]) return ShowPraonicaAdmin(playerid);
    PraonicaMakni(m);
    gPraonicaMjesto[m][pmIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM carwash_bays WHERE id = %d", gPraonicaMjesto[m][pmDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto za pranje obrisano."));
    return ShowPraonicaAdmin(playerid);
}
