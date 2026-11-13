#define BANKA_MAX           16
#define BANKA_SVIJET        9600
#define BANKA_DOMET         2.0
#define BANKA_KASA_DOMET    2.5
#define BANKA_SKIN          150
#define BANKA_OTVARANJE     100
#define BANKA_KARTICA       50
#define BANKA_KREDIT_MAX    50000
#define BANKA_KAMATA        10
#define BANKA_RATA          10
#define BANKA_KAZNA         5
#define BANKA_DAN           86400
#define BANKA_STEDNJA_MAX   1000
#define BANKA_CUVARI        4
#define BANKA_CUVAR_SKIN    71
#define BANKA_PLJACKASA     4
#define BANKA_PLJACKA_S     20
#define BANKA_PLJACKA_PAUZA 3600
#define BANKA_PLIJEN_MIN    20000
#define BANKA_PLIJEN_RASPON 20000
#define BANKA_WANTED        3

enum E_BANKA { bool:bkIma, bkDb, bkInt, Float:bkX, Float:bkY, Float:bkZ, Float:bkA, Float:bkIx, Float:bkIy, Float:bkIz, bkPljacka, bkPljackaKraj, bkPljackano, bool:bkImaAktor, Float:bkAx, Float:bkAy, Float:bkAz, Float:bkAa, bkAktor,
    STREAMER_TAG_PICKUP:bkPickup, STREAMER_TAG_PICKUP:bkPickupU, STREAMER_TAG_3D_TEXT_LABEL:bkTekst, STREAMER_TAG_3D_TEXT_LABEL:bkTekstU, STREAMER_TAG_3D_TEXT_LABEL:bkTekstA }

enum E_BANKA_INT { biIme[24], biInt, Float:biX, Float:biY, Float:biZ }
static const gBankaInt[][E_BANKA_INT] = {
    { "Palomino Creek banka", 0, 2306.380, -15.240, 26.750 },
    { "Gradski ured", 3, 384.808, 173.804, 1008.382 },
    { "Atrium", 18, 1727.285, -1642.945, 20.225 }
};

enum { BA_OTVORI, BA_UPLATI, BA_ISPLATI, BA_NA_STEDNJU, BA_SA_STEDNJE, BA_POSALJI, BA_KREDIT, BA_OTPLATI, BA_KARTICA }
static const gBankaAkcija[][] = { "Otvori racun ($%d)", "Uplati novac", "Podigni novac", "Prebaci na stednju", "Podigni sa stednje", "Posalji novac", "Podigni kredit", "Otplati kredit", "Nova kartica ($%d)" };

static gBanka[BANKA_MAX][E_BANKA];
static gBankaCuvar[BANKA_MAX][BANKA_CUVARI] = { { INVALID_PLAYER_ID, ... }, ... }, gBankaCuvarDb[BANKA_MAX][BANKA_CUVARI], Float:gBankaCuvarPoz[BANKA_MAX][BANKA_CUVARI][4], gNpcBanka[MAX_PLAYERS] = { -1, ... }, gBankaPoruka[MAX_PLAYERS];
static Float:gApBankaPoz[MAX_PLAYERS][4], gApBanka[MAX_PLAYERS], gBankaMeni[MAX_PLAYERS][sizeof gBankaAkcija], gBankaOdabir[MAX_PLAYERS], gBankaSat;

static BankaDug(iznos)
{
    return iznos + iznos * BANKA_KAMATA / 100;
}

static BankaRata(dug)
{
    return (dug + BANKA_RATA - 1) / BANKA_RATA;
}

static BankaKamata(stednja)
{
    new k = stednja / 200;
    return k > BANKA_STEDNJA_MAX ? BANKA_STEDNJA_MAX : k;
}

