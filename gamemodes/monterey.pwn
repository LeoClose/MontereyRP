/*
 __  __  _____  _  _  ____  ____  ____  ____  _  _    ____  _____  __    ____  ____  __      __   _  _
(  \/  )(  _  )( \( )(_  _)( ___)(  _ \( ___)( \/ )  (  _ \(  _  )(  )  ( ___)(  _ \(  )    /__\ ( \/ )
 )    (  )(_)(  )  (   )(   )__)  )   / )__)  \  /    )   / )(_)(  )(__  )__)  )___/ )(__  /(__)\ \  /
(_/\/\_)(_____)(_)\_) (__) (____)(_)\_)(____) (__)   (_)\_)(_____)(____)(____)(__)  (____)(__)(__)(__)

*/
#define MAX_PLAYERS 1000
#define MAX_LJUDI 800
#include <open.mp>
#include <fastcmd>
#include <streamer>
#include <kolizija>
#include <lik>
#include <samp_bcrypt>

#include <Pawn.RakNet>
forward OnCheatDetected(playerid, const ip_address[], type, code);
forward bool:AcUkljuceno(code);
#define AC_USE_CONFIG_FILES false
#define NO_SUSPICION_LOGS
#define AC_MAX_CONNECTS_FROM_IP 3
#define NO_SUSPICION_LOGS
#define AC_USE_STATISTICS false
#define AC_USE_AMMUNATIONS false
#define AC_USE_TUNING_GARAGES false
#define AC_USE_PICKUP_WEAPONS false
#define AC_USE_RESTAURANTS false
#define AC_USE_VENDING_MACHINES false
#define AC_USE_CASINOS false
#include <nex-ac>
#pragma unused KICK_MSG, MAX_CONNECTS_MSG, UNKNOWN_CLIENT_MSG

#define PRENOS_MS   100
#define PRENOS_MIN  8
#define PRENOS_MAX  50

#define ZASTITA_MS     2000

new gPrenosZamrz[MAX_PLAYERS], bool:gPrenosCekaj[MAX_PLAYERS], bool:gAdminZamrznut[MAX_PLAYERS], gZastitaDo[MAX_PLAYERS];
stock PrenosConnect(playerid)
{
    gAdminZamrznut[playerid] = false;
    gPrenosZamrz[playerid] = 0;
    gZastitaDo[playerid] = 0;
}

stock bool:Zasticen(playerid)
{
    return gPrenosZamrz[playerid] != 0 || GetTickCount() - gZastitaDo[playerid] < 0;
}

forward PrenosOdmrzni(playerid, token, Float:x, Float:y, Float:z, zadnji, korak);
public PrenosOdmrzni(playerid, token, Float:x, Float:y, Float:z, zadnji, korak)
{
    if (gPrenosZamrz[playerid] != token) return;
    new n = Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT);
    if (korak < PRENOS_MAX && (korak < PRENOS_MIN || n != zadnji))
    {
        SetTimerEx("PrenosOdmrzni", PRENOS_MS, false, "ddfffdd", playerid, token, x, y, z, n, korak + 1);
        return;
    }
    gPrenosZamrz[playerid] = 0;
    gZastitaDo[playerid] = GetTickCount() + ZASTITA_MS;
    SetPlayerPos(playerid, x, y, z);
    TogglePlayerControllable(playerid, !gAdminZamrznut[playerid]);
    PrenosFade(playerid, false);
}

