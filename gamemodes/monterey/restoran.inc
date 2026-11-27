#define REST_MAX            16
#define REST_VRSTA          3
#define REST_SVIJET         3300
#define REST_GOSTIJU        4
#define REST_NARUDZBI       12
#define REST_STOLOVA        16
#define REST_PUT_MAX        12
#define REST_IZMJENA_POSTO  35
#define REST_MENI           7
#define REST_DOMET          1.6
#define REST_KASA_DOMET     2.5
#define REST_PREDAJA_DOMET  3.2
#define REST_RED_POMAK      1.5
#define REST_ULAZ_DOMET     2.0
#define REST_PLACA_SAT      200
#define REST_AKTIVAN_S      120
#define REST_STRPLJENJE     240
#define REST_IGRAC_CEKA     150
#define REST_GOST_MIN       20
#define REST_GOST_RASPON    25
#define REST_JEDE_S         15
#define REST_HODA_S         30
#define REST_IGRA_S         40
#define REST_TRAKA_MS       50
#define REST_JEDENJE_MS     4500
#define REST_HRANA_SLOT     4
#define REST_NOSI_SLOT      8
#define REST_KUTIJA         19811
#define REST_PULT           0.9
#define REST_ULAZ_POMAK     1.6

enum { RV_PIZZA, RV_BURGER, RV_CLUCKIN }
enum { PT_KASA, PT_KUPAC, PT_ROSTILJ, PT_SLAGANJE, PT_POSAO, PT_POMFRIT, PT_IZLAZ, PT_RADNO, PT_BROJ }
enum { PV_BURGER, PV_PIZZA, PV_SALATA, PV_POMFRIT, PV_PICE, PV_PILETINA }
enum { PG_NEMA, PG_DOLAZI, PG_KASA, PG_STOL_IDE, PG_STOL, PG_JEDE, PG_ODLAZI }
enum { PN_ROSTILJ, PN_POMFRIT, PN_SLAGANJE, PN_NOSI }
enum { PI_NEMA, PI_ROSTILJ, PI_POMFRIT, PI_SLAGANJE, PI_PEC, PI_PAKIRANJE }
enum { PZS_LIJEVO, PZS_DESNO, PZS_INFO, PZS_KUPI, PZS_IZADI, PZS_BROJ }

static const gRsVrstaIme[REST_VRSTA][] = { "Well Stacked Pizza", "Burger Shot", "Cluckin' Bell" };
static const gRsInt[REST_VRSTA] = { 5, 10, 9 };
static const gRsSkin[REST_VRSTA] = { 155, 205, 167 };
static const gRsBoja[REST_VRSTA] = { 0xE74C3CFF, 0xF39C12FF, 0xF1C40FFF };
static const bool:gRsStolovi[REST_VRSTA] = { true, true, false };
static const gRsMeniVrste[REST_VRSTA][REST_MENI] = { { 3, 4, 5, 6, 7, 8, -1 }, { 0, 1, 2, 6, 7, 8, -1 }, { 9, 10, 11, 6, 7, 8, -1 } };
static const gRsTockaIme[PT_BROJ][] = { "Blagajnik", "Mjesto kupca", "Rostilj", "Slaganje i pakiranje", "Zaposlenje", "Friteza", "Izlaz", "Radno mjesto iza pulta" };
static Float:gRsTocka[REST_VRSTA][PT_BROJ][4] = {
    {
        { 375.60, -117.30, 1001.49, 180.0 }, { 375.60, -119.70, 1001.49, 0.0 }, { 373.80, -114.60, 1001.49, 0.0 }, { 377.40, -115.00, 1001.49, 0.0 },
        { 369.1047, -114.7455, 1001.4995, 3.0862 }, { 375.60, -114.60, 1001.49, 0.0 }, { 372.3520, -133.3000, 1001.4922, 0.0 }, { 0.0, 0.0, 0.0, 0.0 }
    },
    {
        { 376.60, -65.85, 1001.51, 180.0 }, { 376.60, -68.20, 1001.51, 0.0 }, { 376.4793, -59.1182, 1001.5078, 92.5779 }, { 379.8015, -57.6346, 1001.5078, 359.5170 },
        { 380.4156, -62.1695, 1001.5078, 267.4921 }, { 376.4560, -61.1247, 1001.5078, 91.0112 }, { 363.11, -74.88, 1001.51, 315.0 }, { 0.0, 0.0, 0.0, 0.0 }
    },
    {
        { 368.10, -4.49, 1001.85, 180.0 }, { 368.10, -6.80, 1001.85, 0.0 }, { 366.50, -2.00, 1001.85, 0.0 }, { 371.8919, -4.1039, 1001.8589, 356.6996 },
        { 364.9724, -5.3514, 1001.8516, 1.8800 }, { 370.2030, -3.9102, 1001.8589, 1.7130 }, { 364.98, -11.61, 1001.85, 0.0 }, { 370.8183, -4.4893, 1001.8589, 178.1213 }
    }
};
static Float:gRsStol[REST_VRSTA][REST_STOLOVA][4], gRsStolDb[REST_VRSTA][REST_STOLOVA], gRsStolova[REST_VRSTA], gRsStolPut[REST_VRSTA][REST_STOLOVA];
static const gRsSjedala[REST_VRSTA] = { 12, 15, 0 };
static const Float:gRsSjedalo[REST_VRSTA][REST_STOLOVA][3] = {
    { { 372.945, -130.125, 270.000 }, { 372.945, -129.031, 270.000 }, { 372.945, -126.773, 270.000 }, { 372.945, -125.680, 270.000 }, { 372.945, -123.148, 270.000 }, { 372.945, -122.062, 270.000 }, { 374.961, -130.125, 90.000 }, { 374.961, -129.031, 90.000 }, { 374.961, -126.773, 90.000 }, { 374.961, -125.680, 90.000 }, { 374.961, -123.148, 90.000 }, { 374.961, -122.062, 90.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 } },
    { { 366.578, -74.977, 181.500 }, { 368.258, -74.977, 179.500 }, { 369.758, -74.977, 179.500 }, { 371.352, -76.656, 274.700 }, { 373.094, -76.719, 77.800 }, { 374.430, -76.602, 272.000 }, { 376.055, -76.523, 93.900 }, { 377.273, -76.633, 274.400 }, { 373.312, -73.453, 359.100 }, { 375.328, -73.453, 357.400 }, { 376.391, -73.453, 359.200 }, { 365.328, -72.094, 5.100 }, { 365.164, -70.391, 186.200 }, { 361.391, -72.133, 353.900 }, { 361.469, -70.156, 182.500 }, { 0.000, 0.000, 0.000 } },
    { { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 }, { 0.000, 0.000, 0.000 } }
};
static const gRsSjedaloPut[REST_VRSTA][REST_STOLOVA][4] = {
    { { 6, 7, 13, 3 }, { 16, 5, 21, 5 }, { 26, 6, 32, 4 }, { 36, 5, 41, 4 }, { 45, 6, 51, 5 }, { 56, 4, 60, 4 }, { 64, 3, 67, 6 }, { 73, 3, 76, 6 }, { 82, 2, 84, 8 }, { 92, 2, 94, 8 }, { 102, 3, 105, 8 }, { 113, 4, 117, 9 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 127, 3, 130, 4 }, { 134, 3, 137, 4 }, { 141, 4, 145, 4 }, { 149, 4, 153, 4 }, { 157, 5, 162, 5 }, { 167, 6, 173, 4 }, { 177, 4, 181, 6 }, { 187, 4, 191, 4 }, { 195, 7, 202, 3 }, { 205, 6, 211, 4 }, { 215, 5, 220, 4 }, { 224, 3, 227, 3 }, { 230, 3, 233, 7 }, { 240, 3, 243, 2 }, { 245, 4, 249, 2 }, { 0, 0, 0, 0 } },
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
};
static const gRsUlazPut[REST_VRSTA][2] = { { 0, 6 }, { 126, 1 }, { 0, 0 } };
static const Float:gRsPutXY[] = {
    372.300, -129.400, 372.300, -125.500, 372.500, -125.200, 375.500, -123.600, 375.600, -123.300, 375.600, -120.200, 375.600, -120.200, 375.600, -123.400,
    375.400, -123.700, 372.400, -125.300, 372.300, -125.600, 372.300, -129.700, 372.400, -129.800, 372.400, -129.800, 372.300, -129.900, 372.400, -131.700,
    375.600, -120.200, 375.600, -127.100, 375.400, -127.400, 373.200, -128.200, 372.600, -128.600, 372.600, -128.600, 372.500, -128.600, 372.300, -128.900,
    372.300, -131.600, 372.400, -131.700, 375.600, -120.200, 375.600, -123.400, 375.400, -123.700, 372.400, -125.300, 372.300, -125.600, 372.400, -126.500,
    372.400, -126.500, 372.300, -126.600, 372.300, -131.600, 372.400, -131.700, 375.600, -120.200, 375.600, -123.400, 375.400, -123.700, 372.400, -125.300,
    372.400, -125.400, 372.400, -125.400, 372.300, -125.500, 372.300, -131.600, 372.400, -131.700, 375.600, -120.200, 372.500, -121.600, 372.300, -121.900,
    372.300, -122.400, 372.500, -122.700, 372.600, -122.700, 372.600, -122.700, 372.500, -122.700, 372.300, -123.000, 372.300, -131.600, 372.400, -131.700,
    375.600, -120.200, 372.500, -121.600, 372.400, -121.700, 372.400, -121.800, 372.400, -121.800, 372.300, -121.900, 372.300, -131.600, 372.400, -131.700,
    375.600, -120.200, 375.600, -129.600, 375.500, -129.800, 375.500, -129.800, 375.600, -129.900, 375.600, -130.400, 375.400, -130.700, 374.400, -131.100,
    372.400, -131.700, 375.600, -120.200, 375.600, -128.500, 375.500, -128.700, 375.500, -128.700, 375.600, -128.800, 375.600, -130.400, 375.400, -130.700,
    374.400, -131.100, 372.400, -131.700, 375.600, -120.200, 375.600, -126.800, 375.600, -126.800, 375.600, -127.100, 375.400, -127.400, 372.700, -128.400,
    372.400, -128.700, 372.300, -129.000, 372.300, -130.600, 372.400, -131.700, 375.600, -120.200, 375.600, -125.700, 375.600, -125.700, 375.600, -127.100,
    375.400, -127.400, 373.200, -128.200, 372.400, -128.700, 372.300, -129.000, 372.300, -131.600, 372.400, -131.700, 375.600, -120.200, 375.600, -122.600,
    375.500, -122.800, 375.500, -122.800, 375.600, -122.900, 375.600, -123.500, 374.500, -124.400, 372.500, -125.200, 372.300, -125.500, 372.300, -131.600,
    372.400, -131.700, 375.600, -120.200, 375.600, -122.300, 375.400, -122.600, 375.300, -122.600, 375.300, -122.600, 375.400, -122.600, 375.600, -122.900,
    375.600, -123.500, 374.600, -124.300, 372.500, -125.200, 372.300, -125.500, 372.300, -131.600, 372.400, -131.700, 376.600, -68.800, 376.600, -68.800,
    371.700, -71.000, 367.100, -74.700, 367.100, -74.700, 367.100, -74.600, 366.800, -74.400, 364.200, -73.800, 376.600, -68.800, 372.300, -70.700,
    368.800, -74.700, 368.800, -74.700, 368.800, -74.600, 368.500, -74.400, 364.200, -73.800, 376.600, -68.800, 372.300, -70.700, 370.300, -74.600,
    370.300, -74.700, 370.300, -74.700, 370.300, -74.600, 370.000, -74.400, 364.200, -73.800, 376.600, -68.800, 372.300, -70.700, 372.100, -71.000,
    371.400, -76.100, 371.400, -76.100, 370.200, -74.500, 369.900, -74.400, 364.200, -73.800, 376.600, -68.800, 377.600, -71.000, 377.600, -72.700,
    376.800, -74.000, 373.300, -76.200, 373.300, -76.200, 373.200, -75.900, 372.900, -75.700, 370.000, -74.400, 364.200, -73.800, 376.600, -68.800,
    377.600, -71.000, 377.600, -72.700, 376.800, -74.000, 373.900, -76.200, 373.900, -76.300, 373.900, -76.300, 372.900, -75.700, 370.000, -74.400,
    364.200, -73.800, 376.600, -68.800, 377.600, -71.000, 377.600, -73.800, 376.500, -76.200, 376.500, -76.200, 376.500, -76.100, 376.300, -75.900,
    375.700, -75.700, 369.900, -74.400, 364.200, -73.800, 376.600, -68.800, 377.600, -71.000, 377.500, -75.100, 377.000, -76.100, 377.000, -76.100,
    375.700, -75.700, 369.900, -74.400, 364.200, -73.800, 376.600, -68.800, 377.600, -71.000, 377.600, -72.700, 376.800, -74.000, 376.500, -74.100,
    373.700, -74.100, 373.600, -74.000, 373.600, -74.000, 373.500, -74.100, 364.200, -73.800, 376.600, -68.800, 377.600, -71.000, 377.600, -72.700,
    376.800, -74.000, 376.500, -74.100, 375.600, -74.000, 375.600, -74.000, 375.500, -74.100, 364.300, -73.900, 364.200, -73.800, 376.600, -68.800,
    377.600, -71.000, 377.600, -72.700, 376.800, -74.000, 376.700, -74.000, 376.700, -74.000, 376.600, -74.100, 364.600, -74.000, 364.200, -73.800,
    376.600, -68.800, 369.800, -71.300, 365.700, -72.600, 365.700, -72.600, 365.700, -72.700, 364.200, -73.800, 376.600, -68.800, 369.800, -71.300,
    365.700, -70.200, 365.700, -70.200, 366.000, -70.400, 366.100, -70.700, 366.100, -72.100, 365.900, -72.400, 365.600, -72.800, 364.200, -73.800,
    376.600, -68.800, 365.400, -72.900, 362.000, -72.100, 362.000, -72.100, 364.200, -73.800, 376.600, -68.800, 365.400, -72.900, 364.100, -72.500,
    362.000, -69.900, 362.000, -69.900, 364.200, -73.800
};

