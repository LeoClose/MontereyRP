#define TUNING_MAX          8
#define TUNING_DOMET        5.0
#define TUNING_SVIJET       9000
#define TUNING_REDOVA       8
#define TUNING_TD           40
#define TUNING_DIJELOVA     32
#define TUNING_X            30.0
#define TUNING_Y            110.0
#define TUNING_SIRINA       200.0
#define TUNING_RED_H        14.0
#define TUNING_POPRAVAK     400
#define TUNING_BOJA         300
#define TUNING_PAINTJOB     2500
#define TUNING_NEON         2000

enum { TK_POPRAVAK, TK_BOJA1, TK_BOJA2, TK_PAINTJOB, TK_MOTOR, TK_NEON, TK_DIO }
enum { TTD_PODNASLOV, TTD_BROJAC, TTD_KUPI, TTD_NATRAG, TTD_NOVAC, TTD_RED }
enum E_TUNING { bool:tgIma, tgDb, Float:tgX, Float:tgY, Float:tgZ, Float:tgA, STREAMER_TAG_3D_TEXT_LABEL:tgTekst, STREAMER_TAG_MAP_ICON:tgIkona }

static const gTuningDio[][] = { "Spojler", "Hauba", "Krov", "Pragovi", "Svjetla", "Nitro", "Auspuh", "Felge", "Stereo", "Hidraulika", "Prednji branik", "Straznji branik", "Desni ventil", "Lijevi ventil" };
static const gTuningDioCijena[] = { 1500, 1200, 1000, 1200, 400, 0, 800, 2000, 500, 3000, 1500, 1500, 600, 600 };
static const gTuningDioPogled[] = { 1, 4, 3, 2, 4, 1, 1, 2, 0, 2, 4, 1, 3, 3 };
static const gTuningMotorCijena[] = { 0, 5000, 10000, 20000 };
static const gTuningNeon[][] = { "Bez", "Crveni", "Plavi", "Zeleni", "Zuti", "Ruzicasti", "Bijeli" };
static const Float:gTuningPogled[][3] = { { -4.5, 5.0, 1.6 }, { 0.0, -6.5, 1.6 }, { -4.5, 0.0, 0.5 }, { 0.0, -3.5, 4.5 }, { 0.0, 6.5, 1.4 } };

static gTuning[TUNING_MAX][E_TUNING];
static gTunMod[MAX_VEHICLES][14], gTunPaint[MAX_VEHICLES], gTunNeon[MAX_VEHICLES], gTunMotor[MAX_VEHICLES], gTunBoja[MAX_VEHICLES][2], gTunNeonObj[MAX_VEHICLES][2];
static gTunU[MAX_PLAYERS], gTunV[MAX_PLAYERS], gTunKat[MAX_PLAYERS], gTunIdx[MAX_PLAYERS], gTunPoc[MAX_PLAYERS], gTunBroj[MAX_PLAYERS];
static gTunDijelovi[MAX_PLAYERS][TUNING_DIJELOVA], gTunKategorije[MAX_PLAYERS][TK_DIO + 14], gTunKatBroj[MAX_PLAYERS];
static PlayerText:gTunTd[MAX_PLAYERS][TUNING_TD], gTunTdBroj[MAX_PLAYERS], PlayerText:gTun[MAX_PLAYERS][TTD_RED + TUNING_REDOVA * 3];
static gApTuning[MAX_PLAYERS];