stock PrenesiIgraca(playerid, Float:x, Float:y, Float:z)
{
    new Float:px, Float:py, Float:pz, bool:cekaj = gPrenosCekaj[playerid];
    gPrenosCekaj[playerid] = false;
    GetPlayerPos(playerid, px, py, pz);
    if (!cekaj && VectorSize(x - px, y - py, z - pz) <= 50.0) return SetPlayerPos(playerid, x, y, z);
    Streamer_UpdateEx(playerid, x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), STREAMER_TYPE_OBJECT);
    if (gPrenosZamrz[playerid] || IsPlayerControllable(playerid))
    {
        static token;
        gPrenosZamrz[playerid] = ++token ? token : ++token;
        TogglePlayerControllable(playerid, false);
        PrenosFade(playerid, true);
        SetTimerEx("PrenosOdmrzni", PRENOS_MS, false, "ddfffdd", playerid, gPrenosZamrz[playerid], x, y, z, -1, 0);
    }
    return SetPlayerPos(playerid, x, y, z);
}
stock ZamrzniIgraca(playerid, bool:moze)
{
    if (gPrenosZamrz[playerid])
    {
        gPrenosZamrz[playerid] = 0;
        gZastitaDo[playerid] = GetTickCount() + ZASTITA_MS;
        PrenosFade(playerid, false);
    }
    return TogglePlayerControllable(playerid, moze && !gAdminZamrznut[playerid]);
}
#undef SetPlayerPos
#define SetPlayerPos PrenesiIgraca

stock PrebaciIgraca(playerid, interijer, svijet, Float:x, Float:y, Float:z, Float:kut = -1000.0)
{
    gPrenosCekaj[playerid] = interijer != GetPlayerInterior(playerid) || svijet != GetPlayerVirtualWorld(playerid);
    if (gPrenosCekaj[playerid]) SavjetPromjena(playerid);
    SetPlayerInterior(playerid, interijer);
    SetPlayerVirtualWorld(playerid, svijet);
    SetPlayerPos(playerid, x, y, z);
    if (kut > -1000.0) SetPlayerFacingAngle(playerid, kut);
    SetCameraBehindPlayer(playerid);
    VrijemeIgrac(playerid);
}
#undef TogglePlayerControllable
#define TogglePlayerControllable ZamrzniIgraca

#pragma dynamic 32768

new Text:Ime;
new Text:ImeRp;

new gVozila[MAX_VEHICLES], gVoziloMjesto[MAX_VEHICLES], gVozilaCount;

CreateVozilo(model, Float:x, Float:y, Float:z, Float:a, c1, c2, respawn, bool:sirena = false)
{
    new v = CreateVehicle(model, x, y, z, a, c1, c2, respawn, sirena);
    if (v != INVALID_VEHICLE_ID)
    {
        gVozila[gVozilaCount] = v;
        gVoziloMjesto[v] = ++gVozilaCount;
        PrikolicaDodaj(v);
        GepekZonaDodaj(v);
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
        VoziloKmSpremi(v);
        ClearGepek(v);
        PrikolicaMakni(v);
        GepekZonaMakni(v);
        VlasnikMakni(v);
    }
    return !!DestroyVehicle(v);
}

#undef CreateVehicle
#undef DestroyVehicle
#define CreateVehicle CreateVozilo
#define DestroyVehicle DestroyVozilo

#define MIN_PASSWORD_LEN    6
#define MAX_PASSWORD_LEN    64
#define SALT_LEN            16
#define HASH_ROUNDS         1000
#define BCRYPT_NITI         4
#define MAX_LOGIN_ATTEMPTS  3

#define SPAWN_X     484.0
#define SPAWN_Y     -1499.5
#define SPAWN_Z     20.6
#define SPAWN_A     270.0
#define DEFAULT_SKIN  271

new DB:gDB;
new gAccountId[MAX_PLAYERS];
new bool:gUlogiran[MAX_PLAYERS];
#define LOGIN_IP_MAX        64
#define LOGIN_BLOK_S        600
enum E_LOGIN_IP { liIp[16], liGreske, liVrijeme }
new gLoginIp[LOGIN_IP_MAX][E_LOGIN_IP];
forward bool:LoginBlokiran(playerid);

main() {}