InitBanka()
{
    assert(BankaDug(1000) == 1100 && BankaRata(1100) == 110 && BankaRata(1101) == 111 && BankaKamata(100) == 0 && BankaKamata(1000000) == BANKA_STEDNJA_MAX);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS banks (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, ix REAL NOT NULL, iy REAL NOT NULL, iz REAL NOT NULL, Aktor INTEGER NOT NULL DEFAULT 0, ax REAL NOT NULL DEFAULT 0, ay REAL NOT NULL DEFAULT 0, az REAL NOT NULL DEFAULT 0, aa REAL NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_guards (id INTEGER PRIMARY KEY AUTOINCREMENT, bank INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bank_accounts (account INTEGER PRIMARY KEY, balance INTEGER NOT NULL DEFAULT 0, savings INTEGER NOT NULL DEFAULT 0, savings_tick INTEGER NOT NULL DEFAULT 0, loan INTEGER NOT NULL DEFAULT 0, loan_rate INTEGER NOT NULL DEFAULT 0, loan_due INTEGER NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, ix, iy, iz, Aktor, ax, ay, az, aa FROM banks ORDER BY id LIMIT %d", BANKA_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++)
    {
        gBanka[s][bkDb] = DB_GetFieldInt(res, 0);
        gBanka[s][bkX] = DB_GetFieldFloat(res, 1);
        gBanka[s][bkY] = DB_GetFieldFloat(res, 2);
        gBanka[s][bkZ] = DB_GetFieldFloat(res, 3);
        gBanka[s][bkA] = DB_GetFieldFloat(res, 4);
        gBanka[s][bkInt] = DB_GetFieldInt(res, 5);
        gBanka[s][bkIx] = DB_GetFieldFloat(res, 6);
        gBanka[s][bkIy] = DB_GetFieldFloat(res, 7);
        gBanka[s][bkIz] = DB_GetFieldFloat(res, 8);
        BankaStvori(s);
        if (DB_GetFieldInt(res, 9))
        {
            gBanka[s][bkAx] = DB_GetFieldFloat(res, 10);
            gBanka[s][bkAy] = DB_GetFieldFloat(res, 11);
            gBanka[s][bkAz] = DB_GetFieldFloat(res, 12);
            gBanka[s][bkAa] = DB_GetFieldFloat(res, 13);
            BankaAktor(s);
        }
        BankaUcitajCuvare(s);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static BankaStvori(s)
{
    new svijet = BANKA_SVIJET + s;
    gBanka[s][bkIma] = true;
    gBanka[s][bkPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ], 0, 0);
    gBanka[s][bkTekst] = CreateDynamic3DTextLabel("Banka\n[Y]", COLOR_ORG, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gBanka[s][bkPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz], svijet, gBanka[s][bkInt]);
    gBanka[s][bkTekstU] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gBanka[s][bkInt]);
}

static BankaAktor(s)
{
    new svijet = BANKA_SVIJET + s;
    gBanka[s][bkImaAktor] = true;
    gBanka[s][bkAktor] = CreateActor(BANKA_SKIN, gBanka[s][bkAx], gBanka[s][bkAy], gBanka[s][bkAz], gBanka[s][bkAa]);
    SetActorVirtualWorld(gBanka[s][bkAktor], svijet);
    SetActorInvulnerable(gBanka[s][bkAktor], true);
    gBanka[s][bkTekstA] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gBanka[s][bkAx], gBanka[s][bkAy], gBanka[s][bkAz] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gBanka[s][bkInt]);
}

static BankaMakniAktor(s)
{
    if (!gBanka[s][bkImaAktor]) return;
    DestroyActor(gBanka[s][bkAktor]);
    DestroyDynamic3DTextLabel(gBanka[s][bkTekstA]);
    gBanka[s][bkImaAktor] = false;
}

static BankaIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - BANKA_SVIJET;
    return s >= 0 && s < BANKA_MAX && gBanka[s][bkIma] && GetPlayerInterior(playerid) == gBanka[s][bkInt] ? s : -1;
}

static BankaVani(playerid, s)
{
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ]);
    SetPlayerFacingAngle(playerid, gBanka[s][bkA] + 180.0);
    SetCameraBehindPlayer(playerid);
}

static BankaCuvarStvori(s, c)
{
    new name[MAX_PLAYER_NAME + 1];
    format(name, sizeof name, "Cuvar_%d", gBankaCuvarDb[s][c]);
    new npc = NPC_Create(name);
    gBankaCuvar[s][c] = npc;
    if (npc == INVALID_PLAYER_ID) return;
    gNpcBanka[npc] = s * BANKA_CUVARI + c;
    NPC_Spawn(npc);
}

static BankaCuvarMakni(s, c)
{
    new npc = gBankaCuvar[s][c];
    gBankaCuvarDb[s][c] = 0;
    gBankaCuvar[s][c] = INVALID_PLAYER_ID;
    if (npc == INVALID_PLAYER_ID) return;
    gNpcBanka[npc] = -1;
    NPC_Destroy(npc);
}

