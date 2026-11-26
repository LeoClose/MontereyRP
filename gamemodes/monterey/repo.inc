#define RP_KAMION_MODEL     525
#define RP_DOMET            2.5
#define RP_ISTOVAR_DOMET    6.0
#define RP_STRANA_DOMET     1.6
#define RP_STRANA_RAZMAK    0.6
#define RP_KUKA             3.2
#define RP_RUB              1.6
#define RP_PLOCNIK_MIN      4.0
#define RP_PLOCNIK_MAX      14.0
#define RP_KUKA_DOMET       1.8
#define RP_UDALJENOST_MIN   400.0
#define RP_UDALJENOST_MAX   1400.0
#define RP_POSTO            1
#define RP_ISPLATA_MIN      80
#define RP_ISPLATA_MAX      500
#define RP_CIJENA_ZADANO    20000
#define RP_DIJELOVA         8
#define RP_KAMION_HP        300.0
#define RP_BOJA             0x95A5A6FF
#define RP_OZNAKA_MODEL     1318
#define RP_TRAKA_MS         40
#define RP_TRAKA_ZELENO     14.0
#define RP_SAVJET_MS        3000
#define RP_PODRUCJE         90.0
#define RP_PODRUCJE_POMAK   60
#define RP_PODRUCJE_BOJA    0x95A5A666

enum { RPF_NEMA, RPF_TRAZI, RPF_VUCE, RPF_RASTAVLJA }

static const gRepoZabranjeni[] = {
    403, 406, 407, 408, 416, 417, 425, 427, 428, 430, 431, 432, 433, 435, 437, 441, 443, 444, 446, 447, 448, 449, 450, 452, 453, 454, 455, 456,
    460, 461, 462, 463, 464, 465, 468, 469, 471, 472, 473, 476, 481, 484, 486, 487, 488, 493, 497, 501, 509, 510, 511, 512, 513, 514, 515, 519,
    520, 521, 522, 523, 524, 525, 530, 531, 532, 537, 538, 539, 544, 548, 553, 556, 557, 563, 564, 569, 570, 571, 572, 573, 574, 577, 578, 581,
    583, 584, 586, 590, 591, 592, 593, 594, 595, 601, 606, 607, 608, 610, 611
};
static const gRepoRezerva[] = { 400, 401, 404, 405, 410, 412, 419, 421, 426, 436, 445, 466, 467, 474, 475, 479, 491, 492, 496, 507, 516, 517, 518, 526, 527, 529, 540, 542, 546, 547, 549, 550, 551, 560, 562, 565, 566, 567, 575, 576, 580, 585, 587, 589, 600, 602, 603 };

static Float:gRepoMjesto[4], Float:gRepoKamionPoz[4], Float:gRepoCilj[4], STREAMER_TAG_3D_TEXT_LABEL:gRepoTekst, bool:gRepoImaMjesto, bool:gRepoImaKamion, bool:gRepoImaCilj, bool:gRepoDozvoljen[212];
static gRepoFaza[MAX_PLAYERS], gRepoKamion[MAX_PLAYERS], gRepoAuto[MAX_PLAYERS], gRepoIsplata[MAX_PLAYERS], gRepoGotovo[MAX_PLAYERS], gRepoDijelovi[MAX_PLAYERS], gRepoSavjet[MAX_PLAYERS];
static bool:gRepoIgra[MAX_PLAYERS], gRepoIgraStrana[MAX_PLAYERS], gRepoIgraTimer[MAX_PLAYERS], Float:gRepoTraka[MAX_PLAYERS], Float:gRepoSmjer[MAX_PLAYERS], Float:gRepoZel[MAX_PLAYERS];
static STREAMER_TAG_OBJECT:gRepoOznaka[MAX_PLAYERS][4], STREAMER_TAG_AREA:gRepoZona[MAX_PLAYERS][4];
static bool:gRepoTrazi[MAX_PLAYERS], gRepoPodrucje[MAX_PLAYERS] = { -1, ... }, gRepoTrazeniModel[MAX_PLAYERS];