public OnGameModeInit()
{
    SetGameModeText("©M:RP by Leo and Krti");
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_OFF);
    bcrypt_set_thread_limit(BCRYPT_NITI);
    DisableInteriorEnterExits();
    Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, 960);
    Streamer_ToggleChunkStream(true);
    Streamer_SetChunkSize(STREAMER_TYPE_OBJECT, 25);
    EnableStuntBonusForAll(false);
    gDB = DB_Open("monterey.db");
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "PRAGMA journal_mode = WAL"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "PRAGMA synchronous = NORMAL"));
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
            zatvor INTEGER NOT NULL DEFAULT 0,\
            level INTEGER NOT NULL DEFAULT 1,\
            respekti INTEGER NOT NULL DEFAULT 0,\
            playtime INTEGER NOT NULL DEFAULT 0,\
            vozacka INTEGER NOT NULL DEFAULT 0,\
            oruzje_dozvola INTEGER NOT NULL DEFAULT 0,\
            job INTEGER NOT NULL DEFAULT 0,\
            payday_earn INTEGER NOT NULL DEFAULT 0\
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
    MapaIdlewoodFlatsUkloni(INVALID_PLAYER_ID);
    MapaMarica();
    MapaHitna();
    MapaOpstina();
    MapaIdlewood();
    MapaIdlewoodFlats();
    MapaDacoradiona();
    WilshirePdUkloni(INVALID_PLAYER_ID);
    WilshirePd();
    MapaBusTaxiUkloni(INVALID_PLAYER_ID);
    MapaBusTaxi();
    MapaDiamondCasino();
    MapaCartierZlatara();
    MapaInternetKafic();
    MapaInternetKaficExtUkloni(INVALID_PLAYER_ID);
    MapaInternetKaficExt();
    MapaFleeca();
    MapaBankaInterijer();
    MapaOruzarnicaInt();
    MapaCartierExtUkloni(INVALID_PLAYER_ID);
    MapaCartierExt();
    MapaCentralBankUkloni(INVALID_PLAYER_ID);
    MapaCentralBank();
    MapaCasinoExtUkloni(INVALID_PLAYER_ID);
    MapaCasinoExt();
    MapaMainStreetUkloni(INVALID_PLAYER_ID);
    MapaMainStreet();
    MapaMarketSt();
    MapaJeffersonFlatsUkloni(INVALID_PLAYER_ID);
    MapaJeffersonFlats();
    MapaSpawnUkloni(INVALID_PLAYER_ID);
    MapaSpawn();
    MapaMontereyPeakUkloni(INVALID_PLAYER_ID);
    BrodUkloni(INVALID_PLAYER_ID);
    MapaMontereyPeak();
    MapaGlenParkUkloni(INVALID_PLAYER_ID);
    MapaGlenPark();
    MapaChinatownUkloni(INVALID_PLAYER_ID);
    MapaChinatown();
    MapaItalijaniUkloni(INVALID_PLAYER_ID);
    MapaItalijani();
    MapaSurenosiUkloni(INVALID_PLAYER_ID);
    MapaSurenosi();
    Lik_OsvjeziMapu();
    InitKapije();
    InitMarica();
    InitPolicija();
    InitMdt();
    InitSluzbe();
    InitGepek();
    InitVozilo();
    InitFirme();
    InitTrgovina();
    InitKazino();
    InitBanka();
    InitRacunalo();
    InitVijesti();
    LoginIpCheck();
    InitKafic();
    InitUtrke();
    InitBilijar();
    InitKosarka();
    InitArkada();
    InitPikado();
    InitKuglanje();
    InitKamp();
    InitKazne();
    InitPlanina();
    InitStandovi();
    InitAntiCheat();
    InitSync();
    InitPoker();
    InitZlatara();
    InitVrijeme();
    InitOruzarnica();
    InitOdjeca();
    InitKuce();
    InitNamjestaj();
    InitTuning();
    InitGaraze();
    InitSalon();
    InitPijaca();
    InitOpstina();
    InitHeli();
    InitVlakovi();
    InitAvioni();
    InitPlaca();
    InitSef();
    InitDroga();
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS social_messages_rcv ON social_messages (receiver, seen)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS social_messages_snd ON social_messages (sender)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS social_comments_post ON social_comments (post)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS social_posts_acc ON social_posts (account)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS social_friends_b ON social_friends (b, accepted)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS bet_slips_acc ON bet_slips (account, status)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS bet_slips_status ON bet_slips (status)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS bet_legs_event ON bet_legs (event)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS bet_events_status ON bet_events (status, start)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS furniture_owner ON furniture (owner)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS player_vehicles_owner ON player_vehicles (owner)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS fines_acc ON fines (account)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS police_records_acc ON police_records (account)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS phone_messages_acc ON phone_messages (account)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS stock_candles_stock ON stock_candles (stock)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS social_group_messages_grp ON social_group_messages (grp, id)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS locker_log_org ON locker_log (org, id)"));
    SetTimer("Sekunda", 1000, true);

    Ime = TextDrawCreate(580.0, 20.0, "Monterey");
    TextDrawFont(Ime, TEXT_DRAW_FONT_3);
    TextDrawLetterSize(Ime, 0.38, 1.5);
    TextDrawAlignment(Ime, TEXT_DRAW_ALIGN_CENTRE);
    TextDrawColour(Ime, 0x1E5AA8FF);
    TextDrawSetShadow(Ime, 0);
    TextDrawSetOutline(Ime, 1);
    TextDrawBackgroundColour(Ime, 0x000000FF);
    TextDrawSetProportional(Ime, true);

    ImeRp = TextDrawCreate(612.0, 34.0, "RolePlay");
    TextDrawFont(ImeRp, TEXT_DRAW_FONT_3);
    TextDrawLetterSize(ImeRp, 0.28, 1.1);
    TextDrawAlignment(ImeRp, TEXT_DRAW_ALIGN_RIGHT);
    TextDrawColour(ImeRp, 0xFFFFFFFF);
    TextDrawSetShadow(ImeRp, 0);
    TextDrawSetOutline(ImeRp, 1);
    TextDrawBackgroundColour(ImeRp, 0x000000FF);
    TextDrawSetProportional(ImeRp, true);
    return 1;
}

