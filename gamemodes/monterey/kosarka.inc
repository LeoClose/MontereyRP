#define KOS_MAX             32
#define KOS_BOJA            0xE67E22E6
#define KOS_MODEL           946
#define KOS_LOPTA           2114
#define KOS_SLOT            3
#define KOS_TOCAKA          24
#define KOS_KORAK_MS        70
#define KOS_NABIJA_MS       50
#define KOS_HOD_MS          150
#define KOS_STUP_Z          2.2
#define KOS_OBRUC_Z         3.2
#define KOS_OBRUC_D         0.38
#define KOS_LOPTA_Z         0.13
#define KOS_TRICA           6.75
#define KOS_ZAKUCAJ         2.2
#define KOS_DOMET           12.0
#define KOS_DODAJ           15.0
#define KOS_UZMI            1.6
#define KOS_UKRADI          1.4
#define KOS_KRADJA_MS       1500
#define KOS_IZLAZ           3.0
#define KOS_TEREN           40.0
#define KOS_TIM_MAX         5
#define KOS_POBJEDA         21
#define KOS_HUD             5

enum { KF_SLOBODNA, KF_NOSI, KF_LET, KF_NEMA }
enum { KH_STOJI, KH_HODA, KH_TRCI }
enum E_KOS { bool:koIma, koDb, Float:koX, Float:koY, Float:koZ, Float:koA, koInt, koSvijet, STREAMER_TAG_OBJECT:koObj, STREAMER_TAG_OBJECT:koLopta, STREAMER_TAG_3D_TEXT_LABEL:koTekst,
    koFaza, koNosi, Float:koLx, Float:koLy, Float:koLz, Float:koRot, koToken, koPn, koPi, koPogodak, koPoeni, koStrijelac, koPrima, koMetaTeren, koMeta, koTeren, koPar, Float:koSx, Float:koSy, Float:koSz, Float:koR, koBod[2] }

static const Float:gKosSa[][4] = {
    { 2768.15625, -2019.6171875, 14.6015625, 90.0 }, { 2795.4921875, -2019.6171875, 14.59375, -90.0 }, { 2533.8828125, -1667.578125, 16.2890625, -90.0 },
    { 2290.640625, -1541.609375, 28.0703125, 0.0 }, { 2290.578125, -1514.2734375, 28.046875, -180.0 }, { 2316.9375, -1541.609375, 26.5, 0.0 }, { 2316.9375, -1514.2734375, 26.5, -180.0 }
};
static const gKosSaModel[sizeof gKosSa] = { 946, 946, 947, 946, 946, 946, 946 };
static const gKosTimIme[2][] = { "PLAVI", "CRVENI" };
static const gKosTimBoja[2][] = { "~b~", "~r~" };

static gKosHint[MAX_PLAYERS], gKos[KOS_MAX][E_KOS], Float:gKosPut[KOS_MAX][KOS_TOCAKA][3], gKosDrzi[MAX_PLAYERS] = { -1, ... }, bool:gKosNabija[MAX_PLAYERS], Float:gKosSnaga[MAX_PLAYERS], bool:gKosGore[MAX_PLAYERS], gKosKradja[MAX_PLAYERS], PlayerText:gKosTd[MAX_PLAYERS][2], gApKos[MAX_PLAYERS][KOS_MAX];
static gKosTeren[MAX_PLAYERS] = { -1, ... }, gKosTim[MAX_PLAYERS], PlayerText:gKosHud[MAX_PLAYERS][KOS_HUD], gKosHod[MAX_PLAYERS], gKosAnimDo[MAX_PLAYERS];

static KosNaprijed(Float:a, &Float:fx, &Float:fy)
{
    fx = -floatsin(a, degrees);
    fy = floatcos(a, degrees);
}

static KosObruc(k, &Float:x, &Float:y, &Float:z)
{
    new Float:fx, Float:fy;
    KosNaprijed(gKos[k][koA], fx, fy);
    x = gKos[k][koX] + fx * KOS_OBRUC_D;
    y = gKos[k][koY] + fy * KOS_OBRUC_D;
    z = gKos[k][koZ] + KOS_OBRUC_Z;
}

static KosSansa(Float:d, Float:snaga)
{
    new osnova = d < KOS_ZAKUCAJ ? 95 : d < 4.5 ? 78 : d < KOS_TRICA ? 62 : d < 9.0 ? 42 : 22, Float:f = 1.0 - floatabs(snaga - 75.0) / 50.0;
    if (f < 0.15) f = 0.15;
    return floatround(float(osnova) * f);
}

static KosAnim(playerid, const lib[], const ime[], bool:petlja, bool:kreci, bool:zamrzni, ms)
{
    ApplyAnimation(playerid, lib, ime, 4.1, petlja, kreci, kreci, zamrzni, 0);
    gKosAnimDo[playerid] = GetTickCount() + ms;
    gKosHod[playerid] = -1;
}

