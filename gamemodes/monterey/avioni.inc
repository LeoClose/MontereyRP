#define AVION_SLOTOVA       6
#define AVION_PARKIRANO     3
#define AVION_STAJALISTA    6
#define AVION_SKIN          61
#define AVION_PISTA_MS      66000
#define AVION_POKUSAJ_MS    2000
#define AVION_PARK_MIN_MS   45000
#define AVION_PARK_MAX_MS   150000
#define AVION_PAUZA_MIN_MS  30000
#define AVION_PAUZA_MAX_MS  150000

#define SEG_DOLAZAK         0
#define SEG_ULAZ            10
#define SEG_IZLAZ           20
#define SEG_ODLAZAK         30
#define SEG_PRELET          40

enum
{
    AF_NISTA,
    AF_DOLAZAK,
    AF_CEKA_ULAZ,
    AF_ULAZ,
    AF_PARKIRAN,
    AF_IZLAZ,
    AF_CEKA_PISTU,
    AF_ODLAZAK,
    AF_PRELET
}

enum E_AVION_SEG { asSeg, asModel, Float:asX, Float:asY, Float:asZ, Float:asA }
static const gAvionSeg[][E_AVION_SEG] = {
    { 0, 577, -1350.000, -250.000, 361.100, 196.93 },
    { 1, 577, 1600.000, 1200.000, 361.100, 132.71 },
    { 2, 577, 900.000, -4300.000, 361.100, 60.26 },
    { 10, 577, 1882.000, -2540.000, 13.650, 0.00 },
    { 20, 577, 1530.000, -2430.500, 13.650, 0.00 },
    { 11, 577, 1882.000, -2540.000, 13.650, 0.00 },
    { 21, 577, 1610.000, -2430.500, 13.650, 0.00 },
    { 12, 577, 1882.000, -2540.000, 13.650, 0.00 },
    { 22, 577, 1690.000, -2430.500, 13.650, 0.00 },
    { 13, 577, 1882.000, -2540.000, 13.650, 0.00 },
    { 23, 577, 1770.000, -2430.500, 13.650, 0.00 },
    { 14, 577, 1882.000, -2540.000, 13.650, 0.00 },
    { 24, 577, 1850.000, -2430.500, 13.650, 0.00 },
    { 15, 577, 1882.000, -2540.000, 13.650, 0.00 },
    { 25, 577, 1930.000, -2430.500, 13.650, 0.00 },
    { 30, 577, 2100.000, -2551.000, 13.650, 180.00 },
    { 31, 577, 2100.000, -2551.000, 13.650, 180.00 },
    { 32, 577, 2100.000, -2551.000, 13.650, 180.00 },
    { 0, 592, -1350.000, -250.000, 362.300, 196.93 },
    { 1, 592, 1600.000, 1200.000, 362.300, 132.71 },
    { 2, 592, 900.000, -4300.000, 362.300, 60.26 },
    { 10, 592, 1882.000, -2540.000, 14.850, 0.00 },
    { 20, 592, 1530.000, -2419.300, 14.850, 0.00 },
    { 11, 592, 1882.000, -2540.000, 14.850, 0.00 },
    { 21, 592, 1610.000, -2419.300, 14.850, 0.00 },
    { 12, 592, 1882.000, -2540.000, 14.850, 0.00 },
    { 22, 592, 1690.000, -2419.300, 14.850, 0.00 },
    { 13, 592, 1882.000, -2540.000, 14.850, 0.00 },
    { 23, 592, 1770.000, -2419.300, 14.850, 0.00 },
    { 14, 592, 1882.000, -2540.000, 14.850, 0.00 },
    { 24, 592, 1850.000, -2419.300, 14.850, 0.00 },
    { 15, 592, 1882.000, -2540.000, 14.850, 0.00 },
    { 25, 592, 1930.000, -2419.300, 14.850, 0.00 },
    { 30, 592, 2100.000, -2551.000, 14.850, 180.00 },
    { 31, 592, 2100.000, -2551.000, 14.850, 180.00 },
    { 32, 592, 2100.000, -2551.000, 14.850, 180.00 },
    { 0, 519, -1350.000, -250.000, 262.200, 196.93 },
    { 1, 519, 1600.000, 1200.000, 262.200, 132.71 },
    { 2, 519, 900.000, -4300.000, 262.200, 60.26 },
    { 10, 519, 1882.000, -2540.000, 14.750, 0.00 },
    { 20, 519, 1530.000, -2411.000, 14.750, 0.00 },
    { 11, 519, 1882.000, -2540.000, 14.750, 0.00 },
    { 21, 519, 1610.000, -2411.000, 14.750, 0.00 },
    { 12, 519, 1882.000, -2540.000, 14.750, 0.00 },
    { 22, 519, 1690.000, -2411.000, 14.750, 0.00 },
    { 13, 519, 1882.000, -2540.000, 14.750, 0.00 },
    { 23, 519, 1770.000, -2411.000, 14.750, 0.00 },
    { 14, 519, 1882.000, -2540.000, 14.750, 0.00 },
    { 24, 519, 1850.000, -2411.000, 14.750, 0.00 },
    { 15, 519, 1882.000, -2540.000, 14.750, 0.00 },
    { 25, 519, 1930.000, -2411.000, 14.750, 0.00 },
    { 30, 519, 2100.000, -2551.000, 14.750, 180.00 },
    { 31, 519, 2100.000, -2551.000, 14.750, 180.00 },
    { 32, 519, 2100.000, -2551.000, 14.750, 180.00 },
    { 40, 519, 300.000, -3600.000, 222.200, 335.22 },
    { 41, 519, 1700.000, 1100.000, 242.200, 203.43 },
    { 0, 593, -1350.000, -250.000, 261.700, 196.93 },
    { 1, 593, 1600.000, 1200.000, 261.700, 132.71 },
    { 2, 593, 900.000, -4300.000, 261.700, 60.26 },
    { 10, 593, 1882.000, -2540.000, 14.250, 0.00 },
    { 20, 593, 1530.000, -2405.900, 14.250, 0.00 },
    { 11, 593, 1882.000, -2540.000, 14.250, 0.00 },
    { 21, 593, 1610.000, -2405.900, 14.250, 0.00 },
    { 12, 593, 1882.000, -2540.000, 14.250, 0.00 },
    { 22, 593, 1690.000, -2405.900, 14.250, 0.00 },
    { 13, 593, 1882.000, -2540.000, 14.250, 0.00 },
    { 23, 593, 1770.000, -2405.900, 14.250, 0.00 },
    { 14, 593, 1882.000, -2540.000, 14.250, 0.00 },
    { 24, 593, 1850.000, -2405.900, 14.250, 0.00 },
    { 15, 593, 1882.000, -2540.000, 14.250, 0.00 },
    { 25, 593, 1930.000, -2405.900, 14.250, 0.00 },
    { 30, 593, 2100.000, -2551.000, 14.250, 180.00 },
    { 31, 593, 2100.000, -2551.000, 14.250, 180.00 },
    { 32, 593, 2100.000, -2551.000, 14.250, 180.00 },
    { 40, 593, 300.000, -3600.000, 221.700, 335.22 },
    { 41, 593, 1700.000, 1100.000, 241.700, 203.43 },
    { 0, 511, -1350.000, -250.000, 262.600, 196.93 },
    { 1, 511, 1600.000, 1200.000, 262.600, 132.71 },
    { 2, 511, 900.000, -4300.000, 262.600, 60.26 },
    { 10, 511, 1882.000, -2540.000, 15.150, 0.00 },
    { 20, 511, 1530.000, -2407.100, 15.150, 0.00 },
    { 11, 511, 1882.000, -2540.000, 15.150, 0.00 },
    { 21, 511, 1610.000, -2407.100, 15.150, 0.00 },
    { 12, 511, 1882.000, -2540.000, 15.150, 0.00 },
    { 22, 511, 1690.000, -2407.100, 15.150, 0.00 },
    { 13, 511, 1882.000, -2540.000, 15.150, 0.00 },
    { 23, 511, 1770.000, -2407.100, 15.150, 0.00 },
    { 14, 511, 1882.000, -2540.000, 15.150, 0.00 },
    { 24, 511, 1850.000, -2407.100, 15.150, 0.00 },
    { 15, 511, 1882.000, -2540.000, 15.150, 0.00 },
    { 25, 511, 1930.000, -2407.100, 15.150, 0.00 },
    { 30, 511, 2100.000, -2551.000, 15.150, 180.00 },
    { 31, 511, 2100.000, -2551.000, 15.150, 180.00 },
    { 32, 511, 2100.000, -2551.000, 15.150, 180.00 },
    { 40, 511, 300.000, -3600.000, 222.600, 335.22 },
    { 41, 511, 1700.000, 1100.000, 242.600, 203.43 }
};

