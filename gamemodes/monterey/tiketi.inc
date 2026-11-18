#define TIKET_MAX           32
#define TIKET_PAUZA_S       30
#define TIKET_LEN           128

enum { TIKET_PITANJE, TIKET_PRIJAVA }
enum { TP_ADMINU = 1, TP_IGRACU }
enum E_TIKET { bool:tkIma, tkTip, tkIgrac, tkRacun, tkCilj, tkTekst[TIKET_LEN], tkVrijeme, tkBroj }
static gTiket[TIKET_MAX][E_TIKET], gTiketBroj, gTiketZadnji[MAX_PLAYERS], gTiketOdabran[MAX_PLAYERS] = { -1, ... }, gTiketPopis[MAX_PLAYERS][TIKET_MAX];
static gTiketOdgovor[MAX_PLAYERS][TIKET_LEN], gTiketPrima[MAX_PLAYERS], gTiketPrimaRacun[MAX_PLAYERS];

static TiketNovi(playerid, tip, cilj, const tekst[])
{
    new sad = gettime();
    if (sad - gTiketZadnji[playerid] < TIKET_PAUZA_S) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pricekajte %d s prije novog upita."), TIKET_PAUZA_S - (sad - gTiketZadnji[playerid]));
    new t = 0;
    while (t < TIKET_MAX && gTiket[t][tkIma]) t++;
    if (t == TIKET_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Previse otvorenih upita, pokusajte kasnije."));
    gTiketZadnji[playerid] = sad;
    gTiket[t][tkIma] = true;
    gTiket[t][tkTip] = tip;
    gTiket[t][tkIgrac] = playerid;
    gTiket[t][tkRacun] = gAccountId[playerid];
    gTiket[t][tkCilj] = cilj;
    gTiket[t][tkVrijeme] = sad;
    gTiket[t][tkBroj] = ++gTiketBroj;
    gTiket[t][tkTekst][0] = EOS;
    strcat(gTiket[t][tkTekst], tekst, TIKET_LEN);
    RemoveBoje(gTiket[t][tkTekst]);
    new poruka[200];
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i] || !gAdmin[i]) continue;
        if (tip == TIKET_PRIJAVA) format(poruka, sizeof poruka, T(i, "[Prijava #%d] %s (%d) prijavljuje %s (%d): %s"), gTiket[t][tkBroj], GetRPIme(playerid), playerid, GetRPIme(cilj), cilj, gTiket[t][tkTekst]);
        else format(poruka, sizeof poruka, T(i, "[Pitanje #%d] %s (%d): %s"), gTiket[t][tkBroj], GetRPIme(playerid), playerid, gTiket[t][tkTekst]);
        SendLong(i, COLOR_ADMIN, poruka);
        SendClientMessage(i, COLOR_ADMIN, T(i, "Odgovorite s /tiketi."));
    }
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vas upit je poslan adminima (#%d)."), gTiket[t][tkBroj]);
}

static bool:TiketValjan(t)
{
    if (t < 0 || t >= TIKET_MAX || !gTiket[t][tkIma]) return false;
    new p = gTiket[t][tkIgrac];
    if (!IsPlayerConnected(p) || gAccountId[p] != gTiket[t][tkRacun])
    {
        gTiket[t][tkIma] = false;
        return false;
    }
    return true;
}

CMD:pitaj(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /pitaj [pitanje]"));
    return TiketNovi(playerid, TIKET_PITANJE, INVALID_PLAYER_ID, params);
}