forward Sekunda();
public Sekunda()
{
    StatsTick();
    KurvaTick();
    PolicijaTick();
    SlucajTick();
    StetaTick();
    TaksiTick();
    TeritorijTick();
    VoziloTick();
    KapijeTick();
    CivilVozilaTick();
    VlakTick();
    BankaTick();
    RacunaloTick();
    VijestiTick();
    ZlataraTick();
    VrijemeTick();
    IkoneTick();
    PlacaTick();
    KaficTick();
    BilijarTick();
    KosTick();
    PikadoTick();
    KuglanjeTick();
    PokerTick();
    SatTick();
    AvionAtcTick();
    DrogaTick();
    UtrkaTick();
    StandTick();
    FirmaTick();
    KaznaTick();
    SpecTick();
    UpitTick();
}

public OnGameModeExit()
{
    StandUgasi();
    OkladaVratiSve();
    PokerVratiSve();
    StatsSaveAll();
    VlasnikStetaSpremiSve();
    VoziloKmSpremiSve();
    DB_Close(gDB);
    return 1;
}

#define MAX_ADMIN_LEVEL     5
#define MAX_RANK_NAME       32
#define ALVL_KICK           1
#define ALVL_VEH            1
#define PRIKOLICA_MODEL     611
#define PRIKOLICA_VELIKA    607
#define ALVL_BAN            2
#define ALVL_ORG            3
#define ALVL_MONEY          4
#define ALVL_MANAGE         MAX_ADMIN_LEVEL
#define COLOR_ADMIN         0xFF9900FF
#define COLOR_ERROR         0xFF6347FF
#define COLOR_INFO          0xCCCCCCFF
new gAdmin[MAX_PLAYERS];
new gIgracZadnji = -1;
new bool:gPremium[MAX_PLAYERS];
new gAdminVeh[MAX_PLAYERS] = {INVALID_VEHICLE_ID, ...};
new gRankName[MAX_ADMIN_LEVEL + 1][MAX_RANK_NAME];
#define AP_ORG_LIST_FIXED   2