InitRepo()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_repo (id INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS repo_cars (account INTEGER PRIMARY KEY, model INTEGER NOT NULL, price INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, parts INTEGER NOT NULL DEFAULT 0, earned INTEGER NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM job_repo WHERE id IN (1, 2, 3)");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new id = DB_GetFieldInt(res, 0);
        for (new k = 0; k < 4; k++)
        {
            new Float:f = DB_GetFieldFloat(res, k + 1);
            if (id == 1) gRepoMjesto[k] = f;
            else if (id == 2) gRepoKamionPoz[k] = f;
            else gRepoCilj[k] = f;
        }
        if (id == 1) gRepoImaMjesto = true;
        else if (id == 2) gRepoImaKamion = true;
        else gRepoImaCilj = true;
    }
    DB_FreeResultSet(res);
    for (new m = 0; m < sizeof gRepoDozvoljen; m++) gRepoDozvoljen[m] = true;
    for (new k = 0; k < sizeof gRepoZabranjeni; k++) gRepoDozvoljen[gRepoZabranjeni[k] - 400] = false;
    for (new i = 0; i < MAX_PLAYERS; i++) gRepoKamion[i] = gRepoAuto[i] = INVALID_VEHICLE_ID;
    if (gRepoImaMjesto) RepoMjestoStvori();
}

static RepoMjestoStvori()
{
    if (IsValidDynamic3DTextLabel(gRepoTekst)) DestroyDynamic3DTextLabel(gRepoTekst);
    gRepoImaMjesto = true;
    gRepoTekst = CreateDynamic3DTextLabel("Repo\nALT", RP_BOJA, gRepoMjesto[0], gRepoMjesto[1], gRepoMjesto[2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gRepoTekst, RP_DOMET);
}

RepoVozilo(v)
{
    if (v == INVALID_VEHICLE_ID) return 0;
    for (new i = 0; i < MAX_PLAYERS; i++) if (gRepoFaza[i] && gRepoAuto[i] == v) return 1;
    return 0;
}

RepoLokacija(&Float:x, &Float:y, &Float:z)
{
    if (!gRepoImaMjesto) return 0;
    x = gRepoMjesto[0];
    y = gRepoMjesto[1];
    z = gRepoMjesto[2];
    return 1;
}

static bool:RepoNaMjestu(playerid, Float:r = RP_DOMET)
{
    return gRepoImaMjesto && !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, r, gRepoMjesto[0], gRepoMjesto[1], gRepoMjesto[2]);
}

static RepoStrana(v, s, &Float:ox, &Float:oy)
{
    new Float:sx, Float:sy, Float:sz;
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    ox = s == 2 ? -(sx / 2.0 + RP_STRANA_RAZMAK) : s == 3 ? sx / 2.0 + RP_STRANA_RAZMAK : 0.0;
    oy = s == 0 ? sy / 2.0 + RP_STRANA_RAZMAK : s == 1 ? -(sy / 2.0 + RP_STRANA_RAZMAK) : 0.0;
}

static RepoStranaMakni(playerid, s)
{
    if (_:gRepoOznaka[playerid][s]) DestroyDynamicObject(gRepoOznaka[playerid][s]);
    if (IsValidDynamicArea(gRepoZona[playerid][s])) DestroyDynamicArea(gRepoZona[playerid][s]);
    gRepoOznaka[playerid][s] = STREAMER_TAG_OBJECT:0;
    gRepoZona[playerid][s] = STREAMER_TAG_AREA:0;
}

static RepoOznakeMakni(playerid)
{
    for (new s = 0; s < 4; s++) RepoStranaMakni(playerid, s);
}

static RepoPodrucjeMakni(playerid)
{
    if (gRepoTrazi[playerid] && IsPlayerConnected(playerid)) IgraObavijestUkloni(playerid);
    gRepoTrazi[playerid] = false;
    if (gRepoPodrucje[playerid] != -1 && IsPlayerConnected(playerid)) PlayerGangZoneDestroy(playerid, gRepoPodrucje[playerid]);
    gRepoPodrucje[playerid] = -1;
}

static RepoTraziPanel(playerid)
{
    new poruka[160];
    format(poruka, sizeof poruka, T(playerid, "Trazite: %s~n~Auto je negdje u oznacenom podrucju na karti. Prikacite ga kamionom unatrag (~y~ALT~w~)."), gImenaVozila[gRepoTrazeniModel[playerid] - 400]);
    IgraObavijestPanel(playerid, T(playerid, "Repo"), RP_BOJA, poruka);
}