enum E_AVION_STAJ { asjModel, asjStaj, Float:asjX, Float:asjY, Float:asjZ }
static const gAvionStaj[][E_AVION_STAJ] = {
    { 577, 0, 1530.000, -2430.500, 13.650 },
    { 577, 1, 1610.000, -2430.500, 13.650 },
    { 577, 2, 1690.000, -2430.500, 13.650 },
    { 577, 3, 1770.000, -2430.500, 13.650 },
    { 577, 4, 1850.000, -2430.500, 13.650 },
    { 577, 5, 1930.000, -2430.500, 13.650 },
    { 592, 0, 1530.000, -2419.300, 14.850 },
    { 592, 1, 1610.000, -2419.300, 14.850 },
    { 592, 2, 1690.000, -2419.300, 14.850 },
    { 592, 3, 1770.000, -2419.300, 14.850 },
    { 592, 4, 1850.000, -2419.300, 14.850 },
    { 592, 5, 1930.000, -2419.300, 14.850 },
    { 519, 0, 1530.000, -2411.000, 14.750 },
    { 519, 1, 1610.000, -2411.000, 14.750 },
    { 519, 2, 1690.000, -2411.000, 14.750 },
    { 519, 3, 1770.000, -2411.000, 14.750 },
    { 519, 4, 1850.000, -2411.000, 14.750 },
    { 519, 5, 1930.000, -2411.000, 14.750 },
    { 593, 0, 1530.000, -2405.900, 14.250 },
    { 593, 1, 1610.000, -2405.900, 14.250 },
    { 593, 2, 1690.000, -2405.900, 14.250 },
    { 593, 3, 1770.000, -2405.900, 14.250 },
    { 593, 4, 1850.000, -2405.900, 14.250 },
    { 593, 5, 1930.000, -2405.900, 14.250 },
    { 511, 0, 1530.000, -2407.100, 15.150 },
    { 511, 1, 1610.000, -2407.100, 15.150 },
    { 511, 2, 1690.000, -2407.100, 15.150 },
    { 511, 3, 1770.000, -2407.100, 15.150 },
    { 511, 4, 1850.000, -2407.100, 15.150 },
    { 511, 5, 1930.000, -2407.100, 15.150 }
};

