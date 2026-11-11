#define CHAT_RANGE          20.0
#define CHAT_RANGE_SAPTANJE    4.0
#define CHAT_RANGE_VIKANJE     40.0
#define CHAT_FADE_STEPS     4
#define MAX_OPIS            128
#define CHAT_SPLIT          110

#define COLOR_CHAT          0xFFFFFFFF
#define COLOR_ME            0xC2A2DAFF

enum { CHAT_GOVOR, CHAT_SAPTANJE, CHAT_VIKANJE }

GetRPIme(playerid)
{
    new name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof name);
    for (new i = 0; name[i]; i++) if (name[i] == '_') name[i] = ' ';
    return name;
}

RemoveBoje(text[])
{
    for (new i = 0; text[i]; i++) if (text[i] == '{' || text[i] == '}') text[i] = '|';
}

ZatamniBoju(color, step)
{
    new pct = 100 - step * 5;
    new r = ((color >>> 24) & 0xFF) * pct / 100;
    new g = ((color >>> 16) & 0xFF) * pct / 100;
    new b = ((color >>> 8) & 0xFF) * pct / 100;
    return (r << 24) | (g << 16) | (b << 8) | (color & 0xFF);
}

PosaljiBlizu(playerid, Float:range, color, const msg[], const a[] = "", const b[] = "", prevedi = 0)
{
    new Float:x, Float:y, Float:z, vw = GetPlayerVirtualWorld(playerid), interior = GetPlayerInterior(playerid), tekst[160];
    GetPlayerPos(playerid, x, y, z);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || !gUlogiran[i]) continue;
        if (GetPlayerVirtualWorld(i) != vw || GetPlayerInterior(i) != interior) continue;
        new Float:d = GetPlayerDistanceFromPoint(i, x, y, z);
        if (d > range) continue;
        if (!prevedi) strcopy(tekst, msg);
        else format(tekst, sizeof tekst, T(i, msg), a, prevedi == 2 ? T(i, b) : b);
        SendLong(i, ZatamniBoju(color, floatround(d / range * CHAT_FADE_STEPS, floatround_floor)), tekst);
    }
}

SendLong(playerid, color, const tekst[])
{
    if (strlen(tekst) <= CHAT_SPLIT)
    {
        SendClientMessage(playerid, color, "%s", tekst);
        return;
    }
    new part[CHAT_SPLIT + 5];
    strmid(part, tekst, 0, CHAT_SPLIT);
    SendClientMessage(playerid, color, "%s ...", part);
    SendClientMessage(playerid, color, "... %s", tekst[CHAT_SPLIT]);
}

public OnPlayerText(playerid, text[])
{
    if (!gUlogiran[playerid]) return 0;
    RemoveBoje(text);
    if (MobitelText(playerid, text)) return 0;
    SendChat(playerid, text, CHAT_GOVOR);
    return 0;
}

CMD:w(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /w [poruka]"));
    RemoveBoje(params);
    SendChat(playerid, params, CHAT_SAPTANJE);
    return 1;
}

CMD:s(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /s [poruka]"));
    RemoveBoje(params);
    SendChat(playerid, params, CHAT_VIKANJE);
    return 1;
}

CMD:shout(playerid, params[]) return cmd_s(playerid, params);

CMD:ooc(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /ooc [poruka]"));
    RemoveBoje(params);
    new msg[160];
    format(msg, sizeof msg, "(( OOC %s: %s ))", GetRPIme(playerid), params);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_INFO, msg);
    return 1;
}

static BezPrijevoda(playerid, params[], const upute[], vrsta)
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, upute));
    RemoveBoje(params);
    ShowChat(playerid, params, vrsta);
    return 1;
}

CMD:t(playerid, params[]) return BezPrijevoda(playerid, params, "Koristenje: /t [poruka]", CHAT_GOVOR);
CMD:wt(playerid, params[]) return BezPrijevoda(playerid, params, "Koristenje: /wt [poruka]", CHAT_SAPTANJE);
CMD:st(playerid, params[]) return BezPrijevoda(playerid, params, "Koristenje: /st [poruka]", CHAT_VIKANJE);