InitKosarka()
{
    assert(KosSansa(1.0, 75.0) == 95 && KosSansa(8.0, 75.0) == 42 && KosSansa(8.0, 0.0) == 6);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM sqlite_master WHERE type = 'table' AND name = 'bb_hoops'"), bool:novo = DB_GetRowCount(res) == 0;
    DB_FreeResultSet(res);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bb_hoops (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    if (novo) for (new i = 0; i < sizeof gKosSa; i++)
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bb_hoops (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, 0, 0)", gKosSa[i][0], gKosSa[i][1], gKosSa[i][2] - KOS_STUP_Z, -gKosSa[i][3]));
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM bb_hoops LIMIT %d", KOS_MAX);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++, DB_SelectNextRow(res))
    {
        gKos[k][koDb] = DB_GetFieldInt(res, 0);
        gKos[k][koX] = DB_GetFieldFloat(res, 1);
        gKos[k][koY] = DB_GetFieldFloat(res, 2);
        gKos[k][koZ] = DB_GetFieldFloat(res, 3);
        gKos[k][koA] = DB_GetFieldFloat(res, 4);
        gKos[k][koInt] = DB_GetFieldInt(res, 5);
        gKos[k][koSvijet] = DB_GetFieldInt(res, 6);
        KosStvori(k);
    }
    DB_FreeResultSet(res);
    KosTereni();
    SetTimer("KosHod", KOS_HOD_MS, true);
}

static KosTereni()
{
    for (new k = 0; k < KOS_MAX; k++) OkladaZatvori(OK_KOSARKA, k);
    for (new p = 0, pZadnji = gIgracZadnji; p <= pZadnji; p++) if (gKosTeren[p] != -1) KosNapusti(p, false);
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (gKos[k][koFaza] == KF_NOSI) KosPusti(gKos[k][koNosi]);
        if (gKos[k][koLopta]) DestroyDynamicObject(gKos[k][koLopta]);
        gKos[k][koLopta] = STREAMER_TAG_OBJECT:0;
        gKos[k][koToken]++;
        gKos[k][koFaza] = KF_NEMA;
        gKos[k][koTeren] = k;
        gKos[k][koPar] = -1;
        gKos[k][koBod][0] = gKos[k][koBod][1] = 0;
    }
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (!gKos[k][koIma] || gKos[k][koPar] != -1) continue;
        new najbolji = -1, Float:najblize = KOS_TEREN, Float:fx, Float:fy;
        KosNaprijed(gKos[k][koA], fx, fy);
        for (new j = k + 1; j < KOS_MAX; j++)
        {
            if (!gKos[j][koIma] || gKos[j][koPar] != -1 || gKos[j][koSvijet] != gKos[k][koSvijet] || gKos[j][koInt] != gKos[k][koInt]) continue;
            new Float:dx = gKos[j][koX] - gKos[k][koX], Float:dy = gKos[j][koY] - gKos[k][koY], Float:d = VectorSize(dx, dy, 0.0), Float:gx, Float:gy;
            KosNaprijed(gKos[j][koA], gx, gy);
            if (d < 8.0 || d >= najblize || (dx * fx + dy * fy) / d < 0.8 || -(dx * gx + dy * gy) / d < 0.8) continue;
            najblize = d;
            najbolji = j;
        }
        if (najbolji == -1) continue;
        gKos[k][koPar] = najbolji;
        gKos[najbolji][koPar] = k;
        gKos[najbolji][koTeren] = k;
    }
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (!gKos[k][koIma]) continue;
        new j = gKos[k][koPar], tekst[64];
        if (j == -1) format(tekst, sizeof tekst, "{F39C12}Kosarka");
        else format(tekst, sizeof tekst, "{F39C12}Kosarka\n%s%s", gKos[k][koTeren] == k ? ("{6EC1FF}") : ("{FF7070}"), gKosTimIme[gKos[k][koTeren] == k ? 0 : 1]);
        UpdateDynamic3DTextLabelText(gKos[k][koTekst], 0xFFFFFFFF, tekst);
        if (gKos[k][koTeren] != k) continue;
        new Float:fx, Float:fy;
        KosNaprijed(gKos[k][koA], fx, fy);
        if (j != -1)
        {
            gKos[k][koSx] = (gKos[k][koX] + gKos[j][koX]) / 2.0;
            gKos[k][koSy] = (gKos[k][koY] + gKos[j][koY]) / 2.0;
            gKos[k][koSz] = (gKos[k][koZ] + gKos[j][koZ]) / 2.0;
            gKos[k][koR] = VectorSize(gKos[j][koX] - gKos[k][koX], gKos[j][koY] - gKos[k][koY], 0.0) / 2.0 + 4.0;
        }
        else
        {
            gKos[k][koSx] = gKos[k][koX] + fx * 4.5;
            gKos[k][koSy] = gKos[k][koY] + fy * 4.5;
            gKos[k][koSz] = gKos[k][koZ];
            gKos[k][koR] = 8.0;
        }
        KosLoptaReset(k);
    }
}

KosConnect(playerid)
{
    gKosDrzi[playerid] = -1;
    gKosTeren[playerid] = -1;
    gKosNabija[playerid] = false;
    for (new i = 0; i < sizeof gKosSa; i++) RemoveBuildingForPlayer(playerid, gKosSaModel[i], gKosSa[i][0], gKosSa[i][1], gKosSa[i][2], 0.5);
}

static KosStvori(k)
{
    gKos[k][koIma] = true;
    gKos[k][koObj] = CreateDynamicObject(KOS_MODEL, gKos[k][koX], gKos[k][koY], gKos[k][koZ] + KOS_STUP_Z, 0.0, 0.0, gKos[k][koA], gKos[k][koSvijet], gKos[k][koInt]);
    gKos[k][koTekst] = CreateDynamic3DTextLabel("{F39C12}Kosarka", 0xFFFFFFFF, gKos[k][koX], gKos[k][koY], gKos[k][koZ] + 1.2, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gKos[k][koSvijet], gKos[k][koInt]);
}

static KosLoptaReset(k)
{
    new Float:fx, Float:fy;
    KosNaprijed(gKos[k][koA], fx, fy);
    gKos[k][koToken]++;
    gKos[k][koFaza] = KF_SLOBODNA;
    gKos[k][koNosi] = INVALID_PLAYER_ID;
    gKos[k][koLx] = gKos[k][koSx];
    gKos[k][koLy] = gKos[k][koSy];
    gKos[k][koLz] = gKos[k][koSz] + KOS_LOPTA_Z;
    gKos[k][koRot] = 0.0;
    if (gKos[k][koLopta])
    {
        StopDynamicObject(gKos[k][koLopta]);
        SetDynamicObjectPos(gKos[k][koLopta], gKos[k][koLx], gKos[k][koLy], gKos[k][koLz]);
        SetDynamicObjectRot(gKos[k][koLopta], 0.0, 0.0, 0.0);
    }
    else gKos[k][koLopta] = CreateDynamicObject(KOS_LOPTA, gKos[k][koLx], gKos[k][koLy], gKos[k][koLz], 0.0, 0.0, 0.0, gKos[k][koSvijet], gKos[k][koInt], -1, 80.0);
}