static const gRsMeni[][] = { "Moo Kids Meal", "Beef Tower", "Meat Stack", "Buster", "Double D-Luxe", "Full Rack", "Salad Meal", "Pomfrit", "Sok", "Cluckin' Little Meal", "Cluckin' Big Meal", "Cluckin' Huge Meal" };
static const gRsStavkaVrsta[] = { PV_BURGER, PV_BURGER, PV_BURGER, PV_PIZZA, PV_PIZZA, PV_PIZZA, PV_SALATA, PV_POMFRIT, PV_PICE, PV_PILETINA, PV_PILETINA, PV_PILETINA };
static const gRsCijena[] = { 6, 10, 14, 7, 11, 15, 8, 4, 3, 6, 10, 14 };
static const Float:gRsZdravlje[] = { 20.0, 35.0, 50.0, 25.0, 40.0, 55.0, 20.0, 12.0, 8.0, 20.0, 35.0, 50.0 };
static const gRsModel[] = { 2703, 2703, 2703, 19580, 19580, 19580, 19811, 19811, 2647, 2703, 2703, 2703 };
static const gRsRuka[] = { 2703, 2703, 2703, 2702, 2702, 2702, 19811, 19811, 2647, 2703, 2703, 2703 };
static const gRsStrane[] = { 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2 };
static const gRsSastojak[][] = { "Donja lepinja", "Meso", "Sir", "Salata", "Rajcica", "Gornja lepinja", "Tijesto", "Umak", "Salama", "Gljive", "Preljev", "Piletina" };
static const gRsRecept[sizeof gRsMeni][7] = {
    { 0, 1, 2, 5, -1, -1, -1 },
    { 0, 1, 2, 3, 4, 5, -1 },
    { 0, 1, 2, 1, 2, 5, -1 },
    { 6, 7, 2, -1, -1, -1, -1 },
    { 6, 7, 2, 8, 9, -1, -1 },
    { 6, 7, 2, 8, 9, 2, -1 },
    { 3, 4, 2, 10, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1 },
    { 0, 11, 3, 5, -1, -1, -1 },
    { 0, 11, 2, 3, 4, 5, -1 },
    { 0, 11, 2, 11, 3, 5, -1 }
};
static const bool:gRsMozeBez[] = { false, false, true, true, true, false, false, false, true, true, true, false };
static const KEY:gRsTipka[] = { KEY_YES, KEY_NO, KEY_WALK, KEY_SPRINT };
static const gRsTipkaIme[][] = { "Y", "N", "ALT", "SPACE" };

enum E_RS { bool:rsIma, rsDb, rsVrsta, Float:rsX, Float:rsY, Float:rsZ, Float:rsA, rsAktor, rsSljedeci, rsBroj,
    STREAMER_TAG_PICKUP:rsPickup, STREAMER_TAG_PICKUP:rsPickupU, STREAMER_TAG_3D_TEXT_LABEL:rsTekst, STREAMER_TAG_3D_TEXT_LABEL:rsTekstU, STREAMER_TAG_3D_TEXT_LABEL:rsOznaka[PT_BROJ] }
static gRs[REST_MAX][E_RS];
enum E_RSG { pgLik, pgFaza, pgStol, pgOd, pgNar }
static gRsGost[REST_MAX][REST_GOSTIJU][E_RSG];
enum E_RSN { bool:pnIma, pnStanje, pnGost, pnIgrac, pnStavka, pnRadi, pnOd, pnBroj, pnBez }
static gRsNar[REST_MAX][REST_NARUDZBI][E_RSN];

static gRsSmjena[MAX_PLAYERS], gRsIgra[MAX_PLAYERS], gRsRad[MAX_PLAYERS] = { -1, ... }, gRsKorak[MAX_PLAYERS], gRsStrana[MAX_PLAYERS], gRsIgraOd[MAX_PLAYERS], gRsTimer[MAX_PLAYERS];
static gRsSlojTipka[MAX_PLAYERS][7], Float:gRsPoz[MAX_PLAYERS], Float:gRsBrzina[MAX_PLAYERS], Float:gRsZelOd[MAX_PLAYERS], Float:gRsZelDo[MAX_PLAYERS];
static PlayerText:gRsLista[MAX_PLAYERS], bool:gRsListaIma[MAX_PLAYERS], gRsListaTekst[MAX_PLAYERS][800];
static gRsSavjet[MAX_PLAYERS] = { -1, ... }, bool:gRsJede[MAX_PLAYERS], gRsApPopis[MAX_PLAYERS][REST_MAX + REST_STOLOVA], gRsApVrsta[MAX_PLAYERS];
static gRsRadio[MAX_PLAYERS], gRsAkcija[MAX_PLAYERS];
static gRsShop[MAX_PLAYERS] = { -1, ... }, gRsShopIdx[MAX_PLAYERS], PlayerText:gRsShopTd[MAX_PLAYERS][PZS_BROJ], gRsShopObj[MAX_PLAYERS] = { INVALID_OBJECT_ID, ... };

#define RsT(%0,%1) gRsTocka[gRs[%0][rsVrsta]][%1]

static bool:RestoranTablica(const ime[])
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM sqlite_master WHERE type = 'table' AND name = '%q'", ime);
    new bool:ima = DB_GetRowCount(res) != 0;
    DB_FreeResultSet(res);
    return ima;
}

InitRestoran()
{
    if (RestoranTablica("pizzerias") && !RestoranTablica("restaurants"))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE pizzerias RENAME TO restaurants"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE restaurants ADD COLUMN vrsta INTEGER NOT NULL DEFAULT 0"));
    }
    if (RestoranTablica("pizza_tables") && !RestoranTablica("restaurant_tables"))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE pizza_tables RENAME TO restaurant_tables"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE restaurant_tables ADD COLUMN vrsta INTEGER NOT NULL DEFAULT 0"));
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS restaurants (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, vrsta INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS restaurant_tables (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, vrsta INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS restaurant_points (vrsta INTEGER NOT NULL, tip INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, PRIMARY KEY (vrsta, tip))"));
    if (RestoranTablica("pizza_points"))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO restaurant_points (vrsta, tip, x, y, z, a) SELECT 0, tip, x, y, z, a FROM pizza_points WHERE tip >= 0 AND tip < %d", PT_IZLAZ));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DROP TABLE pizza_points"));
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT vrsta, tip, x, y, z, a FROM restaurant_points");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new v = DB_GetFieldInt(res, 0), t = DB_GetFieldInt(res, 1);
        if (v < 0 || v >= REST_VRSTA || t < 0 || t >= PT_BROJ) continue;
        for (new k = 0; k < 4; k++) gRsTocka[v][t][k] = DB_GetFieldFloat(res, k + 2);
    }
    DB_FreeResultSet(res);
    for (new v = 0; v < REST_VRSTA; v++)
    {
        for (new t = 0; t < gRsSjedala[v]; t++)
        {
            gRsStol[v][t][0] = gRsSjedalo[v][t][0];
            gRsStol[v][t][1] = gRsSjedalo[v][t][1];
            gRsStol[v][t][2] = gRsTocka[v][PT_KUPAC][2];
            gRsStol[v][t][3] = gRsSjedalo[v][t][2];
            gRsStolPut[v][t] = t;
        }
        gRsStolova[v] = gRsSjedala[v];
    }
    res = DB_ExecuteQuery(gDB, "SELECT id, vrsta, x, y, z, a FROM restaurant_tables ORDER BY id");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new v = DB_GetFieldInt(res, 1);
        if (v < 0 || v >= REST_VRSTA || gRsStolova[v] >= REST_STOLOVA) continue;
        gRsStolDb[v][gRsStolova[v]] = DB_GetFieldInt(res, 0);
        gRsStolPut[v][gRsStolova[v]] = -1;
        for (new k = 0; k < 4; k++) gRsStol[v][gRsStolova[v]][k] = DB_GetFieldFloat(res, k + 2);
        gRsStolova[v]++;
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, vrsta, x, y, z, a FROM restaurants ORDER BY id LIMIT %d", REST_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gRs[s][rsDb] = DB_GetFieldInt(res, 0);
        gRs[s][rsVrsta] = clamp(DB_GetFieldInt(res, 1), 0, REST_VRSTA - 1);
        gRs[s][rsX] = DB_GetFieldFloat(res, 2);
        gRs[s][rsY] = DB_GetFieldFloat(res, 3);
        gRs[s][rsZ] = DB_GetFieldFloat(res, 4);
        gRs[s][rsA] = DB_GetFieldFloat(res, 5);
        RestoranStvori(s);
    }
    DB_FreeResultSet(res);
}

RestoranVrsta(s)
{
    return s >= 0 && s < REST_MAX ? gRs[s][rsVrsta] : 0;
}

static RestoranFirma(s)
{
    return FIRMA_PIZZA + gRs[s][rsVrsta];
}

