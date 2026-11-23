#define AVION_JAVNI         7
#define AVION_ATC_SLOTOVA   4
#define AVION_ATC_MAX       4
#define AVION_SLOTOVA       (AVION_JAVNI + AVION_ATC_SLOTOVA * AVION_ATC_MAX)
#define AVION_INSTANCI      (AVION_ATC_MAX + 1)
#define AVION_ATC_SVIJET    9750
#define AVION_PARKIRANO     4
#define AVION_STAJALISTA    11
#define AVION_SKIN          61
#define AVION_PISTA_MS      60000
#define AVION_POKUSAJ_MS    2000
#define AVION_PARK_MIN_MS   45000
#define AVION_PARK_MAX_MS   150000
#define AVION_PAUZA_MIN_MS  30000
#define AVION_PAUZA_MAX_MS  150000
#define ATC_PARK_MIN_MS     15000
#define ATC_PARK_MAX_MS     40000
#define ATC_PAUZA_MIN_MS    5000
#define ATC_PAUZA_MAX_MS    20000

#define SEG_ULAZ            10
#define SEG_ULAZ_HANGAR     16
#define SEG_IZLAZ           30
#define SEG_ODLAZAK         40
#define SEG_ODLAZAK_HANGAR  50
#define SEG_PRELET          70

#define AK_VELIKI           0
#define AK_SREDNJI          1
#define AK_MALI             2

#define ATC_ROK_S           120
#define ATC_RULANJE         1
#define ATC_GURANJE         2
#define ATC_POLIJETANJE     3
#define ATC_TD_X            448.0
#define ATC_TD_Y            150.0
#define ATC_TD_RED          16.0

enum
{
    AF_NISTA,
    AF_DOLAZAK,
    AF_CEKA_ULAZ,
    AF_ULAZ,
    AF_PARKIRAN,
    AF_IZLAZ,
    AF_ODLAZAK,
    AF_PRELET,
    AF_CEKA_POLIJETANJE
}

