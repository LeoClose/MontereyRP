// Ovo je kod za nase NPCeve kako se ne bi zabijali u zidove
#define HOD_VISINA          0.3     // na ovoj visini ce se castati RayCast (NPC visina je 1.8, ali castamo od 0.3 da ne bi stalno udarali u zidove)
#define HOD_KORAK           4.0     // castamo 4 metra ispred NPCa, ako je zid tu lik ce otic u stranu
#define HOD_RAZMAK          1.5     // na kojoj udaljenosti ce stati od zida
#define HOD_KOLJENO         -0.5
#define HOD_DIONICA         10.0
#define HOD_MAX_KORAKA      20      // broj detour koraka nakon koji ponovno hoda ravno
#define HOD_VOZILO_RAZMAK   0.5
#define HOD_VOZILO_Z        3.0
#define HOD_VOZILO_DOMET    8.0
#define HOD_RAMENA          0.35
#define HOD_OSOBA           0.7
#define HOD_GLEDAJ          2.0
#define HOD_PROVJERA_MS     300
#define HOD_RACUNAJ_MS      600
#define HOD_CEKAJ           33
#define HOD_CELIJA          1.0
#define HOD_MREZA           48
#define HOD_CVOROVA         600
#define HOD_GOMILA          5000
#define HOD_PUT_MAX         24
#define HOD_STEPENICA       1.0
#define HOD_GLADI           16
#define HOD_PLAN_PAUZA_MS   3000
#define HOD_ISTI_CILJ       3.0
#define HOD_CILJ_OSOBA      1.5
#define HOD_TOCKA           0.8

