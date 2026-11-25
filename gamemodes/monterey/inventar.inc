#define MAX_KES             1000000000
#define PAY_RANGE           3.0

new gNovac[MAX_PLAYERS];

NovacConnect(playerid)
{
    gNovac[playerid] = 0;
}

PostaviNovac(playerid, amount, bool:save = true)
{
    gNovac[playerid] = amount;
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, amount);
    if (save) DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET novac = %d WHERE id = %d", amount, gAccountId[playerid]));
}

UcitajNovac(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT novac FROM accounts WHERE id = %d", gAccountId[playerid]);
    new amount = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    PostaviNovac(playerid, amount, false);
}

static PlayerText:gNovacTd[MAX_PLAYERS] = { PlayerText:INVALID_TEXT_DRAW, ... }, gNovacPromjena[MAX_PLAYERS], gNovacToken[MAX_PLAYERS];

static NovacPromjena(playerid, amount)
{
    if (!amount || IsPlayerNPC(playerid)) return;
    if (gNovacTd[playerid] == PlayerText:INVALID_TEXT_DRAW)
    {
        gNovacTd[playerid] = CreatePlayerTextDraw(playerid, 608.0, 101.0, "_");
        PlayerTextDrawFont(playerid, gNovacTd[playerid], TEXT_DRAW_FONT_3);
        PlayerTextDrawLetterSize(playerid, gNovacTd[playerid], 0.5, 1.8);
        PlayerTextDrawAlignment(playerid, gNovacTd[playerid], TEXT_DRAW_ALIGN_RIGHT);
        PlayerTextDrawSetOutline(playerid, gNovacTd[playerid], 2);
        PlayerTextDrawBackgroundColour(playerid, gNovacTd[playerid], 0x000000FF);
        PlayerTextDrawSetProportional(playerid, gNovacTd[playerid], true);
        gNovacPromjena[playerid] = 0;
    }
    gNovacPromjena[playerid] += amount;
    PlayerTextDrawSetString(playerid, gNovacTd[playerid], gNovacPromjena[playerid] < 0 ? ("-$%d") : ("+$%d"), gNovacPromjena[playerid] < 0 ? -gNovacPromjena[playerid] : gNovacPromjena[playerid]);
    PlayerTextDrawColour(playerid, gNovacTd[playerid], gNovacPromjena[playerid] < 0 ? 0xB4191DFF : 0x36682CFF);
    PlayerTextDrawSetPos(playerid, gNovacTd[playerid], 608.0, GetPlayerWantedLevel(playerid) ? 125.0 : 101.0);
    PlayerTextDrawShow(playerid, gNovacTd[playerid]);
    SetTimerEx("NovacSakrij", 3000, false, "ii", playerid, ++gNovacToken[playerid]);
}

forward NovacSakrij(playerid, token);
public NovacSakrij(playerid, token)
{
    if (token != gNovacToken[playerid] || gNovacTd[playerid] == PlayerText:INVALID_TEXT_DRAW) return;
    PlayerTextDrawHide(playerid, gNovacTd[playerid]);
    gNovacPromjena[playerid] = 0;
}

NovacDisconnect(playerid)
{
    gNovacTd[playerid] = PlayerText:INVALID_TEXT_DRAW;
    gNovacPromjena[playerid] = 0;
    gNovacToken[playerid]++;
}

bool:DajNovac(playerid, amount)
{
    new total = gNovac[playerid] + amount;
    if (total < 0 || total > MAX_KES) return false;
    PostaviNovac(playerid, total);
    NovacPromjena(playerid, amount);
    return true;
}

bool:TransferNovac(from, to, amount)
{
    if (!DajNovac(from, -amount)) return false;
    if (DajNovac(to, amount)) return true;
    DajNovac(from, amount);
    return false;
}

ProvjetiBlizinuIgraca(playerid, const s[])
{
    new target = FindPlayer(s);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) { SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden.")); return INVALID_PLAYER_ID; }
    if (target == playerid) { SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to sami sebi.")); return INVALID_PLAYER_ID; }
    new Float:x, Float:y, Float:z;
    GetPlayerPos(target, x, y, z);
    if (!IsPlayerInRangeOfPoint(playerid, PAY_RANGE, x, y, z) || GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(target))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije blizu vas."));
        return INVALID_PLAYER_ID;
    }
    return target;
}

CMD:plati(playerid, params[])
{
    if (IsRanjen(playerid)) return 1;
    new idx = 0, p1[32], p2[16];
    if (!gUlogiran[playerid]) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || strlen(p2) > 9)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /plati [id/ime] [iznos]"));
    new amount = strval(p2), target = ProvjetiBlizinuIgraca(playerid, p1);
    if (target == INVALID_PLAYER_ID) return 1;
    if (amount <= 0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Iznos mora biti veci od 0."));
    if (!TransferNovac(playerid, target, amount)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca."));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Dali ste $%d igracu %s."), amount, GetRPIme(target));
    SendClientMessage(target, COLOR_INFO, T(target, "%s vam je dao $%d."), GetRPIme(playerid), amount);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s vadi novac i daje ga %s.", GetRPIme(playerid), GetRPIme(target), 1);
    return 1;
}