static const gAvionModeli[] = { 577, 592, 519, 593, 511 };

static gAvionNpc[AVION_SLOTOVA], gAvionVozilo[AVION_SLOTOVA], gAvionModel[AVION_SLOTOVA], gAvionFaza[AVION_SLOTOVA], gAvionStajaliste[AVION_SLOTOVA], gAvionRuta[AVION_SLOTOVA], gAvionToken[AVION_SLOTOVA], gNpcAvion[MAX_PLAYERS];
static gAvionJuzna, gAvionSjeverna, gAvionPrijelaz, gAvionPraz, gAvionZauzeto[AVION_STAJALISTA];

static AvionSegIndeks(seg, model)
{
    for (new k = 0; k < sizeof gAvionSeg; k++) if (gAvionSeg[k][asSeg] == seg && gAvionSeg[k][asModel] == model) return k;
    return -1;
}

static AvionStajIndeks(model, s)
{
    for (new k = 0; k < sizeof gAvionStaj; k++) if (gAvionStaj[k][asjModel] == model && gAvionStaj[k][asjStaj] == s) return k;
    return -1;
}

static bool:AvionMali(model)
{
    return model != 577 && model != 592;
}

static AvionSlobodnoStajaliste()
{
    new slobodna[AVION_STAJALISTA], n = 0;
    for (new s = 0; s < AVION_STAJALISTA; s++) if (!gAvionZauzeto[s]) slobodna[n++] = s;
    return n ? slobodna[random(n)] : -1;
}

static AvionPusti(i)
{
    new id = i + 1;
    if (gAvionJuzna == id) gAvionJuzna = 0;
    if (gAvionSjeverna == id) gAvionSjeverna = 0;
    if (gAvionPrijelaz == id) gAvionPrijelaz = 0;
    if (gAvionPraz == id) gAvionPraz = 0;
    for (new s = 0; s < AVION_STAJALISTA; s++) if (gAvionZauzeto[s] == id) gAvionZauzeto[s] = 0;
}

