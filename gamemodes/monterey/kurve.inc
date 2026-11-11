#define MAX_KURVI           16
#define KURVA_CIJENA        30
#define KURVA_TRUBA         8.0
#define KURVA_BJEG          25.0
#define KURVA_TRAJANJE_MS   12000
#define KURVA_PONUDA_MS     45000
#define KURVA_PROZOR        1.9
#define KURVA_ULAZAK_MS     8000
#define KURVA_IZLAZAK_MS    2000
#define KURVA_IZLAZAK_ROK   4000
#define KURVA_NJIHANJE      0.01
#define KURVA_NJIHANJE_MS   400
#define KURVA_PJESKE        3.0
#define KURVA_RAZMAK        0.9

enum { PR_NEMA, PR_STOJI, PR_DOLAZI, PR_PONUDA, PR_ULAZI, PR_USLUGA, PR_IZLAZI, PR_ODLAZI, PR_KRAJ, PR_ULICA_PONUDA, PR_ULICA }
enum E_KURVA { prStanje, prDb, prSkin, prNpc, prKupac, prVozilo, prRok, bool:prAnim, Float:prX, Float:prY, Float:prZ, Float:prA }
static gKurva[MAX_KURVI][E_KURVA];
static gKurvaOd[MAX_PLAYERS];
static gNpcKurva[MAX_PLAYERS];
static gApKurvaDb[MAX_PLAYERS];
static const gKurvaSkin[] = { 63, 64, 85, 152, 178, 207, 237, 238 };

static KurvaStvori(i)
{
    new ime[MAX_PLAYER_NAME + 1];
    format(ime, sizeof ime, "Prost_%d", gKurva[i][prDb]);
    new npc = NPC_Create(ime);
    gKurva[i][prNpc] = npc;
    if (npc == INVALID_PLAYER_ID) return;
    gNpcKurva[npc] = i + 1;
    NPC_Spawn(npc);
}