enum E_AVION_SEG { asSeg, asModel, Float:asX, Float:asY, Float:asZ, Float:asA }
static const gAvionSeg[][E_AVION_SEG] = {
    { 0, 577, -1350.000, -250.000, 360.990, 196.93 },
    { 1, 577, 1600.000, 1200.000, 360.990, 132.71 },
    { 2, 577, 900.000, -4300.000, 360.990, 60.26 },
    { 15, 577, 2105.000, -2530.000, 13.540, 32.20 },
    { 35, 577, 1930.500, -2331.000, 13.540, 90.00 },
    { 40, 577, 2030.000, -2493.000, 13.540, 260.22 },
    { 41, 577, 2030.000, -2493.000, 13.540, 260.22 },
    { 42, 577, 2030.000, -2493.000, 13.540, 260.22 },
    { 0, 592, -1350.000, -250.000, 362.215, 196.93 },
    { 1, 592, 1600.000, 1200.000, 362.215, 132.71 },
    { 2, 592, 900.000, -4300.000, 362.215, 60.26 },
    { 15, 592, 2105.000, -2530.000, 14.765, 32.20 },
    { 35, 592, 1919.300, -2331.000, 14.765, 90.00 },
    { 40, 592, 2030.000, -2493.000, 14.765, 260.22 },
    { 41, 592, 2030.000, -2493.000, 14.765, 260.22 },
    { 42, 592, 2030.000, -2493.000, 14.765, 260.22 },
    { 0, 519, -1350.000, -250.000, 262.111, 196.93 },
    { 1, 519, 1600.000, 1200.000, 262.111, 132.71 },
    { 2, 519, 900.000, -4300.000, 262.111, 60.26 },
    { 10, 519, 2105.000, -2530.000, 14.661, 32.20 },
    { 30, 519, 1525.000, -2454.000, 14.661, 0.00 },
    { 11, 519, 2105.000, -2530.000, 14.661, 32.20 },
    { 31, 519, 1604.000, -2454.000, 14.661, 0.00 },
    { 12, 519, 2105.000, -2530.000, 14.661, 32.20 },
    { 32, 519, 1680.000, -2454.000, 14.661, 0.00 },
    { 13, 519, 2105.000, -2530.000, 14.661, 32.20 },
    { 33, 519, 1765.000, -2454.000, 14.661, 0.00 },
    { 14, 519, 2105.000, -2530.000, 14.661, 32.20 },
    { 34, 519, 1911.000, -2255.000, 14.661, 90.00 },
    { 40, 519, 2030.000, -2493.000, 14.661, 260.22 },
    { 41, 519, 2030.000, -2493.000, 14.661, 260.22 },
    { 42, 519, 2030.000, -2493.000, 14.661, 260.22 },
    { 0, 553, -1350.000, -250.000, 262.349, 196.93 },
    { 1, 553, 1600.000, 1200.000, 262.349, 132.71 },
    { 2, 553, 900.000, -4300.000, 262.349, 60.26 },
    { 10, 553, 2105.000, -2530.000, 14.899, 32.20 },
    { 30, 553, 1525.000, -2456.700, 14.899, 0.00 },
    { 11, 553, 2105.000, -2530.000, 14.899, 32.20 },
    { 31, 553, 1604.000, -2456.700, 14.899, 0.00 },
    { 12, 553, 2105.000, -2530.000, 14.899, 32.20 },
    { 32, 553, 1680.000, -2456.700, 14.899, 0.00 },
    { 13, 553, 2105.000, -2530.000, 14.899, 32.20 },
    { 33, 553, 1765.000, -2456.700, 14.899, 0.00 },
    { 14, 553, 2105.000, -2530.000, 14.899, 32.20 },
    { 34, 553, 1913.700, -2255.000, 14.899, 90.00 },
    { 40, 553, 2030.000, -2493.000, 14.899, 260.22 },
    { 41, 553, 2030.000, -2493.000, 14.899, 260.22 },
    { 42, 553, 2030.000, -2493.000, 14.899, 260.22 },
    { 0, 593, -1350.000, -250.000, 261.620, 196.93 },
    { 1, 593, 1600.000, 1200.000, 261.620, 132.71 },
    { 2, 593, 900.000, -4300.000, 261.620, 60.26 },
    { 16, 593, 1585.000, -2597.000, 14.170, 270.00 },
    { 50, 593, 1617.300, -2635.100, 14.170, 180.00 },
    { 51, 593, 1617.300, -2635.100, 14.170, 180.00 },
    { 52, 593, 1617.300, -2635.100, 14.170, 180.00 },
    { 17, 593, 1585.000, -2597.000, 14.170, 270.00 },
    { 53, 593, 1682.700, -2635.100, 14.170, 180.00 },
    { 54, 593, 1682.700, -2635.100, 14.170, 180.00 },
    { 55, 593, 1682.700, -2635.100, 14.170, 180.00 },
    { 18, 593, 1585.000, -2597.000, 14.170, 270.00 },
    { 56, 593, 1754.200, -2635.100, 14.170, 180.00 },
    { 57, 593, 1754.200, -2635.100, 14.170, 180.00 },
    { 58, 593, 1754.200, -2635.100, 14.170, 180.00 },
    { 19, 593, 1585.000, -2597.000, 14.170, 270.00 },
    { 59, 593, 1822.700, -2635.100, 14.170, 180.00 },
    { 60, 593, 1822.700, -2635.100, 14.170, 180.00 },
    { 61, 593, 1822.700, -2635.100, 14.170, 180.00 },
    { 20, 593, 1585.000, -2597.000, 14.170, 270.00 },
    { 62, 593, 1889.700, -2635.100, 14.170, 180.00 },
    { 63, 593, 1889.700, -2635.100, 14.170, 180.00 },
    { 64, 593, 1889.700, -2635.100, 14.170, 180.00 },
    { 70, 593, 300.000, -3600.000, 221.620, 335.22 },
    { 71, 593, 1700.000, 1100.000, 241.620, 203.43 },
    { 0, 511, -1350.000, -250.000, 262.540, 196.93 },
    { 1, 511, 1600.000, 1200.000, 262.540, 132.71 },
    { 2, 511, 900.000, -4300.000, 262.540, 60.26 },
    { 16, 511, 1585.000, -2597.000, 15.090, 270.00 },
    { 50, 511, 1617.300, -2633.900, 15.090, 180.00 },
    { 51, 511, 1617.300, -2633.900, 15.090, 180.00 },
    { 52, 511, 1617.300, -2633.900, 15.090, 180.00 },
    { 17, 511, 1585.000, -2597.000, 15.090, 270.00 },
    { 53, 511, 1682.700, -2633.900, 15.090, 180.00 },
    { 54, 511, 1682.700, -2633.900, 15.090, 180.00 },
    { 55, 511, 1682.700, -2633.900, 15.090, 180.00 },
    { 18, 511, 1585.000, -2597.000, 15.090, 270.00 },
    { 56, 511, 1754.200, -2633.900, 15.090, 180.00 },
    { 57, 511, 1754.200, -2633.900, 15.090, 180.00 },
    { 58, 511, 1754.200, -2633.900, 15.090, 180.00 },
    { 19, 511, 1585.000, -2597.000, 15.090, 270.00 },
    { 59, 511, 1822.700, -2633.900, 15.090, 180.00 },
    { 60, 511, 1822.700, -2633.900, 15.090, 180.00 },
    { 61, 511, 1822.700, -2633.900, 15.090, 180.00 },
    { 20, 511, 1585.000, -2597.000, 15.090, 270.00 },
    { 62, 511, 1889.700, -2633.900, 15.090, 180.00 },
    { 63, 511, 1889.700, -2633.900, 15.090, 180.00 },
    { 64, 511, 1889.700, -2633.900, 15.090, 180.00 },
    { 70, 511, 300.000, -3600.000, 222.540, 335.22 },
    { 71, 511, 1700.000, 1100.000, 242.540, 203.43 },
    { 0, 513, -1350.000, -250.000, 261.654, 196.93 },
    { 1, 513, 1600.000, 1200.000, 261.654, 132.71 },
    { 2, 513, 900.000, -4300.000, 261.654, 60.26 },
    { 16, 513, 1585.000, -2597.000, 14.204, 270.00 },
    { 50, 513, 1617.300, -2636.800, 14.204, 180.00 },
    { 51, 513, 1617.300, -2636.800, 14.204, 180.00 },
    { 52, 513, 1617.300, -2636.800, 14.204, 180.00 },
    { 17, 513, 1585.000, -2597.000, 14.204, 270.00 },
    { 53, 513, 1682.700, -2636.800, 14.204, 180.00 },
    { 54, 513, 1682.700, -2636.800, 14.204, 180.00 },
    { 55, 513, 1682.700, -2636.800, 14.204, 180.00 },
    { 18, 513, 1585.000, -2597.000, 14.204, 270.00 },
    { 56, 513, 1754.200, -2636.800, 14.204, 180.00 },
    { 57, 513, 1754.200, -2636.800, 14.204, 180.00 },
    { 58, 513, 1754.200, -2636.800, 14.204, 180.00 },
    { 19, 513, 1585.000, -2597.000, 14.204, 270.00 },
    { 59, 513, 1822.700, -2636.800, 14.204, 180.00 },
    { 60, 513, 1822.700, -2636.800, 14.204, 180.00 },
    { 61, 513, 1822.700, -2636.800, 14.204, 180.00 },
    { 20, 513, 1585.000, -2597.000, 14.204, 270.00 },
    { 62, 513, 1889.700, -2636.800, 14.204, 180.00 },
    { 63, 513, 1889.700, -2636.800, 14.204, 180.00 },
    { 64, 513, 1889.700, -2636.800, 14.204, 180.00 },
    { 70, 513, 300.000, -3600.000, 221.654, 335.22 },
    { 71, 513, 1700.000, 1100.000, 241.654, 203.43 },
    { 0, 512, -1350.000, -250.000, 261.426, 196.93 },
    { 1, 512, 1600.000, 1200.000, 261.426, 132.71 },
    { 2, 512, 900.000, -4300.000, 261.426, 60.26 },
    { 16, 512, 1585.000, -2597.000, 13.976, 270.00 },
    { 50, 512, 1617.300, -2636.000, 13.976, 180.00 },
    { 51, 512, 1617.300, -2636.000, 13.976, 180.00 },
    { 52, 512, 1617.300, -2636.000, 13.976, 180.00 },
    { 17, 512, 1585.000, -2597.000, 13.976, 270.00 },
    { 53, 512, 1682.700, -2636.000, 13.976, 180.00 },
    { 54, 512, 1682.700, -2636.000, 13.976, 180.00 },
    { 55, 512, 1682.700, -2636.000, 13.976, 180.00 },
    { 18, 512, 1585.000, -2597.000, 13.976, 270.00 },
    { 56, 512, 1754.200, -2636.000, 13.976, 180.00 },
    { 57, 512, 1754.200, -2636.000, 13.976, 180.00 },
    { 58, 512, 1754.200, -2636.000, 13.976, 180.00 },
    { 19, 512, 1585.000, -2597.000, 13.976, 270.00 },
    { 59, 512, 1822.700, -2636.000, 13.976, 180.00 },
    { 60, 512, 1822.700, -2636.000, 13.976, 180.00 },
    { 61, 512, 1822.700, -2636.000, 13.976, 180.00 },
    { 20, 512, 1585.000, -2597.000, 13.976, 270.00 },
    { 62, 512, 1889.700, -2636.000, 13.976, 180.00 },
    { 63, 512, 1889.700, -2636.000, 13.976, 180.00 },
    { 64, 512, 1889.700, -2636.000, 13.976, 180.00 },
    { 70, 512, 300.000, -3600.000, 221.426, 335.22 },
    { 71, 512, 1700.000, 1100.000, 241.426, 203.43 },
    { 0, 476, -1350.000, -250.000, 261.742, 196.93 },
    { 1, 476, 1600.000, 1200.000, 261.742, 132.71 },
    { 2, 476, 900.000, -4300.000, 261.742, 60.26 },
    { 16, 476, 1585.000, -2597.000, 14.292, 270.00 },
    { 50, 476, 1617.300, -2634.800, 14.292, 180.00 },
    { 51, 476, 1617.300, -2634.800, 14.292, 180.00 },
    { 52, 476, 1617.300, -2634.800, 14.292, 180.00 },
    { 17, 476, 1585.000, -2597.000, 14.292, 270.00 },
    { 53, 476, 1682.700, -2634.800, 14.292, 180.00 },
    { 54, 476, 1682.700, -2634.800, 14.292, 180.00 },
    { 55, 476, 1682.700, -2634.800, 14.292, 180.00 },
    { 18, 476, 1585.000, -2597.000, 14.292, 270.00 },
    { 56, 476, 1754.200, -2634.800, 14.292, 180.00 },
    { 57, 476, 1754.200, -2634.800, 14.292, 180.00 },
    { 58, 476, 1754.200, -2634.800, 14.292, 180.00 },
    { 19, 476, 1585.000, -2597.000, 14.292, 270.00 },
    { 59, 476, 1822.700, -2634.800, 14.292, 180.00 },
    { 60, 476, 1822.700, -2634.800, 14.292, 180.00 },
    { 61, 476, 1822.700, -2634.800, 14.292, 180.00 },
    { 20, 476, 1585.000, -2597.000, 14.292, 270.00 },
    { 62, 476, 1889.700, -2634.800, 14.292, 180.00 },
    { 63, 476, 1889.700, -2634.800, 14.292, 180.00 },
    { 64, 476, 1889.700, -2634.800, 14.292, 180.00 },
    { 70, 476, 300.000, -3600.000, 221.742, 335.22 },
    { 71, 476, 1700.000, 1100.000, 241.742, 203.43 }
};

