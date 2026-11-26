#define RP_KAMION_MODEL     525
#define RP_DOMET            2.5
#define RP_ISTOVAR_DOMET    15.0
#define RP_STRANA_DOMET     1.6
#define RP_STRANA_RAZMAK    0.6
#define RP_KUKA             3.2
#define RP_KUKA_DOMET       1.8
#define RP_UDALJENOST_MIN   400.0
#define RP_UDALJENOST_MAX   1400.0
#define RP_POSTO            5
#define RP_ISPLATA_MIN      300
#define RP_ISPLATA_MAX      3000
#define RP_CIJENA_ZADANO    20000
#define RP_DIJELOVA         8
#define RP_RAD_MS           2500
#define RP_KAMION_HP        300.0
#define RP_BOJA             0x95A5A6FF
#define RP_OZNAKA_MODEL     1318

enum { RPF_NEMA, RPF_TRAZI, RPF_VUCE, RPF_RASTAVLJA }

static const Float:gRepoZadano[4] = { 2183.0, -2253.0, 13.4, 225.0 };
static const gRepoZabranjeni[] = {
    403, 406, 407, 408, 416, 417, 425, 427, 428, 430, 431, 432, 433, 435, 437, 441, 443, 444, 446, 447, 448, 449, 450, 452, 453, 454, 455, 456,
    460, 461, 462, 463, 464, 465, 468, 469, 471, 472, 473, 476, 481, 484, 486, 487, 488, 493, 497, 501, 509, 510, 511, 512, 513, 514, 515, 519,
    520, 521, 522, 523, 524, 525, 530, 531, 532, 537, 538, 539, 544, 548, 553, 556, 557, 563, 564, 569, 570, 571, 572, 573, 574, 577, 578, 581,
    583, 584, 586, 590, 591, 592, 593, 594, 595, 601, 606, 607, 608, 610, 611
};
static const gRepoRezerva[] = { 400, 401, 404, 405, 410, 412, 419, 421, 426, 436, 445, 466, 467, 474, 475, 479, 491, 492, 496, 507, 516, 517, 518, 526, 527, 529, 540, 542, 546, 547, 549, 550, 551, 560, 562, 565, 566, 567, 575, 576, 580, 585, 587, 589, 600, 602, 603 };

static Float:gRepoMjesto[4], Float:gRepoKamionPoz[4], STREAMER_TAG_3D_TEXT_LABEL:gRepoTekst, bool:gRepoImaMjesto, bool:gRepoDozvoljen[212];
static gRepoFaza[MAX_PLAYERS], gRepoKamion[MAX_PLAYERS], gRepoAuto[MAX_PLAYERS], gRepoIsplata[MAX_PLAYERS], gRepoGotovo[MAX_PLAYERS], gRepoDijelovi[MAX_PLAYERS], bool:gRepoRadi[MAX_PLAYERS];
static STREAMER_TAG_OBJECT:gRepoOznaka[MAX_PLAYERS][4];

InitRepo()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_repo (id INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    for (new k = 0; k < 4; k++) gRepoMjesto[k] = gRepoKamionPoz[k] = gRepoZadano[k];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM job_repo WHERE id IN (1, 2)");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        for (new k = 0; k < 4; k++)
        {
            if (DB_GetFieldInt(res, 0) == 1) gRepoMjesto[k] = DB_GetFieldFloat(res, k + 1);
            else gRepoKamionPoz[k] = DB_GetFieldFloat(res, k + 1);
        }
    }
    DB_FreeResultSet(res);
    for (new m = 0; m < sizeof gRepoDozvoljen; m++) gRepoDozvoljen[m] = true;
    for (new k = 0; k < sizeof gRepoZabranjeni; k++) gRepoDozvoljen[gRepoZabranjeni[k] - 400] = false;
    for (new i = 0; i < MAX_PLAYERS; i++) gRepoKamion[i] = gRepoAuto[i] = INVALID_VEHICLE_ID;
    RepoMjestoStvori();
}