#include "monterey/poruke.inc"
#include "monterey/dialog.inc"
#include "monterey/upit.inc"
#include "monterey/chat.inc"
#include "monterey/tekst.inc"
#include "monterey/oznake.inc"
#include "monterey/vrijeme.inc"
#include "monterey/steta.inc"
#include "monterey/hod.inc"
#include "monterey/org.inc"
#include "monterey/inventar.inc"
#include "monterey/admin.inc"
#include "monterey/tiketi.inc"
#include "monterey/zone.inc"
#include "monterey/taxi.inc"
#include "monterey/kurve.inc"
#include "monterey/policija.inc"
#include "monterey/sluzbe.inc"
#include "monterey/vozila.inc"
#include "monterey/vlakovi.inc"
#include "monterey/avioni.inc"
#include "monterey/firme.inc"
#include "monterey/trgovina.inc"
#include "monterey/kazino.inc"
#include "monterey/banka.inc"
#include "monterey/vijesti.inc"
#include "monterey/racunalo.inc"
#include "monterey/kafic.inc"
#include "monterey/oklada.inc"
#include "monterey/obavijest.inc"
#include "monterey/bilijar.inc"
#include "monterey/kosarka.inc"
#include "monterey/arkada.inc"
#include "monterey/pikado.inc"
#include "monterey/kuglanje.inc"
#include "monterey/anticheat.inc"
#include "monterey/poker.inc"
#include "monterey/fit_tablica.inc"
#include "monterey/zlatara.inc"
#include "monterey/oruzarnica.inc"
#include "monterey/odjeca.inc"
#include "monterey/kuce.inc"
#include "monterey/sef.inc"
#include "monterey/droga.inc"
#include "monterey/spec.inc"
#include "monterey/sync.inc"
#include "monterey/namjestaj.inc"
#include "monterey/salon.inc"
#include "monterey/kamp.inc"
#include "monterey/pijaca.inc"
#include "monterey/opstina.inc"
#include "monterey/mape/mapa.inc"
#include "monterey/mape/mapa_marica.inc"
#include "monterey/mape/mapa_hitna.inc"
#include "monterey/mape/mapa_opstina.inc"
#include "monterey/mape/mapa_idlewood.inc"
#include "monterey/mape/mapa_chinatown.inc"
#include "monterey/kapije.inc"
#include "monterey/mape/mapa_italijani.inc"
#include "monterey/mape/mapa_idlewood_flats.inc"
#include "monterey/mape/mapa_dacoradiona.inc"
#include "monterey/mape/wilshire_pd.inc"
#include "monterey/mape/mapa_bus_taxi.inc"
#include "monterey/mape/mapa_diamond_casino.inc"
#include "monterey/mape/mapa_cartier_zlatara.inc"
#include "monterey/mape/mapa_internet_kafic.inc"
#include "monterey/mape/mapa_internet_kafic_ext.inc"
#include "monterey/mape/mapa_fleeca.inc"
#include "monterey/mape/mapa_banka_interijer.inc"
#include "monterey/mape/mapa_oruzarnica_int.inc"
#include "monterey/mape/mapa_cartier_ext.inc"
#include "monterey/mape/mapa_central_bank.inc"
#include "monterey/mape/mapa_casino_ext.inc"
#include "monterey/mape/mapa_main_street.inc"
#include "monterey/mape/mapa_market_st.inc"
#include "monterey/mape/mapa_jefferson_flats.inc"
#include "monterey/mape/mapa_spawn.inc"
#include "monterey/mape/mapa_monterey_peak.inc"
#include "monterey/mape/mapa_glen_park.inc"
#include "monterey/mape/mapa_surenosi.inc"
#include "monterey/brod.inc"
#include "monterey/teritorij.inc"
#include "monterey/mobitel.inc"
#include "monterey/utrke.inc"
#include "monterey/fps.inc"
#include "monterey/standovi.inc"
#include "monterey/kazne.inc"
#include "monterey/planina.inc"
#include "monterey/mdt.inc"
#include "monterey/tuning.inc"
#include "monterey/garaze.inc"
#include "monterey/lokacije.inc"
#include "monterey/placa.inc"
#include "monterey/login.inc"
#include "monterey/registracija.inc"
#include "monterey/stats.inc"
#include "monterey/player.inc"
#include "monterey/civili.inc"
#include "monterey/likovi.inc"

new gLozinkaCeka[MAX_PLAYERS];

