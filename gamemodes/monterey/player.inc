#define SMG_VJESTINA        200
#define ORUZJE_PROVJERA_MS  250

static gOruzjeProvjera[MAX_PLAYERS], bool:gOruzjeBlok[MAX_PLAYERS];

public OnPlayerConnect(playerid)
{
    if (playerid > gIgracZadnji) gIgracZadnji = playerid;
    PrenosConnect(playerid);
    KosConnect(playerid);
    IkoneIgracu(playerid);
    MapaOpstinaUkloni(playerid);
    MapaIdlewoodUkloni(playerid);
    MapaIdlewoodFlatsUkloni(playerid);
    WilshirePdUkloni(playerid);
    MapaBusTaxiUkloni(playerid);
    MapaCartierExtUkloni(playerid);
    MapaInternetKaficExtUkloni(playerid);
    MapaCentralBankUkloni(playerid);
    MapaCasinoExtUkloni(playerid);
    MapaMainStreetUkloni(playerid);
    MapaChinatownUkloni(playerid);
    MapaItalijaniUkloni(playerid);
    MapaSurenosiUkloni(playerid);
    MapaRuskaMafijaUkloni(playerid);
    MapaGettelSalonUkloni(playerid);
    MapaJeffersonFlatsUkloni(playerid);
    MapaSpawnUkloni(playerid);
    MapaMontereyPeakUkloni(playerid);
    MapaGlenParkUkloni(playerid);
    BrodUkloni(playerid);
    PlakatiUkloni(playerid);
    if (IsPlayerNPC(playerid)) return 1;
    new ljudi = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerConnected(i) && !IsPlayerNPC(i)) ljudi++;
    if (ljudi > MAX_LJUDI)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Server je pun."));
        return KickTimer(playerid);
    }
    StetaConnect(playerid);
    PijacaConnect(playerid);
    gAccountId[playerid] = 0;
    gUlogiran[playerid] = false;
    DialogConnect(playerid);
    AdminConnect(playerid);
    JezikConnect(playerid);
    OrgConnect(playerid);
    StatsConnect(playerid);
    NovacConnect(playerid);
    OkladaConnect(playerid);
    PikadoConnect(playerid);
    AntiCheatConnect(playerid);
    SyncConnect(playerid);
    DrogaConnect(playerid);
    KuglanjeConnect(playerid);
    IgraObavijestConnect(playerid);
    SavjetConnect(playerid);
    PokerConnect(playerid);
    InventarConnect(playerid);
    OruzjeConnect(playerid);
    TeritorijConnect(playerid);
    if (ProvjeriBan(playerid) || LoginBlokiran(playerid)) return 1;

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
    PovratakSpremi(playerid);
    if (playerid == gIgracZadnji) do gIgracZadnji--; while (gIgracZadnji >= 0 && !IsPlayerConnected(gIgracZadnji));
    SpecDisconnect(playerid);
    SlucajDisconnect(playerid);
    gPrenosZamrz[playerid] = 0;
    if (IsPlayerNPC(playerid)) return 1;
    BankaOnline(playerid, false);
    TaksiPlayerLeft(playerid);
    KurvaDisconnect(playerid);
    MaricaReset(playerid);
    PolicijaDisconnect(playerid, true);
    PljVrecaOdlazak(playerid);
    DrogaUcinakOdlazi(playerid);
    GepekDisconnect(playerid);
    VoziloDisconnect(playerid);
    SalonDisconnect(playerid);
    OruzarnicaDisconnect(playerid);
    TrgovinaDisconnect(playerid);
    NamjestajDisconnect(playerid);
    MdtDisconnect(playerid);
    OdjecaDisconnect(playerid);
    LoginDisconnect(playerid);
    TuningDisconnect(playerid);
    NovacDisconnect(playerid);
    UpitDisconnect(playerid);
    UtrkaDisconnect(playerid);
    FirmaDisconnect(playerid);
    StandDisconnect(playerid);
    FpsDisconnect(playerid);
    KaznaDisconnect(playerid);
    DostavaDisconnect(playerid);
    RibarKraj(playerid);
    SkladisteKraj(playerid);
    RepoKraj(playerid);
    ProvalaKraj(playerid);
    RestoranDisconnect(playerid);
    KaficDisconnect(playerid);
    BilijarIgracOdlazi(playerid);
    KosOdlazi(playerid);
    PikadoOdlazi(playerid);
    DrogaOdlazi(playerid);
    KuglanjeOdlazi(playerid);
    PokerOdlazi(playerid);
    ArkZatvori(playerid);
    KazinoDisconnect(playerid);
    PcDisconnect(playerid);
    ZlataraDisconnect(playerid);
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
    ShowHud(playerid);
    LoginSpawn(playerid);
    StetaSpawn(playerid);
    AntiCheatSpawn(playerid);
    SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, SMG_VJESTINA);
    SetPlayerSkillLevel(playerid, WEAPONSKILL_MP5, SMG_VJESTINA);
    SetPlayerSkin(playerid, OdjecaSkinIgraca(playerid));
    PrebaciIgraca(playerid, 0, 0, SPAWN_X, SPAWN_Y, SPAWN_Z, SPAWN_A);
    AnimPreload(playerid);
    PolicijaSpawn(playerid);
    GepekSpawn(playerid);
    KucaSpawn(playerid);
    PovratakSpawn(playerid);
    KaznaSpawn(playerid);
    RanjenSpawn(playerid);
    OdjecaSpawn(playerid);
    ZlataraSpawn(playerid);
    VrijemeIgrac(playerid);
    RanjenBolnica(playerid);
    SpecSpawn(playerid);
    FpsSpawn(playerid);
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
    RibarKraj(playerid);
    SkladisteKraj(playerid);
    RepoKraj(playerid);
    ProvalaKraj(playerid);
    RestoranSmjenaKraj(playerid);
    return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    KaznaStanje(playerid, newstate);
    TaksiStateChange(playerid, newstate, oldstate);
    PolicijaStateChange(playerid, newstate);
    MdtStateChange(playerid, oldstate);
    TuningStateChange(playerid, oldstate);
    MaricaEnter(playerid, newstate);
    SlucajStateChange(playerid, newstate);
    VoziloStateChange(playerid, newstate);
    RibarStateChange(playerid, newstate, oldstate);
    SkladisteStateChange(playerid, newstate);
    RepoStateChange(playerid, newstate);
    VlakStateChange(playerid, newstate);
    GarazaStateChange(playerid, newstate, oldstate);
    if (newstate == PLAYER_STATE_DRIVER) VoziloVozac(playerid);
    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (KaznaKey(playerid, newkeys, oldkeys)) return 1;
    if (KurvaOdgovorKey(playerid, newkeys, oldkeys)) return 1;
    if (UpitKey(playerid, newkeys, oldkeys)) return 1;
    if (ArkadaTipke(playerid, newkeys, oldkeys) || DrogaTipke(playerid, newkeys, oldkeys) || TuningKey(playerid, newkeys, oldkeys) || MobitelKey(playerid, newkeys, oldkeys) || MdtKey(playerid, newkeys, oldkeys)) return 1;
    if (RestoranKey(playerid, newkeys, oldkeys) || RibarKey(playerid, newkeys, oldkeys) || RepoKey(playerid, newkeys, oldkeys) || PosaoIgraKey(playerid, newkeys, oldkeys)) return 1;
    if (KradjaKey(playerid, newkeys, oldkeys)) return 1;
    if (KapijaKey(playerid, newkeys, oldkeys)) return 1;
    if (UtrkaKey(playerid, newkeys, oldkeys)) return 1;
    TaksiKey(playerid, newkeys, oldkeys);
    MaricaKey(playerid, newkeys, oldkeys);
    AnimKey(playerid, newkeys, oldkeys);
    VoziloKey(playerid, newkeys, oldkeys);
    KurvaPotrubiKey(playerid, newkeys, oldkeys);
    PolicijaKey(playerid, newkeys, oldkeys);
    PrikolicaAlt(playerid, newkeys, oldkeys);
    TuningNitroKey(playerid, newkeys, oldkeys);
    if (BilijarTipke(playerid, newkeys, oldkeys) || KosTipke(playerid, newkeys, oldkeys) || PikadoTipke(playerid, newkeys, oldkeys) || KuglanjeTipke(playerid, newkeys, oldkeys)) return 1;
    if ((newkeys & KEY_SECONDARY_ATTACK) && !(oldkeys & KEY_SECONDARY_ATTACK)) InterakcijaF(playerid);
    if ((newkeys & KEY_ACTION) && !(oldkeys & KEY_ACTION) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) PijacaKey(playerid);
    if ((newkeys & KEY_YES) && !(oldkeys & KEY_YES)) Interakcija(playerid);
    if ((newkeys & KEY_WALK) && !(oldkeys & KEY_WALK) && !IsRanjen(playerid) && !KosKey(playerid) && !BacenoPodigni(playerid) && !InterakcijaAlt(playerid)) KosPodigni(playerid);
    return 1;
}

