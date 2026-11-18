#define OKLADA_STOLOVA      32
#define OKLADA_MAX          100000
#define OKLADA_POTVRDA_MS   10000

enum { OK_BILIJAR, OK_KOSARKA, OK_PIKADO, OK_KUGLANJE, OK_VRSTA }

static const gOkladaIme[OK_VRSTA][] = { "Bilijar", "Kosarka", "Pikado", "Kuglanje" };
static gOkladaUlog[OK_VRSTA][OKLADA_STOLOVA], gOkladaBanka[OK_VRSTA][OKLADA_STOLOVA];
forward bool:KosPodigni(playerid);

static gOkV[MAX_PLAYERS] = { -1, ... }, gOkS[MAX_PLAYERS], gOkIznos[MAX_PLAYERS], gOkPotvrda[MAX_PLAYERS], gOkPotvrdaOd[MAX_PLAYERS];

OkladaConnect(playerid)
{
    gOkV[playerid] = -1;
    gOkIznos[playerid] = 0;
    gOkPotvrda[playerid] = 0;
}

OkladaBanka(v, s)
{
    return gOkladaBanka[v][s];
}

static OkladaDaj(playerid, iznos)
{
    if (iznos <= 0 || DajNovac(playerid, iznos)) return;
    if (!BankaUplati(gAccountId[playerid], iznos, "Dobitak oklade"))
    {
        BlagajnaDodaj(iznos);
        return;
    }
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Dobitak $%d je uplacen na vas bankovni racun."), iznos);
}

bool:OkladaUdi(playerid, v, s, bool:potvrdi = true)
{
    if (gOkV[playerid] == v && gOkS[playerid] == s) return true;
    new u = gOkladaUlog[v][s], sad = GetTickCount(), kljuc = v * OKLADA_STOLOVA + s + 1;
    if (u && potvrdi && (gOkPotvrda[playerid] != kljuc || sad - gOkPotvrdaOd[playerid] > OKLADA_POTVRDA_MS))
    {
        new tekst[192];
        gOkPotvrda[playerid] = kljuc;
        gOkPotvrdaOd[playerid] = 0;
        format(tekst, sizeof tekst, T(playerid, "Ova igra se igra na okladu.\n\nUlog: {2DD9A9}$%d{FFFFFF}\nPobjednik uzima cijelu banku.\n\nZelite li se pridruziti?"), u);
        PrikaziDialog(playerid, DLG_OKLADA, DIALOG_STYLE_MSGBOX, T(playerid, gOkladaIme[v]), tekst, T(playerid, "Pridruzi se"), T(playerid, "Odustani"));
        return false;
    }
    gOkPotvrda[playerid] = 0;
    if (gOkV[playerid] != -1) OkladaIzadi(playerid, true);
    if (u && !DajNovac(playerid, -u))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulog je $%d, a nemate toliko novca."), u);
        return false;
    }
    gOkV[playerid] = v;
    gOkS[playerid] = s;
    gOkIznos[playerid] = u;
    gOkladaBanka[v][s] += u;
    if (u) SendClientMessage(playerid, 0x2DD9A9FF, T(playerid, "[%s] Ulozili ste $%d. U banci je $%d."), T(playerid, gOkladaIme[v]), u, gOkladaBanka[v][s]);
    return true;
}

DLG:oklada(playerid, response)
{
    new kljuc = gOkPotvrda[playerid];
    if (!response || !kljuc)
    {
        gOkPotvrda[playerid] = 0;
        return 1;
    }
    gOkPotvrdaOd[playerid] = GetTickCount();
    switch ((kljuc - 1) / OKLADA_STOLOVA)
    {
        case OK_BILIJAR: BilijarKey(playerid);
        case OK_KOSARKA: KosPodigni(playerid);
        case OK_PIKADO: PikadoKey(playerid);
        case OK_KUGLANJE: KuglanjeKey(playerid);
    }
    return 1;
}

OkladaIzadi(playerid, bool:vrati)
{
    new v = gOkV[playerid];
    if (v == -1) return;
    new s = gOkS[playerid], u = gOkIznos[playerid];
    gOkV[playerid] = -1;
    gOkIznos[playerid] = 0;
    if (!vrati || !u) return;
    gOkladaBanka[v][s] -= u;
    if (IsPlayerConnected(playerid)) OkladaDaj(playerid, u);
}

OkladaIsplati(v, s, const pobjednici[], n)
{
    new banka = gOkladaBanka[v][s];
    gOkladaBanka[v][s] = 0;
    for (new i = 0; i < MAX_PLAYERS; i++) if (gOkV[i] == v && gOkS[i] == s) gOkV[i] = -1, gOkIznos[i] = 0;
    if (!banka) return;
    if (!n)
    {
        BlagajnaDodaj(banka);
        return;
    }
    new dio = banka / n;
    for (new i = 0; i < n; i++)
    {
        new p = pobjednici[i], d = i ? dio : banka - dio * (n - 1);
        if (!IsPlayerConnected(p)) BlagajnaDodaj(d);
        else
        {
            OkladaDaj(p, d);
            SendClientMessage(p, 0x2DD9A9FF, T(p, "[%s] Osvojili ste $%d!"), T(p, gOkladaIme[v]), d);
        }
    }
}

OkladaZatvori(v, s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gOkV[i] == v && gOkS[i] == s) OkladaIzadi(i, true);
    if (gOkladaBanka[v][s] > 0) BlagajnaDodaj(gOkladaBanka[v][s]);
    gOkladaBanka[v][s] = 0;
    gOkladaUlog[v][s] = 0;
}

OkladaVratiSve()
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gOkV[i] != -1) OkladaIzadi(i, true);
}

static OkladaGdje(playerid, &v, &s)
{
    new r = BilijarOkladaStol(playerid);
    v = OK_BILIJAR;
    if (r == -1)
    {
        r = KosOkladaTeren(playerid);
        v = OK_KOSARKA;
    }
    if (r == -1)
    {
        r = PikadoOkladaStol(playerid);
        v = OK_PIKADO;
    }
    if (r == -1)
    {
        r = KuglanjeOkladaStaza(playerid);
        v = OK_KUGLANJE;
    }
    if (r == -1) return -1;
    s = r < 0 ? -r - 2 : r;
    return r >= 0;
}

CMD:oklada(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new v, s, moze = OkladaGdje(playerid, v, s);
    if (moze == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste za bilijarom, pikadom, kuglanom ni na kosarkaskom terenu."));
    if (!params[0])
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "[%s] Ulog: $%d, u banci: $%d. Promjena: /oklada [iznos] (dok ste sami)."), T(playerid, gOkladaIme[v]), gOkladaUlog[v][s], gOkladaBanka[v][s]);
        return 1;
    }
    if (!ProvjeriBroj(params) || strlen(params) > 6 || strval(params) > OKLADA_MAX) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /oklada [0-%d]"), OKLADA_MAX);
    if (!moze) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulog se moze promijeniti samo dok ste sami, prije pocetka igre."));
    new stari = gOkladaUlog[v][s];
    OkladaIzadi(playerid, true);
    gOkladaUlog[v][s] = strval(params);
    if (!OkladaUdi(playerid, v, s, false))
    {
        gOkladaUlog[v][s] = stari;
        OkladaUdi(playerid, v, s, false);
        return 1;
    }
    if (!gOkladaUlog[v][s]) SendClientMessage(playerid, COLOR_INFO, T(playerid, "[%s] Igra se bez uloga."), T(playerid, gOkladaIme[v]));
    return 1;
}

CMD:bet(playerid, params[]) return cmd_oklada(playerid, params);