enum E_HOD { bool:hdAktivan, bool:hdHoda, Float:hdX, Float:hdY, Float:hdZ, Float:hdKx, Float:hdKy, NPC_MOVE_TYPE:hdNacin, Float:hdBrzina, Float:hdDomet, hdKoraka, hdStrana, hdCeka, hdRacunao, hdPutN, hdPutI, hdPlanOd }
static gHod[MAX_PLAYERS][E_HOD];
static Float:gHodPut[MAX_PLAYERS][HOD_PUT_MAX][3];
static gHodGen, gHodOznaka[HOD_MREZA * HOD_MREZA], gHodOd[HOD_MREZA * HOD_MREZA], bool:gHodZatvoren[HOD_MREZA * HOD_MREZA], Float:gHodG[HOD_MREZA * HOD_MREZA], Float:gHodCz[HOD_MREZA * HOD_MREZA];
static gHodGomila[HOD_GOMILA], Float:gHodGomilaF[HOD_GOMILA], gHodGomilaN, gHodTrag[HOD_CVOROVA + 1];
static const gHodSmjer[8][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

InitHod()
{
    gHodGomilaN = 0;
    HodGurni(3, 3.0);
    HodGurni(1, 1.0);
    HodGurni(4, 4.0);
    HodGurni(2, 2.0);
    assert(HodIzvadi() == 1 && HodIzvadi() == 2 && HodIzvadi() == 3 && HodIzvadi() == 4 && !gHodGomilaN);
    SetTimer("HodProvjera", HOD_PROVJERA_MS, true);
}

static HodGurni(c, Float:f)
{
    if (gHodGomilaN >= HOD_GOMILA) return;
    new i = gHodGomilaN++;
    while (i > 0)
    {
        new r = (i - 1) / 2;
        if (gHodGomilaF[r] <= f) break;
        gHodGomila[i] = gHodGomila[r];
        gHodGomilaF[i] = gHodGomilaF[r];
        i = r;
    }
    gHodGomila[i] = c;
    gHodGomilaF[i] = f;
}

static HodIzvadi()
{
    new c = gHodGomila[0], n = --gHodGomilaN, i = 0, Float:f = gHodGomilaF[n];
    for (;;)
    {
        new d = i * 2 + 1;
        if (d >= n) break;
        if (d + 1 < n && gHodGomilaF[d + 1] < gHodGomilaF[d]) d++;
        if (gHodGomilaF[d] >= f) break;
        gHodGomila[i] = gHodGomila[d];
        gHodGomilaF[i] = gHodGomilaF[d];
        i = d;
    }
    gHodGomila[i] = gHodGomila[n];
    gHodGomilaF[i] = f;
    return c;
}

Float:HodTlo(Float:x, Float:y, Float:z)
{
    new Float:hx, Float:hy, Float:hz;
    if (Kolizija_Zraka(x, y, z + 1.0, x, y, z - 3.0, hx, hy, hz)) return hz + 1.0;
    return z;
}

NPCHodaj(npc, Float:x, Float:y, Float:z, NPC_MOVE_TYPE:nacin = NPC_MOVE_TYPE_WALK, Float:brzina = NPC_MOVE_SPEED_AUTO, Float:domet = 0.2)
{
    new n = gHod[npc][hdPutN];
    if (gHod[npc][hdHoda] && n && VectorSize(x - gHod[npc][hdX], y - gHod[npc][hdY], 0.0) < HOD_ISTI_CILJ)
    {
        if (gHodPut[npc][n - 1][0] == gHod[npc][hdX] && gHodPut[npc][n - 1][1] == gHod[npc][hdY])
        {
            gHodPut[npc][n - 1][0] = x;
            gHodPut[npc][n - 1][1] = y;
            gHodPut[npc][n - 1][2] = z;
        }
    }
    else
    {
        gHod[npc][hdPutN] = 0;
        gHod[npc][hdPutI] = 0;
        gHod[npc][hdPlanOd] = GetTickCount() - HOD_PLAN_PAUZA_MS;
    }
    gHod[npc][hdX] = x;
    gHod[npc][hdY] = y;
    gHod[npc][hdZ] = z;
    gHod[npc][hdNacin] = nacin;
    gHod[npc][hdBrzina] = brzina;
    gHod[npc][hdDomet] = domet;
    gHod[npc][hdKoraka] = 0;
    gHod[npc][hdCeka] = 0;
    gHod[npc][hdHoda] = true;
    if (!gHod[npc][hdStrana]) gHod[npc][hdStrana] = 1;
    HodKorak(npc);
}

static bool:HodPojas(Float:a, Float:b, Float:h, &Float:pocetak, &Float:kraj)
{
    new Float:d = b - a;
    if (floatabs(d) < 0.0001) return floatabs(a) <= h;
    new Float:t1 = (-h - a) / d, Float:t2 = (h - a) / d;
    if (t1 > t2)
    {
        new Float:t = t1;
        t1 = t2;
        t2 = t;
    }
    if (t1 > pocetak) pocetak = t1;
    if (t2 < kraj) kraj = t2;
    return pocetak <= kraj;
}

static Float:HodVozilo(Float:x, Float:y, Float:z, Float:px, Float:py)
{
    new Float:ux = px - x, Float:uy = py - y, Float:duljina = VectorSize(ux, uy, 0.0), Float:najblize = 2.0;
    for (new i = 0; i < gVozilaCount; i++)
    {
        new v = gVozila[i], Float:vx, Float:vy, Float:vz, Float:a, Float:sx, Float:sy, Float:sz;
        GetVehiclePos(v, vx, vy, vz);
        if (floatabs(vz - z) > HOD_VOZILO_Z || VectorSize(vx - x, vy - y, 0.0) > duljina + HOD_VOZILO_DOMET) continue;
        GetVehicleZAngle(v, a);
        GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
        new Float:c = floatcos(a, degrees), Float:s = floatsin(a, degrees), Float:sirina = sx / 2.0 + HOD_VOZILO_RAZMAK, Float:dugo = sy / 2.0 + HOD_VOZILO_RAZMAK;
        new Float:ax = (x - vx) * c + (y - vy) * s, Float:ay = (y - vy) * c - (x - vx) * s;
        new Float:bx = (px - vx) * c + (py - vy) * s, Float:by = (py - vy) * c - (px - vx) * s;
        if ((floatabs(ax) <= sirina && floatabs(ay) <= dugo) || (floatabs(bx) <= sirina && floatabs(by) <= dugo))
        {
            sirina = sx / 2.0;
            dugo = sy / 2.0;
            if ((floatabs(ax) <= sirina && floatabs(ay) <= dugo) || (floatabs(bx) <= sirina && floatabs(by) <= dugo)) continue;
        }
        new Float:pocetak = 0.0, Float:kraj = 1.0;
        if (HodPojas(ax, bx, sirina, pocetak, kraj) && HodPojas(ay, by, dugo, pocetak, kraj) && pocetak < najblize) najblize = pocetak;
    }
    return najblize;
}

static Float:HodOsobe(npc, Float:x, Float:y, Float:z, Float:px, Float:py)
{
    new Float:ux = px - x, Float:uy = py - y, Float:dd = ux * ux + uy * uy, Float:najblize = 2.0, svijet = GetPlayerVirtualWorld(npc);
    if (dd < 0.0001) return najblize;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (i == npc || !IsPlayerConnected(i) || GetPlayerVirtualWorld(i) != svijet || IsPlayerInAnyVehicle(i)) continue;
        new Float:ox, Float:oy, Float:oz;
        GetPlayerPos(i, ox, oy, oz);
        if (floatabs(oz - z) > 2.0) continue;
        new Float:t = ((ox - x) * ux + (oy - y) * uy) / dd;
        if (t <= 0.0 || t > 1.0) continue;
        if (VectorSize(x + ux * t - ox, y + uy * t - oy, 0.0) > HOD_OSOBA) continue;
        if (VectorSize(ox - x, oy - y, 0.0) < HOD_OSOBA) continue;
        if (VectorSize(ox - gHod[npc][hdX], oy - gHod[npc][hdY], 0.0) < HOD_CILJ_OSOBA) continue;
        if (t < najblize) najblize = t;
    }
    return najblize;
}

