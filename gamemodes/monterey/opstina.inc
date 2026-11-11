#define OPSTINA_DOMET       2.5
#define OPSTINA_SKIN        141
#define IME_CIJENA          5000
#define IME_MIN             5
#define IME_MAX             20
#define OPSTINA_SVIJET      8000
#define OPSTINA_VRATA_DOMET 2.0
#define SLOT_ULAZ           2
#define SLOT_UNUTRA         3

enum { SALTER_UPRAVA, SALTER_VOZILA, SALTER_BROJ }
enum E_SALTER { bool:stIma, Float:stX, Float:stY, Float:stZ, Float:stA, stInt, stSvijet, stAktor, STREAMER_TAG_3D_TEXT_LABEL:stTekst }
enum E_PONUDA { pnProdavac, pnRacun, pnVozilo, pnDb, pnCijena }

static gSalter[SALTER_BROJ][E_SALTER];
static gBlagajna;
static gOpVozila[MAX_PLAYERS][VLASNISTVO_MAX], gOpOdabrano[MAX_PLAYERS];
static gPonuda[MAX_PLAYERS][E_PONUDA];

enum E_VRATA { bool:vrIma, Float:vrX, Float:vrY, Float:vrZ, Float:vrA, vrInt, Float:vrIx, Float:vrIy, Float:vrIz, Float:vrIa,
    STREAMER_TAG_PICKUP:vrPickup, STREAMER_TAG_PICKUP:vrPickupU, STREAMER_TAG_3D_TEXT_LABEL:vrTekst, STREAMER_TAG_3D_TEXT_LABEL:vrTekstU }
static gVrata[E_VRATA];
static Float:gApUlaz[MAX_PLAYERS][4];
enum E_OPINT { oiIme[32], oiInt, Float:oiX, Float:oiY, Float:oiZ, Float:oiA }
static const gOpstinaInt[][E_OPINT] = {
    { "City Planning", 3, 384.808, 173.804, 1008.382, 90.0 }
};
static const gSalterNatpis[SALTER_BROJ][] = { "Opstina\nKazne i promjena imena\n[Y]", "Opstina\nProdaja vozila\n[Y]" };

static bool:ImeIspravno(const ime[])
{
    new len = strlen(ime), crta = -1;
    if (len < IME_MIN || len > IME_MAX) return false;
    for (new i = 0; i < len; i++)
    {
        if (ime[i] == '_')
        {
            if (crta != -1) return false;
            crta = i;
            continue;
        }
        new bool:pocetak = i == 0 || i == crta + 1;
        if (pocetak ? (ime[i] < 'A' || ime[i] > 'Z') : (ime[i] < 'a' || ime[i] > 'z')) return false;
    }
    return crta >= 2 && len - crta - 1 >= 2;
}