CMD:me(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /me [radnja]"));
    RemoveBoje(params);
    new msg[160];
    format(msg, sizeof msg, "* %s %s", GetRPIme(playerid), params);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, msg);
    return 1;
}

CMD:do(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /do [opis situacije]"));
    RemoveBoje(params);
    new msg[160];
    format(msg, sizeof msg, "* %s (( %s ))", params, GetRPIme(playerid));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, msg);
    return 1;
}

CMD:setme(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!params[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /setme [opis vaseg lika, do %d znakova]"), MAX_OPIS);
    if (strlen(params) > MAX_OPIS) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Opis moze imati najvise %d znakova."), MAX_OPIS);
    RemoveBoje(params);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET opis = '%q' WHERE id = %d", params, gAccountId[playerid]));
    SendClientMessage(playerid, COLOR_ME, T(playerid, "Opis spremljen: %s"), params);
    return 1;
}

CMD:showme(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new target = playerid;
    if (params[0])
    {
        new idx = 0, p1[32];
        SljedeciParam(params, idx, p1);
        target = FindPlayer(p1);
        if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
        new Float:x, Float:y, Float:z;
        GetPlayerPos(target, x, y, z);
        if (GetPlayerDistanceFromPoint(playerid, x, y, z) > CHAT_RANGE || GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(target))
            return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije u vasoj blizini."));
    }

    new opis[MAX_OPIS + 1];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT opis FROM accounts WHERE id = %d", gAccountId[target]);
    if (DB_GetRowCount(res)) DB_GetFieldString(res, 0, opis, sizeof opis);
    DB_FreeResultSet(res);

    if (!opis[0])
    {
        if (target == playerid) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Nemate opis. Postavite ga s /setme [opis]."));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "%s nema postavljen opis."), GetRPIme(target));
    }
    SendClientMessage(playerid, COLOR_ME, T(playerid, "* Opis (%s): %s"), GetRPIme(target), opis);
    return 1;
}

#define BROJ_JEZIKA          5
#define LANG_NONE           -1
#define SKOLA_TEST          10000
#define SKOLA_X             1481.0
#define SKOLA_Y             -1771.0
#define SKOLA_Z             18.8
#define JEZICI_SERVER       "127.0.0.1:7780/t?f=%s&t=%s"

static const gJezikCode[BROJ_JEZIKA][] = { "hr", "en", "ru", "es", "tr" };
static const gJezikName[BROJ_JEZIKA][] = { "Hrvatski / Srpski / Bosanski", "English", "Russkiy", "Espanol", "Turkce" };

new gJezik[MAX_PLAYERS];
new gNauceniJezici[MAX_PLAYERS];
new gOdabranJezik[MAX_PLAYERS];
new gUciJezik[MAX_PLAYERS];
new gConnSerial[MAX_PLAYERS];
new gPendingPoruka[MAX_PLAYERS][144];

InitJezik()
{
    CreatePickup(1239, 1, SKOLA_X, SKOLA_Y, SKOLA_Z);
    Create3DTextLabel("Skola jezika\n/skola", 0xC2A2DAFF, SKOLA_X, SKOLA_Y, SKOLA_Z + 0.5, 15.0, 0);
}

JezikConnect(playerid)
{
    gConnSerial[playerid]++;
    gJezik[playerid] = 0;
    gNauceniJezici[playerid] = 1;
    gOdabranJezik[playerid] = LANG_NONE;
    gUciJezik[playerid] = LANG_NONE;
}

bool:UcitajJezik(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT jezik, jezici, prijevod, ucenje FROM accounts WHERE id = %d", gAccountId[playerid]);
    new lang = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : LANG_NONE;
    if (lang >= 0 && lang < BROJ_JEZIKA)
    {
        gJezik[playerid] = lang;
        gNauceniJezici[playerid] = DB_GetFieldInt(res, 1) | (1 << lang);
        gOdabranJezik[playerid] = DB_GetFieldInt(res, 2);
        gUciJezik[playerid] = DB_GetFieldInt(res, 3);
    }
    DB_FreeResultSet(res);
    if (lang < 0 || lang >= BROJ_JEZIKA) return false;
    if (gUciJezik[playerid] < 0 || gUciJezik[playerid] >= BROJ_JEZIKA) gUciJezik[playerid] = LANG_NONE;
    else ZapocniUcenje(playerid);
    if (gOdabranJezik[playerid] < 0 || gOdabranJezik[playerid] >= BROJ_JEZIKA || !(gNauceniJezici[playerid] & (1 << gOdabranJezik[playerid])))
        gOdabranJezik[playerid] = LANG_NONE;
    return true;
}

SpremiJezik(playerid)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET jezik = %d, jezici = %d, prijevod = %d, ucenje = %d WHERE id = %d",
        gJezik[playerid], gNauceniJezici[playerid], gOdabranJezik[playerid], gUciJezik[playerid], gAccountId[playerid]));
}