static AvionCekaj(i, ms)
{
    SetTimerEx("AvionKorak", ms, false, "ii", i, gAvionToken[i]);
}

InitAvioni()
{
    for (new i = 0; i < AVION_SLOTOVA; i++)
    {
        gAvionToken[i]++;
        if (i < AVION_PARKIRANO) SetTimerEx("AvionParkirani", 5000 + i * 3000, false, "i", i);
        else AvionCekaj(i, 20000 + random(AVION_PAUZA_MAX_MS));
    }
}

static bool:AvionStvori(i, model, Float:x, Float:y, Float:z, Float:a)
{
    new name[MAX_PLAYER_NAME + 1];
    format(name, sizeof name, "Kapetan_%d", i);
    new npc = NPC_Create(name);
    if (npc == INVALID_PLAYER_ID) return false;
    new v = CreateVehicle(model, x, y, z, a, -1, -1, -1);
    if (v == INVALID_VEHICLE_ID)
    {
        NPC_Destroy(npc);
        return false;
    }
    gAvionNpc[i] = npc;
    gAvionVozilo[i] = v;
    gAvionModel[i] = model;
    gNpcAvion[npc] = i + 1;
    NPC_Spawn(npc);
    return true;
}

forward AvionParkirani(i);
public AvionParkirani(i)
{
    new s = AvionSlobodnoStajaliste(), model = gAvionModeli[random(sizeof gAvionModeli)], k = AvionStajIndeks(model, s);
    if (s < 0 || k < 0 || !AvionStvori(i, model, gAvionStaj[k][asjX], gAvionStaj[k][asjY], gAvionStaj[k][asjZ], 0.0))
    {
        AvionCekaj(i, AVION_PAUZA_MIN_MS);
        return;
    }
    gAvionZauzeto[s] = i + 1;
    gAvionStajaliste[i] = s;
    gAvionFaza[i] = AF_PARKIRAN;
}

static AvionNovi(i)
{
    new model = gAvionModeli[random(sizeof gAvionModeli)];
    if (AvionMali(model) && random(4) == 0)
    {
        new k = AvionSegIndeks(SEG_PRELET + random(2), model);
        if (k < 0 || !AvionStvori(i, model, gAvionSeg[k][asX], gAvionSeg[k][asY], gAvionSeg[k][asZ], gAvionSeg[k][asA]))
        {
            AvionCekaj(i, AVION_PAUZA_MIN_MS);
            return;
        }
        gAvionRuta[i] = gAvionSeg[k][asSeg];
        gAvionFaza[i] = AF_PRELET;
        return;
    }
    new s = AvionSlobodnoStajaliste();
    if (gAvionJuzna || gAvionPrijelaz || s < 0)
    {
        AvionCekaj(i, AVION_POKUSAJ_MS * 5);
        return;
    }
    new k = AvionSegIndeks(SEG_DOLAZAK + random(3), model);
    if (k < 0 || !AvionStvori(i, model, gAvionSeg[k][asX], gAvionSeg[k][asY], gAvionSeg[k][asZ], gAvionSeg[k][asA]))
    {
        AvionCekaj(i, AVION_PAUZA_MIN_MS);
        return;
    }
    gAvionJuzna = i + 1;
    gAvionPrijelaz = i + 1;
    gAvionZauzeto[s] = i + 1;
    gAvionStajaliste[i] = s;
    gAvionRuta[i] = gAvionSeg[k][asSeg];
    gAvionFaza[i] = AF_DOLAZAK;
}

static AvionSnimka(i, seg)
{
    new ime[16];
    format(ime, sizeof ime, "av_%d_%d", seg, gAvionModel[i]);
    NPC_StartPlayback(gAvionNpc[i], ime);
}

