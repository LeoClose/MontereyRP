#define OZNAKA_MAX          8192

static STREAMER_TAG_3D_TEXT_LABEL:gOznakaKopija[OZNAKA_MAX][BROJ_JEZIKA], bool:gOznakaIma[OZNAKA_MAX], STREAMER_TAG_AREA:gOznakaZona[OZNAKA_MAX];
static STREAMER_TAG_AREA:gInterakcijaZadnja[MAX_PLAYERS], gInterakcijaVrijeme[MAX_PLAYERS];

#define INTERAKCIJA_ZONA    0x494E54
enum { SAVJET_ALT, SAVJET_KUCA, SAVJET_GARAZA, SAVJET_IZLAZ, SAVJET_ULAZ, SAVJET_PODIGNI, SAVJET_GEPEK }
#define INTERAKCIJA_MS      5000
#define INTERAKCIJA_PAUZA_MS 5000
#define SAVJET_X            71.0
#define SAVJET_DNO          324.0
#define SAVJET_SIRINA       70.0
#define SAVJET_BLOK_MS      2000

static PlayerText:gSavjetTd[MAX_PLAYERS], bool:gSavjetIma[MAX_PLAYERS], gSavjetToken[MAX_PLAYERS], gSavjetBlok[MAX_PLAYERS], STREAMER_TAG_AREA:gSavjetZona[MAX_PLAYERS];

SavjetConnect(playerid)
{
    gSavjetIma[playerid] = false;
    gSavjetToken[playerid]++;
    gSavjetBlok[playerid] = 0;
}

SavjetMakni(playerid)
{
    gSavjetToken[playerid]++;
    if (!gSavjetIma[playerid]) return;
    PlayerTextDrawDestroy(playerid, gSavjetTd[playerid]);
    gSavjetIma[playerid] = false;
}

SavjetPromjena(playerid)
{
    SavjetMakni(playerid);
    gSavjetBlok[playerid] = GetTickCount() + SAVJET_BLOK_MS;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
    SavjetPromjena(playerid);
    VrijemeIgrac(playerid);
    return 1;
}

Savjet(playerid, const tekst[], ms = 3000)
{
    if (GetTickCount() - gSavjetBlok[playerid] < 0) return;
    new redova = 1;
    for (new p = strfind(tekst, "~n~"); p != -1; p = strfind(tekst, "~n~", false, p + 3)) redova++;
    if (gSavjetIma[playerid]) PlayerTextDrawDestroy(playerid, gSavjetTd[playerid]);
    new PlayerText:td = TdTekst(playerid, SAVJET_X, SAVJET_DNO - float(redova) * 9.0, tekst, 0xFFFFFFFF, 0.2, 0.9, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, 0x000000AA);
    PlayerTextDrawTextSize(playerid, td, 0.0, SAVJET_SIRINA);
    PlayerTextDrawShow(playerid, td);
    gSavjetTd[playerid] = td;
    gSavjetIma[playerid] = true;
    gSavjetZona[playerid] = STREAMER_TAG_AREA:0;
    SetTimerEx("SavjetSkrij", ms, false, "ii", playerid, ++gSavjetToken[playerid]);
}

forward SavjetSkrij(playerid, token);
public SavjetSkrij(playerid, token)
{
    if (token != gSavjetToken[playerid] || !gSavjetIma[playerid]) return;
    PlayerTextDrawDestroy(playerid, gSavjetTd[playerid]);
    gSavjetIma[playerid] = false;
}

static OznakaPrevedi(l, const hr[], out[], len)
{
    out[0] = EOS;
    new cijelo[TEKST_LEN];
    strcat(cijelo, TJ(l, hr));
    if (strcmp(cijelo, hr)) return strcat(out, cijelo, len);
    new red[TEKST_LEN], n = 0;
    for (new i = 0; ; i++)
    {
        if (hr[i] && hr[i] != '\n' && n < sizeof red - 1)
        {
            red[n++] = hr[i];
            continue;
        }
        red[n] = EOS;
        strcat(out, TJ(l, red), len);
        n = 0;
        if (!hr[i]) break;
        if (hr[i] == '\n') strcat(out, "\n", len);
    }
    return 1;
}