AdminDajNovac(playerid, const params[])
{
    new idx = 0, p1[32], p2[16];
    if (!ProvjeriOvlasti(playerid, ALVL_MONEY)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || strlen(p2) > 9 + _:(p2[0] == '-') || !(ProvjeriBroj(p2) || (p2[0] == '-' && ProvjeriBroj(p2[1]))))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [iznos, negativan oduzima]"));
    new amount = strval(p2), target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (!amount || !DajNovac(target, amount)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravan iznos (igrac ima $%d)."), gNovac[target]);
    new an[MAX_PLAYER_NAME + 1], tn[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, an, sizeof an);
    GetPlayerName(target, tn, sizeof tn);
    printf("[MRP] novac: %s -> %s %d (sada %d)", an, tn, amount, gNovac[target]);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s sada ima $%d."), tn, gNovac[target]);
    SendClientMessage(target, COLOR_ADMIN, T(target, "Admin vam je %s $%d."), amount > 0 ? T(target, "dao") : T(target, "oduzeo"), amount > 0 ? amount : -amount);
    return 1;
}

enum
{
    ITEM_MARIHUANA, ITEM_KOKAIN, ITEM_SJEME,
    ITEM_COLT, ITEM_DEAGLE, ITEM_SACMARICA, ITEM_UZI, ITEM_MP5, ITEM_AK47, ITEM_M4, ITEM_NOZ, ITEM_PALICA,
    ITEM_MUN_PISTOLJ, ITEM_MUN_SACMARICA, ITEM_MUN_SMG, ITEM_MUN_PUSKA,
    ITEM_PENDREK, ITEM_CRIJEVO, ITEM_KANISTER, ITEM_KANISTER_BENZIN, ITEM_KANISTER_DIZEL,
    ITEM_MOBITEL, ITEM_NAOCALE1, ITEM_NAOCALE2, ITEM_NAOCALE3, ITEM_BANDANA1, ITEM_BANDANA2, ITEM_BANDANA3, ITEM_KAPA1, ITEM_KAPA2, ITEM_KAPA3, ITEM_ZETONI, ITEM_KARTICA,
    ITEM_SAT1, ITEM_SAT2, ITEM_SAT3, ITEM_PRSTEN, ITEM_LANAC, ITEM_OGRLICA, ITEM_UKRADENI_NAKIT, ITEM_ZLATO, ITEM_ZLATNE_NAOCALE, ITEM_SREBRNE_NAOCALE,
    ITEM_METH, ITEM_KOKA, ITEM_KEMIKALIJE, ITEM_CIGARETE, ITEM_SRECKA, ITEM_KUTIJA, ITEM_EKSPLOZIV, ITEM_SANDUK_KOKA, ITEM_SANDUK_KEM
}

enum E_ITEM { ITEM_NAME[28], ITEM_UNIT[4], ITEM_MAX, ITEM_WEAPON, ITEM_AMMO, ITEM_MODEL }
new const gItems[][E_ITEM] = {
    { "Marihuana",              "g",   500, 0,  -1, 1578  },
    { "Kokain",                 "g",   200, 0,  -1, 1575  },
    { "Sjemenke marihuane",     "kom", 50,  0,  -1, 1279  },
    { "Pistolj (Colt 45)",      "kom", 3,   22, ITEM_MUN_PISTOLJ, 346   },
    { "Desert Eagle",           "kom", 3,   24, ITEM_MUN_PISTOLJ, 348   },
    { "Sacmarica",              "kom", 3,   25, ITEM_MUN_SACMARICA, 349   },
    { "Uzi",                    "kom", 3,   28, ITEM_MUN_SMG, 352   },
    { "MP5",                    "kom", 3,   29, ITEM_MUN_SMG, 353   },
    { "AK-47",                  "kom", 3,   30, ITEM_MUN_PUSKA, 355   },
    { "M4",                     "kom", 3,   31, ITEM_MUN_PUSKA, 356   },
    { "Noz",                    "kom", 3,   4,  -1, 335   },
    { "Palica",                 "kom", 3,   5,  -1, 336   },
    { "Municija za pistolj",    "kom", 500, 0,  -1, 2061  },
    { "Patrone za sacmaricu",   "kom", 200, 0,  -1, 2061  },
    { "Municija za automat",    "kom", 500, 0,  -1, 2040  },
    { "Municija za pusku",      "kom", 500, 0,  -1, 2040  },
    { "Pendrek",                "kom", 3,   3,  -1, 334   },
    { "Crijevo za gorivo",      "kom", 1,   0,  -1, 19621 },
    { "Prazan kanister",        "kom", 3,   0,  -1, 1650  },
    { "Kanister benzina",       "L",   20,  0,  -1, 1650  },
    { "Kanister dizela",        "L",   20,  0,  -1, 1650  },
    { "Mobitel",                "kom", 1,   0,  -1, 18868 },
    { "Naocale 1",              "kom", 3,   0,  -1, 19006 },
    { "Naocale 2",              "kom", 3,   0,  -1, 19007 },
    { "Naocale 3",              "kom", 3,   0,  -1, 19022 },
    { "Bandana 1",              "kom", 3,   0,  -1, 18891 },
    { "Bandana 2",              "kom", 3,   0,  -1, 18892 },
    { "Bandana 3",              "kom", 3,   0,  -1, 18893 },
    { "Kapa 1",                 "kom", 3,   0,  -1, 18926 },
    { "Kapa 2",                 "kom", 3,   0,  -1, 18927 },
    { "Kapa 3",                 "kom", 3,   0,  -1, 18939 },
    { "Kazino zetoni",          "kom", 10000000, 0, -1, 1212 },
    { "Bankovna kartica",       "kom", 1,   0,  -1, 1581  },
    { "Rucni sat",              "kom", 3,   0,  -1, 19039 },
    { "Sportski sat",           "kom", 3,   0,  -1, 19045 },
    { "Zlatni sat",             "kom", 3,   0,  -1, 19042 },
    { "Zlatni prsten",          "kom", 5,   0,  -1, 19941 },
    { "Zlatni lanac",           "kom", 5,   0,  -1, 19941 },
    { "Dijamantna ogrlica",     "kom", 3,   0,  -1, 19941 },
    { "Ukradeni nakit",         "kom", 50,  0,  -1, 19941 },
    { "Zlatna poluga",          "kom", 100, 0,  -1, 19941 },
    { "Zlatne naocale",         "kom", 3,   0,  -1, 19017 },
    { "Srebrne naocale",        "kom", 3,   0,  -1, 19023 },
    { "Metamfetamin",           "g",   200, 0,  -1, 1579  },
    { "Koka pasta",             "g",   500, 0,  -1, 1577  },
    { "Kemikalije",             "l",   20,  0,  -1, 1576  },
    { "Cigarete",               "kom", 40,  0,  -1, 19896 },
    { "Srecka",                 "kom", 10,  0,  -1, 1581  },
    { "Kutija robe",            "kom", 1,   0,  -1, 1220  },
    { "Eksploziv",              "kom", 6,   0,  -1, 1654  },
    { "Sanduk koka paste",      "kom", 1,   0,  -1, 1220  },
    { "Sanduk kemikalija",      "kom", 1,   0,  -1, 1220  }
};
#define BROJ_ITEMA          sizeof (gItems)
#define MAX_PUSAKA          7
#define KORISTENJE_COOLDOWN   60000

new gInv[MAX_PLAYERS][BROJ_ITEMA];
static gInvList[MAX_PLAYERS][BROJ_ITEMA];
static gInvCount[MAX_PLAYERS];
static gInvItem[MAX_PLAYERS];
static gItemUsedAt[MAX_PLAYERS];
forward bool:PolicajacNaDuznosti(playerid);
static bool:gInvSpremi[MAX_PLAYERS], gFriskOd[MAX_PLAYERS], gFriskMeta[MAX_PLAYERS], gFriskRacun[MAX_PLAYERS], gFriskAkcija[MAX_PLAYERS][4];
#define FRISK_MS            5000

InitInventar()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS inventory (\
        account INTEGER NOT NULL, item INTEGER NOT NULL, amount INTEGER NOT NULL, PRIMARY KEY (account, item))"));
    InventarCheck();
}

InventarConnect(playerid)
{
    for (new i = 0; i < BROJ_ITEMA; i++) gInv[playerid][i] = 0;
    gItemUsedAt[playerid] = GetTickCount() - KORISTENJE_COOLDOWN;
    gFriskOd[playerid] = 0;
}

LoadInventar(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT item, amount FROM inventory WHERE account = %d", gAccountId[playerid]);
    if (DB_GetRowCount(res)) do
    {
        new item = DB_GetFieldInt(res, 0);
        gInv[playerid][item] = DB_GetFieldInt(res, 1);
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
}

bool:GiveItem(playerid, item, amount)
{
    new total = gInv[playerid][item] + amount;
    if (total < 0 || total > gItems[item][ITEM_MAX]) return false;
    if (amount > 0 && gItems[item][ITEM_AMMO] != -1 && BrojOruzja(playerid) + amount > MAX_PUSAKA) return false;
    gInv[playerid][item] = total;
    if (total) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO inventory (account, item, amount) VALUES (%d, %d, %d)", gAccountId[playerid], item, total));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM inventory WHERE account = %d AND item = %d", gAccountId[playerid], item));
    if (!total && gItems[item][ITEM_WEAPON]) StoreItem(playerid, item);
    if (!total) PredmetNestao(playerid, item);
    return true;
}

static PredmetNestao(playerid, item)
{
    if (item == ITEM_MOBITEL) MobitelZatvori(playerid);
    else if ((item >= ITEM_NAOCALE1 && item <= ITEM_KAPA3) || item == ITEM_ZLATNE_NAOCALE || item == ITEM_SREBRNE_NAOCALE) OdjecaPredmetNestao(playerid, item);
    else if ((item >= ITEM_SAT1 && item <= ITEM_SAT3) || item == ITEM_LANAC || item == ITEM_OGRLICA) ZlataraPredmetNestao(playerid, item);
}

