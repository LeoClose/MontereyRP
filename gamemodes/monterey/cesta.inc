#define CESTA_MAX_TOCAKA    3072
#define CESTA_POLA_SIRINE   0.9
#define CESTA_DESNO         2.5
#define CESTA_PARKING       4.0
#define CESTA_Z_VOZILA      0.8
#define CESTA_BRZINA        18.0
#define CESTA_UBRZANJE      3.0
#define CESTA_BOCNO         12.0
#define CESTA_ZAVOJ_MIN     10.0
#define CESTA_REZ           20.0
#define CESTA_KORAK         6.0
#define CESTA_KRAJ          1.0
#define TAKSI_USPORI        15.0
#define TAKSI_DOSEG         20
#define TAKSI_STOP          7.0
#define TAKSI_SIRINA        2.4
#define OBILAZAK_NAPRIJED   6.0
#define OBILAZAK_STRANA     3.5
#define OBILAZAK_BRZINA     0.5

static gCestaG[sizeof gCestaTocka];
static gCestaOtac[sizeof gCestaTocka];
static gCestaKrug[sizeof gCestaTocka];
static gCestaBroj;
static gCestaRuta[sizeof gCestaTocka];
static gCestaZadnjaRuta;
static gHrpa[sizeof gCestaVeza + 2][2];
static gHrpaN;
static gCestaPrije[sizeof gCestaVeza];
static gCestaPrijePocetak[sizeof gCestaTocka + 1];
static Float:gCestaRaw[sizeof gCestaTocka + 2][3];

Float:CestaTlo(Float:x, Float:y, Float:z)
{
    return HodTlo(x, y, z) - 1.0 + CESTA_Z_VOZILA;
}

static Float:CestaMin(Float:a, Float:b) { return a < b ? a : b; }
static Float:CestaMax(Float:a, Float:b) { return a > b ? a : b; }