InitTuning()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS tuning_garages (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS vehicle_tuning (vehicle INTEGER PRIMARY KEY, mods TEXT NOT NULL DEFAULT '', paintjob INTEGER NOT NULL DEFAULT 3, neon INTEGER NOT NULL DEFAULT 0, motor INTEGER NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM tuning_garages ORDER BY id LIMIT %d", TUNING_MAX);
    for (new g = 0, n = DB_GetRowCount(res); g < n; g++, DB_SelectNextRow(res))
    {
        gTuning[g][tgDb] = DB_GetFieldInt(res, 0);
        gTuning[g][tgX] = DB_GetFieldFloat(res, 1);
        gTuning[g][tgY] = DB_GetFieldFloat(res, 2);
        gTuning[g][tgZ] = DB_GetFieldFloat(res, 3);
        gTuning[g][tgA] = DB_GetFieldFloat(res, 4);
        TuningStvori(g);
    }
    DB_FreeResultSet(res);
    for (new i = 0; i < MAX_PLAYERS; i++) gTunU[i] = -1;
}

static TuningStvori(g)
{
    gTuning[g][tgIma] = true;
    gTuning[g][tgTekst] = CreateDynamic3DTextLabel("Tuning\n[Y]", COLOR_ORG, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ] + 0.8, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gTuning[g][tgIkona] = CreateDynamicMapIcon(gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ], 27, 0, 0, 0);
}

TuningMotor(v)
{
    return v > 0 && v < MAX_VEHICLES ? gTunMotor[v] : 0;
}

static TuningNeon(v, boja)
{
    for (new i = 0; i < 2; i++)
    {
        if (gTunNeonObj[v][i]) DestroyObject(gTunNeonObj[v][i]);
        gTunNeonObj[v][i] = 0;
        if (!boja) continue;
        gTunNeonObj[v][i] = CreateObject(18646 + boja, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        AttachObjectToVehicle(gTunNeonObj[v][i], v, i ? 0.9 : -0.9, 0.0, -0.55, 0.0, 0.0, 0.0);
    }
}

static TuningVrati(v)
{
    for (new s = 0; s < 14; s++)
    {
        new c = GetVehicleComponentInSlot(v, CARMODTYPE:s);
        if (c && c != gTunMod[v][s]) RemoveVehicleComponent(v, c);
        if (gTunMod[v][s] && c != gTunMod[v][s]) AddVehicleComponent(v, gTunMod[v][s]);
    }
    ChangeVehiclePaintjob(v, gTunPaint[v]);
    ChangeVehicleColours(v, gTunBoja[v][0], gTunBoja[v][1]);
    TuningNeon(v, gTunNeon[v]);
}

TuningPrimijeni(v)
{
    new db = VlasnikDb(v);
    if (!db) return 0;
    for (new s = 0; s < 14; s++) gTunMod[v][s] = 0;
    gTunPaint[v] = 3;
    gTunNeon[v] = 0;
    gTunMotor[v] = 0;
    new mods[160], DBResult:res = DB_ExecuteQuery(gDB, "SELECT p.c1, p.c2, IFNULL(t.mods, ''), IFNULL(t.paintjob, 3), IFNULL(t.neon, 0), IFNULL(t.motor, 0) FROM player_vehicles p LEFT JOIN vehicle_tuning t ON t.vehicle = p.id WHERE p.id = %d", db);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        return 0;
    }
    gTunBoja[v][0] = DB_GetFieldInt(res, 0);
    gTunBoja[v][1] = DB_GetFieldInt(res, 1);
    DB_GetFieldString(res, 2, mods, sizeof mods);
    gTunPaint[v] = DB_GetFieldInt(res, 3);
    gTunNeon[v] = clamp(DB_GetFieldInt(res, 4), 0, sizeof gTuningNeon - 1);
    gTunMotor[v] = clamp(DB_GetFieldInt(res, 5), 0, sizeof gTuningMotorCijena - 1);
    DB_FreeResultSet(res);
    for (new i = 0, c = 0; ; i++)
    {
        if ('0' <= mods[i] <= '9')
        {
            c = c * 10 + mods[i] - '0';
            continue;
        }
        if (c >= 1000 && c <= 1193 && VehicleCanHaveComponent(GetVehicleModel(v), c)) gTunMod[v][_:GetVehicleComponentType(c)] = c;
        c = 0;
        if (!mods[i]) break;
    }
    TuningVrati(v);
    return 1;
}

TuningMakni(v)
{
    TuningNeon(v, 0);
    gTunMotor[v] = 0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM vehicle_tuning WHERE vehicle = %d", VlasnikDb(v)));
}

