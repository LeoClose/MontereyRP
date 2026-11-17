#define ARK_MAX             24
#define ARK_CIJENA          5
#define ARK_SW              14
#define ARK_SH              10
#define ARK_CELIJA          (ARK_SW * ARK_SH)
#define ARK_CEL             12.0
#define ARK_X               (320.0 - ARK_SW * ARK_CEL / 2.0)
#define ARK_Y               150.0
#define ARK_TD              (ARK_CELIJA + 7)
#define ARK_KRAJ_MS         5000
#define ARK_DOMET           1.6
#define ARK_TR              8
#define ARK_TR_H            20.0
#define ARK_TR_MS           80
#define ARK_AUTO            ((ARK_TR - 1) * 3)
#define ARK_ASFALT          0x3A3A3AFF
#define ARK_ZUTO            0xFFD32AFF
#define ARK_METE            5
#define ARK_PUC_ZDRAVLJE    100
#define ARK_PUC_STETA       20
#define ARK_PUC_TLO         100.0
#define ARK_PUC_FIG_W       44.0
#define ARK_PUC_FIG_H       80.0
#define ARK_PUC_FIG_Y       38.0
#define ARK_PUC_MS          60
#define ARK_PUC_W           220.0
#define ARK_PUC_H           150.0
#define ARK_PUC_X           (320.0 - ARK_PUC_W / 2.0)
#define ARK_PUC_META        3
#define ARK_PUC_FLASH       (ARK_PUC_META + ARK_METE + 1)
#define ARK_PUC_NISAN       (ARK_PUC_FLASH + 1)
#define ARK_PUC_POMAK       4.0
#define ARK_PUC_PAUZA       280

enum { AI_ZMIJA, AI_PUCANJE, AI_MEMORIJA, AI_TRAKE }
enum E_ARK { bool:amIma, amDb, Float:amX, Float:amY, Float:amZ, Float:amA, amInt, amSvijet, amIgra, STREAMER_TAG_OBJECT:amObj, STREAMER_TAG_3D_TEXT_LABEL:amTekst,
    amIgrac, amToken, amBod, amZivoti, amSmjer, amRep, amDuz, amHrana, amMeta, amMs, bool:amKraj }

static const gArkModel[] = { 2681, 2779, 2778, 2872 };
static const gArkIme[][] = { "Snake", "Streljana", "Memorija", "Dodger" };
static const gArkKauboj[] = { 34, 158, 161, 133 };
static const gArkCivil[] = { 10, 12, 101, 157 };
static const gArkSimonBoja[9] = { 0xFF4757FF, 0x2ED573FF, 0x1E90FFFF, 0xFFA502FF, 0xECCC68FF, 0xA55EEAFF, 0x00D2D3FF, 0xFF6B81FF, 0x7BED9FFF };
static const gArkSmjerX[] = { 0, 1, 0, -1 };
static const gArkSmjerY[] = { -1, 0, 1, 0 };

static gArk[ARK_MAX][E_ARK], gArkZmija[ARK_MAX][ARK_CELIJA], bool:gArkPolje[ARK_MAX][ARK_CELIJA];
static Float:gArkCx[ARK_MAX], Float:gArkCy[ARK_MAX], gArkCod[ARK_MAX], gArkMt[ARK_MAX][ARK_METE], gArkMe[ARK_MAX][ARK_METE], gArkMod[ARK_MAX][ARK_METE];
static gArkAutomat[MAX_PLAYERS] = { -1, ... }, PlayerText:gArkTd[MAX_PLAYERS][ARK_TD], gArkBr[MAX_PLAYERS], PlayerText:gArkBodTd[MAX_PLAYERS], PlayerText:gArkInfoTd[MAX_PLAYERS], gApArk[MAX_PLAYERS][ARK_MAX];