RestoranLokacija(i, &Float:x, &Float:y, &Float:z, &Float:a)
{
    if (i >= REST_MAX) return -1;
    if (!gRs[i][rsIma]) return 0;
    x = gRs[i][rsX];
    y = gRs[i][rsY];
    z = gRs[i][rsZ];
    a = gRs[i][rsA];
    return 1;
}

RestoranLokacijaIme(playerid, i, ime[], size = sizeof ime)
{
    format(ime, size, "%s", T(playerid, gRsVrstaIme[gRs[i][rsVrsta]]));
}

static RestoranNaziv(playerid, s)
{
    new ime[32];
    strcat(ime, T(playerid, gRsVrstaIme[gRs[s][rsVrsta]]));
    return ime;
}

static RestoranUnutra(s, &Float:x, &Float:y, &Float:z, &Float:a)
{
    a = RsT(s, PT_IZLAZ)[3];
    x = RsT(s, PT_IZLAZ)[0] + REST_ULAZ_POMAK * floatsin(-a, degrees);
    y = RsT(s, PT_IZLAZ)[1] + REST_ULAZ_POMAK * floatcos(-a, degrees);
    z = RsT(s, PT_IZLAZ)[2];
}

static RestoranStvori(s)
{
    gRs[s][rsIma] = true;
    gRs[s][rsBroj] = 0;
    for (new g = 0; g < REST_GOSTIJU; g++) gRsGost[s][g][pgLik] = INVALID_LIK, gRsGost[s][g][pgFaza] = PG_NEMA, gRsGost[s][g][pgStol] = -1;
    for (new n = 0; n < REST_NARUDZBI; n++) gRsNar[s][n][pnIma] = false;
    gRs[s][rsPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gRs[s][rsX], gRs[s][rsY], gRs[s][rsZ], 0, 0);
    gRs[s][rsTekst] = CreateDynamic3DTextLabel(gRsVrstaIme[gRs[s][rsVrsta]], gRsBoja[gRs[s][rsVrsta]], gRs[s][rsX], gRs[s][rsY], gRs[s][rsZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gRs[s][rsTekst], REST_ULAZ_DOMET, SAVJET_ULAZ);
    RestoranOznakeStvori(s);
    FirmaUcitaj(RestoranFirma(s), s, gRs[s][rsDb]);
}

static bool:RestoranImaStanicu(v, t)
{
    if (t != PT_ROSTILJ) return true;
    for (new m = 0; m < REST_MENI && gRsMeniVrste[v][m] != -1; m++) if (gRsStavkaVrsta[gRsMeniVrste[v][m]] == PV_BURGER) return true;
    return false;
}

static RestoranOznakeStvori(s)
{
    new svijet = REST_SVIJET + s, v = gRs[s][rsVrsta], unutra = gRsInt[v];
    gRs[s][rsPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, RsT(s, PT_IZLAZ)[0], RsT(s, PT_IZLAZ)[1], RsT(s, PT_IZLAZ)[2], svijet, unutra);
    gRs[s][rsTekstU] = CreateDynamic3DTextLabel("Izlaz", COLOR_ORG, RsT(s, PT_IZLAZ)[0], RsT(s, PT_IZLAZ)[1], RsT(s, PT_IZLAZ)[2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, unutra);
    InterakcijaOznaka(gRs[s][rsTekstU], REST_ULAZ_DOMET, SAVJET_IZLAZ);
    gRs[s][rsAktor] = CreateActor(gRsSkin[v], RsT(s, PT_KASA)[0], RsT(s, PT_KASA)[1], RsT(s, PT_KASA)[2], RsT(s, PT_KASA)[3]);
    SetActorVirtualWorld(gRs[s][rsAktor], svijet);
    SetActorInvulnerable(gRs[s][rsAktor], true);
    for (new t = 0; t < PT_BROJ; t++)
    {
        gRs[s][rsOznaka][t] = STREAMER_TAG_3D_TEXT_LABEL:0;
        if (t == PT_KUPAC || t == PT_IZLAZ || t == PT_RADNO || !RestoranImaStanicu(v, t)) continue;
        new Float:x = RsT(s, t)[0], Float:y = RsT(s, t)[1], Float:z = RsT(s, t)[2] + 1.1;
        if (t == PT_POSAO) z = RsT(s, t)[2] + 0.4;
        else if (t != PT_KASA)
        {
            x += 0.7 * floatsin(-RsT(s, t)[3], degrees);
            y += 0.7 * floatcos(-RsT(s, t)[3], degrees);
            z = RsT(s, t)[2] + 0.25;
        }
        gRs[s][rsOznaka][t] = CreateDynamic3DTextLabel(t == PT_KASA ? ("Blagajna") : t == PT_POSAO ? ("Posao\n/posao") : t == PT_SLAGANJE && v == RV_PIZZA ? ("Slaganje i pec za pizzu") : gRsTockaIme[t], gRsBoja[v], x, y, z, 6.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, unutra);
        InterakcijaOznaka(gRs[s][rsOznaka][t], t == PT_KASA ? REST_KASA_DOMET + 1.0 : REST_DOMET + 0.8);
    }
}

static RestoranOznakeMakni(s)
{
    DestroyActor(gRs[s][rsAktor]);
    DestroyDynamicPickup(gRs[s][rsPickupU]);
    DestroyDynamic3DTextLabel(gRs[s][rsTekstU]);
    for (new t = 0; t < PT_BROJ; t++) if (gRs[s][rsOznaka][t]) DestroyDynamic3DTextLabel(gRs[s][rsOznaka][t]);
}

RestoranIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - REST_SVIJET;
    return s >= 0 && s < REST_MAX && gRs[s][rsIma] && GetPlayerInterior(playerid) == gRsInt[gRs[s][rsVrsta]] ? s : -1;
}

static bool:RestoranTu(playerid, s, t, Float:r = REST_DOMET)
{
    return IsPlayerInRangeOfPoint(playerid, r, RsT(s, t)[0], RsT(s, t)[1], RsT(s, t)[2]);
}

static bool:RestoranImaRadno(s)
{
    return RsT(s, PT_RADNO)[0] != 0.0 || RsT(s, PT_RADNO)[1] != 0.0;
}

static bool:RestoranKodPosla(playerid, s, Float:r = REST_DOMET)
{
    return RestoranTu(playerid, s, PT_POSAO, r) || (gRsSmjena[playerid] == s + 1 && RestoranImaRadno(s) && RestoranTu(playerid, s, PT_RADNO, r));
}

static RestoranStanica(playerid, s)
{
    new najbliza = -1, Float:najmanje = REST_DOMET, v = gRs[s][rsVrsta];
    static const stanice[] = { PT_ROSTILJ, PT_POMFRIT, PT_SLAGANJE };
    for (new k = 0; k < sizeof stanice; k++)
    {
        new t = stanice[k];
        if (!RestoranImaStanicu(v, t)) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, RsT(s, t)[0], RsT(s, t)[1], RsT(s, t)[2]);
        if (d <= najmanje) najmanje = d, najbliza = t;
    }
    return najbliza;
}

static bool:RestoranNosi(playerid)
{
    new s = gRsSmjena[playerid] - 1, n = gRsRad[playerid];
    return s >= 0 && n >= 0 && gRsNar[s][n][pnIma] && gRsNar[s][n][pnStanje] == PN_NOSI;
}

RestoranUlaz(playerid)
{
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new s = RestoranIgraca(playerid);
    if (s != -1)
    {
        if (!RestoranTu(playerid, s, PT_IZLAZ, REST_ULAZ_DOMET)) return 0;
        if (RestoranNosi(playerid)) return ObavijestGreska(playerid, T(playerid, "Prvo predajte narudzbu koju nosite.")), 1;
        RestoranShopZatvori(playerid);
        PrebaciIgraca(playerid, 0, 0, gRs[s][rsX], gRs[s][rsY], gRs[s][rsZ], gRs[s][rsA] + 180.0);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < REST_MAX; s++)
    {
        if (!gRs[s][rsIma] || !IsPlayerInRangeOfPoint(playerid, REST_ULAZ_DOMET, gRs[s][rsX], gRs[s][rsY], gRs[s][rsZ])) continue;
        new Float:x, Float:y, Float:z, Float:a;
        RestoranUnutra(s, x, y, z, a);
        PrebaciIgraca(playerid, gRsInt[gRs[s][rsVrsta]], REST_SVIJET + s, x, y, z, a);
        return 1;
    }
    return 0;
}

static RestoranRadnika(s)
{
    new n = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gRsSmjena[i] == s + 1) n++;
    return n;
}

static RestoranRadnicima(s, const tekst[], broj, bool:greska = false)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gRsSmjena[i] != s + 1) continue;
        new poruka[144];
        format(poruka, sizeof poruka, T(i, tekst), broj);
        if (greska) ObavijestGreska(i, poruka);
        else ObavijestInfo(i, RestoranNaziv(i, s), poruka);
    }
}

static RestoranPocetnoStanje(stavka)
{
    switch (gRsStavkaVrsta[stavka])
    {
        case PV_BURGER: return PN_ROSTILJ;
        case PV_POMFRIT, PV_PILETINA: return PN_POMFRIT;
    }
    return PN_SLAGANJE;
}

static RestoranMeniBroj(v)
{
    new n = 0;
    while (n < REST_MENI && gRsMeniVrste[v][n] != -1) n++;
    return n;
}

static RestoranIzmjena(stavka)
{
    if (random(100) >= REST_IZMJENA_POSTO) return 0;
    new moze[7], n = 0, maska = 0;
    for (new k = 1; k < 7 && gRsRecept[stavka][k] != -1; k++)
    {
        new sastojak = gRsRecept[stavka][k];
        if (!gRsMozeBez[sastojak] || (gRsStavkaVrsta[stavka] == PV_PIZZA && sastojak == 2) || (maska & (1 << sastojak))) continue;
        maska |= 1 << sastojak;
        moze[n++] = sastojak;
    }
    if (!n) return 0;
    new prvi = random(n), bez = 1 << moze[prvi];
    if (n > 1 && random(4) == 0) bez |= 1 << moze[(prvi + 1 + random(n - 1)) % n];
    return bez;
}

static RestoranNovaNarudzba(s, gost, igrac, stavka)
{
    for (new n = 0; n < REST_NARUDZBI; n++)
    {
        if (gRsNar[s][n][pnIma]) continue;
        gRsNar[s][n][pnIma] = true;
        gRsNar[s][n][pnStanje] = RestoranPocetnoStanje(stavka);
        gRsNar[s][n][pnGost] = gost;
        gRsNar[s][n][pnIgrac] = igrac;
        gRsNar[s][n][pnStavka] = stavka;
        gRsNar[s][n][pnRadi] = INVALID_PLAYER_ID;
        gRsNar[s][n][pnOd] = gettime();
        gRsNar[s][n][pnBroj] = ++gRs[s][rsBroj] % 100;
        gRsNar[s][n][pnBez] = igrac == INVALID_PLAYER_ID ? RestoranIzmjena(stavka) : 0;
        RestoranRadnicima(s, "Nova narudzba #%d.", gRsNar[s][n][pnBroj]);
        return n;
    }
    return -1;
}

