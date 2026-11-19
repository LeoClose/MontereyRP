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

enum E_PLANINAR { plNpc, plGore, plDolje, bool:plNaVrhu, Float:plDx, Float:plDy, Float:plDz, Float:plVx, Float:plVy, Float:plVz }
static gPlaninar[PLANINA_NPC][E_PLANINAR], gPlaninarN, gNpcPlaninar[MAX_PLAYERS];
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
    for (new i = 0; i < MAX_PLAYERS; i++) gNpcPlaninar[i] = -1;
    new Float:vx = PLANINA_X, Float:vy = PLANINA_Y, Float:vz = -100.0;
    for (new dx = -PLANINA_SKEN; dx <= PLANINA_SKEN; dx += 5) for (new dy = -PLANINA_SKEN; dy <= PLANINA_SKEN; dy += 5)
    {
        new Float:x = PLANINA_X + float(dx), Float:y = PLANINA_Y + float(dy), Float:z = PlaninaTlo(x, y);
        if (z > vz) { vx = x; vy = y; vz = z; }
    }
    if (vz < 50.0) return;
    new Float:tx[40], Float:ty[40], Float:tz[40];
    for (new p = 0; p < PLANINA_NPC; p++)
    {
        new n = 0;
        for (new pomak = 0; pomak < 36 && !n; pomak += 6) n = PlaninaPut(vx, vy, vz, float(p * 36 + pomak), tx, ty, tz, sizeof tx);
        if (!n) continue;
        new g = gPlaninarN, gore = NPC_CreatePath(), dolje = NPC_CreatePath();
        for (new i = n - 1; i >= 0; i--) NPC_AddPointToPath(gore, tx[i], ty[i], tz[i] + 1.0, 0.8);
        for (new i = 0; i < n; i++) NPC_AddPointToPath(dolje, tx[i], ty[i], tz[i] + 1.0, 0.8);
        gPlaninar[g][plGore] = gore;
        gPlaninar[g][plDolje] = dolje;
        gPlaninar[g][plDx] = tx[n - 1]; gPlaninar[g][plDy] = ty[n - 1]; gPlaninar[g][plDz] = tz[n - 1] + 1.0;
        gPlaninar[g][plVx] = tx[0]; gPlaninar[g][plVy] = ty[0]; gPlaninar[g][plVz] = tz[0] + 1.0;
        gPlaninar[g][plNaVrhu] = g % 2 == 1;
        new ime[MAX_PLAYER_NAME + 1];
        format(ime, sizeof ime, "Planinar_%d", g);
        gPlaninar[g][plNpc] = NPC_Create(ime);
        if (gPlaninar[g][plNpc] == INVALID_PLAYER_ID)
        {
            NPC_DestroyPath(gore);
            NPC_DestroyPath(dolje);
            continue;
        }
        gNpcPlaninar[gPlaninar[g][plNpc]] = g;
        gPlaninarN++;
        NPC_Spawn(gPlaninar[g][plNpc]);
    }
}

PlaninaSpawn(npcid)
{
    new g = gNpcPlaninar[npcid];
    if (g == -1) return 0;
    NPC_SetSkin(npcid, gPlaninarSkinovi[random(sizeof gPlaninarSkinovi)]);
    NPC_SetInvulnerable(npcid, true);
    if (gPlaninar[g][plNaVrhu]) NPC_SetPos(npcid, gPlaninar[g][plVx], gPlaninar[g][plVy], gPlaninar[g][plVz]);
    else NPC_SetPos(npcid, gPlaninar[g][plDx], gPlaninar[g][plDy], gPlaninar[g][plDz]);
    SetTimerEx("PlaninarKreni", 1000 + random(PLANINA_PAUZA_MAX), false, "ii", g, npcid);
    return 1;
}

forward PlaninarKreni(g, npcid);
public PlaninarKreni(g, npcid)
{
    if (gPlaninar[g][plNpc] != npcid) return;
    NPC_MoveByPath(npcid, gPlaninar[g][plNaVrhu] ? gPlaninar[g][plDolje] : gPlaninar[g][plGore], NPC_MOVE_TYPE_WALK);
}

PlaninaPutGotov(npcid, pathid)
{
    new g = gNpcPlaninar[npcid];
    if (g == -1) return 0;
    if (pathid != gPlaninar[g][plGore] && pathid != gPlaninar[g][plDolje]) return 1;
    gPlaninar[g][plNaVrhu] = pathid == gPlaninar[g][plGore];
    SetTimerEx("PlaninarKreni", PLANINA_PAUZA_MIN + random(PLANINA_PAUZA_MAX - PLANINA_PAUZA_MIN), false, "ii", g, npcid);
    return 1;
}
