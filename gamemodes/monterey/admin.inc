InitAdmin()
{

    DB_FreeResultSet(DB_ExecuteQuery(gDB,
        "CREATE TABLE IF NOT EXISTS admin_ranks (\
            level INTEGER PRIMARY KEY,\
            name TEXT NOT NULL\
        )"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB,
        "INSERT OR IGNORE INTO admin_ranks (level, name) VALUES \
            (1, 'Pomocnik'), (2, 'Moderator'), (3, 'Administrator'), (4, 'Glavni Administrator'), (5, 'Vlasnik')"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB,
        "CREATE TABLE IF NOT EXISTS bans (\
            id INTEGER PRIMARY KEY AUTOINCREMENT,\
            name TEXT NOT NULL COLLATE NOCASE,\
            ip TEXT NOT NULL,\
            admin TEXT NOT NULL,\
            reason TEXT NOT NULL,\
            created INTEGER NOT NULL,\
            expires INTEGER NOT NULL\
        )"));
    LoadRankove();
}

LoadRankove()
{
    for (new i = 1; i <= MAX_ADMIN_LEVEL; i++) format(gRankName[i], MAX_RANK_NAME, "Admin %d", i);
    gRankName[0] = "Igrac";
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT level, name FROM admin_ranks WHERE level BETWEEN 1 AND %d", MAX_ADMIN_LEVEL);
    for (new i = DB_GetRowCount(res); i > 0; i--)
    {
        DB_GetFieldString(res, 1, gRankName[DB_GetFieldInt(res, 0)], MAX_RANK_NAME);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

LoadAdmina(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT admin, premium FROM accounts WHERE id = %d", gAccountId[playerid]);
    gAdmin[playerid] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    gPremium[playerid] = DB_GetRowCount(res) && DB_GetFieldInt(res, 1);
    DB_FreeResultSet(res);
    if (gAdmin[playerid] > 0)
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Prijavljeni ste kao %s (razina %d)."), gRankName[gAdmin[playerid]], gAdmin[playerid]);
}

AdminPrijava(playerid)
{
    new name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof name);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || !gUlogiran[i] || gAdmin[i] < 1) continue;
        if (gAdmin[playerid])
            SendClientMessage(i, COLOR_ADMIN, T(i, "Igrac: %s se prijavio na server | Pozicija: Admin"), name);
        else
            SendClientMessage(i, COLOR_ADMIN, T(i, "Igrac: %s se prijavio na server | Pozicija: Obican igrac"), name);
    }
}

bool:PostaviAdmina(const name[], level)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q'", name);
    new found = DB_GetRowCount(res);
    DB_FreeResultSet(res);
    if (!found) return false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET admin = %d WHERE name = '%q'", level, name));

    new pid = FindPlayerByName(name);
    if (pid != INVALID_PLAYER_ID && gUlogiran[pid])
    {
        gAdmin[pid] = level;
        if (level > 0) SendClientMessage(pid, COLOR_ADMIN, T(pid, "Vasa admin razina je sada %d (%s)."), level, gRankName[level]);
        else SendClientMessage(pid, COLOR_ADMIN, T(pid, "Vise niste admin."));
    }
    return true;
}

bool:PostaviPremium(const name[], bool:ima)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM accounts WHERE name = '%q'", name);
    new found = DB_GetRowCount(res);
    DB_FreeResultSet(res);
    if (!found) return false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET premium = %d WHERE name = '%q'", ima, name));

    new pid = FindPlayerByName(name);
    if (pid != INVALID_PLAYER_ID && gUlogiran[pid])
    {
        gPremium[pid] = ima;
        SendClientMessage(pid, COLOR_ADMIN, ima ? T(pid, "Dobili ste PREMIUM status.") : T(pid, "Vise nemate PREMIUM status."));
    }
    return true;
}

bool:PostaviNazivRanka(level, const name[])
{
    if (level < 1 || level > MAX_ADMIN_LEVEL || !name[0] || strlen(name) >= MAX_RANK_NAME) return false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO admin_ranks (level, name) VALUES (%d, '%q')", level, name));
    LoadRankove();
    return true;
}

bool:ProvjeriBan(playerid)
{
    new name[MAX_PLAYER_NAME + 1], ip[16];
    GetPlayerName(playerid, name, sizeof name);
    GetPlayerIp(playerid, ip, sizeof ip);
    new DBResult:res = DB_ExecuteQuery(gDB,
        "SELECT admin, reason, expires FROM bans WHERE (name = '%q' OR ip = '%q') AND (expires = 0 OR expires > %d) LIMIT 1",
        name, ip, gettime());
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        return false;
    }
    new admin[MAX_PLAYER_NAME + 1], reason[96];
    DB_GetFieldString(res, 0, admin, sizeof admin);
    DB_GetFieldString(res, 1, reason, sizeof reason);
    new expires = DB_GetFieldInt(res, 2);
    DB_FreeResultSet(res);

    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Banani ste sa servera. Admin: %s, razlog: %s"), admin, reason);
    if (expires) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ban istice za %d h."), (expires - gettime()) / 3600 + 1);
    else SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ban je trajan."));
    KickTimer(playerid);
    return true;
}

Odbanaj(const name[])
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM bans WHERE name = '%q'", name);
    new n = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    if (n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bans WHERE name = '%q'", name));
    return n;
}