static HrpaGurni(f, n)
{
    new i = gHrpaN++;
    while (i && gHrpa[(i - 1) / 2][0] > f)
    {
        gHrpa[i] = gHrpa[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    gHrpa[i][0] = f;
    gHrpa[i][1] = n;
}

static HrpaIzvadi(&f)
{
    new n = gHrpa[0][1], i = 0, zadnji[2];
    f = gHrpa[0][0];
    zadnji = gHrpa[--gHrpaN];
    for (;;)
    {
        new c = i * 2 + 1;
        if (c >= gHrpaN) break;
        if (c + 1 < gHrpaN && gHrpa[c + 1][0] < gHrpa[c][0]) c++;
        if (gHrpa[c][0] >= zadnji[0]) break;
        gHrpa[i] = gHrpa[c];
        i = c;
    }
    gHrpa[i] = zadnji;
    return n;
}

static CestaRazmak(a, b)
{
    return floatround(VectorSize(float(gCestaTocka[a][0] - gCestaTocka[b][0]), float(gCestaTocka[a][1] - gCestaTocka[b][1]), float(gCestaTocka[a][2] - gCestaTocka[b][2])));
}

static CestaTrazi(od, cilj, &duljina)
{
    gCestaBroj++;
    gHrpaN = 0;
    gCestaG[od] = 0;
    gCestaOtac[od] = -1;
    gCestaKrug[od] = gCestaBroj;
    HrpaGurni(CestaRazmak(od, cilj), od);
    while (gHrpaN)
    {
        new f, n = HrpaIzvadi(f);
        if (n == cilj)
        {
            new broj = 0;
            duljina = gCestaG[cilj] / 10;
            for (new i = cilj; i != -1; i = gCestaOtac[i]) gCestaRuta[broj++] = i;
            return broj;
        }
        if (f > gCestaG[n] + CestaRazmak(n, cilj)) continue;
        for (new l = gCestaPocetak[n]; l < gCestaPocetak[n + 1]; l++)
        {
            new m = gCestaVeza[l] >> 1, g = gCestaG[n] + CestaRazmak(n, m);
            if (gCestaKrug[m] == gCestaBroj && g >= gCestaG[m]) continue;
            gCestaKrug[m] = gCestaBroj;
            gCestaG[m] = g;
            gCestaOtac[m] = n;
            HrpaGurni(g + CestaRazmak(m, cilj), m);
        }
    }
    return 0;
}

static bool:CestaDvosmjerna(a, b)
{
    for (new l = gCestaPocetak[a]; l < gCestaPocetak[a + 1]; l++)
        if (gCestaVeza[l] >> 1 == b) return bool:(gCestaVeza[l] & 1);
    return false;
}

CestaNajbliza(Float:x, Float:y, Float:z, &Float:razmak = 0.0, bool:bezVisine = false, bool:vidljiva = false)
{
    new best = 0, Float:bd = 1.0e12;
    for (new i = 0; i < sizeof gCestaTocka; i++)
    {
        new Float:dx = float(gCestaTocka[i][0]) / 10.0 - x, Float:dy = float(gCestaTocka[i][1]) / 10.0 - y, Float:dz = bezVisine ? 0.0 : float(gCestaTocka[i][2]) / 10.0 - z;
        new Float:d = dx * dx + dy * dy + dz * dz;
        if (d >= bd || (vidljiva && d < 10000.0 && !CestaSlobodno(x, y, z, x + dx, y + dy, float(gCestaTocka[i][2]) / 10.0 + CESTA_Z_VOZILA))) continue;
        bd = d;
        best = i;
    }
    razmak = floatsqroot(bd);
    return best;
}

CestaIza(cilj, Float:metara)
{
    new tren = cilj, pret = -1;
    for (new korak = 0; korak < 16 && metara > 0.0; korak++)
    {
        new nadeno = -1;
        for (new l = gCestaPrijePocetak[tren]; l < gCestaPrijePocetak[tren + 1] && nadeno == -1; l++)
            if (gCestaPrije[l] != pret && gCestaPrije[l] != tren) nadeno = gCestaPrije[l];
        if (nadeno == -1) break;
        metara -= float(CestaRazmak(nadeno, tren)) / 10.0;
        pret = tren;
        tren = nadeno;
    }
    return tren;
}

static bool:CestaDodaj(Float:t[][4], max, &n, Float:x, Float:y, Float:z, Float:v)
{
    new koraka = 1;
    if (n)
    {
        new Float:px = t[n - 1][0], Float:py = t[n - 1][1], Float:pz = t[n - 1][2];
        koraka = floatround(VectorSize(x - px, y - py, 0.0) / CESTA_KORAK, floatround_ceil);
        if (koraka < 1) koraka = 1;
        if (n + koraka > max) return false;
        for (new i = 1; i < koraka; i++)
        {
            new Float:f = float(i) / float(koraka);
            t[n][0] = px + (x - px) * f;
            t[n][1] = py + (y - py) * f;
            t[n][2] = CestaTlo(t[n][0], t[n][1], pz + (z - pz) * f);
            t[n++][3] = CESTA_BRZINA;
        }
    }
    else if (max < 1) return false;
    t[n][0] = x;
    t[n][1] = y;
    t[n][2] = CestaTlo(x, y, z);
    t[n++][3] = v;
    return true;
}

static bool:CestaZaglati(raw, Float:t[][4], max, &n)
{
    if (!CestaDodaj(t, max, n, gCestaRaw[0][0], gCestaRaw[0][1], gCestaRaw[0][2], CESTA_BRZINA)) return false;
    for (new i = 1; i + 1 < raw; i++)
    {
        new Float:ax = gCestaRaw[i - 1][0] - gCestaRaw[i][0], Float:ay = gCestaRaw[i - 1][1] - gCestaRaw[i][1], Float:az = gCestaRaw[i - 1][2] - gCestaRaw[i][2];
        new Float:cx = gCestaRaw[i + 1][0] - gCestaRaw[i][0], Float:cy = gCestaRaw[i + 1][1] - gCestaRaw[i][1], Float:cz = gCestaRaw[i + 1][2] - gCestaRaw[i][2];
        new Float:la = VectorSize(ax, ay, 0.0), Float:lc = VectorSize(cx, cy, 0.0);
        if (la < 0.01 || lc < 0.01) continue;
        new Float:cos = (ax * cx + ay * cy) / (la * lc);
        new Float:skret = 180.0 - acos(cos > 1.0 ? 1.0 : (cos < -1.0 ? -1.0 : cos));
        if (skret < 6.0)
        {
            if (!CestaDodaj(t, max, n, gCestaRaw[i][0], gCestaRaw[i][1], gCestaRaw[i][2], CESTA_BRZINA)) return false;
            continue;
        }
        new Float:r = CestaMin(CestaMin(la, lc) / 2.0, CESTA_REZ);
        while (r > 2.0 && !CestaSlobodno(gCestaRaw[i][0] + ax * r / la, gCestaRaw[i][1] + ay * r / la, gCestaRaw[i][2] + az * r / la, gCestaRaw[i][0] + cx * r / lc, gCestaRaw[i][1] + cy * r / lc, gCestaRaw[i][2] + cz * r / lc)) r /= 2.0;
        new Float:polumjer = r * floattan(90.0 - skret / 2.0, degrees);
        new Float:v = CestaMax(CESTA_ZAVOJ_MIN, CestaMin(CESTA_BRZINA, floatsqroot(CESTA_BOCNO * polumjer)));
        new koraka = clamp(floatround(skret / 5.0), 3, 24);
        for (new j = 0; j <= koraka; j++)
        {
            new Float:f = float(j) / float(koraka), Float:a = (1.0 - f) * (1.0 - f) * r / la, Float:c = f * f * r / lc;
            if (!CestaDodaj(t, max, n, gCestaRaw[i][0] + ax * a + cx * c, gCestaRaw[i][1] + ay * a + cy * c, gCestaRaw[i][2] + az * a + cz * c, v)) return false;
        }
    }
    return CestaDodaj(t, max, n, gCestaRaw[raw - 1][0], gCestaRaw[raw - 1][1], gCestaRaw[raw - 1][2], CESTA_KRAJ);
}

static Float:CestaBrzine(Float:t[][4], n)
{
    t[0][3] = 2.0;
    t[n - 1][3] = CESTA_KRAJ;
    for (new i = n - 2; i >= 0; i--)
        t[i][3] = CestaMin(t[i][3], floatsqroot(t[i + 1][3] * t[i + 1][3] + 2.0 * CESTA_UBRZANJE * VectorSize(t[i + 1][0] - t[i][0], t[i + 1][1] - t[i][1], 0.0)));
    for (new i = 1; i < n; i++)
        t[i][3] = CestaMin(t[i][3], floatsqroot(t[i - 1][3] * t[i - 1][3] + 2.0 * CESTA_UBRZANJE * VectorSize(t[i][0] - t[i - 1][0], t[i][1] - t[i - 1][1], 0.0)));
    new Float:sekundi = 0.0, Float:prije = t[0][3];
    for (new i = 1; i < n; i++)
    {
        new Float:v = (prije + t[i][3]) / 2.0;
        prije = t[i][3];
        sekundi += VectorSize(t[i][0] - t[i - 1][0], t[i][1] - t[i - 1][1], 0.0) / v;
        t[i][3] = v / 10.0;
    }
    t[0][3] /= 10.0;
    return sekundi;
}

CestaPut(od, cilj, Float:t[][4], max, &n, &sekundi, bool:imaStart = false, Float:sx = 0.0, Float:sy = 0.0, Float:sz = 0.0, Float:sa = 0.0)
{
    new duljina, broj = CestaTrazi(od, cilj, duljina), raw = 0;
    gCestaZadnjaRuta = broj;
    if (!broj) return -1;
    if (imaStart)
    {
        gCestaRaw[0][0] = sx; gCestaRaw[0][1] = sy; gCestaRaw[0][2] = sz;
        gCestaRaw[1][0] = sx - floatsin(sa, degrees) * 6.0; gCestaRaw[1][1] = sy + floatcos(sa, degrees) * 6.0; gCestaRaw[1][2] = sz;
        raw = CestaSlobodno(sx, sy, sz, gCestaRaw[1][0], gCestaRaw[1][1], sz) ? 2 : 1;
    }
    for (new i = broj - 1; i >= 0; i--)
    {
        new a = gCestaRuta[i], Float:x = float(gCestaTocka[a][0]) / 10.0, Float:y = float(gCestaTocka[a][1]) / 10.0, Float:z = float(gCestaTocka[a][2]) / 10.0 + CESTA_Z_VOZILA;
        if (broj > 1)
        {
            new p = i < broj - 1 ? gCestaRuta[i + 1] : a, q = i ? gCestaRuta[i - 1] : a;
            new Float:dx = float(gCestaTocka[q][0] - gCestaTocka[p][0]), Float:dy = float(gCestaTocka[q][1] - gCestaTocka[p][1]), Float:d = VectorSize(dx, dy, 0.0);
            new Float:pomak = !i ? CESTA_PARKING : (CestaDvosmjerna(a, gCestaRuta[i - 1]) ? CESTA_DESNO : 0.0);
            if (!i && d > 0.01 && !CestaSlobodno(x, y, z, x + dy / d * pomak, y - dx / d * pomak, z)) pomak = CestaDvosmjerna(gCestaRuta[1], a) ? CESTA_DESNO : 0.0;
            if (d > 0.01)
            {
                x += dy / d * pomak;
                y -= dx / d * pomak;
            }
        }
        gCestaRaw[raw][0] = x; gCestaRaw[raw][1] = y; gCestaRaw[raw++][2] = z;
    }
    n = 0;
    if (raw < 2)
    {
        gCestaRaw[1] = gCestaRaw[0];
        raw = 2;
    }
    if (!CestaZaglati(raw, t, max, n)) return -1;
    sekundi = floatround(CestaBrzine(t, n), floatround_ceil);
    return duljina;
}

bool:CestaSmjer(&Float:dx, &Float:dy)
{
    if (gCestaZadnjaRuta < 2) return false;
    new a = gCestaRuta[1], b = gCestaRuta[0];
    dx = float(gCestaTocka[b][0] - gCestaTocka[a][0]);
    dy = float(gCestaTocka[b][1] - gCestaTocka[a][1]);
    new Float:d = VectorSize(dx, dy, 0.0);
    if (d < 0.01) return false;
    dx /= d;
    dy /= d;
    return true;
}

CestaZona(Float:x, Float:y)
{
    new ime[28];
    for (new i = 0; i < sizeof gZona; i++)
    {
        if (x < gZona[i][zMinX] || x > gZona[i][zMaxX] || y < gZona[i][zMinY] || y > gZona[i][zMaxY]) continue;
        strcat(ime, gZona[i][zIme]);
        return ime;
    }
    strcat(ime, "Los Santos");
    return ime;
}

static Float:gPrepVozila[MAX_VEHICLES][3], gPrepVozilaM[MAX_VEHICLES][2];
static Float:gPrepIgraci[MAX_PLAYERS][3], gPrepIgraciM[MAX_PLAYERS][2], gPrepIgracId[MAX_PLAYERS], gPrepBroj;

CestaPocetak(Float:x, Float:y, Float:z, Float:a)
{
    new Float:d, od = CestaNajbliza(x - floatsin(a, degrees) * 15.0, y + floatcos(a, degrees) * 15.0, z, d);
    if (CestaSlobodno(x, y, z, float(gCestaTocka[od][0]) / 10.0, float(gCestaTocka[od][1]) / 10.0, float(gCestaTocka[od][2]) / 10.0 + CESTA_Z_VOZILA)) return od;
    return CestaNajbliza(x, y, z, d, false, true);
}

VoziloSmjer(v, &Float:x, &Float:y, &Float:z, &Float:fx, &Float:fy)
{
    new Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    fx = -floatsin(a, degrees);
    fy = floatcos(a, degrees);
}

CestaNastavi(v, const Float:t[][4], n, &idx)
{
    new Float:x, Float:y, Float:z, Float:fx, Float:fy;
    VoziloSmjer(v, x, y, z, fx, fy);
    while (idx < n - 1)
    {
        new Float:dx = t[idx][0] - x, Float:dy = t[idx][1] - y;
        if (dx * fx + dy * fy > 4.0 || VectorSize(dx, dy, 0.0) > 30.0) break;
        idx++;
    }
}

TaksiSnimi()
{
    for (new i = 0; i < gVozilaCount; i++)
    {
        GetVehiclePos(gVozila[i], gPrepVozila[i][0], gPrepVozila[i][1], gPrepVozila[i][2]);
        gPrepVozilaM[i][0] = floatround(gPrepVozila[i][0]);
        gPrepVozilaM[i][1] = floatround(gPrepVozila[i][1]);
    }
    gPrepBroj = 0;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i] || IsPlayerInAnyVehicle(i) || GetPlayerState(i) == PLAYER_STATE_SPECTATING) continue;
        GetPlayerPos(i, gPrepIgraci[gPrepBroj][0], gPrepIgraci[gPrepBroj][1], gPrepIgraci[gPrepBroj][2]);
        gPrepIgraciM[gPrepBroj][0] = floatround(gPrepIgraci[gPrepBroj][0]);
        gPrepIgraciM[gPrepBroj][1] = floatround(gPrepIgraci[gPrepBroj][1]);
        gPrepIgracId[gPrepBroj++] = i;
    }
}