InitArkada()
{
    assert(gArkSmjerX[1] + gArkSmjerX[3] == 0 && gArkSmjerY[0] + gArkSmjerY[2] == 0);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS arcade_machines (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL, game INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS arcade_scores (machine INTEGER NOT NULL, account INTEGER NOT NULL, score INTEGER NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS arcade_scores_machine ON arcade_scores (machine, score DESC)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world, game FROM arcade_machines LIMIT %d", ARK_MAX);
    for (new m = 0, n = DB_GetRowCount(res); m < n; m++, DB_SelectNextRow(res))
    {
        gArk[m][amDb] = DB_GetFieldInt(res, 0);
        gArk[m][amX] = DB_GetFieldFloat(res, 1);
        gArk[m][amY] = DB_GetFieldFloat(res, 2);
        gArk[m][amZ] = DB_GetFieldFloat(res, 3);
        gArk[m][amA] = DB_GetFieldFloat(res, 4);
        gArk[m][amInt] = DB_GetFieldInt(res, 5);
        gArk[m][amSvijet] = DB_GetFieldInt(res, 6);
        gArk[m][amIgra] = clamp(DB_GetFieldInt(res, 7), AI_ZMIJA, AI_TRAKE);
        ArkStvori(m);
    }
    DB_FreeResultSet(res);
}

static ArkStvori(m)
{
    new tekst[64];
    format(tekst, sizeof tekst, "{F39C12}%s\n{FFFFFF}[Y] $%d", gArkIme[gArk[m][amIgra]], ARK_CIJENA);
    gArk[m][amIma] = true;
    gArk[m][amIgrac] = INVALID_PLAYER_ID;
    gArk[m][amObj] = CreateDynamicObject(gArkModel[gArk[m][amIgra]], gArk[m][amX], gArk[m][amY], gArk[m][amZ], 0.0, 0.0, gArk[m][amA], gArk[m][amSvijet], gArk[m][amInt]);
    gArk[m][amTekst] = CreateDynamic3DTextLabel(tekst, 0xFFFFFFFF, gArk[m][amX], gArk[m][amY], gArk[m][amZ] + 2.1, 6.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gArk[m][amSvijet], gArk[m][amInt]);
}

static PlayerText:ArkKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja, bool:klik = false)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "_");
    PlayerTextDrawLetterSize(playerid, td, 0.0, h / 9.0);
    PlayerTextDrawTextSize(playerid, td, x + w, y + h);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    if (klik) PlayerTextDrawSetSelectable(playerid, td, true);
    PlayerTextDrawShow(playerid, td);
    gArkTd[playerid][gArkBr[playerid]++] = td;
    return td;
}

static PlayerText:ArkTekst(playerid, Float:x, Float:y, const tekst[], Float:vel, boja, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_CENTER)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawLetterSize(playerid, td, vel * 0.25, vel);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawBackgroundColour(playerid, td, 0x000000AA);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawSetProportional(playerid, td, true);
    PlayerTextDrawShow(playerid, td);
    gArkTd[playerid][gArkBr[playerid]++] = td;
    return td;
}

static PlayerText:ArkSprite(playerid, Float:x, Float:y, Float:w, Float:h, const tex[], boja, bool:prikazi = true)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tex);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    if (prikazi) PlayerTextDrawShow(playerid, td);
    gArkTd[playerid][gArkBr[playerid]++] = td;
    return td;
}

static ArkPomakni(playerid, i, Float:x, Float:y, Float:w, Float:h)
{
    new PlayerText:td = gArkTd[playerid][i];
    PlayerTextDrawHide(playerid, td);
    PlayerTextDrawSetPos(playerid, td, ARK_PUC_X + x - w / 2.0, ARK_Y + y - h / 2.0);
    PlayerTextDrawShow(playerid, td);
}

static Float:ArkPucSpotX(i)
{
    return ARK_PUC_W * (float(i) + 0.5) / float(ARK_METE);
}

static ArkCelija(playerid, c, boja)
{
    PlayerTextDrawBoxColour(playerid, gArkTd[playerid][c + 1], boja);
    PlayerTextDrawShow(playerid, gArkTd[playerid][c + 1]);
}

static ArkBodovi(m)
{
    new p = gArk[m][amIgrac], tekst[64];
    if (gArk[m][amIgra] == AI_PUCANJE) format(tekst, sizeof tekst, "~w~%s: ~y~%d   ~w~%s: ~r~%d", T(p, "BODOVI"), gArk[m][amBod], T(p, "ZDRAVLJE"), gArk[m][amZivoti]);
    else if (gArk[m][amIgra] == AI_MEMORIJA) format(tekst, sizeof tekst, "~w~%s: ~y~%d   %s", T(p, "BODOVI"), gArk[m][amBod], gArk[m][amSmjer] ? T(p, "~g~TVOJ RED") : T(p, "~p~GLEDAJ"));
    else format(tekst, sizeof tekst, "~w~%s: ~y~%d", T(p, "BODOVI"), gArk[m][amBod]);
    PlayerTextDrawSetString(p, gArkBodTd[p], tekst);
}

bool:ArkadaKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || gArkAutomat[playerid] != -1) return false;
    new m = -1;
    for (new i = 0; i < ARK_MAX && m == -1; i++)
        if (gArk[i][amIma] && gArk[i][amSvijet] == GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, ARK_DOMET, gArk[i][amX], gArk[i][amY], gArk[i][amZ] + 1.0)) m = i;
    if (m == -1) return false;
    if (gArk[m][amIgrac] != INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Netko vec igra na ovom automatu.")), true;
    if (MdtOtvoren(playerid)) return true;
    if (MobitelOtvoren(playerid)) MobitelZatvori(playerid);
    if (!DajNovac(playerid, -ARK_CIJENA)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), true;
    BlagajnaDodaj(ARK_CIJENA);
    ArkPokreni(playerid, m);
    return true;
}

