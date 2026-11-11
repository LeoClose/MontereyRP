/*
 __  __  _____  _  _  ____  ____  ____  ____  _  _    ____  _____  __    ____  ____  __      __   _  _
(  \/  )(  _  )( \( )(_  _)( ___)(  _ \( ___)( \/ )  (  _ \(  _  )(  )  ( ___)(  _ \(  )    /__\ ( \/ )
 )    (  )(_)(  )  (   )(   )__)  )   / )__)  \  /    )   / )(_)(  )(__  )__)  )___/ )(__  /(__)\ \  /
(_/\/\_)(_____)(_)\_) (__) (____)(_)\_)(____) (__)   (_)\_)(_____)(____)(____)(__)  (____)(__)(__)(__)

*/
#define MAX_PLAYERS 150
#define MAX_LJUDI 100
#include <open.mp>
#include <fastcmd>
#include <streamer>
#include <kolizija>

#pragma dynamic 32768

new gVozila[MAX_VEHICLES], gVoziloMjesto[MAX_VEHICLES], gVozilaCount;

CreateVozilo(model, Float:x, Float:y, Float:z, Float:a, c1, c2, respawn, bool:sirena = false)
{
    new v = CreateVehicle(model, x, y, z, a, c1, c2, respawn, sirena);
    if (v != INVALID_VEHICLE_ID)
    {
        gVozila[gVozilaCount] = v;
        gVoziloMjesto[v] = ++gVozilaCount;
        PrikolicaDodaj(v);
        VoziloDodaj(v);
    }
    return v;
}

bool:DestroyVozilo(v)
{
    if (v > 0 && v < MAX_VEHICLES && gVoziloMjesto[v])
    {
        new m = gVoziloMjesto[v] - 1, zadnji = gVozila[--gVozilaCount];
        gVozila[m] = zadnji;
        gVoziloMjesto[zadnji] = m + 1;
        gVoziloMjesto[v] = 0;
        ClearGepek(v);
        PrikolicaMakni(v);
        VlasnikMakni(v);
    }
    return DestroyVehicle(v);
}

#define CreateVehicle CreateVozilo
#define DestroyVehicle DestroyVozilo

#define MIN_PASSWORD_LEN    6
#define MAX_PASSWORD_LEN    64
#define SALT_LEN            16
#define HASH_ROUNDS         1000
#define MAX_LOGIN_ATTEMPTS  3

#define SPAWN_X     2486.0
#define SPAWN_Y     -1666.0
#define SPAWN_Z     13.5
#define SPAWN_A     90.0
#define DEFAULT_SKIN  271

new DB:gDB;
new gAccountId[MAX_PLAYERS];
new bool:gUlogiran[MAX_PLAYERS];
new gLoginAttempts[MAX_PLAYERS];

main() {}

public OnGameModeInit()
{
    SetGameModeText("Roleplay");
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_OFF);
    DisableInteriorEnterExits();
    EnableStuntBonusForAll(false);
    gDB = DB_Open("monterey.db");
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "PRAGMA journal_mode = WAL"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB,
        "CREATE TABLE IF NOT EXISTS accounts (\
            id INTEGER PRIMARY KEY AUTOINCREMENT,\
            name TEXT NOT NULL UNIQUE COLLATE NOCASE,\
            hash TEXT NOT NULL,\
            salt TEXT NOT NULL,\
            registered INTEGER NOT NULL,\
            last_login INTEGER,\
            admin INTEGER NOT NULL DEFAULT 0,\
            premium INTEGER NOT NULL DEFAULT 0,\
            opis TEXT NOT NULL DEFAULT '',\
            jezik INTEGER NOT NULL DEFAULT 0,\
            jezici INTEGER NOT NULL DEFAULT 1,\
            prijevod INTEGER NOT NULL DEFAULT -1,\
            ucenje INTEGER NOT NULL DEFAULT -1,\
            novac INTEGER NOT NULL DEFAULT 1000,\
            org INTEGER NOT NULL DEFAULT 0,\
            org_rank TEXT NOT NULL DEFAULT '',\
            org_perms INTEGER NOT NULL DEFAULT 0,\
            org_skin INTEGER NOT NULL DEFAULT -1,\
            wanted INTEGER NOT NULL DEFAULT 0,\
            zatvor INTEGER NOT NULL DEFAULT 0\
        )"));
    TekstPokreni();
    InitSteta();
    InitDialog();
    InitAdmin();
    InitJezik();
    InitOrg();
    InitInventar();
    InitMobitel();
    InitTeritorij();
    InitTaksi();
    InitKurve();
    InitHq();
    MapaOpstinaUkloni(INVALID_PLAYER_ID);
    MapaIdlewoodUkloni(INVALID_PLAYER_ID);
    MapaMarica();
    MapaHitna();
    MapaOpstina();
    MapaIdlewood();
    MapaDacoradiona();
    InitMarica();
    InitPolicija();
    InitSluzbe();
    InitGepek();
    InitVozilo();
    InitTrgovina();
    InitOruzarnica();
    InitOdjeca();
    InitSalon();
    InitPijaca();
    InitGranica();
    InitOpstina();
    InitHeli();
    InitVlakovi();
    SetTimer("Sekunda", 1000, true);
    return 1;
}