PokaziJezike(playerid)
{
    new list[160], len = 0;
    for (new i = 0; i < BROJ_JEZIKA; i++) DodajStupac(list, len, gJezikName[i]);
    return PrikaziDialog(playerid, DLG_JEZIK, DIALOG_STYLE_LIST, T(playerid, "Vas materinji jezik"), list, T(playerid, "Odaberi"), T(playerid, "Izlaz"));
}

DLG:jezik(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= BROJ_JEZIKA) return KickTimer(playerid);
    gJezik[playerid] = listitem;
    gNauceniJezici[playerid] = 1 << listitem;
    SpremiJezik(playerid);
    return ZavrsiLogin(playerid);
}

DLG:jezik_promjena(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= BROJ_JEZIKA || listitem == gJezik[playerid]) return 1;
    gNauceniJezici[playerid] = (gNauceniJezici[playerid] & ~(1 << gJezik[playerid])) | (1 << listitem);
    if (gOdabranJezik[playerid] == gJezik[playerid] || gOdabranJezik[playerid] == listitem) gOdabranJezik[playerid] = LANG_NONE;
    if (gUciJezik[playerid] == listitem) gUciJezik[playerid] = LANG_NONE;
    gJezik[playerid] = listitem;
    SpremiJezik(playerid);
    return SendClientMessage(playerid, COLOR_ME, T(playerid, "Jezik je promijenjen: %s."), gJezikName[listitem]);
}

DLG:skola(playerid, response, listitem)
{
    if (!response) return 1;
    new lang = NtiJezik(playerid, listitem, false);
    if (lang == LANG_NONE) return 1;
    gUciJezik[playerid] = lang;
    ZapocniUcenje(playerid);
    SpremiJezik(playerid);
    return SendClientMessage(playerid, COLOR_ME, T(playerid, "Upisali ste tecaj: %s. Nauciti cete ga za %d sekundi."), gJezikName[lang], SKOLA_TEST / 1000);
}

DLG:prijevod(playerid, response, listitem)
{
    if (!response) return 1;
    gOdabranJezik[playerid] = listitem == 0 ? LANG_NONE : NtiJezik(playerid, listitem - 1, true);
    SpremiJezik(playerid);
    if (gOdabranJezik[playerid] == LANG_NONE) return SendClientMessage(playerid, COLOR_ME, T(playerid, "Vase poruke se vise ne prevode."));
    return SendClientMessage(playerid, COLOR_ME, T(playerid, "Vase poruke ce se prevoditi na: %s."), gJezikName[gOdabranJezik[playerid]]);
}

NtiJezik(playerid, n, bool:known)
{
    for (new i = 0; i < BROJ_JEZIKA; i++)
    {
        new bool:has = (gNauceniJezici[playerid] & (1 << i)) != 0;
        if (i == gJezik[playerid] || has != known) continue;
        if (n-- == 0) return i;
    }
    return LANG_NONE;
}

CMD:jezik(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new list[160], len = 0;
    for (new i = 0; i < BROJ_JEZIKA; i++) DodajStupac(list, len, gJezikName[i]);
    return PrikaziDialog(playerid, DLG_JEZIK_PROMJENA, DIALOG_STYLE_LIST, T(playerid, "Vas materinji jezik"), list, T(playerid, "Odaberi"), T(playerid, "Odustani"));
}