static RepoMjestoStvori()
{
    if (gRepoImaMjesto) DestroyDynamic3DTextLabel(gRepoTekst);
    gRepoImaMjesto = true;
    gRepoTekst = CreateDynamic3DTextLabel("Repo\nALT", RP_BOJA, gRepoMjesto[0], gRepoMjesto[1], gRepoMjesto[2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gRepoTekst, RP_DOMET);
}

RepoLokacija(&Float:x, &Float:y, &Float:z)
{
    x = gRepoMjesto[0];
    y = gRepoMjesto[1];
    z = gRepoMjesto[2];
    return 1;
}

static bool:RepoNaMjestu(playerid, Float:r = RP_DOMET)
{
    return !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, r, gRepoMjesto[0], gRepoMjesto[1], gRepoMjesto[2]);
}

static RepoStrana(v, s, &Float:ox, &Float:oy)
{
    new Float:sx, Float:sy, Float:sz;
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    ox = s == 2 ? -(sx / 2.0 + RP_STRANA_RAZMAK) : s == 3 ? sx / 2.0 + RP_STRANA_RAZMAK : 0.0;
    oy = s == 0 ? sy / 2.0 + RP_STRANA_RAZMAK : s == 1 ? -(sy / 2.0 + RP_STRANA_RAZMAK) : 0.0;
}

static RepoOznakeMakni(playerid)
{
    for (new s = 0; s < 4; s++)
    {
        if (_:gRepoOznaka[playerid][s]) DestroyDynamicObject(gRepoOznaka[playerid][s]);
        gRepoOznaka[playerid][s] = STREAMER_TAG_OBJECT:0;
    }
}