AdminConnect(playerid)
{
    gAdmin[playerid] = 0;
    gPremium[playerid] = false;
    gAdminVeh[playerid] = INVALID_VEHICLE_ID;
}

AdminDisconnect(playerid)
{
    gAdmin[playerid] = 0;
    if (gAdminVeh[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gAdminVeh[playerid]);
    gAdminVeh[playerid] = INVALID_VEHICLE_ID;
}

RestParam(const src[], idx, dest[], size = sizeof (dest))
{
    while (src[idx] == ' ') idx++;
    new end = idx;
    while (src[end] >= ' ' && src[end] < 127) end++;
    strmid(dest, src, idx, end, size);
}

FindPlayerByName(const name[])
{
    new other[MAX_PLAYER_NAME + 1];
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i)) continue;
        GetPlayerName(i, other, sizeof other);
        if (!strcmp(other, name, true)) return i;
    }
    return INVALID_PLAYER_ID;
}

FindPlayer(const s[])
{
    if (ProvjeriBroj(s))
    {
        new id = strval(s);
        return (id < MAX_PLAYERS && IsPlayerConnected(id)) ? id : INVALID_PLAYER_ID;
    }
    new other[MAX_PLAYER_NAME + 1], found = INVALID_PLAYER_ID;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i)) continue;
        GetPlayerName(i, other, sizeof other);
        if (strfind(other, s, true) != -1)
        {
            if (found != INVALID_PLAYER_ID) return INVALID_PLAYER_ID;
            found = i;
        }
    }
    return found;
}

bool:MozeKazniti(playerid, target)
{
    if (target == playerid)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to napraviti sami sebi."));
        return false;
    }
    if (gAdmin[target] >= gAdmin[playerid] && gAdmin[playerid] < MAX_ADMIN_LEVEL)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete kazniti admina iste ili vise razine."));
        return false;
    }
    return true;
}

CMD:a(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, 1)) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /a [poruka]"));
    RemoveBoje(params);
    new msg[200], name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof name);
    format(msg, sizeof msg, "[A] %s %s: %s", gRankName[gAdmin[playerid]], name, params);
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && gAdmin[i] > 0) SendLong(i, COLOR_ADMIN, msg);
    return 1;
}

CMD:pm(playerid, params[])
{
    new idx = 0, p1[32];
    if (!ProvjeriOvlasti(playerid, 1)) return 1;
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /pm [id/ime] [poruka]"));
    while (params[idx] == ' ') idx++;
    if (!params[idx]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /pm [id/ime] [poruka]"));
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    RemoveBoje(params);
    new msg[200], an[MAX_PLAYER_NAME + 1], tn[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, an, sizeof an);
    GetPlayerName(target, tn, sizeof tn);
    format(msg, sizeof msg, T(target, "(( PM od admina %s: %s ))"), an, params[idx]);
    SendLong(target, COLOR_ADMIN, msg);
    format(msg, sizeof msg, T(playerid, "(( PM za %s: %s ))"), tn, params[idx]);
    SendLong(playerid, COLOR_ADMIN, msg);
    return 1;
}

CMD:goto(playerid, params[])
{
    new idx = 0, p1[32], Float:x, Float:y, Float:z;
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /goto [id/ime]"));
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    GetPlayerPos(target, x, y, z);
    SetPlayerInterior(playerid, GetPlayerInterior(target));
    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(target));
    SetPlayerPos(playerid, x + 1.0, y, z + 0.5);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Teleportirani ste do igraca %s."), GetRPIme(target));
    return 1;
}

CMD:jetpack(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, 1)) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti pjeske."));
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
    {
        if (!SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nije moguce promijeniti jetpack."));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Jetpack je iskljucen."));
    }
    if (IsRanjen(playerid) || ImaLisice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete koristiti jetpack dok ste ranjeni ili imate lisice."));
    if (!SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nije moguce promijeniti jetpack."));
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Jetpack je ukljucen."));
}

CMD:afix(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, 1)) return 1;
    new v = GetPlayerVehicleID(playerid);
    if (!v) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti u vozilu."));
    if (!RepairVehicle(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo nije moguce popraviti."));
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo je popravljeno."));
}

CMD:admini(playerid, params[])
{
    new name[MAX_PLAYER_NAME + 1], count = 0;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Admini online:"));
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || gAdmin[i] < 1) continue;
        GetPlayerName(i, name, sizeof name);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "  %s - %s (%d)"), name, gRankName[gAdmin[i]], gAdmin[i]);
        count++;
    }
    if (!count) SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Trenutno nema admina online."));
    return 1;
}

AdminKick(playerid, const params[])
{
    new idx = 0, p1[32], rest[96];
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [razlog]"));
    RestParam(params, idx, rest);
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (!MozeKazniti(playerid, target)) return 1;
    if (!rest[0]) rest = "bez razloga";

    new an[MAX_PLAYER_NAME + 1], tn[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, an, sizeof an);
    GetPlayerName(target, tn, sizeof tn);
    PosaljiSvima(COLOR_ADMIN, "AdmCmd: %s je izbacio igraca %s. Razlog: %s", an, tn, rest);
    KickTimer(target);
    return 1;
}