static InventarCheck()
{
    new t = MAX_PLAYERS - 1;
    gNovac[t] = 100;
    assert(!DajNovac(t, -101) && gNovac[t] == 100);
    assert(DajNovac(t, -100) && gNovac[t] == 0);
    assert(!DajNovac(t, MAX_KES + 1) && gNovac[t] == 0);
    assert(GiveItem(t, ITEM_KOKAIN, 5) && gInv[t][ITEM_KOKAIN] == 5);
    assert(!GiveItem(t, ITEM_KOKAIN, -6) && !GiveItem(t, ITEM_KOKAIN, gItems[ITEM_KOKAIN][ITEM_MAX]));
    assert(GiveItem(t, ITEM_KOKAIN, -5) && gInv[t][ITEM_KOKAIN] == 0);
}

static KoristiItem(playerid, item)
{
    if (gItems[item][ITEM_WEAPON]) return IzvadiOruzje(playerid, item);
    if (item == ITEM_MOBITEL) return MobitelPrebaci(playerid);
    if ((item >= ITEM_NAOCALE1 && item <= ITEM_KAPA3) || item == ITEM_ZLATNE_NAOCALE || item == ITEM_SREBRNE_NAOCALE) return OdjecaKapaPredmet(playerid, item);
    if (item == ITEM_CRIJEVO) return VoziloIsprazni(playerid);
    if (item >= ITEM_SAT1 && item <= ITEM_SAT3) return ZlataraSat(playerid, item);
    if (item == ITEM_LANAC || item == ITEM_OGRLICA) return ZlataraOgrlica(playerid, item);
    if (item == ITEM_UKRADENI_NAKIT) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Ukradeni nakit prodajte preprodavacu."));
    if (item >= ITEM_PRSTEN && item <= ITEM_ZLATO) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Nakit i zlato prodajete u zlatari."));
    if (item == ITEM_KARTICA) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Karticu koristite u banci i na mobitelu (Internet, Banka)."));
    if (item == ITEM_ZETONI) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zetone mijenjate na blagajni kazina."));
    if (item == ITEM_KANISTER) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prazan kanister napunite na pumpi naredbom /tankaj."));
    if (item == ITEM_KANISTER_BENZIN || item == ITEM_KANISTER_DIZEL) return VoziloKanister(playerid, item, -1);
    if (item == ITEM_SJEME) return DrogaPosadi(playerid);
    if (item == ITEM_SRECKA) return SreckaOgrebi(playerid);
    if (item == ITEM_CIGARETE)
    {
        if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti pjeske."));
        if (!GiveItem(playerid, item, -1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pali cigaretu.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (item == ITEM_KOKA || item == ITEM_KEMIKALIJE) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koka pasta i kemikalije preraduju se u laboratoriju mafije (/lab)."));
    if (item != ITEM_MARIHUANA && item != ITEM_KOKAIN && item != ITEM_METH)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Municija se puni kad izvadite oruzje."));
    if (GetTickCount() - gItemUsedAt[playerid] < KORISTENJE_COOLDOWN)
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pricekajte jos %d s."), (KORISTENJE_COOLDOWN - (GetTickCount() - gItemUsedAt[playerid])) / 1000 + 1);
    if (!GiveItem(playerid, item, -1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
    gItemUsedAt[playerid] = GetTickCount();
    switch (item)
    {
        case ITEM_MARIHUANA: PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pali dzoint.", GetRPIme(playerid), "", 1);
        case ITEM_KOKAIN: PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s smrka liniju kokaina.", GetRPIme(playerid), "", 1);
        case ITEM_METH: PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pusi metamfetamin iz lule.", GetRPIme(playerid), "", 1);
    }
    DrogaUcinak(playerid, item);
    return 1;
}

PrikaziInventar(playerid)
{
    new list[512] = "Predmet\tKolicina\n", row[64], count = 0;
    gInvSpremi[playerid] = ImaIzvadenoOruzje(playerid);
    if (gInvSpremi[playerid]) strcat(list, "{8AB4F8}Spremi izvadeno oruzje\t\n");
    new len = strlen(list);
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gInv[playerid][i]) continue;
        format(row, sizeof row, T(playerid, "%s\t%d %s"), T(playerid, gItems[i][ITEM_NAME]), gInv[playerid][i], T(playerid, gItems[i][ITEM_UNIT]));
        if (DodajStupac(list, len, row)) gInvList[playerid][count++] = i;
    }
    gInvCount[playerid] = count;
    if (!count && !gInvSpremi[playerid]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Inventar je prazan. Novac: $%d."), gNovac[playerid]);
    new title[48];
    format(title, sizeof title, T(playerid, "Inventar - novac: $%d"), gNovac[playerid]);
    return PrikaziDialog(playerid, DLG_INVENTAR, DIALOG_STYLE_TABLIST_HEADERS, title, list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static PredmetOpcije(playerid)
{
    new item = gInvItem[playerid], title[64];
    if (!gInv[playerid][item]) return PrikaziInventar(playerid);
    format(title, sizeof title, T(playerid, "%s (%d %s)"), T(playerid, gItems[item][ITEM_NAME]), gInv[playerid][item], T(playerid, gItems[item][ITEM_UNIT]));
    return PrikaziDialog(playerid, DLG_INVENTAR_PREDMET, DIALOG_STYLE_LIST, title, T(playerid, "Koristi\nDaj igracu\nBaci"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:inventar(playerid, response, listitem)
{
    new empty[1];
    if (!response) return 1;
    if (gInvSpremi[playerid] && listitem == 0)
    {
        cmd_spremi(playerid, empty);
        return PrikaziInventar(playerid);
    }
    if (gInvSpremi[playerid]) listitem--;
    if (listitem < 0 || listitem >= gInvCount[playerid]) return 1;
    gInvItem[playerid] = gInvList[playerid][listitem];
    return PredmetOpcije(playerid);
}

DLG:inventar_predmet(playerid, response, listitem)
{
    if (!response) return PrikaziInventar(playerid);
    switch (listitem)
    {
        case 0:
        {
            new item = gInvItem[playerid];
            KoristiItem(playerid, item);
            if (item == ITEM_KANISTER_BENZIN || item == ITEM_KANISTER_DIZEL) return 1;
            return PredmetOpcije(playerid);
        }
        case 1: return PrikaziDialog(playerid, DLG_INVENTAR_DAJ, DIALOG_STYLE_INPUT, T(playerid, "Daj igracu"), T(playerid, "Unesite: [id/ime] [kolicina]\nIgrac mora biti blizu vas."), T(playerid, "Daj"), T(playerid, "Natrag"));
        case 2: return PrikaziDialog(playerid, DLG_INVENTAR_BACI, DIALOG_STYLE_INPUT, T(playerid, "Baci"), T(playerid, "Unesite kolicinu koju bacate:"), T(playerid, "Baci"), T(playerid, "Natrag"));
    }
    return PredmetOpcije(playerid);
}

DLG:inventar_daj(playerid, response, const inputtext[])
{
    new idx = 0, p1[32], p2[16], item = gInvItem[playerid];
    if (!response) return PredmetOpcije(playerid);
    if (!SljedeciParam(inputtext, idx, p1) || !SljedeciParam(inputtext, idx, p2) || !ProvjeriBroj(p2) || strlen(p2) > 9)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [id/ime] [kolicina]"));
        return PredmetOpcije(playerid);
    }
    new amount = strval(p2), target = ProvjetiBlizinuIgraca(playerid, p1);
    if (target == INVALID_PLAYER_ID) return PredmetOpcije(playerid);
    if (!GiveItem(playerid, item, -amount))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko."));
        return PredmetOpcije(playerid);
    }
    if (!GiveItem(target, item, amount))
    {
        GiveItem(playerid, item, amount);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "%s ne moze nositi toliko (najvise %d %s, ukupno %d pusaka)."), GetRPIme(target), gItems[item][ITEM_MAX], T(playerid, gItems[item][ITEM_UNIT]), MAX_PUSAKA);
        return PredmetOpcije(playerid);
    }
    SendClientMessage(target, COLOR_INFO, T(target, "%s vam je dao %d %s %s."), GetRPIme(playerid), amount, T(target, gItems[item][ITEM_UNIT]), T(target, gItems[item][ITEM_NAME]));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s nesto predaje %s.", GetRPIme(playerid), GetRPIme(target), 1);
    return PredmetOpcije(playerid);
}

DLG:inventar_baci(playerid, response, const inputtext[])
{
    new item = gInvItem[playerid], amount = strval(inputtext);
    if (!response) return PredmetOpcije(playerid);
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 9 || amount <= 0 || !GiveItem(playerid, item, -amount))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna kolicina."));
        return PredmetOpcije(playerid);
    }
    BaciNaPod(playerid, item, amount);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, item == ITEM_MOBITEL ? ("* %s vadi SIM karticu i baca mobitel.") : ("* %s nesto baca."), GetRPIme(playerid), "", 1);
    return PredmetOpcije(playerid);
}

#define BACENO_MAX          200
#define BACENO_DOMET        1.5
#define BACENO_STREAM       30.0
#define BACENO_NAPRIJED     0.6
#define BACENO_NOVAC        -1
#define BACENO_NOVAC_MODEL  1212

enum E_BACENO { bnPredmet, bnKolicina, STREAMER_TAG_OBJECT:bnObjekt, STREAMER_TAG_AREA:bnZona, Float:bnX, Float:bnY, Float:bnZ, bnSvijet }
static gBaceno[BACENO_MAX][E_BACENO];
static gBacenoSljedeci;

static BaciNaPod(playerid, item, amount)
{
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    new Float:px = x - BACENO_NAPRIJED * floatsin(a, degrees), Float:py = y + BACENO_NAPRIJED * floatcos(a, degrees);
    if (Kolizija_Zraka(x, y, z, px, py, z)) { px = x; py = y; }
    z = HodTlo(px, py, z) - 0.95;
    new b = gBacenoSljedeci;
    gBacenoSljedeci = (b + 1) % BACENO_MAX;
    BacenoMakni(b);
    gBaceno[b][bnPredmet] = item;
    gBaceno[b][bnKolicina] = amount;
    gBaceno[b][bnX] = px;
    gBaceno[b][bnY] = py;
    gBaceno[b][bnZ] = z;
    gBaceno[b][bnSvijet] = GetPlayerVirtualWorld(playerid);
    gBaceno[b][bnObjekt] = CreateDynamicObject(gItems[item][ITEM_MODEL], px, py, z, gItems[item][ITEM_WEAPON] ? 90.0 : 0.0, 0.0, a, gBaceno[b][bnSvijet], GetPlayerInterior(playerid), -1, BACENO_STREAM, BACENO_STREAM);
    gBaceno[b][bnZona] = InterakcijaZona(px, py, z + 0.95, BACENO_DOMET, gBaceno[b][bnSvijet], GetPlayerInterior(playerid), SAVJET_PODIGNI);
}

static BacenoMakni(b)
{
    if (!gBaceno[b][bnKolicina]) return;
    DestroyDynamicObject(gBaceno[b][bnObjekt]);
    if (gBaceno[b][bnZona]) DestroyDynamicArea(gBaceno[b][bnZona]);
    gBaceno[b][bnZona] = STREAMER_TAG_AREA:0;
    gBaceno[b][bnKolicina] = 0;
}

BaciNovac(Float:x, Float:y, Float:z, svijet, unutra, iznos)
{
    if (iznos <= 0) return;
    new b = gBacenoSljedeci;
    gBacenoSljedeci = (b + 1) % BACENO_MAX;
    BacenoMakni(b);
    z = HodTlo(x, y, z) - 0.95;
    gBaceno[b][bnPredmet] = BACENO_NOVAC;
    gBaceno[b][bnKolicina] = iznos;
    gBaceno[b][bnX] = x;
    gBaceno[b][bnY] = y;
    gBaceno[b][bnZ] = z;
    gBaceno[b][bnSvijet] = svijet;
    gBaceno[b][bnObjekt] = CreateDynamicObject(BACENO_NOVAC_MODEL, x, y, z + 0.1, 0.0, 0.0, float(random(360)), svijet, unutra, -1, BACENO_STREAM, BACENO_STREAM);
    gBaceno[b][bnZona] = InterakcijaZona(x, y, z + 0.95, BACENO_DOMET, svijet, unutra, SAVJET_PODIGNI);
}

bool:BacenoPodigni(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return false;
    new b = -1, Float:bd = BACENO_DOMET, vw = GetPlayerVirtualWorld(playerid);
    for (new i = 0; i < BACENO_MAX; i++)
    {
        if (!gBaceno[i][bnKolicina] || gBaceno[i][bnSvijet] != vw) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, gBaceno[i][bnX], gBaceno[i][bnY], gBaceno[i][bnZ]);
        if (d < bd) { bd = d; b = i; }
    }
    if (b == -1) return false;
    new item = gBaceno[b][bnPredmet], amount = gBaceno[b][bnKolicina];
    if (item == BACENO_NOVAC)
    {
        if (!DajNovac(playerid, amount))
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete primiti toliko novca."));
            return true;
        }
        BacenoMakni(b);
        SavjetMakni(playerid);
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, false, false, false, false, 0);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s nesto podize s poda.", GetRPIme(playerid), "", 1);
        return true;
    }
    if (!GiveItem(playerid, item, amount))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        return true;
    }
    BacenoMakni(b);
    SavjetMakni(playerid);
    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, false, false, false, false, 0);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Podigli ste %d %s %s."), amount, T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s nesto podize s poda.", GetRPIme(playerid), "", 1);
    return true;
}

