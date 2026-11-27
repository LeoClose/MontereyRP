#define AVION_JAVNI         7
#define AVION_ATC_SLOTOVA   6
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
#define ATC_PARK_MIN_MS     8000
#define ATC_PARK_MAX_MS     25000
#define ATC_PAUZA_MIN_MS    3000
#define ATC_PAUZA_MAX_MS    10000

#define SEG_ULAZ            10
#define SEG_ULAZ_HANGAR     16
#define SEG_IZLAZ           30
#define SEG_ODLAZAK         40
#define SEG_ODLAZAK_HANGAR  50
#define SEG_PRELET          70

#define AK_VELIKI           0
#define AK_SREDNJI          1
#define AK_MALI             2

#define ATC_ROK_S           40
#define ATC_HITNO_ROK_S     25
#define ATC_HITNO_POSTO     20
#define ATC_INCIDENATA      3
#define ATC_KASNJENJA       3
#define ATC_KAZNA_S         300
#define ATC_KAZNI           32
#define ATC_ODOBRENJE       90
#define ATC_RULANJE         1
#define ATC_GURANJE         2
#define ATC_POLIJETANJE     3
#define ATC_SLIJETANJE      4
#define ATC_TD_X            30.0
#define ATC_TD_Y            110.0
#define ATC_TD_W            320.0
#define ATC_TD_RED          14.0

enum { ATD_POZADINA, ATD_NASLOV, ATD_TRAKA, ATD_PODNASLOV, ATD_DNO, ATD_INFO, ATD_POMOC, ATD_ODOBRI, ATD_CEKAJ, ATD_RED }

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
    AF_CEKA_POLIJETANJE,
    AF_PRILAZ
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
static const gAtcZahtjevi[][] = { "", "trazi rulanje do parkinga", "trazi guranje i rulanje", "trazi polijetanje", "trazi slijetanje" };
static const gAtcStanja[][] = { "slobodno", "slijece", "ceka rulanje", "rula do parkinga", "parkiran", "guranje i rulanje", "polijece", "prelijece", "ceka polijetanje", "na prilazu" };

static gAvionNpc[AVION_SLOTOVA], gAvionVozilo[AVION_SLOTOVA], gAvionModel[AVION_SLOTOVA], gAvionFaza[AVION_SLOTOVA], gAvionStajaliste[AVION_SLOTOVA], gAvionRuta[AVION_SLOTOVA], gAvionToken[AVION_SLOTOVA];
static bool:gAvionOdobreno[AVION_SLOTOVA], gAvionZahtjev[AVION_SLOTOVA], gAvionZahtjevOd[AVION_SLOTOVA];
static gInstVlasnik[AVION_INSTANCI], gInstJuzna[AVION_INSTANCI], gInstSjeverna[AVION_INSTANCI], gInstZauzeto[AVION_INSTANCI][AVION_STAJALISTA], gInstOdobreno[AVION_INSTANCI];
static gAtcInst[MAX_PLAYERS], PlayerText:gAtcTd[MAX_PLAYERS][ATD_RED + AVION_ATC_SLOTOVA * 3];
static const gAvionPrilazOd[3][2] = { { 90000, 55000 }, { 124000, 80000 }, { 48000, 24000 } };
static gAtcKaznaRacun[ATC_KAZNI], gAtcKaznaDo[ATC_KAZNI];
static gAvionZnak[AVION_SLOTOVA], bool:gAvionHitno[AVION_SLOTOVA], bool:gAvionDrzi[AVION_SLOTOVA], bool:gAvionSam[AVION_SLOTOVA];
static gAtcIdx[MAX_PLAYERS], bool:gAtcMis[MAX_PLAYERS], gAtcStrana[MAX_PLAYERS], gAtcSmjer[MAX_PLAYERS], gAtcPonovi[MAX_PLAYERS], gAtcIncidenti[MAX_PLAYERS], gAtcKasni[MAX_PLAYERS], gAtcZaradaSmjena[MAX_PLAYERS];

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
    AvionNoviZnak(i);
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
        AvionNoviZnak(i);
        return;
    }
    if (k && s >= 0)
    {
        new j = AvionSegIndeks(random(3), model);
        if (j < 0)
        {
            AvionCekaj(i, AvionPauza(i));
            return;
        }
        gInstZauzeto[k][s] = i + 1;
        gAvionStajaliste[i] = s;
        gAvionModel[i] = model;
        gAvionRuta[i] = gAvionSeg[j][asSeg];
        gAvionFaza[i] = AF_PRILAZ;
        gAvionHitno[i] = random(100) < ATC_HITNO_POSTO;
        AvionNoviZnak(i);
        AvionKorak(i, gAvionToken[i]);
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
    gAvionHitno[i] = gAvionDrzi[i] = gAvionSam[i] = false;
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
    new ime[16], od = 0;
    format(ime, sizeof ime, "av_%d_%d", seg, gAvionModel[i]);
    if (AvionInst(i) && gAvionFaza[i] == AF_DOLAZAK && seg >= 0 && seg < 3) od = gAvionPrilazOd[seg][AvionKlasa(gAvionModel[i]) == AK_VELIKI ? 1 : 0];
    if (!Lik_Snimka(gAvionNpc[i], ime, od)) AvionKraj(i);
}