static bool:HodZraka(Float:x, Float:y, Float:z, Float:px, Float:py, Float:pz, &Float:hx, &Float:hy)
{
    new Float:ux = px - x, Float:uy = py - y, Float:d = VectorSize(ux, uy, 0.0), Float:najblize = 2.0, Float:tx, Float:ty, Float:tz;
    if (d < 0.0001) return false;
    new Float:nx = -uy / d * HOD_RAMENA, Float:ny = ux / d * HOD_RAMENA;
    for (new i = -1; i <= 1; i++)
    {
        new Float:ox = nx * float(i), Float:oy = ny * float(i);
        if (Kolizija_Zraka(x + ox, y + oy, z + HOD_VISINA, px + ox, py + oy, pz + HOD_VISINA, tx, ty, tz) || (!i && Kolizija_Zraka(x, y, z + HOD_KOLJENO, px, py, pz + HOD_KOLJENO, tx, ty, tz)))
        {
            new Float:t = VectorSize(tx - ox - x, ty - oy - y, 0.0) / d;
            if (t < najblize) najblize = t;
        }
    }
    if (najblize > 1.0) return false;
    hx = x + ux * najblize;
    hy = y + uy * najblize;
    return true;
}

static bool:HodZid(npc, Float:x, Float:y, Float:z, Float:px, Float:py, Float:pz, &Float:hx, &Float:hy)
{
    new Float:t = 2.0, Float:wx, Float:wy;
    if (HodZraka(x, y, z, px, py, pz, wx, wy)) t = VectorSize(wx - x, wy - y, 0.0) / VectorSize(px - x, py - y, 0.0);
    new Float:tv = HodVozilo(x, y, z, px, py), Float:to = HodOsobe(npc, x, y, z, px, py);
    if (tv < t) t = tv;
    if (to < t) t = to;
    if (t > 1.0) return false;
    hx = x + (px - x) * t;
    hy = y + (py - y) * t;
    return true;
}

static bool:HodPod(Float:x, Float:y, Float:z, &Float:tz)
{
    new Float:hx, Float:hy, Float:hz;
    if (!Kolizija_Zraka(x, y, z + 1.2, x, y, z - 2.5, hx, hy, hz)) return false;
    tz = hz + 1.0;
    return true;
}