static Float:TaksiNaPutu(Float:dx, Float:dy, Float:dz, Float:fx, Float:fy)
{
    if (floatabs(dz) > 4.0) return TAKSI_USPORI;
    new Float:uz = dx * fx + dy * fy - 2.5;
    return uz > -1.0 && floatabs(dx * fy - dy * fx) < TAKSI_SIRINA ? uz : TAKSI_USPORI;
}

Float:CestaPrepreka(v, m, Float:x, Float:y, Float:z, &bool:kupac, &Float:ox, &Float:oy, zanemari = 0)
{
    new Float:a, Float:naj = TAKSI_USPORI, mx = floatround(x), my = floatround(y);
    GetVehicleZAngle(v, a);
    new Float:fx = -floatsin(a, degrees), Float:fy = floatcos(a, degrees);
    kupac = false;
    for (new i = 0; i < gVozilaCount; i++)
    {
        new dx = gPrepVozilaM[i][0] - mx, dy = gPrepVozilaM[i][1] - my;
        if (dx > TAKSI_DOSEG || dx < -TAKSI_DOSEG || dy > TAKSI_DOSEG || dy < -TAKSI_DOSEG || gVozila[i] == v || gVozila[i] == zanemari) continue;
        new Float:d = TaksiNaPutu(gPrepVozila[i][0] - x, gPrepVozila[i][1] - y, gPrepVozila[i][2] - z, fx, fy);
        if (d < naj)
        {
            naj = d;
            ox = gPrepVozila[i][0];
            oy = gPrepVozila[i][1];
        }
    }
    for (new i = 0; i < gPrepBroj; i++)
    {
        new dx = gPrepIgraciM[i][0] - mx, dy = gPrepIgraciM[i][1] - my;
        if (dx > TAKSI_DOSEG || dx < -TAKSI_DOSEG || dy > TAKSI_DOSEG || dy < -TAKSI_DOSEG) continue;
        new Float:d = TaksiNaPutu(gPrepIgraci[i][0] - x, gPrepIgraci[i][1] - y, gPrepIgraci[i][2] - z, fx, fy);
        if (d < naj)
        {
            naj = d;
            kupac = gPrepIgracId[i] == m;
            ox = gPrepIgraci[i][0];
            oy = gPrepIgraci[i][1];
        }
    }
    return naj;
}