static RestoranJedi(playerid, stavka)
{
    if (gRsJede[playerid]) RemovePlayerAttachedObject(playerid, REST_HRANA_SLOT);
    new Float:hp;
    GetPlayerHealth(playerid, hp);
    hp += gRsZdravlje[stavka];
    SetPlayerHealth(playerid, hp > 100.0 ? 100.0 : hp);
    SetPlayerAttachedObject(playerid, REST_HRANA_SLOT, gRsRuka[stavka], 6, 0.06, 0.03, 0.02, 0.0, 0.0, 0.0);
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
        ApplyAnimation(playerid, gRsStavkaVrsta[stavka] == PV_PICE ? ("VENDING") : ("FOOD"), gRsStavkaVrsta[stavka] == PV_PICE ? ("VEND_Drink2_P") : ("EAT_Burger"), 4.1, false, false, false, false, 0, SYNC_ALL);
    gRsJede[playerid] = true;
    SetTimerEx("RestoranPojeo", REST_JEDENJE_MS, false, "ii", playerid, gAccountId[playerid]);
}

forward RestoranPojeo(playerid, racun);
public RestoranPojeo(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || !gRsJede[playerid]) return;
    gRsJede[playerid] = false;
    RemovePlayerAttachedObject(playerid, REST_HRANA_SLOT);
}

static RestoranPult(s, &Float:x, &Float:y)
{
    new Float:a = RsT(s, PT_KASA)[3];
    x = RsT(s, PT_KASA)[0] + REST_PULT * floatsin(-a, degrees);
    y = RsT(s, PT_KASA)[1] + REST_PULT * floatcos(-a, degrees);
}

static RestoranShopPrikazi(playerid)
{
    new s = gRsShop[playerid], v = gRs[s][rsVrsta], r = gRsMeniVrste[v][gRsShopIdx[playerid]], info[128], Float:x, Float:y;
    format(info, sizeof info, "%s~n~~g~$%d~n~~w~+%.0f HP~n~%d/%d", T(playerid, gRsMeni[r]), gRsCijena[r], gRsZdravlje[r], gRsShopIdx[playerid] + 1, RestoranMeniBroj(v));
    PlayerTextDrawSetString(playerid, gRsShopTd[playerid][PZS_INFO], info);
    if (gRsShopObj[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gRsShopObj[playerid]);
    RestoranPult(s, x, y);
    gRsShopObj[playerid] = CreatePlayerObject(playerid, gRsModel[r], x, y, RsT(s, PT_KASA)[2] + 0.05, gRsModel[r] == 19580 || gRsModel[r] == 2647 ? 0.0 : 90.0, 0.0, RsT(s, PT_KASA)[3] + 90.0);
}

static RestoranShopOtvori(playerid, s)
{
    new Float:a = RsT(s, PT_KASA)[3], Float:x, Float:y;
    RestoranPult(s, x, y);
    gRsShop[playerid] = s;
    gRsShopIdx[playerid] = 0;
    SetPlayerCameraPos(playerid, x + 1.1 * floatsin(-a, degrees), y + 1.1 * floatcos(-a, degrees), RsT(s, PT_KASA)[2] + 0.7);
    SetPlayerCameraLookAt(playerid, x, y, RsT(s, PT_KASA)[2] + 0.05);
    KupovinaGumbi(playerid, gRsShopTd[playerid], 322.0, 150.0);
    RestoranShopPrikazi(playerid);
    AktorKaze(playerid, gRs[s][rsAktor], "Blagajnik", "Dobar dan, sto ce biti?");
}

RestoranShopZatvori(playerid)
{
    if (gRsShop[playerid] == -1) return;
    gRsShop[playerid] = -1;
    if (gRsShopObj[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gRsShopObj[playerid]);
    gRsShopObj[playerid] = INVALID_OBJECT_ID;
    KupovinaZatvori(playerid, gRsShopTd[playerid]);
}

RestoranKlik(playerid, PlayerText:td)
{
    new s = gRsShop[playerid];
    if (s == -1) return 0;
    if (td == gRsShopTd[playerid][PZS_IZADI] || RestoranIgraca(playerid) != s)
    {
        RestoranShopZatvori(playerid);
        return 1;
    }
    new v = gRs[s][rsVrsta], broj = RestoranMeniBroj(v);
    if (td == gRsShopTd[playerid][PZS_LIJEVO] || td == gRsShopTd[playerid][PZS_DESNO])
    {
        gRsShopIdx[playerid] = (gRsShopIdx[playerid] + (td == gRsShopTd[playerid][PZS_DESNO] ? 1 : broj - 1)) % broj;
        RestoranShopPrikazi(playerid);
        return 1;
    }
    if (td != gRsShopTd[playerid][PZS_KUPI]) return 1;
    new r = gRsMeniVrste[v][gRsShopIdx[playerid]];
    for (new n = 0; n < REST_NARUDZBI; n++)
        if (gRsNar[s][n][pnIma] && gRsNar[s][n][pnIgrac] == playerid) return ObavijestGreska(playerid, T(playerid, "Vec cekate narudzbu.")), 1;
    if (!DajNovac(playerid, -gRsCijena[r])) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca.")), 1;
    if (FirmaImaVlasnika(RestoranFirma(s), s)) FirmaProdaja(RestoranFirma(s), s, gRsCijena[r], false);
    else BlagajnaDodaj(gRsCijena[r]);
    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    RestoranShopZatvori(playerid);
    if (gRsStavkaVrsta[r] != PV_PICE && RestoranRadnika(s))
    {
        new n = RestoranNovaNarudzba(s, -1, playerid, r);
        if (n != -1)
        {
            new tekst[128];
            format(tekst, sizeof tekst, T(playerid, "Platili ste $%d. Pricekajte, radnik ce vam donijeti narudzbu #%d."), gRsCijena[r], gRsNar[s][n][pnBroj]);
            ObavijestInfo(playerid, RestoranNaziv(playerid, s), tekst);
            return 1;
        }
    }
    AktorKaze(playerid, gRs[s][rsAktor], "Blagajnik", "Izvolite, dobar tek!");
    RestoranJedi(playerid, r);
    return 1;
}

static RestoranStolSlobodan(s)
{
    new v = gRs[s][rsVrsta], zauzet[REST_STOLOVA], slobodni[REST_STOLOVA], n = 0;
    for (new g = 0; g < REST_GOSTIJU; g++) if (gRsGost[s][g][pgFaza] != PG_NEMA && gRsGost[s][g][pgStol] >= 0) zauzet[gRsGost[s][g][pgStol]] = 1;
    for (new t = 0; t < gRsStolova[v]; t++) if (!zauzet[t]) slobodni[n++] = t;
    return n ? slobodni[random(n)] : -1;
}

static RestoranPutDodaj(s, od, broj, Float:tocke[], &n, bool:obrnuto = false)
{
    new Float:z = RsT(s, PT_KUPAC)[2];
    for (new i = 0; i < broj && n < REST_PUT_MAX; i++, n++)
    {
        new k = od + (obrnuto ? broj - 1 - i : i);
        tocke[n * 3] = gRsPutXY[k * 2];
        tocke[n * 3 + 1] = gRsPutXY[k * 2 + 1];
        tocke[n * 3 + 2] = z;
    }
}

static RestoranPutTocka(Float:tocke[], &n, Float:x, Float:y, Float:z)
{
    if (n >= REST_PUT_MAX) return;
    tocke[n * 3] = x;
    tocke[n * 3 + 1] = y;
    tocke[n * 3 + 2] = z;
    n++;
}

static RestoranGostRed(s, g, &Float:x, &Float:y, &Float:z)
{
    new Float:k = RsT(s, PT_KASA)[3], Float:pomak = (float(g) - 1.5) * 0.9, Float:a = k + 90.0;
    x = RsT(s, PT_KASA)[0] + REST_RED_POMAK * floatsin(-k, degrees) + pomak * floatsin(-a, degrees);
    y = RsT(s, PT_KASA)[1] + REST_RED_POMAK * floatcos(-k, degrees) + pomak * floatcos(-a, degrees);
    z = RsT(s, PT_KUPAC)[2];
}

static RestoranGostNovi(s)
{
    new g = 0;
    while (g < REST_GOSTIJU && gRsGost[s][g][pgFaza] != PG_NEMA) g++;
    if (g == REST_GOSTIJU) return;
    new Float:x, Float:y, Float:z, Float:a;
    RestoranUnutra(s, x, y, z, a);
    new lik = Lik_Stvori(gKupacSkinovi[random(sizeof gKupacSkinovi)], x, y, z, a, REST_SVIJET + s, gRsInt[gRs[s][rsVrsta]]);
    if (lik == INVALID_LIK) return;
    CivilPostavi(lik, true);
    gRsGost[s][g][pgLik] = lik;
    gRsGost[s][g][pgFaza] = PG_DOLAZI;
    gRsGost[s][g][pgStol] = -1;
    gRsGost[s][g][pgNar] = -1;
    gRsGost[s][g][pgOd] = gettime();
    SetTimerEx("RestoranGostKreni", 700, false, "iii", s, g, lik);
}

forward RestoranGostKreni(s, g, lik);
public RestoranGostKreni(s, g, lik)
{
    if (gRsGost[s][g][pgLik] != lik || gRsGost[s][g][pgFaza] != PG_DOLAZI) return;
    new Float:x, Float:y, Float:z, Float:tocke[REST_PUT_MAX * 3], n = 0, v = gRs[s][rsVrsta];
    RestoranGostRed(s, g, x, y, z);
    RestoranPutDodaj(s, gRsUlazPut[v][0], gRsUlazPut[v][1], tocke, n);
    RestoranPutTocka(tocke, n, x, y, z);
    Lik_IdiPutem(lik, tocke, n, LIK_HOD);
}

static RestoranGostMakni(s, g)
{
    if (gRsGost[s][g][pgLik] != INVALID_LIK) Lik_Ukloni(gRsGost[s][g][pgLik]);
    gRsGost[s][g][pgLik] = INVALID_LIK;
    gRsGost[s][g][pgFaza] = PG_NEMA;
    gRsGost[s][g][pgStol] = -1;
    gRsGost[s][g][pgNar] = -1;
}

static RestoranGostVrata(s, lik, t = -1)
{
    new Float:x, Float:y, Float:z, Float:a, Float:tocke[REST_PUT_MAX * 3], n = 0, v = gRs[s][rsVrsta], b = t >= 0 ? gRsStolPut[v][t] : -1;
    RestoranUnutra(s, x, y, z, a);
    if (b != -1) RestoranPutDodaj(s, gRsSjedaloPut[v][b][2], gRsSjedaloPut[v][b][3], tocke, n);
    else if (t == -1) RestoranPutDodaj(s, gRsUlazPut[v][0], gRsUlazPut[v][1], tocke, n, true);
    RestoranPutTocka(tocke, n, x, y, z);
    Lik_IdiPutem(lik, tocke, n, LIK_HOD);
}

static RestoranGostOdlazi(s, g)
{
    new lik = gRsGost[s][g][pgLik];
    if (lik == INVALID_LIK)
    {
        RestoranGostMakni(s, g);
        return;
    }
    new bool:sjedi = gRsGost[s][g][pgFaza] == PG_STOL || gRsGost[s][g][pgFaza] == PG_JEDE, t = gRsGost[s][g][pgStol], bool:ide = gRsGost[s][g][pgFaza] == PG_STOL_IDE;
    gRsGost[s][g][pgFaza] = PG_ODLAZI;
    gRsGost[s][g][pgStol] = -1;
    gRsGost[s][g][pgOd] = gettime();
    Lik_OcistiAnimaciju(lik);
    if (sjedi)
    {
        Lik_Animacija(lik, "FOOD", "FF_Sit_Out_180", 4.1, 0, 1, 1, 0, 0);
        SetTimerEx("RestoranGostIzlaz", 1500, false, "iiii", s, g, lik, t);
    }
    else RestoranGostVrata(s, lik, ide ? -2 : -1);
}

forward RestoranGostIzlaz(s, g, lik, t);
public RestoranGostIzlaz(s, g, lik, t)
{
    if (gRsGost[s][g][pgLik] != lik || gRsGost[s][g][pgFaza] != PG_ODLAZI) return;
    Lik_OcistiAnimaciju(lik);
    RestoranGostVrata(s, lik, t);
}

static RestoranNasumicnaStavka(v)
{
    new opcije[REST_MENI], n = 0;
    for (new m = 0; m < REST_MENI && gRsMeniVrste[v][m] != -1; m++) if (gRsStavkaVrsta[gRsMeniVrste[v][m]] != PV_PICE) opcije[n++] = gRsMeniVrste[v][m];
    return opcije[random(n)];
}

static RestoranGostNaKasi(s, g)
{
    new lik = gRsGost[s][g][pgLik], v = gRs[s][rsVrsta], Float:x, Float:y, Float:z;
    RestoranGostRed(s, g, x, y, z);
    Lik_Stani(lik);
    Lik_Postavi(lik, x, y, z);
    Lik_Kut(lik, RsT(s, PT_KASA)[3] + 180.0);
    new n = RestoranNovaNarudzba(s, g, INVALID_PLAYER_ID, RestoranNasumicnaStavka(v));
    if (n == -1)
    {
        RestoranGostOdlazi(s, g);
        return;
    }
    gRsGost[s][g][pgNar] = n;
    gRsGost[s][g][pgOd] = gettime();
    ApplyActorAnimation(gRs[s][rsAktor], "PED", "IDLE_chat", 4.1, false, false, false, false, 2500);
    new t = gRsStolovi[v] && random(2) ? RestoranStolSlobodan(s) : -1;
    if (t == -1)
    {
        gRsGost[s][g][pgFaza] = PG_KASA;
        return;
    }
    gRsGost[s][g][pgStol] = t;
    gRsGost[s][g][pgFaza] = PG_STOL_IDE;
    new b = gRsStolPut[v][t];
    if (b == -1)
    {
        Lik_Idi(lik, gRsStol[v][t][0], gRsStol[v][t][1], gRsStol[v][t][2], LIK_HOD);
        return;
    }
    new Float:tocke[REST_PUT_MAX * 3], k = 0;
    RestoranPutDodaj(s, gRsSjedaloPut[v][b][0], gRsSjedaloPut[v][b][1], tocke, k);
    Lik_IdiPutem(lik, tocke, k, LIK_HOD);
}

static RestoranGostSjedne(s, g)
{
    new lik = gRsGost[s][g][pgLik], t = gRsGost[s][g][pgStol], v = gRs[s][rsVrsta];
    if (t < 0)
    {
        gRsGost[s][g][pgFaza] = PG_KASA;
        return;
    }
    Lik_Stani(lik);
    Lik_Postavi(lik, gRsStol[v][t][0], gRsStol[v][t][1], gRsStol[v][t][2]);
    Lik_Kut(lik, gRsStol[v][t][3]);
    Lik_Animacija(lik, "FOOD", "FF_Sit_In", 4.1, 0, 1, 1, 1, 0);
    gRsGost[s][g][pgFaza] = PG_STOL;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gRsSmjena[i] == s + 1 && gRsRad[i] >= 0 && gRsRad[i] == gRsGost[s][g][pgNar] && gRsNar[s][gRsRad[i]][pnStanje] == PN_NOSI) SetPlayerCheckpoint(i, gRsStol[v][t][0], gRsStol[v][t][1], gRsStol[v][t][2], 1.0);
    SetTimerEx("RestoranGostSjedi", 1400, false, "iii", s, g, lik);
}

forward RestoranGostSjedi(s, g, lik);
public RestoranGostSjedi(s, g, lik)
{
    if (gRsGost[s][g][pgLik] != lik || gRsGost[s][g][pgFaza] != PG_STOL) return;
    Lik_Animacija(lik, "FOOD", "FF_Sit_Loop", 4.1, 1, 1, 1, 1, 0);
}

RestoranStigao(lik)
{
    for (new s = 0; s < REST_MAX; s++)
    {
        if (!gRs[s][rsIma]) continue;
        for (new g = 0; g < REST_GOSTIJU; g++)
        {
            if (gRsGost[s][g][pgLik] != lik) continue;
            switch (gRsGost[s][g][pgFaza])
            {
                case PG_DOLAZI: RestoranGostNaKasi(s, g);
                case PG_STOL_IDE: RestoranGostSjedne(s, g);
                case PG_ODLAZI: RestoranGostMakni(s, g);
            }
            return 1;
        }
    }
    return 0;
}

static bool:RestoranMeta(s, n, &Float:x, &Float:y, &Float:z)
{
    new i = gRsNar[s][n][pnIgrac];
    if (i != INVALID_PLAYER_ID) return GetPlayerPos(i, x, y, z), true;
    new g = gRsNar[s][n][pnGost];
    if (g < 0 || gRsGost[s][g][pgLik] == INVALID_LIK) return false;
    Lik_Pozicija(gRsGost[s][g][pgLik], x, y, z);
    return true;
}

static RestoranNarudzbaMakni(s, n)
{
    new r = gRsNar[s][n][pnRadi];
    if (r != INVALID_PLAYER_ID && gRsRad[r] == n && gRsSmjena[r] == s + 1) RestoranRadnikPusti(r);
    gRsNar[s][n][pnIma] = false;
}

static RestoranKupacOdlazi(s, n)
{
    new kupac = gRsNar[s][n][pnIgrac], g = gRsNar[s][n][pnGost], stavka = gRsNar[s][n][pnStavka];
    RestoranRadnicima(s, "Kupac s narudzbom #%d je otisao nezadovoljan.", gRsNar[s][n][pnBroj], true);
    RestoranNarudzbaMakni(s, n);
    if (kupac != INVALID_PLAYER_ID && IsPlayerConnected(kupac))
    {
        DajNovac(kupac, gRsCijena[stavka]);
        if (FirmaImaVlasnika(RestoranFirma(s), s)) FirmaProdaja(RestoranFirma(s), s, -gRsCijena[stavka], false);
        ObavijestGreska(kupac, T(kupac, "Radnik je pokvario vasu narudzbu. Novac vam je vracen."));
    }
    if (g >= 0 && gRsGost[s][g][pgFaza] != PG_NEMA && gRsGost[s][g][pgFaza] != PG_ODLAZI)
    {
        gRsGost[s][g][pgNar] = -1;
        RestoranGostOdlazi(s, g);
    }
}

static RestoranNaslov(playerid, const vrsta[], s, n, naslov[], len = sizeof naslov)
{
    format(naslov, len, "%s - #%d %s", T(playerid, vrsta), gRsNar[s][n][pnBroj], T(playerid, gRsMeni[gRsNar[s][n][pnStavka]]));
}

static RestoranIgraKraj(playerid)
{
    if (gRsTimer[playerid]) KillTimer(gRsTimer[playerid]);
    gRsTimer[playerid] = 0;
    IgraTdMakni(playerid);
    if (gRsIgra[playerid] != PI_NEMA)
    {
        TogglePlayerControllable(playerid, true);
        ClearAnimations(playerid, SYNC_ALL);
    }
    gRsIgra[playerid] = PI_NEMA;
}

static RestoranRadnikPusti(playerid)
{
    new s = gRsSmjena[playerid] - 1, n = gRsRad[playerid];
    RestoranIgraKraj(playerid);
    if (s >= 0 && n >= 0 && gRsNar[s][n][pnIma] && gRsNar[s][n][pnRadi] == playerid)
    {
        gRsNar[s][n][pnRadi] = INVALID_PLAYER_ID;
        if (gRsNar[s][n][pnStanje] == PN_NOSI) gRsNar[s][n][pnStanje] = RestoranPocetnoStanje(gRsNar[s][n][pnStavka]);
    }
    if (n >= 0)
    {
        RemovePlayerAttachedObject(playerid, REST_NOSI_SLOT);
        DisablePlayerCheckpoint(playerid);
    }
    gRsRad[playerid] = -1;
}

static RestoranUzmi(playerid, s, stanje)
{
    new najbolji = -1;
    for (new n = 0; n < REST_NARUDZBI; n++)
        if (gRsNar[s][n][pnIma] && gRsNar[s][n][pnStanje] == stanje && gRsNar[s][n][pnRadi] == INVALID_PLAYER_ID && (najbolji == -1 || gRsNar[s][n][pnOd] < gRsNar[s][najbolji][pnOd])) najbolji = n;
    if (najbolji == -1) return -1;
    gRsNar[s][najbolji][pnRadi] = playerid;
    gRsRad[playerid] = najbolji;
    gRsIgraOd[playerid] = gettime();
    gRsAkcija[playerid] = gettime();
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "INT_SHOP", "shop_cashier", 4.1, true, false, false, true, 0, SYNC_ALL);
    return najbolji;
}