static OznakaVidljiva(STREAMER_TAG_3D_TEXT_LABEL:id, l)
{
    new igraci[MAX_PLAYERS], n = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerConnected(i) && gJezik[i] == l) igraci[n++] = i;
    if (!n) igraci[n++] = INVALID_PLAYER_ID;
    Streamer_SetArrayData(STREAMER_TYPE_3D_TEXT_LABEL, id, E_STREAMER_PLAYER_ID, igraci, n);
}

STREAMER_TAG_3D_TEXT_LABEL:OznakaStvori(const text[], color, Float:x, Float:y, Float:z, Float:drawdistance, attachedplayer = INVALID_PLAYER_ID, attachedvehicle = INVALID_VEHICLE_ID, testlos = 0, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = STREAMER_3D_TEXT_LABEL_SD, STREAMER_TAG_AREA:areaid = STREAMER_TAG_AREA:-1, priority = 0)
{
    new STREAMER_TAG_3D_TEXT_LABEL:id = CreateDynamic3DTextLabel(text, color, x, y, z, drawdistance, attachedplayer, attachedvehicle, testlos, worldid, interiorid, playerid, streamdistance, areaid, priority);
    if (playerid != -1 || _:id <= 0 || _:id >= OZNAKA_MAX) return id;
    new prijevod[BROJ_JEZIKA][TEKST_LEN], bool:razlika = false;
    for (new l = 1; l < BROJ_JEZIKA; l++)
    {
        OznakaPrevedi(l, text, prijevod[l], TEKST_LEN);
        if (strcmp(prijevod[l], text)) razlika = true;
    }
    if (!razlika) return id;
    gOznakaIma[_:id] = true;
    gOznakaKopija[_:id][0] = id;
    OznakaVidljiva(id, 0);
    for (new l = 1; l < BROJ_JEZIKA; l++)
    {
        new STREAMER_TAG_3D_TEXT_LABEL:k = CreateDynamic3DTextLabel(prijevod[l], color, x, y, z, drawdistance, attachedplayer, attachedvehicle, testlos, worldid, interiorid, -1, streamdistance, areaid, priority);
        gOznakaKopija[_:id][l] = k;
        OznakaVidljiva(k, l);
    }
    return id;
}

OznakaUnisti(STREAMER_TAG_3D_TEXT_LABEL:id)
{
    if (_:id > 0 && _:id < OZNAKA_MAX && gOznakaZona[_:id])
    {
        DestroyDynamicArea(gOznakaZona[_:id]);
        gOznakaZona[_:id] = STREAMER_TAG_AREA:0;
    }
    if (_:id > 0 && _:id < OZNAKA_MAX && gOznakaIma[_:id])
    {
        gOznakaIma[_:id] = false;
        for (new l = 1; l < BROJ_JEZIKA; l++) DestroyDynamic3DTextLabel(gOznakaKopija[_:id][l]);
    }
    return DestroyDynamic3DTextLabel(id);
}

OznakaTekst(STREAMER_TAG_3D_TEXT_LABEL:id, color, const text[])
{
    if (_:id > 0 && _:id < OZNAKA_MAX && gOznakaIma[_:id])
    {
        new prijevod[TEKST_LEN];
        for (new l = 1; l < BROJ_JEZIKA; l++)
        {
            OznakaPrevedi(l, text, prijevod, sizeof prijevod);
            UpdateDynamic3DTextLabelText(gOznakaKopija[_:id][l], color, prijevod);
        }
    }
    return UpdateDynamic3DTextLabelText(id, color, text);
}

STREAMER_TAG_AREA:InterakcijaZona(Float:x, Float:y, Float:z, Float:r, svijet = -1, interijer = -1, tip = SAVJET_ALT)
{
    new STREAMER_TAG_AREA:a = CreateDynamicSphere(x, y, z, r, svijet, interijer);
    Streamer_SetIntData(STREAMER_TYPE_AREA, a, E_STREAMER_EXTRA_ID, INTERAKCIJA_ZONA + tip);
    return a;
}