static KosBroj(t, tim)
{
    new n = 0;
    for (new p = 0, pZadnji = gIgracZadnji; p <= pZadnji; p++) if (gKosTeren[p] == t && gKosTim[p] == tim) n++;
    return n;
}

static PlayerText:KosHudTd(playerid, Float:x, Float:y, Float:vel, TEXT_DRAW_ALIGN:poravnanje, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "_");
    PlayerTextDrawLetterSize(playerid, td, vel * 0.25, vel);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawBackgroundColour(playerid, td, 0x000000AA);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, td, true);
    return td;
}

static KosHudStvori(playerid)
{
    gKosHud[playerid][0] = KosHudTd(playerid, 320.0, 4.0, 0.9, TEXT_DRAW_ALIGN_CENTER, 0xFFFFFFFF);
    PlayerTextDrawSetString(playerid, gKosHud[playerid][0], "_~n~_~n~_~n~_~n~_~n~_~n~_");
    PlayerTextDrawUseBox(playerid, gKosHud[playerid][0], true);
    PlayerTextDrawBoxColour(playerid, gKosHud[playerid][0], 0x0B1622C8);
    PlayerTextDrawTextSize(playerid, gKosHud[playerid][0], 0.0, 190.0);
    gKosHud[playerid][1] = KosHudTd(playerid, 320.0, 6.0, 1.5, TEXT_DRAW_ALIGN_CENTER, 0xFFFFFFFF);
    PlayerTextDrawFont(playerid, gKosHud[playerid][1], TEXT_DRAW_FONT_2);
    gKosHud[playerid][2] = KosHudTd(playerid, 230.0, 24.0, 0.85, TEXT_DRAW_ALIGN_LEFT, 0x6EC1FFFF);
    gKosHud[playerid][3] = KosHudTd(playerid, 410.0, 24.0, 0.85, TEXT_DRAW_ALIGN_RIGHT, 0xFF7070FF);
    gKosHud[playerid][4] = KosHudTd(playerid, 320.0, 418.0, 1.0, TEXT_DRAW_ALIGN_CENTER, 0xFFFFFFFF);
    PlayerTextDrawUseBox(playerid, gKosHud[playerid][4], true);
    PlayerTextDrawBoxColour(playerid, gKosHud[playerid][4], 0x0B1622B4);
    PlayerTextDrawTextSize(playerid, gKosHud[playerid][4], 0.0, 300.0);
    KosKontrole(playerid);
    for (new i = 0; i < KOS_HUD; i++) PlayerTextDrawShow(playerid, gKosHud[playerid][i]);
}

static KosKontrole(playerid)
{
    if (gKosTeren[playerid] == -1) return;
    PlayerTextDrawSetString(playerid, gKosHud[playerid][4], T(playerid, gKosDrzi[playerid] != -1 ? ("~y~LMB ~w~sut   ~y~N ~w~dodaj   ~y~ALT ~w~ispusti") : ("~y~ALT ~w~uzmi / ukradi loptu")));
}

static KosHudOsvjezi(t)
{
    new popis[2][256], ime[24];
    for (new p = 0, pZadnji = gIgracZadnji; p <= pZadnji; p++)
    {
        if (gKosTeren[p] != t) continue;
        format(ime, sizeof ime, "%s", GetRPIme(p));
        TdCisti(ime);
        if (popis[gKosTim[p]][0]) strcat(popis[gKosTim[p]], "~n~");
        strcat(popis[gKosTim[p]], ime);
    }
    for (new i = 0; i < 2; i++) if (!popis[i][0]) strcat(popis[i], "-");
    for (new p = 0, pZadnji = gIgracZadnji; p <= pZadnji; p++)
    {
        if (gKosTeren[p] != t) continue;
        new nas = gKosTim[p], naslov[96];
        format(naslov, sizeof naslov, "%s%s%s%s  ~w~%d : %d  %s%s%s%s", gKosTimBoja[0], nas == 0 ? ("[") : (""), T(p, gKosTimIme[0]), nas == 0 ? ("]") : (""), gKos[t][koBod][0], gKos[t][koBod][1], gKosTimBoja[1], nas == 1 ? ("[") : (""), T(p, gKosTimIme[1]), nas == 1 ? ("]") : (""));
        PlayerTextDrawSetString(p, gKosHud[p][1], naslov);
        PlayerTextDrawSetString(p, gKosHud[p][2], popis[0]);
        PlayerTextDrawSetString(p, gKosHud[p][3], popis[1]);
    }
}

static bool:KosPridruzi(playerid, t, zeljeni = -1)
{
    if (gKosTeren[playerid] == t) return true;
    if (gKosTeren[playerid] != -1) KosNapusti(playerid, true);
    new a = KosBroj(t, 0), b = KosBroj(t, 1), tim = zeljeni != -1 ? zeljeni : a <= b ? 0 : 1;
    if ((tim == 0 ? a : b) >= KOS_TIM_MAX) tim = 1 - tim;
    if ((tim == 0 ? a : b) >= KOS_TIM_MAX) return IgraObavijest(playerid, T(playerid, "Kosarka"), OBAVIJEST_GRESKA, T(playerid, "Oba tima su puna.")), false;
    if (!OkladaUdi(playerid, OK_KOSARKA, t)) return false;
    gKosTeren[playerid] = t;
    gKosTim[playerid] = tim;
    KosHudStvori(playerid);
    KosHudOsvjezi(t);
    new tekst[64];
    format(tekst, sizeof tekst, "%s%s", gKosTimBoja[tim], T(playerid, gKosTimIme[tim]));
    GameTextForPlayer(playerid, tekst, 2000, 6);
    new poruka[160];
    format(poruka, sizeof poruka, T(playerid, "Igrate za tim %s. Prvi do %d poena pobjeduje. Udaljite se s terena za izlaz."), T(playerid, gKosTimIme[tim]), KOS_POBJEDA);
    IgraObavijest(playerid, T(playerid, "Kosarka"), KOS_BOJA, poruka, 0);
    return true;
}