static ArkPokreni(playerid, m)
{
    gArkAutomat[playerid] = m;
    gArkBr[playerid] = 0;
    gArk[m][amIgrac] = playerid;
    gArk[m][amToken]++;
    gArk[m][amBod] = 0;
    gArk[m][amKraj] = false;
    TogglePlayerControllable(playerid, false);
    SetPlayerFacingAngle(playerid, gArk[m][amA] + 180.0);
    if (gArk[m][amIgra] == AI_ZMIJA)
    {
        ArkKutija(playerid, ARK_X - 8.0, ARK_Y - 34.0, ARK_SW * ARK_CEL + 14.5, ARK_SH * ARK_CEL + 64.0, 0x0B1A10E6);
        for (new c = 0; c < ARK_CELIJA; c++)
        {
            ArkKutija(playerid, ARK_X + float(c % ARK_SW) * ARK_CEL, ARK_Y + float(c / ARK_SW) * ARK_CEL, ARK_CEL - 1.5, ARK_CEL - 1.5, 0x10301AFF);
            gArkPolje[m][c] = false;
        }
        ArkTekst(playerid, 320.0, ARK_Y - 30.0, "SNAKE", 1.6, 0x7CFC00FF);
        gArkBodTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_SH * ARK_CEL + 6.0, "_", 1.0, 0xFFFFFFFF);
        gArkInfoTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_SH * ARK_CEL + 18.0, T(playerid, "W/A/S/D - smjer   ENTER - izlaz"), 0.8, 0xAAAAAAFF);
        gArk[m][amSmjer] = 1;
        gArk[m][amRep] = 0;
        gArk[m][amDuz] = 3;
        gArk[m][amMs] = 170;
        for (new i = 0; i < 3; i++)
        {
            new c = (ARK_SH / 2) * ARK_SW + 3 + i;
            gArkZmija[m][i] = c;
            gArkPolje[m][c] = true;
            ArkCelija(playerid, c, i == 2 ? 0xB6FF6AFF : 0x4CD137FF);
        }
        ArkHrana(m);
    }
    else if (gArk[m][amIgra] == AI_TRAKE)
    {
        ArkKutija(playerid, 320.0 - 70.0, ARK_Y - 34.0, 140.0, ARK_TR * ARK_TR_H + 64.0, 0x111111E6);
        for (new c = 0; c < ARK_TR * 3; c++)
        {
            ArkKutija(playerid, 320.0 - 61.0 + float(c % 3) * 41.0, ARK_Y + float(c / 3) * ARK_TR_H, 38.0, ARK_TR_H - 2.0, ARK_ASFALT);
            gArkPolje[m][c] = false;
        }
        ArkTekst(playerid, 320.0, ARK_Y - 30.0, "DODGER", 1.6, ARK_ZUTO);
        gArkBodTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_TR * ARK_TR_H + 6.0, "_", 1.0, 0xFFFFFFFF);
        gArkInfoTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_TR * ARK_TR_H + 18.0, T(playerid, "A/D - traka   ENTER - izlaz"), 0.8, 0xAAAAAAFF);
        gArk[m][amSmjer] = 1;
        gArk[m][amMeta] = 0;
        gArk[m][amRep] = 1;
        gArk[m][amMs] = 300;
        gArk[m][amHrana] = GetTickCount() + 600;
        ArkCelija(playerid, ARK_AUTO + 1, ARK_ZUTO);
        SetTimerEx("ArkTrakeTick", ARK_TR_MS, false, "ii", m, gArk[m][amToken]);
    }
    else if (gArk[m][amIgra] == AI_PUCANJE)
    {
        ArkKutija(playerid, ARK_PUC_X - 10.0, ARK_Y - 34.0, ARK_PUC_W + 20.0, ARK_PUC_H + 72.0, 0x10141EE6);
        ArkSprite(playerid, ARK_PUC_X, ARK_Y, ARK_PUC_W, ARK_PUC_H, "ld_grav:sky", 0xFFFFFFFF);
        ArkSprite(playerid, ARK_PUC_X, ARK_Y + ARK_PUC_TLO, ARK_PUC_W, ARK_PUC_H - ARK_PUC_TLO, "LD_SPAC:white", 0xB08850FF);
        for (new i = 0; i < ARK_METE; i++)
        {
            new PlayerText:td = ArkSprite(playerid, ARK_PUC_X + ArkPucSpotX(i) - ARK_PUC_FIG_W / 2.0, ARK_Y + ARK_PUC_FIG_Y, ARK_PUC_FIG_W, ARK_PUC_FIG_H, "_", 0xFFFFFFFF, false);
            PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_MODEL_PREVIEW);
            PlayerTextDrawBackgroundColour(playerid, td, 0x00000000);
            PlayerTextDrawSetPreviewModel(playerid, td, gArkKauboj[0]);
            gArkMt[m][i] = -1;
        }
        ArkSprite(playerid, ARK_PUC_X, ARK_Y + ARK_PUC_TLO - 8.0, ARK_PUC_W, 24.0, "LD_OTB:fen", 0xFFFFFFFF);
        ArkSprite(playerid, ARK_PUC_X, ARK_Y, ARK_PUC_W, ARK_PUC_H, "LD_SPAC:white", 0xC0000070, false);
        ArkSprite(playerid, -50.0, -50.0, 16.0, 1.5, "LD_SPAC:white", 0xFF3B3BFF);
        ArkSprite(playerid, -50.0, -50.0, 1.5, 16.0, "LD_SPAC:white", 0xFF3B3BFF);
        ArkSprite(playerid, -50.0, -50.0, 3.0, 3.0, "LD_BEAT:chit", 0xFFFFFFFF);
        ArkTekst(playerid, 320.0, ARK_Y - 30.0, "STRELJANA", 1.6, 0xFF6B3BFF);
        gArkBodTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_PUC_H + 6.0, "_", 1.0, 0xFFFFFFFF);
        gArkInfoTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + ARK_PUC_H + 18.0, T(playerid, "WASD nisan (ALT fino), LMB pucaj, ne gadjajte civile   ENTER - izlaz"), 0.8, 0xAAAAAAFF);
        gArk[m][amZivoti] = ARK_PUC_ZDRAVLJE;
        gArk[m][amSmjer] = 0;
        gArk[m][amMs] = 1400;
        gArk[m][amHrana] = GetTickCount() + 500;
        gArk[m][amRep] = 0;
        gArkCx[m] = ARK_PUC_W / 2.0;
        gArkCy[m] = ARK_PUC_H / 2.0;
        gArkCod[m] = GetTickCount();
        SetTimerEx("ArkPucTick", ARK_PUC_MS, false, "ii", m, gArk[m][amToken]);
    }
    else
    {
        ArkKutija(playerid, 320.0 - 88.0, ARK_Y - 34.0, 176.0, 194.0, 0x1E1230E6);
        for (new c = 0; c < 9; c++) ArkKutija(playerid, 320.0 - 75.0 + float(c % 3) * 52.0, ARK_Y + float(c / 3) * 40.0, 46.0, 34.0, ArkTamna(gArkSimonBoja[c]), true);
        ArkTekst(playerid, 320.0, ARK_Y - 30.0, "MEMORIJA", 1.6, 0xD980FAFF);
        gArkBodTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + 124.0, "_", 1.0, 0xFFFFFFFF);
        gArkInfoTd[playerid] = ArkTekst(playerid, 320.0, ARK_Y + 138.0, T(playerid, "Zapamtite i ponovite niz   ENTER - izlaz"), 0.8, 0xAAAAAAFF);
        SelectTextDraw(playerid, 0xD980FFFF);
        gArk[m][amDuz] = 0;
        ArkSimonDodaj(m);
    }
    ArkBodovi(m);
    if (gArk[m][amIgra] == AI_ZMIJA) SetTimerEx("ArkZmijaTick", gArk[m][amMs], false, "ii", m, gArk[m][amToken]);
}