AdminBan(playerid, const params[])
{
    new idx = 0, p1[32], p2[32], rest[96];
    if (!ProvjeriOvlasti(playerid, ALVL_BAN)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [dani, 0 = trajno] [razlog]"));
    RestParam(params, idx, rest);
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (!MozeKazniti(playerid, target)) return 1;
    if (!rest[0]) rest = "bez razloga";

    new days = strval(p2);
    if (days > 3650) days = 0;
    new an[MAX_PLAYER_NAME + 1], tn[MAX_PLAYER_NAME + 1], ip[16];
    GetPlayerName(playerid, an, sizeof an);
    GetPlayerName(target, tn, sizeof tn);
    GetPlayerIp(target, ip, sizeof ip);
    DB_FreeResultSet(DB_ExecuteQuery(gDB,
        "INSERT INTO bans (name, ip, admin, reason, created, expires) VALUES ('%q', '%q', '%q', '%q', %d, %d)",
        tn, ip, an, rest, gettime(), days ? gettime() + days * 86400 : 0));

    if (days) { new dani[12]; valstr(dani, days); PosaljiSvima(COLOR_ADMIN, "AdmCmd: %s je banao igraca %s na %s dana. Razlog: %s", an, tn, dani, rest); }
    else PosaljiSvima(COLOR_ADMIN, "AdmCmd: %s je trajno banao igraca %s. Razlog: %s", an, tn, rest);
    KickTimer(target);
    return 1;
}

AdminOdban(playerid, const params[])
{
    new idx = 0, p1[32];
    if (!ProvjeriOvlasti(playerid, ALVL_BAN)) return 1;
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [ime]"));
    if (!Odbanaj(p1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac %s nema aktivan ban."), p1);
    new an[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, an, sizeof an);
    PosaljiSvima(COLOR_ADMIN, "AdmCmd: %s je odbanao igraca %s.", an, p1);
    return 1;
}

new const gImenaVozila[][] =
{
    "Landstalker",
    "Bravura",
    "Buffalo",
    "Linerunner",
    "Perennial",
    "Sentinel",
    "Dumper",
    "Firetruck",
    "Trashmaster",
    "Stretch",
    "Manana",
    "Infernus",
    "Voodoo",
    "Pony",
    "Mule",
    "Cheetah",
    "Ambulance",
    "Leviathan",
    "Moonbeam",
    "Esperanto",
    "Taxi",
    "Washington",
    "Bobcat",
    "Mr Whoopee",
    "BF Injection",
    "Hunter",
    "Premier",
    "Enforcer",
    "Securicar",
    "Banshee",
    "Predator",
    "Bus",
    "Rhino",
    "Barracks",
    "Hotknife",
    "Trailer 1",
    "Previon",
    "Coach",
    "Cabbie",
    "Stallion",
    "Rumpo",
    "RC Bandit",
    "Romero",
    "Packer",
    "Monster",
    "Admiral",
    "Squalo",
    "Seasparrow",
    "Pizzaboy",
    "Tram",
    "Trailer 2",
    "Turismo",
    "Speeder",
    "Reefer",
    "Tropic",
    "Flatbed",
    "Yankee",
    "Caddy",
    "Solair",
    "Berkley's RC Van",
    "Skimmer",
    "PCJ-600",
    "Faggio",
    "Freeway",
    "RC Baron",
    "RC Raider",
    "Glendale",
    "Oceanic",
    "Sanchez",
    "Sparrow",
    "Patriot",
    "Quad",
    "Coastguard",
    "Dinghy",
    "Hermes",
    "Sabre",
    "Rustler",
    "ZR-350",
    "Walton",
    "Regina",
    "Comet",
    "BMX",
    "Burrito",
    "Camper",
    "Marquis",
    "Baggage",
    "Dozer",
    "Maverick",
    "News Chopper",
    "Rancher",
    "FBI Rancher",
    "Virgo",
    "Greenwood",
    "Jetmax",
    "Hotring",
    "Sandking",
    "Blista Compact",
    "Police Maverick",
    "Boxville",
    "Benson",
    "Mesa",
    "RC Goblin",
    "Hotring Racer A",
    "Hotring Racer B",
    "Bloodring Banger",
    "Rancher Lure",
    "Super GT",
    "Elegant",
    "Journey",
    "Bike",
    "Mountain Bike",
    "Beagle",
    "Cropduster",
    "Stunt",
    "Tanker",
    "Roadtrain",
    "Nebula",
    "Majestic",
    "Buccaneer",
    "Shamal",
    "Hydra",
    "FCR-900",
    "NRG-500",
    "HPV1000",
    "Cement Truck",
    "Tow Truck",
    "Fortune",
    "Cadrona",
    "FBI Truck",
    "Willard",
    "Forklift",
    "Tractor",
    "Combine",
    "Feltzer",
    "Remington",
    "Slamvan",
    "Blade",
    "Freight",
    "Streak",
    "Vortex",
    "Vincent",
    "Bullet",
    "Clover",
    "Sadler",
    "Firetruck LA",
    "Hustler",
    "Intruder",
    "Primo",
    "Cargobob",
    "Tampa",
    "Sunrise",
    "Merit",
    "Utility",
    "Nevada",
    "Yosemite",
    "Windsor",
    "Monster A",
    "Monster B",
    "Uranus",
    "Jester",
    "Sultan",
    "Stratum",
    "Elegy",
    "Raindance",
    "RC Tiger",
    "Flash",
    "Tahoma",
    "Savanna",
    "Bandito",
    "Freight Flat",
    "Streak Carriage",
    "Kart",
    "Mower",
    "Duneride",
    "Sweeper",
    "Broadway",
    "Tornado",
    "AT-400",
    "DFT-30",
    "Huntley",
    "Stafford",
    "BF-400",
    "Newsvan",
    "Tug",
    "Trailer 3",
    "Emperor",
    "Wayfarer",
    "Euros",
    "Hotdog",
    "Club",
    "Freight Box",
    "Trailer 4",
    "Andromada",
    "Dodo",
    "RC Cam",
    "Launch",
    "Police Car LSPD",
    "Police Car SFPD",
    "Police Car LVPD",
    "Police Ranger",
    "Picador",
    "SWAT",
    "Alpha",
    "Phoenix",
    "Glendale Shit",
    "Sadler Shit",
    "Luggage Trailer A",
    "Luggage Trailer B",
    "Stair Trailer",
    "Boxville Black",
    "Farm Plow",
    "Utility Trailer"
};

AdminVozilo(playerid, const params[])
{
    new idx = 0, p1[32], p2[32], p3[32];
    if (!ProvjeriOvlasti(playerid, ALVL_VEH)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [model 400-611] [boja1] [boja2]"));
    new model = strval(p1);
    if (model < 400 || model > 611) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Model mora biti izmedu 400 i 611."));
    new c1 = SljedeciParam(params, idx, p2) ? strval(p2) : -1;
    new c2 = SljedeciParam(params, idx, p3) ? strval(p3) : -1;
    return AdminStvoriVozilo(playerid, model, c1, c2);
}

static AdminStvoriVozilo(playerid, model, c1, c2)
{
    if (gAdminVeh[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gAdminVeh[playerid]);
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    if (model == PRIKOLICA_MODEL)
    {
        x += 5.0 * floatsin(-a, degrees);
        y += 5.0 * floatcos(-a, degrees);
    }
    new veh = CreateVehicle(model, x, y, z + 1.0, a, c1, c2, -1);
    if (veh == INVALID_VEHICLE_ID)
    {
        gAdminVeh[playerid] = INVALID_VEHICLE_ID;
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo se ne moze stvoriti."));
    }
    gAdminVeh[playerid] = veh;
    SetVehicleVirtualWorld(veh, GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(veh, GetPlayerInterior(playerid));
    if (model != PRIKOLICA_MODEL) PutPlayerInVehicle(playerid, veh, 0);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo %d stvoreno. Brise se u admin panelu (Vozila)."), model);
    return 1;
}

ModelVozila(const ime[])
{
    new djelomicno = 0;
    for (new i = 0; i < sizeof gImenaVozila; i++)
    {
        if (!strcmp(gImenaVozila[i], ime, true)) return 400 + i;
        if (!djelomicno && strfind(gImenaVozila[i], ime, true) != -1) djelomicno = 400 + i;
    }
    return djelomicno;
}

CMD:veh(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_VEH)) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /veh [ime vozila]"));
    new model = ModelVozila(params);
    if (!model) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo nije pronadeno."));
    return AdminStvoriVozilo(playerid, model, -1, -1);
}

CMD:prikolica(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_VEH)) return 1;
    return AdminStvoriVozilo(playerid, PRIKOLICA_MODEL, -1, -1);
}

CMD:dozvola(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    new idx = 0, p1[32], p2[16];
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /dozvola [id/ime] [vozacka/oruzje]"));
    new bool:oruzje = SljedeciParam(params, idx, p2) && !strcmp(p2, "oruzje", true);
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (oruzje) DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET oruzje_dozvola = 1 - oruzje_dozvola WHERE id = %d", gAccountId[target]));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET vozacka = 1 - vozacka WHERE id = %d", gAccountId[target]));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT vozacka, oruzje_dozvola FROM accounts WHERE id = %d", gAccountId[target]), bool:ima = DB_GetRowCount(res) && DB_GetFieldInt(res, oruzje ? 1 : 0);
    DB_FreeResultSet(res);
    if (oruzje && ima)
    {
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s sada ima dozvolu za oruzje."), GetRPIme(target));
        SendClientMessage(target, COLOR_INFO, T(target, "Dobili ste dozvolu za oruzje."));
    }
    else if (oruzje)
    {
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s vise nema dozvolu za oruzje."), GetRPIme(target));
        SendClientMessage(target, COLOR_INFO, T(target, "Oduzeta vam je dozvola za oruzje."));
    }
    else if (ima)
    {
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s sada ima vozacku dozvolu."), GetRPIme(target));
        SendClientMessage(target, COLOR_INFO, T(target, "Dobili ste vozacku dozvolu."));
    }
    else
    {
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s vise nema vozacku dozvolu."), GetRPIme(target));
        SendClientMessage(target, COLOR_INFO, T(target, "Oduzeta vam je vozacka dozvola."));
    }
    return 1;
}