static RestoranTrakaPocni(playerid, igra, const vrsta[], s, n, Float:od, Float:zelDo, boja1, boja3, const uputa[], Float:brzina)
{
    new naslov[64];
    gRsIgra[playerid] = igra;
    gRsIgraOd[playerid] = gettime();
    gRsPoz[playerid] = 0.0;
    gRsBrzina[playerid] = brzina;
    RestoranNaslov(playerid, vrsta, s, n, naslov);
    IgraPanel(playerid, naslov, 60.0, gRsBoja[gRs[s][rsVrsta]]);
    IgraTraka(playerid, od, zelDo, boja1, 0x27AE60FF, boja3, uputa);
    gRsZelOd[playerid] = od;
    gRsZelDo[playerid] = zelDo;
    if (gRsTimer[playerid]) KillTimer(gRsTimer[playerid]);
    gRsTimer[playerid] = SetTimerEx("RestoranTrakaKorak", REST_TRAKA_MS, true, "i", playerid);
}

static RestoranPecenjeUputa(playerid, s, n, uputa[], len = sizeof uputa)
{
    format(uputa, len, T(playerid, "~y~SPACE~w~ kad je zeleno (%d/%d). ~r~Crveno~w~ - sirovo, ~l~crno~w~ - zagorjelo."), gRsStrana[playerid] + 1, gRsStrane[gRsNar[s][n][pnStavka]]);
}

static RestoranPeci(playerid, igra, s, n)
{
    new uputa[128], Float:od = float(30 + random(45));
    RestoranPecenjeUputa(playerid, s, n, uputa);
    RestoranTrakaPocni(playerid, igra, igra == PI_PEC ? ("Pec za pizzu") : ("Rostilj"), s, n, od, od + 12.0, 0xC0392BFF, 0x2C2C2CFF, uputa, 1.3 + float(random(5)) * 0.1 + float(gRsStrana[playerid]) * 0.3);
}

static RestoranPrzi(playerid, s, n)
{
    new Float:od = float(30 + random(45)), bool:piletina = gRsStavkaVrsta[gRsNar[s][n][pnStavka]] == PV_PILETINA;
    RestoranTrakaPocni(playerid, PI_POMFRIT, "Friteza", s, n, od, od + 12.0, 0xF4D03FFF, 0x6E2C00FF, piletina ? T(playerid, "Izvadite piletinu: ~y~SPACE~w~ kad je zlatna (zeleno).") : T(playerid, "Izvadite pomfrit: ~y~SPACE~w~ kad je zlatan (zeleno)."), 1.4 + float(random(5)) * 0.1 + float(gRsStrana[playerid]) * 0.3);
}