static ArkHrana(m)
{
    new slobodno = ARK_CELIJA - gArk[m][amDuz], r = random(slobodno);
    for (new c = 0; c < ARK_CELIJA; c++)
    {
        if (gArkPolje[m][c] || r-- > 0) continue;
        gArk[m][amHrana] = c;
        ArkCelija(gArk[m][amIgrac], c, 0xFF4757FF);
        return;
    }
    gArk[m][amHrana] = -1;
}

forward ArkZmijaTick(m, token);
public ArkZmijaTick(m, token)
{
    if (token != gArk[m][amToken] || gArk[m][amKraj]) return;
    new p = gArk[m][amIgrac], KEY:tipke, gore, strana, s = gArk[m][amSmjer], n = -1;
    GetPlayerKeys(p, tipke, gore, strana);
    if (gore < 0) n = 0;
    else if (gore > 0) n = 2;
    else if (strana < 0) n = 3;
    else if (strana > 0) n = 1;
    if (n != -1 && n != (s + 2) % 4) s = gArk[m][amSmjer] = n;
    new glava = gArkZmija[m][(gArk[m][amRep] + gArk[m][amDuz] - 1) % ARK_CELIJA], x = glava % ARK_SW + gArkSmjerX[s], y = glava / ARK_SW + gArkSmjerY[s];
    if (x < 0 || x >= ARK_SW || y < 0 || y >= ARK_SH)
    {
        ArkKraj(m);
        return;
    }
    new c = y * ARK_SW + x, bool:jede = c == gArk[m][amHrana];
    if (!jede)
    {
        new rep = gArkZmija[m][gArk[m][amRep]];
        gArkPolje[m][rep] = false;
        ArkCelija(p, rep, 0x10301AFF);
        gArk[m][amRep] = (gArk[m][amRep] + 1) % ARK_CELIJA;
        gArk[m][amDuz]--;
    }
    if (gArkPolje[m][c])
    {
        ArkKraj(m);
        return;
    }
    ArkCelija(p, glava, 0x4CD137FF);
    gArkZmija[m][(gArk[m][amRep] + gArk[m][amDuz]) % ARK_CELIJA] = c;
    gArk[m][amDuz]++;
    gArkPolje[m][c] = true;
    ArkCelija(p, c, 0xB6FF6AFF);
    if (jede)
    {
        gArk[m][amBod] += 10;
        if (gArk[m][amMs] > 75) gArk[m][amMs] -= 5;
        PlayerPlaySound(p, 1056, 0.0, 0.0, 0.0);
        ArkBodovi(m);
        if (gArk[m][amDuz] >= ARK_CELIJA)
        {
            ArkKraj(m);
            return;
        }
        ArkHrana(m);
    }
    SetTimerEx("ArkZmijaTick", gArk[m][amMs], false, "ii", m, token);
}