enum E_AVION_STAJ { asjModel, asjStaj, Float:asjX, Float:asjY, Float:asjZ, Float:asjA }
static const gAvionStaj[][E_AVION_STAJ] = {
    { 577, 5, 1930.500, -2331.000, 13.540, 90.0 },
    { 592, 5, 1919.300, -2331.000, 14.765, 90.0 },
    { 519, 0, 1525.000, -2454.000, 14.661, 0.0 },
    { 519, 1, 1604.000, -2454.000, 14.661, 0.0 },
    { 519, 2, 1680.000, -2454.000, 14.661, 0.0 },
    { 519, 3, 1765.000, -2454.000, 14.661, 0.0 },
    { 519, 4, 1911.000, -2255.000, 14.661, 90.0 },
    { 553, 0, 1525.000, -2456.700, 14.899, 0.0 },
    { 553, 1, 1604.000, -2456.700, 14.899, 0.0 },
    { 553, 2, 1680.000, -2456.700, 14.899, 0.0 },
    { 553, 3, 1765.000, -2456.700, 14.899, 0.0 },
    { 553, 4, 1913.700, -2255.000, 14.899, 90.0 },
    { 593, 6, 1617.300, -2635.100, 14.170, 180.0 },
    { 593, 7, 1682.700, -2635.100, 14.170, 180.0 },
    { 593, 8, 1754.200, -2635.100, 14.170, 180.0 },
    { 593, 9, 1822.700, -2635.100, 14.170, 180.0 },
    { 593, 10, 1889.700, -2635.100, 14.170, 180.0 },
    { 511, 6, 1617.300, -2633.900, 15.090, 180.0 },
    { 511, 7, 1682.700, -2633.900, 15.090, 180.0 },
    { 511, 8, 1754.200, -2633.900, 15.090, 180.0 },
    { 511, 9, 1822.700, -2633.900, 15.090, 180.0 },
    { 511, 10, 1889.700, -2633.900, 15.090, 180.0 },
    { 513, 6, 1617.300, -2636.800, 14.204, 180.0 },
    { 513, 7, 1682.700, -2636.800, 14.204, 180.0 },
    { 513, 8, 1754.200, -2636.800, 14.204, 180.0 },
    { 513, 9, 1822.700, -2636.800, 14.204, 180.0 },
    { 513, 10, 1889.700, -2636.800, 14.204, 180.0 },
    { 512, 6, 1617.300, -2636.000, 13.976, 180.0 },
    { 512, 7, 1682.700, -2636.000, 13.976, 180.0 },
    { 512, 8, 1754.200, -2636.000, 13.976, 180.0 },
    { 512, 9, 1822.700, -2636.000, 13.976, 180.0 },
    { 512, 10, 1889.700, -2636.000, 13.976, 180.0 },
    { 476, 6, 1617.300, -2634.800, 14.292, 180.0 },
    { 476, 7, 1682.700, -2634.800, 14.292, 180.0 },
    { 476, 8, 1754.200, -2634.800, 14.292, 180.0 },
    { 476, 9, 1822.700, -2634.800, 14.292, 180.0 },
    { 476, 10, 1889.700, -2634.800, 14.292, 180.0 }
};

