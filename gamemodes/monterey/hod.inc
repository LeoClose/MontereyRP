// Ovo je kod za nase NPCeve kako se ne bi zabijali u zidove
#define HOD_VISINA          0.3     // na ovoj visini ce se castati RayCast (NPC visina je 1.8, ali castamo od 0.3 da ne bi stalno udarali u zidove)
#define HOD_KORAK           4.0     // castamo 4 metra ispred NPCa, ako je zid tu lik ce otic u stranu
#define HOD_RAZMAK          1.5     // na kojoj udaljenosti ce stati od zida
#define HOD_KOLJENO         -0.5
#define HOD_DIONICA         10.0
#define HOD_MAX_KORAKA      12      // broj detour koraka nakon koji ponovno hoda ravno
#define HOD_VOZILO_RAZMAK   0.5
#define HOD_VOZILO_Z        3.0
#define HOD_VOZILO_DOMET    8.0

enum E_HOD { bool:hdAktivan, Float:hdX, Float:hdY, Float:hdZ, Float:hdKx, Float:hdKy, NPC_MOVE_TYPE:hdNacin, Float:hdBrzina, Float:hdDomet, hdKoraka, hdStrana }
static gHod[MAX_PLAYERS][E_HOD];

Float:HodTlo(Float:x, Float:y, Float:z)
{
    new Float:hx, Float:hy, Float:hz;
    if (Kolizija_Zraka(x, y, z + 1.0, x, y, z - 3.0, hx, hy, hz)) return hz + 1.0;
    return z;
}

NPCHodaj(npc, Float:x, Float:y, Float:z, NPC_MOVE_TYPE:nacin = NPC_MOVE_TYPE_WALK, Float:brzina = NPC_MOVE_SPEED_AUTO, Float:domet = 0.2)
{
    gHod[npc][hdX] = x;
    gHod[npc][hdY] = y;
    gHod[npc][hdZ] = z;
    gHod[npc][hdNacin] = nacin;
    gHod[npc][hdBrzina] = brzina;
    gHod[npc][hdDomet] = domet;
    gHod[npc][hdKoraka] = 0;
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

static bool:HodVozilo(Float:x, Float:y, Float:z, Float:px, Float:py, &Float:hx, &Float:hy)
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
        if ((floatabs(ax) <= sirina && floatabs(ay) <= dugo) || (floatabs(bx) <= sirina && floatabs(by) <= dugo)) continue;
        new Float:pocetak = 0.0, Float:kraj = 1.0;
        if (HodPojas(ax, bx, sirina, pocetak, kraj) && HodPojas(ay, by, dugo, pocetak, kraj) && pocetak < najblize) najblize = pocetak;
    }
    if (najblize > 1.0) return false;
    hx = x + ux * najblize;
    hy = y + uy * najblize;
    return true;
}

static bool:HodZid(Float:x, Float:y, Float:z, Float:px, Float:py, Float:pz, &Float:hx, &Float:hy)
{
    new Float:hz, Float:vx, Float:vy;
    new bool:zid = Kolizija_Zraka(x, y, z + HOD_VISINA, px, py, pz + HOD_VISINA, hx, hy, hz) || Kolizija_Zraka(x, y, z + HOD_KOLJENO, px, py, pz + HOD_KOLJENO, hx, hy, hz);
    if (!HodVozilo(x, y, z, px, py, vx, vy)) return zid;
    if (zid && VectorSize(hx - x, hy - y, 0.0) <= VectorSize(vx - x, vy - y, 0.0)) return true;
    hx = vx;
    hy = vy;
    return true;
}

static HodKorak(npc)
{
    new Float:x, Float:y, Float:z, Float:hx, Float:hy;
    new Float:tx = gHod[npc][hdX], Float:ty = gHod[npc][hdY], Float:tz = gHod[npc][hdZ];
    NPC_GetPos(npc, x, y, z);
    new Float:dx = tx - x, Float:dy = ty - y, Float:d = VectorSize(dx, dy, 0.0);
    new Float:px = tx, Float:py = ty, Float:pz = tz;
    if (d > HOD_DIONICA)
    {
        px = x + dx / d * HOD_DIONICA;
        py = y + dy / d * HOD_DIONICA;
        pz = HodTlo(px, py, z + (tz - z) * HOD_DIONICA / d);
    }
    if (gHod[npc][hdKoraka] < HOD_MAX_KORAKA && HodZid(x, y, z, px, py, pz, hx, hy))
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
            new Float:a = atan2(dy, dx), bool:ima = false;
            for (new k = 0; k < 6 && !ima; k++)
            {
                new Float:kut = a + float(45 + 45 * (k / 2)) * float(k % 2 ? -gHod[npc][hdStrana] : gHod[npc][hdStrana]);
                px = x + HOD_KORAK * floatcos(kut, degrees);
                py = y + HOD_KORAK * floatsin(kut, degrees);
                pz = HodTlo(px, py, z);
                if (HodZid(x, y, z, px, py, pz, hx, hy)) continue;
                if (k % 2) gHod[npc][hdStrana] = -gHod[npc][hdStrana];
                ima = true;
            }
            if (!ima)
            {
                px = tx;
                py = ty;
                pz = tz;
            }
        }
    }
    gHod[npc][hdAktivan] = px != tx || py != ty;
    gHod[npc][hdKx] = px;
    gHod[npc][hdKy] = py;
    NPC_Move(npc, px, py, pz, gHod[npc][hdNacin], gHod[npc][hdBrzina], gHod[npc][hdAktivan] ? 0.3 : gHod[npc][hdDomet]);
}

bool:NPCStigao(npc)
{
    if (!gHod[npc][hdAktivan]) return false;
    new Float:x, Float:y, Float:z;
    NPC_GetPos(npc, x, y, z);
    if (VectorSize(x - gHod[npc][hdKx], y - gHod[npc][hdKy], 0.0) > 2.0) { gHod[npc][hdAktivan] = false; return false; }
    HodKorak(npc);
    return true;
}

bool:NPCStop(npc)
{
    gHod[npc][hdAktivan] = false;
    return NPC_StopMove(npc);
}
#define NPC_StopMove NPCStop
