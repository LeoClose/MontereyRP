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

enum E_HOD { bool:hdAktivan, bool:hdHoda, Float:hdX, Float:hdY, Float:hdZ, Float:hdKx, Float:hdKy, NPC_MOVE_TYPE:hdNacin, Float:hdBrzina, Float:hdDomet, hdKoraka, hdStrana, hdCeka, hdRacunao }
static gHod[MAX_PLAYERS][E_HOD];

InitHod()
{
    SetTimer("HodProvjera", HOD_PROVJERA_MS, true);
}

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
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (i == npc || !IsPlayerConnected(i) || GetPlayerVirtualWorld(i) != svijet || IsPlayerInAnyVehicle(i)) continue;
        new Float:ox, Float:oy, Float:oz;
        GetPlayerPos(i, ox, oy, oz);
        if (floatabs(oz - z) > 2.0) continue;
        new Float:t = ((ox - x) * ux + (oy - y) * uy) / dd;
        if (t <= 0.0 || t > 1.0) continue;
        if (VectorSize(x + ux * t - ox, y + uy * t - oy, 0.0) > HOD_OSOBA) continue;
        if (VectorSize(ox - x, oy - y, 0.0) < HOD_OSOBA) continue;
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

static HodKorak(npc)
{
    new Float:x, Float:y, Float:z, Float:hx, Float:hy;
    new Float:tx = gHod[npc][hdX], Float:ty = gHod[npc][hdY], Float:tz = gHod[npc][hdZ];
    NPC_GetPos(npc, x, y, z);
    gHod[npc][hdRacunao] = GetTickCount();
    new Float:dx = tx - x, Float:dy = ty - y, Float:d = VectorSize(dx, dy, 0.0);
    new Float:px = tx, Float:py = ty, Float:pz = tz;
    if (d > HOD_DIONICA)
    {
        px = x + dx / d * HOD_DIONICA;
        py = y + dy / d * HOD_DIONICA;
        pz = HodTlo(px, py, z + (tz - z) * HOD_DIONICA / d);
    }
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
    gHod[npc][hdAktivan] = px != tx || py != ty;
    gHod[npc][hdKx] = px;
    gHod[npc][hdKy] = py;
    NPC_Move(npc, px, py, pz, gHod[npc][hdNacin], gHod[npc][hdBrzina], gHod[npc][hdAktivan] ? 0.3 : gHod[npc][hdDomet]);
}

forward HodProvjera();
public HodProvjera()
{
    new sad = GetTickCount();
    for (new npc = 0; npc < MAX_PLAYERS; npc++)
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
    return NPC_StopMove(npc);
}
#define NPC_StopMove NPCStop