CestaSlobodno(Float:x, Float:y, Float:z, Float:px, Float:py, Float:pz)
{
    new Float:dx = px - x, Float:dy = py - y, Float:d = VectorSize(dx, dy, 0.0);
    if (d < 0.1) return 1;
    new Float:bx = -dy / d * CESTA_POLA_SIRINE, Float:by = dx / d * CESTA_POLA_SIRINE;
    return !Kolizija_Zraka(x, y, z, px, py, pz) && !Kolizija_Zraka(x + bx, y + by, z, px + bx, py + by, pz) && !Kolizija_Zraka(x - bx, y - by, z, px - bx, py - by, pz);
}

CestaObidi(npc, v, Float:ox, Float:oy, Float:brzina)
{
    new Float:x, Float:y, Float:z, Float:fx, Float:fy;
    VoziloSmjer(v, x, y, z, fx, fy);
    new Float:rx = fy, Float:ry = -fx;
    new Float:prva = (ox - x) * rx + (oy - y) * ry > 0.0 ? -OBILAZAK_STRANA : OBILAZAK_STRANA;
    for (new k = 0; k < 2; k++)
    {
        new Float:strana = k ? -prva : prva;
        new Float:px = ox + fx * OBILAZAK_NAPRIJED + rx * strana, Float:py = oy + fy * OBILAZAK_NAPRIJED + ry * strana;
        new Float:pz = CestaTlo(px, py, z);
        if (!CestaSlobodno(x, y, z, px, py, pz)) continue;
        NPC_Move(npc, px, py, pz, NPC_MOVE_TYPE_DRIVE, brzina * OBILAZAK_BRZINA);
        return 1;
    }
    return 0;
}