CMD:wound(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /wound [id/ime]"));
    new target = FindPlayer(params);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden ili nije prijavljen."));
    if (IsRanjen(target)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac je ranjen."));
    RanjenOdmah(target);
    return 1;
}

AdminObrisiVozilo(playerid)
{
    if (!ProvjeriOvlasti(playerid, ALVL_VEH)) return 1;
    if (gAdminVeh[playerid] == INVALID_VEHICLE_ID) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate stvoreno vozilo."));
    DestroyVehicle(gAdminVeh[playerid]);
    gAdminVeh[playerid] = INVALID_VEHICLE_ID;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo obrisano."));
    return 1;
}

AdminPostaviRazinu(playerid, const params[])
{
    new idx = 0, p1[32], p2[32];
    if (!ProvjeriOvlasti(playerid, ALVL_MANAGE)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [razina 0-%d]"), MAX_ADMIN_LEVEL);
    new level = strval(p2), target = FindPlayer(p1);
    if (level > MAX_ADMIN_LEVEL) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Razina mora biti 0-%d."), MAX_ADMIN_LEVEL);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden ili nije prijavljen."));
    new tn[MAX_PLAYER_NAME + 1];
    GetPlayerName(target, tn, sizeof tn);
    PostaviAdmina(tn, level);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s sada ima razinu %d (%s)."), tn, level, gRankName[level]);
    return 1;
}