static const gAvionModeli[] = { 577, 592, 519, 553, 519, 553, 593, 511, 513, 512, 476 };
static const gAtcZahtjevi[][] = { "", "rulanje do parkinga", "guranje i rulanje", "polijetanje" };
static const gAtcStanja[][] = { "slobodno", "na prilazu", "ceka rulanje", "rula do parkinga", "parkiran", "guranje i rulanje", "polijece", "prelijece", "ceka polijetanje" };

static gAvionNpc[AVION_SLOTOVA], gAvionVozilo[AVION_SLOTOVA], gAvionModel[AVION_SLOTOVA], gAvionFaza[AVION_SLOTOVA], gAvionStajaliste[AVION_SLOTOVA], gAvionRuta[AVION_SLOTOVA], gAvionToken[AVION_SLOTOVA];
static bool:gAvionOdobreno[AVION_SLOTOVA], gAvionZahtjev[AVION_SLOTOVA], gAvionZahtjevOd[AVION_SLOTOVA];
static gInstVlasnik[AVION_INSTANCI], gInstJuzna[AVION_INSTANCI], gInstSjeverna[AVION_INSTANCI], gInstZauzeto[AVION_INSTANCI][AVION_STAJALISTA], gInstOdobreno[AVION_INSTANCI];
static gAtcInst[MAX_PLAYERS], PlayerText:gAtcTd[MAX_PLAYERS][AVION_ATC_SLOTOVA + 3];