InitCesta()
{
    for (new l = 0; l < sizeof gCestaVeza; l++) gCestaPrijePocetak[gCestaVeza[l] >> 1]++;
    for (new i = 1; i < sizeof gCestaTocka; i++) gCestaPrijePocetak[i] += gCestaPrijePocetak[i - 1];
    gCestaPrijePocetak[sizeof gCestaTocka] = sizeof gCestaVeza;
    for (new u = sizeof gCestaTocka - 1; u >= 0; u--)
        for (new l = gCestaPocetak[u]; l < gCestaPocetak[u + 1]; l++) gCestaPrije[--gCestaPrijePocetak[gCestaVeza[l] >> 1]] = u;
    new duljina, broj = CestaTrazi(0, sizeof gCestaTocka - 1, duljina);
    assert(broj >= 2 && gCestaRuta[0] == sizeof gCestaTocka - 1 && gCestaRuta[broj - 1] == 0 && duljina > 0);
    for (new i = 0; i + 1 < broj; i++) assert(CestaRazmak(gCestaRuta[i], gCestaRuta[i + 1]) < 2000);
    assert(CestaTrazi(5, 5, duljina) == 1 && duljina == 0);
    assert(CestaIza(5, 25.0) != 5 && CestaTrazi(CestaIza(5, 25.0), 5, duljina) >= 2 && duljina < 400);
    new Float:d;
    assert(CestaNajbliza(float(gCestaTocka[7][0]) / 10.0, float(gCestaTocka[7][1]) / 10.0, float(gCestaTocka[7][2]) / 10.0, d) == 7 && d < 0.2);

    static Float:t[CESTA_MAX_TOCAKA][4];
    new n, sek;
    duljina = CestaPut(CestaNajbliza(2683.8, -2504.0, 13.2, d), CestaNajbliza(1154.6, -859.0, 47.0, d), t, sizeof t, n, sek);
    assert(duljina > 0 && n > 2 && sek > duljina / 18 - 1);
    for (new i = 2; i < n; i++)
    {
        new Float:ax = t[i - 1][0] - t[i - 2][0], Float:ay = t[i - 1][1] - t[i - 2][1], Float:bx = t[i][0] - t[i - 1][0], Float:by = t[i][1] - t[i - 1][1];
        new Float:la = VectorSize(ax, ay, 0.0), Float:lb = VectorSize(bx, by, 0.0);
        assert(lb < CESTA_KORAK + 0.5 && t[i][3] > 0.0);
        if (la > 0.2 && lb > 0.2) assert((ax * bx + ay * by) / (la * lb) > 0.5);
    }
}