static TuningSpremi(v)
{
    new mods[160], dio[8];
    for (new s = 0; s < 14; s++)
    {
        if (!gTunMod[v][s]) continue;
        format(dio, sizeof dio, "%d ", gTunMod[v][s]);
        strcat(mods, dio);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO vehicle_tuning (vehicle, mods, paintjob, neon, motor) VALUES (%d, '%q', %d, %d, %d)", VlasnikDb(v), mods, gTunPaint[v], gTunNeon[v], gTunMotor[v]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE player_vehicles SET c1 = %d, c2 = %d WHERE id = %d", gTunBoja[v][0], gTunBoja[v][1], VlasnikDb(v)));
}

static TuningPaintjoba(model)
{
    switch (model)
    {
        case 483: return 1;
        case 575: return 2;
        case 534, 535, 536, 558, 559, 560, 561, 562, 565, 567, 576: return 3;
    }
    return 0;
}

static TuningNitroCijena(c)
{
    return c == 1010 ? 5000 : c == 1008 ? 3000 : 1500;
}

static PlayerText:TuningKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gTunTd[playerid][gTunTdBroj[playerid]++] = td;
    return td;
}

static PlayerText:TuningTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font = TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, font);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    gTunTd[playerid][gTunTdBroj[playerid]++] = td;
    return td;
}

static TuningKamera(playerid, pogled)
{
    new v = gTunV[playerid], Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    new Float:ox = gTuningPogled[pogled][0], Float:oy = gTuningPogled[pogled][1];
    SetPlayerCameraPos(playerid, x + ox * floatcos(a, degrees) - oy * floatsin(a, degrees), y + ox * floatsin(a, degrees) + oy * floatcos(a, degrees), z + gTuningPogled[pogled][2]);
    SetPlayerCameraLookAt(playerid, x, y, z + 0.3);
}

static TuningKatIme(playerid, kat, ime[], size = sizeof (ime))
{
    switch (kat)
    {
        case TK_POPRAVAK: format(ime, size, T(playerid, "Popravak karoserije"));
        case TK_BOJA1: format(ime, size, T(playerid, "Primarna boja"));
        case TK_BOJA2: format(ime, size, T(playerid, "Sekundarna boja"));
        case TK_PAINTJOB: format(ime, size, T(playerid, "Paintjob"));
        case TK_MOTOR: format(ime, size, T(playerid, "Motor"));
        case TK_NEON: format(ime, size, T(playerid, "Neon"));
        default: format(ime, size, T(playerid, gTuningDio[kat - TK_DIO]));
    }
}

static TuningStavka(playerid, i, &vrijednost, &cijena, ime[], size = sizeof (ime))
{
    new kat = gTunKat[playerid], v = gTunV[playerid];
    if (kat == -1)
    {
        vrijednost = gTunKategorije[playerid][i];
        cijena = -1;
        TuningKatIme(playerid, vrijednost, ime, size);
        return;
    }
    switch (kat)
    {
        case TK_POPRAVAK:
        {
            vrijednost = 0;
            cijena = TUNING_POPRAVAK;
            format(ime, size, T(playerid, "Popravi karoseriju"));
        }
        case TK_BOJA1, TK_BOJA2:
        {
            vrijednost = i;
            cijena = gTunBoja[v][kat - TK_BOJA1] == i ? 0 : TUNING_BOJA;
            format(ime, size, T(playerid, "Boja %d"), i);
        }
        case TK_PAINTJOB:
        {
            vrijednost = i ? i - 1 : 3;
            cijena = gTunPaint[v] == vrijednost || !i ? 0 : TUNING_PAINTJOB;
            if (i) format(ime, size, T(playerid, "Paintjob %d"), i);
            else format(ime, size, T(playerid, "Bez"));
        }
        case TK_MOTOR:
        {
            vrijednost = i;
            cijena = gTuningMotorCijena[i];
            if (i) format(ime, size, T(playerid, "Stupanj %d"), i);
            else format(ime, size, T(playerid, "Tvornicki"));
        }
        case TK_NEON:
        {
            vrijednost = i;
            cijena = i ? TUNING_NEON : 0;
            format(ime, size, T(playerid, gTuningNeon[i]));
        }
        default:
        {
            vrijednost = i ? gTunDijelovi[playerid][i - 1] : 0;
            cijena = !i ? 0 : kat - TK_DIO == 5 ? TuningNitroCijena(vrijednost) : gTuningDioCijena[kat - TK_DIO];
            if (i) format(ime, size, "%s %d", T(playerid, gTuningDio[kat - TK_DIO]), i);
            else format(ime, size, T(playerid, "Tvornicki"));
        }
    }
}