static KosNapusti(playerid, bool:osvjezi)
{
    new t = gKosTeren[playerid];
    if (t == -1) return;
    gKosTeren[playerid] = -1;
    if (IsPlayerConnected(playerid)) for (new i = 0; i < KOS_HUD; i++) PlayerTextDrawDestroy(playerid, gKosHud[playerid][i]);
    if (!osvjezi)
    {
        OkladaIzadi(playerid, false);
        return;
    }
    new a = KosBroj(t, 0), b = KosBroj(t, 1);
    if (a + b == 0)
    {
        OkladaIzadi(playerid, true);
        OkladaZatvori(OK_KOSARKA, t);
        gKos[t][koBod][0] = gKos[t][koBod][1] = 0;
        return;
    }
    OkladaIzadi(playerid, false);
    if (OkladaBanka(OK_KOSARKA, t) && (!a || !b)) KosPobjeda(t, a ? 0 : 1);
    else KosHudOsvjezi(t);
}

KosOkladaTeren(playerid)
{
    new t = gKosTeren[playerid];
    if (t == -1) return -1;
    return KosBroj(t, 0) + KosBroj(t, 1) == 1 ? t : -2 - t;
}

static KosPobjeda(t, tim)
{
    new pobjednici[KOS_TIM_MAX], n = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gKosTeren[i] != t) continue;
        if (gKosTim[i] == tim && n < sizeof pobjednici) pobjednici[n++] = i;
        new tekst[64];
        format(tekst, sizeof tekst, gKosTim[i] == tim ? ("~g~%s") : ("~r~%s"), T(i, gKosTim[i] == tim ? ("POBJEDA!") : ("PORAZ")));
        GameTextForPlayer(i, tekst, 3000, 3);
        new poruka[160];
        format(poruka, sizeof poruka, T(i, "[Kosarka] Tim %s je pobijedio %d : %d! Nova utakmica pocinje."), T(i, gKosTimIme[tim]), gKos[t][koBod][tim], gKos[t][koBod][1 - tim]);
        IgraObavijest(i, T(i, "Kosarka"), KOS_BOJA, poruka);
    }
    OkladaIsplati(OK_KOSARKA, t, pobjednici, n);
    gKos[t][koBod][0] = gKos[t][koBod][1] = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gKosTeren[i] != t || OkladaUdi(i, OK_KOSARKA, t, false)) continue;
        if (gKosDrzi[i] != -1) KosIspusti(i);
        KosNapusti(i, false);
        IgraObavijest(i, T(i, "Kosarka"), KOS_BOJA, T(i, "Napustili ste kosarkaski teren."));
    }
    KosHudOsvjezi(t);
}

static KosLuk(k, Float:ax, Float:ay, Float:az, Float:bx, Float:by, Float:bz, Float:vis, koraka)
{
    for (new i = 1; i <= koraka && gKos[k][koPn] < KOS_TOCAKA; i++)
    {
        new Float:t = float(i) / float(koraka), n = gKos[k][koPn]++;
        gKosPut[k][n][0] = ax + (bx - ax) * t;
        gKosPut[k][n][1] = ay + (by - ay) * t;
        gKosPut[k][n][2] = az + (bz - az) * t + 4.0 * vis * t * (1.0 - t);
    }
}

static KosZadnja(k, &Float:x, &Float:y, &Float:z)
{
    new n = gKos[k][koPn] - 1;
    x = gKosPut[k][n][0];
    y = gKosPut[k][n][1];
    z = gKosPut[k][n][2];
}

static KosPusti(playerid)
{
    new k = gKosDrzi[playerid];
    if (k == -1) return -1;
    RemovePlayerAttachedObject(playerid, KOS_SLOT);
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) ClearAnimations(playerid);
    gKosHod[playerid] = -1;
    gKosDrzi[playerid] = -1;
    KosKontrole(playerid);
    if (gKosNabija[playerid])
    {
        gKosNabija[playerid] = false;
        PlayerTextDrawDestroy(playerid, gKosTd[playerid][0]);
        PlayerTextDrawDestroy(playerid, gKosTd[playerid][1]);
    }
    gKosDrzi[playerid] = -1;
    gKos[k][koNosi] = INVALID_PLAYER_ID;
    new Float:x, Float:y, Float:z, Float:a, Float:fx, Float:fy;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    KosNaprijed(a, fx, fy);
    gKos[k][koLx] = x + fx * 0.5;
    gKos[k][koLy] = y + fy * 0.5;
    gKos[k][koLz] = z + 0.6;
    gKos[k][koPn] = gKos[k][koPi] = 0;
    gKos[k][koPogodak] = -1;
    gKos[k][koPrima] = INVALID_PLAYER_ID;
    gKos[k][koStrijelac] = playerid;
    gKos[k][koMetaTeren] = -1;
    SetDynamicObjectPos(gKos[k][koLopta], gKos[k][koLx], gKos[k][koLy], gKos[k][koLz]);
    return k;
}

static KosKreni(k)
{
    gKos[k][koFaza] = KF_LET;
    gKos[k][koToken]++;
    KosKorak(k, gKos[k][koToken]);
}

