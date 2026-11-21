#define STAZA_RUTA          512
#define STAZA_DODATNO       2500
#define STAZA_D_VEZA        10
#define STAZA_K_VEZA        2
#define STAZA_KORAK         10.0
#define STAZA_CELIJA        4096
#define STAZA_PRSA          0.9
#define STAZA_BOK           0.35
#define STAZA_SPOJ          14.0
#define STAZA_DZ            3.5
#define STAZA_KONST         (sizeof gStazaTocka)
#define STAZA_SVE           (sizeof gStazaTocka + STAZA_DODATNO)

static const Float:gStazaPodrucja[][6] = {
    { 150.0, -2100.0, 950.0, -1780.0, 0.6, 12.0 }
};

static gStazaG[STAZA_SVE], gStazaOtac[STAZA_SVE], gStazaKrug[STAZA_SVE], gStazaBroj;
static gStazaHrpa[sizeof gStazaVeza + STAZA_DODATNO * STAZA_D_VEZA + sizeof gStazaTocka * STAZA_K_VEZA + 2][2], gStazaHrpaN;
static gStazaD[STAZA_DODATNO][3], gStazaDN, gStazaDVeza[STAZA_DODATNO][STAZA_D_VEZA], gStazaDVezaN[STAZA_DODATNO];
static gStazaKVeza[sizeof gStazaTocka][STAZA_K_VEZA], gStazaKVezaN[sizeof gStazaTocka], bool:gStazaBlok[sizeof gStazaVeza];
static gStazaCelija[STAZA_CELIJA];

static StazaK(i, k)
{
    return i < STAZA_KONST ? gStazaTocka[i][k] : gStazaD[i - STAZA_KONST][k];
}