static bool:HodPlaniraj(npc, Float:sx, Float:sy, Float:sz)
{
    new Float:tx = gHod[npc][hdX], Float:ty = gHod[npc][hdY], Float:hx, Float:hy;
    new si = clamp(HOD_MREZA / 2 - floatround((tx - sx) / (2.0 * HOD_CELIJA)), 0, HOD_MREZA - 1);
    new sj = clamp(HOD_MREZA / 2 - floatround((ty - sy) / (2.0 * HOD_CELIJA)), 0, HOD_MREZA - 1);
    new start = si * HOD_MREZA + sj, najbolji = start, bool:stigao = false;
    new Float:pocetna = VectorSize(tx - sx, ty - sy, 0.0), Float:najblize = pocetna;
    gHodGen++;
    gHodGomilaN = 0;
    gHodOznaka[start] = gHodGen;
    gHodZatvoren[start] = false;
    gHodG[start] = 0.0;
    gHodOd[start] = -1;
    gHodCz[start] = sz;
    HodGurni(start, pocetna);
    for (new n = 0; n < HOD_CVOROVA && gHodGomilaN; )
    {
        new c = HodIzvadi();
        if (gHodZatvoren[c]) continue;
        gHodZatvoren[c] = true;
        n++;
        new ci = c / HOD_MREZA, cj = c % HOD_MREZA, Float:cx = sx + float(ci - si) * HOD_CELIJA, Float:cy = sy + float(cj - sj) * HOD_CELIJA, Float:cz = gHodCz[c];
        new Float:h = VectorSize(tx - cx, ty - cy, 0.0);
        if (h < najblize)
        {
            najblize = h;
            najbolji = c;
        }
        if (h <= HOD_CELIJA)
        {
            stigao = true;
            break;
        }
        for (new k = 0; k < sizeof gHodSmjer; k++)
        {
            new ni = ci + gHodSmjer[k][0], nj = cj + gHodSmjer[k][1];
            if (ni < 0 || nj < 0 || ni >= HOD_MREZA || nj >= HOD_MREZA) continue;
            new s = ni * HOD_MREZA + nj, Float:g = gHodG[c] + (k < 4 ? HOD_CELIJA : HOD_CELIJA * 1.4142);
            if (gHodOznaka[s] == gHodGen && (gHodZatvoren[s] || g >= gHodG[s])) continue;
            new Float:nx = cx + float(gHodSmjer[k][0]) * HOD_CELIJA, Float:ny = cy + float(gHodSmjer[k][1]) * HOD_CELIJA, Float:nz;
            if (!HodPod(nx, ny, cz, nz) || floatabs(nz - cz) > HOD_STEPENICA || HodZraka(cx, cy, cz, nx, ny, nz, hx, hy)) continue;
            gHodOznaka[s] = gHodGen;
            gHodZatvoren[s] = false;
            gHodG[s] = g;
            gHodOd[s] = c;
            gHodCz[s] = nz;
            HodGurni(s, g + VectorSize(tx - nx, ty - ny, 0.0));
        }
    }
    if (najbolji == start || (!stigao && najblize > pocetna - 2.0)) return false;
    new n = 0;
    for (new c = najbolji; c != -1 && n < sizeof gHodTrag; c = gHodOd[c]) gHodTrag[n++] = c;
    new a = n - 1, m = 0;
    while (a > 0 && m < HOD_PUT_MAX)
    {
        new b = a - 1, Float:ax = sx + float(gHodTrag[a] / HOD_MREZA - si) * HOD_CELIJA, Float:ay = sy + float(gHodTrag[a] % HOD_MREZA - sj) * HOD_CELIJA;
        for (new k = a - HOD_GLADI > 0 ? a - HOD_GLADI : 0; k < a - 1; k++)
        {
            new Float:kx = sx + float(gHodTrag[k] / HOD_MREZA - si) * HOD_CELIJA, Float:ky = sy + float(gHodTrag[k] % HOD_MREZA - sj) * HOD_CELIJA;
            if (HodZraka(ax, ay, gHodCz[gHodTrag[a]], kx, ky, gHodCz[gHodTrag[k]], hx, hy)) continue;
            b = k;
            break;
        }
        gHodPut[npc][m][0] = sx + float(gHodTrag[b] / HOD_MREZA - si) * HOD_CELIJA;
        gHodPut[npc][m][1] = sy + float(gHodTrag[b] % HOD_MREZA - sj) * HOD_CELIJA;
        gHodPut[npc][m][2] = gHodCz[gHodTrag[b]];
        m++;
        a = b;
    }
    if (stigao && !a && m < HOD_PUT_MAX && !HodZraka(gHodPut[npc][m - 1][0], gHodPut[npc][m - 1][1], gHodPut[npc][m - 1][2], tx, ty, gHod[npc][hdZ], hx, hy))
    {
        gHodPut[npc][m][0] = tx;
        gHodPut[npc][m][1] = ty;
        gHodPut[npc][m][2] = gHod[npc][hdZ];
        m++;
    }
    gHod[npc][hdPutN] = m;
    gHod[npc][hdPutI] = 0;
    gHod[npc][hdKoraka] = 0;
    return true;
}