CMD:frisk(playerid, params[])
{
    new idx = 0, p1[32], body[128];
    if (!gUlogiran[playerid]) return 1;
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime]"));
    new target = ProvjetiBlizinuIgraca(playerid, p1);
    if (target == INVALID_PLAYER_ID) return 1;
    if (target == playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to napraviti sami sebi."));
    gFriskOd[target] = playerid + 1;
    format(body, sizeof body, T(target, "%s vas zeli pretresti. Dopustate li?"), GetRPIme(playerid));
    PrikaziDialog(target, DLG_FRISK, DIALOG_STYLE_MSGBOX, T(target, "Pretres"), body, T(target, "Prihvati"), T(target, "Odbij"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zahtjev za pretres poslan igracu %s."), GetRPIme(target));
    return 1;
}

DLG:frisk(playerid, response)
{
    new p = gFriskOd[playerid] - 1, Float:x, Float:y, Float:z, Float:px, Float:py, Float:pz;
    gFriskOd[playerid] = 0;
    if (p < 0 || !gUlogiran[p]) return 1;
    if (!response) return SendClientMessage(p, COLOR_ERROR, T(p, "%s je odbio pretres."), GetRPIme(playerid));
    GetPlayerPos(playerid, x, y, z);
    if (GetPlayerDistanceFromPoint(p, x, y, z) > CHAT_RANGE) return SendClientMessage(p, COLOR_ERROR, T(p, "Igrac nije u vasoj blizini."));
    PosaljiBlizu(p, CHAT_RANGE, COLOR_ME, "* %s pretresa %s.", GetRPIme(p), GetRPIme(playerid), 1);
    GetPlayerPos(p, px, py, pz);
    SetPlayerFacingAngle(p, atan2(y - py, x - px) - 90.0);
    ApplyAnimation(p, "POLICE", "plc_drgbst_01", 4.1, true, false, false, false, 0);
    ApplyAnimation(playerid, "POLICE", "crm_drgbst_01", 4.1, true, false, false, false, 0);
    SetTimerEx("FriskEnd", FRISK_MS, false, "iiii", p, playerid, gAccountId[p], gAccountId[playerid]);
    return 1;
}

forward FriskEnd(p, playerid, racunP, racun);
public FriskEnd(p, playerid, racunP, racun)
{
    if (!gUlogiran[p] || !gUlogiran[playerid] || gAccountId[p] != racunP || gAccountId[playerid] != racun) return 1;
    ClearAnimations(p);
    ClearAnimations(playerid);
    VratiLisice(playerid);
    gFriskMeta[p] = playerid;
    gFriskRacun[p] = racun;
    return FriskPrikazi(p, playerid);
}

static const gDroge[] = { ITEM_MARIHUANA, ITEM_KOKAIN, ITEM_SJEME, ITEM_METH, ITEM_KOKA, ITEM_KEMIKALIJE };
enum { FA_VRECA = 1, FA_DROGA, FA_ORUZJE }

static bool:FriskDroga(t)
{
    for (new i = 0; i < sizeof gDroge; i++) if (gInv[t][gDroge[i]]) return true;
    return false;
}

static bool:FriskVatreno(item)
{
    return gItems[item][ITEM_AMMO] != -1 || (item >= ITEM_MUN_PISTOLJ && item <= ITEM_MUN_PUSKA);
}

static bool:FriskOruzje(t)
{
    for (new i = 0; i < BROJ_ITEMA; i++) if (gInv[t][i] && FriskVatreno(i)) return true;
    return false;
}

static bool:FriskDozvola(t)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT oruzje_dozvola FROM accounts WHERE id = %d", gAccountId[t]), bool:dozvola = DB_GetRowCount(res) && DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    return dozvola;
}

static FriskPrikazi(p, playerid)
{
    new list[1024], row[96], title[64], len = 0, n = 0, bool:policija = PolicajacNaDuznosti(p);
    if (policija)
    {
        if (BankaVrecaIznos(playerid))
        {
            format(row, sizeof row, T(p, "{FF6347}Oduzmi vrecu novca ($%d)"), BankaVrecaIznos(playerid));
            DodajStupac(list, len, row);
            gFriskAkcija[p][n++] = FA_VRECA;
        }
        if (FriskDroga(playerid))
        {
            DodajStupac(list, len, T(p, "{FF6347}Oduzmi drogu"));
            gFriskAkcija[p][n++] = FA_DROGA;
        }
        if (FriskOruzje(playerid) && !FriskDozvola(playerid))
        {
            DodajStupac(list, len, T(p, "{FF6347}Oduzmi oruzje (nema dozvolu)"));
            gFriskAkcija[p][n++] = FA_ORUZJE;
        }
    }
    gFriskAkcija[p][n] = 0;
    if (!policija && BankaVrecaIznos(playerid))
    {
        format(row, sizeof row, T(p, "Vreca novca ($%d)"), BankaVrecaIznos(playerid));
        DodajStupac(list, len, row);
    }
    new predmeta = 0;
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gInv[playerid][i]) continue;
        format(row, sizeof row, "%s - %d %s", T(p, gItems[i][ITEM_NAME]), gInv[playerid][i], T(p, gItems[i][ITEM_UNIT]));
        DodajStupac(list, len, row);
        predmeta++;
    }
    format(title, sizeof title, T(p, "Pretres: %s"), GetRPIme(playerid));
    if (!policija) return PrikaziDialog(p, DLG_NONE, DIALOG_STYLE_MSGBOX, title, len ? list : T(p, "Nista nije pronadeno."), T(p, "Zatvori"));
    if (!n && !predmeta && !BankaVrecaIznos(playerid)) return PrikaziDialog(p, DLG_NONE, DIALOG_STYLE_MSGBOX, title, T(p, "Nista nije pronadeno."), T(p, "Zatvori"));
    return PrikaziDialog(p, DLG_FRISK_LISTA, DIALOG_STYLE_LIST, title, list, T(p, "Odaberi"), T(p, "Zatvori"));
}