static RestoranRostiljPocni(playerid, s)
{
    new n = RestoranUzmi(playerid, s, PN_ROSTILJ);
    if (n == -1) return ObavijestGreska(playerid, T(playerid, "Nema narudzbi za rostilj.")), 1;
    gRsStrana[playerid] = 0;
    RestoranPeci(playerid, PI_ROSTILJ, s, n);
    return 1;
}

static RestoranPomfritPocni(playerid, s)
{
    new n = RestoranUzmi(playerid, s, PN_POMFRIT);
    if (n == -1) return ObavijestGreska(playerid, T(playerid, "Nema narudzbi za fritezu.")), 1;
    gRsStrana[playerid] = 0;
    RestoranPrzi(playerid, s, n);
    return 1;
}

static RestoranSlaganjePocni(playerid, s)
{
    new n = RestoranUzmi(playerid, s, PN_SLAGANJE), naslov[64];
    if (n == -1) return ObavijestGreska(playerid, T(playerid, "Nema narudzbi za slaganje.")), 1;
    gRsIgra[playerid] = PI_SLAGANJE;
    gRsKorak[playerid] = 0;
    for (new k = 0; k < 7; k++) gRsSlojTipka[playerid][k] = random(sizeof gRsTipka);
    RestoranNaslov(playerid, "Slaganje", s, n, naslov);
    IgraPanel(playerid, naslov, 120.0, gRsBoja[gRs[s][rsVrsta]]);
    gIgraTd[playerid][IGTD_SLOJEVI] = TdTekst(playerid, IGRA_TRAKA_X, 300.0, " ", 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_1);
    gIgraTd[playerid][IGTD_UPUTA] = TdTekst(playerid, IGRA_TRAKA_X, 386.0, T(playerid, "Pritisnite tipku za sljedeci sloj. Sastojak koji kupac ne zeli (listic) preskocite tipkom ~y~C~w~. Kriva tipka - kupac odlazi!"), 0xFFFFFFFF, 0.2, 0.9, TEXT_DRAW_FONT_1);
    PlayerTextDrawShow(playerid, gIgraTd[playerid][IGTD_UPUTA]);
    RestoranSlojevi(playerid, s, n);
    return 1;
}

static RestoranSlojevi(playerid, s, n)
{
    new tekst[320], red[64], stavka = gRsNar[s][n][pnStavka];
    for (new k = 0; k < 7 && gRsRecept[stavka][k] != -1; k++)
    {
        format(red, sizeof red, "%s%s %s [%s]~n~", k < gRsKorak[playerid] ? ("~g~") : k == gRsKorak[playerid] ? ("~y~") : ("~w~"), k == gRsKorak[playerid] ? (">") : ("-"), T(playerid, gRsSastojak[gRsRecept[stavka][k]]), gRsTipkaIme[gRsSlojTipka[playerid][k]]);
        strcat(tekst, red);
    }
    PlayerTextDrawSetString(playerid, gIgraTd[playerid][IGTD_SLOJEVI], tekst);
    PlayerTextDrawShow(playerid, gIgraTd[playerid][IGTD_SLOJEVI]);
}

static RestoranPakiranjePocni(playerid, s, n)
{
    new Float:od = float(15 + random(60));
    RestoranTrakaPocni(playerid, PI_PAKIRANJE, "Pakiranje", s, n, od, od + 11.0, 0x3A3A3AFF, 0x3A3A3AFF, T(playerid, "Zatvorite vrecicu: ~y~SPACE~w~ kad je oznaka u zelenom."), 2.6);
}

forward RestoranTrakaKorak(playerid);
public RestoranTrakaKorak(playerid)
{
    new igra = gRsIgra[playerid];
    if (igra != PI_ROSTILJ && igra != PI_POMFRIT && igra != PI_PEC && igra != PI_PAKIRANJE) return;
    gRsPoz[playerid] += gRsBrzina[playerid];
    if (igra == PI_PAKIRANJE)
    {
        if (gRsPoz[playerid] >= 100.0 || gRsPoz[playerid] <= 0.0) gRsBrzina[playerid] = -gRsBrzina[playerid];
    }
    else if (gRsPoz[playerid] >= 100.0)
    {
        new s = gRsSmjena[playerid] - 1;
        PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
        RestoranRadnikPusti(playerid);
        if (s >= 0) IgraObavijest(playerid, RestoranNaziv(playerid, s), OBAVIJEST_GRESKA, igra == PI_PEC ? T(playerid, "Pizza je zagorjela! Slozite je ponovno.") : T(playerid, "Zagorjelo je! Pokusajte ponovno."));
        return;
    }
    IgraOznakaPomakni(playerid, gRsPoz[playerid]);
}

static RestoranGotovo(playerid, s, n)
{
    RestoranIgraKraj(playerid);
    gRsAkcija[playerid] = gettime();
    gRsNar[s][n][pnStanje] = PN_NOSI;
    SetPlayerAttachedObject(playerid, REST_NOSI_SLOT, REST_KUTIJA, 6, 0.1, 0.02, -0.05, 0.0, 0.0, 0.0);
    new Float:x, Float:y, Float:z, tekst[128];
    if (RestoranMeta(s, n, x, y, z)) SetPlayerCheckpoint(playerid, x, y, z, 1.0);
    format(tekst, sizeof tekst, T(playerid, "Narudzba #%d je spremna. Odnesite je kupcu (oznacen) i pritisnite ALT kraj njega."), gRsNar[s][n][pnBroj]);
    ObavijestInfo(playerid, RestoranNaziv(playerid, s), tekst);
}

bool:RestoranKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    new igra = gRsIgra[playerid];
    if (igra == PI_NEMA) return false;
    new s = gRsSmjena[playerid] - 1, n = gRsRad[playerid];
    if (s < 0 || n < 0)
    {
        RestoranRadnikPusti(playerid);
        return true;
    }
    new stavka = gRsNar[s][n][pnStavka];
    if (igra == PI_SLAGANJE)
    {
        new pritisnuta = -1, bool:bez = (gRsNar[s][n][pnBez] & (1 << gRsRecept[stavka][gRsKorak[playerid]])) != 0;
        for (new k = 0; k < sizeof gRsTipka; k++) if ((newkeys & gRsTipka[k]) && !(oldkeys & gRsTipka[k])) pritisnuta = k;
        if ((newkeys & KEY_CROUCH) && !(oldkeys & KEY_CROUCH)) pritisnuta = sizeof gRsTipka;
        if (pritisnuta == -1) return true;
        if (bez ? pritisnuta != sizeof gRsTipka : pritisnuta != gRsSlojTipka[playerid][gRsKorak[playerid]])
        {
            PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
            RestoranIgraKraj(playerid);
            gRsRad[playerid] = -1;
            RestoranKupacOdlazi(s, n);
            return true;
        }
        PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
        if (++gRsKorak[playerid] >= 7 || gRsRecept[stavka][gRsKorak[playerid]] == -1)
        {
            IgraTdMakni(playerid);
            if (gRsStavkaVrsta[stavka] == PV_PIZZA)
            {
                gRsStrana[playerid] = 0;
                RestoranPeci(playerid, PI_PEC, s, n);
            }
            else RestoranPakiranjePocni(playerid, s, n);
            return true;
        }
        RestoranSlojevi(playerid, s, n);
        return true;
    }
    if (!(newkeys & KEY_SPRINT) || (oldkeys & KEY_SPRINT)) return true;
    new bool:pogodak = gRsPoz[playerid] >= gRsZelOd[playerid] && gRsPoz[playerid] <= gRsZelDo[playerid];
    PlayerPlaySound(playerid, pogodak ? 1056 : 1055, 0.0, 0.0, 0.0);
    if (!pogodak) return true;
    if (igra == PI_PAKIRANJE)
    {
        RestoranGotovo(playerid, s, n);
        return true;
    }
    if (++gRsStrana[playerid] < gRsStrane[stavka])
    {
        if (igra == PI_POMFRIT) RestoranPrzi(playerid, s, n);
        else RestoranPeci(playerid, igra, s, n);
        return true;
    }
    if (igra == PI_PEC || gRsRecept[stavka][0] == -1)
    {
        RestoranPakiranjePocni(playerid, s, n);
        return true;
    }
    RestoranIgraKraj(playerid);
    gRsAkcija[playerid] = gettime();
    gRsNar[s][n][pnStanje] = PN_SLAGANJE;
    gRsNar[s][n][pnRadi] = INVALID_PLAYER_ID;
    gRsRad[playerid] = -1;
    IgraObavijest(playerid, RestoranNaziv(playerid, s), gRsBoja[gRs[s][rsVrsta]], T(playerid, "Peceno! Narudzba ide na slaganje."));
    return true;
}

static RestoranPredaj(playerid, s, n)
{
    new Float:x, Float:y, Float:z;
    if (!RestoranMeta(s, n, x, y, z) || !IsPlayerInRangeOfPoint(playerid, REST_PREDAJA_DOMET, x, y, z)) return 0;
    new stavka = gRsNar[s][n][pnStavka], kupac = gRsNar[s][n][pnIgrac], g = gRsNar[s][n][pnGost], tekst[128];
    gRsNar[s][n][pnIma] = false;
    gRsRad[playerid] = -1;
    gRsAkcija[playerid] = gettime();
    RemovePlayerAttachedObject(playerid, REST_NOSI_SLOT);
    DisablePlayerCheckpoint(playerid);
    ApplyAnimation(playerid, "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0, SYNC_ALL);
    if (kupac != INVALID_PLAYER_ID)
    {
        RestoranJedi(kupac, stavka);
        format(tekst, sizeof tekst, T(kupac, "%s vam je donio narudzbu. Dobar tek!"), GetRPIme(playerid));
        ObavijestInfo(kupac, RestoranNaziv(kupac, s), tekst);
    }
    else
    {
        FirmaProdaja(RestoranFirma(s), s, gRsCijena[stavka], false);
        if (g >= 0 && gRsGost[s][g][pgLik] != INVALID_LIK)
        {
            gRsGost[s][g][pgNar] = -1;
            if (gRsGost[s][g][pgFaza] == PG_STOL)
            {
                gRsGost[s][g][pgFaza] = PG_JEDE;
                gRsGost[s][g][pgOd] = gettime();
                Lik_Animacija(gRsGost[s][g][pgLik], "FOOD", "FF_Sit_Eat1", 4.1, 1, 1, 1, 1, 0);
            }
            else RestoranGostOdlazi(s, g);
        }
    }
    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    ObavijestInfo(playerid, RestoranNaziv(playerid, s), T(playerid, "Narudzba predana. Za rad u restoranu placu dobivate na placi."));
    return 1;
}

RestoranAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    new s = RestoranIgraca(playerid);
    if (s == -1) return 0;
    if (gRsShop[playerid] != -1) return 1;
    if (RestoranNosi(playerid))
    {
        if (!RestoranPredaj(playerid, s, gRsRad[playerid])) ObavijestGreska(playerid, T(playerid, "Pridite blize kupcu da mu predate narudzbu."));
        return 1;
    }
    if (gRsSmjena[playerid] == s + 1)
    {
        switch (RestoranStanica(playerid, s))
        {
            case PT_ROSTILJ: return RestoranRostiljPocni(playerid, s);
            case PT_POMFRIT: return RestoranPomfritPocni(playerid, s);
            case PT_SLAGANJE: return RestoranSlaganjePocni(playerid, s);
        }
        if (RestoranKodPosla(playerid, s)) return RestoranIzbornik(playerid, s), 1;
        return 0;
    }
    if (RestoranKodPosla(playerid, s)) return RestoranIzbornik(playerid, s), 1;
    if (RestoranTu(playerid, s, PT_KUPAC, REST_KASA_DOMET + 1.0))
    {
        RestoranShopOtvori(playerid, s);
        return 1;
    }
    return 0;
}