static bool:HodPlan(npc, Float:x, Float:y, Float:z)
{
    new sad = GetTickCount();
    if (sad - gHod[npc][hdPlanOd] < HOD_PLAN_PAUZA_MS) return false;
    if (HodPlaniraj(npc, x, y, z)) return true;
    gHod[npc][hdPutN] = 0;
    gHod[npc][hdPutI] = 0;
    gHod[npc][hdPlanOd] = sad;
    return false;
}

static HodCilj(npc, &Float:x, &Float:y, &Float:z)
{
    new i = gHod[npc][hdPutI];
    if (i < gHod[npc][hdPutN])
    {
        x = gHodPut[npc][i][0];
        y = gHodPut[npc][i][1];
        z = gHodPut[npc][i][2];
        return;
    }
    x = gHod[npc][hdX];
    y = gHod[npc][hdY];
    z = gHod[npc][hdZ];
}

static HodDionica(Float:x, Float:y, Float:z, Float:tx, Float:ty, Float:tz, &Float:px, &Float:py, &Float:pz)
{
    new Float:dx = tx - x, Float:dy = ty - y, Float:d = VectorSize(dx, dy, 0.0);
    px = tx;
    py = ty;
    pz = tz;
    if (d <= HOD_DIONICA) return;
    px = x + dx / d * HOD_DIONICA;
    py = y + dy / d * HOD_DIONICA;
    pz = HodTlo(px, py, z + (tz - z) * HOD_DIONICA / d);
}