AdminPremium(playerid, const params[])
{
    new idx = 0, p1[32], p2[32];
    if (!ProvjeriOvlasti(playerid, ALVL_MONEY)) return 1;
    if (!SljedeciParam(params, idx, p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2) || strval(p2) > 1)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime] [1 = daj, 0 = makni]"));
    new name[MAX_PLAYER_NAME + 1], target = FindPlayer(p1), bool:ima = strval(p2) == 1;
    if (target != INVALID_PLAYER_ID) GetPlayerName(target, name, sizeof name);
    else strcat(name, p1);
    if (!PostaviPremium(name, ima)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    SendClientMessage(playerid, COLOR_ADMIN, ima ? T(playerid, "%s sada ima PREMIUM.") : T(playerid, "%s vise nema PREMIUM."), name);
    return 1;
}

AdminNazivRanka(playerid, const params[])
{
    new idx = 0, p1[32], rest[96];
    if (!ProvjeriOvlasti(playerid, ALVL_MANAGE)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [razina 1-%d] [naziv]"), MAX_ADMIN_LEVEL);
    RestParam(params, idx, rest);
    if (!PostaviNazivRanka(strval(p1), rest))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Razina mora biti 1-%d, naziv 1-%d znakova."), MAX_ADMIN_LEVEL, MAX_RANK_NAME - 1);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Razina %d se sada zove: %s"), strval(p1), rest);
    return 1;
}

public OnRconCommand(cmd[])
{
    new idx = 0, word[32], p1[MAX_PLAYER_NAME + 1], p2[32], rest[MAX_RANK_NAME];
    SljedeciParam(cmd, idx, word);

    if (!strcmp(word, "setadmin", true))
    {
        if (!SljedeciParam(cmd, idx, p1) || !SljedeciParam(cmd, idx, p2) || !ProvjeriBroj(p2) || strval(p2) > MAX_ADMIN_LEVEL)
        {
            printf("Koristenje: setadmin <ime> <razina 0-%d>", MAX_ADMIN_LEVEL);
            return 1;
        }
        new level = strval(p2);
        if (PostaviAdmina(p1, level)) printf("%s sada ima admin razinu %d (%s).", p1, level, gRankName[level]);
        else printf("Racun %s ne postoji.", p1);
        return 1;
    }
    if (!strcmp(word, "nazivranka", true))
    {
        if (!SljedeciParam(cmd, idx, p2) || !ProvjeriBroj(p2))
        {
            printf("Koristenje: nazivranka <razina 1-%d> <naziv>", MAX_ADMIN_LEVEL);
            return 1;
        }
        RestParam(cmd, idx, rest);
        if (PostaviNazivRanka(strval(p2), rest)) printf("Razina %d se sada zove: %s", strval(p2), rest);
        else printf("Razina mora biti 1-%d, naziv 1-%d znakova.", MAX_ADMIN_LEVEL, MAX_RANK_NAME - 1);
        return 1;
    }
    if (!strcmp(word, "rankovi", true))
    {
        for (new i = 1; i <= MAX_ADMIN_LEVEL; i++) printf("%d - %s", i, gRankName[i]);
        return 1;
    }
    if (!strcmp(word, "unban", true))
    {
        if (!SljedeciParam(cmd, idx, p1)) print("Koristenje: unban <ime>");
        else if (Odbanaj(p1)) printf("%s je odbanan.", p1);
        else printf("%s nema aktivan ban.", p1);
        return 1;
    }
    return 0;
}

enum
{
    AP_KICK, AP_BAN, AP_UNBAN, AP_VEH, AP_SETADMIN, AP_RANKNAME, AP_MONEY, AP_ITEM, AP_PREMIUM, AP_BROJ,
    AP_TER_KREIRAJ, AP_TER_VLASNIK, AP_TER_NPC,
    AP_ORG_CREATE, AP_ORG_LEADER, AP_ORG_REMOVE, AP_ORG_SKIN_ADD, AP_ORG_SKIN_DEL, AP_ORG_VEH, AP_ORG_BOJA
}