static RestoranIzbornik(playerid, s)
{
    if (PosaoIgraca(playerid) != POSAO_RESTORAN) return UpitDaNe(playerid, UPIT_RESTORAN, s, 0, RestoranNaziv(playerid, s), T(playerid, "Zelite li se zaposliti u restoranu?"));
    if (gRsSmjena[playerid] != s + 1) return UpitDaNe(playerid, UPIT_RESTORAN, s, 1, RestoranNaziv(playerid, s), T(playerid, "Zelite li zapoceti smjenu?"));
    return UpitDaNe(playerid, UPIT_RESTORAN, s, 2, RestoranNaziv(playerid, s), T(playerid, "Zelite li zavrsiti smjenu?"));
}

static RestoranPresvuci(playerid, s, bool:radno)
{
    Fade(playerid, 0);
    SetTimerEx("RestoranPresvuciKraj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "iiii", playerid, gAccountId[playerid], s, radno);
}

forward RestoranPresvuciKraj(playerid, racun, s, radno);
public RestoranPresvuciKraj(playerid, racun, s, radno)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    new bool:tu = RestoranIgraca(playerid) == s;
    SetPlayerSkin(playerid, radno && gRsSmjena[playerid] == s + 1 ? gRsSkin[gRs[s][rsVrsta]] : OdjecaSkinIgraca(playerid));
    if (tu && RestoranImaRadno(s))
    {
        new t = radno ? PT_RADNO : PT_POSAO;
        SetPlayerPos(playerid, RsT(s, t)[0], RsT(s, t)[1], RsT(s, t)[2]);
        SetPlayerFacingAngle(playerid, RsT(s, t)[3]);
        SetCameraBehindPlayer(playerid);
        if (radno) Savjet(playerid, T(playerid, "~y~ALT~w~ kod stanice - radi~n~~y~/posao~w~ - zavrsi smjenu"), 6000);
    }
    Fade(playerid, FADE_SLOJEVA + 1);
}

RestoranUpit(playerid, s, opcija, bool:da)
{
    if (!da || RestoranIgraca(playerid) != s) return 1;
    if (!(opcija == 2 && gRsSmjena[playerid] == s + 1) && !RestoranKodPosla(playerid, s, REST_DOMET + 2.0)) return 1;
    if (opcija == 0 && PosaoIgraca(playerid) != POSAO_RESTORAN)
    {
        RibarKraj(playerid);
        PosaoPostavi(playerid, POSAO_RESTORAN);
        return ObavijestInfo(playerid, RestoranNaziv(playerid, s), T(playerid, "Zaposleni ste u restoranu. Pritisnite ALT ovdje (ili /posao) za pocetak smjene."));
    }
    if (PosaoIgraca(playerid) != POSAO_RESTORAN) return 1;
    if (opcija == 1 && gRsSmjena[playerid] != s + 1)
    {
        RestoranSmjenaKraj(playerid);
        gRsSmjena[playerid] = s + 1;
        gRsAkcija[playerid] = gettime();
        if (gRs[s][rsSljedeci] < gettime()) gRs[s][rsSljedeci] = gettime() + 5;
        RestoranPresvuci(playerid, s, true);
        return ObavijestInfo(playerid, RestoranNaziv(playerid, s), T(playerid, "Smjena je pocela. Pecite na rostilju, frizi i u peci, slozite i zapakirajte narudzbu, pa je odnesite kupcu."));
    }
    if (opcija == 2 && gRsSmjena[playerid] == s + 1)
    {
        RestoranSmjenaKraj(playerid, true);
        return ObavijestInfo(playerid, RestoranNaziv(playerid, s), T(playerid, "Zavrsili ste smjenu."));
    }
    return 1;
}

RestoranPosao(playerid)
{
    new s = RestoranIgraca(playerid);
    if (s == -1 && PosaoIgraca(playerid) == POSAO_RESTORAN) return ObavijestGreska(playerid, T(playerid, "Smjenu zapocinjete u restoranu (ALT kod mjesta za posao).")), 1;
    if (s == -1 || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    if (gRsSmjena[playerid] != s + 1 && !RestoranKodPosla(playerid, s)) return 0;
    RestoranIzbornik(playerid, s);
    return 1;
}

RestoranPlaca(playerid)
{
    new iznos = REST_PLACA_SAT * gRsRadio[playerid] / 3600;
    gRsRadio[playerid] = 0;
    return iznos;
}

static RestoranListaMakni(playerid)
{
    if (gRsListaIma[playerid]) PlayerTextDrawDestroy(playerid, gRsLista[playerid]);
    gRsListaIma[playerid] = false;
    gRsListaTekst[playerid][0] = EOS;
}

RestoranSmjenaKraj(playerid, bool:fade = false)
{
    if (!gRsSmjena[playerid]) return;
    RestoranRadnikPusti(playerid);
    RestoranListaMakni(playerid);
    if (IsPlayerConnected(playerid) && fade) RestoranPresvuci(playerid, gRsSmjena[playerid] - 1, false);
    gRsSmjena[playerid] = 0;
    if (IsPlayerConnected(playerid) && !fade) SetPlayerSkin(playerid, OdjecaSkinIgraca(playerid));
}

RestoranDisconnect(playerid)
{
    gRsSmjena[playerid] = 0;
    gRsRad[playerid] = -1;
    gRsListaIma[playerid] = false;
    gRsIgra[playerid] = PI_NEMA;
    IgraTdReset(playerid);
    gRsJede[playerid] = false;
    gRsSavjet[playerid] = -1;
    gRsRadio[playerid] = 0;
    gRsShop[playerid] = -1;
    gRsShopObj[playerid] = INVALID_OBJECT_ID;
    if (gRsTimer[playerid]) KillTimer(gRsTimer[playerid]);
    gRsTimer[playerid] = 0;
    for (new s = 0; s < REST_MAX; s++)
    {
        if (!gRs[s][rsIma]) continue;
        for (new n = 0; n < REST_NARUDZBI; n++)
        {
            if (!gRsNar[s][n][pnIma]) continue;
            if (gRsNar[s][n][pnIgrac] == playerid) RestoranNarudzbaMakni(s, n);
            else if (gRsNar[s][n][pnRadi] == playerid)
            {
                gRsNar[s][n][pnRadi] = INVALID_PLAYER_ID;
                if (gRsNar[s][n][pnStanje] == PN_NOSI) gRsNar[s][n][pnStanje] = RestoranPocetnoStanje(gRsNar[s][n][pnStavka]);
            }
        }
    }
}

RestoranCheckpoint(playerid)
{
    return gRsSmjena[playerid] && gRsRad[playerid] >= 0 ? 1 : 0;
}

static RestoranListaOsvjezi(playerid, s)
{
    static const stanje[][] = { "~r~rostilj", "~y~friteza", "~y~slaganje", "~g~nosi se" };
    new tekst[800], red[64], n = 0;
    strcat(tekst, T(playerid, "~r~NARUDZBE~w~~n~"));
    for (new k = 0; k < REST_NARUDZBI; k++)
    {
        if (!gRsNar[s][k][pnIma]) continue;
        format(red, sizeof red, "#%d %s - %s~w~%s~n~", gRsNar[s][k][pnBroj], T(playerid, gRsMeni[gRsNar[s][k][pnStavka]]), T(playerid, stanje[gRsNar[s][k][pnStanje]]), gRsNar[s][k][pnRadi] == playerid ? (" *") : (""));
        strcat(tekst, red);
        for (new b = 0; b < sizeof gRsSastojak; b++)
        {
            if (!(gRsNar[s][k][pnBez] & (1 << b))) continue;
            format(red, sizeof red, T(playerid, "~r~   bez: %s~w~~n~"), T(playerid, gRsSastojak[b]));
            strcat(tekst, red);
        }
        n++;
    }
    if (!n) strcat(tekst, T(playerid, "~w~Nema narudzbi."));
    if (!strcmp(tekst, gRsListaTekst[playerid])) return;
    strcopy(gRsListaTekst[playerid], tekst, sizeof gRsListaTekst[]);
    if (!gRsListaIma[playerid])
    {
        gRsLista[playerid] = TdTekst(playerid, 500.0, 150.0, tekst, 0xFFFFFFFF, 0.22, 1.0, TEXT_DRAW_FONT_1);
        PlayerTextDrawUseBox(playerid, gRsLista[playerid], true);
        PlayerTextDrawBoxColour(playerid, gRsLista[playerid], 0x101010B0);
        PlayerTextDrawTextSize(playerid, gRsLista[playerid], 630.0, 0.0);
        gRsListaIma[playerid] = true;
    }
    else PlayerTextDrawSetString(playerid, gRsLista[playerid], tekst);
    PlayerTextDrawShow(playerid, gRsLista[playerid]);
}

static RestoranSavjeti(playerid, s)
{
    new k = -1, v = gRs[s][rsVrsta];
    if (gRsIgra[playerid] == PI_NEMA && gRsRad[playerid] == -1) k = RestoranStanica(playerid, s);
    if (k != -1 && k != gRsSavjet[playerid])
        Savjet(playerid, k == PT_ROSTILJ ? T(playerid, "~y~ALT~w~ - peci meso") : k == PT_POMFRIT ? T(playerid, "~y~ALT~w~ - friteza") : v == RV_PIZZA ? T(playerid, "~y~ALT~w~ - slozi, ispeci pizzu i zapakiraj") : T(playerid, "~y~ALT~w~ - slozi i zapakiraj"), 4000);
    gRsSavjet[playerid] = k;
    new n = gRsRad[playerid], Float:x, Float:y, Float:z;
    if (n < 0 || gRsNar[s][n][pnStanje] != PN_NOSI) return;
    new g = gRsNar[s][n][pnGost];
    if (gRsNar[s][n][pnIgrac] == INVALID_PLAYER_ID && (g < 0 || gRsGost[s][g][pgFaza] != PG_STOL_IDE)) return;
    if (RestoranMeta(s, n, x, y, z)) SetPlayerCheckpoint(playerid, x, y, z, 1.0);
}

RestoranTick()
{
    new sad = gettime(), radnici[REST_MAX];
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gRsSmjena[i]) continue;
        new s = gRsSmjena[i] - 1;
        if (!IsPlayerConnected(i) || RestoranIgraca(i) != s || PosaoIgraca(i) != POSAO_RESTORAN || IsRanjen(i))
        {
            RestoranSmjenaKraj(i);
            if (IsPlayerConnected(i)) ObavijestInfo(i, RestoranNaziv(i, s), T(i, "Izasli ste iz restorana i zavrsili smjenu."));
            continue;
        }
        if (gRsIgra[i] != PI_NEMA && sad - gRsIgraOd[i] >= REST_IGRA_S)
        {
            RestoranRadnikPusti(i);
            ObavijestGreska(i, T(i, "Predugo vam je trebalo. Narudzba se vraca na cekanje."));
        }
        radnici[s]++;
        if (sad - gRsAkcija[i] < REST_AKTIVAN_S && gRsRadio[i] < 3600) gRsRadio[i]++;
        RestoranListaOsvjezi(i, s);
        RestoranSavjeti(i, s);
    }
    for (new s = 0; s < REST_MAX; s++)
    {
        if (!gRs[s][rsIma]) continue;
        new radnika = radnici[s];
        if (radnika && sad >= gRs[s][rsSljedeci])
        {
            gRs[s][rsSljedeci] = sad + REST_GOST_MIN + random(REST_GOST_RASPON);
            RestoranGostNovi(s);
        }
        for (new n = 0; n < REST_NARUDZBI; n++)
        {
            if (!gRsNar[s][n][pnIma]) continue;
            new kupac = gRsNar[s][n][pnIgrac];
            if (kupac == INVALID_PLAYER_ID) continue;
            if (!IsPlayerConnected(kupac) || RestoranIgraca(kupac) != s || sad - gRsNar[s][n][pnOd] >= REST_IGRAC_CEKA || !radnika)
            {
                new stavka = gRsNar[s][n][pnStavka];
                RestoranNarudzbaMakni(s, n);
                if (IsPlayerConnected(kupac))
                {
                    RestoranJedi(kupac, stavka);
                    ObavijestInfo(kupac, RestoranNaziv(kupac, s), T(kupac, "Blagajnik vam je sam predao narudzbu. Dobar tek!"));
                }
            }
        }
        for (new g = 0; g < REST_GOSTIJU; g++)
        {
            new faza = gRsGost[s][g][pgFaza], proslo = sad - gRsGost[s][g][pgOd];
            switch (faza)
            {
                case PG_DOLAZI: if (proslo >= REST_HODA_S) RestoranGostNaKasi(s, g);
                case PG_STOL_IDE: if (proslo >= REST_HODA_S * 2) RestoranGostSjedne(s, g);
                case PG_JEDE: if (proslo >= REST_JEDE_S) RestoranGostOdlazi(s, g);
                case PG_ODLAZI: if (proslo >= REST_HODA_S) RestoranGostMakni(s, g);
            }
            if ((faza == PG_KASA || faza == PG_STOL || faza == PG_STOL_IDE) && proslo >= REST_STRPLJENJE)
            {
                new n = gRsGost[s][g][pgNar];
                if (n >= 0 && gRsNar[s][n][pnIma] && gRsNar[s][n][pnGost] == g) RestoranKupacOdlazi(s, n);
                else RestoranGostOdlazi(s, g);
            }
        }
    }
}