static RepoAutoMakni(playerid)
{
    RepoPodrucjeMakni(playerid);
    RepoOznakeMakni(playerid);
    if (gRepoAuto[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gRepoAuto[playerid]);
    gRepoAuto[playerid] = INVALID_VEHICLE_ID;
}

static bool:RepoUlica(&Float:x, &Float:y, &Float:z, &Float:a)
{
    new Float:px, Float:py, Float:pz, Float:rx, Float:ry, Float:rz;
    if (!Lik_TockaStaze(gRepoMjesto[0], gRepoMjesto[1], RP_UDALJENOST_MIN, RP_UDALJENOST_MAX, px, py, pz)) return false;
    new Float:d = Lik_Cesta(px, py, pz, rx, ry, rz);
    if (d < RP_PLOCNIK_MIN || d > RP_PLOCNIK_MAX) return false;
    new Float:ux = (rx - px) / d, Float:uy = (ry - py) / d, Float:o = 0.5;
    while (o < d && !Lik_NaCesti(px + ux * o, py + uy * o, pz)) o += 0.5;
    if (o >= d) o = d - 3.0;
    x = px + ux * (o + RP_RUB);
    y = py + uy * (o + RP_RUB);
    z = pz;
    new Float:smjer = random(2) ? 1.0 : -1.0;
    a = atan2(uy * smjer, ux * smjer);
    return true;
}

static RepoDioPostavi(v, s, dio)
{
    new VEHICLE_PANEL_STATUS:paneli, VEHICLE_DOOR_STATUS:vrata, VEHICLE_LIGHT_STATUS:svjetla, VEHICLE_TIRE_STATUS:gume;
    GetVehicleDamageStatus(v, paneli, vrata, svjetla, gume);
    switch (s * 2 + dio)
    {
        case 0: vrata = VEHICLE_DOOR_STATUS:((_:vrata & ~0xFF) | 0x04);
        case 1: paneli = VEHICLE_PANEL_STATUS:((_:paneli & ~0x00F00000) | 0x00300000), svjetla = VEHICLE_LIGHT_STATUS:(_:svjetla | 0x05);
        case 2: vrata = VEHICLE_DOOR_STATUS:((_:vrata & ~0xFF00) | 0x0400);
        case 3: paneli = VEHICLE_PANEL_STATUS:((_:paneli & ~0x0F000000) | 0x03000000), svjetla = VEHICLE_LIGHT_STATUS:(_:svjetla | 0x40);
        case 4: vrata = VEHICLE_DOOR_STATUS:((_:vrata & ~0xFF0000) | 0x040000);
        case 5: gume = VEHICLE_TIRE_STATUS:(_:gume | 0x0C);
        case 6: vrata = VEHICLE_DOOR_STATUS:((_:vrata & ~0xFF000000) | 0x04000000);
        case 7: gume = VEHICLE_TIRE_STATUS:(_:gume | 0x03);
    }
    UpdateVehicleDamageStatus(v, paneli, vrata, svjetla, gume);
}

static RepoNovoAuto(playerid)
{
    RepoAutoMakni(playerid);
    new model, cijena, Float:x, Float:y, Float:z, Float:a, dijelovi = 0, zaradjeno = 0;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT model, price, x, y, z, a, parts, earned FROM repo_cars WHERE account = %d", gAccountId[playerid]);
    new bool:spremljeno = DB_GetRowCount(res) > 0;
    if (spremljeno)
    {
        model = DB_GetFieldInt(res, 0);
        cijena = DB_GetFieldInt(res, 1);
        x = DB_GetFieldFloat(res, 2);
        y = DB_GetFieldFloat(res, 3);
        z = DB_GetFieldFloat(res, 4);
        a = DB_GetFieldFloat(res, 5);
        dijelovi = DB_GetFieldInt(res, 6);
        zaradjeno = DB_GetFieldInt(res, 7);
    }
    DB_FreeResultSet(res);
    if (!spremljeno)
    {
        cijena = RP_CIJENA_ZADANO;
        model = SalonNasumicnoVozilo(cijena, gRepoDozvoljen);
        if (!model) model = gRepoRezerva[random(sizeof gRepoRezerva)], cijena = RP_CIJENA_ZADANO;
        new bool:nasao = false, Float:hx, Float:hy, Float:hz;
        a = float(random(360));
        for (new k = 0; k < 60 && !nasao; k++) nasao = RepoUlica(x, y, z, a);
        if (!nasao) Lik_Cesta(gRepoMjesto[0] + RP_UDALJENOST_MIN, gRepoMjesto[1], gRepoMjesto[2], x, y, z);
        if (Kolizija_Zraka(x, y, z + 1.5, x, y, z - 5.0, hx, hy, hz)) z = hz;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO repo_cars (account, model, price, x, y, z, a, parts, earned) VALUES (%d, %d, %d, %f, %f, %f, %f, 0, 0)", gAccountId[playerid], model, cijena, x, y, z, a));
    }
    gRepoAuto[playerid] = CreateVehicle(model, x, y, z + 0.6, a, -1, -1, -1);
    if (gRepoAuto[playerid] == INVALID_VEHICLE_ID) return 0;
    GepekZonaMakni(gRepoAuto[playerid]);
    VoziloSet(gRepoAuto[playerid], VP_MOTOR, 0);
    VoziloSet(gRepoAuto[playerid], VP_VRATA, 1);
    gRepoIsplata[playerid] = clamp(cijena / 100 * RP_POSTO, RP_ISPLATA_MIN, RP_ISPLATA_MAX);
    gRepoGotovo[playerid] = zaradjeno;
    gRepoDijelovi[playerid] = dijelovi;
    for (new b = 0; b < RP_DIJELOVA; b++) if (dijelovi >> b & 1) RepoDioPostavi(gRepoAuto[playerid], b / 2, b % 2);
    gRepoFaza[playerid] = RPF_TRAZI;
    gRepoSavjet[playerid] = 0;
    DisablePlayerCheckpoint(playerid);
    new Float:kut = float(random(360)), Float:pomak = float(random(RP_PODRUCJE_POMAK)), Float:cx = x + pomak * floatcos(kut, degrees), Float:cy = y + pomak * floatsin(kut, degrees);
    gRepoPodrucje[playerid] = CreatePlayerGangZone(playerid, cx - RP_PODRUCJE, cy - RP_PODRUCJE, cx + RP_PODRUCJE, cy + RP_PODRUCJE);
    PlayerGangZoneShow(playerid, gRepoPodrucje[playerid], RP_PODRUCJE_BOJA);
    gRepoTrazeniModel[playerid] = model;
    gRepoTrazi[playerid] = true;
    RepoTraziPanel(playerid);
    return 1;
}

static RepoPocni(playerid)
{
    if (!gRepoImaKamion || !gRepoImaCilj) return ObavijestGreska(playerid, T(playerid, "Repo posao jos nije postavljen.")), 1;
    new v = CreateVehicle(RP_KAMION_MODEL, gRepoKamionPoz[0], gRepoKamionPoz[1], gRepoKamionPoz[2] + 0.5, gRepoKamionPoz[3], 0, 0, -1);
    if (v == INVALID_VEHICLE_ID) return ObavijestGreska(playerid, T(playerid, "Trenutno nema slobodnih kamiona.")), 1;
    gRepoKamion[playerid] = v;
    PutPlayerInVehicle(playerid, v, 0);
    VoziloSet(v, VP_MOTOR, 1);
    RepoNovoAuto(playerid);
    Savjet(playerid, T(playerid, "Dovezite se unatrag do auta i pritisnite ~y~ALT~w~~n~Odvezite ga na mjesto za dovoz~n~Obidite auto i pritisnite ~y~ALT~w~ kod strelica"), 8000);
    return 1;
}

static RepoIgraKraj(playerid)
{
    if (!gRepoIgra[playerid]) return;
    gRepoIgra[playerid] = false;
    if (gRepoIgraTimer[playerid]) KillTimer(gRepoIgraTimer[playerid]);
    gRepoIgraTimer[playerid] = 0;
    if (!IsPlayerConnected(playerid)) return;
    IgraTdMakni(playerid);
    ClearAnimations(playerid, SYNC_ALL);
}

RepoKraj(playerid)
{
    if (!gRepoFaza[playerid]) return;
    RepoIgraKraj(playerid);
    gRepoFaza[playerid] = RPF_NEMA;
    RepoAutoMakni(playerid);
    if (gRepoKamion[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gRepoKamion[playerid]);
    gRepoKamion[playerid] = INVALID_VEHICLE_ID;
    if (IsPlayerConnected(playerid)) DisablePlayerCheckpoint(playerid);
}

static RepoBlizuStrane(playerid)
{
    new v = gRepoAuto[playerid], Float:px, Float:py, Float:pz, Float:vx, Float:vy, Float:vz, Float:va;
    GetPlayerPos(playerid, px, py, pz);
    GetVehiclePos(v, vx, vy, vz);
    GetVehicleZAngle(v, va);
    new Float:c = floatcos(va, degrees), Float:sn = floatsin(va, degrees);
    for (new s = 0; s < 4; s++)
    {
        if ((gRepoDijelovi[playerid] >> (s * 2) & 3) == 3) continue;
        new Float:ox, Float:oy;
        RepoStrana(v, s, ox, oy);
        if (VectorSize(px - (vx + ox * c - oy * sn), py - (vy + ox * sn + oy * c), pz - vz) <= RP_STRANA_DOMET) return s;
    }
    return -1;
}

static RepoIgraPanel(playerid)
{
    IgraPanel(playerid, T(playerid, "Repo"), 60.0, RP_BOJA);
    IgraTraka(playerid, gRepoZel[playerid], gRepoZel[playerid] + RP_TRAKA_ZELENO, 0x1F3A5FFF, 0x27AE60FF, 0x1F3A5FFF, T(playerid, "Odvrnite dio: ~y~SPACE~w~ kad je oznaka u zelenom."));
    IgraOznakaPomakni(playerid, gRepoTraka[playerid]);
}

static RepoRastavi(playerid)
{
    if (gRepoIgra[playerid]) return 1;
    new s = RepoBlizuStrane(playerid);
    if (s == -1) return ObavijestGreska(playerid, T(playerid, "Pridite strelici kraj auta.")), 1;
    new v = gRepoAuto[playerid], Float:px, Float:py, Float:pz, Float:vx, Float:vy, Float:vz, Float:va, Float:ox, Float:oy;
    GetPlayerPos(playerid, px, py, pz);
    GetVehiclePos(v, vx, vy, vz);
    GetVehicleZAngle(v, va);
    RepoStrana(v, s, ox, oy);
    new Float:sx = vx + ox * floatcos(va, degrees) - oy * floatsin(va, degrees), Float:sy = vy + ox * floatsin(va, degrees) + oy * floatcos(va, degrees);
    SetPlayerPos(playerid, sx, sy, pz);
    SetPlayerFacingAngle(playerid, atan2(-(sx - vx), sy - vy));
    ApplyAnimation(playerid, "CAR", "Fixn_Car_Loop", 4.1, true, false, false, false, 0, SYNC_ALL);
    gRepoIgra[playerid] = true;
    gRepoIgraStrana[playerid] = s;
    gRepoTraka[playerid] = 0.0;
    gRepoSmjer[playerid] = 2.4;
    gRepoZel[playerid] = float(15 + random(65));
    RepoIgraPanel(playerid);
    gRepoIgraTimer[playerid] = SetTimerEx("RepoTrakaKorak", RP_TRAKA_MS, true, "i", playerid);
    return 1;
}

forward RepoTrakaKorak(playerid);
public RepoTrakaKorak(playerid)
{
    if (!gRepoIgra[playerid]) return;
    gRepoTraka[playerid] += gRepoSmjer[playerid];
    if (gRepoTraka[playerid] >= 100.0 || gRepoTraka[playerid] <= 0.0) gRepoSmjer[playerid] = -gRepoSmjer[playerid];
    IgraOznakaPomakni(playerid, gRepoTraka[playerid]);
}

static RepoDioSkini(playerid, s, bool:cijel)
{
    new dio = (gRepoDijelovi[playerid] >> (s * 2) & 1) ? 1 : 0;
    gRepoDijelovi[playerid] |= 1 << (s * 2 + dio);
    RepoDioPostavi(gRepoAuto[playerid], s, dio);
    if ((gRepoDijelovi[playerid] >> (s * 2) & 3) == 3) RepoStranaMakni(playerid, s);
    new n = 0, poruka[144];
    for (new b = 0; b < RP_DIJELOVA; b++) if (gRepoDijelovi[playerid] >> b & 1) n++;
    new iznos = cijel ? gRepoIsplata[playerid] / RP_DIJELOVA : 0;
    if (iznos && DajNovac(playerid, iznos)) gRepoGotovo[playerid] += iznos;
    PlayerPlaySound(playerid, cijel ? 1056 : 1055, 0.0, 0.0, 0.0);
    if (cijel) format(poruka, sizeof poruka, T(playerid, "Skinuli ste dio (%d/%d). +$%d"), n, RP_DIJELOVA, iznos);
    else format(poruka, sizeof poruka, T(playerid, "Dio je unisten (%d/%d). Za njega ne dobivate nista."), n, RP_DIJELOVA);
    IgraObavijest(playerid, T(playerid, "Repo"), cijel ? RP_BOJA : OBAVIJEST_GRESKA, poruka);
    if (n < RP_DIJELOVA)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE repo_cars SET parts = %d, earned = %d WHERE account = %d", gRepoDijelovi[playerid], gRepoGotovo[playerid], gAccountId[playerid]));
        return;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM repo_cars WHERE account = %d", gAccountId[playerid]));
    format(poruka, sizeof poruka, T(playerid, "Auto je rastavljen. Ukupno ste zaradili $%d."), gRepoGotovo[playerid]);
    IgraObavijest(playerid, T(playerid, "Repo"), RP_BOJA, poruka);
    RepoNovoAuto(playerid);
}

static RepoIstovari(playerid)
{
    new k = gRepoKamion[playerid], v = gRepoAuto[playerid], Float:ox, Float:oy;
    DetachTrailerFromVehicle(k);
    SetVehiclePos(v, gRepoCilj[0], gRepoCilj[1], gRepoCilj[2] + 0.5);
    SetVehicleZAngle(v, gRepoCilj[3]);
    VoziloSet(v, VP_VRATA, 1);
    gRepoFaza[playerid] = RPF_RASTAVLJA;
    DisablePlayerCheckpoint(playerid);
    RepoOznakeMakni(playerid);
    for (new s = 0; s < 4; s++)
    {
        if ((gRepoDijelovi[playerid] >> (s * 2) & 3) == 3) continue;
        RepoStrana(v, s, ox, oy);
        gRepoOznaka[playerid][s] = CreateDynamicObject(RP_OZNAKA_MODEL, 0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0, 0, playerid);
        AttachDynamicObjectToVehicle(gRepoOznaka[playerid][s], v, ox, oy, 1.0, 0.0, 0.0, 0.0);
        gRepoZona[playerid][s] = InterakcijaZona(0.0, 0.0, 0.0, RP_STRANA_DOMET, 0, 0, SAVJET_DIO);
        AttachDynamicAreaToVehicle(gRepoZona[playerid][s], v, ox, oy, 0.0);
    }
    ObavijestInfo(playerid, T(playerid, "Repo"), T(playerid, "Auto je na mjestu. Izadite, obidite ga i pritisnite ALT kod strelica da skinete dijelove."));
}

static bool:RepoIzaKamiona(playerid)
{
    new k = gRepoKamion[playerid], v = gRepoAuto[playerid], Float:kx, Float:ky, Float:kz, Float:ka, Float:vx, Float:vy, Float:vz, Float:sx, Float:sy, Float:sz;
    GetVehiclePos(k, kx, ky, kz);
    GetVehicleZAngle(k, ka);
    GetVehiclePos(v, vx, vy, vz);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    return VectorSize(vx - (kx + floatsin(ka, degrees) * RP_KUKA), vy - (ky - floatcos(ka, degrees) * RP_KUKA), vz - kz) <= sy / 2.0 + RP_KUKA_DOMET;
}

static bool:RepoNaCilju(playerid)
{
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gRepoAuto[playerid], x, y, z);
    return VectorSize(x - gRepoCilj[0], y - gRepoCilj[1], z - gRepoCilj[2]) < RP_ISTOVAR_DOMET;
}

static RepoVuce(playerid)
{
    RepoPodrucjeMakni(playerid);
    gRepoFaza[playerid] = RPF_VUCE;
    gRepoSavjet[playerid] = 0;
    SetPlayerCheckpoint(playerid, gRepoCilj[0], gRepoCilj[1], gRepoCilj[2], RP_ISTOVAR_DOMET / 2.0);
    ObavijestInfo(playerid, T(playerid, "Repo"), T(playerid, "Auto je prikacen. Odvezite ga na mjesto za dovoz (oznaceno na karti)."));
}

static RepoTrazi(playerid)
{
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gRepoAuto[playerid], x, y, z);
    gRepoFaza[playerid] = RPF_TRAZI;
    gRepoSavjet[playerid] = 0;
    SetPlayerCheckpoint(playerid, x, y, z, 4.0);
}