forward AvionKorak(i, token);
public AvionKorak(i, token)
{
    if (token != gAvionToken[i]) return;
    new id = i + 1;
    switch (gAvionFaza[i])
    {
        case AF_NISTA: AvionNovi(i);
        case AF_CEKA_ULAZ:
        {
            if (gAvionSjeverna)
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            gAvionSjeverna = id;
            gAvionPrijelaz = 0;
            gAvionFaza[i] = AF_ULAZ;
            AvionSnimka(i, SEG_ULAZ + gAvionStajaliste[i]);
        }
        case AF_PARKIRAN:
        {
            if (gAvionSjeverna || gAvionPraz)
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            gAvionSjeverna = id;
            gAvionPraz = id;
            gAvionFaza[i] = AF_IZLAZ;
            AvionSnimka(i, SEG_IZLAZ + gAvionStajaliste[i]);
        }
        case AF_CEKA_PISTU:
        {
            if (gAvionJuzna)
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            gAvionJuzna = id;
            gAvionPraz = 0;
            gAvionFaza[i] = AF_ODLAZAK;
            AvionSnimka(i, SEG_ODLAZAK + random(3));
            SetTimerEx("AvionPistaSlobodna", AVION_PISTA_MS, false, "ii", i, gAvionToken[i]);
        }
    }
}

forward AvionPistaSlobodna(i, token);
public AvionPistaSlobodna(i, token)
{
    if (token == gAvionToken[i] && gAvionJuzna == i + 1) gAvionJuzna = 0;
}

AvionSpawn(npcid)
{
    new i = gNpcAvion[npcid] - 1;
    if (i < 0) return;
    NPC_SetSkin(npcid, AVION_SKIN);
    NPC_SetInvulnerable(npcid, true);
    SetVehicleParamsEx(gAvionVozilo[i], 1, 1, 0, 0, 0, 0, 0);
    NPC_PutInVehicle(npcid, gAvionVozilo[i], 0);
    SetTimerEx("AvionKreni", 1000, false, "ii", i, gAvionToken[i]);
}

forward AvionKreni(i, token);
public AvionKreni(i, token)
{
    if (token != gAvionToken[i] || !gAvionVozilo[i]) return;
    if (gAvionFaza[i] == AF_PARKIRAN) AvionCekaj(i, AVION_PARK_MIN_MS + random(AVION_PARK_MAX_MS - AVION_PARK_MIN_MS));
    else AvionSnimka(i, gAvionRuta[i]);
}

static AvionKraj(i)
{
    AvionPusti(i);
    gAvionToken[i]++;
    gNpcAvion[gAvionNpc[i]] = 0;
    NPC_Destroy(gAvionNpc[i]);
    DestroyVehicle(gAvionVozilo[i]);
    gAvionVozilo[i] = 0;
    gAvionFaza[i] = AF_NISTA;
    AvionCekaj(i, AVION_PAUZA_MIN_MS + random(AVION_PAUZA_MAX_MS - AVION_PAUZA_MIN_MS));
}

AvionPlaybackEnd(npcid)
{
    new i = gNpcAvion[npcid] - 1;
    if (i < 0) return 0;
    switch (gAvionFaza[i])
    {
        case AF_DOLAZAK:
        {
            gAvionJuzna = 0;
            gAvionFaza[i] = AF_CEKA_ULAZ;
            AvionKorak(i, gAvionToken[i]);
        }
        case AF_ULAZ:
        {
            gAvionSjeverna = 0;
            gAvionFaza[i] = AF_PARKIRAN;
            AvionCekaj(i, AVION_PARK_MIN_MS + random(AVION_PARK_MAX_MS - AVION_PARK_MIN_MS));
        }
        case AF_IZLAZ:
        {
            gAvionSjeverna = 0;
            gAvionZauzeto[gAvionStajaliste[i]] = 0;
            gAvionFaza[i] = AF_CEKA_PISTU;
            AvionKorak(i, gAvionToken[i]);
        }
        default: AvionKraj(i);
    }
    return 1;
}

AvionMrtav(v)
{
    for (new i = 0; i < AVION_SLOTOVA; i++)
    {
        if (!gAvionVozilo[i] || gAvionVozilo[i] != v) continue;
        AvionKraj(i);
        return 1;
    }
    return 0;
}

AvionEnter(playerid, vehicleid)
{
    if (IsPlayerNPC(playerid)) return 0;
    for (new i = 0; i < AVION_SLOTOVA; i++)
    {
        if (!gAvionVozilo[i] || gAvionVozilo[i] != vehicleid) continue;
        ClearAnimations(playerid);
        return 1;
    }
    return 0;
}