static const gApItem[][] = {
    "Izbaci igraca", "Banaj igraca", "Odbanaj igraca", "Stvori vozilo", "Obrisi stvoreno vozilo",
    "Admini online", "Organizacije", "Daj novac", "Daj predmet", "Postavi admina", "Nazivi admin rankova", "Teritoriji", "Premium status", "Taksi stanice", "Kurve", "Vatrogasni kamioni", "Ambulante", "Pumpe goriva", "Trgovine", "Saloni automobila", "Opstina", "Auto pijaca", "Broj mobitela", "Mehanicarska vozila", "Oruzarnice", "Odjeca i frizeri", "Kuce", "Tuning garaze", "Garaze", "Trgovine namjestaja"
};
static const gApLevel[sizeof gApItem] = { ALVL_KICK, ALVL_BAN, ALVL_BAN, ALVL_VEH, ALVL_VEH, 1, ALVL_ORG, ALVL_MONEY, ALVL_MONEY, ALVL_MANAGE, ALVL_MANAGE, ALVL_ORG, ALVL_MONEY, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_MONEY, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG, ALVL_ORG };

static gApAction[MAX_PLAYERS];
static gApOrg[MAX_PLAYERS];

CMD:apanel(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, 1)) return 1;
    return ShowAdminPanel(playerid);
}