static AvionInst(i)
{
    return i < AVION_JAVNI ? 0 : 1 + (i - AVION_JAVNI) / AVION_ATC_SLOTOVA;
}

static AvionPrvi(k)
{
    return k ? AVION_JAVNI + (k - 1) * AVION_ATC_SLOTOVA : 0;
}

static AvionZadnji(k)
{
    return k ? AvionPrvi(k) + AVION_ATC_SLOTOVA : AVION_JAVNI;
}

static bool:AvionInstAktivna(k)
{
    return !k || gInstVlasnik[k] != 0;
}

static AvionKlasa(model)
{
    switch (model)
    {
        case 577, 592: return AK_VELIKI;
        case 519, 553: return AK_SREDNJI;
    }
    return AK_MALI;
}

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

static AvionSlobodnoStajaliste(k, model)
{
    new slobodna[AVION_STAJALISTA], n = 0;
    for (new s = 0; s < AVION_STAJALISTA; s++) if (!gInstZauzeto[k][s] && AvionStajIndeks(model, s) != -1) slobodna[n++] = s;
    return n ? slobodna[random(n)] : -1;
}

static AvionSegUlaz(i)
{
    new s = gAvionStajaliste[i];
    return s < 6 ? SEG_ULAZ + s : SEG_ULAZ_HANGAR + s - 6;
}

static AvionPusti(i)
{
    new id = i + 1, k = AvionInst(i);
    if (gInstJuzna[k] == id) gInstJuzna[k] = 0;
    if (gInstSjeverna[k] == id) gInstSjeverna[k] = 0;
    for (new s = 0; s < AVION_STAJALISTA; s++) if (gInstZauzeto[k][s] == id) gInstZauzeto[k][s] = 0;
}

static AvionCekaj(i, ms)
{
    SetTimerEx("AvionKorak", ms, false, "ii", i, gAvionToken[i]);
}

static AvionParkVrijeme(i)
{
    return AvionInst(i) ? ATC_PARK_MIN_MS + random(ATC_PARK_MAX_MS - ATC_PARK_MIN_MS) : AVION_PARK_MIN_MS + random(AVION_PARK_MAX_MS - AVION_PARK_MIN_MS);
}

static AvionPauza(i)
{
    return AvionInst(i) ? ATC_PAUZA_MIN_MS + random(ATC_PAUZA_MAX_MS - ATC_PAUZA_MIN_MS) : AVION_PAUZA_MIN_MS + random(AVION_PAUZA_MAX_MS - AVION_PAUZA_MIN_MS);
}

InitAvioni()
{
    ZoneAerodromi();
    for (new i = 0; i < AVION_SLOTOVA; i++) gAvionNpc[i] = INVALID_LIK;
    for (new i = 0; i < AVION_JAVNI; i++)
    {
        gAvionToken[i]++;
        if (i < AVION_PARKIRANO) SetTimerEx("AvionParkirani", 5000 + i * 3000, false, "ii", i, gAvionToken[i]);
        else AvionCekaj(i, 20000 + random(AVION_PAUZA_MAX_MS));
    }
}

static bool:AvionPilot(i)
{
    new Float:x, Float:y, Float:z, k = AvionInst(i);
    GetVehiclePos(gAvionVozilo[i], x, y, z);
    new lik = Lik_Stvori(AVION_SKIN, x, y, z, 0.0, k ? AVION_ATC_SVIJET + k : 0);
    if (lik == INVALID_LIK) return false;
    gAvionNpc[i] = lik;
    Lik_Neranjiv(lik, true);
    SetVehicleParamsEx(gAvionVozilo[i], 1, 1, 0, 0, 0, 0, 0);
    Lik_UVozilo(lik, gAvionVozilo[i], 0);
    SetTimerEx("AvionKreni", 1000, false, "ii", i, gAvionToken[i]);
    return true;
}

static AvionBezPilota(i)
{
    new lik = gAvionNpc[i];
    if (lik == INVALID_LIK) return;
    gAvionNpc[i] = INVALID_LIK;
    Lik_Ukloni(lik);
}

