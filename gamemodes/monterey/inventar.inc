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

bool:DajNovac(playerid, amount)
{
    new total = gNovac[playerid] + amount;
    if (total < 0 || total > MAX_KES) return false;
    PostaviNovac(playerid, total);
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
    ITEM_MOBITEL, ITEM_NAOCALE1, ITEM_NAOCALE2, ITEM_NAOCALE3, ITEM_BANDANA1, ITEM_BANDANA2, ITEM_BANDANA3, ITEM_KAPA1, ITEM_KAPA2, ITEM_KAPA3, ITEM_ZETONI, ITEM_KARTICA
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
    { "Bankovna kartica",       "kom", 1,   0,  -1, 1581  }
};
#define BROJ_ITEMA          sizeof (gItems)
#define MAX_PUSAKA          7
#define KORISTENJE_COOLDOWN   60000

new gInv[MAX_PLAYERS][BROJ_ITEMA];
static gInvList[MAX_PLAYERS][BROJ_ITEMA];
static gInvCount[MAX_PLAYERS];
static gInvItem[MAX_PLAYERS];
static gItemUsedAt[MAX_PLAYERS];
static gFriskOd[MAX_PLAYERS];
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
    else if (item >= ITEM_NAOCALE1 && item <= ITEM_KAPA3) OdjecaPredmetNestao(playerid, item);
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
    new Float:v;
    if (gItems[item][ITEM_WEAPON]) return IzvadiOruzje(playerid, item);
    if (item == ITEM_MOBITEL) return MobitelPrebaci(playerid);
    if (item >= ITEM_NAOCALE1 && item <= ITEM_KAPA3) return OdjecaKapaPredmet(playerid, item);
    if (item == ITEM_CRIJEVO) return VoziloIsprazni(playerid);
    if (item == ITEM_KARTICA) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Karticu koristite u banci i na mobitelu (Internet, Banka)."));
    if (item == ITEM_ZETONI) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zetone mijenjate na blagajni kazina."));
    if (item == ITEM_KANISTER) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prazan kanister napunite na pumpi naredbom /tankaj."));
    if (item == ITEM_KANISTER_BENZIN || item == ITEM_KANISTER_DIZEL) return VoziloKanister(playerid, item, -1);
    if (item != ITEM_MARIHUANA && item != ITEM_KOKAIN)
        return SendClientMessage(playerid, COLOR_INFO, item == ITEM_SJEME ? T(playerid, "Sjemenke se sade na teritoriju za uzgoj.") : T(playerid, "Municija se puni kad izvadite oruzje."));
    if (GetTickCount() - gItemUsedAt[playerid] < KORISTENJE_COOLDOWN)
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pricekajte jos %d s."), (KORISTENJE_COOLDOWN - (GetTickCount() - gItemUsedAt[playerid])) / 1000 + 1);
    if (!GiveItem(playerid, item, -1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
    gItemUsedAt[playerid] = GetTickCount();
    switch (item)
    {
        case ITEM_MARIHUANA:
        {
            GetPlayerHealth(playerid, v);
            SetPlayerHealth(playerid, v + 10.0 > 100.0 ? 100.0 : v + 10.0);
            PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pali dzoint.", GetRPIme(playerid), "", 1);
        }
        case ITEM_KOKAIN:
        {
            GetPlayerArmour(playerid, v);
            SetPlayerArmour(playerid, v + 20.0 > 100.0 ? 100.0 : v + 20.0);
            PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s smrka liniju kokaina.", GetRPIme(playerid), "", 1);
        }
    }
    return 1;
}

PrikaziInventar(playerid)
{
    new list[512] = "Predmet\tKolicina\n{8AB4F8}Spremi izvadeno oruzje\t\n", row[64], count = 0;
    new len = strlen(list);
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gInv[playerid][i]) continue;
        format(row, sizeof row, T(playerid, "%s\t%d %s"), T(playerid, gItems[i][ITEM_NAME]), gInv[playerid][i], T(playerid, gItems[i][ITEM_UNIT]));
        if (DodajStupac(list, len, row)) gInvList[playerid][count++] = i;
    }
    gInvCount[playerid] = count;
    if (!count) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Inventar je prazan. Novac: $%d."), gNovac[playerid]);
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
    if (listitem == 0)
    {
        cmd_spremi(playerid, empty);
        return PrikaziInventar(playerid);
    }
    if (--listitem < 0 || listitem >= gInvCount[playerid]) return 1;
    gInvItem[playerid] = gInvList[playerid][listitem];
    return PredmetOpcije(playerid);
}