// Interakcija je na ALT
static InterakcijaF(playerid)
{
    if (!gUlogiran[playerid] || PrikolicaUnutra(playerid)) return;
    if (GarazaKey(playerid)) return;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return;
    if (ProvalaF(playerid)) return;
    if (HqUlazIzlaz(playerid)) return;
    KucaKey(playerid);
}

static InterakcijaAlt(playerid)
{
    if (PikadoCilja(playerid) || KuglanjeCilja(playerid) || BilijarCilja(playerid)) return 1;
    return RestoranAlt(playerid) || RacunaloKey(playerid) || StandMeni(playerid) || KurvaUlica(playerid) || TrgovinaKasa(playerid) || OruzarnicaKasa(playerid) || NamjestajKasa(playerid) || OdjecaKasa(playerid) || OpstinaSalter(playerid) || DilerKey(playerid) || KazinoKey(playerid) || BankaKasa(playerid) || ZlataraKasa(playerid) || KaficKasa(playerid) || BilijarKey(playerid) || PikadoKey(playerid) || KuglanjeKey(playerid) || PokerKey(playerid) || ArkadaKey(playerid) || PreprodavacKey(playerid) || DrogaKey(playerid) || KampKey(playerid) || SalonUlaz(playerid) || PijacaKey(playerid) || ProvalaAlt(playerid) || GepekAlt(playerid) || BankaPljackaAlt(playerid) || VrecaAlt(playerid) || ZlataraAlt(playerid) || LabAlt(playerid) || RibarAlt(playerid) || SkladisteAlt(playerid) || RepoAlt(playerid);
}