InitKurve()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS hooker_spots (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, skin INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, skin FROM hooker_spots ORDER BY id LIMIT %d", MAX_KURVI), i = 0;
    for (new n = DB_GetRowCount(res); n > 0; n--)
    {
        gKurva[i][prDb] = DB_GetFieldInt(res, 0);
        gKurva[i][prX] = DB_GetFieldFloat(res, 1);
        gKurva[i][prY] = DB_GetFieldFloat(res, 2);
        gKurva[i][prZ] = DB_GetFieldFloat(res, 3);
        gKurva[i][prA] = DB_GetFieldFloat(res, 4);
        gKurva[i][prSkin] = DB_GetFieldInt(res, 5);
        gKurva[i][prStanje] = PR_STOJI;
        KurvaStvori(i++);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static KurvaGovor(p, const tekst[], const a[] = "", const b[] = "")
{
    PosaljiBlizu(p, CHAT_RANGE, COLOR_CHAT, tekst, a, b, 1);
}

static KurvaMotor(v, motor)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    SetVehicleParamsEx(v, motor, l, a, d, bn, bt, o);
}

static bool:KurvaMozeAuto(v)
{
    return !BezKrova(GetVehicleModel(v)) && GetVehicleSeats(v) >= 1 && GetVehicleOccupant(v, 1) == INVALID_PLAYER_ID;
}

static KurvaProzor(v, &Float:x, &Float:y, &Float:z)
{
    new Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    x += floatcos(a, degrees) * KURVA_PROZOR;
    y += floatsin(a, degrees) * KURVA_PROZOR;
}

KurvaUlica(playerid)
{
    if (!gUlogiran[playerid] || gKurvaOd[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    for (new i = 0; i < MAX_KURVI; i++)
    {
        if (gKurva[i][prStanje] != PR_STOJI || gKurva[i][prNpc] == INVALID_PLAYER_ID || GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, KURVA_PJESKE, gKurva[i][prX], gKurva[i][prY], gKurva[i][prZ])) continue;
        new cijena[12], body[64];
        gKurva[i][prStanje] = PR_ULICA_PONUDA;
        gKurva[i][prKupac] = playerid;
        gKurva[i][prRok] = GetTickCount() + KURVA_PONUDA_MS;
        gKurvaOd[playerid] = i + 1;
        valstr(cijena, KURVA_CIJENA);
        KurvaGovor(playerid, "Kurva kaze: Hej dragi, za $%s ti uljepsam dan. Sto kazes?", cijena);
        format(body, sizeof body, T(playerid, "Cijena: $%d"), KURVA_CIJENA);
        PrikaziDialog(playerid, DLG_KURVA, DIALOG_STYLE_MSGBOX, T(playerid, "Kurva"), body, T(playerid, "Prihvati"), T(playerid, "Odbij"));
        return 1;
    }
    return 0;
}

static KurvaUlicaKraj(i)
{
    new p = gKurva[i][prKupac];
    gKurvaOd[p] = 0;
    gKurva[i][prKupac] = INVALID_PLAYER_ID;
    if (gKurva[i][prStanje] == PR_ULICA)
    {
        TogglePlayerControllable(p, true);
        ClearAnimations(p);
        NPC_ClearAnimations(gKurva[i][prNpc]);
    }
    gKurva[i][prStanje] = PR_STOJI;
}

static KurvaUlicaPocni(i, p)
{
    new Float:a = gKurva[i][prA];
    gKurva[i][prStanje] = PR_ULICA;
    gKurva[i][prRok] = GetTickCount() + KURVA_TRAJANJE_MS;
    SetPlayerPos(p, gKurva[i][prX] - floatsin(a, degrees) * KURVA_RAZMAK, gKurva[i][prY] + floatcos(a, degrees) * KURVA_RAZMAK, gKurva[i][prZ]);
    SetPlayerFacingAngle(p, a + 180.0);
    SetCameraBehindPlayer(p);
    TogglePlayerControllable(p, false);
    ApplyAnimation(p, "BLOWJOBZ", "BJ_STAND_LOOP_P", 4.1, true, false, false, false, KURVA_TRAJANJE_MS, SYNC_ALL);
    NPC_ApplyAnimation(gKurva[i][prNpc], "BLOWJOBZ", "BJ_STAND_LOOP_W", 4.1, true, false, false, false, KURVA_TRAJANJE_MS);
}

CMD:kurva(playerid, params[])
{
    if (!KurvaUlica(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod kurve."));
    return 1;
}

CMD:hooker(playerid, params[]) return cmd_kurva(playerid, params);
CMD:whore(playerid, params[]) return cmd_kurva(playerid, params);
CMD:shlyuha(playerid, params[]) return cmd_kurva(playerid, params);
CMD:puta(playerid, params[]) return cmd_kurva(playerid, params);
CMD:fahise(playerid, params[]) return cmd_kurva(playerid, params);

KurvaPotrubiKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!(newkeys & KEY_CROUCH) || (oldkeys & KEY_CROUCH) || !gUlogiran[playerid] || gKurvaOd[playerid] || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
    new v = GetPlayerVehicleID(playerid), Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(v, vx, vy, vz);
    if (VectorSize(vx, vy, vz) > 0.1 || !KurvaMozeAuto(v)) return 1;
    for (new i = 0; i < MAX_KURVI; i++)
    {
        new npc = gKurva[i][prNpc], Float:x, Float:y, Float:z;
        if (gKurva[i][prStanje] != PR_STOJI || npc == INVALID_PLAYER_ID || !IsPlayerInRangeOfPoint(playerid, KURVA_TRUBA, gKurva[i][prX], gKurva[i][prY], gKurva[i][prZ])) continue;
        gKurva[i][prKupac] = playerid;
        gKurva[i][prVozilo] = v;
        gKurva[i][prStanje] = PR_DOLAZI;
        gKurva[i][prRok] = GetTickCount() + KURVA_PONUDA_MS;
        gKurva[i][prAnim] = false;
        gKurvaOd[playerid] = i + 1;
        KurvaProzor(v, x, y, z);
        NPCHodaj(npc, x, y, z, NPC_MOVE_TYPE_WALK);
        return 1;
    }
    return 1;
}

KurvaSpawn(npcid)
{
    new i = gNpcKurva[npcid] - 1;
    if (i < 0) return;
    NPC_SetSkin(npcid, gKurva[i][prSkin]);
    NPC_SetInvulnerable(npcid, true);
    NPC_SetPos(npcid, gKurva[i][prX], gKurva[i][prY], gKurva[i][prZ]);
    NPC_SetFacingAngle(npcid, gKurva[i][prA]);
}

static KurvaVrati(i)
{
    new p = gKurva[i][prKupac], npc = gKurva[i][prNpc], v = gKurva[i][prVozilo], st = gKurva[i][prStanje], Float:x, Float:y, Float:z;
    if (st == PR_ULICA_PONUDA || st == PR_ULICA)
    {
        KurvaUlicaKraj(i);
        return;
    }
    gKurvaOd[p] = 0;
    gKurva[i][prKupac] = INVALID_PLAYER_ID;
    if (st == PR_USLUGA)
    {
        NPC_ExitVehicle(npc);
        gKurva[i][prStanje] = PR_IZLAZI;
        gKurva[i][prRok] = GetTickCount() + KURVA_IZLAZAK_MS;
        return;
    }
    if (st == PR_ULAZI)
    {
        NPC_RemoveFromVehicle(npc);
        KurvaProzor(v, x, y, z);
        NPC_SetPos(npc, x, y, z);
        KurvaMotor(v, 1);
    }
    gKurva[i][prStanje] = PR_ODLAZI;
    NPCHodaj(npc, gKurva[i][prX], gKurva[i][prY], gKurva[i][prZ], NPC_MOVE_TYPE_WALK);
}

KurvaPomak(npcid)
{
    new i = gNpcKurva[npcid] - 1;
    if (i < 0) return;
    if (gKurva[i][prStanje] == PR_ODLAZI) gKurva[i][prStanje] = PR_KRAJ;
    if (gKurva[i][prStanje] != PR_DOLAZI) return;
    new p = gKurva[i][prKupac], cijena[12], body[64], Float:nx, Float:ny, Float:nz, Float:vx, Float:vy, Float:vz;
    gKurva[i][prStanje] = PR_PONUDA;
    gKurva[i][prRok] = GetTickCount() + KURVA_PONUDA_MS;
    NPC_GetPos(npcid, nx, ny, nz);
    GetVehiclePos(gKurva[i][prVozilo], vx, vy, vz);
    NPC_SetFacingAngle(npcid, atan2(vy - ny, vx - nx) - 90.0);
    valstr(cijena, KURVA_CIJENA);
    KurvaGovor(p, "Kurva kaze: Hej dragi, za $%s ti uljepsam dan. Sto kazes?", cijena);
    format(body, sizeof body, T(p, "Cijena: $%d"), KURVA_CIJENA);
    PrikaziDialog(p, DLG_KURVA, DIALOG_STYLE_MSGBOX, T(p, "Kurva"), body, T(p, "Prihvati"), T(p, "Odbij"));
}

DLG:kurva(playerid, response)
{
    new i = gKurvaOd[playerid] - 1;
    if (i >= 0 && gKurva[i][prStanje] == PR_ULICA_PONUDA && gKurva[i][prKupac] == playerid)
    {
        if (!response)
        {
            KurvaGovor(playerid, "%s kaze: Ne hvala.", GetRPIme(playerid));
            KurvaGovor(playerid, "Kurva kaze: Kako hoces, dragi.");
            KurvaUlicaKraj(i);
        }
        else if (!DajNovac(playerid, -KURVA_CIJENA))
        {
            KurvaGovor(playerid, "Kurva kaze: Nemas ti toliko, dragi.");
            KurvaUlicaKraj(i);
        }
        else KurvaUlicaPocni(i, playerid);
        return 1;
    }
    if (i < 0 || gKurva[i][prStanje] != PR_PONUDA || gKurva[i][prKupac] != playerid) return 1;
    new v = gKurva[i][prVozilo];
    if (!response || GetVehicleOccupant(v, 1) != INVALID_PLAYER_ID)
    {
        KurvaGovor(playerid, "%s kaze: Ne hvala.", GetRPIme(playerid));
        KurvaGovor(playerid, "Kurva kaze: Kako hoces, dragi.");
        KurvaVrati(i);
        return 1;
    }
    if (!DajNovac(playerid, -KURVA_CIJENA))
    {
        KurvaGovor(playerid, "Kurva kaze: Nemas ti toliko, dragi.");
        KurvaVrati(i);
        return 1;
    }
    KurvaGovor(playerid, "%s kaze: Upadaj.", GetRPIme(playerid));
    KurvaMotor(v, 0);
    NPC_EnterVehicle(gKurva[i][prNpc], v, 1, NPC_MOVE_TYPE_WALK);
    gKurva[i][prStanje] = PR_ULAZI;
    gKurva[i][prRok] = GetTickCount() + KURVA_ULAZAK_MS;
    return 1;
}

KurvaDisconnect(playerid)
{
    if (gKurvaOd[playerid]) KurvaVrati(gKurvaOd[playerid] - 1);
}

forward KurvaNjihaj(i, smjer);
public KurvaNjihaj(i, smjer)
{
    if (gKurva[i][prStanje] != PR_USLUGA) return;
    new Float:a, v = gKurva[i][prVozilo];
    GetVehicleZAngle(v, a);
    SetVehicleAngularVelocity(v, -floatsin(a, degrees) * KURVA_NJIHANJE * float(smjer), floatcos(a, degrees) * KURVA_NJIHANJE * float(smjer), 0.0);
    SetTimerEx("KurvaNjihaj", KURVA_NJIHANJE_MS, false, "ii", i, -smjer);
}

KurvaTick()
{
    new sad = GetTickCount();
    for (new i = 0; i < MAX_KURVI; i++)
    {
        new st = gKurva[i][prStanje], p = gKurva[i][prKupac], npc = gKurva[i][prNpc];
        if (st == PR_ULICA_PONUDA || st == PR_ULICA)
        {
            if (sad - gKurva[i][prRok] < 0) continue;
            KurvaGovor(p, st == PR_ULICA ? ("Kurva kaze: Hvala, dragi. Navrati opet.") : ("Kurva kaze: Nemam cijelu noc, dragi."));
            KurvaUlicaKraj(i);
        }
        else if (st == PR_KRAJ)
        {
            NPC_SetFacingAngle(npc, gKurva[i][prA]);
            gKurva[i][prStanje] = PR_STOJI;
        }
        else if (st == PR_IZLAZI && sad - gKurva[i][prRok] >= 0)
        {
            new v = gKurva[i][prVozilo], Float:x, Float:y, Float:z, bool:sjedi = NPC_GetVehicle(npc) == v;
            if (sjedi && sad - gKurva[i][prRok] < KURVA_IZLAZAK_ROK) continue;
            if (sjedi)
            {
                NPC_RemoveFromVehicle(npc);
                KurvaProzor(v, x, y, z);
                NPC_SetPos(npc, x, y, z);
            }
            KurvaMotor(v, 1);
            gKurva[i][prStanje] = PR_ODLAZI;
            NPCHodaj(npc, gKurva[i][prX], gKurva[i][prY], gKurva[i][prZ], NPC_MOVE_TYPE_WALK);
        }
        else if (st == PR_DOLAZI || st == PR_PONUDA || st == PR_ULAZI || st == PR_USLUGA)
        {
            if (!IsPlayerInVehicle(p, gKurva[i][prVozilo]) || !IsPlayerInRangeOfPoint(p, KURVA_BJEG, gKurva[i][prX], gKurva[i][prY], gKurva[i][prZ])) KurvaVrati(i);
            else if (st == PR_ULAZI)
            {
                new bool:sjedi = NPC_GetVehicle(npc) == gKurva[i][prVozilo] && !NPC_IsEnteringVehicle(npc);
                if (!sjedi && sad - gKurva[i][prRok] < 0) continue;
                if (!sjedi) NPC_PutInVehicle(npc, gKurva[i][prVozilo], 1);
                gKurva[i][prStanje] = PR_USLUGA;
                gKurva[i][prRok] = sad + KURVA_TRAJANJE_MS;
            }
            else if (sad - gKurva[i][prRok] >= 0)
            {
                if (st == PR_USLUGA) KurvaGovor(p, "Kurva kaze: Hvala, dragi. Trubi kad god pozelis.");
                else KurvaGovor(p, "Kurva kaze: Nemam cijelu noc, dragi.");
                KurvaVrati(i);
            }
            else if (st == PR_USLUGA && !gKurva[i][prAnim])
            {
                gKurva[i][prAnim] = true;
                ApplyAnimation(p, "BLOWJOBZ", "BJ_CAR_LOOP_P", 4.1, true, false, false, false, KURVA_TRAJANJE_MS, SYNC_ALL);
                NPC_ApplyAnimation(npc, "BLOWJOBZ", "BJ_CAR_LOOP_W", 4.1, true, false, false, false, KURVA_TRAJANJE_MS);
                KurvaNjihaj(i, 1);
            }
        }
    }
}

static KurvaBroj()
{
    new n = 0;
    for (new i = 0; i < MAX_KURVI; i++) if (gKurva[i][prStanje] != PR_NEMA) n++;
    return n;
}

ShowKurve(playerid)
{
    new list[1024], row[64], title[64], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova na mojoj poziciji"));
    for (new i = 0; i < MAX_KURVI; i++)
    {
        if (gKurva[i][prStanje] == PR_NEMA) continue;
        format(row, sizeof row, "[%d] %s", gKurva[i][prDb], CestaZona(gKurva[i][prX], gKurva[i][prY]));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, T(playerid, "Kurve (%d/%d)"), KurvaBroj(), MAX_KURVI);
    return PrikaziDialog(playerid, DLG_APANEL_KURVA, DIALOG_STYLE_LIST, title, list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static NovaKurva(playerid)
{
    new i = 0, Float:x, Float:y, Float:z, Float:a;
    while (i < MAX_KURVI && gKurva[i][prStanje] != PR_NEMA) i++;
    if (i == MAX_KURVI) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dostignut je limit kurvi (%d)."), MAX_KURVI);
    if (IsPlayerInAnyVehicle(playerid) || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kurva se ovdje ne moze stvoriti."));
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    new skin = gKurvaSkin[random(sizeof gKurvaSkin)];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO hooker_spots (x, y, z, a, skin) VALUES (%f, %f, %f, %f, %d)", x, y, z, a, skin));
    gKurva[i][prDb] = ZadnjiId();
    gKurva[i][prX] = x;
    gKurva[i][prY] = y;
    gKurva[i][prZ] = z;
    gKurva[i][prA] = a;
    gKurva[i][prSkin] = skin;
    gKurva[i][prStanje] = PR_STOJI;
    KurvaStvori(i);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kurva dodana: %s."), CestaZona(x, y));
}

DLG:apanel_kurve(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        NovaKurva(playerid);
        return ShowKurve(playerid);
    }
    new n = listitem;
    for (new i = 0; i < MAX_KURVI; i++)
    {
        if (gKurva[i][prStanje] == PR_NEMA || --n) continue;
        new body[96];
        gApKurvaDb[playerid] = gKurva[i][prDb];
        format(body, sizeof body, T(playerid, "Obrisati kurvu: %s?"), CestaZona(gKurva[i][prX], gKurva[i][prY]));
        return PrikaziDialog(playerid, DLG_APANEL_KURVA_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Kurve"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowKurve(playerid);
}

DLG:apanel_kurve_brisi(playerid, response)
{
    if (!response) return ShowKurve(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new i = 0;
    while (i < MAX_KURVI && (gKurva[i][prStanje] == PR_NEMA || gKurva[i][prDb] != gApKurvaDb[playerid])) i++;
    if (i == MAX_KURVI) return ShowKurve(playerid);
    if (gKurva[i][prStanje] != PR_STOJI)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kurva je trenutno zauzeta, pokusajte kasnije."));
        return ShowKurve(playerid);
    }
    if (gKurva[i][prNpc] != INVALID_PLAYER_ID)
    {
        gNpcKurva[gKurva[i][prNpc]] = 0;
        NPC_Destroy(gKurva[i][prNpc]);
    }
    gKurva[i][prStanje] = PR_NEMA;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM hooker_spots WHERE id = %d", gApKurvaDb[playerid]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kurva obrisana."));
    return ShowKurve(playerid);
}