CMD:report(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new idx = 0, p1[32];
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /report [id/ime] [razlog]"));
    while (params[idx] == ' ') idx++;
    if (!params[idx]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /report [id/ime] [razlog]"));
    new cilj = FindPlayer(p1);
    if (cilj == INVALID_PLAYER_ID || !gUlogiran[cilj]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (cilj == playerid) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete to sami sebi."));
    return TiketNovi(playerid, TIKET_PRIJAVA, cilj, params[idx]);
}

CMD:tiketi(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    new list[2048], red[160], len = 0, n = 0, sad = gettime();
    for (new t = 0; t < TIKET_MAX; t++)
    {
        if (!TiketValjan(t)) continue;
        new kratko[40];
        strcat(kratko, gTiket[t][tkTekst], sizeof kratko);
        format(red, sizeof red, "#%d %s %s (%d) - %d min: %s", gTiket[t][tkBroj], gTiket[t][tkTip] == TIKET_PRIJAVA ? T(playerid, "{FF6347}[Prijava]{FFFFFF}") : T(playerid, "{8AB4F8}[Pitanje]{FFFFFF}"), GetRPIme(gTiket[t][tkIgrac]), gTiket[t][tkIgrac], (sad - gTiket[t][tkVrijeme]) / 60, kratko);
        DodajStupac(list, len, red);
        gTiketPopis[playerid][n++] = t;
    }
    if (!n) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Nema otvorenih upita."));
    return PrikaziDialog(playerid, DLG_TIKETI, DIALOG_STYLE_LIST, T(playerid, "Upiti igraca"), list, T(playerid, "Otvori"), T(playerid, "Zatvori"));
}

static TiketPrikazi(playerid, const prijevod[])
{
    new t = gTiketOdabran[playerid];
    if (!TiketValjan(t)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Upit vise ne postoji."));
    new body[600], naslov[48];
    if (gTiket[t][tkTip] == TIKET_PRIJAVA) format(body, sizeof body, T(playerid, "Igrac: %s (%d)\nPrijavljeni: %s (%d)\n\n%s"), GetRPIme(gTiket[t][tkIgrac]), gTiket[t][tkIgrac], IsPlayerConnected(gTiket[t][tkCilj]) ? GetRPIme(gTiket[t][tkCilj]) : ("-"), gTiket[t][tkCilj], gTiket[t][tkTekst]);
    else format(body, sizeof body, T(playerid, "Igrac: %s (%d)\n\n%s"), GetRPIme(gTiket[t][tkIgrac]), gTiket[t][tkIgrac], gTiket[t][tkTekst]);
    if (prijevod[0])
    {
        strcat(body, "\n\n{8AB4F8}");
        strcat(body, T(playerid, "Prijevod:"));
        strcat(body, " {FFFFFF}");
        strcat(body, prijevod);
    }
    strcat(body, "\n\n");
    strcat(body, T(playerid, "Upisite odgovor (prazno = zatvori bez odgovora):"));
    format(naslov, sizeof naslov, T(playerid, "Upit #%d"), gTiket[t][tkBroj]);
    return PrikaziDialog(playerid, DLG_TIKET, DIALOG_STYLE_INPUT, naslov, body, T(playerid, "Odgovori"), T(playerid, "Natrag"));
}

static bool:TiketPrevedi(playerid, vrsta, od, za, const tekst[])
{
    if (od == za) return false;
    new url[64];
    format(url, sizeof url, JEZICI_SERVER, gJezikCode[od], gJezikCode[za]);
    return HTTP((gConnSerial[playerid] << 12) | (vrsta << 10) | playerid, HTTP_POST, url, tekst, "TiketPrijevod") != 0;
}

DLG:tiketi(playerid, response, listitem)
{
    if (!response || !gAdmin[playerid] || listitem < 0 || listitem >= TIKET_MAX) return 1;
    new t = gTiketPopis[playerid][listitem];
    if (!TiketValjan(t)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Upit vise ne postoji."));
    gTiketOdabran[playerid] = t;
    if (TiketPrevedi(playerid, TP_ADMINU, gJezik[gTiket[t][tkIgrac]], gJezik[playerid], gTiket[t][tkTekst])) return 1;
    return TiketPrikazi(playerid, "");
}

static TiketPosalji(admin, const tekst[])
{
    new p = gTiketPrima[admin];
    if (!IsPlayerConnected(p) || gAccountId[p] != gTiketPrimaRacun[admin]) return;
    new poruka[220], ime[MAX_PLAYER_NAME + 1];
    GetPlayerName(admin, ime, sizeof ime);
    format(poruka, sizeof poruka, T(p, "(( Admin %s odgovara: %s ))"), ime, tekst);
    SendLong(p, COLOR_ADMIN, poruka);
    PlayerPlaySound(p, 1085, 0.0, 0.0, 0.0);
}

DLG:tiket(playerid, response, const inputtext[])
{
    if (!gAdmin[playerid]) return 1;
    new t = gTiketOdabran[playerid];
    new prazno[1];
    if (!response) return cmd_tiketi(playerid, prazno);
    if (!TiketValjan(t)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Upit vise ne postoji."));
    new p = gTiket[t][tkIgrac], broj = gTiket[t][tkBroj], ime[MAX_PLAYER_NAME + 1];
    gTiket[t][tkIma] = false;
    gTiketOdabran[playerid] = -1;
    GetPlayerName(playerid, ime, sizeof ime);
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && gAdmin[i]) SendClientMessage(i, COLOR_ADMIN, T(i, inputtext[0] ? ("Admin %s je odgovorio na upit #%d.") : ("Admin %s je zatvorio upit #%d.")), ime, broj);
    if (!inputtext[0]) return 1;
    gTiketOdgovor[playerid][0] = EOS;
    strcat(gTiketOdgovor[playerid], inputtext, TIKET_LEN);
    RemoveBoje(gTiketOdgovor[playerid]);
    gTiketPrima[playerid] = p;
    gTiketPrimaRacun[playerid] = gAccountId[p];
    if (!TiketPrevedi(playerid, TP_IGRACU, gJezik[playerid], gJezik[p], gTiketOdgovor[playerid])) TiketPosalji(playerid, gTiketOdgovor[playerid]);
    return 1;
}

forward TiketPrijevod(index, response_code, const data[]);
public TiketPrijevod(index, response_code, const data[])
{
    new playerid = index & 0x3FF;
    if (playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid) || (index >>> 12) != gConnSerial[playerid]) return 1;
    new tekst[TIKET_LEN + 16];
    if (response_code == 200 && data[0]) strcat(tekst, data);
    RemoveBoje(tekst);
    if (((index >>> 10) & 3) == TP_ADMINU) return TiketPrikazi(playerid, tekst);
    TiketPosalji(playerid, tekst[0] ? tekst : gTiketOdgovor[playerid]);
    return 1;
}