forward KosKorak(k, token);
public KosKorak(k, token)
{
    if (token != gKos[k][koToken] || gKos[k][koFaza] != KF_LET) return;
    new p = gKos[k][koPi]++;
    if (p >= gKos[k][koPn])
    {
        KosSlijetanje(k);
        return;
    }
    new Float:ox = p ? gKosPut[k][p - 1][0] : gKos[k][koLx], Float:oy = p ? gKosPut[k][p - 1][1] : gKos[k][koLy], Float:oz = p ? gKosPut[k][p - 1][2] : gKos[k][koLz];
    new Float:d = VectorSize(gKosPut[k][p][0] - ox, gKosPut[k][p][1] - oy, gKosPut[k][p][2] - oz);
    gKos[k][koRot] += d * 160.0;
    MoveDynamicObject(gKos[k][koLopta], gKosPut[k][p][0], gKosPut[k][p][1], gKosPut[k][p][2], d < 0.01 ? 0.1 : d * 1000.0 / KOS_KORAK_MS, gKos[k][koRot], 0.0, 0.0);
    if (p == gKos[k][koPogodak]) KosObjavi(k);
    SetTimerEx("KosKorak", KOS_KORAK_MS, false, "ii", k, token);
}

static KosObjavi(k)
{
    new s = gKos[k][koStrijelac], t = gKos[k][koMetaTeren];
    if (!IsPlayerConnected(s) || t == -1) return;
    new h = gKos[k][koMeta];
    if (gKos[h][koPar] != -1 && gKosTeren[s] == t && (gKos[h][koTeren] == h ? 0 : 1) == gKosTim[s])
    {
        GameTextForPlayer(s, T(s, "~r~Krivi kos!"), 1500, 6);
        return;
    }
    new Float:x = gKos[t][koX], Float:y = gKos[t][koY], Float:z = gKos[t][koZ];
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i] || GetPlayerVirtualWorld(i) != gKos[k][koSvijet] || !IsPlayerInRangeOfPoint(i, 30.0, x, y, z)) continue;
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
        if (gKosTeren[i] != -1) continue;
        new poruka[160];
        format(poruka, sizeof poruka, T(i, "[Kosarka] %s je zabio za %d poena!"), GetRPIme(s), gKos[k][koPoeni]);
        IgraObavijest(i, T(i, "Kosarka"), KOS_BOJA, poruka);
    }
    GameTextForPlayer(s, gKos[k][koPoeni] == 3 ? ("~y~+3") : ("~g~+2"), 1500, 6);
    if (gKosDrzi[s] == -1 && GetPlayerState(s) == PLAYER_STATE_ONFOOT) KosAnim(s, "BSKTBALL", "BBALL_react_score", false, false, false, 1200);
    if (gKosTeren[s] != t) return;
    new tim = gKosTim[s];
    gKos[t][koBod][tim] += gKos[k][koPoeni];
    if (gKos[t][koBod][tim] < KOS_POBJEDA)
    {
        KosHudOsvjezi(t);
        return;
    }
    KosPobjeda(t, tim);
}

static KosSlijetanje(k)
{
    KosZadnja(k, gKos[k][koLx], gKos[k][koLy], gKos[k][koLz]);
    gKos[k][koFaza] = KF_SLOBODNA;
    if (gKos[k][koRot] > 3600.0)
    {
        gKos[k][koRot] = 0.0;
        SetDynamicObjectRot(gKos[k][koLopta], 0.0, 0.0, 0.0);
    }
    new q = gKos[k][koPrima];
    gKos[k][koPrima] = INVALID_PLAYER_ID;
    if (q != INVALID_PLAYER_ID && IsPlayerConnected(q) && gKosDrzi[q] == -1 && GetPlayerState(q) == PLAYER_STATE_ONFOOT && GetPlayerVirtualWorld(q) == gKos[k][koSvijet] && IsPlayerInRangeOfPoint(q, 2.5, gKos[k][koLx], gKos[k][koLy], gKos[k][koLz])) KosUhvati(q, k, false);
    else if (VectorSize(gKos[k][koLx] - gKos[k][koSx], gKos[k][koLy] - gKos[k][koSy], 0.0) > gKos[k][koR] + KOS_IZLAZ) KosLoptaReset(k);
}

static KosUhvati(playerid, k, bool:sagni)
{
    gKos[k][koToken]++;
    gKos[k][koFaza] = KF_NOSI;
    gKos[k][koNosi] = playerid;
    gKosDrzi[playerid] = k;
    StopDynamicObject(gKos[k][koLopta]);
    SetDynamicObjectPos(gKos[k][koLopta], gKos[k][koX], gKos[k][koY], gKos[k][koZ] - 20.0);
    SetPlayerAttachedObject(playerid, KOS_SLOT, KOS_LOPTA, 6, 0.35, 0.0, 0.0);
    KosKontrole(playerid);
    if (sagni) KosAnim(playerid, "BSKTBALL", "BBALL_pickup", false, false, false, 700);
    else KosAnim(playerid, "BSKTBALL", "BBALL_idle", false, false, false, 400);
}

static KosIspusti(playerid)
{
    new k = KosPusti(playerid);
    if (k == -1) return;
    new Float:fx, Float:fy, Float:a, Float:gz = gKos[k][koZ] + KOS_LOPTA_Z;
    GetPlayerFacingAngle(playerid, a);
    KosNaprijed(a, fx, fy);
    KosLuk(k, gKos[k][koLx], gKos[k][koLy], gKos[k][koLz], gKos[k][koLx] + fx * 0.3, gKos[k][koLy] + fy * 0.3, gz, 0.1, 3);
    KosLuk(k, gKos[k][koLx] + fx * 0.3, gKos[k][koLy] + fy * 0.3, gz, gKos[k][koLx] + fx * 0.8, gKos[k][koLy] + fy * 0.8, gz, 0.5, 4);
    KosLuk(k, gKos[k][koLx] + fx * 0.8, gKos[k][koLy] + fy * 0.8, gz, gKos[k][koLx] + fx * 1.2, gKos[k][koLy] + fy * 1.2, gz, 0.2, 3);
    KosKreni(k);
}