static RestoranUdjiKaoAdmin(playerid, v)
{
    new s = 0;
    while (s < REST_MAX && (!gRs[s][rsIma] || gRs[s][rsVrsta] != v)) s++;
    if (s == REST_MAX) return ObavijestGreska(playerid, T(playerid, "Prvo postavite barem jedan restoran te vrste.")), 0;
    new Float:x, Float:y, Float:z, Float:a;
    RestoranUnutra(s, x, y, z, a);
    AdminPrebaci(playerid, gRsInt[v], REST_SVIJET + s, x, y, z);
    return 1;
}

CMD:restoranint(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    new v = strval(params);
    if (!ProvjeriBroj(params) || v < 0 || v >= REST_VRSTA) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Koristenje: /restoranint [0 pizza, 1 burger, 2 cluckin]"));
    RestoranUdjiKaoAdmin(playerid, v);
    return 1;
}

ShowRestoranAdmin(playerid)
{
    new list[256], red[64], len = 0;
    for (new v = 0; v < REST_VRSTA; v++)
    {
        new n = 0;
        for (new s = 0; s < REST_MAX; s++) if (gRs[s][rsIma] && gRs[s][rsVrsta] == v) n++;
        format(red, sizeof red, "%s (%d)", gRsVrstaIme[v], n);
        DodajStupac(list, len, red);
    }
    return PrikaziDialog(playerid, DLG_APANEL_RESTORAN_VRSTA, DIALOG_STYLE_LIST, T(playerid, "Restorani"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_restoran_vrsta(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (listitem < 0 || listitem >= REST_VRSTA) return 1;
    gRsApVrsta[playerid] = listitem;
    return ShowRestoranVrsta(playerid);
}

static ShowRestoranVrsta(playerid)
{
    new v = gRsApVrsta[playerid], list[3072], red[96], len = 0, n = 0;
    format(red, sizeof red, T(playerid, "{8AB4F8}+ Novi restoran: %s (ulaz na mojoj poziciji)"), gRsVrstaIme[v]);
    DodajStupac(list, len, red);
    for (new t = 0; t < PT_BROJ; t++)
    {
        format(red, sizeof red, T(playerid, "Postavi: %s (moja pozicija u interijeru)"), T(playerid, t == PT_SLAGANJE && v == RV_PIZZA ? ("Slaganje i pec za pizzu") : gRsTockaIme[t]));
        DodajStupac(list, len, red);
    }
    format(red, sizeof red, T(playerid, "+ Dodaj stol (moja pozicija) - %d/%d"), gRsStolova[v], REST_STOLOVA);
    DodajStupac(list, len, red);
    DodajStupac(list, len, T(playerid, "Udji u interijer restorana"));
    for (new t = 0; t < gRsStolova[v]; t++)
    {
        format(red, sizeof red, T(playerid, "Stol %d - obrisi"), t + 1);
        DodajStupac(list, len, red);
        gRsApPopis[playerid][n++] = REST_MAX + t;
    }
    for (new s = 0; s < REST_MAX; s++)
    {
        if (!gRs[s][rsIma] || gRs[s][rsVrsta] != v) continue;
        format(red, sizeof red, T(playerid, "[%d] %s (%s) - obrisi"), gRs[s][rsDb], gRsVrstaIme[v], CestaZona(gRs[s][rsX], gRs[s][rsY]));
        DodajStupac(list, len, red);
        gRsApPopis[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_RESTORAN, DIALOG_STYLE_LIST, gRsVrstaIme[v], list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static RestoranStolMakni(v, t)
{
    for (new s = 0; s < REST_MAX; s++)
    {
        if (!gRs[s][rsIma] || gRs[s][rsVrsta] != v) continue;
        for (new g = 0; g < REST_GOSTIJU; g++)
        {
            if (gRsGost[s][g][pgFaza] == PG_NEMA || gRsGost[s][g][pgStol] < 0) continue;
            if (gRsGost[s][g][pgStol] == t)
            {
                new n = gRsGost[s][g][pgNar];
                if (n >= 0 && gRsNar[s][n][pnIma] && gRsNar[s][n][pnGost] == g) RestoranKupacOdlazi(s, n);
                else RestoranGostOdlazi(s, g);
            }
            else if (gRsGost[s][g][pgStol] > t) gRsGost[s][g][pgStol]--;
        }
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM restaurant_tables WHERE id = %d", gRsStolDb[v][t]));
    for (new k = t; k < gRsStolova[v] - 1; k++)
    {
        gRsStolDb[v][k] = gRsStolDb[v][k + 1];
        gRsStolPut[v][k] = gRsStolPut[v][k + 1];
        for (new j = 0; j < 4; j++) gRsStol[v][k][j] = gRsStol[v][k + 1][j];
    }
    gRsStolova[v]--;
}

DLG:apanel_restoran(playerid, response, listitem)
{
    if (!response) return ShowRestoranAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new v = gRsApVrsta[playerid], u = RestoranIgraca(playerid), bool:unutra = u != -1 && gRs[u][rsVrsta] == v, Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    if (listitem == 0)
    {
        new s = 0;
        while (s < REST_MAX && gRs[s][rsIma]) s++;
        if (s == REST_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ulaz se postavlja samo izvana.")), ShowRestoranVrsta(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO restaurants (vrsta, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", v, x, y, z, a));
        gRs[s][rsDb] = ZadnjiId();
        gRs[s][rsVrsta] = v;
        gRs[s][rsX] = x;
        gRs[s][rsY] = y;
        gRs[s][rsZ] = z;
        gRs[s][rsA] = a;
        RestoranStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Restoran je postavljen."));
        return ShowRestoranVrsta(playerid);
    }
    if (listitem >= 1 && listitem <= PT_BROJ + 1)
    {
        if (!unutra) return ObavijestGreska(playerid, T(playerid, "Morate biti u interijeru restorana te vrste (/restoranint).")), ShowRestoranVrsta(playerid);
        if (listitem <= PT_BROJ)
        {
            new t = listitem - 1;
            gRsTocka[v][t][0] = x;
            gRsTocka[v][t][1] = y;
            gRsTocka[v][t][2] = z;
            gRsTocka[v][t][3] = a;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO restaurant_points (vrsta, tip, x, y, z, a) VALUES (%d, %d, %f, %f, %f, %f)", v, t, x, y, z, a));
            for (new s = 0; s < REST_MAX; s++) if (gRs[s][rsIma] && gRs[s][rsVrsta] == v) RestoranOznakeMakni(s), RestoranOznakeStvori(s);
            if (t == PT_KASA) SetPlayerPos(playerid, x + 1.2 * floatsin(-a, degrees), y + 1.2 * floatcos(-a, degrees), z);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tocka je postavljena za sve restorane te vrste."));
            return ShowRestoranVrsta(playerid);
        }
        if (gRsStolova[v] >= REST_STOLOVA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowRestoranVrsta(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO restaurant_tables (vrsta, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", v, x, y, z, a));
        gRsStolDb[v][gRsStolova[v]] = ZadnjiId();
        gRsStolPut[v][gRsStolova[v]] = -1;
        gRsStol[v][gRsStolova[v]][0] = x;
        gRsStol[v][gRsStolova[v]][1] = y;
        gRsStol[v][gRsStolova[v]][2] = z;
        gRsStol[v][gRsStolova[v]][3] = a;
        gRsStolova[v]++;
        ApplyAnimation(playerid, "FOOD", "FF_Sit_Loop", 4.1, true, true, true, true, 0, SYNC_ALL);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Stol je dodan. Ovako ce NPC sjediti; ako ne izgleda dobro, obrisite stol i postavite se drugacije."));
        return ShowRestoranVrsta(playerid);
    }
    if (listitem == PT_BROJ + 2)
    {
        RestoranUdjiKaoAdmin(playerid, v);
        return 1;
    }
    new i = listitem - PT_BROJ - 3;
    if (i < 0 || i >= sizeof gRsApPopis[]) return 1;
    new k = gRsApPopis[playerid][i];
    if (k >= REST_MAX)
    {
        if (k - REST_MAX < gRsStolova[v]) RestoranStolMakni(v, k - REST_MAX);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Stol je obrisan."));
        return ShowRestoranVrsta(playerid);
    }
    new s = k;
    if (!gRs[s][rsIma]) return ShowRestoranVrsta(playerid);
    for (new p = 0, iZadnji = gIgracZadnji; p <= iZadnji; p++)
    {
        if (gRsSmjena[p] == s + 1) RestoranSmjenaKraj(p);
        if (gRsShop[p] == s) RestoranShopZatvori(p);
    }
    for (new g = 0; g < REST_GOSTIJU; g++) RestoranGostMakni(s, g);
    for (new n = 0; n < REST_NARUDZBI; n++) gRsNar[s][n][pnIma] = false;
    RestoranOznakeMakni(s);
    DestroyDynamicPickup(gRs[s][rsPickup]);
    DestroyDynamic3DTextLabel(gRs[s][rsTekst]);
    FirmaMakni(RestoranFirma(s), s);
    gRs[s][rsIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM restaurants WHERE id = %d", gRs[s][rsDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Restoran je obrisan."));
    return ShowRestoranVrsta(playerid);
}