bool:RepoKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gRepoFaza[playerid]) return false;
    if (gRepoIgra[playerid])
    {
        if (!(newkeys & KEY_SPRINT) || (oldkeys & KEY_SPRINT)) return true;
        new s = gRepoIgraStrana[playerid], bool:pogodak = gRepoTraka[playerid] >= gRepoZel[playerid] && gRepoTraka[playerid] <= gRepoZel[playerid] + RP_TRAKA_ZELENO;
        RepoIgraKraj(playerid);
        if (gRepoFaza[playerid] == RPF_RASTAVLJA) RepoDioSkini(playerid, s, pogodak);
        return true;
    }
    if (!(newkeys & KEY_FIRE) || (oldkeys & KEY_FIRE) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerVehicleID(playerid) != gRepoKamion[playerid]) return false;
    if (gRepoFaza[playerid] == RPF_TRAZI)
    {
        if (!RepoIzaKamiona(playerid)) return ObavijestGreska(playerid, T(playerid, "Auto nije iza kamiona.")), true;
        AttachTrailerToVehicle(gRepoAuto[playerid], gRepoKamion[playerid]);
        RepoVuce(playerid);
        return true;
    }
    if (gRepoFaza[playerid] != RPF_VUCE) return false;
    if (RepoNaCilju(playerid))
    {
        RepoIstovari(playerid);
        return true;
    }
    DetachTrailerFromVehicle(gRepoKamion[playerid]);
    RepoTrazi(playerid);
    ObavijestInfo(playerid, T(playerid, "Repo"), T(playerid, "Otkacili ste auto."));
    return true;
}