static RepoAutoMakni(playerid)
{
    RepoOznakeMakni(playerid);
    if (gRepoAuto[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gRepoAuto[playerid]);
    gRepoAuto[playerid] = INVALID_VEHICLE_ID;
}

static RepoNovoAuto(playerid)
{
    RepoAutoMakni(playerid);
    new cijena = RP_CIJENA_ZADANO, model = SalonNasumicnoVozilo(cijena, gRepoDozvoljen);
    if (!model) model = gRepoRezerva[random(sizeof gRepoRezerva)], cijena = RP_CIJENA_ZADANO;
    new Float:x, Float:y, Float:z, Float:a = float(random(360)), bool:nasao = false;
    for (new k = 0; k < 15 && !nasao; k++)
    {
        new Float:kut = float(random(360)), Float:d = RP_UDALJENOST_MIN + float(random(floatround(RP_UDALJENOST_MAX - RP_UDALJENOST_MIN)));
        Lik_Cesta(gRepoMjesto[0] + d * floatcos(kut, degrees), gRepoMjesto[1] + d * floatsin(kut, degrees), gRepoMjesto[2], x, y, z);
        nasao = VectorSize(x - gRepoMjesto[0], y - gRepoMjesto[1], 0.0) > RP_UDALJENOST_MIN * 0.75;
    }
    for (new k = 0; k < 8; k++)
    {
        if (!Lik_NaCesti(x - 6.0 * floatsin(float(k * 45), degrees), y + 6.0 * floatcos(float(k * 45), degrees), z)) continue;
        a = float(k * 45);
        x += 2.0 * floatcos(a, degrees);
        y += 2.0 * floatsin(a, degrees);
        break;
    }
    gRepoAuto[playerid] = CreateVehicle(model, x, y, z + 0.6, a, -1, -1, -1);
    if (gRepoAuto[playerid] == INVALID_VEHICLE_ID) return 0;
    VoziloSet(gRepoAuto[playerid], VP_MOTOR, 0);
    VoziloSet(gRepoAuto[playerid], VP_VRATA, 1);
    gRepoIsplata[playerid] = clamp(cijena / 100 * RP_POSTO, RP_ISPLATA_MIN, RP_ISPLATA_MAX);
    gRepoGotovo[playerid] = 0;
    gRepoDijelovi[playerid] = 0;
    gRepoFaza[playerid] = RPF_TRAZI;
    SetPlayerCheckpoint(playerid, x, y, z, 4.0);
    new poruka[144];
    format(poruka, sizeof poruka, T(playerid, "Pronadite %s (oznaceno na karti) i prikacite ga kamionom unatrag."), gImenaVozila[model - 400]);
    ObavijestInfo(playerid, T(playerid, "Repo"), poruka);
    return 1;
}

static RepoPocni(playerid)
{
    new v = CreateVehicle(RP_KAMION_MODEL, gRepoKamionPoz[0], gRepoKamionPoz[1], gRepoKamionPoz[2] + 0.5, gRepoKamionPoz[3], 0, 0, -1);
    if (v == INVALID_VEHICLE_ID) return ObavijestGreska(playerid, T(playerid, "Trenutno nema slobodnih kamiona.")), 1;
    gRepoKamion[playerid] = v;
    PutPlayerInVehicle(playerid, v, 0);
    VoziloSet(v, VP_MOTOR, 1);
    RepoNovoAuto(playerid);
    Savjet(playerid, T(playerid, "Dovezite se unatrag do auta da ga prikacite~n~Odvezite ga na repo mjesto~n~Obidite auto i pritisnite ~y~ALT~w~ kod strelica"), 8000);
    return 1;
}

RepoKraj(playerid)
{
    if (!gRepoFaza[playerid]) return;
    gRepoFaza[playerid] = RPF_NEMA;
    gRepoRadi[playerid] = false;
    RepoAutoMakni(playerid);
    if (gRepoKamion[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gRepoKamion[playerid]);
    gRepoKamion[playerid] = INVALID_VEHICLE_ID;
    if (IsPlayerConnected(playerid)) DisablePlayerCheckpoint(playerid);
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

static RepoRastavi(playerid)
{
    if (gRepoRadi[playerid]) return 1;
    new s = RepoBlizuStrane(playerid);
    if (s == -1) return ObavijestGreska(playerid, T(playerid, "Pridite strelici kraj auta.")), 1;
    gRepoRadi[playerid] = true;
    ApplyAnimation(playerid, "CAR", "Fixn_Car_Loop", 4.1, true, false, false, false, 0, SYNC_ALL);
    SetTimerEx("RepoDioGotov", RP_RAD_MS, false, "iii", playerid, gAccountId[playerid], s);
    return 1;
}

forward RepoDioGotov(playerid, racun, s);
public RepoDioGotov(playerid, racun, s)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || !gRepoRadi[playerid]) return;
    gRepoRadi[playerid] = false;
    ClearAnimations(playerid, SYNC_ALL);
    if (gRepoFaza[playerid] != RPF_RASTAVLJA) return;
    new dio = (gRepoDijelovi[playerid] >> (s * 2) & 1) ? 1 : 0;
    gRepoDijelovi[playerid] |= 1 << (s * 2 + dio);
    RepoDioPostavi(gRepoAuto[playerid], s, dio);
    if ((gRepoDijelovi[playerid] >> (s * 2) & 3) == 3 && _:gRepoOznaka[playerid][s])
    {
        DestroyDynamicObject(gRepoOznaka[playerid][s]);
        gRepoOznaka[playerid][s] = STREAMER_TAG_OBJECT:0;
    }
    new n = 0, poruka[144];
    for (new b = 0; b < RP_DIJELOVA; b++) if (gRepoDijelovi[playerid] >> b & 1) n++;
    new iznos = n == RP_DIJELOVA ? gRepoIsplata[playerid] - gRepoGotovo[playerid] : gRepoIsplata[playerid] / RP_DIJELOVA;
    if (DajNovac(playerid, iznos)) gRepoGotovo[playerid] += iznos;
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    if (n < RP_DIJELOVA)
    {
        format(poruka, sizeof poruka, T(playerid, "Skinuli ste dio (%d/%d). +$%d"), n, RP_DIJELOVA, iznos);
        GameTextForPlayer(playerid, poruka, 2000, 3);
        return;
    }
    format(poruka, sizeof poruka, T(playerid, "Auto je rastavljen. Ukupno ste zaradili $%d."), gRepoGotovo[playerid]);
    IgraObavijest(playerid, T(playerid, "Repo"), RP_BOJA, poruka);
    RepoNovoAuto(playerid);
}

static RepoIstovari(playerid)
{
    new k = gRepoKamion[playerid], v = gRepoAuto[playerid], Float:ox, Float:oy;
    DetachTrailerFromVehicle(k);
    VoziloSet(v, VP_VRATA, 1);
    gRepoFaza[playerid] = RPF_RASTAVLJA;
    DisablePlayerCheckpoint(playerid);
    RepoOznakeMakni(playerid);
    for (new s = 0; s < 4; s++)
    {
        RepoStrana(v, s, ox, oy);
        gRepoOznaka[playerid][s] = CreateDynamicObject(RP_OZNAKA_MODEL, 0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0, 0, playerid);
        AttachDynamicObjectToVehicle(gRepoOznaka[playerid][s], v, ox, oy, 1.0, 0.0, 0.0, 0.0);
    }
    ObavijestInfo(playerid, T(playerid, "Repo"), T(playerid, "Auto je na mjestu. Izadite, obidite ga i pritisnite ALT kod strelica da skinete dijelove."));
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
        new bool:vuce = GetVehicleTrailer(k) == v;
        if (gRepoFaza[i] == RPF_VUCE)
        {
            new Float:x, Float:y, Float:z;
            GetVehiclePos(k, x, y, z);
            if (!vuce)
            {
                gRepoFaza[i] = RPF_TRAZI;
                GetVehiclePos(v, x, y, z);
                SetPlayerCheckpoint(i, x, y, z, 4.0);
                ObavijestGreska(i, T(i, "Auto se otkacio. Ponovo ga prikacite."));
            }
            else if (VectorSize(x - gRepoMjesto[0], y - gRepoMjesto[1], z - gRepoMjesto[2]) < RP_ISTOVAR_DOMET) RepoIstovari(i);
            continue;
        }
        if (!vuce && GetPlayerVehicleID(i) == k)
        {
            new Float:kx, Float:ky, Float:kz, Float:fx, Float:fy, Float:ka, Float:vx, Float:vy, Float:vz, Float:sx, Float:sy, Float:sz;
            GetVehiclePos(k, kx, ky, kz);
            GetVehicleZAngle(k, ka);
            fx = -floatsin(ka, degrees);
            fy = floatcos(ka, degrees);
            GetVehiclePos(v, vx, vy, vz);
            GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
            if (VectorSize(vx - (kx - fx * RP_KUKA), vy - (ky - fy * RP_KUKA), vz - kz) > sy / 2.0 + RP_KUKA_DOMET) continue;
            AttachTrailerToVehicle(v, k);
            vuce = true;
        }
        if (!vuce) continue;
        gRepoFaza[i] = RPF_VUCE;
        SetPlayerCheckpoint(i, gRepoMjesto[0], gRepoMjesto[1], gRepoMjesto[2], RP_ISTOVAR_DOMET / 2.0);
        ObavijestInfo(i, T(i, "Repo"), T(i, "Auto je prikacen. Odvezite ga na repo mjesto (oznaceno na karti)."));
    }
}

ShowRepoAdmin(playerid)
{
    new list[256], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Repo mjesto na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Repo kamion na mojoj poziciji"));
    return PrikaziDialog(playerid, DLG_APANEL_REPO, DIALOG_STYLE_LIST, T(playerid, "Repo posao"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_repo(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG) || listitem < 0 || listitem > 1) return 1;
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
    else
    {
        gRepoKamionPoz[0] = x, gRepoKamionPoz[1] = y, gRepoKamionPoz[2] = z, gRepoKamionPoz[3] = a;
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Repo kamion postavljen."));
    }
    return ShowRepoAdmin(playerid);
}