static KosMeta(playerid)
{
    new Float:px, Float:py, Float:pz, Float:a, Float:fx, Float:fy, najbolji = -1, Float:najblize = KOS_DOMET;
    GetPlayerPos(playerid, px, py, pz);
    GetPlayerFacingAngle(playerid, a);
    KosNaprijed(a, fx, fy);
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (!gKos[k][koIma] || gKos[k][koSvijet] != GetPlayerVirtualWorld(playerid)) continue;
        new Float:rx, Float:ry, Float:rz;
        KosObruc(k, rx, ry, rz);
        new Float:d = VectorSize(rx - px, ry - py, 0.0);
        if (d >= najblize || (d > 1.0 && ((rx - px) * fx + (ry - py) * fy) / d < 0.35)) continue;
        najblize = d;
        najbolji = k;
    }
    return najbolji;
}

static KosSutni(playerid, Float:snaga)
{
    new t = KosMeta(playerid), k = KosPusti(playerid);
    if (k == -1) return;
    new Float:sx = gKos[k][koLx], Float:sy = gKos[k][koLy], Float:sz = gKos[k][koLz] + 0.4, Float:gz = gKos[k][koZ] + KOS_LOPTA_Z;
    if (t == -1)
    {
        new Float:a, Float:fx, Float:fy;
        GetPlayerFacingAngle(playerid, a);
        KosNaprijed(a, fx, fy);
        KosAnim(playerid, "BSKTBALL", "BBALL_Jump_Shot", false, true, false, 900);
        KosLuk(k, sx, sy, sz, sx + fx * 6.0, sy + fy * 6.0, gz, 2.5, 9);
        KosLuk(k, sx + fx * 6.0, sy + fy * 6.0, gz, sx + fx * 7.2, sy + fy * 7.2, gz, 0.5, 4);
        KosLuk(k, sx + fx * 7.2, sy + fy * 7.2, gz, sx + fx * 7.8, sy + fy * 7.8, gz, 0.2, 3);
        KosKreni(k);
        return;
    }
    new Float:rx, Float:ry, Float:rz, Float:hx, Float:hy;
    KosObruc(t, rx, ry, rz);
    KosNaprijed(gKos[t][koA], hx, hy);
    gKos[k][koMetaTeren] = gKos[t][koTeren];
    gKos[k][koMeta] = t;
    new Float:d = VectorSize(rx - sx, ry - sy, 0.0), koraka = floatround(d * 1.2);
    if (koraka < 5) koraka = 5;
    if (koraka > 12) koraka = 12;
    new bool:zakucaj = d < KOS_ZAKUCAJ, Float:vis = zakucaj ? 0.3 : 1.0 + d * 0.18, Float:tz = gKos[t][koZ] + KOS_LOPTA_Z;
    if (zakucaj) KosAnim(playerid, "BSKTBALL", "BBALL_Dnk", false, true, false, 1100);
    else KosAnim(playerid, "BSKTBALL", "BBALL_Jump_Shot", false, true, false, 900);
    if (random(100) < KosSansa(d, snaga))
    {
        KosLuk(k, sx, sy, sz, rx, ry, rz + 0.12, vis, zakucaj ? 3 : koraka);
        gKos[k][koPogodak] = gKos[k][koPn] - 1;
        gKos[k][koPoeni] = d > KOS_TRICA ? 3 : 2;
        KosLuk(k, rx, ry, rz + 0.12, rx, ry, rz - 0.6, 0.0, 2);
        KosLuk(k, rx, ry, rz - 0.6, rx + hx * 0.3, ry + hy * 0.3, tz, 0.0, 2);
        KosLuk(k, rx + hx * 0.3, ry + hy * 0.3, tz, rx + hx * 1.3, ry + hy * 1.3, tz, 0.5, 4);
        KosLuk(k, rx + hx * 1.3, ry + hy * 1.3, tz, rx + hx * 1.8, ry + hy * 1.8, tz, 0.2, 3);
    }
    else
    {
        new Float:ox = rx + float(random(51) - 25) / 100.0, Float:oy = ry + float(random(51) - 25) / 100.0, Float:kut = atan2(hy, hx) + float(random(141) - 70), Float:dalj = 1.5 + float(random(21)) / 10.0;
        new Float:bx = rx + floatcos(kut, degrees) * dalj, Float:by = ry + floatsin(kut, degrees) * dalj, Float:ux = (bx - rx) / dalj, Float:uy = (by - ry) / dalj;
        KosLuk(k, sx, sy, sz, ox, oy, rz + 0.1, vis, zakucaj ? 3 : koraka);
        KosLuk(k, ox, oy, rz + 0.1, bx, by, tz, 1.0, 6);
        KosLuk(k, bx, by, tz, bx + ux * 0.9, by + uy * 0.9, tz, 0.4, 3);
        KosLuk(k, bx + ux * 0.9, by + uy * 0.9, tz, bx + ux * 1.3, by + uy * 1.3, tz, 0.15, 2);
        SetTimerEx("KosPromasaj", KOS_KORAK_MS * gKos[k][koPn] / 2, false, "i", playerid);
    }
    KosKreni(k);
}

forward KosPromasaj(playerid);
public KosPromasaj(playerid)
{
    if (IsPlayerConnected(playerid) && gKosDrzi[playerid] == -1 && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && GetTickCount() - gKosAnimDo[playerid] >= 0) KosAnim(playerid, "BSKTBALL", "BBALL_react_miss", false, false, false, 1200);
}