static bool:AvionStvori(i, model, Float:x, Float:y, Float:z, Float:a, bool:pilot = true)
{
    new v = CreateVehicle(model, x, y, z, a, -1, -1, -1);
    if (v == INVALID_VEHICLE_ID) return false;
    new k = AvionInst(i);
    SetVehicleVirtualWorld(v, k ? AVION_ATC_SVIJET + k : 0);
    gAvionVozilo[i] = v;
    gAvionModel[i] = model;
    gAvionNpc[i] = INVALID_LIK;
    if (pilot && !AvionPilot(i))
    {
        DestroyVehicle(v);
        gAvionVozilo[i] = 0;
        return false;
    }
    return true;
}

static AvionPoleti(i, seg)
{
    gAvionRuta[i] = seg;
    if (gAvionNpc[i] != INVALID_LIK) AvionSnimka(i, seg);
    else if (!AvionPilot(i)) AvionKraj(i);
}

forward AvionSidi(i, token);
public AvionSidi(i, token)
{
    if (token == gAvionToken[i] && gAvionFaza[i] == AF_PARKIRAN) AvionBezPilota(i);
}

forward AvionParkirani(i, token);
public AvionParkirani(i, token)
{
    new k = AvionInst(i);
    if (token != gAvionToken[i] || !AvionInstAktivna(k)) return;
    new model = gAvionModeli[random(sizeof gAvionModeli)], s = AvionSlobodnoStajaliste(k, model), j = s < 0 ? -1 : AvionStajIndeks(model, s);
    if (j < 0 || !AvionStvori(i, model, gAvionStaj[j][asjX], gAvionStaj[j][asjY], gAvionStaj[j][asjZ], gAvionStaj[j][asjA], false))
    {
        AvionCekaj(i, AvionPauza(i));
        return;
    }
    gInstZauzeto[k][s] = i + 1;
    gAvionStajaliste[i] = s;
    gAvionFaza[i] = AF_PARKIRAN;
    AvionCekaj(i, AvionParkVrijeme(i));
}

static AvionNovi(i)
{
    new k = AvionInst(i), model = gAvionModeli[random(sizeof gAvionModeli)], s = AvionSlobodnoStajaliste(k, model);
    if (AvionKlasa(model) == AK_MALI && (s < 0 || (!k && random(4) == 0)))
    {
        new j = AvionSegIndeks(SEG_PRELET + random(2), model);
        if (j < 0 || !AvionStvori(i, model, gAvionSeg[j][asX], gAvionSeg[j][asY], gAvionSeg[j][asZ], gAvionSeg[j][asA]))
        {
            AvionCekaj(i, AvionPauza(i));
            return;
        }
        gAvionRuta[i] = gAvionSeg[j][asSeg];
        gAvionFaza[i] = AF_PRELET;
        return;
    }
    if (gInstJuzna[k] || s < 0)
    {
        AvionCekaj(i, AVION_POKUSAJ_MS * 5);
        return;
    }
    new j = AvionSegIndeks(random(3), model);
    if (j < 0 || !AvionStvori(i, model, gAvionSeg[j][asX], gAvionSeg[j][asY], gAvionSeg[j][asZ], gAvionSeg[j][asA]))
    {
        AvionCekaj(i, AvionPauza(i));
        return;
    }
    gInstJuzna[k] = i + 1;
    gInstZauzeto[k][s] = i + 1;
    gAvionStajaliste[i] = s;
    gAvionRuta[i] = gAvionSeg[j][asSeg];
    gAvionFaza[i] = AF_DOLAZAK;
}

static AvionUgasi(i)
{
    AvionPusti(i);
    gAvionZahtjev[i] = 0;
    gAvionOdobreno[i] = false;
    gAvionToken[i]++;
    gAvionFaza[i] = AF_NISTA;
    if (!gAvionVozilo[i]) return;
    AvionBezPilota(i);
    DestroyVehicle(gAvionVozilo[i]);
    gAvionVozilo[i] = 0;
}

static AvionKraj(i)
{
    AvionUgasi(i);
    if (AvionInstAktivna(AvionInst(i))) AvionCekaj(i, AvionPauza(i));
}

static AvionSnimka(i, seg)
{
    new ime[16];
    format(ime, sizeof ime, "av_%d_%d", seg, gAvionModel[i]);
    if (!Lik_Snimka(gAvionNpc[i], ime)) AvionKraj(i);
}

static AvionIme(model, ime[], size = sizeof (ime))
{
    switch (model)
    {
        case 577: format(ime, size, "AT-400");
        case 592: format(ime, size, "Andromada");
        case 519: format(ime, size, "Shamal");
        case 553: format(ime, size, "Nevada");
        case 593: format(ime, size, "Dodo");
        case 511: format(ime, size, "Beagle");
        case 513: format(ime, size, "Stuntplane");
        case 512: format(ime, size, "Cropduster");
        default: format(ime, size, "Rustler");
    }
}

static bool:AvionDozvola(i, zahtjev)
{
    new k = AvionInst(i);
    if (!k || gAvionOdobreno[i] || (gAvionZahtjev[i] && gettime() - gAvionZahtjevOd[i] > ATC_ROK_S))
    {
        gAvionOdobreno[i] = false;
        gAvionZahtjev[i] = 0;
        return true;
    }
    if (!gAvionZahtjev[i])
    {
        gAvionZahtjev[i] = zahtjev;
        gAvionZahtjevOd[i] = gettime();
        PlayerPlaySound(gInstVlasnik[k] - 1, 1058, 0.0, 0.0, 0.0);
    }
    return false;
}