static BankaUcitajCuvare(s)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM bank_guards WHERE bank = %d ORDER BY id LIMIT %d", gBanka[s][bkDb], BANKA_CUVARI);
    for (new c = 0, n = DB_GetRowCount(res); c < n; c++, DB_SelectNextRow(res))
    {
        gBankaCuvarDb[s][c] = DB_GetFieldInt(res, 0);
        for (new i = 0; i < 4; i++) gBankaCuvarPoz[s][c][i] = DB_GetFieldFloat(res, 1 + i);
        BankaCuvarStvori(s, c);
    }
    DB_FreeResultSet(res);
}

BankaSpawn(npcid)
{
    new k = gNpcBanka[npcid];
    if (k == -1) return;
    new s = k / BANKA_CUVARI, c = k % BANKA_CUVARI;
    NPC_SetSkin(npcid, BANKA_CUVAR_SKIN);
    NPC_SetInterior(npcid, gBanka[s][bkInt]);
    NPC_SetVirtualWorld(npcid, BANKA_SVIJET + s);
    NPC_SetPos(npcid, gBankaCuvarPoz[s][c][0], gBankaCuvarPoz[s][c][1], gBankaCuvarPoz[s][c][2]);
    NPC_SetFacingAngle(npcid, gBankaCuvarPoz[s][c][3]);
    NPC_SetHealth(npcid, 100.0);
    NPC_SetAmmo(npcid, 500);
    NPC_SetWeapon(npcid, WEAPON_COLT45);
    NPC_SetWeaponAccuracy(npcid, WEAPON_COLT45, 0.5);
    NPC_EnableReloading(npcid, true);
}

static BankaEkipa(playerid, s)
{
    if (!gOrg[playerid]) return 0;
    new n = 0;
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && gOrg[i] == gOrg[playerid] && BankaIgraca(i) == s && !IsRanjen(i)) n++;
    return n;
}

bool:BankaBlokira(playerid)
{
    if (IsPlayerNPC(playerid)) return false;
    new s = BankaIgraca(playerid);
    if (s == -1) return false;
    if (gBanka[s][bkPljacka]) return false;
    return BankaEkipa(playerid, s) < BANKA_PLJACKASA;
}

static BankaPljackaStani(s)
{
    gBanka[s][bkPljacka] = 0;
    if (gBanka[s][bkImaAktor]) ClearActorAnimations(gBanka[s][bkAktor]);
    for (new c = 0; c < BANKA_CUVARI; c++)
    {
        new npc = gBankaCuvar[s][c];
        if (npc == INVALID_PLAYER_ID) continue;
        if (NPC_IsDead(npc)) NPC_Respawn(npc);
        else NPC_StopAim(npc);
    }
}

BankaUpdate(playerid)
{
    if (!gUlogiran[playerid] || !gOrg[playerid]) return;
    new s = BankaIgraca(playerid), WEAPON:w = GetPlayerWeapon(playerid);
    if (s == -1 || gBanka[s][bkPljacka] || w < WEAPON_COLT45 || w > WEAPON_MINIGUN) return;
    new meta = GetPlayerTargetPlayer(playerid), bool:cilj = gBanka[s][bkImaAktor] && GetPlayerTargetActor(playerid) == gBanka[s][bkAktor];
    if (!cilj && meta != INVALID_PLAYER_ID && gNpcBanka[meta] != -1 && gNpcBanka[meta] / BANKA_CUVARI == s) cilj = true;
    if (!cilj || BankaEkipa(playerid, s) < BANKA_PLJACKASA) return;
    new sad = gettime();
    if (gBanka[s][bkPljackano] && sad - gBanka[s][bkPljackano] < BANKA_PLJACKA_PAUZA)
    {
        if (sad - gBankaPoruka[playerid] < 10) return;
        gBankaPoruka[playerid] = sad;
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ova banka je nedavno opljackana."));
        return;
    }
    gBanka[s][bkPljacka] = gOrg[playerid];
    gBanka[s][bkPljackaKraj] = sad + BANKA_PLJACKA_S;
    gBanka[s][bkPljackano] = sad;
    if (gBanka[s][bkImaAktor]) ApplyActorAnimation(gBanka[s][bkAktor], "SHOP", "SHP_Rob_HandsUp", 4.1, false, false, false, true, 0);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i]) continue;
        if (gOrg[i] == gOrg[playerid] && BankaIgraca(i) == s)
        {
            for (new k = 0; k < BANKA_WANTED; k++) DodajWanted(i);
            SendClientMessage(i, COLOR_ERROR, T(i, "Pljacka banke je pocela! Izdrzite %d s unutar banke."), BANKA_PLJACKA_S);
        }
        else if (PolicajacNaDuznosti(i)) SendClientMessage(i, COLOR_ERROR, T(i, "Dojava: pljacka banke u tijeku, %s."), CestaZona(gBanka[s][bkX], gBanka[s][bkY]));
    }
}