static const gApKat[][] = { "Igraci", "Vozila", "Organizacije", "Teritoriji", "Administracija" };
static const gApKatStavke[sizeof gApKat][11] = {
    { 0, 1, 2, 7, 8, 22, -1, -1, -1, -1, -1 }, { 3, 4, 13, 15, 16, 23, 17, 19, 21, 27, 28 }, { 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, { 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, { 5, 9, 10, 12, 14, 18, 24, 25, 26, 20, 29 }
};
static gApKatOdabrana[MAX_PLAYERS];

ShowAdminPanel(playerid)
{
    new list[256], row[48], title[64], len = 0;
    for (new k = 0; k < sizeof gApKat; k++)
    {
        new najniza = MAX_ADMIN_LEVEL;
        for (new j = 0; j < sizeof gApKatStavke[]; j++) if (gApKatStavke[k][j] != -1 && gApLevel[gApKatStavke[k][j]] < najniza) najniza = gApLevel[gApKatStavke[k][j]];
        format(row, sizeof row, T(playerid, "%s%s"), gAdmin[playerid] >= najniza ? ("") : ("{777777}"), T(playerid, gApKat[k]));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, T(playerid, "Admin panel - %s (%d)"), gRankName[gAdmin[playerid]], gAdmin[playerid]);
    return PrikaziDialog(playerid, DLG_APANEL_KAT, DIALOG_STYLE_LIST, title, list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

ShowKategoriju(playerid)
{
    new k = gApKatOdabrana[playerid], list[256], row[48], len = 0;
    for (new j = 0; j < sizeof gApKatStavke[]; j++)
    {
        new i = gApKatStavke[k][j];
        if (i == -1) break;
        format(row, sizeof row, T(playerid, "%s%s"), gAdmin[playerid] >= gApLevel[i] ? ("") : ("{777777}"), T(playerid, gApItem[i]));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL, DIALOG_STYLE_LIST, T(playerid, gApKat[k]), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_kat(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gApKat) return 1;
    gApKatOdabrana[playerid] = listitem;
    if (gApKatStavke[listitem][1] == -1) return OtvoriStavku(playerid, gApKatStavke[listitem][0]);
    return ShowKategoriju(playerid);
}

ShowPopisOrg(playerid)
{
    static list[4096];
    new name[MAX_ORG_NAME], row[96];
    list[0] = EOS;
    strcat(list, T(playerid, "{8AB4F8}+ Nova organizacija\n{8AB4F8}Obrisi org vozilo u kojem sjedim\n"));
    new len = strlen(list);
    new DBResult:res = DB_ExecuteQuery(gDB,
        "SELECT o.id, o.name, o.type, (SELECT COUNT(*) FROM accounts a WHERE a.org = o.id) FROM orgs o ORDER BY o.id");
    if (DB_GetRowCount(res)) do
    {
        DB_GetFieldString(res, 1, name);
        format(row, sizeof row, T(playerid, "[%d] %s - %s, %d clanova"), DB_GetFieldInt(res, 0), name,
            T(playerid, gOrgTypeName[clamp(DB_GetFieldInt(res, 2), 0, sizeof gOrgTypeName - 1)]), DB_GetFieldInt(res, 3));
    } while (DodajStupac(list, len, row) && DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    return PrikaziDialog(playerid, DLG_APANEL_ORGOVI, DIALOG_STYLE_LIST, T(playerid, "Admin panel - Organizacije"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

ShowIzbornikOrg(playerid)
{
    new name[MAX_ORG_NAME], title[64];
    if (!GetOrgIme(gApOrg[playerid], name)) return ShowPopisOrg(playerid);
    format(title, sizeof title, T(playerid, "Organizacija: %s"), name);
    return PrikaziDialog(playerid, DLG_APANEL_ORG, DIALOG_STYLE_LIST, title,
        T(playerid, "Postavi lidera\nMakni clana\nDodaj skin\nUkloni skin\nParkiraj vozilo ovdje\nObrisi organizaciju\nPostavi HQ ovdje\nPromijeni boju"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

ShowUnos(playerid, action, const prompt[])
{
    gApAction[playerid] = action;
    return PrikaziDialog(playerid, DLG_APANEL_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Admin panel"), prompt, T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

static Akcija(playerid, action, const input[])
{
    new p[160];
    switch (action)
    {
        case AP_KICK:         { format(p, sizeof p, "%s", input); AdminKick(playerid, p); }
        case AP_BAN:          { format(p, sizeof p, "%s", input); AdminBan(playerid, p); }
        case AP_UNBAN:        { format(p, sizeof p, "%s", input); AdminOdban(playerid, p); }
        case AP_VEH:          { format(p, sizeof p, "%s", input); AdminVozilo(playerid, p); }
        case AP_SETADMIN:     { format(p, sizeof p, "%s", input); AdminPostaviRazinu(playerid, p); }
        case AP_RANKNAME:     { format(p, sizeof p, "%s", input); AdminNazivRanka(playerid, p); }
        case AP_MONEY:        { format(p, sizeof p, "%s", input); AdminDajNovac(playerid, p); }
        case AP_ITEM:         { format(p, sizeof p, "%s", input); AdminGiveItem(playerid, p); }
        case AP_PREMIUM:      { format(p, sizeof p, "%s", input); AdminPremium(playerid, p); }
        case AP_BROJ:         { format(p, sizeof p, "%s", input); MobitelPostaviBroj(playerid, p); }
        case AP_TER_KREIRAJ:  { format(p, sizeof p, "%s", input); CreateTer(playerid, p); }
        case AP_TER_VLASNIK:  { format(p, sizeof p, "%d %s", TerOdabraniDb(playerid), input); TerVlasnik(playerid, p); }
        case AP_TER_NPC:      { format(p, sizeof p, "%d %s", TerOdabraniDb(playerid), input); TerDodajDilera(playerid, p); }
        case AP_ORG_CREATE:   { format(p, sizeof p, "%d %s", gApOrg[playerid], input); KreirajOrg(playerid, p); }
        case AP_ORG_LEADER:   { format(p, sizeof p, "%d %s", gApOrg[playerid], input); OrgPostaviLidera(playerid, p); }
        case AP_ORG_REMOVE:   { format(p, sizeof p, "%s", input); OrgMakni(playerid, p); }
        case AP_ORG_SKIN_ADD: { format(p, sizeof p, "%d dodaj %s", gApOrg[playerid], input); OrgSkin(playerid, p); }
        case AP_ORG_SKIN_DEL: { format(p, sizeof p, "%d ukloni %s", gApOrg[playerid], input); OrgSkin(playerid, p); }
        case AP_ORG_VEH:      { format(p, sizeof p, "%d %s", gApOrg[playerid], input); OrgVozilo(playerid, p); }
        case AP_ORG_BOJA:     { format(p, sizeof p, "%s", input); OrgPostaviBoju(playerid, gApOrg[playerid], p); }
    }
}

DLG:apanel(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (listitem < 0 || listitem >= sizeof gApKatStavke[] || gApKatStavke[gApKatOdabrana[playerid]][listitem] == -1) return 1;
    return OtvoriStavku(playerid, gApKatStavke[gApKatOdabrana[playerid]][listitem]);
}

static OtvoriStavku(playerid, stavka)
{
    new empty[1];
    if (!ProvjeriOvlasti(playerid, gApLevel[stavka])) return ShowAdminPanel(playerid);
    switch (stavka)
    {
        case 0: return ShowUnos(playerid, AP_KICK, T(playerid, "Unesite: [id/ime] [razlog]"));
        case 1: return ShowUnos(playerid, AP_BAN, T(playerid, "Unesite: [id/ime] [dani, 0 = trajno] [razlog]"));
        case 2: return ShowUnos(playerid, AP_UNBAN, T(playerid, "Unesite tocno ime igraca:"));
        case 3: return ShowUnos(playerid, AP_VEH, T(playerid, "Unesite: [model 400-611] [boja1] [boja2]"));
        case 4: AdminObrisiVozilo(playerid);
        case 5: cmd_admini(playerid, empty);
        case 6: return ShowPopisOrg(playerid);
        case 11: return ShowPopisTer(playerid);
        case 13: return ShowTaksiStanice(playerid);
        case 14: return ShowKurve(playerid);
        case 15: return ShowKamione(playerid, ORG_VATROGASCI);
        case 16: return ShowKamione(playerid, ORG_MEDICINSKA);
        case 17: return ShowPumpe(playerid);
        case 18: return ShowTrgovine(playerid);
        case 19: return ShowSaloni(playerid);
        case 20: return ShowOpstinaAdmin(playerid);
        case 21: return ShowPijacaAdmin(playerid);
        case 23: return ShowMehanicare(playerid);
        case 24: return ShowOruzarnice(playerid);
        case 25: return ShowOdjece(playerid);
        case 26: return ShowKuceAdmin(playerid);
        case 27: return ShowTuningAdmin(playerid);
        case 28: return ShowGarazeAdmin(playerid);
        case 29: return ShowNamjestaje(playerid);
        case 22: return ShowUnos(playerid, AP_BROJ, T(playerid, "Unesite: [id/ime] [novi broj, 7 znamenki]"));
        case 12: return ShowUnos(playerid, AP_PREMIUM, T(playerid, "Unesite: [id/ime] [1 = daj, 0 = makni]"));
        case 7: return ShowUnos(playerid, AP_MONEY, T(playerid, "Unesite: [id/ime] [iznos, negativan oduzima]"));
        case 8:
        {
            new body[1024] = "Predmeti:\n", row[64];
            new len = strlen(body);
            for (new i = 0; i < BROJ_ITEMA; i++)
            {
                format(row, sizeof row, T(playerid, "  %d - %s (najvise %d %s)"), i, T(playerid, gItems[i][ITEM_NAME]), gItems[i][ITEM_MAX], T(playerid, gItems[i][ITEM_UNIT]));
                DodajStupac(body, len, row);
            }
            strcat(body, "\nUnesite: [id/ime] [predmet] [kolicina, negativna oduzima]");
            return ShowUnos(playerid, AP_ITEM, body);
        }
        case 9:
        {
            new body[96];
            format(body, sizeof body, T(playerid, "Unesite: [id/ime] [razina 0-%d]"), MAX_ADMIN_LEVEL);
            return ShowUnos(playerid, AP_SETADMIN, body);
        }
        case 10:
        {
            new body[512] = "Trenutni nazivi:\n", row[48];
            new len = strlen(body);
            for (new i = 1; i <= MAX_ADMIN_LEVEL; i++)
            {
                format(row, sizeof row, T(playerid, "  %d - %s"), i, gRankName[i]);
                DodajStupac(body, len, row);
            }
            strcat(body, "\nUnesite: [razina] [novi naziv]");
            return ShowUnos(playerid, AP_RANKNAME, body);
        }
    }
    return ShowKategoriju(playerid);
}

DLG:apanel_orgovi(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (listitem == 0)
        return PrikaziDialog(playerid, DLG_APANEL_TIP_ORG, DIALOG_STYLE_LIST, T(playerid, "Tip nove organizacije"), T(playerid, "Banda\nMafija\nLegalna\nLSPD\nVatrogasci i hitna pomoc\nTaksi sluzba\nMehanicari"), T(playerid, "Dalje"), T(playerid, "Natrag"));
    if (listitem == 1)
    {
        OrgUkloniVozilo(playerid);
        return ShowPopisOrg(playerid);
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM orgs ORDER BY id LIMIT 1 OFFSET %d", listitem - AP_ORG_LIST_FIXED);
    gApOrg[playerid] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return ShowIzbornikOrg(playerid);
}

DLG:apanel_tip_org(playerid, response, listitem)
{
    if (!response) return ShowPopisOrg(playerid);
    gApOrg[playerid] = listitem >= ORG_MEDICINSKA ? listitem + 1 : listitem;
    return ShowUnos(playerid, AP_ORG_CREATE, T(playerid, "Unesite ime nove organizacije:"));
}

DLG:apanel_org(playerid, response, listitem)
{
    if (!response) return ShowPopisOrg(playerid);
    switch (listitem)
    {
        case 0: return ShowUnos(playerid, AP_ORG_LEADER, T(playerid, "Unesite id/ime novog lidera (mora biti online):"));
        case 1: return ShowUnos(playerid, AP_ORG_REMOVE, T(playerid, "Unesite id/ime clana (offline: tocno ime):"));
        case 2: return ShowUnos(playerid, AP_ORG_SKIN_ADD, T(playerid, "Unesite ID skina (0-311) koji dodajete:"));
        case 3: return ShowUnos(playerid, AP_ORG_SKIN_DEL, T(playerid, "Unesite ID skina (0-311) koji uklanjate:"));
        case 4: return ShowUnos(playerid, AP_ORG_VEH, T(playerid, "Vozilo se parkira na vasem mjestu.\nUnesite: [model 400-611] [boja1] [boja2]"));
        case 6: return ShowHqIzbor(playerid, gApOrg[playerid]);
        case 7: return ShowUnos(playerid, AP_ORG_BOJA, T(playerid, "Unesite boju u hex obliku, npr. FF0000:"));
        case 5:
        {
            new name[MAX_ORG_NAME], body[160];
            if (!GetOrgIme(gApOrg[playerid], name)) return ShowPopisOrg(playerid);
            format(body, sizeof body, T(playerid, "Obrisati organizaciju '%s'?\nSvi clanovi, skinovi i vozila bit ce uklonjeni."), name);
            return PrikaziDialog(playerid, DLG_APANEL_BRISANJE_ORG, DIALOG_STYLE_MSGBOX, T(playerid, "Brisanje organizacije"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
        }
    }
    return ShowIzbornikOrg(playerid);
}

DLG:apanel_brisanje_org(playerid, response)
{
    new p[16];
    if (!response) return ShowIzbornikOrg(playerid);
    format(p, sizeof p, "%d", gApOrg[playerid]);
    ObrisiOrg(playerid, p);
    return ShowPopisOrg(playerid);
}

DLG:apanel_unos(playerid, response, const inputtext[])
{
    new action = gApAction[playerid];
    gOrgNova = 0;
    if (response && inputtext[0]) Akcija(playerid, action, inputtext);
    if (action == AP_TER_KREIRAJ) return ShowPopisTer(playerid);
    if (action == AP_TER_VLASNIK || action == AP_TER_NPC) return ShowTerMeni(playerid);
    if (action == AP_ORG_CREATE)
    {
        if (!gOrgNova) return ShowPopisOrg(playerid);
        gApOrg[playerid] = gOrgNova;
        return ShowHqIzbor(playerid, gOrgNova);
    }
    if (action >= AP_ORG_CREATE) return ShowIzbornikOrg(playerid);
    if (response && action == AP_VEH) return 1;
    return ShowKategoriju(playerid);
}