static ArkPucSkrij(m, i)
{
    gArkMt[m][i] = -1;
    PlayerTextDrawHide(gArk[m][amIgrac], gArkTd[gArk[m][amIgrac]][ARK_PUC_META + i]);
}

static ArkPucFigura(m, i, model, Float:rx)
{
    new p = gArk[m][amIgrac], PlayerText:td = gArkTd[p][ARK_PUC_META + i];
    PlayerTextDrawSetPreviewModel(p, td, model);
    PlayerTextDrawSetPreviewRot(p, td, rx, 0.0, 0.0, 1.0);
    PlayerTextDrawShow(p, td);
}

static ArkPucNova(m)
{
    new slobodna[ARK_METE], n = 0;
    for (new i = 0; i < ARK_METE; i++) if (gArkMt[m][i] == -1) slobodna[n++] = i;
    if (!n) return;
    new i = slobodna[random(n)], bool:civil = random(100) < 25, sad = GetTickCount(), rok = 1600 - gArk[m][amBod] * 3;
    if (rok < 650) rok = 650;
    gArkMt[m][i] = civil ? 1 : 0;
    gArkMod[m][i] = civil ? gArkCivil[random(sizeof gArkCivil)] : gArkKauboj[random(sizeof gArkKauboj)];
    gArkMe[m][i] = sad + (civil ? 1800 : rok);
    ArkPucFigura(m, i, gArkMod[m][i], 0.0);
}

static ArkPucPogodjen(m)
{
    new p = gArk[m][amIgrac];
    PlayerPlaySound(p, 1055, 0.0, 0.0, 0.0);
    gArk[m][amZivoti] -= ARK_PUC_STETA;
    if (gArk[m][amZivoti] < 0) gArk[m][amZivoti] = 0;
    ArkBodovi(m);
    PlayerTextDrawShow(p, gArkTd[p][ARK_PUC_FLASH]);
    gArk[m][amSmjer] = GetTickCount() + 150;
    if (gArk[m][amZivoti] > 0) return 1;
    ArkKraj(m);
    return 0;
}

forward ArkPucTick(m, token);
public ArkPucTick(m, token)
{
    if (token != gArk[m][amToken] || gArk[m][amKraj]) return;
    new p = gArk[m][amIgrac], KEY:tipke, gore, strana, sad = GetTickCount();
    GetPlayerKeys(p, tipke, gore, strana);
    new Float:brzina = (tipke & KEY_WALK) ? ARK_PUC_POMAK * 0.3 : ARK_PUC_POMAK;
    gArkCx[m] += float(strana) / 128.0 * brzina;
    gArkCy[m] += float(gore) / 128.0 * brzina;
    gArkCx[m] = gArkCx[m] < 0.0 ? 0.0 : gArkCx[m] > ARK_PUC_W ? ARK_PUC_W : gArkCx[m];
    gArkCy[m] = gArkCy[m] < 0.0 ? 0.0 : gArkCy[m] > ARK_PUC_H ? ARK_PUC_H : gArkCy[m];
    if (gArk[m][amSmjer] && sad - gArk[m][amSmjer] >= 0)
    {
        gArk[m][amSmjer] = 0;
        PlayerTextDrawHide(p, gArkTd[p][ARK_PUC_FLASH]);
    }
    if (sad - gArk[m][amHrana] >= 0)
    {
        ArkPucNova(m);
        gArk[m][amMs] = 1400 - gArk[m][amBod] * 3;
        if (gArk[m][amMs] < 550) gArk[m][amMs] = 550;
        gArk[m][amHrana] = sad + gArk[m][amMs];
    }
    for (new i = 0; i < ARK_METE; i++)
    {
        new t = gArkMt[m][i];
        if (t == -1 || sad - gArkMe[m][i] < 0) continue;
        ArkPucSkrij(m, i);
        if (t == 0 && !ArkPucPogodjen(m)) return;
    }
    new Float:a = 1.5 + float(sad - gArkCod[m]) * 0.0008, Float:x, Float:y;
    if (a > 7.0) a = 7.0;
    ArkPucNisan(m, a, float(sad % 360000) / 1000.0, x, y);
    ArkPomakni(p, ARK_PUC_NISAN, x, y, 16.0, 1.5);
    ArkPomakni(p, ARK_PUC_NISAN + 1, x, y, 1.5, 16.0);
    ArkPomakni(p, ARK_PUC_NISAN + 2, x, y, 3.0, 3.0);
    SetTimerEx("ArkPucTick", ARK_PUC_MS, false, "ii", m, token);
}