static RepoIzbornik(playerid)
{
    if (!gRepoFaza[playerid]) return UpitDaNe(playerid, UPIT_REPO, 0, 1, T(playerid, "Repo"), T(playerid, "Zelite li zapoceti smjenu?"));
    return UpitDaNe(playerid, UPIT_REPO, 0, 2, T(playerid, "Repo"), T(playerid, "Zelite li zavrsiti smjenu?"));
}

RepoUpit(playerid, opcija, bool:da)
{
    if (!da || !RepoNaMjestu(playerid, RP_DOMET + 2.0)) return 1;
    if (opcija == 1 && !gRepoFaza[playerid]) return RepoPocni(playerid);
    if (opcija == 2 && gRepoFaza[playerid])
    {
        RepoKraj(playerid);
        return ObavijestInfo(playerid, T(playerid, "Repo"), T(playerid, "Smjena je zavrsena."));
    }
    return 1;
}

RepoPosao(playerid)
{
    if (gRepoFaza[playerid])
    {
        RepoKraj(playerid);
        return ObavijestInfo(playerid, T(playerid, "Repo"), T(playerid, "Smjena je zavrsena.")), 1;
    }
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || !RepoNaMjestu(playerid)) return 0;
    return RepoIzbornik(playerid), 1;
}

RepoAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    if (gRepoFaza[playerid] == RPF_RASTAVLJA && RepoBlizuStrane(playerid) != -1) return RepoRastavi(playerid);
    if (!RepoNaMjestu(playerid)) return 0;
    return RepoIzbornik(playerid), 1;
}

RepoStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_DRIVER) return;
    new v = GetPlayerVehicleID(playerid);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gRepoFaza[i]) continue;
        if (v == gRepoAuto[i] || (v == gRepoKamion[i] && i != playerid))
        {
            RemovePlayerFromVehicle(playerid);
            return;
        }
    }
}

static RepoUspravi(v)
{
    new Float:hp, Float:qw, Float:qx, Float:qy, Float:qz;
    GetVehicleHealth(v, hp);
    if (hp < 1000.0) SetVehicleHealth(v, 1000.0);
    GetVehicleRotationQuat(v, qw, qx, qy, qz);
    if (1.0 - 2.0 * (qx * qx + qy * qy) > 0.3) return;
    new Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    SetVehiclePos(v, x, y, z + 1.0);
    SetVehicleZAngle(v, a);
}

RepoTick()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gRepoFaza[i]) continue;
        new k = gRepoKamion[i], v = gRepoAuto[i], Float:hp;
        GetVehicleHealth(k, hp);
        if (!IsPlayerConnected(i) || IsRanjen(i) || !GetVehicleModel(k) || hp < RP_KAMION_HP || !GetVehicleModel(v))
        {
            RepoKraj(i);
            if (IsPlayerConnected(i)) ObavijestGreska(i, T(i, "Repo smjena je zavrsena."));
            continue;
        }
        if (gRepoFaza[i] == RPF_RASTAVLJA) continue;
        if (gRepoTrazi[i] && !IgraObavijestVidljiva(i)) RepoTraziPanel(i);
        new bool:vuce = GetVehicleTrailer(k) == v;
        if (gRepoFaza[i] == RPF_TRAZI && !vuce) RepoUspravi(v);
        if (gRepoFaza[i] == RPF_TRAZI && vuce) RepoVuce(i);
        else if (gRepoFaza[i] == RPF_VUCE && !vuce)
        {
            RepoTrazi(i);
            ObavijestGreska(i, T(i, "Auto se otkacio. Ponovo ga prikacite."));
        }
        new savjet = 0;
        if (GetPlayerVehicleID(i) == k)
        {
            if (gRepoFaza[i] == RPF_TRAZI && RepoIzaKamiona(i)) savjet = 1;
            else if (gRepoFaza[i] == RPF_VUCE && RepoNaCilju(i)) savjet = 2;
        }
        if (savjet != gRepoSavjet[i] && savjet) Savjet(i, savjet == 1 ? T(i, "~y~ALT~w~ - zakaci auto") : T(i, "~y~ALT~w~ - otkaci auto"), RP_SAVJET_MS);
        gRepoSavjet[i] = savjet;
    }
}