forward Sekunda();
public Sekunda()
{
    GranicaTick();
    KurvaTick();
    PolicijaTick();
    SlucajTick();
    StetaTick();
    TaksiTick();
    TeritorijTick();
    VoziloTick();
    VlakTick();
}

public OnGameModeExit()
{
    DB_Close(gDB);
    return 1;
}

#define MAX_ADMIN_LEVEL     5
#define MAX_RANK_NAME       32
#define ALVL_KICK           1
#define ALVL_VEH            1
#define PRIKOLICA_MODEL     611
#define ALVL_BAN            2
#define ALVL_ORG            3
#define ALVL_MONEY          4
#define ALVL_MANAGE         MAX_ADMIN_LEVEL
#define COLOR_ADMIN         0xFF9900FF
#define COLOR_ERROR         0xFF6347FF
#define COLOR_INFO          0xCCCCCCFF
new gAdmin[MAX_PLAYERS];
new bool:gPremium[MAX_PLAYERS];
new gAdminVeh[MAX_PLAYERS] = {INVALID_VEHICLE_ID, ...};
new gRankName[MAX_ADMIN_LEVEL + 1][MAX_RANK_NAME];
#define AP_ORG_LIST_FIXED   2

#include "monterey/dialog.inc"
#include "monterey/chat.inc"
#include "monterey/tekst.inc"
#include "monterey/steta.inc"
#include "monterey/hod.inc"
#include "monterey/org.inc"
#include "monterey/inventar.inc"
#include "monterey/admin.inc"
#include "monterey/ceste.inc"
#include "monterey/cesta.inc"
#include "monterey/taxi.inc"
#include "monterey/kurve.inc"
#include "monterey/policija.inc"
#include "monterey/sluzbe.inc"
#include "monterey/vozila.inc"
#include "monterey/vlakovi.inc"
#include "monterey/trgovina.inc"
#include "monterey/oruzarnica.inc"
#include "monterey/odjeca.inc"
#include "monterey/salon.inc"
#include "monterey/pijaca.inc"
#include "monterey/granica.inc"
#include "monterey/opstina.inc"
#include "monterey/mape/mapa.inc"
#include "monterey/mape/mapa_marica.inc"
#include "monterey/mape/mapa_hitna.inc"
#include "monterey/mape/mapa_opstina.inc"
#include "monterey/mape/mapa_idlewood.inc"
#include "monterey/mape/mapa_dacoradiona.inc"
#include "monterey/teritorij.inc"
#include "monterey/mobitel.inc"
#include "monterey/player.inc"

DLG:registracija(playerid, response, const inputtext[])
{
    if (!response) return KickTimer(playerid);

    new len = strlen(inputtext);
    if (len < MIN_PASSWORD_LEN || len > MAX_PASSWORD_LEN)
        return PrikaziRegistraciju(playerid, "{FF6347}Lozinka mora imati 6-64 znakova.\n\n");

    new name[MAX_PLAYER_NAME + 1], salt[SALT_LEN + 1], hash[65];
    GetPlayerName(playerid, name, sizeof name);
    GenerateSalt(salt);
    HashPassword(inputtext, salt, hash);

    DB_FreeResultSet(DB_ExecuteQuery(gDB,
        "INSERT INTO accounts (name, hash, salt, registered, last_login, jezik) VALUES ('%q', '%q', '%q', %d, %d, -1)",
        name, hash, salt, gettime(), gettime()));

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q' LIMIT 1", name);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        SendClientMessage(playerid, 0xFF6347FF, T(playerid, "Registracija nije uspjela. Ponovno se spojite."));
        return KickTimer(playerid);
    }
    gAccountId[playerid] = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);

    SendClientMessage(playerid, 0x33CC33FF, T(playerid, "Racun je registriran. Dobrodosli na Monterey RolePlay!"));
    return ZavrsiLogin(playerid);
}