DLG:registracija(playerid, response, const inputtext[])
{
    if (!response) return KickTimer(playerid);

    new len = strlen(inputtext);
    if (len < MIN_PASSWORD_LEN || len > MAX_PASSWORD_LEN)
        return PrikaziRegistraciju(playerid, "{FF6347}Lozinka mora imati 6-64 znakova.\n\n");

    if (gLozinkaCeka[playerid] == gConnSerial[playerid]) return 1;
    gLozinkaCeka[playerid] = gConnSerial[playerid];
    bcrypt_hash(playerid, "RegistracijaHash", inputtext, BCRYPT_COST, "d", gConnSerial[playerid]);
    return 1;
}

forward RegistracijaHash(playerid, hashid, serijal);
public RegistracijaHash(playerid, hashid, serijal)
{
    new hash[BCRYPT_HASH_LENGTH], name[MAX_PLAYER_NAME + 1];
    bcrypt_get_hash(hash);
    if (!IsPlayerConnected(playerid) || gConnSerial[playerid] != serijal) return 1;
    gLozinkaCeka[playerid] = 0;
    GetPlayerName(playerid, name, sizeof name);

    DB_FreeResultSet(DB_ExecuteQuery(gDB,
        "INSERT INTO accounts (name, hash, salt, registered, last_login, jezik) VALUES ('%q', '%q', '', %d, %d, -1)",
        name, hash, gettime(), gettime()));

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

    if (gLozinkaCeka[playerid] == gConnSerial[playerid]) return 1;
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

    if (stored[0] == '$')
    {
        gLozinkaCeka[playerid] = gConnSerial[playerid];
        bcrypt_verify(playerid, "PrijavaProvjera", inputtext, stored, "dd", gConnSerial[playerid], id);
        return 1;
    }
    HashPassword(inputtext, salt, hash);
    if (strcmp(hash, stored) != 0) return NevazecaLozinka(playerid);
    bcrypt_hash(playerid, "LozinkaPretvori", inputtext, BCRYPT_COST, "d", id);
    return PrijavaUspjela(playerid, id);
}

forward PrijavaProvjera(playerid, bool:uspjeh, serijal, id);
public PrijavaProvjera(playerid, bool:uspjeh, serijal, id)
{
    if (!IsPlayerConnected(playerid) || gConnSerial[playerid] != serijal) return 1;
    gLozinkaCeka[playerid] = 0;
    if (!uspjeh) return NevazecaLozinka(playerid);
    return PrijavaUspjela(playerid, id);
}