AtcKazna(playerid)
{
    for (new k = 0; k < ATC_KAZNI; k++) if (gAtcKaznaRacun[k] == gAccountId[playerid] && gAtcKaznaDo[k] > gettime()) return gAtcKaznaDo[k] - gettime();
    return 0;
}

static AtcSmijeni(playerid)
{
    new k = 0;
    for (new j = 0; j < ATC_KAZNI; j++)
    {
        if (gAtcKaznaRacun[j] == gAccountId[playerid] || gAtcKaznaDo[j] <= gettime())
        {
            k = j;
            break;
        }
        if (gAtcKaznaDo[j] < gAtcKaznaDo[k]) k = j;
    }
    gAtcKaznaRacun[k] = gAccountId[playerid];
    gAtcKaznaDo[k] = gettime() + ATC_KAZNA_S;
    AtcKraj(playerid);
    ObavijestGreska(playerid, T(playerid, "Smijenjeni ste zbog previse incidenata. Ponovno mozete raditi u tornju za 5 minuta."));
}

static bool:AtcIncident(playerid)
{
    if (++gAtcIncidenti[playerid] < ATC_INCIDENATA) return false;
    AtcSmijeni(playerid);
    return true;
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

static bool:AvionSlobodno(i, zahtjev)
{
    new k = AvionInst(i), id = i + 1, bool:mali = AvionKlasa(gAvionModel[i]) == AK_MALI;
    switch (zahtjev)
    {
        case ATC_SLIJETANJE: return !gInstJuzna[k];
        case ATC_RULANJE: return mali || !gInstSjeverna[k];
        case ATC_GURANJE: return !gInstJuzna[k] && (mali || !gInstSjeverna[k]);
        case ATC_POLIJETANJE: return !gInstJuzna[k] || gInstJuzna[k] == id;
    }
    return true;
}

static bool:AvionTrazi(i, zahtjev)
{
    new k = AvionInst(i);
    if (!k || gAvionSam[i])
    {
        if (!AvionSlobodno(i, zahtjev)) return false;
        gAvionSam[i] = false;
        return true;
    }
    if (gAvionOdobreno[i])
    {
        gAvionOdobreno[i] = false;
        gAvionZahtjev[i] = 0;
        return true;
    }
    if (gAvionZahtjev[i] != zahtjev)
    {
        gAvionZahtjev[i] = zahtjev;
        gAvionZahtjevOd[i] = gettime();
        gAvionDrzi[i] = false;
        PlayerPlaySound(gInstVlasnik[k] - 1, 1058, 0.0, 0.0, 0.0);
    }
    return false;
}

static AvionZnakIme(i, ime[], size = sizeof (ime))
{
    static const prefiks[][] = { "LS", "SF", "LV", "MA" };
    new model[16];
    AvionIme(gAvionModel[i], model);
    format(ime, size, "%s%03d %s", prefiks[gAvionZnak[i] / 1000], gAvionZnak[i] % 1000, model);
}

static AvionNoviZnak(i)
{
    gAvionZnak[i] = random(4) * 1000 + 100 + random(900);
}

static AtcRok(i)
{
    return (gAvionHitno[i] && gAvionZahtjev[i] == ATC_SLIJETANJE ? ATC_HITNO_ROK_S : ATC_ROK_S) - (gettime() - gAvionZahtjevOd[i]);
}

AtcNaSmjeni(playerid)
{
    return gAtcInst[playerid] ? 1 : 0;
}

static AtcRedTekst(playerid, i, lijevo[], desno[], &boja)
{
    new ime[24];
    desno[0] = EOS;
    if (gAvionFaza[i] == AF_NISTA)
    {
        format(lijevo, 96, "- %s", T(playerid, gAtcStanja[0]));
        boja = 0x7F8C8DFF;
        return;
    }
    AvionZnakIme(i, ime);
    if (gAvionZahtjev[i])
    {
        new ostalo = max(0, AtcRok(i));
        format(lijevo, 96, "%s - %s", ime, T(playerid, gAtcZahtjevi[gAvionZahtjev[i]]));
        if (gAvionHitno[i] && gAvionZahtjev[i] == ATC_SLIJETANJE) format(desno, 32, T(playerid, "HITNO %d s"), ostalo);
        else format(desno, 32, "%d s", ostalo);
        boja = gAvionHitno[i] && gAvionZahtjev[i] == ATC_SLIJETANJE ? 0xE74C3CFF : (ostalo <= 15 ? 0xE67E22FF : 0xF1C40FFF);
        return;
    }
    format(lijevo, 96, "%s - %s", ime, T(playerid, gAtcStanja[gAvionFaza[i]]));
    boja = 0xBDC3C7FF;
}

static AtcTdOsvjezi(playerid)
{
    new k = gAtcInst[playerid], lijevo[96], desno[32], boja, tekst[128];
    if (!k) return;
    format(tekst, sizeof tekst, T(playerid, "Pista: %s~w~   Rulna staza: %s"), gInstJuzna[k] ? T(playerid, "~r~zauzeta") : T(playerid, "~g~slobodna"), gInstSjeverna[k] ? T(playerid, "~r~zauzeta") : T(playerid, "~g~slobodna"));
    PlayerTextDrawSetString(playerid, gAtcTd[playerid][ATD_PODNASLOV], tekst);
    for (new r = 0; r < AVION_ATC_SLOTOVA; r++)
    {
        new i = AvionPrvi(k) + r, bool:odabran = r == gAtcIdx[playerid];
        AtcRedTekst(playerid, i, lijevo, desno, boja);
        PlayerTextDrawColour(playerid, gAtcTd[playerid][ATD_RED + r * 3], odabran ? 0xF0F0F0EE : 0x000000B4);
        PlayerTextDrawColour(playerid, gAtcTd[playerid][ATD_RED + r * 3 + 1], odabran ? 0x000000FF : boja);
        PlayerTextDrawColour(playerid, gAtcTd[playerid][ATD_RED + r * 3 + 2], odabran ? 0x000000FF : boja);
        PlayerTextDrawSetString(playerid, gAtcTd[playerid][ATD_RED + r * 3 + 1], lijevo);
        PlayerTextDrawSetString(playerid, gAtcTd[playerid][ATD_RED + r * 3 + 2], desno[0] ? desno : "_");
        for (new t = 0; t < 3; t++) PlayerTextDrawShow(playerid, gAtcTd[playerid][ATD_RED + r * 3 + t]);
    }
    format(tekst, sizeof tekst, T(playerid, "Odobreno: %d   Kasnjenja: %d   Incidenti: %d/%d   Zarada: $%d"), gInstOdobreno[k], gAtcKasni[playerid], gAtcIncidenti[playerid], ATC_INCIDENATA, gAtcZaradaSmjena[playerid]);
    PlayerTextDrawSetString(playerid, gAtcTd[playerid][ATD_INFO], tekst);
}

static AtcTdPokazi(playerid)
{
    gAtcTd[playerid][ATD_POZADINA] = TdSlika(playerid, ATC_TD_X, ATC_TD_Y, ATC_TD_W, 40.0, 0x1E5AA8F0);
    gAtcTd[playerid][ATD_NASLOV] = TdTekst(playerid, ATC_TD_X + ATC_TD_W / 2.0, ATC_TD_Y + 6.0, T(playerid, "Kontrolni toranj"), 0xFFFFFFFF, 0.6, 2.6, TEXT_DRAW_FONT_3, TEXT_DRAW_ALIGN_CENTER);
    gAtcTd[playerid][ATD_TRAKA] = TdSlika(playerid, ATC_TD_X, ATC_TD_Y + 40.0, ATC_TD_W, ATC_TD_RED, 0x000000F0);
    gAtcTd[playerid][ATD_PODNASLOV] = TdTekst(playerid, ATC_TD_X + 4.0, ATC_TD_Y + 42.0, "_", 0x5B8DBFFF, 0.2, 1.0, TEXT_DRAW_FONT_2);
    for (new r = 0; r < AVION_ATC_SLOTOVA; r++)
    {
        new Float:y = ATC_TD_Y + 40.0 + ATC_TD_RED * float(r + 1);
        gAtcTd[playerid][ATD_RED + r * 3] = TdSlika(playerid, ATC_TD_X, y, ATC_TD_W, ATC_TD_RED, 0x000000B4);
        new PlayerText:naziv = TdTekst(playerid, ATC_TD_X + 5.0, y + 2.0, "_", 0xFFFFFFFF, 0.22, 1.05, TEXT_DRAW_FONT_1);
        PlayerTextDrawTextSize(playerid, naziv, ATC_TD_X + ATC_TD_W - 2.0, ATC_TD_RED - 2.0);
        PlayerTextDrawSetSelectable(playerid, naziv, true);
        gAtcTd[playerid][ATD_RED + r * 3 + 1] = naziv;
        gAtcTd[playerid][ATD_RED + r * 3 + 2] = TdTekst(playerid, ATC_TD_X + ATC_TD_W - 5.0, y + 2.0, "_", 0xFFFFFFFF, 0.22, 1.05, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    }
    new Float:dno = ATC_TD_Y + 40.0 + ATC_TD_RED * float(AVION_ATC_SLOTOVA + 1) + 4.0;
    gAtcTd[playerid][ATD_DNO] = TdSlika(playerid, ATC_TD_X, dno, ATC_TD_W, 48.0, 0x000000D2);
    gAtcTd[playerid][ATD_INFO] = TdTekst(playerid, ATC_TD_X + 5.0, dno + 3.0, "_", 0xFFFFFFFF, 0.18, 0.9, TEXT_DRAW_FONT_1);
    gAtcTd[playerid][ATD_POMOC] = TdTekst(playerid, ATC_TD_X + ATC_TD_W / 2.0, dno + 36.0, T(playerid, "W/S: odabir  Desno: odobri  Lijevo: cekaj  Y: mis"), 0x8AA0B6FF, 0.16, 0.8, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawTextSize(playerid, gAtcTd[playerid][ATD_POMOC], 10.0, ATC_TD_W);
    PlayerTextDrawSetSelectable(playerid, gAtcTd[playerid][ATD_POMOC], true);
    gAtcTd[playerid][ATD_ODOBRI] = TdTekst(playerid, ATC_TD_X + ATC_TD_W - 60.0, dno + 17.0, T(playerid, "ODOBRI"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gAtcTd[playerid][ATD_ODOBRI], true);
    PlayerTextDrawBoxColour(playerid, gAtcTd[playerid][ATD_ODOBRI], 0x2E7D32FF);
    PlayerTextDrawTextSize(playerid, gAtcTd[playerid][ATD_ODOBRI], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gAtcTd[playerid][ATD_ODOBRI], true);
    gAtcTd[playerid][ATD_CEKAJ] = TdTekst(playerid, ATC_TD_X + 60.0, dno + 17.0, T(playerid, "CEKAJ"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gAtcTd[playerid][ATD_CEKAJ], true);
    PlayerTextDrawBoxColour(playerid, gAtcTd[playerid][ATD_CEKAJ], 0x1E5AA8FF);
    PlayerTextDrawTextSize(playerid, gAtcTd[playerid][ATD_CEKAJ], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gAtcTd[playerid][ATD_CEKAJ], true);
    for (new t = 0; t < ATD_RED; t++) PlayerTextDrawShow(playerid, gAtcTd[playerid][t]);
    new KEY:tipke, gore, strana;
    GetPlayerKeys(playerid, tipke, gore, strana);
    gAtcStrana[playerid] = strana;
    gAtcSmjer[playerid] = 0;
    gAtcMis[playerid] = false;
    gAtcIdx[playerid] = 0;
    AtcTdOsvjezi(playerid);
}

static AtcTdSakrij(playerid)
{
    for (new t = 0; t < sizeof gAtcTd[]; t++) PlayerTextDrawDestroy(playerid, gAtcTd[playerid][t]);
    if (gAtcMis[playerid]) CancelSelectTextDraw(playerid);
    gAtcMis[playerid] = false;
}

static AtcPilot(playerid, i, const poruka[], bool:greska = false)
{
    new ime[24], tekst[160];
    AvionZnakIme(i, ime);
    format(tekst, sizeof tekst, "%s: %s", ime, poruka);
    IgraObavijest(playerid, T(playerid, "Kontrolni toranj"), greska ? 0xC0392BE6 : 0x5B8DBFE6, tekst);
}

AvionAtcTick()
{
    for (new k = 1; k < AVION_INSTANCI; k++)
    {
        if (!gInstVlasnik[k]) continue;
        new playerid = gInstVlasnik[k] - 1;
        for (new i = AvionPrvi(k); i < AvionZadnji(k); i++)
        {
            if (!gAvionZahtjev[i] || AtcRok(i) > 0) continue;
            PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
            if (++gAtcKasni[playerid] % ATC_KASNJENJA == 0)
            {
                IgraObavijest(playerid, T(playerid, "Kontrolni toranj"), 0xC0392BE6, T(playerid, "Previse kasnjenja - uprava vam je upisala incident."));
                if (AtcIncident(playerid)) break;
            }
            if (gAvionFaza[i] == AF_PRILAZ)
            {
                AtcPilot(playerid, i, T(playerid, "Nismo dobili odobrenje, preusmjeravamo se na drugi aerodrom."), true);
                AvionKraj(i);
                continue;
            }
            AtcPilot(playerid, i, T(playerid, "Predugo cekamo, nastavljamo kad bude slobodno. Zalit cemo se na kasnjenje."), true);
            gAvionZahtjev[i] = 0;
            gAvionSam[i] = true;
        }
        if (gAtcInst[playerid] == k) AtcTdOsvjezi(playerid);
    }
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
    gAtcIncidenti[playerid] = gAtcKasni[playerid] = gAtcZaradaSmjena[playerid] = 0;
    SetPlayerVirtualWorld(playerid, AVION_ATC_SVIJET + k);
    for (new i = AvionPrvi(k), n = 0; i < AvionZadnji(k); i++, n++)
    {
        AvionUgasi(i);
        if (n < 2) SetTimerEx("AvionParkirani", 2000 + n * 2000, false, "ii", i, gAvionToken[i]);
        else AvionCekaj(i, 4000 + n * 7000);
    }
    TogglePlayerControllable(playerid, false);
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
    TogglePlayerControllable(playerid, true);
}

static AtcOdobri(playerid)
{
    new k = gAtcInst[playerid], i = AvionPrvi(k) + gAtcIdx[playerid], z = gAvionZahtjev[i];
    if (!z)
    {
        PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        return;
    }
    if (!AvionSlobodno(i, z))
    {
        gAvionZahtjev[i] = 0;
        PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
        if (gAvionFaza[i] == AF_PRILAZ)
        {
            AtcPilot(playerid, i, T(playerid, "Pista je zauzeta! Prekidamo prilaz i odlazimo. Prijavljujemo incident."), true);
            AvionKraj(i);
        }
        else AtcPilot(playerid, i, z == ATC_RULANJE ? T(playerid, "Rulna staza je zauzeta! Zaustavljamo se. Prijavljujemo incident.") : T(playerid, "Pista je zauzeta! Zaustavljamo se. Prijavljujemo incident."), true);
        if (AtcIncident(playerid)) return;
        AtcTdOsvjezi(playerid);
        return;
    }
    new iznos = ATC_ODOBRENJE * (gAvionHitno[i] && z == ATC_SLIJETANJE ? 2 : 1);
    if (z == ATC_SLIJETANJE) gAvionHitno[i] = false;
    gAvionOdobreno[i] = true;
    gInstOdobreno[k]++;
    gAtcZaradaSmjena[playerid] += iznos;
    AtcZarada(playerid, iznos);
    PlayerPlaySound(playerid, 1083, 0.0, 0.0, 0.0);
    AvionKorak(i, gAvionToken[i]);
    AtcTdOsvjezi(playerid);
}

static AtcCekaj(playerid)
{
    new i = AvionPrvi(gAtcInst[playerid]) + gAtcIdx[playerid];
    if (!gAvionZahtjev[i])
    {
        PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        return;
    }
    if (gAvionDrzi[i])
    {
        AtcPilot(playerid, i, T(playerid, "Ne mozemo dulje cekati, trebamo odgovor."), true);
        return;
    }
    gAvionDrzi[i] = true;
    gAvionZahtjevOd[i] = gettime();
    AtcPilot(playerid, i, T(playerid, "Razumio, cekamo."));
    AtcTdOsvjezi(playerid);
}

AtcEsc(playerid)
{
    gAtcMis[playerid] = false;
}

AtcKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gAtcInst[playerid]) return 0;
    if ((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        gAtcMis[playerid] = !gAtcMis[playerid];
        if (gAtcMis[playerid]) SelectTextDraw(playerid, 0x5B8DBFFF);
        else CancelSelectTextDraw(playerid);
        return 1;
    }
    return 0;
}

AtcUpdate(playerid)
{
    if (!gAtcInst[playerid]) return;
    new KEY:tipke, gore, strana, smjer = 0, sad = GetTickCount();
    GetPlayerKeys(playerid, tipke, gore, strana);
    if (strana != gAtcStrana[playerid])
    {
        gAtcStrana[playerid] = strana;
        if (strana == KEY_RIGHT)
        {
            AtcOdobri(playerid);
            return;
        }
        if (strana == KEY_LEFT)
        {
            AtcCekaj(playerid);
            return;
        }
    }
    if (gore < 0) smjer = -1;
    else if (gore > 0) smjer = 1;
    if (!smjer)
    {
        gAtcSmjer[playerid] = 0;
        return;
    }
    if (smjer == gAtcSmjer[playerid] && sad < gAtcPonovi[playerid]) return;
    gAtcPonovi[playerid] = sad + (smjer == gAtcSmjer[playerid] ? 120 : 350);
    gAtcSmjer[playerid] = smjer;
    gAtcIdx[playerid] = (gAtcIdx[playerid] + smjer + AVION_ATC_SLOTOVA) % AVION_ATC_SLOTOVA;
    AtcTdOsvjezi(playerid);
}

bool:AtcKlik(playerid, PlayerText:td)
{
    if (!gAtcInst[playerid]) return false;
    if (td == gAtcTd[playerid][ATD_POMOC])
    {
        gAtcMis[playerid] = false;
        CancelSelectTextDraw(playerid);
        return true;
    }
    if (td == gAtcTd[playerid][ATD_ODOBRI])
    {
        AtcOdobri(playerid);
        return true;
    }
    if (td == gAtcTd[playerid][ATD_CEKAJ])
    {
        AtcCekaj(playerid);
        return true;
    }
    for (new r = 0; r < AVION_ATC_SLOTOVA; r++)
    {
        if (td != gAtcTd[playerid][ATD_RED + r * 3 + 1]) continue;
        if (r == gAtcIdx[playerid]) AtcOdobri(playerid);
        else
        {
            gAtcIdx[playerid] = r;
            AtcTdOsvjezi(playerid);
        }
        return true;
    }
    return false;
}

CMD:toranj(playerid, params[])
{
    if (!AtcNaSmjeni(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste na smjeni u kontrolnom tornju."));
    gAtcMis[playerid] = true;
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
        case AF_PRILAZ:
        {
            if (!AvionTrazi(i, ATC_SLIJETANJE))
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            new j = AvionSegIndeks(gAvionRuta[i], gAvionModel[i]);
            if (j < 0 || !AvionStvori(i, gAvionModel[i], gAvionSeg[j][asX], gAvionSeg[j][asY], gAvionSeg[j][asZ], gAvionSeg[j][asA]))
            {
                AvionKraj(i);
                return;
            }
            gInstJuzna[k] = id;
            gAvionFaza[i] = AF_DOLAZAK;
        }
        case AF_CEKA_ULAZ:
        {
            if (!AvionTrazi(i, ATC_RULANJE))
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            if (!mali)
            {
                gInstSjeverna[k] = id;
                if (gInstJuzna[k] == id) gInstJuzna[k] = 0;
            }
            gAvionFaza[i] = AF_ULAZ;
            AvionSnimka(i, AvionSegUlaz(i));
        }
        case AF_PARKIRAN:
        {
            if (!AvionTrazi(i, mali ? ATC_POLIJETANJE : ATC_GURANJE))
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
            if (!AvionTrazi(i, ATC_POLIJETANJE))
            {
                AvionCekaj(i, AVION_POKUSAJ_MS);
                return;
            }
            gInstJuzna[k] = id;
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
