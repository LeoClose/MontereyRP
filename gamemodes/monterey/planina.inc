#define PLANINA_NPC          10
#define PLANINA_X            1193.4
#define PLANINA_Y            -2054.9
#define PLANINA_SKEN         110
#define PLANINA_KORAK        4.0
#define PLANINA_DOMET        125.0
#define PLANINA_DNO          20.0
#define PLANINA_NAGIB        1.3
#define PLANINA_PAUZA_MIN    4000
#define PLANINA_PAUZA_MAX    15000
#define PLANINA_TOCAKA       40

enum E_PLANINAR { plLik, plBroj, bool:plNaVrhu }
static gPlaninar[PLANINA_NPC][E_PLANINAR], gPlaninarN;
static Float:gPlaninaGore[PLANINA_NPC][PLANINA_TOCAKA * 3], Float:gPlaninaDolje[PLANINA_NPC][PLANINA_TOCAKA * 3];
static const gPlaninarSkinovi[] = { 7, 14, 23, 26, 29, 37, 44, 56, 59, 72, 93, 101, 141, 170, 184, 192, 211, 216, 250, 261 };

static Float:PlaninaTlo(Float:x, Float:y)
{
    new Float:hx, Float:hy, Float:hz;
    if (!Kolizija_Zraka(x, y, 250.0, x, y, -10.0, hx, hy, hz)) return -100.0;
    return hz;
}

static PlaninaPut(Float:vx, Float:vy, Float:vz, Float:kut, Float:tx[], Float:ty[], Float:tz[], max)
{
    new n = 0;
    tx[n] = vx; ty[n] = vy; tz[n] = vz; n++;
    for (new Float:r = PLANINA_KORAK; r <= PLANINA_DOMET && n < max; r += PLANINA_KORAK)
    {
        new Float:x = vx + r * floatcos(kut, degrees), Float:y = vy + r * floatsin(kut, degrees), Float:z = PlaninaTlo(x, y);
        if (z < -50.0) return 0;
        if (floatabs(z - tz[n - 1]) > PLANINA_KORAK * PLANINA_NAGIB) return 0;
        tx[n] = x; ty[n] = y; tz[n] = z; n++;
        if (z < PLANINA_DNO) break;
    }
    return n >= 6 ? n : 0;
}

InitPlanina()
{
    new Float:vx = PLANINA_X, Float:vy = PLANINA_Y, Float:vz = -100.0;
    for (new dx = -PLANINA_SKEN; dx <= PLANINA_SKEN; dx += 5) for (new dy = -PLANINA_SKEN; dy <= PLANINA_SKEN; dy += 5)
    {
        new Float:x = PLANINA_X + float(dx), Float:y = PLANINA_Y + float(dy), Float:z = PlaninaTlo(x, y);
        if (z > vz) { vx = x; vy = y; vz = z; }
    }
    if (vz < 50.0) return;
    new Float:tx[PLANINA_TOCAKA], Float:ty[PLANINA_TOCAKA], Float:tz[PLANINA_TOCAKA];
    for (new p = 0; p < PLANINA_NPC; p++)
    {
        new n = 0;
        for (new pomak = 0; pomak < 36 && !n; pomak += 6) n = PlaninaPut(vx, vy, vz, float(p * 36 + pomak), tx, ty, tz, sizeof tx);
        if (!n) continue;
        new g = gPlaninarN, bool:vrh = g % 2 == 1, k = vrh ? 0 : n - 1;
        for (new i = 0; i < n; i++)
        {
            gPlaninaGore[g][i * 3] = tx[n - 1 - i]; gPlaninaGore[g][i * 3 + 1] = ty[n - 1 - i]; gPlaninaGore[g][i * 3 + 2] = tz[n - 1 - i] + 1.0;
            gPlaninaDolje[g][i * 3] = tx[i]; gPlaninaDolje[g][i * 3 + 1] = ty[i]; gPlaninaDolje[g][i * 3 + 2] = tz[i] + 1.0;
        }
        new lik = Lik_Stvori(gPlaninarSkinovi[random(sizeof gPlaninarSkinovi)], tx[k], ty[k], tz[k] + 1.0);
        if (lik < 0) continue;
        gPlaninar[g][plLik] = lik;
        gPlaninar[g][plBroj] = n;
        gPlaninar[g][plNaVrhu] = vrh;
        gPlaninarN++;
        SetTimerEx("PlaninarKreni", 1000 + random(PLANINA_PAUZA_MAX), false, "i", g);
    }
}

forward PlaninarKreni(g);
public PlaninarKreni(g)
{
    if (gPlaninar[g][plNaVrhu]) Lik_IdiPutem(gPlaninar[g][plLik], gPlaninaDolje[g], gPlaninar[g][plBroj], LIK_HOD);
    else Lik_IdiPutem(gPlaninar[g][plLik], gPlaninaGore[g], gPlaninar[g][plBroj], LIK_HOD);
}

PlaninaStigao(lik)
{
    for (new g = 0; g < gPlaninarN; g++)
    {
        if (gPlaninar[g][plLik] != lik) continue;
        gPlaninar[g][plNaVrhu] = !gPlaninar[g][plNaVrhu];
        SetTimerEx("PlaninarKreni", PLANINA_PAUZA_MIN + random(PLANINA_PAUZA_MAX - PLANINA_PAUZA_MIN), false, "i", g);
        return 1;
    }
    return 0;
}