static KosDodaj(playerid)
{
    new Float:px, Float:py, Float:pz, Float:a, Float:fx, Float:fy, q = INVALID_PLAYER_ID, Float:najblize = KOS_DODAJ;
    GetPlayerPos(playerid, px, py, pz);
    GetPlayerFacingAngle(playerid, a);
    KosNaprijed(a, fx, fy);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (i == playerid || gKosTeren[i] != gKosTeren[playerid] || gKosTim[i] != gKosTim[playerid] || gKosDrzi[i] != -1 || GetPlayerState(i) != PLAYER_STATE_ONFOOT || GetPlayerVirtualWorld(i) != GetPlayerVirtualWorld(playerid)) continue;
        new Float:x, Float:y, Float:z;
        GetPlayerPos(i, x, y, z);
        new Float:d = VectorSize(x - px, y - py, z - pz);
        if (d >= najblize || d < 0.5 || ((x - px) * fx + (y - py) * fy) / d < 0.5) continue;
        najblize = d;
        q = i;
    }
    if (q == INVALID_PLAYER_ID) return IgraObavijest(playerid, T(playerid, "Kosarka"), OBAVIJEST_GRESKA, T(playerid, "Nema suigraca ispred vas.")), 0;
    new Float:x, Float:y, Float:z, koraka = floatround(najblize);
    GetPlayerPos(q, x, y, z);
    new k = KosPusti(playerid);
    if (koraka < 3) koraka = 3;
    if (koraka > 10) koraka = 10;
    KosAnim(playerid, "GRENADE", "WEAPON_throwu", false, false, false, 600);
    KosLuk(k, gKos[k][koLx], gKos[k][koLy], gKos[k][koLz], x, y, z + 0.3, 0.3 + najblize * 0.06, koraka);
    gKos[k][koPrima] = q;
    KosKreni(k);
    return 1;
}

static KosUkradi(playerid)
{
    if (GetTickCount() - gKosKradja[playerid] < KOS_KRADJA_MS) return 0;
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    for (new k = 0; k < KOS_MAX; k++)
    {
        new h = gKos[k][koNosi];
        if (!gKos[k][koIma] || gKos[k][koFaza] != KF_NOSI || h == playerid || GetPlayerVirtualWorld(h) != GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(h, KOS_UKRADI, px, py, pz)) continue;
        new t = gKosTeren[h];
        if (t != -1 && gKosTeren[playerid] == t && gKosTim[playerid] == gKosTim[h]) continue;
        if (t != -1 && !KosPridruzi(playerid, t, 1 - gKosTim[h])) return 1;
        gKosKradja[playerid] = GetTickCount();
        KosAnim(playerid, "BSKTBALL", "BBALL_def_loop", false, false, false, 700);
        if (random(100) >= 35) return GameTextForPlayer(playerid, T(playerid, "~r~Promasaj"), 1000, 6), 1;
        KosPusti(h);
        KosAnim(h, "BSKTBALL", "BBALL_SkidStop_R", false, false, false, 700);
        KosUhvati(playerid, k, false);
        new poruka[96];
        format(poruka, sizeof poruka, T(h, "%s vam je ukrao loptu!"), GetRPIme(playerid));
        IgraObavijest(h, T(h, "Kosarka"), OBAVIJEST_GRESKA, poruka);
        GameTextForPlayer(playerid, T(playerid, "~g~Ukradeno!"), 1000, 6);
        return 1;
    }
    return 0;
}

KosKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    if (gKosDrzi[playerid] == -1) return 0;
    KosIspusti(playerid);
    return 1;
}

bool:KosPodigni(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || gKosDrzi[playerid] != -1) return false;
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (!gKos[k][koIma] || gKos[k][koFaza] != KF_SLOBODNA || gKos[k][koSvijet] != GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, KOS_UZMI, gKos[k][koLx], gKos[k][koLy], gKos[k][koLz] + 0.8)) continue;
        if (!KosPridruzi(playerid, gKos[k][koTeren])) return true;
        KosUhvati(playerid, k, true);
        return true;
    }
    if (KosUkradi(playerid)) return true;
    if (gKosTeren[playerid] != -1) return false;
    for (new k = 0; k < KOS_MAX; k++)
    {
        new t = gKos[k][koTeren];
        if (!gKos[k][koIma] || t != k || gKos[t][koSvijet] != GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, gKos[t][koR], gKos[t][koSx], gKos[t][koSy], gKos[t][koSz] + 1.0)) continue;
        KosPridruzi(playerid, t);
        return true;
    }
    return false;
}

forward KosHod();
public KosHod()
{
    new sad = GetTickCount();
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (!gKos[k][koIma] || gKos[k][koFaza] != KF_NOSI) continue;
        new p = gKos[k][koNosi];
        if (gKosNabija[p] || sad - gKosAnimDo[p] < 0 || GetPlayerState(p) != PLAYER_STATE_ONFOOT) continue;
        new KEY:tipke, gore, strana;
        GetPlayerKeys(p, tipke, gore, strana);
        new hod = !gore && !strana ? KH_STOJI : (tipke & KEY_SPRINT) ? KH_TRCI : KH_HODA, lib[16], ime[32];
        GetAnimationName(GetPlayerAnimationIndex(p), lib, sizeof lib, ime, sizeof ime);
        if (strcmp(lib, "BSKTBALL", true)) gKosHod[p] = -1;
        if (hod == gKosHod[p]) continue;
        gKosHod[p] = hod;
        if (hod == KH_STOJI) ApplyAnimation(p, "BSKTBALL", "BBALL_idleloop", 4.1, true, false, false, false, 0);
        else ApplyAnimation(p, "BSKTBALL", hod == KH_TRCI ? ("BBALL_run") : ("BBALL_walk"), 4.1, true, true, true, true, 1);
    }
}

forward KosNabijaTick(playerid);
public KosNabijaTick(playerid)
{
    if (!gKosNabija[playerid] || gKosDrzi[playerid] == -1) return;
    gKosSnaga[playerid] += gKosGore[playerid] ? 5.0 : -5.0;
    if (gKosSnaga[playerid] >= 100.0) gKosSnaga[playerid] = 100.0, gKosGore[playerid] = false;
    if (gKosSnaga[playerid] <= 0.0) gKosSnaga[playerid] = 0.0, gKosGore[playerid] = true;
    SnagaBarPostavi(playerid, gKosTd[playerid][1], gKosSnaga[playerid], 62.5, 87.5);
    SetTimerEx("KosNabijaTick", KOS_NABIJA_MS, false, "i", playerid);
}

KosTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    if (gKosDrzi[playerid] == -1) return 0;
    if ((newkeys & KEY_NO) && !(oldkeys & KEY_NO)) return KosDodaj(playerid), 1;
    if ((newkeys & KEY_FIRE) && !(oldkeys & KEY_FIRE))
    {
        new t = KosMeta(playerid);
        if (t != -1)
        {
            new Float:rx, Float:ry, Float:rz;
            KosObruc(t, rx, ry, rz);
            if (IsPlayerInRangeOfPoint(playerid, KOS_ZAKUCAJ, rx, ry, rz - 2.0))
            {
                KosSutni(playerid, 75.0);
                return 1;
            }
        }
        gKosNabija[playerid] = true;
        gKosSnaga[playerid] = 0.0;
        gKosGore[playerid] = true;
        SnagaBarStvori(playerid, gKosTd[playerid][0], gKosTd[playerid][1]);
        KosAnim(playerid, "BSKTBALL", "BBALL_idleloop", true, false, true, 0);
        KosNabijaTick(playerid);
        return 1;
    }
    if (gKosNabija[playerid] && !(newkeys & KEY_FIRE) && (oldkeys & KEY_FIRE))
    {
        KosSutni(playerid, gKosSnaga[playerid]);
        return 1;
    }
    return 0;
}

KosOdlazi(playerid)
{
    if (gKosDrzi[playerid] != -1) KosIspusti(playerid);
    KosNapusti(playerid, true);
}

KosTick()
{
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (!gKos[k][koIma] || gKos[k][koFaza] != KF_NOSI) continue;
        new p = gKos[k][koNosi];
        if (GetPlayerVirtualWorld(p) != gKos[k][koSvijet] || !IsPlayerInRangeOfPoint(p, gKos[k][koR] + KOS_IZLAZ, gKos[k][koSx], gKos[k][koSy], gKos[k][koSz] + 1.0))
        {
            KosPusti(p);
            KosLoptaReset(k);
            IgraObavijest(p, T(p, "Kosarka"), KOS_BOJA, T(p, "Lopta je vracena na teren."));
        }
        else if (GetPlayerState(p) != PLAYER_STATE_ONFOOT) KosIspusti(p);
    }
    new sad = GetTickCount();
    for (new p = 0, pZadnji = gIgracZadnji; p <= pZadnji; p++)
    {
        if (!gUlogiran[p] || gKosTeren[p] != -1 || sad - gKosHint[p] < 30000 || GetPlayerState(p) != PLAYER_STATE_ONFOOT) continue;
        for (new k = 0; k < KOS_MAX; k++)
        {
            if (!gKos[k][koIma] || gKos[k][koFaza] != KF_SLOBODNA || gKos[k][koSvijet] != GetPlayerVirtualWorld(p) || !IsPlayerInRangeOfPoint(p, KOS_UZMI * 2.0, gKos[k][koLx], gKos[k][koLy], gKos[k][koLz] + 0.8)) continue;
            gKosHint[p] = sad;
            Savjet(p, T(p, "~y~ALT~w~ - interakcija"), 2000);
            break;
        }
    }
    for (new p = 0, pZadnji = gIgracZadnji; p <= pZadnji; p++)
    {
        new t = gKosTeren[p];
        if (t == -1 || gKosDrzi[p] != -1) continue;
        if (GetPlayerVirtualWorld(p) == gKos[t][koSvijet] && IsPlayerInRangeOfPoint(p, gKos[t][koR] + KOS_IZLAZ, gKos[t][koSx], gKos[t][koSy], gKos[t][koSz] + 1.0)) continue;
        KosNapusti(p, true);
        IgraObavijest(p, T(p, "Kosarka"), KOS_BOJA, T(p, "Napustili ste kosarkaski teren."));
    }
}

ShowKosevi(playerid)
{
    new list[2048], red[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "+ Novi kos na mojoj poziciji (gledajte prema terenu)"));
    for (new k = 0; k < KOS_MAX; k++)
    {
        if (!gKos[k][koIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Kos (%s) - obrisi"), gKos[k][koDb], CestaZona(gKos[k][koX], gKos[k][koY]));
        DodajStupac(list, len, red);
        gApKos[playerid][n++] = k;
    }
    return PrikaziDialog(playerid, DLG_APANEL_KOS, DIALOG_STYLE_LIST, T(playerid, "Kosarkaski kosevi"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_kos(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new k = 0, Float:x, Float:y, Float:z, Float:a;
        while (k < KOS_MAX && gKos[k][koIma]) k++;
        if (k == KOS_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj koseva.")), ShowKosevi(playerid);
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        z -= 1.0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bb_hoops (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid)));
        gKos[k][koDb] = ZadnjiId();
        gKos[k][koX] = x;
        gKos[k][koY] = y;
        gKos[k][koZ] = z;
        gKos[k][koA] = a;
        gKos[k][koInt] = GetPlayerInterior(playerid);
        gKos[k][koSvijet] = GetPlayerVirtualWorld(playerid);
        KosStvori(k);
        KosTereni();
        new Float:fx, Float:fy;
        KosNaprijed(a, fx, fy);
        SetPlayerPos(playerid, x + fx * 4.0, y + fy * 4.0, z + 1.0);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kos je postavljen."));
        return ShowKosevi(playerid);
    }
    new i = listitem - 1;
    if (i < 0 || i >= KOS_MAX) return 1;
    new k = gApKos[playerid][i];
    if (!gKos[k][koIma]) return ShowKosevi(playerid);
    if (gKos[k][koFaza] == KF_NOSI) KosPusti(gKos[k][koNosi]);
    gKos[k][koToken]++;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bb_hoops WHERE id = %d", gKos[k][koDb]));
    DestroyDynamicObject(gKos[k][koObj]);
    DestroyDynamicObject(gKos[k][koLopta]);
    DestroyDynamic3DTextLabel(gKos[k][koTekst]);
    gKos[k][koLopta] = STREAMER_TAG_OBJECT:0;
    gKos[k][koIma] = false;
    KosTereni();
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kos je obrisan."));
    return ShowKosevi(playerid);
}