static ArkPucNisan(m, Float:a, Float:t, &Float:x, &Float:y)
{
    x = gArkCx[m] + a * (0.65 * floatsin(t * 97.0, degrees) + 0.35 * floatsin(t * 233.0 + 20.0, degrees));
    y = gArkCy[m] + a * 0.8 * (0.6 * floatsin(t * 131.0 + 40.0, degrees) + 0.4 * floatsin(t * 263.0, degrees));
}

static ArkPucaj(m)
{
    new p = gArk[m][amIgrac], sad = GetTickCount();
    if (sad - gArk[m][amRep] < ARK_PUC_PAUZA) return;
    gArk[m][amRep] = sad;
    new Float:a = 1.5 + float(sad - gArkCod[m]) * 0.0008, Float:x, Float:y;
    if (a > 7.0) a = 7.0;
    ArkPucNisan(m, a, float(sad % 360000) / 1000.0, x, y);
    gArkCod[m] = sad;
    if (y < ARK_PUC_FIG_Y + 4.0 || y > ARK_PUC_TLO) return;
    for (new i = 0; i < ARK_METE; i++)
    {
        new t = gArkMt[m][i];
        if ((t != 0 && t != 1) || floatabs(x - ArkPucSpotX(i)) > ARK_PUC_FIG_W * 0.3) continue;
        ArkPucFigura(m, i, gArkMod[m][i], 90.0);
        gArkMt[m][i] = 2;
        if (t == 1)
        {
            PlayerPlaySound(p, 1055, 0.0, 0.0, 0.0);
            GameTextForPlayer(p, T(p, "~r~Ubili ste civila!"), 2500, 3);
            ArkKraj(m);
            return;
        }
        gArk[m][amBod] += 10 + (gArkMe[m][i] - sad) / 100;
        gArkMe[m][i] = sad + 450;
        PlayerPlaySound(p, 1056, 0.0, 0.0, 0.0);
        ArkBodovi(m);
        return;
    }
}

static ArkTamna(boja)
{
    return ((boja >>> 1) & 0x7F7F7F00) | 0xFF;
}

static ArkSimonDodaj(m)
{
    new p = gArk[m][amIgrac];
    for (new c = 0; c < 9; c++) ArkCelija(p, c, ArkTamna(gArkSimonBoja[c]));
    if (gArk[m][amDuz] >= ARK_CELIJA)
    {
        ArkKraj(m);
        return;
    }
    gArkZmija[m][gArk[m][amDuz]++] = random(9);
    gArk[m][amSmjer] = 0;
    gArk[m][amRep] = 0;
    ArkBodovi(m);
    SetTimerEx("ArkSimonKorak", 800, false, "ii", m, ++gArk[m][amToken]);
}

forward ArkSimonKorak(m, token);
public ArkSimonKorak(m, token)
{
    if (token != gArk[m][amToken] || gArk[m][amKraj]) return;
    new p = gArk[m][amIgrac], i = gArk[m][amRep]++;
    if (i >= gArk[m][amDuz] * 2)
    {
        gArk[m][amSmjer] = 1;
        gArk[m][amRep] = 0;
        ArkBodovi(m);
        return;
    }
    new c = gArkZmija[m][i / 2], ms = 180;
    if (i % 2 == 0)
    {
        ArkCelija(p, c, gArkSimonBoja[c]);
        PlayerPlaySound(p, 1056, 0.0, 0.0, 0.0);
        ms = 620 - gArk[m][amDuz] * 20;
        if (ms < 260) ms = 260;
    }
    else ArkCelija(p, c, ArkTamna(gArkSimonBoja[c]));
    SetTimerEx("ArkSimonKorak", ms, false, "ii", m, token);
}

forward ArkSimonUgasi(m, c, token);
public ArkSimonUgasi(m, c, token)
{
    if (token == gArk[m][amToken] && !gArk[m][amKraj]) ArkCelija(gArk[m][amIgrac], c, ArkTamna(gArkSimonBoja[c]));
}