static bool:TuningUgradeno(playerid, vrijednost)
{
    new kat = gTunKat[playerid], v = gTunV[playerid];
    switch (kat)
    {
        case -1, TK_POPRAVAK: return false;
        case TK_BOJA1, TK_BOJA2: return gTunBoja[v][kat - TK_BOJA1] == vrijednost;
        case TK_PAINTJOB: return gTunPaint[v] == vrijednost;
        case TK_MOTOR: return gTunMotor[v] == vrijednost;
        case TK_NEON: return gTunNeon[v] == vrijednost;
    }
    return gTunMod[v][kat - TK_DIO] == vrijednost;
}

static TuningPrikazi(playerid)
{
    new ime[48], cijena, vrijednost, tekst[48];
    if (gTunKat[playerid] == -1) format(ime, sizeof ime, T(playerid, "KATEGORIJE"));
    else TuningKatIme(playerid, gTunKat[playerid], ime);
    PlayerTextDrawSetString(playerid, gTun[playerid][TTD_PODNASLOV], ime);
    format(tekst, sizeof tekst, "%d / %d", gTunIdx[playerid] + 1, gTunBroj[playerid]);
    PlayerTextDrawSetString(playerid, gTun[playerid][TTD_BROJAC], tekst);
    format(tekst, sizeof tekst, T(playerid, "Novac: ~g~$%d"), gNovac[playerid]);
    PlayerTextDrawSetString(playerid, gTun[playerid][TTD_NOVAC], tekst);
    for (new r = 0; r < TUNING_REDOVA; r++)
    {
        new i = gTunPoc[playerid] + r, PlayerText:pozadina = gTun[playerid][TTD_RED + r * 3], PlayerText:naziv = gTun[playerid][TTD_RED + r * 3 + 1], PlayerText:iznos = gTun[playerid][TTD_RED + r * 3 + 2];
        if (i >= gTunBroj[playerid])
        {
            PlayerTextDrawHide(playerid, pozadina);
            PlayerTextDrawHide(playerid, naziv);
            PlayerTextDrawHide(playerid, iznos);
            continue;
        }
        new bool:odabran = i == gTunIdx[playerid];
        TuningStavka(playerid, i, vrijednost, cijena, ime);
        if (cijena < 0) tekst = ">";
        else if (TuningUgradeno(playerid, vrijednost)) format(tekst, sizeof tekst, T(playerid, "Ugradeno"));
        else if (!cijena) tekst = "$0";
        else format(tekst, sizeof tekst, "$%d", cijena);
        PlayerTextDrawColour(playerid, pozadina, odabran ? 0xF0F0F0EE : 0x000000B4);
        PlayerTextDrawColour(playerid, naziv, odabran ? 0x000000FF : 0xFFFFFFFF);
        PlayerTextDrawColour(playerid, iznos, odabran ? 0x000000FF : 0xFFFFFFFF);
        PlayerTextDrawSetString(playerid, naziv, ime);
        PlayerTextDrawSetString(playerid, iznos, tekst);
        PlayerTextDrawShow(playerid, pozadina);
        PlayerTextDrawShow(playerid, naziv);
        PlayerTextDrawShow(playerid, iznos);
    }
    if (gTunKat[playerid] == -1) PlayerTextDrawHide(playerid, gTun[playerid][TTD_KUPI]);
    else PlayerTextDrawShow(playerid, gTun[playerid][TTD_KUPI]);
}