DLG:frisk_lista(playerid, response, listitem)
{
    new t = gFriskMeta[playerid], Float:x, Float:y, Float:z;
    if (!response || t < 0 || t >= MAX_PLAYERS || !gUlogiran[t] || gAccountId[t] != gFriskRacun[playerid] || !PolicajacNaDuznosti(playerid)) return 1;
    GetPlayerPos(t, x, y, z);
    if (!IsPlayerInRangeOfPoint(playerid, 3.0, x, y, z)) return ObavijestGreska(playerid, T(playerid, "Igrac nije u vasoj blizini.")), 1;
    if (listitem < 0 || listitem >= sizeof gFriskAkcija[]) return FriskPrikazi(playerid, t);
    for (new k = 0; k <= listitem; k++) if (!gFriskAkcija[playerid][k]) return FriskPrikazi(playerid, t);
    switch (gFriskAkcija[playerid][listitem])
    {
        case FA_VRECA:
        {
            new iznos = BankaVrecaOduzmi(t, playerid), tekst[96];
            if (iznos)
            {
                format(tekst, sizeof tekst, T(playerid, "Oduzeli ste vrecu novca ($%d)."), iznos);
                ObavijestInfo(playerid, T(playerid, "Pretres"), tekst);
                ObavijestGreska(t, T(t, "Policija vam je oduzela vrecu novca."));
            }
        }
        case FA_DROGA:
        {
            for (new i = 0; i < sizeof gDroge; i++) if (gInv[t][gDroge[i]]) GiveItem(t, gDroge[i], -gInv[t][gDroge[i]]);
            ObavijestInfo(playerid, T(playerid, "Pretres"), T(playerid, "Oduzeli ste drogu."));
            ObavijestGreska(t, T(t, "Policija vam je oduzela drogu."));
        }
        case FA_ORUZJE:
        {
            if (FriskDozvola(t)) return FriskPrikazi(playerid, t);
            OruzjeSpremiVatreno(t);
            for (new i = 0; i < BROJ_ITEMA; i++) if (gInv[t][i] && FriskVatreno(i)) GiveItem(t, i, -gInv[t][i]);
            ObavijestInfo(playerid, T(playerid, "Pretres"), T(playerid, "Oduzeli ste oruzje i municiju."));
            ObavijestGreska(t, T(t, "Policija vam je oduzela oruzje i municiju."));
        }
    }
    return FriskPrikazi(playerid, t);
}

CMD:inventar(playerid, params[])
{
    if (IsRanjen(playerid)) return 1;
    if (!gUlogiran[playerid]) return 1;
    return PrikaziInventar(playerid);
}

CMD:inv(playerid, params[])
{
    return cmd_inventar(playerid, params);
}

static bool:PredmetZaAdmina(item)
{
    if (gItems[item][ITEM_WEAPON] || (item >= ITEM_MUN_PISTOLJ && item <= ITEM_MUN_PUSKA)) return false;
    if (item >= ITEM_NAOCALE1 && item <= ITEM_KAPA3) return false;
    if ((item >= ITEM_SAT1 && item <= ITEM_OGRLICA) || item == ITEM_ZLATNE_NAOCALE || item == ITEM_SREBRNE_NAOCALE) return false;
    return true;
}