InitOpstina()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS city_hall (slot INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS city_budget (id INTEGER PRIMARY KEY CHECK (id = 1), amount INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS fines (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, amount INTEGER NOT NULL, reason TEXT NOT NULL, officer TEXT NOT NULL, created INTEGER NOT NULL, paid INTEGER NOT NULL DEFAULT 0)"));

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT amount FROM city_budget WHERE id = 1");
    gBlagajna = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);

    res = DB_ExecuteQuery(gDB, "SELECT slot, x, y, z, a, interior, world FROM city_hall");
    if (DB_GetRowCount(res)) do
    {
        new s = DB_GetFieldInt(res, 0);
        if (s == SLOT_ULAZ || s == SLOT_UNUTRA)
        {
            new Float:x = DB_GetFieldFloat(res, 1), Float:y = DB_GetFieldFloat(res, 2), Float:z = DB_GetFieldFloat(res, 3), Float:a = DB_GetFieldFloat(res, 4);
            if (s == SLOT_ULAZ)
            {
                gVrata[vrX] = x;
                gVrata[vrY] = y;
                gVrata[vrZ] = z;
                gVrata[vrA] = a;
            }
            else
            {
                gVrata[vrIx] = x;
                gVrata[vrIy] = y;
                gVrata[vrIz] = z;
                gVrata[vrIa] = a;
                gVrata[vrInt] = DB_GetFieldInt(res, 5);
            }
            continue;
        }
        if (s < 0 || s >= SALTER_BROJ) continue;
        gSalter[s][stX] = DB_GetFieldFloat(res, 1);
        gSalter[s][stY] = DB_GetFieldFloat(res, 2);
        gSalter[s][stZ] = DB_GetFieldFloat(res, 3);
        gSalter[s][stA] = DB_GetFieldFloat(res, 4);
        gSalter[s][stInt] = DB_GetFieldInt(res, 5);
        gSalter[s][stSvijet] = DB_GetFieldInt(res, 6);
        SalterStvori(s);
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM city_hall WHERE slot IN (%d, %d)", SLOT_ULAZ, SLOT_UNUTRA);
    if (DB_GetFieldInt(res, 0) == 2) VrataStvori();
    DB_FreeResultSet(res);

    assert(ImeIspravno("Ivan_Horvat") && ImeIspravno("Jo_Li"));
    assert(!ImeIspravno("ivan_Horvat") && !ImeIspravno("IvanHorvat") && !ImeIspravno("Ivan_Ho_Rvat") && !ImeIspravno("Ivan_H0rvat") && !ImeIspravno("I_Horvat"));
}

static SalterStvori(s)
{
    gSalter[s][stIma] = true;
    gSalter[s][stAktor] = CreateActor(OPSTINA_SKIN, gSalter[s][stX], gSalter[s][stY], gSalter[s][stZ], gSalter[s][stA]);
    SetActorVirtualWorld(gSalter[s][stAktor], gSalter[s][stSvijet]);
    gSalter[s][stTekst] = CreateDynamic3DTextLabel(gSalterNatpis[s], COLOR_ORG, gSalter[s][stX], gSalter[s][stY], gSalter[s][stZ] + 1.2, 8.0,
        INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gSalter[s][stSvijet], gSalter[s][stInt]);
}

static SalterUkloni(s)
{
    if (!gSalter[s][stIma]) return 0;
    DestroyActor(gSalter[s][stAktor]);
    DestroyDynamic3DTextLabel(gSalter[s][stTekst]);
    gSalter[s][stIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM city_hall WHERE slot = %d", s));
    return 1;
}

static SalterPostavi(playerid, s)
{
    if (gSalter[s][stIma])
    {
        DestroyActor(gSalter[s][stAktor]);
        DestroyDynamic3DTextLabel(gSalter[s][stTekst]);
    }
    GetPlayerPos(playerid, gSalter[s][stX], gSalter[s][stY], gSalter[s][stZ]);
    GetPlayerFacingAngle(playerid, gSalter[s][stA]);
    gSalter[s][stInt] = GetPlayerInterior(playerid);
    gSalter[s][stSvijet] = GetPlayerVirtualWorld(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO city_hall (slot, x, y, z, a, interior, world) VALUES (%d, %f, %f, %f, %f, %d, %d)",
        s, gSalter[s][stX], gSalter[s][stY], gSalter[s][stZ], gSalter[s][stA], gSalter[s][stInt], gSalter[s][stSvijet]));
    SalterStvori(s);
}

static VrataStvori()
{
    if (gVrata[vrIma])
    {
        DestroyDynamicPickup(gVrata[vrPickup]);
        DestroyDynamicPickup(gVrata[vrPickupU]);
        DestroyDynamic3DTextLabel(gVrata[vrTekst]);
        DestroyDynamic3DTextLabel(gVrata[vrTekstU]);
    }
    gVrata[vrIma] = true;
    gVrata[vrPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gVrata[vrX], gVrata[vrY], gVrata[vrZ], 0, 0);
    gVrata[vrTekst] = CreateDynamic3DTextLabel("Opstina\n[Y]", COLOR_ORG, gVrata[vrX], gVrata[vrY], gVrata[vrZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gVrata[vrPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gVrata[vrIx], gVrata[vrIy], gVrata[vrIz], OPSTINA_SVIJET, gVrata[vrInt]);
    gVrata[vrTekstU] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gVrata[vrIx], gVrata[vrIy], gVrata[vrIz] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, OPSTINA_SVIJET, gVrata[vrInt]);
}

static OpstinaUnutra(playerid)
{
    SetPlayerInterior(playerid, gVrata[vrInt]);
    SetPlayerVirtualWorld(playerid, OPSTINA_SVIJET);
    SetPlayerPos(playerid, gVrata[vrIx], gVrata[vrIy], gVrata[vrIz]);
    SetPlayerFacingAngle(playerid, gVrata[vrIa]);
    SetCameraBehindPlayer(playerid);
}

OpstinaUlaz(playerid)
{
    if (!gVrata[vrIma]) return 0;
    if (GetPlayerVirtualWorld(playerid) == OPSTINA_SVIJET && GetPlayerInterior(playerid) == gVrata[vrInt])
    {
        if (!IsPlayerInRangeOfPoint(playerid, OPSTINA_VRATA_DOMET, gVrata[vrIx], gVrata[vrIy], gVrata[vrIz])) return 0;
        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);
        SetPlayerPos(playerid, gVrata[vrX], gVrata[vrY], gVrata[vrZ]);
        SetPlayerFacingAngle(playerid, gVrata[vrA] + 180.0);
        SetCameraBehindPlayer(playerid);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, OPSTINA_VRATA_DOMET, gVrata[vrX], gVrata[vrY], gVrata[vrZ])) return 0;
    OpstinaUnutra(playerid);
    return 1;
}