static TuningPregled(playerid)
{
    new v = gTunV[playerid], kat = gTunKat[playerid], vrijednost, cijena, ime[48];
    TuningVrati(v);
    if (kat == -1) return;
    TuningStavka(playerid, gTunIdx[playerid], vrijednost, cijena, ime);
    switch (kat)
    {
        case TK_BOJA1: ChangeVehicleColours(v, vrijednost, gTunBoja[v][1]);
        case TK_BOJA2: ChangeVehicleColours(v, gTunBoja[v][0], vrijednost);
        case TK_PAINTJOB: ChangeVehiclePaintjob(v, vrijednost);
        case TK_NEON: TuningNeon(v, vrijednost);
        case TK_POPRAVAK, TK_MOTOR: {}
        default:
        {
            new stari = GetVehicleComponentInSlot(v, CARMODTYPE:(kat - TK_DIO));
            if (stari) RemoveVehicleComponent(v, stari);
            if (vrijednost) AddVehicleComponent(v, vrijednost);
        }
    }
}

static TuningOtvoriKat(playerid, kat)
{
    new v = gTunV[playerid], model = GetVehicleModel(v);
    gTunKat[playerid] = kat;
    gTunIdx[playerid] = 0;
    gTunPoc[playerid] = 0;
    switch (kat)
    {
        case -1: gTunBroj[playerid] = gTunKatBroj[playerid];
        case TK_POPRAVAK: gTunBroj[playerid] = 1;
        case TK_BOJA1, TK_BOJA2: gTunBroj[playerid] = 256;
        case TK_PAINTJOB: gTunBroj[playerid] = TuningPaintjoba(model) + 1;
        case TK_MOTOR: gTunBroj[playerid] = sizeof gTuningMotorCijena;
        case TK_NEON: gTunBroj[playerid] = sizeof gTuningNeon;
        default:
        {
            new n = 0;
            for (new c = 1000; c <= 1193 && n < TUNING_DIJELOVA; c++)
                if (_:GetVehicleComponentType(c) == kat - TK_DIO && VehicleCanHaveComponent(model, c)) gTunDijelovi[playerid][n++] = c;
            gTunBroj[playerid] = n + 1;
        }
    }
    if (kat >= 0)
    {
        new vrijednost, cijena, ime[48];
        for (new i = 0; i < gTunBroj[playerid]; i++)
        {
            TuningStavka(playerid, i, vrijednost, cijena, ime);
            if (!TuningUgradeno(playerid, vrijednost)) continue;
            gTunIdx[playerid] = i;
            gTunPoc[playerid] = i >= TUNING_REDOVA ? i - TUNING_REDOVA + 1 : 0;
            break;
        }
    }
    TuningKamera(playerid, kat == -1 || kat < TK_DIO ? (kat == TK_NEON ? 2 : kat == TK_MOTOR ? 4 : 0) : gTuningDioPogled[kat - TK_DIO]);
    TuningPregled(playerid);
    TuningPrikazi(playerid);
}

static TuningIzbornik(playerid)
{
    new v = gTunV[playerid], model = GetVehicleModel(v), n = 0;
    gTunKategorije[playerid][n++] = TK_POPRAVAK;
    gTunKategorije[playerid][n++] = TK_BOJA1;
    gTunKategorije[playerid][n++] = TK_BOJA2;
    if (TuningPaintjoba(model)) gTunKategorije[playerid][n++] = TK_PAINTJOB;
    gTunKategorije[playerid][n++] = TK_MOTOR;
    gTunKategorije[playerid][n++] = TK_NEON;
    for (new s = 0; s < 14; s++)
    {
        for (new c = 1000; c <= 1193; c++)
        {
            if (_:GetVehicleComponentType(c) != s || !VehicleCanHaveComponent(model, c)) continue;
            gTunKategorije[playerid][n++] = TK_DIO + s;
            break;
        }
    }
    gTunKatBroj[playerid] = n;
}