CMD:skola(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (!IsPlayerInRangeOfPoint(playerid, 3.0, SKOLA_X, SKOLA_Y, SKOLA_Z))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod skole jezika (gradska vijecnica)."));
    if (gUciJezik[playerid] != LANG_NONE)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vec ucite %s."), gJezikName[gUciJezik[playerid]]);
    new list[160], len = 0;
    for (new i = 0; i < BROJ_JEZIKA; i++)
        if (!(gNauceniJezici[playerid] & (1 << i))) DodajStupac(list, len, gJezikName[i]);
    if (!list[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vec znate sve jezike."));
    return PrikaziDialog(playerid, DLG_SKOLA, DIALOG_STYLE_LIST, T(playerid, "Skola jezika"), list, T(playerid, "Upisi"), T(playerid, "Odustani"));
}

CMD:prijevod(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new list[192], count = 0;
    strcat(list, T(playerid, "Bez prijevoda (prikazi sto napisem)\n"));
    new len = strlen(list);
    for (new i = 0; i < BROJ_JEZIKA; i++)
        if (i != gJezik[playerid] && (gNauceniJezici[playerid] & (1 << i))) { DodajStupac(list, len, gJezikName[i]); count++; }
    if (!count) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Znate samo materinji jezik. Naucite novi u skoli jezika (/skola)."));
    return PrikaziDialog(playerid, DLG_PRIJEVOD, DIALOG_STYLE_LIST, T(playerid, "Jezik vasih poruka"), list, T(playerid, "Odaberi"), T(playerid, "Odustani"));
}

ZapocniUcenje(playerid)
{
    SetTimerEx("UcenjeZavrseno", SKOLA_TEST, false, "ii", playerid, gAccountId[playerid]);
}

forward UcenjeZavrseno(playerid, account);
public UcenjeZavrseno(playerid, account)
{
    if (gAccountId[playerid] != account || gUciJezik[playerid] == LANG_NONE) return;
    gNauceniJezici[playerid] |= 1 << gUciJezik[playerid];
    SendClientMessage(playerid, COLOR_ME, T(playerid, "Naucili ste %s! Odaberite jezik svojih poruka s /prijevod."), gJezikName[gUciJezik[playerid]]);
    gUciJezik[playerid] = LANG_NONE;
    SpremiJezik(playerid);
}

SendChat(playerid, const text[], vrsta)
{
    if (gOdabranJezik[playerid] != LANG_NONE)
    {
        new url[64];
        format(url, sizeof url, JEZICI_SERVER, gJezikCode[gJezik[playerid]], gJezikCode[gOdabranJezik[playerid]]);
        strcopy(gPendingPoruka[playerid], text);
        if (HTTP((gConnSerial[playerid] << 12) | (vrsta << 10) | playerid, HTTP_POST, url, text, "Prijevod")) return;
    }
    ShowChat(playerid, text, vrsta);
}

ShowChat(playerid, const text[], vrsta)
{
    new bool:mobitel = MobitelURazgovoru(playerid) != 0;
    switch (vrsta)
    {
        case CHAT_SAPTANJE: PosaljiBlizu(playerid, CHAT_RANGE_SAPTANJE, COLOR_CHAT, mobitel ? ("%s sapuce (telefon): %s") : ("%s sapuce: %s"), GetRPIme(playerid), text, 1);
        case CHAT_VIKANJE:  PosaljiBlizu(playerid, CHAT_RANGE_VIKANJE, COLOR_CHAT, mobitel ? ("%s vice (telefon): %s!") : ("%s vice: %s!"), GetRPIme(playerid), text, 1);
        default:         PosaljiBlizu(playerid, CHAT_RANGE, COLOR_CHAT, mobitel ? ("%s kaze (telefon): %s") : ("%s kaze: %s"), GetRPIme(playerid), text, 1);
    }
    MobitelGovor(playerid, text, vrsta);
}

forward Prijevod(index, response_code, const data[]);
public Prijevod(index, response_code, const data[])
{
    new playerid = index & 0x3FF;
    if (playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid) || (index >>> 12) != gConnSerial[playerid]) return 1;
    new text[144];
    if (response_code == 200 && data[0]) strcopy(text, data);
    else strcopy(text, gPendingPoruka[playerid]);
    RemoveBoje(text);
    ShowChat(playerid, text, (index >>> 10) & 3);
    return 1;
}