static BankaPljackaTick()
{
    new sad = gettime();
    for (new s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma] || !gBanka[s][bkPljacka]) continue;
        new org = gBanka[s][bkPljacka], prisutni[MAX_PLAYERS], n = 0;
        for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && gOrg[i] == org && BankaIgraca(i) == s && !IsRanjen(i)) prisutni[n++] = i;
        if (!n)
        {
            for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && gOrg[i] == org) SendClientMessage(i, COLOR_ERROR, T(i, "Pljacka banke je propala."));
            BankaPljackaStani(s);
            continue;
        }
        if (sad >= gBanka[s][bkPljackaKraj])
        {
            new dio = (BANKA_PLIJEN_MIN + random(BANKA_PLIJEN_RASPON)) / n;
            for (new k = 0; k < n; k++)
            {
                DajNovac(prisutni[k], dio);
                SendClientMessage(prisutni[k], COLOR_INFO, T(prisutni[k], "Pljacka uspjela! Vas dio: $%d."), dio);
            }
            BankaPljackaStani(s);
            continue;
        }
        for (new c = 0; c < BANKA_CUVARI; c++)
        {
            new npc = gBankaCuvar[s][c], meta = INVALID_PLAYER_ID, Float:naj = 100.0, Float:x, Float:y, Float:z;
            if (npc == INVALID_PLAYER_ID || NPC_IsDead(npc)) continue;
            NPC_GetPos(npc, x, y, z);
            for (new k = 0; k < n; k++)
            {
                new Float:d = GetPlayerDistanceFromPoint(prisutni[k], x, y, z);
                if (d < naj) { naj = d; meta = prisutni[k]; }
            }
            if (meta != INVALID_PLAYER_ID && !NPC_IsAimingAtPlayer(npc, meta)) NPC_AimAtPlayer(npc, meta, true, 400 + random(400));
        }
    }
}


BankaUlaz(playerid)
{
    new s = BankaIgraca(playerid);
    if (s != -1)
    {
        if (!IsPlayerInRangeOfPoint(playerid, BANKA_DOMET, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz])) return 0;
        BankaVani(playerid, s);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma] || !IsPlayerInRangeOfPoint(playerid, BANKA_DOMET, gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ])) continue;
        SetPlayerInterior(playerid, gBanka[s][bkInt]);
        SetPlayerVirtualWorld(playerid, BANKA_SVIJET + s);
        SetPlayerPos(playerid, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz]);
        SetCameraBehindPlayer(playerid);
        if (gBanka[s][bkImaAktor]) AktorKaze(playerid, gBanka[s][bkAktor], "Sluzbenik", "Dobar dan, izvolite na salter.");
        return 1;
    }
    return 0;
}

static bool:BankaUcitaj(racun, &stanje, &stednja, &kredit, &rata, &rok)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT balance, savings, loan, loan_rate, loan_due FROM bank_accounts WHERE account = %d", racun);
    new bool:ima = DB_GetRowCount(res) > 0;
    if (ima)
    {
        stanje = DB_GetFieldInt(res, 0);
        stednja = DB_GetFieldInt(res, 1);
        kredit = DB_GetFieldInt(res, 2);
        rata = DB_GetFieldInt(res, 3);
        rok = DB_GetFieldInt(res, 4);
    }
    DB_FreeResultSet(res);
    return ima;
}

bool:BankaTekst(playerid, tekst[], size = sizeof (tekst))
{
    new stanje, stednja, kredit, rata, rok;
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok)) return false;
    format(tekst, size, T(playerid, "Racun: $%d~n~Stednja: $%d"), stanje, stednja);
    if (!kredit)
    {
        strcat(tekst, T(playerid, "~n~Kredit: nema"), size);
        return true;
    }
    new red[96], sati = (rok - gettime()) / 3600;
    format(red, sizeof red, T(playerid, "~n~Kredit: $%d~n~Rata $%d za %d h"), kredit, rata < kredit ? rata : kredit, sati < 0 ? 0 : sati);
    strcat(tekst, red, size);
    return true;
}