static ArkSimonKlik(m, c)
{
    if (gArk[m][amSmjer] != 1) return;
    new p = gArk[m][amIgrac], tocna = gArkZmija[m][gArk[m][amRep]];
    if (c != tocna)
    {
        PlayerPlaySound(p, 1055, 0.0, 0.0, 0.0);
        ArkCelija(p, tocna, gArkSimonBoja[tocna]);
        ArkKraj(m);
        return;
    }
    ArkCelija(p, c, gArkSimonBoja[c]);
    PlayerPlaySound(p, 1056, 0.0, 0.0, 0.0);
    SetTimerEx("ArkSimonUgasi", 220, false, "iii", m, c, gArk[m][amToken]);
    if (++gArk[m][amRep] < gArk[m][amDuz]) return;
    gArk[m][amBod] = gArk[m][amDuz];
    ArkSimonDodaj(m);
}

static bool:ArkTrakeKorak(m)
{
    new p = gArk[m][amIgrac], bool:novo[ARK_TR * 3], prije = gArk[m][amBod];
    for (new c = 0; c < ARK_TR * 3; c++)
    {
        if (!gArkPolje[m][c]) continue;
        new n = c + 3;
        if (n >= ARK_TR * 3)
        {
            gArk[m][amBod]++;
            continue;
        }
        if (n == ARK_AUTO + gArk[m][amSmjer]) return false;
        novo[n] = true;
    }
    if (--gArk[m][amRep] <= 0)
    {
        new a = random(3);
        novo[a] = true;
        if (gArk[m][amBod] >= 15 && !random(3)) novo[(a + 1 + random(2)) % 3] = true;
        gArk[m][amRep] = 2 + random(2);
    }
    for (new c = 0; c < ARK_TR * 3; c++)
    {
        if (novo[c] == gArkPolje[m][c]) continue;
        gArkPolje[m][c] = novo[c];
        ArkCelija(p, c, novo[c] ? 0xFF4757FF : ARK_ASFALT);
    }
    gArk[m][amMs] = 300 - gArk[m][amBod] * 4;
    if (gArk[m][amMs] < 90) gArk[m][amMs] = 90;
    if (gArk[m][amBod] != prije) ArkBodovi(m);
    return true;
}

forward ArkTrakeTick(m, token);
public ArkTrakeTick(m, token)
{
    if (token != gArk[m][amToken] || gArk[m][amKraj]) return;
    new p = gArk[m][amIgrac], KEY:tipke, gore, strana;
    GetPlayerKeys(p, tipke, gore, strana);
    new smjer = strana < 0 ? -1 : strana > 0 ? 1 : 0, l = gArk[m][amSmjer] + smjer;
    if (smjer && !gArk[m][amMeta] && l >= 0 && l <= 2)
    {
        ArkCelija(p, ARK_AUTO + gArk[m][amSmjer], ARK_ASFALT);
        gArk[m][amSmjer] = l;
        if (gArkPolje[m][ARK_AUTO + l])
        {
            ArkCelija(p, ARK_AUTO + l, 0xFF0000FF);
            ArkKraj(m);
            return;
        }
        ArkCelija(p, ARK_AUTO + l, ARK_ZUTO);
    }
    gArk[m][amMeta] = smjer;
    if (GetTickCount() - gArk[m][amHrana] >= 0)
    {
        gArk[m][amHrana] = GetTickCount() + gArk[m][amMs];
        if (!ArkTrakeKorak(m))
        {
            ArkCelija(p, ARK_AUTO + gArk[m][amSmjer], 0xFF0000FF);
            ArkKraj(m);
            return;
        }
    }
    SetTimerEx("ArkTrakeTick", ARK_TR_MS, false, "ii", m, token);
}

bool:ArkadaKlik(playerid, PlayerText:td)
{
    new m = gArkAutomat[playerid];
    if (m == -1 || gArk[m][amIgra] != AI_MEMORIJA) return false;
    if (gArk[m][amKraj]) return true;
    for (new c = 0; c < 9; c++)
    {
        if (td != gArkTd[playerid][c + 1]) continue;
        ArkSimonKlik(m, c);
        break;
    }
    return true;
}

static ArkKraj(m)
{
    if (gArk[m][amKraj]) return;
    gArk[m][amKraj] = true;
    gArk[m][amToken]++;
    new p = gArk[m][amIgrac], tekst[512], red[64];
    if (gArk[m][amBod] > 0) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO arcade_scores (machine, account, score, created) VALUES (%d, %d, %d, %d)", gArk[m][amDb], gAccountId[p], gArk[m][amBod], gettime()));
    format(tekst, sizeof tekst, "~r~%s~n~~w~%s: ~y~%d~n~~n~~w~TOP 5", T(p, "KRAJ IGRE"), T(p, "BODOVI"), gArk[m][amBod]);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT replace(a.name, '_', ' '), MAX(s.score) AS best FROM arcade_scores s JOIN accounts a ON a.id = s.account WHERE s.machine = %d GROUP BY s.account ORDER BY best DESC LIMIT 5", gArk[m][amDb]);
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        new ime[MAX_PLAYER_NAME + 1];
        DB_GetFieldString(res, 0, ime, sizeof ime);
        TdCisti(ime);
        format(red, sizeof red, "~n~~w~%d. %s ~y~%d", i + 1, ime, DB_GetFieldInt(res, 1));
        strcat(tekst, red);
    }
    DB_FreeResultSet(res);
    ArkKutija(p, 320.0 - 90.0, ARK_Y - 4.0, 180.0, 128.0, 0x000000E6);
    ArkTekst(p, 320.0, ARK_Y, tekst, 1.0, 0xFFFFFFFF);
    SetTimerEx("ArkZatvoriTimer", ARK_KRAJ_MS, false, "ii", m, gArk[m][amToken]);
}