CMD:dajoruzje(playerid, params[])
{
    new idx = 0, p1[32], p2[8], p3[8];
    if (!ProvjeriOvlasti(playerid, ALVL_MONEY)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2))
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /dajoruzje [id/ime] [oruzje] [municija]"));
        for (new i = 0; i < BROJ_ITEMA; i++) if (gItems[i][ITEM_WEAPON]) SendClientMessage(playerid, COLOR_INFO, "  %d - %s", i, T(playerid, gItems[i][ITEM_NAME]));
        return 1;
    }
    new item = strval(p2), target = FindPlayer(p1), municija = SljedeciParam(params, idx, p3) && ProvjeriBroj(p3) ? strval(p3) : 0;
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (item < 0 || item >= BROJ_ITEMA || !gItems[item][ITEM_WEAPON]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "To nije oruzje."));
    if (!GiveItem(target, item, 1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac ne moze nositi vise tog oruzja."));
    new ammo = gItems[item][ITEM_AMMO];
    if (ammo != -1 && municija > 0)
    {
        if (municija > gItems[ammo][ITEM_MAX] - gInv[target][ammo]) municija = gItems[ammo][ITEM_MAX] - gInv[target][ammo];
        if (municija > 0) GiveItem(target, ammo, municija);
    }
    else municija = 0;
    printf("[MRP] oruzje: %d -> %d %s +%d municije", playerid, target, gItems[item][ITEM_NAME], municija);
    SendClientMessage(target, COLOR_INFO, T(target, "Admin vam je dao: %s (%d municije)."), T(target, gItems[item][ITEM_NAME]), municija);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Dali ste %s igracu %s (%d municije)."), T(playerid, gItems[item][ITEM_NAME]), GetRPIme(target), municija);
}

AdminGiveItem(playerid, const params[])
{
    new idx = 0, p1[32], p2[8], p3[16];
    if (!ProvjeriOvlasti(playerid, ALVL_MONEY)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || !SljedeciParam(params, idx, p3)
        || strlen(p3) > 9 + _:(p3[0] == '-') || !(ProvjeriBroj(p3) || (p3[0] == '-' && ProvjeriBroj(p3[1]))))
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [predmet] [kolicina, negativna oduzima]"));
        for (new i = 0; i < BROJ_ITEMA; i++) if (PredmetZaAdmina(i)) SendClientMessage(playerid, COLOR_INFO, T(playerid, "  %d - %s (najvise %d %s)"), i, T(playerid, gItems[i][ITEM_NAME]), gItems[i][ITEM_MAX], T(playerid, gItems[i][ITEM_UNIT]));
        return 1;
    }
    new item = strval(p2), amount = strval(p3), target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (item >= BROJ_ITEMA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Predmet ne postoji (0-%d)."), BROJ_ITEMA - 1);
    if (!PredmetZaAdmina(item)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Taj predmet se ne daje ovdje. Oruzje: /dajoruzje."));
    if (!amount || !GiveItem(target, item, amount))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna kolicina (igrac ima %d, najvise %d)."), gInv[target][item], gItems[item][ITEM_MAX]);
    new an[MAX_PLAYER_NAME + 1], tn[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, an, sizeof an);
    GetPlayerName(target, tn, sizeof tn);
    printf("[MRP] predmet: %s -> %s %s %d (sada %d)", an, tn, gItems[item][ITEM_NAME], amount, gInv[target][item]);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s sada ima %d %s %s."), tn, gInv[target][item], T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]));
    return 1;
}

#define ORUZJE_SLOTOVI        13
#define ORUZJE_TRPLJENJE_MS   3000
#define ORUZJE_MUNICIJA_VISAK 2
#define AC_ORUZJE             15
#define AC_MUNICIJA_DODANA    16
#define AC_MUNICIJA_BESKRAJ   17

forward bool:SluzbenoOruzje(playerid, WEAPON:w);

static gEquipPredmet[MAX_PLAYERS][ORUZJE_SLOTOVI];
static gEquipAmmo[MAX_PLAYERS][ORUZJE_SLOTOVI];
static gOruzjeLoseOd[MAX_PLAYERS], gMunicijaLoseOd[MAX_PLAYERS];

OruzjeConnect(playerid)
{
    for (new s = 0; s < ORUZJE_SLOTOVI; s++) gEquipPredmet[playerid][s] = -1;
    gOruzjeLoseOd[playerid] = gMunicijaLoseOd[playerid] = 0;
}

BrojOruzja(playerid)
{
    new count = 0;
    for (new i = 0; i < BROJ_ITEMA; i++) if (gItems[i][ITEM_AMMO] != -1) count += gInv[playerid][i];
    return count;
}

StoreSlot(playerid, slot)
{
    new item = gEquipPredmet[playerid][slot], ammoItem = gItems[item][ITEM_AMMO], WEAPON:weapon, ammo;
    GetPlayerWeaponData(playerid, WEAPON_SLOT:slot, weapon, ammo);
    gEquipPredmet[playerid][slot] = -1;
    if (ammoItem != -1 && _:weapon == gItems[item][ITEM_WEAPON])
        GiveItem(playerid, ammoItem, max(0, min(min(ammo, gEquipAmmo[playerid][slot]), gItems[ammoItem][ITEM_MAX] - gInv[playerid][ammoItem])));
    else if (ammoItem != -1)
        GiveItem(playerid, ammoItem, max(0, min(gEquipAmmo[playerid][slot], gItems[ammoItem][ITEM_MAX] - gInv[playerid][ammoItem])));
    RemovePlayerWeapon(playerid, WEAPON:gItems[item][ITEM_WEAPON]);
}

bool:ImaIzvadenoOruzje(playerid)
{
    for (new s = 0; s < ORUZJE_SLOTOVI; s++) if (gEquipPredmet[playerid][s] != -1) return true;
    return false;
}

OruzjeSpremiVatreno(playerid)
{
    for (new slot = 0; slot < ORUZJE_SLOTOVI; slot++)
    {
        new item = gEquipPredmet[playerid][slot];
        if (item != -1 && gItems[item][ITEM_AMMO] != -1) StoreSlot(playerid, slot);
    }
}

StoreUtor(playerid, slot)
{
    if (gEquipPredmet[playerid][slot] != -1) StoreSlot(playerid, slot);
}

StoreItem(playerid, item)
{
    new slot = _:GetWeaponSlot(WEAPON:gItems[item][ITEM_WEAPON]);
    if (gEquipPredmet[playerid][slot] == item) StoreSlot(playerid, slot);
}

IzvadiOruzje(playerid, item)
{
    new weapon = gItems[item][ITEM_WEAPON], ammoItem = gItems[item][ITEM_AMMO];
    new slot = _:GetWeaponSlot(WEAPON:weapon);
    if (gEquipPredmet[playerid][slot] != -1) StoreSlot(playerid, slot);
    new ammo = ammoItem == -1 ? 1 : gInv[playerid][ammoItem];
    if (!ammo) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate municije (%s)."), T(playerid, gItems[ammoItem][ITEM_NAME]));
    if (ammoItem != -1) GiveItem(playerid, ammoItem, -ammo);
    gEquipPredmet[playerid][slot] = item;
    gEquipAmmo[playerid][slot] = ammo;
    GivePlayerWeapon(playerid, WEAPON:weapon, ammo);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s vadi %s.", GetRPIme(playerid), gItems[item][ITEM_NAME], 2);
    return 1;
}

OruzjeLegit(playerid, WEAPON:w)
{
    if (w == WEAPON_FIST || w == WEAPON_PARACHUTE || SluzbenoOruzje(playerid, w)) return true;
    new slot = _:GetWeaponSlot(w);
    if (slot < 0 || slot >= ORUZJE_SLOTOVI) return false;
    new item = gEquipPredmet[playerid][slot];
    return item != -1 && gItems[item][ITEM_WEAPON] == _:w && (gItems[item][ITEM_AMMO] == -1 || gEquipAmmo[playerid][slot] >= 0);
}

OruzjeServerMunicija(playerid, WEAPON:w)
{
    new slot = _:GetWeaponSlot(w);
    if (slot < 0 || slot >= ORUZJE_SLOTOVI) return -1;
    new item = gEquipPredmet[playerid][slot];
    if (item == -1 || gItems[item][ITEM_WEAPON] != _:w || gItems[item][ITEM_AMMO] == -1) return -1;
    return gEquipAmmo[playerid][slot];
}

bool:OruzjePucanj(playerid, WEAPON:w)
{
    if (!OruzjeLegit(playerid, w)) return false;
    new slot = _:GetWeaponSlot(w);
    if (slot < 0 || slot >= ORUZJE_SLOTOVI) return true;
    new item = gEquipPredmet[playerid][slot];
    if (item == -1 || gItems[item][ITEM_WEAPON] != _:w || gItems[item][ITEM_AMMO] == -1) return true;
    if (--gEquipAmmo[playerid][slot] >= 0) return true;
    OnCheatDetected(playerid, "", 0, AC_MUNICIJA_BESKRAJ);
    return false;
}

OruzjeNadzor(playerid, WEAPON:w)
{
    new sad = GetTickCount();
    if (!OruzjeLegit(playerid, w))
    {
        gMunicijaLoseOd[playerid] = 0;
        if (!gOruzjeLoseOd[playerid]) gOruzjeLoseOd[playerid] = sad;
        else if (sad - gOruzjeLoseOd[playerid] >= ORUZJE_TRPLJENJE_MS)
        {
            gOruzjeLoseOd[playerid] = 0;
            RemovePlayerWeapon(playerid, w);
            OnCheatDetected(playerid, "", 0, AC_ORUZJE);
        }
        return;
    }
    gOruzjeLoseOd[playerid] = 0;
    new slot = _:GetWeaponSlot(w), item = slot >= 0 && slot < ORUZJE_SLOTOVI ? gEquipPredmet[playerid][slot] : -1;
    if (item == -1 || gItems[item][ITEM_WEAPON] != _:w || gItems[item][ITEM_AMMO] == -1 || GetPlayerAmmo(playerid) <= gEquipAmmo[playerid][slot] + ORUZJE_MUNICIJA_VISAK)
    {
        gMunicijaLoseOd[playerid] = 0;
        return;
    }
    if (!gMunicijaLoseOd[playerid]) gMunicijaLoseOd[playerid] = sad;
    else if (sad - gMunicijaLoseOd[playerid] >= ORUZJE_TRPLJENJE_MS)
    {
        gMunicijaLoseOd[playerid] = 0;
        SetPlayerAmmo(playerid, w, gEquipAmmo[playerid][slot]);
        OnCheatDetected(playerid, "", 0, AC_MUNICIJA_DODANA);
    }
}

SpremiSvoOruzje(playerid)
{
    new count = 0;
    for (new s = 0; s < ORUZJE_SLOTOVI; s++)
        if (gEquipPredmet[playerid][s] != -1) { StoreSlot(playerid, s); count++; }
    return count;
}

OruzjeDeath(playerid)
{
    for (new s = 0; s < ORUZJE_SLOTOVI; s++)
    {
        new item = gEquipPredmet[playerid][s];
        if (item == -1) continue;
        gEquipPredmet[playerid][s] = -1;
        GiveItem(playerid, item, -1);
    }
}

CMD:spremi(playerid, params[])
{
    if (!SpremiSvoOruzje(playerid)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Nemate izvadeno oruzje iz inventara."));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s sprema oruzje.", GetRPIme(playerid), "", 1);
    return 1;
}

#define DROGA_TRAJE_MS      300000
#define DROGA_MARIHUANA     20.0
#define DROGA_KOKAIN_STETA  0.7
#define DROGA_METH_VJESTINA 200

#define OVISNOST_S          3600
#define OVISNOST_EPIZODA    6
#define OVISNOST_STETA      10.0
#define VIZUAL_MS           90000
#define HALUC_MAX           2
#define HALUC_TRAJE_MS      8000

static gKokainDo[MAX_PLAYERS], gMethDo[MAX_PLAYERS], gMethVjestina[MAX_PLAYERS][11];
static gOvisnostSljedeci[MAX_PLAYERS], gOvisnostEpizoda[MAX_PLAYERS], bool:gOvisnostUcitana[MAX_PLAYERS];
static gVizualDo[MAX_PLAYERS], gVizualVrsta[MAX_PLAYERS], PlayerText:gVizualTd[MAX_PLAYERS] = { PlayerText:INVALID_TEXT_DRAW, ... }, gVizualKorak[MAX_PLAYERS];
static gHaluc[MAX_PLAYERS][HALUC_MAX], gHalucOd[MAX_PLAYERS][HALUC_MAX], gHalucSljedeca[MAX_PLAYERS];
static const gHalucSkin[] = { 7, 14, 23, 29, 47, 78, 134, 162, 200, 230 };

static bool:DrogaTraje(kraj)
{
    return kraj && GetTickCount() - kraj < 0;
}

static MethVjestineVrati(playerid)
{
    if (!gMethDo[playerid]) return;
    gMethDo[playerid] = 0;
    for (new s = 0; s < 11; s++) SetPlayerSkillLevel(playerid, WEAPONSKILL:s, gMethVjestina[playerid][s]);
}

static OvisnostSpremi(playerid)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO drug_addiction (account, next, episodes) VALUES (%d, %d, %d)", gAccountId[playerid], gOvisnostSljedeci[playerid], gOvisnostEpizoda[playerid]));
}

static OvisnostUcitaj(playerid)
{
    gOvisnostUcitana[playerid] = true;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS drug_addiction (account INTEGER PRIMARY KEY, next INTEGER NOT NULL, episodes INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT next, episodes FROM drug_addiction WHERE account = %d", gAccountId[playerid]);
    gOvisnostSljedeci[playerid] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    gOvisnostEpizoda[playerid] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 1) : 0;
    DB_FreeResultSet(res);
}

static OvisnostTick(playerid)
{
    if (!gOvisnostUcitana[playerid]) OvisnostUcitaj(playerid);
    if (!gOvisnostSljedeci[playerid] || gettime() < gOvisnostSljedeci[playerid] || IsRanjen(playerid)) return;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return;
    new Float:h;
    GetHP(playerid, h);
    SetHP(playerid, h - OVISNOST_STETA < 5.0 ? 5.0 : h - OVISNOST_STETA);
    ApplyAnimation(playerid, "FOOD", "EAT_Vomit_P", 4.1, false, false, false, false, 0);
    SetPlayerDrunkLevel(playerid, 3000);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s povraca.", GetRPIme(playerid), "", 1);
    if (++gOvisnostEpizoda[playerid] >= OVISNOST_EPIZODA)
    {
        gOvisnostSljedeci[playerid] = 0;
        gOvisnostEpizoda[playerid] = 0;
        ObavijestInfo(playerid, T(playerid, "Droga"), T(playerid, "Prebrodili ste krizu. Vise niste ovisni."));
    }
    else
    {
        gOvisnostSljedeci[playerid] = gettime() + OVISNOST_S;
        ObavijestGreska(playerid, T(playerid, "Apstinencijska kriza! Bez nove doze bit ce vam lose svakih sat vremena."));
    }
    OvisnostSpremi(playerid);
}

static VizualMakni(playerid)
{
    gVizualDo[playerid] = 0;
    if (gVizualTd[playerid] != PlayerText:INVALID_TEXT_DRAW) PlayerTextDrawDestroy(playerid, gVizualTd[playerid]);
    gVizualTd[playerid] = PlayerText:INVALID_TEXT_DRAW;
    SetPlayerDrunkLevel(playerid, 0);
}

static VizualBoja(playerid)
{
    if (gVizualTd[playerid] == PlayerText:INVALID_TEXT_DRAW) return;
    new k = ++gVizualKorak[playerid] % 4, boja;
    switch (gVizualVrsta[playerid])
    {
        case ITEM_MARIHUANA: boja = 0x3CB04418 + k * 4;
        case ITEM_KOKAIN: boja = k == 0 ? 0xFFFFFF40 : 0xB0D8FF14;
        default: boja = k < 2 ? 0xFF603020 : 0x9040FF20;
    }
    PlayerTextDrawBoxColour(playerid, gVizualTd[playerid], boja);
    PlayerTextDrawShow(playerid, gVizualTd[playerid]);
}

static VizualPocni(playerid, item)
{
    if (gVizualTd[playerid] == PlayerText:INVALID_TEXT_DRAW)
    {
        gVizualTd[playerid] = CreatePlayerTextDraw(playerid, 0.0, 0.0, "_");
        PlayerTextDrawLetterSize(playerid, gVizualTd[playerid], 0.0, 50.0);
        PlayerTextDrawTextSize(playerid, gVizualTd[playerid], 640.0, 0.0);
        PlayerTextDrawUseBox(playerid, gVizualTd[playerid], true);
    }
    gVizualVrsta[playerid] = item;
    gVizualDo[playerid] = GetTickCount() + (item == ITEM_METH ? DROGA_TRAJE_MS : VIZUAL_MS);
    gHalucSljedeca[playerid] = GetTickCount() + 15000;
    VizualBoja(playerid);
}

static HalucMakni(playerid, k)
{
    if (gHaluc[playerid][k] == INVALID_LIK) return;
    Lik_Ukloni(gHaluc[playerid][k]);
    gHaluc[playerid][k] = INVALID_LIK;
}

static HalucTick(playerid)
{
    new Float:x, Float:y, Float:z, Float:hx, Float:hy, Float:hz, sad = GetTickCount();
    GetPlayerPos(playerid, x, y, z);
    for (new k = 0; k < HALUC_MAX; k++)
    {
        if (gHaluc[playerid][k] == INVALID_LIK) continue;
        if (sad - gHalucOd[playerid][k] > HALUC_TRAJE_MS || !Lik_Pozicija(gHaluc[playerid][k], hx, hy, hz) || VectorSize(hx - x, hy - y, 0.0) < 1.6)
        {
            HalucMakni(playerid, k);
            PlayerPlaySound(playerid, 1136, 0.0, 0.0, 0.0);
            continue;
        }
        Lik_IdiDo(gHaluc[playerid][k], x, y, z, LIK_SPRINT);
    }
    if (gVizualVrsta[playerid] != ITEM_METH || sad - gHalucSljedeca[playerid] < 0 || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return;
    gHalucSljedeca[playerid] = sad + 25000 + random(20000);
    new Float:a;
    GetPlayerFacingAngle(playerid, a);
    for (new k = 0; k < HALUC_MAX; k++)
    {
        if (gHaluc[playerid][k] != INVALID_LIK || (k && random(2))) continue;
        new Float:kut = a + 180.0 + float(random(120) - 60), Float:d = 14.0 + float(random(6));
        new lik = Lik_Stvori(gHalucSkin[random(sizeof gHalucSkin)], x - d * floatsin(kut, degrees), y + d * floatcos(kut, degrees), z, kut + 180.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
        if (lik == INVALID_LIK) continue;
        Lik_SamoZa(lik, playerid);
        Lik_Neranjiv(lik, true);
        Lik_IdiDo(lik, x, y, z, LIK_SPRINT);
        gHaluc[playerid][k] = lik;
        gHalucOd[playerid][k] = sad;
    }
}

DrogaUcinakOdlazi(playerid)
{
    for (new k = 0; k < HALUC_MAX; k++) HalucMakni(playerid, k);
    gVizualDo[playerid] = 0;
    gVizualTd[playerid] = PlayerText:INVALID_TEXT_DRAW;
    gOvisnostUcitana[playerid] = false;
}

DrogaUcinak(playerid, item)
{
    if (!gOvisnostUcitana[playerid]) OvisnostUcitaj(playerid);
    if (!gOvisnostSljedeci[playerid]) ObavijestGreska(playerid, T(playerid, "Postali ste ovisni. Ako ne uzmete novu dozu unutar sat vremena, bit ce vam lose."));
    gOvisnostSljedeci[playerid] = gettime() + OVISNOST_S;
    gOvisnostEpizoda[playerid] = 0;
    OvisnostSpremi(playerid);
    VizualPocni(playerid, item);
    SetPlayerDrunkLevel(playerid, item == ITEM_METH ? 4500 : item == ITEM_KOKAIN ? 2300 : 2600);
    switch (item)
    {
        case ITEM_MARIHUANA:
        {
            new Float:o;
            GetArmor(playerid, o);
            SetArmor(playerid, o + DROGA_MARIHUANA > 100.0 ? 100.0 : o + DROGA_MARIHUANA);
            ObavijestInfo(playerid, T(playerid, "Droga"), T(playerid, "Marihuana vas je opustila: +20 oklopa."));
        }
        case ITEM_KOKAIN:
        {
            gKokainDo[playerid] = GetTickCount() + DROGA_TRAJE_MS;
            ObavijestInfo(playerid, T(playerid, "Droga"), T(playerid, "Kokain: 5 minuta primate 30% manje stete."));
        }
        case ITEM_METH:
        {
            if (!gMethDo[playerid]) for (new s = 0; s < 11; s++)
            {
                new tren = GetPlayerSkillLevel(playerid, WEAPONSKILL:s), gornja = s == 0 || s == 4 || s == 6 ? 998 : 999, nova = tren + DROGA_METH_VJESTINA;
                gMethVjestina[playerid][s] = tren;
                if (nova > gornja) nova = gornja;
                if (nova > tren) SetPlayerSkillLevel(playerid, WEAPONSKILL:s, nova);
            }
            gMethDo[playerid] = GetTickCount() + DROGA_TRAJE_MS;
            ObavijestInfo(playerid, T(playerid, "Droga"), T(playerid, "Metamfetamin: 5 minuta bolje rukujete oruzjem."));
        }
    }
}

DrogaUcinakReset(playerid)
{
    gKokainDo[playerid] = 0;
    for (new k = 0; k < HALUC_MAX; k++) HalucMakni(playerid, k);
    VizualMakni(playerid);
    MethVjestineVrati(playerid);
}

DrogaUcinakTick(playerid)
{
    OvisnostTick(playerid);
    if (gVizualDo[playerid])
    {
        if (GetTickCount() - gVizualDo[playerid] >= 0) VizualMakni(playerid);
        else
        {
            VizualBoja(playerid);
            if (GetPlayerDrunkLevel(playerid) < 2100) SetPlayerDrunkLevel(playerid, gVizualVrsta[playerid] == ITEM_METH ? 4000 : 2200);
        }
    }
    HalucTick(playerid);
    if (gKokainDo[playerid] && !DrogaTraje(gKokainDo[playerid]))
    {
        gKokainDo[playerid] = 0;
        ObavijestInfo(playerid, T(playerid, "Droga"), T(playerid, "Ucinak kokaina je prosao."));
    }
    if (gMethDo[playerid] && !DrogaTraje(gMethDo[playerid]))
    {
        MethVjestineVrati(playerid);
        ObavijestInfo(playerid, T(playerid, "Droga"), T(playerid, "Ucinak metamfetamina je prosao."));
    }
}

DrogaUcinakConnect(playerid)
{
    gKokainDo[playerid] = 0;
    gMethDo[playerid] = 0;
    gVizualDo[playerid] = 0;
    gVizualTd[playerid] = PlayerText:INVALID_TEXT_DRAW;
    gOvisnostUcitana[playerid] = false;
    gOvisnostSljedeci[playerid] = 0;
    for (new k = 0; k < HALUC_MAX; k++) gHaluc[playerid][k] = INVALID_LIK;
}

Float:DrogaStetaFaktor(playerid)
{
    return DrogaTraje(gKokainDo[playerid]) ? DROGA_KOKAIN_STETA : 1.0;
}