static HodKorak(npc)
{
    new Float:x, Float:y, Float:z, Float:hx, Float:hy, Float:tx, Float:ty, Float:tz, Float:px, Float:py, Float:pz;
    NPC_GetPos(npc, x, y, z);
    gHod[npc][hdRacunao] = GetTickCount();
    while (gHod[npc][hdPutI] < gHod[npc][hdPutN] && VectorSize(gHodPut[npc][gHod[npc][hdPutI]][0] - x, gHodPut[npc][gHod[npc][hdPutI]][1] - y, 0.0) < HOD_TOCKA)
    {
        gHod[npc][hdPutI]++;
        gHod[npc][hdKoraka] = 0;
    }
    HodCilj(npc, tx, ty, tz);
    HodDionica(x, y, z, tx, ty, tz, px, py, pz);
    if (HodZraka(x, y, z, px, py, pz, hx, hy) && HodPlan(npc, x, y, z))
    {
        HodCilj(npc, tx, ty, tz);
        HodDionica(x, y, z, tx, ty, tz, px, py, pz);
    }
    new Float:dx = tx - x, Float:dy = ty - y, Float:d = VectorSize(dx, dy, 0.0);
    if (gHod[npc][hdKoraka] < HOD_MAX_KORAKA && HodZid(npc, x, y, z, px, py, pz, hx, hy))
    {
        new Float:zid = VectorSize(hx - x, hy - y, 0.0);
        gHod[npc][hdKoraka]++;
        if (zid > HOD_KORAK)
        {
            px = x + dx / d * (zid - HOD_RAZMAK);
            py = y + dy / d * (zid - HOD_RAZMAK);
            pz = HodTlo(px, py, z);
        }
        else
        {
            new Float:a = atan2(dy, dx), bool:ima = false, Float:korak = d > HOD_KORAK ? HOD_KORAK : d < HOD_RAZMAK ? HOD_RAZMAK : d;
            for (new k = 0; k < 10 && !ima; k++)
            {
                new Float:kut = a + float(30 + 30 * (k / 2)) * float(k % 2 ? -gHod[npc][hdStrana] : gHod[npc][hdStrana]);
                px = x + korak * floatcos(kut, degrees);
                py = y + korak * floatsin(kut, degrees);
                pz = HodTlo(px, py, z);
                if (HodZid(npc, x, y, z, px, py, pz, hx, hy)) continue;
                if (k % 2) gHod[npc][hdStrana] = -gHod[npc][hdStrana];
                ima = true;
            }
            if (!ima)
            {
                if (gHod[npc][hdCeka]++ < HOD_CEKAJ)
                {
                    gHod[npc][hdKoraka]--;
                    gHod[npc][hdAktivan] = true;
                    gHod[npc][hdKx] = x;
                    gHod[npc][hdKy] = y;
                    NPC_StopMove(npc);
                    return;
                }
                px = tx;
                py = ty;
                pz = tz;
            }
        }
    }
    gHod[npc][hdCeka] = 0;
    gHod[npc][hdAktivan] = px != gHod[npc][hdX] || py != gHod[npc][hdY];
    gHod[npc][hdKx] = px;
    gHod[npc][hdKy] = py;
    NPC_Move(npc, px, py, pz, gHod[npc][hdNacin], gHod[npc][hdBrzina], gHod[npc][hdAktivan] ? 0.3 : gHod[npc][hdDomet]);
}

forward HodProvjera();
public HodProvjera()
{
    new sad = GetTickCount();
    for (new npc = 0, npcZadnji = gIgracZadnji; npc <= npcZadnji; npc++)
    {
        if (!gHod[npc][hdHoda]) continue;
        if (!IsPlayerConnected(npc) || !IsPlayerNPC(npc) || IsPlayerInAnyVehicle(npc))
        {
            gHod[npc][hdHoda] = false;
            gHod[npc][hdAktivan] = false;
            continue;
        }
        if (NPC_IsEnteringVehicle(npc)) continue;
        if (gHod[npc][hdCeka])
        {
            HodKorak(npc);
            continue;
        }
        if (!NPC_IsMoving(npc) || sad - gHod[npc][hdRacunao] < HOD_RACUNAJ_MS) continue;
        new Float:x, Float:y, Float:z, Float:hx, Float:hy;
        NPC_GetPos(npc, x, y, z);
        new Float:dx = gHod[npc][hdKx] - x, Float:dy = gHod[npc][hdKy] - y, Float:d = VectorSize(dx, dy, 0.0);
        if (d < 0.5) continue;
        new Float:korak = d < HOD_GLEDAJ ? d : HOD_GLEDAJ;
        if (HodZid(npc, x, y, z, x + dx / d * korak, y + dy / d * korak, z, hx, hy)) HodKorak(npc);
    }
}

bool:NPCStigao(npc)
{
    if (!gHod[npc][hdAktivan])
    {
        gHod[npc][hdHoda] = false;
        return false;
    }
    if (gHod[npc][hdCeka]) return true;
    new Float:x, Float:y, Float:z;
    NPC_GetPos(npc, x, y, z);
    if (VectorSize(x - gHod[npc][hdKx], y - gHod[npc][hdKy], 0.0) > 2.0)
    {
        gHod[npc][hdAktivan] = false;
        gHod[npc][hdHoda] = false;
        return false;
    }
    HodKorak(npc);
    return true;
}

bool:NPCStop(npc)
{
    gHod[npc][hdAktivan] = false;
    gHod[npc][hdHoda] = false;
    gHod[npc][hdCeka] = 0;
    gHod[npc][hdPutN] = 0;
    gHod[npc][hdPutI] = 0;
    return NPC_StopMove(npc);
}
#define NPC_StopMove NPCStop