static BankaRacunPoBroju(const broj[])
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT account FROM phone_numbers WHERE number = '%q'", broj);
    new racun = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return racun;
}

static bool:BankaPosalji(playerid, racun, iznos)
{
    new stanje, stednja, kredit, rata, rok, a, b, c, d, e;
    if (iznos <= 0 || iznos > MAX_KES)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Iznos mora biti veci od 0."));
        return false;
    }
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovni racun."));
        return false;
    }
    if (racun == gAccountId[playerid])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to sami sebi."));
        return false;
    }
    if (!racun || !BankaUcitaj(racun, a, b, c, d, e))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Primatelj nema bankovni racun."));
        return false;
    }
    if (stanje < iznos)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
        return false;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d WHERE account = %d", iznos, gAccountId[playerid]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d WHERE account = %d", iznos, racun));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Poslali ste $%d."), iznos);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i] || gAccountId[i] != racun) continue;
        SendClientMessage(i, COLOR_INFO, T(i, "Na vas racun je stiglo $%d od %s."), iznos, GetRPIme(playerid));
    }
    return true;
}

bool:BankaPosaljiBroj(playerid, const broj[], iznos)
{
    if (!gInv[playerid][ITEM_KARTICA])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovnu karticu."));
        return false;
    }
    return BankaPosalji(playerid, BankaRacunPoBroju(broj), iznos);
}

BankaTick()
{
    BankaPljackaTick();
    if (++gBankaSat < 60) return;
    gBankaSat = 0;
    new sad = gettime();
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        new stanje, stednja, kredit, rata, rok;
        if (!gUlogiran[i] || !BankaUcitaj(gAccountId[i], stanje, stednja, kredit, rata, rok)) continue;
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT savings_tick FROM bank_accounts WHERE account = %d", gAccountId[i]);
        new tick = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        if (stednja > 0 && sad - tick >= BANKA_DAN)
        {
            new kamata = BankaKamata(stednja);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET savings = savings + %d, savings_tick = %d WHERE account = %d", kamata, tick + BANKA_DAN, gAccountId[i]));
            if (kamata) SendClientMessage(i, COLOR_INFO, T(i, "Banka: kamata na stednju $%d."), kamata);
        }
        if (!kredit || sad < rok) continue;
        new placa = rata < kredit ? rata : kredit, sRacuna = stanje < placa ? stanje : placa, gotovina = placa - sRacuna;
        if (gNovac[i] < gotovina)
        {
            new kazna = kredit * BANKA_KAZNA / 100;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET loan = loan + %d, loan_due = %d WHERE account = %d", kazna, rok + BANKA_DAN, gAccountId[i]));
            SendClientMessage(i, COLOR_ERROR, T(i, "Banka: nemate za ratu kredita ($%d). Zatezna kamata $%d."), placa, kazna);
            continue;
        }
        if (gotovina) DajNovac(i, -gotovina);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d, loan = loan - %d, loan_due = %d WHERE account = %d", sRacuna, placa, rok + BANKA_DAN, gAccountId[i]));
        SendClientMessage(i, COLOR_INFO, T(i, "Banka: placena rata kredita $%d, preostalo $%d."), placa, kredit - placa);
    }
}

BankaKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new s = BankaIgraca(playerid);
    if (s == -1 || !gBanka[s][bkImaAktor] || !IsPlayerInRangeOfPoint(playerid, BANKA_KASA_DOMET, gBanka[s][bkAx], gBanka[s][bkAy], gBanka[s][bkAz])) return 0;
    new stanje, stednja, kredit, rata, rok, list[512], row[64], len = 0, n = 0, naslov[96];
    if (!BankaUcitaj(gAccountId[playerid], stanje, stednja, kredit, rata, rok)) gBankaMeni[playerid][n++] = BA_OTVORI;
    else
    {
        for (new a = BA_UPLATI; a <= BA_KREDIT; a++) if (a != BA_KREDIT || !kredit) gBankaMeni[playerid][n++] = a;
        if (kredit) gBankaMeni[playerid][n++] = BA_OTPLATI;
        if (!gInv[playerid][ITEM_KARTICA]) gBankaMeni[playerid][n++] = BA_KARTICA;
    }
    for (new i = 0; i < n; i++)
    {
        format(row, sizeof row, T(playerid, gBankaAkcija[gBankaMeni[playerid][i]]), gBankaMeni[playerid][i] == BA_OTVORI ? BANKA_OTVARANJE : BANKA_KARTICA);
        DodajStupac(list, len, row);
    }
    for (new i = n; i < sizeof gBankaAkcija; i++) gBankaMeni[playerid][i] = -1;
    if (BankaTekst(playerid, naslov)) for (new i = 0; naslov[i]; i++) if (naslov[i] == '~') naslov[i] = ' ';
    AktorKaze(playerid, gBanka[s][bkAktor], "Sluzbenik", "Izvolite, kako vam mogu pomoci?");
    PrikaziDialog(playerid, DLG_BANKA, DIALOG_STYLE_LIST, naslov[0] ? naslov : T(playerid, "Banka"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
    return 1;
}

DLG:banka(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gBankaAkcija || BankaIgraca(playerid) == -1) return 1;
    new a = gBankaMeni[playerid][listitem], body[160];
    if (a == -1) return 1;
    if (a == BA_OTVORI || a == BA_KARTICA)
    {
        new cijena = a == BA_OTVORI ? BANKA_OTVARANJE : BANKA_KARTICA;
        if (gNovac[playerid] < cijena) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        if (!GiveItem(playerid, ITEM_KARTICA, 1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        DajNovac(playerid, -cijena);
        if (a == BA_OTVORI) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO bank_accounts (account) VALUES (%d)", gAccountId[playerid]));
        SendClientMessage(playerid, COLOR_INFO, T(playerid, a == BA_OTVORI ? ("Otvorili ste racun i dobili bankovnu karticu.") : ("Dobili ste novu bankovnu karticu.")));
        return BankaKasa(playerid);
    }
    if (a != BA_UPLATI && !gInv[playerid][ITEM_KARTICA]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovnu karticu."));
    gBankaOdabir[playerid] = a;
    if (a == BA_POSALJI) format(body, sizeof body, T(playerid, "Unesite: [id/ime primatelja] [iznos]"));
    else if (a == BA_KREDIT) format(body, sizeof body, T(playerid, "Iznos kredita (najvise $%d). Kamata %d%%, otplata u %d dnevnih rata:"), BANKA_KREDIT_MAX, BANKA_KAMATA, BANKA_RATA);
    else format(body, sizeof body, T(playerid, "Unesite iznos:"));
    return PrikaziDialog(playerid, DLG_BANKA_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Banka"), body, T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

DLG:banka_unos(playerid, response, const inputtext[])
{
    if (BankaIgraca(playerid) == -1) return 1;
    if (!response) return BankaKasa(playerid);
    new a = gBankaOdabir[playerid], racun = gAccountId[playerid], stanje, stednja, kredit, rata, rok, idx = 0, p1[32], p2[16];
    if (!BankaUcitaj(racun, stanje, stednja, kredit, rata, rok)) return 1;
    if (a == BA_POSALJI)
    {
        if (!SljedeciParam(inputtext, idx, p1) || !SljedeciParam(inputtext, idx, p2) || !ProvjeriBroj(p2) || strlen(p2) > 9) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [id/ime primatelja] [iznos]"));
        new t = FindPlayer(p1), primatelj = 0;
        if (t != INVALID_PLAYER_ID && gUlogiran[t]) primatelj = gAccountId[t];
        else
        {
            for (new i = 0; p1[i]; i++) if (p1[i] == ' ') p1[i] = '_';
            new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q' COLLATE NOCASE", p1);
            if (DB_GetRowCount(res)) primatelj = DB_GetFieldInt(res, 0);
            DB_FreeResultSet(res);
        }
        BankaPosalji(playerid, primatelj, strval(p2));
        return BankaKasa(playerid);
    }
    new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 9 ? strval(inputtext) : 0;
    if (n <= 0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Iznos mora biti veci od 0."));
    switch (a)
    {
        case BA_UPLATI:
        {
            if (gNovac[playerid] < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
            DajNovac(playerid, -n);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d WHERE account = %d", n, racun));
        }
        case BA_ISPLATI:
        {
            if (stanje < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
            if (!DajNovac(playerid, n)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d WHERE account = %d", n, racun));
        }
        case BA_NA_STEDNJU:
        {
            if (stanje < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d, savings = savings + %d, savings_tick = CASE WHEN savings = 0 THEN %d ELSE savings_tick END WHERE account = %d", n, n, gettime(), racun));
        }
        case BA_SA_STEDNJE:
        {
            if (stednja < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko na stednji."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d, savings = savings - %d WHERE account = %d", n, n, racun));
        }
        case BA_KREDIT:
        {
            if (kredit) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vec imate kredit."));
            if (n > BANKA_KREDIT_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najveci kredit je $%d."), BANKA_KREDIT_MAX);
            new dug = BankaDug(n);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d, loan = %d, loan_rate = %d, loan_due = %d WHERE account = %d", n, dug, BankaRata(dug), gettime() + BANKA_DAN, racun));
            SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kredit odobren: $%d na racun. Dug $%d, rata $%d svaka 24 h."), n, dug, BankaRata(dug));
        }
        case BA_OTPLATI:
        {
            if (n > kredit) n = kredit;
            if (stanje < n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko novca na racunu."));
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance - %d, loan = loan - %d WHERE account = %d", n, n, racun));
        }
    }
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Transakcija uspjesna."));
    return BankaKasa(playerid);
}

ShowBanke(playerid)
{
    new list[1024], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova banka na mojoj poziciji"));
    for (new s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma]) continue;
        format(row, sizeof row, "[%d] %s%s", gBanka[s][bkDb], CestaZona(gBanka[s][bkX], gBanka[s][bkY]), T(playerid, gBanka[s][bkImaAktor] ? ("") : (" - bez sluzbenika")));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_BANKA, DIALOG_STYLE_LIST, T(playerid, "Banke"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_banka(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Banka se postavlja samo izvana."));
        GetPlayerPos(playerid, gApBankaPoz[playerid][0], gApBankaPoz[playerid][1], gApBankaPoz[playerid][2]);
        GetPlayerFacingAngle(playerid, gApBankaPoz[playerid][3]);
        new list[256], len = 0;
        for (new i = 0; i < sizeof gBankaInt; i++) DodajStupac(list, len, T(playerid, gBankaInt[i][biIme]));
        DodajStupac(list, len, T(playerid, "Prilagodeni (postavite ulaz kasnije na svojoj poziciji)"));
        return PrikaziDialog(playerid, DLG_APANEL_BANKA_INT, DIALOG_STYLE_LIST, T(playerid, "Odaberite interijer"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    new n = listitem;
    for (new s = 0; s < BANKA_MAX; s++)
    {
        if (!gBanka[s][bkIma] || --n) continue;
        gApBanka[playerid] = s;
        return PrikaziDialog(playerid, DLG_APANEL_BANKA_MENI, DIALOG_STYLE_LIST, T(playerid, "Banke"), T(playerid, "Postavi sluzbenika na mojoj poziciji\nPostavi unutarnji ulaz na mojoj poziciji\nDodaj cuvara na mojoj poziciji\nObrisi sve cuvare\nObrisi banku"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    return ShowBanke(playerid);
}

DLG:apanel_banka_int(playerid, response, listitem)
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!response || listitem < 0 || listitem > sizeof gBankaInt) return ShowBanke(playerid);
    new s = 0, i = listitem < sizeof gBankaInt ? listitem : 0;
    while (s < BANKA_MAX && gBanka[s][bkIma]) s++;
    if (s == BANKA_MAX)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj banaka."));
        return ShowBanke(playerid);
    }
    gBanka[s][bkX] = gApBankaPoz[playerid][0];
    gBanka[s][bkY] = gApBankaPoz[playerid][1];
    gBanka[s][bkZ] = gApBankaPoz[playerid][2];
    gBanka[s][bkA] = gApBankaPoz[playerid][3];
    gBanka[s][bkInt] = gBankaInt[i][biInt];
    gBanka[s][bkIx] = gBankaInt[i][biX];
    gBanka[s][bkIy] = gBankaInt[i][biY];
    gBanka[s][bkIz] = gBankaInt[i][biZ];
    gBanka[s][bkImaAktor] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO banks (x, y, z, a, interior, ix, iy, iz) VALUES (%f, %f, %f, %f, %d, %f, %f, %f)", gBanka[s][bkX], gBanka[s][bkY], gBanka[s][bkZ], gBanka[s][bkA], gBanka[s][bkInt], gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz]));
    gBanka[s][bkDb] = ZadnjiId();
    BankaStvori(s);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Banka dodana: %s. Udite i postavite sluzbenika."), CestaZona(gBanka[s][bkX], gBanka[s][bkY]));
    return ShowBanke(playerid);
}

static BankaObrisi(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && BankaIgraca(i) == s) BankaVani(i, s);
    gBanka[s][bkPljacka] = 0;
    for (new c = 0; c < BANKA_CUVARI; c++) BankaCuvarMakni(s, c);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_guards WHERE bank = %d", gBanka[s][bkDb]));
    BankaMakniAktor(s);
    DestroyDynamicPickup(gBanka[s][bkPickup]);
    DestroyDynamicPickup(gBanka[s][bkPickupU]);
    DestroyDynamic3DTextLabel(gBanka[s][bkTekst]);
    DestroyDynamic3DTextLabel(gBanka[s][bkTekstU]);
    gBanka[s][bkIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM banks WHERE id = %d", gBanka[s][bkDb]));
}

DLG:apanel_banka_meni(playerid, response, listitem)
{
    new s = gApBanka[playerid];
    if (!response || !gBanka[s][bkIma]) return ShowBanke(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        if (BankaIgraca(playerid) != s)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te banke."));
            return ShowBanke(playerid);
        }
        BankaMakniAktor(s);
        GetPlayerPos(playerid, gBanka[s][bkAx], gBanka[s][bkAy], gBanka[s][bkAz]);
        GetPlayerFacingAngle(playerid, gBanka[s][bkAa]);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE banks SET Aktor = 1, ax = %f, ay = %f, az = %f, aa = %f WHERE id = %d", gBanka[s][bkAx], gBanka[s][bkAy], gBanka[s][bkAz], gBanka[s][bkAa], gBanka[s][bkDb]));
        BankaAktor(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Sluzbenik je postavljen."));
    }
    else if (listitem == 1)
    {
        if (!GetPlayerInterior(playerid))
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Stanite u interijer koji zelite koristiti."));
            return ShowBanke(playerid);
        }
        for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && BankaIgraca(i) == s) BankaVani(i, s);
        BankaMakniAktor(s);
        DestroyDynamicPickup(gBanka[s][bkPickup]);
        DestroyDynamicPickup(gBanka[s][bkPickupU]);
        DestroyDynamic3DTextLabel(gBanka[s][bkTekst]);
        DestroyDynamic3DTextLabel(gBanka[s][bkTekstU]);
        gBanka[s][bkInt] = GetPlayerInterior(playerid);
        GetPlayerPos(playerid, gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz]);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE banks SET interior = %d, ix = %f, iy = %f, iz = %f, Aktor = 0 WHERE id = %d", gBanka[s][bkInt], gBanka[s][bkIx], gBanka[s][bkIy], gBanka[s][bkIz], gBanka[s][bkDb]));
        BankaStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Unutarnji ulaz postavljen. Ponovno postavite sluzbenika."));
    }
    else if (listitem == 2)
    {
        new c = 0;
        while (c < BANKA_CUVARI && gBankaCuvarDb[s][c]) c++;
        if (BankaIgraca(playerid) != s) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te banke."));
        else if (c == BANKA_CUVARI) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Banka vec ima %d cuvara."), BANKA_CUVARI);
        else
        {
            GetPlayerPos(playerid, gBankaCuvarPoz[s][c][0], gBankaCuvarPoz[s][c][1], gBankaCuvarPoz[s][c][2]);
            GetPlayerFacingAngle(playerid, gBankaCuvarPoz[s][c][3]);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bank_guards (bank, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", gBanka[s][bkDb], gBankaCuvarPoz[s][c][0], gBankaCuvarPoz[s][c][1], gBankaCuvarPoz[s][c][2], gBankaCuvarPoz[s][c][3]));
            gBankaCuvarDb[s][c] = ZadnjiId();
            BankaCuvarStvori(s, c);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Cuvar je postavljen."));
        }
    }
    else if (listitem == 3)
    {
        for (new c = 0; c < BANKA_CUVARI; c++) BankaCuvarMakni(s, c);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bank_guards WHERE bank = %d", gBanka[s][bkDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Cuvari obrisani."));
    }
    else
    {
        BankaObrisi(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Banka obrisana."));
    }
    return ShowBanke(playerid);
}