static VrataSpremi(slot, Float:x, Float:y, Float:z, Float:a, interior, svijet)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO city_hall (slot, x, y, z, a, interior, world) VALUES (%d, %f, %f, %f, %f, %d, %d)", slot, x, y, z, a, interior, svijet));
}

static VrataPostavi(playerid, interior, Float:ix, Float:iy, Float:iz, Float:ia)
{
    gVrata[vrX] = gApUlaz[playerid][0];
    gVrata[vrY] = gApUlaz[playerid][1];
    gVrata[vrZ] = gApUlaz[playerid][2];
    gVrata[vrA] = gApUlaz[playerid][3];
    gVrata[vrInt] = interior;
    gVrata[vrIx] = ix;
    gVrata[vrIy] = iy;
    gVrata[vrIz] = iz;
    gVrata[vrIa] = ia;
    VrataSpremi(SLOT_ULAZ, gVrata[vrX], gVrata[vrY], gVrata[vrZ], gVrata[vrA], 0, 0);
    VrataSpremi(SLOT_UNUTRA, ix, iy, iz, ia, interior, OPSTINA_SVIJET);
    VrataStvori();
    OpstinaUnutra(playerid);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Ulaz u opstinu je postavljen. Sada ste unutra: postavite saltere."));
}

static ShowOpstinaInt(playerid)
{
    new list[256], len = 0;
    for (new i = 0; i < sizeof gOpstinaInt; i++) DodajStupac(list, len, gOpstinaInt[i][oiIme]);
    DodajStupac(list, len, T(playerid, "{8AB4F8}Vlastiti ID i lokacija"));
    return PrikaziDialog(playerid, DLG_APANEL_OPSTINA_INT, DIALOG_STYLE_LIST, T(playerid, "Interijer opstine"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_opstina_int(playerid, response, listitem)
{
    if (!response) return ShowOpstinaAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem >= 0 && listitem < sizeof gOpstinaInt)
    {
        VrataPostavi(playerid, gOpstinaInt[listitem][oiInt], gOpstinaInt[listitem][oiX], gOpstinaInt[listitem][oiY], gOpstinaInt[listitem][oiZ], gOpstinaInt[listitem][oiA]);
        return ShowOpstinaAdmin(playerid);
    }
    return PrikaziDialog(playerid, DLG_APANEL_OPSTINA_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Interijer opstine"), T(playerid, "Unesite: [interior] [x] [y] [z]"), T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

DLG:apanel_opstina_unos(playerid, response, const inputtext[])
{
    new interior, Float:x, Float:y, Float:z;
    if (!response) return ShowOpstinaInt(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!DialogInterijer(inputtext, interior, x, y, z))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [interior] [x] [y] [z]"));
        return ShowOpstinaInt(playerid);
    }
    VrataPostavi(playerid, interior, x, y, z, 0.0);
    return ShowOpstinaAdmin(playerid);
}

static bool:SalterBlizu(playerid, s)
{
    return gSalter[s][stIma] && GetPlayerVirtualWorld(playerid) == gSalter[s][stSvijet] && GetPlayerInterior(playerid) == gSalter[s][stInt]
        && IsPlayerInRangeOfPoint(playerid, OPSTINA_DOMET, gSalter[s][stX], gSalter[s][stY], gSalter[s][stZ]);
}

static BlagajnaDodaj(iznos)
{
    gBlagajna += iznos;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO city_budget (id, amount) VALUES (1, %d)", gBlagajna));
}

KaznaDodaj(target, policajac, iznos, const razlog[])
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO fines (account, amount, reason, officer, created) VALUES (%d, %d, '%q', '%q', %d)",
        gAccountId[target], iznos, razlog, GetRPIme(policajac), gettime()));
}