static TuningOtvori(playerid, g)
{
    new v = GetPlayerVehicleID(playerid);
    gTunU[playerid] = g;
    gTunV[playerid] = v;
    gTunTdBroj[playerid] = 0;
    SetVehiclePos(v, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ]);
    SetVehicleZAngle(v, gTuning[g][tgA]);
    SetVehicleVelocity(v, 0.0, 0.0, 0.0);
    SetVehicleVirtualWorld(v, TUNING_SVIJET + playerid);
    SetPlayerVirtualWorld(playerid, TUNING_SVIJET + playerid);
    PutPlayerInVehicle(playerid, v, 0);
    TogglePlayerControllable(playerid, false);
    TuningKutija(playerid, TUNING_X, TUNING_Y, TUNING_SIRINA, 40.0, 0x1E5AA8F0);
    TuningTekst(playerid, TUNING_X + TUNING_SIRINA / 2.0, TUNING_Y + 6.0, "TUNING", 0xFFFFFFFF, 0.7, 2.8, TEXT_DRAW_FONT_3, TEXT_DRAW_ALIGN_CENTER);
    TuningKutija(playerid, TUNING_X, TUNING_Y + 40.0, TUNING_SIRINA, TUNING_RED_H, 0x000000F0);
    gTun[playerid][TTD_PODNASLOV] = TuningTekst(playerid, TUNING_X + 4.0, TUNING_Y + 42.0, "_", 0x5B8DBFFF, 0.2, 1.0, TEXT_DRAW_FONT_2);
    gTun[playerid][TTD_BROJAC] = TuningTekst(playerid, TUNING_X + TUNING_SIRINA - 4.0, TUNING_Y + 42.0, "_", 0xFFFFFFFF, 0.2, 1.0, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_RIGHT);
    for (new r = 0; r < TUNING_REDOVA; r++)
    {
        new Float:y = TUNING_Y + 40.0 + TUNING_RED_H * float(r + 1);
        gTun[playerid][TTD_RED + r * 3] = TuningKutija(playerid, TUNING_X, y, TUNING_SIRINA, TUNING_RED_H, 0x000000B4);
        new PlayerText:naziv = TuningTekst(playerid, TUNING_X + 5.0, y + 2.0, "_", 0xFFFFFFFF, 0.22, 1.05);
        PlayerTextDrawTextSize(playerid, naziv, TUNING_X + TUNING_SIRINA - 2.0, TUNING_RED_H - 2.0);
        PlayerTextDrawSetSelectable(playerid, naziv, true);
        gTun[playerid][TTD_RED + r * 3 + 1] = naziv;
        gTun[playerid][TTD_RED + r * 3 + 2] = TuningTekst(playerid, TUNING_X + TUNING_SIRINA - 5.0, y + 2.0, "_", 0xFFFFFFFF, 0.22, 1.05, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    }
    new Float:dno = TUNING_Y + 40.0 + TUNING_RED_H * float(TUNING_REDOVA + 1) + 4.0;
    TuningKutija(playerid, TUNING_X, dno, TUNING_SIRINA, 34.0, 0x000000D2);
    gTun[playerid][TTD_NOVAC] = TuningTekst(playerid, TUNING_X + 5.0, dno + 3.0, "_", 0xFFFFFFFF, 0.2, 1.0);
    gTun[playerid][TTD_KUPI] = TuningTekst(playerid, TUNING_X + 50.0, dno + 18.0, T(playerid, "KUPI"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gTun[playerid][TTD_KUPI], true);
    PlayerTextDrawBoxColour(playerid, gTun[playerid][TTD_KUPI], 0x2E7D32FF);
    PlayerTextDrawTextSize(playerid, gTun[playerid][TTD_KUPI], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gTun[playerid][TTD_KUPI], true);
    gTun[playerid][TTD_NATRAG] = TuningTekst(playerid, TUNING_X + 150.0, dno + 18.0, T(playerid, "NATRAG"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gTun[playerid][TTD_NATRAG], true);
    PlayerTextDrawBoxColour(playerid, gTun[playerid][TTD_NATRAG], 0x1E5AA8FF);
    PlayerTextDrawTextSize(playerid, gTun[playerid][TTD_NATRAG], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gTun[playerid][TTD_NATRAG], true);
    for (new i = 0; i < gTunTdBroj[playerid]; i++) PlayerTextDrawShow(playerid, gTunTd[playerid][i]);
    TuningIzbornik(playerid);
    TuningOtvoriKat(playerid, -1);
    SelectTextDraw(playerid, 0x5B8DBFFF);
}

TuningZatvori(playerid)
{
    new g = gTunU[playerid];
    if (g == -1) return;
    gTunU[playerid] = -1;
    new v = gTunV[playerid];
    for (new i = 0; i < gTunTdBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gTunTd[playerid][i]);
    gTunTdBroj[playerid] = 0;
    CancelSelectTextDraw(playerid);
    if (!GetVehicleModel(v)) return;
    TuningVrati(v);
    SetVehicleVirtualWorld(v, 0);
    if (!IsPlayerConnected(playerid)) return;
    SetPlayerVirtualWorld(playerid, 0);
    PutPlayerInVehicle(playerid, v, 0);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
}

TuningDisconnect(playerid)
{
    TuningZatvori(playerid);
    gTunU[playerid] = -1;
}

static TuningKupi(playerid)
{
    new v = gTunV[playerid], kat = gTunKat[playerid], vrijednost, cijena, ime[48];
    TuningStavka(playerid, gTunIdx[playerid], vrijednost, cijena, ime);
    if (TuningUgradeno(playerid, vrijednost)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "To je vec ugradeno."));
    if (cijena && !DajNovac(playerid, -cijena)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    switch (kat)
    {
        case TK_POPRAVAK: RepairVehicle(v);
        case TK_BOJA1, TK_BOJA2: gTunBoja[v][kat - TK_BOJA1] = vrijednost;
        case TK_PAINTJOB: gTunPaint[v] = vrijednost;
        case TK_MOTOR: gTunMotor[v] = vrijednost;
        case TK_NEON: gTunNeon[v] = vrijednost;
        default: gTunMod[v][kat - TK_DIO] = vrijednost;
    }
    if (kat != TK_POPRAVAK) TuningSpremi(v);
    PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
    TuningPregled(playerid);
    TuningPrikazi(playerid);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d)."), ime, cijena);
}

TuningKlik(playerid, PlayerText:td)
{
    if (gTunU[playerid] == -1) return 0;
    if (!GetVehicleModel(gTunV[playerid]) || GetPlayerVehicleID(playerid) != gTunV[playerid])
    {
        TuningZatvori(playerid);
        return 1;
    }
    if (td == gTun[playerid][TTD_NATRAG])
    {
        if (gTunKat[playerid] == -1) TuningZatvori(playerid);
        else
        {
            new kat = gTunKat[playerid];
            TuningOtvoriKat(playerid, -1);
            for (new i = 0; i < gTunKatBroj[playerid]; i++) if (gTunKategorije[playerid][i] == kat) gTunIdx[playerid] = i;
            gTunPoc[playerid] = gTunIdx[playerid] >= TUNING_REDOVA ? gTunIdx[playerid] - TUNING_REDOVA + 1 : 0;
            TuningPrikazi(playerid);
        }
        return 1;
    }
    if (td == gTun[playerid][TTD_KUPI])
    {
        if (gTunKat[playerid] != -1) TuningKupi(playerid);
        return 1;
    }
    for (new r = 0; r < TUNING_REDOVA; r++)
    {
        if (td != gTun[playerid][TTD_RED + r * 3 + 1]) continue;
        new i = gTunPoc[playerid] + r;
        if (i >= gTunBroj[playerid]) return 1;
        if (gTunKat[playerid] == -1)
        {
            TuningOtvoriKat(playerid, gTunKategorije[playerid][i]);
            return 1;
        }
        gTunIdx[playerid] = i;
        if (r == TUNING_REDOVA - 1 && i + 1 < gTunBroj[playerid]) gTunPoc[playerid] = min(gTunPoc[playerid] + TUNING_REDOVA - 1, gTunBroj[playerid] - TUNING_REDOVA);
        else if (!r && gTunPoc[playerid]) gTunPoc[playerid] = max(gTunPoc[playerid] - TUNING_REDOVA + 1, 0);
        TuningPregled(playerid);
        TuningPrikazi(playerid);
        return 1;
    }
    return 1;
}

TuningUlaz(playerid)
{
    if (!gUlogiran[playerid] || gTunU[playerid] != -1 || GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerVirtualWorld(playerid)) return 0;
    for (new g = 0; g < TUNING_MAX; g++)
    {
        if (!gTuning[g][tgIma] || !IsPlayerInRangeOfPoint(playerid, TUNING_DOMET, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ])) continue;
        new v = GetPlayerVehicleID(playerid);
        if (!VlasnikDb(v) || VlasnikVozila(v) != gAccountId[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Tuning je dostupan samo za vlastita vozila.")), 1;
        TuningOtvori(playerid, g);
        return 1;
    }
    return 0;
}

ShowTuningAdmin(playerid)
{
    new list[512], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova tuning garaza na mojoj poziciji"));
    for (new g = 0; g < TUNING_MAX; g++)
    {
        if (!gTuning[g][tgIma]) continue;
        format(row, sizeof row, "[%d] %s", gTuning[g][tgDb], CestaZona(gTuning[g][tgX], gTuning[g][tgY]));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_TUNING, DIALOG_STYLE_LIST, T(playerid, "Tuning garaze"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static TuningNova(playerid)
{
    new g = 0;
    while (g < TUNING_MAX && gTuning[g][tgIma]) g++;
    if (g == TUNING_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Tuning garaza se postavlja samo izvana."));
    new v = GetPlayerVehicleID(playerid);
    if (v)
    {
        GetVehiclePos(v, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ]);
        GetVehicleZAngle(v, gTuning[g][tgA]);
    }
    else
    {
        GetPlayerPos(playerid, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ]);
        GetPlayerFacingAngle(playerid, gTuning[g][tgA]);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO tuning_garages (x, y, z, a) VALUES (%f, %f, %f, %f)", gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ], gTuning[g][tgA]));
    gTuning[g][tgDb] = ZadnjiId();
    TuningStvori(g);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tuning garaza dodana: %s."), CestaZona(gTuning[g][tgX], gTuning[g][tgY]));
}

DLG:apanel_tuning(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        TuningNova(playerid);
        return ShowTuningAdmin(playerid);
    }
    new n = listitem;
    for (new g = 0; g < TUNING_MAX; g++)
    {
        if (!gTuning[g][tgIma] || --n) continue;
        gApTuning[playerid] = g;
        new body[96];
        format(body, sizeof body, T(playerid, "Obrisati tuning garazu: %s?"), CestaZona(gTuning[g][tgX], gTuning[g][tgY]));
        return PrikaziDialog(playerid, DLG_APANEL_TUNING_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Tuning garaze"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowTuningAdmin(playerid);
}

DLG:apanel_tuning_brisi(playerid, response)
{
    new g = gApTuning[playerid];
    if (response && ProvjeriOvlasti(playerid, ALVL_ORG) && g >= 0 && g < TUNING_MAX && gTuning[g][tgIma])
    {
        for (new i = 0; i < MAX_PLAYERS; i++) if (gTunU[i] == g) TuningZatvori(i);
        DestroyDynamic3DTextLabel(gTuning[g][tgTekst]);
        DestroyDynamicMapIcon(gTuning[g][tgIkona]);
        gTuning[g][tgIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM tuning_garages WHERE id = %d", gTuning[g][tgDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tuning garaza obrisana."));
    }
    return ShowTuningAdmin(playerid);
}