ShowRepoAdmin(playerid)
{
    new list[384], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Repo mjesto na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Repo kamion na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto za dovoz repo auta na mojoj poziciji"));
    return PrikaziDialog(playerid, DLG_APANEL_REPO, DIALOG_STYLE_LIST, T(playerid, "Repo posao"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_repo(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG) || listitem < 0 || listitem > 2) return 1;
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ovo se postavlja samo izvana.")), ShowRepoAdmin(playerid);
    new Float:x, Float:y, Float:z, Float:a, v = GetPlayerVehicleID(playerid);
    if (v) GetVehiclePos(v, x, y, z), GetVehicleZAngle(v, a);
    else GetPlayerPos(playerid, x, y, z), GetPlayerFacingAngle(playerid, a);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO job_repo (id, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", listitem + 1, x, y, z, a));
    if (listitem == 0)
    {
        gRepoMjesto[0] = x, gRepoMjesto[1] = y, gRepoMjesto[2] = z, gRepoMjesto[3] = a;
        RepoMjestoStvori();
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Repo mjesto postavljeno."));
    }
    else if (listitem == 1)
    {
        gRepoKamionPoz[0] = x, gRepoKamionPoz[1] = y, gRepoKamionPoz[2] = z, gRepoKamionPoz[3] = a;
        gRepoImaKamion = true;
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Repo kamion postavljen."));
    }
    else
    {
        gRepoCilj[0] = x, gRepoCilj[1] = y, gRepoCilj[2] = z, gRepoCilj[3] = a;
        gRepoImaCilj = true;
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto za dovoz repo auta postavljeno."));
    }
    return ShowRepoAdmin(playerid);
}