forward ArkZatvoriTimer(m, token);
public ArkZatvoriTimer(m, token)
{
    if (token == gArk[m][amToken] && gArk[m][amIgrac] != INVALID_PLAYER_ID) ArkZatvori(gArk[m][amIgrac]);
}

ArkZatvori(playerid)
{
    new m = gArkAutomat[playerid];
    if (m == -1) return;
    gArkAutomat[playerid] = -1;
    gArk[m][amIgrac] = INVALID_PLAYER_ID;
    gArk[m][amToken]++;
    for (new i = 0; i < gArkBr[playerid]; i++) PlayerTextDrawDestroy(playerid, gArkTd[playerid][i]);
    gArkBr[playerid] = 0;
    if (!IsPlayerConnected(playerid)) return;
    if (gArk[m][amIgra] == AI_MEMORIJA) CancelSelectTextDraw(playerid);
    TogglePlayerControllable(playerid, true);
}

ArkadaIgra(playerid)
{
    return gArkAutomat[playerid] != -1;
}

ArkadaEsc(playerid)
{
    new m = gArkAutomat[playerid];
    if (m == -1) return;
    if (gArk[m][amKraj]) ArkZatvori(playerid);
    else ArkKraj(m);
}

bool:ArkadaTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (gArkAutomat[playerid] == -1) return false;
    if ((newkeys & KEY_SECONDARY_ATTACK) && !(oldkeys & KEY_SECONDARY_ATTACK)) ArkadaEsc(playerid);
    else if ((newkeys & KEY_FIRE) && !(oldkeys & KEY_FIRE))
    {
        new m = gArkAutomat[playerid];
        if (gArk[m][amIgra] == AI_PUCANJE && !gArk[m][amKraj]) ArkPucaj(m);
    }
    return true;
}

ShowArkade(playerid)
{
    new list[2048], red[96], len = 0, n = 0;
    for (new g = 0; g < sizeof gArkIme; g++)
    {
        format(red, sizeof red, T(playerid, "+ Novi automat: %s (ispred mene)"), T(playerid, gArkIme[g]));
        DodajStupac(list, len, red);
    }
    for (new m = 0; m < ARK_MAX; m++)
    {
        if (!gArk[m][amIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] %s (%s) - obrisi"), gArk[m][amDb], T(playerid, gArkIme[gArk[m][amIgra]]), CestaZona(gArk[m][amX], gArk[m][amY]));
        DodajStupac(list, len, red);
        gApArk[playerid][n++] = m;
    }
    return PrikaziDialog(playerid, DLG_APANEL_ARKADA, DIALOG_STYLE_LIST, T(playerid, "Arkade"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_arkada(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem < sizeof gArkIme)
    {
        new m = 0, Float:x, Float:y, Float:z, Float:a;
        while (m < ARK_MAX && gArk[m][amIma]) m++;
        if (m == ARK_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj automata.")), ShowArkade(playerid);
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        x -= floatsin(a, degrees) * 1.0;
        y += floatcos(a, degrees) * 1.0;
        z -= 1.0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO arcade_machines (x, y, z, a, interior, world, game) VALUES (%f, %f, %f, %f, %d, %d, %d)", x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid), listitem));
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT last_insert_rowid()");
        gArk[m][amDb] = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        gArk[m][amX] = x;
        gArk[m][amY] = y;
        gArk[m][amZ] = z;
        gArk[m][amA] = a;
        gArk[m][amInt] = GetPlayerInterior(playerid);
        gArk[m][amSvijet] = GetPlayerVirtualWorld(playerid);
        gArk[m][amIgra] = listitem;
        ArkStvori(m);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Automat je postavljen."));
        return ShowArkade(playerid);
    }
    new i = listitem - sizeof gArkIme;
    if (i < 0 || i >= ARK_MAX) return 1;
    new m = gApArk[playerid][i];
    if (!gArk[m][amIma]) return ShowArkade(playerid);
    if (gArk[m][amIgrac] != INVALID_PLAYER_ID) ArkZatvori(gArk[m][amIgrac]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM arcade_machines WHERE id = %d", gArk[m][amDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM arcade_scores WHERE machine = %d", gArk[m][amDb]));
    DestroyDynamicObject(gArk[m][amObj]);
    DestroyDynamic3DTextLabel(gArk[m][amTekst]);
    gArk[m][amIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Automat je obrisan."));
    return ShowArkade(playerid);
}