static Interakcija(playerid)
{
    if (TuningUlaz(playerid)) return;
    if (!gUlogiran[playerid] || IsRanjen(playerid) || MobitelOtvoren(playerid) || MdtOtvoren(playerid)) return;
    if (IsPlayerInAnyVehicle(playerid)) VoziloMeni(playerid);
    else if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && !VoziloMeni(playerid)) PrikaziInventar(playerid);
}

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (TaserPogodak(playerid, weaponid, hittype, hitid)) return 0;
    if (!OruzjePucanj(playerid, weaponid)) return 0;
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
    AfkUpdate(playerid);
    SpecUpdate(playerid);
    IkoneUpdate(playerid);
    RanjenUpdate(playerid);
    MobitelUpdate(playerid);
    VoziloUpdate(playerid);
    TuningUpdate(playerid);
    TrgovinaUpdate(playerid);
    BankaUpdate(playerid);
    ZlataraUpdate(playerid);
    TaksiPovratak(playerid);
    new WEAPON:w = GetPlayerWeapon(playerid);
    if (w != WEAPON_FIST && w != WEAPON:-1 && gUlogiran[playerid])
    {
        new sad = GetTickCount(), bool:puska = w >= WEAPON_COLT45 && w <= WEAPON_MINIGUN;
        if (sad - gOruzjeProvjera[playerid] >= ORUZJE_PROVJERA_MS)
        {
            gOruzjeProvjera[playerid] = sad;
            if (puska) gOruzjeBlok[playerid] = IsShootingBlocked(playerid);
            OruzjeNadzor(playerid, w);
        }
        if (puska && gOruzjeBlok[playerid]) SetPlayerArmedWeapon(playerid, WEAPON_FIST);
    }
    if (w != WEAPON_FIST && ImaLisice(playerid)) SetPlayerArmedWeapon(playerid, WEAPON_FIST);
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    AfkAktivan(playerid);
    if (KaznaBlokira(playerid)) return 1;
    new cmd[32] = "cmd_", pos = 1, len = 4;
    while (cmdtext[pos] && cmdtext[pos] != ' ')
    {
        if (len >= sizeof cmd - 1)
            return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Komanda ne postoji, /help za spisak svih komandi"));
        cmd[len++] = tolower(cmdtext[pos++]);
    }
    cmd[len] = EOS;
    while (cmdtext[pos] == ' ') pos++;
    if (ArkadaIgra(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete koristiti komande dok igrate na automatu (ENTER - izlaz)."));
    if (len > 4 && funcidx(cmd) != -1 && CallLocalFunction(cmd, "is", playerid, cmdtext[pos])) return 1;
    return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Komanda ne postoji, /help za spisak svih komandi"));
}

static PomocPisi(out[], &len, const dio[])
{
    for (new i = 0; dio[i] && len < 4094; i++) out[len++] = dio[i];
    out[len] = EOS;
}

static PomocRed(out[], &len, const red[])
{
    new n = strlen(red), i = 0, dv = strfind(red, ":"), kom = strfind(red, "/"), bool:komanda = false;
    while (i < n && red[i] == ' ') i++;
    if (dv != -1 && (kom == -1 || dv < kom))
    {
        PomocPisi(out, len, "{8AB4F8}");
        for (; i <= dv && len < 4094; i++) out[len++] = red[i];
        PomocPisi(out, len, "{FFFFFF}");
    }
    for (; i < n && len < 4080; i++)
    {
        new c = red[i];
        if (c == '/' && (i == 0 || red[i - 1] == ' ' || red[i - 1] == '(' || red[i - 1] == ','))
        {
            PomocPisi(out, len, "{FFD54F}");
            komanda = true;
        }
        else if (komanda && (c == ' ' || c == ',' || c == ')'))
        {
            PomocPisi(out, len, "{FFFFFF}");
            komanda = false;
        }
        out[len++] = c;
    }
    out[len] = EOS;
    if (komanda) PomocPisi(out, len, "{FFFFFF}");
    PomocPisi(out, len, "\n");
}

CMD:help(playerid, params[])
{
    new tekst[4096], len = 0;
    PomocRed(tekst, len, T(playerid, "  Chat: /me [radnja], /do [opis], /setme [opis lika], /showme [id/ime]"));
    PomocRed(tekst, len, T(playerid, "  Govor: /w (sapat), /s ili /shout (vika), /ooc (izvan uloge)"));
    PomocRed(tekst, len, T(playerid, "  Bez prijevoda: /t (govor), /wt (sapat), /st (vika)"));
    PomocRed(tekst, len, T(playerid, "  Pretres: /frisk [id/ime]. HQ: F ili /ulaz"));
    PomocRed(tekst, len, T(playerid, "  Animacije: /anim (popis), /anim [broj], /anim 0 ili Space za prekid"));
    PomocRed(tekst, len, T(playerid, "  Sluzbe: /duty, /heal i /aparat (vatrogasci i hitna pomoc)"));
    PomocRed(tekst, len, T(playerid, "  Ozljede: /odustani (nakon 10 s, gubite inventar)"));
    PomocRed(tekst, len, T(playerid, "  Kurva: ALT ili /kurva kod nje, truba iz vozila"));
    PomocRed(tekst, len, T(playerid, "  LSPD: /wanted, /unwanted, /cuff, /uncuff, /drag, /detain, /dragout, /taser, /locker, /stop, /arrest"));
    PomocRed(tekst, len, T(playerid, "  Vozilo: /trunk (prtljaznik), /hood (hauba), /zakaci (prikolica). Lider LSPD: /lockerlog"));
    PomocRed(tekst, len, T(playerid, "  Prtljaznik: /frisktrunk, /trunkput (LSPD), /trunkout"));
    PomocRed(tekst, len, T(playerid, "  Vozilo: /motor (tipka 2), /svjetla, /prozor, /pojas, /zakljucaj, /prometna [id/ime]"));
    PomocRed(tekst, len, T(playerid, "  Vozilo: /tankaj (kod pumpe), /ukradi (bez kljuceva). LSPD: /provjeripojas [id/ime]"));
    PomocRed(tekst, len, T(playerid, "  Novac i predmeti: /plati [id/ime] [iznos], /inv, /spremi (spremi izvadeno oruzje)"));
    PomocRed(tekst, len, T(playerid, "  Organizacija: /org (clanovi, skin, pozivi, teritoriji)"));
    PomocRed(tekst, len, T(playerid, "  Teritoriji: ALT ili /diler (kod dilera)"));
    PomocRed(tekst, len, T(playerid, "  Jezici: /skola (gradska vijecnica), /prijevod, mobitel (Postavke, Jezik)"));
    PomocRed(tekst, len, T(playerid, "  Trgovine: F za ulaz, ALT kod blagajne"));
    PomocRed(tekst, len, T(playerid, "  Salon automobila: ALT kod salona. Vlastito vozilo: /parkiraj (novo mjesto parkiranja)"));
    PomocRed(tekst, len, T(playerid, "  Opstina: ALT kod saltera (kazne, promjena imena, prodaja vozila)"));
    PomocRed(tekst, len, T(playerid, "  Auto pijaca: ALT u svom vozilu kod otkupa (1/3 cijene), ALT kod vozila na prodaji (kupnja)"));
    PomocRed(tekst, len, T(playerid, "  LSPD: /kazna [id/ime] [iznos] [razlog]"));
    PomocRed(tekst, len, T(playerid, "  LSPD: /mdt (osobe, vozila, potjernice, pracenje na karti)"));
    PomocRed(tekst, len, T(playerid, "  Ostalo: /admini"));
    PomocRed(tekst, len, T(playerid, "  Kuce: /kuca (vlasnik, adresa, cijena i najam najblize kuce)"));
    PomocRed(tekst, len, T(playerid, "  Firme: /firma (unutar trgovine 24/7 ili frizera: kupnja, zarada, rad)"));
    PomocRed(tekst, len, T(playerid, "  Statistika: /stats"));
    PomocRed(tekst, len, T(playerid, "  Kamera: /fps (prvo lice)"));
    if (gAdmin[playerid]) PomocRed(tekst, len, T(playerid, "  Admin: /ahelp"));
    return PrikaziDialog(playerid, DLG_NONE, DIALOG_STYLE_MSGBOX, T(playerid, "Komande:"), tekst, T(playerid, "Zatvori"));
}

CMD:ahelp(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, 1)) return 1;
    new tekst[4096], len = 0, naslov[96];
    format(naslov, sizeof naslov, T(playerid, "Admin komande (razina %d - %s):"), gAdmin[playerid], gRankName[gAdmin[playerid]]);
    PomocRed(tekst, len, T(playerid, "  /a [poruka] - admin chat"));
    PomocRed(tekst, len, T(playerid, "  /goto [id/ime] - teleport do igraca"));
    PomocRed(tekst, len, T(playerid, "  /gotomark - teleport do oznake na karti"));
    PomocRed(tekst, len, T(playerid, "  /jetpack - ukljuci ili iskljuci jetpack"));
    PomocRed(tekst, len, T(playerid, "  /afix - popravi vozilo u kojem se nalazis"));
    PomocRed(tekst, len, T(playerid, "  /veh [ime vozila] - stvori vozilo"));
    PomocRed(tekst, len, T(playerid, "  /wound [id/ime] - odmah rani igraca"));
    PomocRed(tekst, len, T(playerid, "  /unjail [id/ime] - pusti iz zatvora, /unwanted [id/ime] - skini potjernicu"));
    PomocRed(tekst, len, T(playerid, "  /dozvola [id/ime] - daj ili oduzmi vozacku dozvolu"));
    PomocRed(tekst, len, T(playerid, "  /pm [id/ime] [poruka] - privatna poruka igracu"));
    PomocRed(tekst, len, T(playerid, "  /apanel - igraci, vozila, organizacije, teritoriji, administracija"));
    PomocRed(tekst, len, T(playerid, "  /freeze [id/ime], /unfreeze [id/ime] - zamrzni ili odmrzni igraca"));
    PomocRed(tekst, len, T(playerid, "  Banke: /pljacka (postavke pljacke). Kapije: /kapija, /kapijaorg, /kapijabrisi"));
    if (gAdmin[playerid] >= ALVL_MANAGE)
    {
        PomocRed(tekst, len, T(playerid, "  /time [0-23] - sat, /weather [0-22] - vrijeme na serveru"));
    }
    PomocRed(tekst, len, T(playerid, "  /kazni [id/ime] [1 plivanje / 2 metenje] [kolicina] [razlog], /oslobodi [id/ime]"));
    PomocRed(tekst, len, T(playerid, "  /kick, /ban, /unban, /dajnovac, /dajpredmet, /dajoruzje, /obrisivozilo, /premium, /postavibroj"));
    PomocRed(tekst, len, T(playerid, "  RCON konzola: setadmin [ime] [razina], nazivranka [razina] [naziv], rankovi, unban [ime]"));
    return PrikaziDialog(playerid, DLG_NONE, DIALOG_STYLE_MSGBOX, naslov, tekst, T(playerid, "Zatvori"));
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

static const gAnimLib[][] = { "ROB_BANK", "BEACH", "SMOKING", "GANGS", "BAR", "DANCING", "ON_LOOKERS", "CAMERA", "BOMBER", "COP_AMBIENT", "FOOD", "SHOP", "BSKTBALL", "BD_FIRE", "GRENADE", "KNIFE", "POOL" };
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
