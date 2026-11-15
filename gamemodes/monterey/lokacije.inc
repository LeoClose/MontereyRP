#define LOK_STRANA          40
#define LOK_PRETHODNA       -2
#define LOK_SLJEDECA        -3

static const gLokKat[][] = { "Organizacije", "Kazino", "Banke", "Zlatare", "Oruzarnice", "Trgovine", "Odjeca", "Namjestaj", "Saloni", "Tuning garaze", "Opstina", "Kuce", "Garaze" };
static gLokOdabrana[MAX_PLAYERS], gLokStrana[MAX_PLAYERS], gLokRed[MAX_PLAYERS][LOK_STRANA + 2], gLokBroj[MAX_PLAYERS];

static LokacijaUzmi(kat, i, &Float:x, &Float:y, &Float:z, &Float:a)
{
    switch (kat)
    {
        case 0: return OrgLokacija(i, x, y, z, a);
        case 1: return KazinoLokacija(i, x, y, z, a);
        case 2: return BankaLokacija(i, x, y, z, a);
        case 3: return ZlataraLokacija(i, x, y, z, a);
        case 4: return OruzarnicaLokacija(i, x, y, z, a);
        case 5: return TrgovinaLokacija(i, x, y, z, a);
        case 6: return OdjecaLokacija(i, x, y, z, a);
        case 7: return NamjestajLokacija(i, x, y, z, a);
        case 8: return SalonLokacija(i, x, y, z, a);
        case 9: return TuningLokacija(i, x, y, z, a);
        case 10: return OpstinaLokacija(i, x, y, z, a);
        case 11: return KucaLokacija(i, x, y, z, a);
        case 12: return GarazaLokacija(i, x, y, z, a);
    }
    return -1;
}

static LokacijaIme(playerid, kat, i, r, ime[], size = sizeof (ime))
{
    if (kat == 0 && GetOrgIme(r, ime, size)) return;
    format(ime, size, "%s #%d", T(playerid, gLokKat[kat]), i + 1);
}

static LokacijeKat(playerid)
{
    new list[512], len = 0;
    for (new k = 0; k < sizeof gLokKat; k++) DodajStupac(list, len, T(playerid, gLokKat[k]));
    return PrikaziDialog(playerid, DLG_LOKACIJE, DIALOG_STYLE_LIST, T(playerid, "Lokacije"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static LokacijeLista(playerid)
{
    new kat = gLokOdabrana[playerid], list[2048], len = 0, red = 0, preskoci = gLokStrana[playerid] * LOK_STRANA, ime[48], Float:x, Float:y, Float:z, Float:a, r;
    if (gLokStrana[playerid])
    {
        DodajStupac(list, len, T(playerid, "<< Prethodna strana"));
        gLokRed[playerid][red++] = LOK_PRETHODNA;
    }
    new bazni = red;
    for (new i = 0; (r = LokacijaUzmi(kat, i, x, y, z, a)) != -1; i++)
    {
        if (!r || preskoci-- > 0) continue;
        if (red - bazni == LOK_STRANA)
        {
            DodajStupac(list, len, T(playerid, "Sljedeca strana >>"));
            gLokRed[playerid][red++] = LOK_SLJEDECA;
            break;
        }
        LokacijaIme(playerid, kat, i, r, ime);
        DodajStupac(list, len, ime);
        gLokRed[playerid][red++] = i;
    }
    gLokBroj[playerid] = red;
    if (!red)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema lokacija u ovoj kategoriji."));
        return LokacijeKat(playerid);
    }
    return PrikaziDialog(playerid, DLG_LOKACIJE_LISTA, DIALOG_STYLE_LIST, T(playerid, gLokKat[kat]), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

CMD:lokacije(playerid, params[])
{
    if (!ProvjeriOvlasti(playerid, ALVL_KICK)) return 1;
    return LokacijeKat(playerid);
}

DLG:lokacije(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= sizeof gLokKat) return 1;
    gLokOdabrana[playerid] = listitem;
    gLokStrana[playerid] = 0;
    return LokacijeLista(playerid);
}

DLG:lokacije_lista(playerid, response, listitem)
{
    if (!response) return LokacijeKat(playerid);
    if (listitem < 0 || listitem >= gLokBroj[playerid]) return 1;
    new i = gLokRed[playerid][listitem], Float:x, Float:y, Float:z, Float:a, ime[48];
    if (i == LOK_PRETHODNA || i == LOK_SLJEDECA)
    {
        gLokStrana[playerid] += i == LOK_SLJEDECA ? 1 : -1;
        return LokacijeLista(playerid);
    }
    new r = LokacijaUzmi(gLokOdabrana[playerid], i, x, y, z, a);
    if (r <= 0) return LokacijeLista(playerid);
    if (IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Stisni Enter da izadjes iz vozila."));
    PrebaciIgraca(playerid, 0, 0, x + 1.0, y, z + 0.5, a);
    LokacijaIme(playerid, gLokOdabrana[playerid], i, r, ime);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Teleportirani ste do lokacije: %s."), ime);
    return 1;
}