DLG:inventar_predmet(playerid, response, listitem)
{
    if (!response) return PrikaziInventar(playerid);
    switch (listitem)
    {
        case 0: { KoristiItem(playerid, gInvItem[playerid]); return PredmetOpcije(playerid); }
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

enum E_BACENO { bnPredmet, bnKolicina, STREAMER_TAG_OBJECT:bnObjekt, Float:bnX, Float:bnY, Float:bnZ, bnSvijet }
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
    if (gBaceno[b][bnKolicina]) DestroyDynamicObject(gBaceno[b][bnObjekt]);
    gBaceno[b][bnPredmet] = item;
    gBaceno[b][bnKolicina] = amount;
    gBaceno[b][bnX] = px;
    gBaceno[b][bnY] = py;
    gBaceno[b][bnZ] = z;
    gBaceno[b][bnSvijet] = GetPlayerVirtualWorld(playerid);
    gBaceno[b][bnObjekt] = CreateDynamicObject(gItems[item][ITEM_MODEL], px, py, z, gItems[item][ITEM_WEAPON] ? 90.0 : 0.0, 0.0, a, gBaceno[b][bnSvijet], GetPlayerInterior(playerid), -1, BACENO_STREAM, BACENO_STREAM);
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
    if (!GiveItem(playerid, item, amount))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        return true;
    }
    gBaceno[b][bnKolicina] = 0;
    DestroyDynamicObject(gBaceno[b][bnObjekt]);
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
    new list[640], row[64], title[64], len = 0;
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gInv[playerid][i]) continue;
        format(row, sizeof row, "%s - %d %s", T(p, gItems[i][ITEM_NAME]), gInv[playerid][i], T(p, gItems[i][ITEM_UNIT]));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, T(p, "Pretres: %s"), GetRPIme(playerid));
    return PrikaziDialog(p, DLG_NONE, DIALOG_STYLE_MSGBOX, title, len ? list : T(p, "Nista nije pronadeno."), T(p, "Zatvori"));
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

AdminGiveItem(playerid, const params[])
{
    new idx = 0, p1[32], p2[8], p3[16];
    if (!ProvjeriOvlasti(playerid, ALVL_MONEY)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || !SljedeciParam(params, idx, p3)
        || strlen(p3) > 9 + _:(p3[0] == '-') || !(ProvjeriBroj(p3) || (p3[0] == '-' && ProvjeriBroj(p3[1]))))
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [predmet] [kolicina, negativna oduzima]"));
        for (new i = 0; i < BROJ_ITEMA; i++) SendClientMessage(playerid, COLOR_INFO, T(playerid, "  %d - %s (najvise %d %s)"), i, T(playerid, gItems[i][ITEM_NAME]), gItems[i][ITEM_MAX], T(playerid, gItems[i][ITEM_UNIT]));
        return 1;
    }
    new item = strval(p2), amount = strval(p3), target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (item >= BROJ_ITEMA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Predmet ne postoji (0-%d)."), BROJ_ITEMA - 1);
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

static gEquipPredmet[MAX_PLAYERS][ORUZJE_SLOTOVI];
static gEquipAmmo[MAX_PLAYERS][ORUZJE_SLOTOVI];

OruzjeConnect(playerid)
{
    for (new s = 0; s < ORUZJE_SLOTOVI; s++) gEquipPredmet[playerid][s] = -1;
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
        GiveItem(playerid, ammoItem, min(min(ammo, gEquipAmmo[playerid][slot]), gItems[ammoItem][ITEM_MAX] - gInv[playerid][ammoItem]));
    RemovePlayerWeapon(playerid, WEAPON:gItems[item][ITEM_WEAPON]);
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
    if (w == WEAPON_FIST) return true;
    new slot = _:GetWeaponSlot(w);
    if (slot < 0 || slot >= ORUZJE_SLOTOVI) return false;
    new item = gEquipPredmet[playerid][slot];
    return item != -1 && gItems[item][ITEM_WEAPON] == _:w;
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