OpstinaSalter(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    if (SalterBlizu(playerid, SALTER_UPRAVA)) return ShowUprava(playerid), 1;
    if (SalterBlizu(playerid, SALTER_VOZILA)) return ShowProdaja(playerid), 1;
    return 0;
}

// kazne i promjena imena

static ShowUprava(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*), COALESCE(SUM(amount), 0) FROM fines WHERE account = %d AND paid = 0", gAccountId[playerid]);
    new broj = DB_GetFieldInt(res, 0), ukupno = DB_GetFieldInt(res, 1), list[160];
    DB_FreeResultSet(res);
    format(list, sizeof list, T(playerid, "Plati kazne (%d, ukupno $%d)\nPromjena imena ($%d)\nProdaja kuce"), broj, ukupno, IME_CIJENA);
    return PrikaziDialog(playerid, DLG_OPSTINA, DIALOG_STYLE_LIST, T(playerid, "Opstina"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static ShowKazne(playerid)
{
    new list[2048], row[160], razlog[64], izdao[MAX_PLAYER_NAME + 1], len = 0;
    format(list, sizeof list, "%s\t%s\t%s\n", T(playerid, "Iznos"), T(playerid, "Razlog"), T(playerid, "Izdao"));
    len = strlen(list);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT amount, reason, officer FROM fines WHERE account = %d AND paid = 0 ORDER BY id LIMIT 20", gAccountId[playerid]);
    new n = DB_GetRowCount(res);
    if (n) do
    {
        DB_GetFieldString(res, 1, razlog);
        DB_GetFieldString(res, 2, izdao);
        format(row, sizeof row, "$%d\t%s\t%s", DB_GetFieldInt(res, 0), razlog, izdao);
    } while (DodajStupac(list, len, row) && DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    if (!n)
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Nemate neplacenih kazni."));
        return ShowUprava(playerid);
    }
    return PrikaziDialog(playerid, DLG_OPSTINA_KAZNE, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Neplacene kazne"), list, T(playerid, "Plati"), T(playerid, "Natrag"));
}

DLG:opstina(playerid, response, listitem)
{
    if (!response || !SalterBlizu(playerid, SALTER_UPRAVA)) return 1;
    if (listitem == 0) return ShowKazne(playerid);
    if (listitem == 2) return KucaProdajaPopis(playerid);
    new body[192];
    format(body, sizeof body, T(playerid, "Promjena imena kosta $%d.\nUnesite novo ime u obliku Ime_Prezime:"), IME_CIJENA);
    return PrikaziDialog(playerid, DLG_OPSTINA_IME, DIALOG_STYLE_INPUT, T(playerid, "Promjena imena"), body, T(playerid, "Promijeni"), T(playerid, "Natrag"));
}

DLG:opstina_kazne(playerid, response, listitem)
{
    if (!response) return ShowUprava(playerid);
    if (!SalterBlizu(playerid, SALTER_UPRAVA) || listitem < 0) return 1;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, amount FROM fines WHERE account = %d AND paid = 0 ORDER BY id LIMIT 1 OFFSET %d", gAccountId[playerid], listitem);
    new ima = DB_GetRowCount(res), id = ima ? DB_GetFieldInt(res, 0) : 0, iznos = ima ? DB_GetFieldInt(res, 1) : 0;
    DB_FreeResultSet(res);
    if (!ima) return ShowKazne(playerid);
    if (!DajNovac(playerid, -iznos)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), ShowKazne(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE fines SET paid = %d WHERE id = %d", gettime(), id));
    BlagajnaDodaj(iznos);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste kaznu od $%d."), iznos);
    return ShowKazne(playerid);
}

DLG:opstina_ime(playerid, response, const inputtext[])
{
    if (!response) return ShowUprava(playerid);
    if (!SalterBlizu(playerid, SALTER_UPRAVA)) return 1;
    if (!ImeIspravno(inputtext)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ime mora biti u obliku Ime_Prezime (samo slova, %d-%d znakova)."), IME_MIN, IME_MAX), ShowUprava(playerid);
    if (gNovac[playerid] < IME_CIJENA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), ShowUprava(playerid);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM accounts WHERE name = '%q' LIMIT 1", inputtext);
    new zauzeto = DB_GetRowCount(res);
    DB_FreeResultSet(res);
    if (zauzeto || SetPlayerName(playerid, inputtext) != 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "To ime je zauzeto.")), ShowUprava(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET name = '%q' WHERE id = %d", inputtext, gAccountId[playerid]));
    DajNovac(playerid, -IME_CIJENA);
    BlagajnaDodaj(IME_CIJENA);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vase novo ime je %s. Od sada se prijavljujete s tim imenom."), inputtext);
}

// prodaja vozila

static ShowProdaja(playerid)
{
    new n = VlasnistvaIgraca(playerid, gOpVozila[playerid]), list[512], row[64], tablica[12], len = 0;
    if (!n) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Nemate vozila u vlasnistvu."));
    for (new i = 0; i < n; i++)
    {
        VoziloTablica(gOpVozila[playerid][i], tablica);
        format(row, sizeof row, "%s (%s)", gImenaVozila[GetVehicleModel(gOpVozila[playerid][i]) - 400], tablica);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_OPSTINA_VOZILA, DIALOG_STYLE_LIST, T(playerid, "Vasa vozila - prodaja"), list, T(playerid, "Prodaj"), T(playerid, "Zatvori"));
}

DLG:opstina_vozila(playerid, response, listitem)
{
    if (!response || !SalterBlizu(playerid, SALTER_VOZILA) || listitem < 0 || listitem >= VLASNISTVO_MAX) return 1;
    new v = gOpVozila[playerid][listitem];
    if (VlasnikVozila(v) != gAccountId[playerid]) return ShowProdaja(playerid);
    gOpOdabrano[playerid] = v;
    return PrikaziDialog(playerid, DLG_OPSTINA_PRODAJA, DIALOG_STYLE_INPUT, T(playerid, "Prodaja vozila"),
        T(playerid, "Kupac mora stajati pored vas.\nUnesite: [id/ime kupca] [cijena]"), T(playerid, "Ponudi"), T(playerid, "Natrag"));
}

DLG:opstina_prodaja(playerid, response, const inputtext[])
{
    if (!response) return ShowProdaja(playerid);
    new v = gOpOdabrano[playerid], idx = 0, p1[32], p2[16];
    if (!SalterBlizu(playerid, SALTER_VOZILA) || VlasnikVozila(v) != gAccountId[playerid]) return 1;
    if (!SljedeciParam(inputtext, idx, p1) || !SljedeciParam(inputtext, idx, p2) || !ProvjeriBroj(p2) || strlen(p2) > 9)
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [id/ime kupca] [cijena]")), ShowProdaja(playerid);
    new target = ProvjetiBlizinuIgraca(playerid, p1), cijena = strval(p2);
    if (target == INVALID_PLAYER_ID) return ShowProdaja(playerid);
    if (cijena < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Cijena mora biti veca od 0.")), ShowProdaja(playerid);
    gPonuda[target][pnProdavac] = playerid;
    gPonuda[target][pnRacun] = gAccountId[playerid];
    gPonuda[target][pnVozilo] = v;
    gPonuda[target][pnDb] = VlasnikDb(v);
    gPonuda[target][pnCijena] = cijena;
    new body[192], tablica[12];
    VoziloTablica(v, tablica);
    format(body, sizeof body, T(target, "%s vam prodaje vozilo %s (%s) za $%d.\nPrihvacate li?"), GetRPIme(playerid), gImenaVozila[GetVehicleModel(v) - 400], tablica, cijena);
    PrikaziDialog(target, DLG_OPSTINA_PONUDA, DIALOG_STYLE_MSGBOX, T(target, "Kupnja vozila"), body, T(target, "Kupi"), T(target, "Odbij"));
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Ponuda je poslana igracu %s."), GetRPIme(target));
}

DLG:opstina_ponuda(playerid, response)
{
    new prodavac = gPonuda[playerid][pnProdavac], racun = gPonuda[playerid][pnRacun], v = gPonuda[playerid][pnVozilo], cijena = gPonuda[playerid][pnCijena];
    gPonuda[playerid][pnRacun] = 0;
    new bool:vrijedi = racun && IsPlayerConnected(prodavac) && gUlogiran[prodavac] && gAccountId[prodavac] == racun
        && VlasnikVozila(v) == racun && VlasnikDb(v) == gPonuda[playerid][pnDb];
    if (!response)
    {
        if (vrijedi) SendClientMessage(prodavac, COLOR_INFO, T(prodavac, "%s je odbio ponudu."), GetRPIme(playerid));
        return 1;
    }
    if (!vrijedi) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ponuda vise ne vrijedi."));
    new Float:x, Float:y, Float:z;
    GetPlayerPos(prodavac, x, y, z);
    if (!IsPlayerInRangeOfPoint(playerid, PAY_RANGE, x, y, z) || GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(prodavac))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije blizu vas."));
    if (BrojVlasnistava(gAccountId[playerid]) >= VLASNISTVO_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mozete imati najvise %d vozila."), VLASNISTVO_MAX);
    if (!TransferNovac(playerid, prodavac, cijena)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), SendClientMessage(prodavac, COLOR_ERROR, T(prodavac, "Kupac nema dovoljno novca."));
    VlasnikPrenesi(v, gAccountId[playerid]);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s preuzima kljuceve vozila od %s.", GetRPIme(playerid), GetRPIme(prodavac), 1);
    SendClientMessage(prodavac, COLOR_INFO, T(prodavac, "Prodali ste vozilo za $%d."), cijena);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste vozilo za $%d."), cijena);
}

// Admin panel

ShowOpstinaAdmin(playerid)
{
    new list[256];
    format(list, sizeof list, T(playerid, "Postavi salter za kazne i promjenu imena ovdje\nPostavi salter za prodaju vozila ovdje\nPostavi ulaz u opstinu ovdje\nGradska blagajna: $%d\nUkloni salter za kazne i promjenu imena\nUkloni salter za prodaju vozila"), gBlagajna);
    return PrikaziDialog(playerid, DLG_APANEL_OPSTINA, DIALOG_STYLE_LIST, T(playerid, "Opstina"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_opstina(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == SALTER_UPRAVA || listitem == SALTER_VOZILA)
    {
        SalterPostavi(playerid, listitem);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Salter je postavljen. Sluzbenik stoji na vasem mjestu."));
    }
    else if (listitem == SLOT_ULAZ)
    {
        if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ulaz se postavlja samo izvana."));
        else
        {
            GetPlayerPos(playerid, gApUlaz[playerid][0], gApUlaz[playerid][1], gApUlaz[playerid][2]);
            GetPlayerFacingAngle(playerid, gApUlaz[playerid][3]);
            return ShowOpstinaInt(playerid);
        }
    }
    else if (listitem == 4 || listitem == 5)
    {
        if (SalterUkloni(listitem - 4)) SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Salter je uklonjen."));
        else SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Taj salter nije postavljen."));
    }
    return ShowOpstinaAdmin(playerid);
}