forward LozinkaPretvori(playerid, hashid, id);
public LozinkaPretvori(playerid, hashid, id)
{
    new hash[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(hash);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET hash = '%q', salt = '' WHERE id = %d", hash, id));
    return 1;
}

PrijavaUspjela(playerid, id)
{
    new s = LoginIpIgraca(playerid, false);
    if (s != -1) gLoginIp[s][liGreske] = 0;

    gAccountId[playerid] = id;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET last_login = %d WHERE id = %d", gettime(), id));
    SendClientMessage(playerid, 0x33CC33FF, T(playerid, "Uspjesno ste se prijavili. Dobrodosli natrag!"));
    return ZavrsiLogin(playerid);
}

PrikaziRegistraciju(playerid, const error[])
{
    new body[160];
    RegistracijaTd(playerid);
    format(body, sizeof body, T(playerid, "%sOvo ime nije registrirano.\nUnesite lozinku (6-64 znakova):"), error);
    return PrikaziDialog(playerid, DLG_REGISTRACIJA, DIALOG_STYLE_PASSWORD, T(playerid, "Registracija"), body, T(playerid, "Registriraj"), T(playerid, "Izlaz"));
}

PrikaziLogin(playerid, const error[])
{
    new body[160];
    LoginTd(playerid);
    format(body, sizeof body, T(playerid, "%sOvo ime je registrirano.\nUnesite svoju lozinku:"), error);
    return PrikaziDialog(playerid, DLG_PRIJAVA, DIALOG_STYLE_PASSWORD, T(playerid, "Prijava"), body, T(playerid, "Prijava"), T(playerid, "Izlaz"));
}

static LoginIpSlot(const ip[], bool:stvori)
{
    new slot = 0, sad = gettime();
    for (new i = 0; i < LOGIN_IP_MAX; i++)
    {
        if (gLoginIp[i][liGreske] && sad - gLoginIp[i][liVrijeme] >= LOGIN_BLOK_S) gLoginIp[i][liGreske] = 0;
        if (gLoginIp[i][liGreske] && !strcmp(gLoginIp[i][liIp], ip)) return i;
        if (gLoginIp[slot][liGreske] && (!gLoginIp[i][liGreske] || gLoginIp[i][liVrijeme] < gLoginIp[slot][liVrijeme])) slot = i;
    }
    if (!stvori) return -1;
    strcopy(gLoginIp[slot][liIp], ip, 16);
    gLoginIp[slot][liGreske] = 0;
    return slot;
}

static LoginIpIgraca(playerid, bool:stvori)
{
    new ip[16];
    GetPlayerIp(playerid, ip, sizeof ip);
    return LoginIpSlot(ip, stvori);
}

LoginIpCheck()
{
    new s = LoginIpSlot("10.0.0.1", true);
    gLoginIp[s][liGreske] = MAX_LOGIN_ATTEMPTS;
    gLoginIp[s][liVrijeme] = gettime();
    assert(LoginIpSlot("10.0.0.1", false) == s && LoginIpSlot("10.0.0.2", false) == -1 && LoginIpSlot("10.0.0.2", true) != s);
    gLoginIp[s][liVrijeme] -= LOGIN_BLOK_S;
    assert(LoginIpSlot("10.0.0.1", false) == -1);
    for (new i = 0; i < LOGIN_IP_MAX; i++) gLoginIp[i][liGreske] = 0;
}

bool:LoginBlokiran(playerid)
{
    new s = LoginIpIgraca(playerid, false);
    if (s == -1 || gLoginIp[s][liGreske] < MAX_LOGIN_ATTEMPTS) return false;
    SendClientMessage(playerid, 0xFF6347FF, T(playerid, "Previse neuspjelih pokusaja prijave."));
    KickTimer(playerid);
    return true;
}

NevazecaLozinka(playerid)
{
    new s = LoginIpIgraca(playerid, true);
    gLoginIp[s][liVrijeme] = gettime();
    if (++gLoginIp[s][liGreske] >= MAX_LOGIN_ATTEMPTS)
    {
        SendClientMessage(playerid, 0xFF6347FF, T(playerid, "Previse neuspjelih pokusaja prijave."));
        return KickTimer(playerid);
    }
    new error[64];
    format(error, sizeof error, T(playerid, "{FF6347}Pogresna lozinka (%d/%d).\n\n"), gLoginIp[s][liGreske], MAX_LOGIN_ATTEMPTS);
    return PrikaziLogin(playerid, error);
}

ZavrsiLogin(playerid)
{
    if (!UcitajJezik(playerid)) return PokaziJezike(playerid);
    LoginUlaz(playerid);
    return 1;
}

ZavrsiUlaz(playerid)
{
    gUlogiran[playerid] = true;
    if (!StatsLoad(playerid)) return 0;
    LoadAdmina(playerid);
    UcitajClana(playerid);
    UcitajNovac(playerid);
    LoadInventar(playerid);
    MobitelPrijava(playerid);
    OdjecaPrijava(playerid);
    KucaPrijava(playerid);
    PolicijaPrijava(playerid);
    TeritorijPrijava(playerid);
    SetSpawnInfo(playerid, NO_TEAM, DEFAULT_SKIN, SPAWN_X, SPAWN_Y, SPAWN_Z, SPAWN_A);
    TogglePlayerSpectating(playerid, false);
    AdminPrijava(playerid);
    return 1;
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

stock ShowHud(playerid)
{
    TextDrawShowForPlayer(playerid, Ime);
    TextDrawShowForPlayer(playerid, ImeRp);
}

forward KickajIgraca(playerid);
public KickajIgraca(playerid)
{
    Kick(playerid);
}

