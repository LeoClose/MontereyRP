public OnPlayerConnect(playerid)
{
    MapaOpstinaUkloni(playerid);
    MapaIdlewoodUkloni(playerid);
    MapaIdlewoodFlatsUkloni(playerid);
    MapaChinatownUkloni(playerid);
    if (IsPlayerNPC(playerid)) return 1;
    new ljudi = 0;
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && !IsPlayerNPC(i)) ljudi++;
    if (ljudi > MAX_LJUDI)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Server je pun."));
        return KickTimer(playerid);
    }
    StetaConnect(playerid);
    PijacaConnect(playerid);
    GranicaConnect(playerid);
    gAccountId[playerid] = 0;
    gUlogiran[playerid] = false;
    gLoginAttempts[playerid] = 0;
    DialogConnect(playerid);
    AdminConnect(playerid);
    JezikConnect(playerid);
    OrgConnect(playerid);
    StatsConnect(playerid);
    NovacConnect(playerid);
    InventarConnect(playerid);
    OruzjeConnect(playerid);
    TeritorijConnect(playerid);
    if (ProvjeriBan(playerid)) return 1;

    new name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof name);

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT jezik FROM accounts WHERE name = '%q' LIMIT 1", name);
    new exists = DB_GetRowCount(res);
    if (exists && DB_GetFieldInt(res, 0) > 0 && DB_GetFieldInt(res, 0) < BROJ_JEZIKA) gJezik[playerid] = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);

    if (exists) PrikaziLogin(playerid, "");
    else PrikaziRegistraciju(playerid, "");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    SlucajDisconnect(playerid);
    if (IsPlayerNPC(playerid)) return 1;
    TaksiPlayerLeft(playerid);
    KurvaDisconnect(playerid);
    MaricaReset(playerid);
    PolicijaDisconnect(playerid, true);
    GepekDisconnect(playerid);
    VoziloDisconnect(playerid);
    SalonDisconnect(playerid);
    OruzarnicaDisconnect(playerid);
    MdtDisconnect(playerid);
    OdjecaDisconnect(playerid);
    TuningDisconnect(playerid);
    KucaDisconnect(playerid);
    MobitelDisconnect(playerid);
    SpremiSvoOruzje(playerid);
    StatsDisconnect(playerid);
    gUlogiran[playerid] = false;
    AdminDisconnect(playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    if (IsPlayerNPC(playerid)) return 1;
    TogglePlayerSpectating(playerid, true);
    if (gUlogiran[playerid]) SetTimerEx("ReturnPlayer", 100, false, "i", playerid);
    return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    return 0;
}

forward ReturnPlayer(playerid);
public ReturnPlayer(playerid)
{
    if (gUlogiran[playerid]) TogglePlayerSpectating(playerid, false);
}

public OnPlayerSpawn(playerid)
{
    if (IsPlayerNPC(playerid)) return 1;
    StetaSpawn(playerid);
    SetPlayerSkin(playerid, OdjecaSkinIgraca(playerid));
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, SPAWN_X, SPAWN_Y, SPAWN_Z);
    SetPlayerFacingAngle(playerid, SPAWN_A);
    SetCameraBehindPlayer(playerid);
    AnimPreload(playerid);
    PolicijaSpawn(playerid);
    GepekSpawn(playerid);
    KucaSpawn(playerid);
    RanjenSpawn(playerid);
    OdjecaSpawn(playerid);
    return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if (IsPlayerNPC(playerid)) return 1;
    MobitelZatvori(playerid);
    TaksiPlayerLeft(playerid);
    KurvaDisconnect(playerid);
    MaricaReset(playerid);
    PolicijaDisconnect(playerid, false);
    if (!RanjenPadni(playerid)) OruzjeDeath(playerid);
    TeritorijDeath(playerid);
    return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    TaksiStateChange(playerid, newstate, oldstate);
    PolicijaStateChange(playerid, newstate);
    MdtStateChange(playerid, oldstate);
    TuningStateChange(playerid, oldstate);
    MaricaEnter(playerid, newstate);
    SlucajStateChange(playerid, newstate);
    VoziloStateChange(playerid, newstate);
    VlakStateChange(playerid, newstate);
    if (newstate == PLAYER_STATE_DRIVER) VoziloVozac(playerid);
    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (TuningKey(playerid, newkeys, oldkeys) || MobitelKey(playerid, newkeys, oldkeys) || MdtKey(playerid, newkeys, oldkeys)) return 1;
    TaksiKey(playerid, newkeys, oldkeys);
    MaricaKey(playerid, newkeys, oldkeys);
    AnimKey(playerid, newkeys, oldkeys);
    VoziloKey(playerid, newkeys, oldkeys);
    KurvaPotrubiKey(playerid, newkeys, oldkeys);
    if ((newkeys & KEY_YES) && !(oldkeys & KEY_YES)) Interakcija(playerid);
    return 1;
}