AtcNaSmjeni(playerid)
{
    return gAtcInst[playerid] ? 1 : 0;
}

static PlayerText:AtcTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:vel)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, td, vel, vel * 4.0);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawBackgroundColour(playerid, td, 0x000000AA);
    return td;
}

static AtcTdOsvjezi(playerid)
{
    new k = gAtcInst[playerid], red[96], ime[16];
    for (new r = 0; r < AVION_ATC_SLOTOVA; r++)
    {
        new i = AvionPrvi(k) + r;
        if (!gAvionVozilo[i]) format(red, sizeof red, "%d. %s", i + 1, T(playerid, gAtcStanja[0]));
        else
        {
            AvionIme(gAvionModel[i], ime);
            if (gAvionZahtjev[i]) format(red, sizeof red, "~y~%d. %s: %s ~g~[%s]", i + 1, ime, T(playerid, gAtcZahtjevi[gAvionZahtjev[i]]), T(playerid, "ODOBRI"));
            else format(red, sizeof red, "%d. %s: %s", i + 1, ime, T(playerid, gAtcStanja[gAvionFaza[i]]));
        }
        PlayerTextDrawSetString(playerid, gAtcTd[playerid][2 + r], red);
    }
    PlayerTextDrawSetString(playerid, gAtcTd[playerid][2 + AVION_ATC_SLOTOVA], T(playerid, "Odobreno: %d~n~/toranj - kursor, ESC - kamera"), gInstOdobreno[k]);
}

static AtcTdPokazi(playerid)
{
    gAtcTd[playerid][0] = TdSlika(playerid, ATC_TD_X - 4.0, ATC_TD_Y - 4.0, 192.0, 28.0 + ATC_TD_RED * AVION_ATC_SLOTOVA + 26.0, 0x0B1A2ACC);
    gAtcTd[playerid][1] = AtcTekst(playerid, ATC_TD_X, ATC_TD_Y, T(playerid, "Kontrolni toranj"), 0x5B8DBFFF, 0.3);
    for (new r = 0; r < AVION_ATC_SLOTOVA; r++)
    {
        new PlayerText:td = AtcTekst(playerid, ATC_TD_X, ATC_TD_Y + 22.0 + ATC_TD_RED * r, "_", 0xDDDDDDFF, 0.22);
        PlayerTextDrawTextSize(playerid, td, ATC_TD_X + 186.0, ATC_TD_RED - 2.0);
        PlayerTextDrawSetSelectable(playerid, td, true);
        gAtcTd[playerid][2 + r] = td;
    }
    gAtcTd[playerid][2 + AVION_ATC_SLOTOVA] = AtcTekst(playerid, ATC_TD_X, ATC_TD_Y + 26.0 + ATC_TD_RED * AVION_ATC_SLOTOVA, "_", 0xAAAAAAFF, 0.2);
    for (new t = 0; t < sizeof gAtcTd[]; t++) PlayerTextDrawShow(playerid, gAtcTd[playerid][t]);
    AtcTdOsvjezi(playerid);
}

static AtcTdSakrij(playerid)
{
    for (new t = 0; t < sizeof gAtcTd[]; t++) PlayerTextDrawDestroy(playerid, gAtcTd[playerid][t]);
    CancelSelectTextDraw(playerid);
}

AvionAtcTick()
{
    for (new k = 1; k < AVION_INSTANCI; k++) if (gInstVlasnik[k]) AtcTdOsvjezi(gInstVlasnik[k] - 1);
}

bool:AtcPocni(playerid)
{
    new k = 1;
    while (k < AVION_INSTANCI && gInstVlasnik[k]) k++;
    if (k == AVION_INSTANCI) return false;
    gInstVlasnik[k] = playerid + 1;
    gInstJuzna[k] = 0;
    gInstSjeverna[k] = 0;
    gInstOdobreno[k] = 0;
    for (new s = 0; s < AVION_STAJALISTA; s++) gInstZauzeto[k][s] = 0;
    gAtcInst[playerid] = k;
    SetPlayerVirtualWorld(playerid, AVION_ATC_SVIJET + k);
    for (new i = AvionPrvi(k), n = 0; i < AvionZadnji(k); i++, n++)
    {
        AvionUgasi(i);
        if (n < 2) SetTimerEx("AvionParkirani", 2000 + n * 2000, false, "ii", i, gAvionToken[i]);
        else AvionCekaj(i, 5000 + n * 8000);
    }
    AtcTdPokazi(playerid);
    return true;
}

AtcKraj(playerid)
{
    new k = gAtcInst[playerid];
    if (!k) return;
    gAtcInst[playerid] = 0;
    gInstVlasnik[k] = 0;
    for (new i = AvionPrvi(k); i < AvionZadnji(k); i++) AvionUgasi(i);
    if (!IsPlayerConnected(playerid)) return;
    SetPlayerVirtualWorld(playerid, 0);
    AtcTdSakrij(playerid);
}