static StazaGurni(f, n)
{
    new i = gStazaHrpaN++;
    while (i && gStazaHrpa[(i - 1) / 2][0] > f)
    {
        gStazaHrpa[i] = gStazaHrpa[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    gStazaHrpa[i][0] = f;
    gStazaHrpa[i][1] = n;
}

static StazaIzvadi(&f)
{
    new n = gStazaHrpa[0][1], i = 0, zadnji[2];
    f = gStazaHrpa[0][0];
    zadnji = gStazaHrpa[--gStazaHrpaN];
    for (;;)
    {
        new c = i * 2 + 1;
        if (c >= gStazaHrpaN) break;
        if (c + 1 < gStazaHrpaN && gStazaHrpa[c + 1][0] < gStazaHrpa[c][0]) c++;
        if (gStazaHrpa[c][0] >= zadnji[0]) break;
        gStazaHrpa[i] = gStazaHrpa[c];
        i = c;
    }
    gStazaHrpa[i] = zadnji;
    return n;
}

static StazaRazmak(a, b)
{
    return floatround(VectorSize(float(StazaK(a, 0) - StazaK(b, 0)), float(StazaK(a, 1) - StazaK(b, 1)), float(StazaK(a, 2) - StazaK(b, 2))));
}

static bool:StazaZid(Float:ax, Float:ay, Float:az, Float:bx, Float:by, Float:bz)
{
    new Float:dx = bx - ax, Float:dy = by - ay, Float:d = VectorSize(dx, dy, 0.0), Float:hx, Float:hy, Float:hz;
    if (d < 0.5) return false;
    new Float:px = -dy / d * STAZA_BOK, Float:py = dx / d * STAZA_BOK;
    for (new s = -1; s <= 1; s += 2)
        if (!Kolizija_Zraka(ax + px * float(s), ay + py * float(s), az + STAZA_PRSA, bx + px * float(s), by + py * float(s), bz + STAZA_PRSA, hx, hy, hz)) return false;
    return true;
}

static bool:StazaZidIzmedu(a, b)
{
    return StazaZid(float(StazaK(a, 0)) / 10.0, float(StazaK(a, 1)) / 10.0, float(StazaK(a, 2)) / 10.0, float(StazaK(b, 0)) / 10.0, float(StazaK(b, 1)) / 10.0, float(StazaK(b, 2)) / 10.0);
}

static StazaSpoji(a, b)
{
    if (a < STAZA_KONST)
    {
        if (gStazaKVezaN[a] >= STAZA_K_VEZA) return false;
        gStazaKVeza[a][gStazaKVezaN[a]++] = b;
        return true;
    }
    new e = a - STAZA_KONST;
    if (gStazaDVezaN[e] >= STAZA_D_VEZA) return false;
    for (new i = 0; i < gStazaDVezaN[e]; i++) if (gStazaDVeza[e][i] == b) return false;
    gStazaDVeza[e][gStazaDVezaN[e]++] = b;
    return true;
}

static StazaSpojiObje(a, b)
{
    if (StazaZidIzmedu(a, b) || floatabs(float(StazaK(a, 2) - StazaK(b, 2)) / 10.0) > STAZA_DZ) return;
    if (!StazaSpoji(a, b)) return;
    if (!StazaSpoji(b, a))
    {
        if (a < STAZA_KONST) gStazaKVezaN[a]--;
        else gStazaDVezaN[a - STAZA_KONST]--;
    }
}

static StazaPodrucje(p)
{
    new Float:x1 = gStazaPodrucja[p][0], Float:y1 = gStazaPodrucja[p][1], Float:x2 = gStazaPodrucja[p][2], Float:y2 = gStazaPodrucja[p][3], Float:zmin = gStazaPodrucja[p][4], Float:zmax = gStazaPodrucja[p][5];
    new cols = floatround((x2 - x1) / STAZA_KORAK, floatround_floor) + 1, rows = floatround((y2 - y1) / STAZA_KORAK, floatround_floor) + 1, pocetak = gStazaDN;
    if (cols * rows > STAZA_CELIJA) return;
    new blizu[1024], nb = 0;
    for (new i = 0; i < STAZA_KONST && nb < sizeof blizu; i++)
    {
        new Float:x = float(gStazaTocka[i][0]) / 10.0, Float:y = float(gStazaTocka[i][1]) / 10.0;
        if (x >= x1 - STAZA_SPOJ && x <= x2 + STAZA_SPOJ && y >= y1 - STAZA_SPOJ && y <= y2 + STAZA_SPOJ) blizu[nb++] = i;
    }
    for (new r = 0; r < rows; r++) for (new c = 0; c < cols; c++)
    {
        new cel = r * cols + c;
        gStazaCelija[cel] = -1;
        if (gStazaDN >= STAZA_DODATNO) continue;
        new Float:x = x1 + float(c) * STAZA_KORAK, Float:y = y1 + float(r) * STAZA_KORAK, Float:hx, Float:hy, Float:hz, Float:gx, Float:gy, Float:gz;
        if (!Kolizija_Zraka(x, y, zmax + 3.0, x, y, zmin - 1.0, hx, hy, hz) || hz < zmin || hz > zmax) continue;
        if (Kolizija_Zraka(x, y, hz + 0.3, x, y, hz + 4.0, gx, gy, gz)) continue;
        new bool:dupla = false;
        for (new b = 0; b < nb && !dupla; b++) dupla = VectorSize(float(gStazaTocka[blizu[b]][0]) / 10.0 - x, float(gStazaTocka[blizu[b]][1]) / 10.0 - y, 0.0) < 4.0;
        if (dupla) continue;
        new d = gStazaDN++;
        gStazaD[d][0] = floatround(x * 10.0);
        gStazaD[d][1] = floatround(y * 10.0);
        gStazaD[d][2] = floatround(hz * 10.0);
        gStazaDVezaN[d] = 0;
        gStazaCelija[cel] = STAZA_KONST + d;
    }
    for (new r = 0; r < rows; r++) for (new c = 0; c < cols; c++)
    {
        new a = gStazaCelija[r * cols + c];
        if (a == -1) continue;
        if (c + 1 < cols && gStazaCelija[r * cols + c + 1] != -1) StazaSpojiObje(a, gStazaCelija[r * cols + c + 1]);
        if (r + 1 < rows && gStazaCelija[(r + 1) * cols + c] != -1) StazaSpojiObje(a, gStazaCelija[(r + 1) * cols + c]);
        if (r + 1 < rows && c + 1 < cols && gStazaCelija[(r + 1) * cols + c + 1] != -1) StazaSpojiObje(a, gStazaCelija[(r + 1) * cols + c + 1]);
        if (r + 1 < rows && c > 0 && gStazaCelija[(r + 1) * cols + c - 1] != -1) StazaSpojiObje(a, gStazaCelija[(r + 1) * cols + c - 1]);
    }
    for (new d = pocetak; d < gStazaDN; d++)
    {
        new a = STAZA_KONST + d, najbliza = -1, Float:nd = STAZA_SPOJ;
        for (new b = 0; b < nb; b++)
        {
            new Float:r = float(StazaRazmak(a, blizu[b])) / 10.0;
            if (r < nd && gStazaKVezaN[blizu[b]] < STAZA_K_VEZA && !StazaZidIzmedu(a, blizu[b])) { nd = r; najbliza = blizu[b]; }
        }
        if (najbliza != -1) StazaSpojiObje(a, najbliza);
    }
}

InitStaze()
{
    new blok = 0;
    for (new n = 0; n < STAZA_KONST; n++) for (new l = gStazaPocetak[n]; l < gStazaPocetak[n + 1]; l++)
    {
        gStazaBlok[l] = StazaZidIzmedu(n, gStazaVeza[l]);
        if (gStazaBlok[l]) blok++;
    }
    for (new p = 0; p < sizeof gStazaPodrucja; p++) StazaPodrucje(p);
    printf("[Staze] %d veza kroz zid uklonjeno, %d novih tocaka (plaza, mol).", blok, gStazaDN);
}

StazaTocka(i, &Float:x, &Float:y, &Float:z)
{
    x = float(StazaK(i, 0)) / 10.0;
    y = float(StazaK(i, 1)) / 10.0;
    z = float(StazaK(i, 2)) / 10.0;
}

StazaBrojTocaka()
{
    return STAZA_KONST + gStazaDN;
}

static bool:StazaZiva(i)
{
    return i < STAZA_KONST || gStazaDVezaN[i - STAZA_KONST] > 0;
}

StazaNajbliza(Float:x, Float:y, Float:z, &Float:razmak = 0.0)
{
    new best = 0, Float:bd = 1.0e12;
    for (new i = 0, n = StazaBrojTocaka(); i < n; i++)
    {
        new Float:dx = float(StazaK(i, 0)) / 10.0 - x, Float:dy = float(StazaK(i, 1)) / 10.0 - y, Float:dz = float(StazaK(i, 2)) / 10.0 - z;
        new Float:d = dx * dx + dy * dy + dz * dz * 4.0;
        if (d >= bd || !StazaZiva(i)) continue;
        bd = d;
        best = i;
    }
    razmak = floatsqroot(bd);
    return best;
}

static StazaOpusti(n, m, cilj)
{
    new g = gStazaG[n] + StazaRazmak(n, m);
    if (gStazaKrug[m] == gStazaBroj && g >= gStazaG[m]) return;
    gStazaKrug[m] = gStazaBroj;
    gStazaG[m] = g;
    gStazaOtac[m] = n;
    StazaGurni(g + StazaRazmak(m, cilj), m);
}

StazaPut(od, cilj, ruta[], max)
{
    gStazaBroj++;
    gStazaHrpaN = 0;
    gStazaG[od] = 0;
    gStazaOtac[od] = -1;
    gStazaKrug[od] = gStazaBroj;
    StazaGurni(StazaRazmak(od, cilj), od);
    while (gStazaHrpaN)
    {
        new f, n = StazaIzvadi(f);
        if (n == cilj)
        {
            new broj = 0;
            for (new i = cilj; i != -1; i = gStazaOtac[i]) broj++;
            if (broj > max) return 0;
            new k = broj;
            for (new i = cilj; i != -1; i = gStazaOtac[i]) ruta[--k] = i;
            return broj;
        }
        if (f > gStazaG[n] + StazaRazmak(n, cilj)) continue;
        if (n < STAZA_KONST)
        {
            for (new l = gStazaPocetak[n]; l < gStazaPocetak[n + 1]; l++) if (!gStazaBlok[l]) StazaOpusti(n, gStazaVeza[l], cilj);
            for (new j = 0; j < gStazaKVezaN[n]; j++) StazaOpusti(n, gStazaKVeza[n][j], cilj);
        }
        else
        {
            new e = n - STAZA_KONST;
            for (new j = 0; j < gStazaDVezaN[e]; j++) StazaOpusti(n, gStazaDVeza[e][j], cilj);
        }
    }
    return 0;
}