InterakcijaOznaka(STREAMER_TAG_3D_TEXT_LABEL:id, Float:r = 2.5, tip = SAVJET_ALT)
{
    if (_:id <= 0 || _:id >= OZNAKA_MAX) return;
    new Float:x, Float:y, Float:z, svijet[1], interijer[1];
    Streamer_GetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, id, E_STREAMER_X, x);
    Streamer_GetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, id, E_STREAMER_Y, y);
    Streamer_GetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, id, E_STREAMER_Z, z);
    Streamer_GetArrayData(STREAMER_TYPE_3D_TEXT_LABEL, id, E_STREAMER_WORLD_ID, svijet);
    Streamer_GetArrayData(STREAMER_TYPE_3D_TEXT_LABEL, id, E_STREAMER_INTERIOR_ID, interijer);
    gOznakaZona[_:id] = InterakcijaZona(x, y, z - 1.0, r, svijet[0], interijer[0], tip);
}

InterakcijaUsao(playerid, STREAMER_TAG_AREA:areaid)
{
    new tip = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - INTERAKCIJA_ZONA;
    if (tip < SAVJET_ALT || tip > SAVJET_GEPEK) return 0;
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 1;
    new sad = GetTickCount();
    if (sad - gSavjetBlok[playerid] < 0) return 1;
    if (areaid == gInterakcijaZadnja[playerid] && sad - gInterakcijaVrijeme[playerid] < INTERAKCIJA_PAUZA_MS) return 1;
    gInterakcijaZadnja[playerid] = areaid;
    gInterakcijaVrijeme[playerid] = sad;
    switch (tip)
    {
        case SAVJET_KUCA: Savjet(playerid, T(playerid, "~y~F~w~ - kuca"), INTERAKCIJA_MS);
        case SAVJET_GARAZA: Savjet(playerid, T(playerid, "~y~F~w~ - garaza"), INTERAKCIJA_MS);
        case SAVJET_IZLAZ: Savjet(playerid, T(playerid, "~y~F~w~ - izlaz"), INTERAKCIJA_MS);
        case SAVJET_ULAZ: Savjet(playerid, T(playerid, "~y~F~w~ - ulaz"), INTERAKCIJA_MS);
        case SAVJET_PODIGNI: Savjet(playerid, T(playerid, "~y~ALT~w~ - podigni"), INTERAKCIJA_MS);
        case SAVJET_GEPEK: Savjet(playerid, T(playerid, "~y~ALT~w~ - prtljaznik"), INTERAKCIJA_MS);
        default: Savjet(playerid, T(playerid, "~y~ALT~w~ - interakcija"), INTERAKCIJA_MS);
    }
    if (gSavjetIma[playerid]) gSavjetZona[playerid] = areaid;
    return 1;
}

InterakcijaIzasao(playerid, STREAMER_TAG_AREA:areaid)
{
    if (!gSavjetIma[playerid] || gSavjetZona[playerid] != areaid) return 0;
    SavjetMakni(playerid);
    return 1;
}

OznakeIgrac(playerid)
{
    for (new id = 1; id < OZNAKA_MAX; id++)
    {
        if (!gOznakaIma[id]) continue;
        for (new l = 0; l < BROJ_JEZIKA; l++)
        {
            Streamer_RemoveArrayData(STREAMER_TYPE_3D_TEXT_LABEL, gOznakaKopija[id][l], E_STREAMER_PLAYER_ID, playerid);
            if (gJezik[playerid] == l) Streamer_AppendArrayData(STREAMER_TYPE_3D_TEXT_LABEL, gOznakaKopija[id][l], E_STREAMER_PLAYER_ID, playerid);
        }
    }
    Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
}

#define CreateDynamic3DTextLabel OznakaStvori
#define DestroyDynamic3DTextLabel OznakaUnisti
#define UpdateDynamic3DTextLabelText OznakaTekst