// Interakcija je na Y
static Interakcija(playerid)
{
    if (gUlogiran[playerid] && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && HqUlazIzlaz(playerid)) return;
    if (TuningUlaz(playerid) || GarazaKey(playerid) || KurvaUlica(playerid) || BacenoPodigni(playerid) || TrgovinaKasa(playerid) || OruzarnicaKasa(playerid) || OdjecaKasa(playerid) || KucaKey(playerid) || OpstinaSalter(playerid) || SalonUlaz(playerid) || PijacaKey(playerid)) return;
    DilerKey(playerid);
}

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (TaserPogodak(playerid, weaponid, hittype, hitid)) return 0;
    RanjenPogodak(playerid, hittype, hitid);
    HeliPucnjava(playerid, hittype);
    if (IsShootingBlocked(playerid))
    {
        StetaBlokiraj(playerid);
        return 0;
    }
    StetaVozilo(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    return 1;
}

public OnPlayerUpdate(playerid)
{
    RanjenUpdate(playerid);
    MobitelUpdate(playerid);
    VoziloUpdate(playerid);
    TuningUpdate(playerid);
    TrgovinaUpdate(playerid);
    TaksiPovratak(playerid);
    new WEAPON:w = GetPlayerWeapon(playerid);
    if (w >= WEAPON_COLT45 && w <= WEAPON_MINIGUN && IsShootingBlocked(playerid)) SetPlayerArmedWeapon(playerid, WEAPON_FIST);
    if (w != WEAPON_FIST && ImaLisice(playerid)) SetPlayerArmedWeapon(playerid, WEAPON_FIST);
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    new cmd[32] = "cmd_", pos = 1, len = 4;
    while (cmdtext[pos] && cmdtext[pos] != ' ')
    {
        if (len >= sizeof cmd - 1)
            return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Komanda ne postoji, /help za spisak svih komandi"));
        cmd[len++] = tolower(cmdtext[pos++]);
    }
    cmd[len] = EOS;
    while (cmdtext[pos] == ' ') pos++;
    if (len > 4 && funcidx(cmd) != -1 && CallLocalFunction(cmd, "is", playerid, cmdtext[pos])) return 1;
    return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Komanda ne postoji, /help za spisak svih komandi"));
}

CMD:help(playerid, params[])
{
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Komande:"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Chat: /me [radnja], /do [opis], /setme [opis lika], /showme [id/ime]"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Govor: /w (sapat), /s ili /shout (vika), /ooc (izvan uloge)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Bez prijevoda: /t (govor), /wt (sapat), /st (vika)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Pretres: /frisk [id/ime]. HQ: Y ili /ulaz"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Animacije: /anim (popis), /anim [broj], /anim 0 ili Space za prekid"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Sluzbe: /duty, /heal i /aparat (vatrogasci i hitna pomoc)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Ozljede: /odustani (nakon 10 s, gubite inventar)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Kurva: Y ili /kurva kod nje, truba iz vozila"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  LSPD: /wanted, /unwanted, /cuff, /uncuff, /drag, /taser, /locker, /stop, /arrest"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Vozilo: /trunk (prtljaznik), /hood (hauba), /zakaci (prikolica). Lider LSPD: /lockerlog"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Prtljaznik: /frisktrunk, /trunkput (LSPD), /trunkout"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Vozilo: /motor (tipka 2), /svjetla, /prozor, /pojas, /zakljucaj, /prometna [id/ime]"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Vozilo: /tankaj (kod pumpe), /ukradi (bez kljuceva). LSPD: /provjeripojas [id/ime]"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Novac i predmeti: /plati [id/ime] [iznos], /inv, /spremi (spremi izvadeno oruzje)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Organizacija: /org (clanovi, skin, pozivi, teritoriji)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Teritoriji: Y ili /diler (kod dilera)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Jezici: /skola (gradska vijecnica), /prijevod, mobitel (Postavke, Jezik)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Trgovine: Y za ulaz i kod blagajne"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Salon automobila: Y kod salona. Vlastito vozilo: /parkiraj (novo mjesto parkiranja)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Opstina: Y kod saltera (kazne, promjena imena, prodaja vozila)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Auto pijaca: Y u svom vozilu kod otkupa (1/3 cijene), Y kod vozila na prodaji (kupnja)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  LSPD: /kazna [id/ime] [iznos] [razlog]"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  LSPD: /mdt (osobe, vozila, potjernice, pracenje na karti)"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Ostalo: /admini"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Statistika: /stats"));
    if (gAdmin[playerid]) SendClientMessage(playerid, COLOR_INFO, T(playerid, "  Admin: /ahelp"));
    return 1;
}

CMD:ahelp(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, 1)) return 1;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Admin komande (razina %d - %s):"), gAdmin[playerid], gRankName[gAdmin[playerid]]);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /a [poruka] - admin chat"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /goto [id/ime] - teleport do igraca"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /jetpack - ukljuci ili iskljuci jetpack"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /afix - popravi vozilo u kojem se nalazis"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /veh [ime vozila] - stvori vozilo"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /wound [id/ime] - odmah rani igraca"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /unjail [id/ime] - pusti iz zatvora, /unwanted [id/ime] - skini potjernicu"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /dozvola [id/ime] - daj ili oduzmi vozacku dozvolu"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /pm [id/ime] [poruka] - privatna poruka igracu"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  /apanel - igraci, vozila, organizacije, teritoriji, administracija"));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "  RCON konzola: setadmin [ime] [razina], nazivranka [razina] [naziv], rankovi, unban [ime]"));
    return 1;
}