DLG:prijava(playerid, response, const inputtext[])
{
    if (!response) return KickTimer(playerid);

    new name[MAX_PLAYER_NAME + 1], salt[SALT_LEN + 1], stored[65], hash[65];
    GetPlayerName(playerid, name, sizeof name);

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, hash, salt FROM accounts WHERE name = '%q' LIMIT 1", name);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        return PrikaziRegistraciju(playerid, "");
    }
    new id = DB_GetFieldInt(res, 0);
    DB_GetFieldString(res, 1, stored, sizeof stored);
    DB_GetFieldString(res, 2, salt, sizeof salt);
    DB_FreeResultSet(res);

    if (!inputtext[0] || strlen(inputtext) > MAX_PASSWORD_LEN)
        return NevazecaLozinka(playerid);

    HashPassword(inputtext, salt, hash);
    if (strcmp(hash, stored) != 0) return NevazecaLozinka(playerid);

    gAccountId[playerid] = id;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET last_login = %d WHERE id = %d", gettime(), id));
    SendClientMessage(playerid, 0x33CC33FF, T(playerid, "Uspjesno ste se prijavili. Dobrodosli natrag!"));
    return ZavrsiLogin(playerid);
}

PrikaziRegistraciju(playerid, const error[])
{
    new body[160];
    format(body, sizeof body, T(playerid, "%sOvo ime nije registrirano.\nUnesite lozinku (6-64 znakova):"), error);
    return PrikaziDialog(playerid, DLG_REGISTRACIJA, DIALOG_STYLE_PASSWORD, T(playerid, "Registracija"), body, T(playerid, "Registriraj"), T(playerid, "Izlaz"));
}

PrikaziLogin(playerid, const error[])
{
    new body[160];
    format(body, sizeof body, T(playerid, "%sOvo ime je registrirano.\nUnesite svoju lozinku:"), error);
    return PrikaziDialog(playerid, DLG_PRIJAVA, DIALOG_STYLE_PASSWORD, T(playerid, "Prijava"), body, T(playerid, "Prijava"), T(playerid, "Izlaz"));
}

NevazecaLozinka(playerid)
{
    if (++gLoginAttempts[playerid] >= MAX_LOGIN_ATTEMPTS)
    {
        SendClientMessage(playerid, 0xFF6347FF, T(playerid, "Previse neuspjelih pokusaja prijave."));
        return KickTimer(playerid);
    }
    new error[64];
    format(error, sizeof error, T(playerid, "{FF6347}Pogresna lozinka (%d/%d).\n\n"), gLoginAttempts[playerid], MAX_LOGIN_ATTEMPTS);
    return PrikaziLogin(playerid, error);
}

ZavrsiLogin(playerid)
{
    if (!UcitajJezik(playerid)) return PokaziJezike(playerid);
    gUlogiran[playerid] = true;
    LoadAdmina(playerid);
    UcitajClana(playerid);
    UcitajNovac(playerid);
    LoadInventar(playerid);
    MobitelPrijava(playerid);
    OdjecaPrijava(playerid);
    PolicijaPrijava(playerid);
    TeritorijPrijava(playerid);
    SetSpawnInfo(playerid, NO_TEAM, DEFAULT_SKIN, SPAWN_X, SPAWN_Y, SPAWN_Z, SPAWN_A);
    TogglePlayerSpectating(playerid, false);
    return 1;
}

GenerateSalt(salt[SALT_LEN + 1])
{
    static const chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (new i = 0; i < SALT_LEN; i++) salt[i] = chars[random(sizeof chars - 1)];
    salt[SALT_LEN] = EOS;
}

HashPassword(const password[], const salt[], hash[65])
{
    new tmp[65];
    SHA256_Hash(password, salt, hash, 65);
    for (new i = 1; i < HASH_ROUNDS; i++)
    {
        SHA256_Hash(hash, salt, tmp, 65);
        hash = tmp;
    }
}

KickTimer(playerid)
{
    SetTimerEx("KickajIgraca", 200, false, "i", playerid);
    return 1;
}

forward KickajIgraca(playerid);
public KickajIgraca(playerid)
{
    Kick(playerid);
}
