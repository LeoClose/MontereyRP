#define UPIT_DOMET          4.0
#define UPIT_BOJA           0xF39C12FF

enum { UPIT_NEMA, UPIT_DILER, UPIT_DOB, UPIT_PREPRODAVAC, UPIT_PIJACA, UPIT_TAKSI, UPIT_RANJEN, UPIT_RIBAR, UPIT_RESTORAN, UPIT_SKLADISTE, UPIT_REPO, UPIT_PROVALA, UPIT_PRIJEVOZ, UPIT_TORANJ }

static gUpit[MAX_PLAYERS], gUpitMeta[MAX_PLAYERS], gUpitOpcija[MAX_PLAYERS], Float:gUpitPos[MAX_PLAYERS][3];

static UpitPostavi(playerid, tip, meta, opcija)
{
    gUpit[playerid] = tip;
    gUpitMeta[playerid] = meta;
    gUpitOpcija[playerid] = opcija;
    GetPlayerPos(playerid, gUpitPos[playerid][0], gUpitPos[playerid][1], gUpitPos[playerid][2]);
}

UpitKolicina(playerid, tip, meta, opcija, const naslov[], const info[])
{
    new tekst[256];
    format(tekst, sizeof tekst, "%s~n~%s", info, T(playerid, "Upisite kolicinu u chat ili N za odbijanje."));
    UpitPostavi(playerid, tip, meta, opcija);
    return IgraObavijest(playerid, naslov, UPIT_BOJA, tekst, 0);
}

UpitDaNe(playerid, tip, meta, opcija, const naslov[], const info[])
{
    new tekst[256];
    format(tekst, sizeof tekst, "%s~n~%s", info, T(playerid, "Y - da, N - ne"));
    UpitPostavi(playerid, tip, meta, opcija);
    return IgraObavijest(playerid, naslov, UPIT_BOJA, tekst, 0);
}

UpitPrekini(playerid)
{
    if (!gUpit[playerid]) return;
    gUpit[playerid] = UPIT_NEMA;
    IgraObavijestUkloni(playerid);
}

UpitMakni(playerid, tip)
{
    if (gUpit[playerid] == tip) UpitPrekini(playerid);
}

UpitChat(playerid, const text[])
{
    new tip = gUpit[playerid];
    if ((tip != UPIT_DILER && tip != UPIT_DOB && tip != UPIT_PREPRODAVAC) || !ProvjeriBroj(text) || strlen(text) > 5) return 0;
    new meta = gUpitMeta[playerid], opcija = gUpitOpcija[playerid], n = strval(text);
    UpitPrekini(playerid);
    switch (tip)
    {
        case UPIT_DILER: DilerUpit(playerid, meta, n);
        case UPIT_DOB: DobUpit(playerid, meta, opcija, n);
        case UPIT_PREPRODAVAC: PreprodavacUpit(playerid, meta, n);
    }
    return 1;
}

UpitKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    new tip = gUpit[playerid];
    if (tip == UPIT_NEMA) return 0;
    new bool:da = (newkeys & KEY_YES) && !(oldkeys & KEY_YES), bool:ne = (newkeys & KEY_NO) && !(oldkeys & KEY_NO);
    if (!ne && !(da && (tip == UPIT_PIJACA || tip == UPIT_TAKSI || tip == UPIT_RANJEN || tip == UPIT_RIBAR || tip == UPIT_RESTORAN || tip == UPIT_SKLADISTE || tip == UPIT_REPO || tip == UPIT_PROVALA || tip == UPIT_PRIJEVOZ || tip == UPIT_TORANJ))) return 0;
    new meta = gUpitMeta[playerid], opcija = gUpitOpcija[playerid];
    UpitPrekini(playerid);
    switch (tip)
    {
        case UPIT_PIJACA: PijacaUpit(playerid, meta, opcija, da);
        case UPIT_TAKSI: TaksiUpit(playerid, meta, da);
        case UPIT_RANJEN: RanjenUpit(playerid, da);
        case UPIT_RIBAR: RibarUpit(playerid, opcija, da);
        case UPIT_RESTORAN: RestoranUpit(playerid, meta, opcija, da);
        case UPIT_SKLADISTE: SkladisteUpit(playerid, opcija, da);
        case UPIT_REPO: RepoUpit(playerid, opcija, da);
        case UPIT_PROVALA: ProvalaUpit(playerid, meta, da);
        case UPIT_PRIJEVOZ: PrijevozUpit(playerid, meta, opcija, da);
        case UPIT_TORANJ: AtcUpit(playerid, opcija, da);
        default: Govor(playerid, "%s kaze: Ne hvala.", GetRPIme(playerid));
    }
    return 1;
}

UpitTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUpit[i] && !IsPlayerInRangeOfPoint(i, UPIT_DOMET, gUpitPos[i][0], gUpitPos[i][1], gUpitPos[i][2])) UpitPrekini(i);
}

UpitDisconnect(playerid)
{
    gUpit[playerid] = UPIT_NEMA;
}