CMD:pay(playerid, params[]) return cmd_plati(playerid, params);
CMD:holster(playerid, params[]) return cmd_spremi(playerid, params);
CMD:dealer(playerid, params[]) return cmd_diler(playerid, params);
CMD:school(playerid, params[]) return cmd_skola(playerid, params);
CMD:translate(playerid, params[]) return cmd_prijevod(playerid, params);
CMD:admins(playerid, params[]) return cmd_admini(playerid, params);
CMD:inventory(playerid, params[]) return cmd_inventar(playerid, params);

enum E_ANIMACIJA { aIme[20], aLib[12], aNaziv[20], bool:aPetlja, bool:aDrzi }

static const gAnimacije[][E_ANIMACIJA] = {
    { "Ruke gore", "ROB_BANK", "SHP_HandsUp_Scr", false, true },
    { "Sjedenje", "BEACH", "ParkSit_M_loop", true, false },
    { "Lezanje", "BEACH", "bather", true, false },
    { "Pusenje", "SMOKING", "M_smklean_loop", true, false },
    { "Naslanjanje", "GANGS", "leanIDLE", true, false },
    { "Pijenje", "BAR", "dnk_stndM_loop", true, false },
    { "Ples 1", "DANCING", "DAN_Down_A", true, false },
    { "Ples 2", "DANCING", "DAN_Left_A", true, false },
    { "Ples 3", "DANCING", "DAN_Loop_A", true, false },
    { "Mahanje", "ON_LOOKERS", "wave_loop", true, false },
    { "Klecanje", "CAMERA", "camcrch_idleloop", true, false },
    { "Cucanje", "BOMBER", "BOM_Plant_Loop", true, false },
    { "Razgovor", "PED", "IDLE_CHAT", true, false },
    { "Razmisljanje", "COP_AMBIENT", "Coplook_think", true, false },
    { "Povracanje", "FOOD", "EAT_Vomit_P", false, false }
};

static const gAnimLib[][] = { "ROB_BANK", "BEACH", "SMOKING", "GANGS", "BAR", "DANCING", "ON_LOOKERS", "CAMERA", "BOMBER", "COP_AMBIENT", "FOOD", "SHOP" };
static bool:gAnimAktivna[MAX_PLAYERS];

AnimPreload(playerid)
{
    gAnimAktivna[playerid] = false;
    ApplyAnimation(playerid, "DEALER", "null", 4.1, false, false, false, false, 0);
    ApplyAnimation(playerid, "BLOWJOBZ", "null", 4.1, false, false, false, false, 0);
    ApplyAnimation(playerid, "CRACK", "null", 4.1, false, false, false, false, 0);
    ApplyAnimation(playerid, "PED", "null", 4.1, false, false, false, false, 0);
    ApplyAnimation(playerid, "CAR", "null", 4.1, false, false, false, false, 0);
    ApplyAnimation(playerid, "HAIRCUTS", "null", 4.1, false, false, false, false, 0);
    for (new i = 0; i < sizeof gAnimLib; i++) ApplyAnimation(playerid, gAnimLib[i], "null", 4.1, false, false, false, false, 0);
}

static AnimStop(playerid)
{
    if (!gAnimAktivna[playerid]) return;
    gAnimAktivna[playerid] = false;
    ClearAnimations(playerid);
}

static AnimPusti(playerid, i)
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || ImaLisice(playerid)) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti pjeske."));
    if (i < 0)
    {
        AnimStop(playerid);
        return 1;
    }
    gAnimAktivna[playerid] = true;
    ApplyAnimation(playerid, gAnimacije[i][aLib], gAnimacije[i][aNaziv], 4.1, gAnimacije[i][aPetlja], false, false, gAnimacije[i][aDrzi], 0, SYNC_ALL);
    return 1;
}

AnimKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if ((newkeys & KEY_SPRINT) && !(oldkeys & KEY_SPRINT)) AnimStop(playerid);
}

CMD:anim(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (params[0])
    {
        new n = strval(params);
        if (!ProvjeriBroj(params) || n > sizeof gAnimacije) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Koristenje: /anim [0-%d]"), sizeof gAnimacije);
        return AnimPusti(playerid, n - 1);
    }
    new list[640], row[32], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}Prekini animaciju"));
    for (new i = 0; i < sizeof gAnimacije; i++)
    {
        format(row, sizeof row, "%d. %s", i + 1, T(playerid, gAnimacije[i][aIme]));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_ANIM, DIALOG_STYLE_LIST, T(playerid, "Animacije"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DLG:anim(playerid, response, listitem)
{
    if (response && listitem >= 0 && listitem <= sizeof gAnimacije) AnimPusti(playerid, listitem - 1);
    return 1;
}

CMD:animation(playerid, params[]) return cmd_anim(playerid, params);
