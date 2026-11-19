#define STAZA_RUTA          512

static gStazaG[sizeof gStazaTocka], gStazaOtac[sizeof gStazaTocka], gStazaKrug[sizeof gStazaTocka], gStazaBroj;
static gStazaHrpa[sizeof gStazaVeza + 2][2], gStazaHrpaN;

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
    return floatround(VectorSize(float(gStazaTocka[a][0] - gStazaTocka[b][0]), float(gStazaTocka[a][1] - gStazaTocka[b][1]), float(gStazaTocka[a][2] - gStazaTocka[b][2])));
}

StazaTocka(i, &Float:x, &Float:y, &Float:z)
{
    x = float(gStazaTocka[i][0]) / 10.0;
    y = float(gStazaTocka[i][1]) / 10.0;
    z = float(gStazaTocka[i][2]) / 10.0;
}

StazaBrojTocaka()
{
    return sizeof gStazaTocka;
}

StazaNajbliza(Float:x, Float:y, Float:z, &Float:razmak = 0.0)
{
    new best = 0, Float:bd = 1.0e12;
    for (new i = 0; i < sizeof gStazaTocka; i++)
    {
        new Float:dx = float(gStazaTocka[i][0]) / 10.0 - x, Float:dy = float(gStazaTocka[i][1]) / 10.0 - y, Float:dz = float(gStazaTocka[i][2]) / 10.0 - z;
        new Float:d = dx * dx + dy * dy + dz * dz * 4.0;
        if (d >= bd) continue;
        bd = d;
        best = i;
    }
    razmak = floatsqroot(bd);
    return best;
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
        for (new l = gStazaPocetak[n]; l < gStazaPocetak[n + 1]; l++)
        {
            new m = gStazaVeza[l], g = gStazaG[n] + StazaRazmak(n, m);
            if (gStazaKrug[m] == gStazaBroj && g >= gStazaG[m]) continue;
            gStazaKrug[m] = gStazaBroj;
            gStazaG[m] = g;
            gStazaOtac[m] = n;
            StazaGurni(g + StazaRazmak(m, cilj), m);
        }
    }
    return 0;
}