bool:AtcKlik(playerid, PlayerText:td)
{
    new k = gAtcInst[playerid];
    if (!k) return false;
    for (new r = 0; r < AVION_ATC_SLOTOVA; r++)
    {
        if (td != gAtcTd[playerid][2 + r]) continue;
        new i = AvionPrvi(k) + r;
        if (!gAvionVozilo[i] || !gAvionZahtjev[i]) return true;
        gAvionOdobreno[i] = true;
        gInstOdobreno[k]++;
        AtcZarada(playerid);
        PlayerPlaySound(playerid, 1083, 0.0, 0.0, 0.0);
        AvionKorak(i, gAvionToken[i]);
        AtcTdOsvjezi(playerid);
        return true;
    }
    return false;
}

CMD:toranj(playerid, params[])
{
    if (!AtcNaSmjeni(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste na smjeni u kontrolnom tornju."));
    SelectTextDraw(playerid, 0x5B8DBFFF);
    return 1;
}

CMD:atc(playerid, params[]) return cmd_toranj(playerid, params);

forward AvionKorak(i, token);
public AvionKorak(i, token)
{
    new k = AvionInst(i);
    if (token != gAvionToken[i] || !AvionInstAktivna(k)) return;
    new id = i + 1, bool:mali = AvionKlasa(gAvionModel[i]) == AK_MALI;
    switch (gAvionFaza[i])
    {
        case AF_NISTA: AvionNovi(i);
        case AF_CEKA_ULAZ:
        {
            if ((!mali && gInstSjeverna[k]) || !AvionDozvola(i, ATC_RULANJE))
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            if (!mali)
            {
                gInstSjeverna[k] = id;
                gInstJuzna[k] = 0;
            }
            gAvionFaza[i] = AF_ULAZ;
            AvionSnimka(i, AvionSegUlaz(i));
        }
        case AF_PARKIRAN:
        {
            if (gInstJuzna[k] || (!mali && gInstSjeverna[k]) || !AvionDozvola(i, mali ? ATC_POLIJETANJE : ATC_GURANJE))
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            gInstJuzna[k] = id;
            if (mali)
            {
                gInstZauzeto[k][gAvionStajaliste[i]] = 0;
                gAvionFaza[i] = AF_ODLAZAK;
                AvionPoleti(i, SEG_ODLAZAK_HANGAR + (gAvionStajaliste[i] - 6) * 3 + random(3));
                SetTimerEx("AvionPistaSlobodna", AVION_PISTA_MS, false, "ii", i, gAvionToken[i]);
                return;
            }
            gInstSjeverna[k] = id;
            gAvionFaza[i] = AF_IZLAZ;
            AvionPoleti(i, SEG_IZLAZ + gAvionStajaliste[i]);
        }
        case AF_CEKA_POLIJETANJE:
        {
            if (!AvionDozvola(i, ATC_POLIJETANJE))
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            gAvionFaza[i] = AF_ODLAZAK;
            AvionSnimka(i, SEG_ODLAZAK + random(3));
            SetTimerEx("AvionPistaSlobodna", AVION_PISTA_MS, false, "ii", i, gAvionToken[i]);
        }
    }
}

forward AvionPistaSlobodna(i, token);
public AvionPistaSlobodna(i, token)
{
    new k = AvionInst(i);
    if (token == gAvionToken[i] && gInstJuzna[k] == i + 1) gInstJuzna[k] = 0;
}

forward AvionKreni(i, token);
public AvionKreni(i, token)
{
    if (token != gAvionToken[i] || !gAvionVozilo[i]) return;
    AvionSnimka(i, gAvionRuta[i]);
}

AvionPlaybackEnd(lik)
{
    new i = -1;
    for (new j = 0; j < AVION_SLOTOVA; j++) if (gAvionNpc[j] == lik) i = j;
    if (i < 0 || lik == INVALID_LIK) return 0;
    new k = AvionInst(i);
    switch (gAvionFaza[i])
    {
        case AF_DOLAZAK:
        {
            gAvionFaza[i] = AF_CEKA_ULAZ;
            AvionKorak(i, gAvionToken[i]);
        }
        case AF_ULAZ:
        {
            if (gInstSjeverna[k] == i + 1) gInstSjeverna[k] = 0;
            if (gInstJuzna[k] == i + 1) gInstJuzna[k] = 0;
            gAvionFaza[i] = AF_PARKIRAN;
            AvionCekaj(i, AvionParkVrijeme(i));
            SetTimerEx("AvionSidi", 500, false, "ii", i, gAvionToken[i]);
        }
        case AF_IZLAZ:
        {
            gInstSjeverna[k] = 0;
            gInstZauzeto[k][gAvionStajaliste[i]] = 0;
            gAvionFaza[i] = AF_CEKA_POLIJETANJE;
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
