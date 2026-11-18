static KuglanaPloca(Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, txd, const lib[], const tex[], boja = 0)
{
    new STREAMER_TAG_OBJECT:o = CreateDynamicObject(19377, x, y, z, rx, ry, rz, -1, KUGLANA_INT, -1, 200.0, 200.0);
    SetDynamicObjectMaterial(o, 0, txd, lib, tex, boja);
}

static KuglanaObjekt(model, Float:x, Float:y, Float:z, Float:rz)
{
    CreateDynamicObject(model, KUGLANA_X + x, KUGLANA_Y + y, KUGLANA_Z + z, 0.0, 0.0, rz, -1, KUGLANA_INT, -1, 200.0, 200.0);
}

MapaKuglana()
{
    new const Float:X = KUGLANA_X, Float:Y = KUGLANA_Y, Float:Z = KUGLANA_Z;
    KuglanaPloca(X, Y - 4.8, Z - 0.05, 0.0, 90.0, 0.0, 19595, "lsappartments1", "carpet4-256x256", 0xFF6A3FA0);
    KuglanaPloca(X, Y + 4.8, Z - 0.05, 0.0, 90.0, 0.0, 14650, "ab_trukstpc", "mp_CJ_WOOD5");
    KuglanaPloca(X, Y + 14.4, Z - 0.05, 0.0, 90.0, 0.0, 14650, "ab_trukstpc", "mp_CJ_WOOD5");
    KuglanaPloca(X, Y + 24.0, Z - 0.05, 0.0, 90.0, 0.0, 10765, "airportgnd_sfse", "black64");
    for (new i = 0; i < 4; i++)
    {
        new Float:y = Y - 4.8 + 9.6 * float(i);
        if (i < 3) KuglanaPloca(X, y, Z + 4.0, 0.0, 90.0, 0.0, 10765, "airportgnd_sfse", "black64");
        KuglanaPloca(X - 4.8, y, Z + 4.7, 0.0, 0.0, 0.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF3B2A55);
        KuglanaPloca(X + 4.8, y, Z + 4.7, 0.0, 0.0, 0.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF3B2A55);
    }
    KuglanaPloca(X, Y - 9.6, Z + 4.7, 0.0, 0.0, 90.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF3B2A55);
    KuglanaPloca(X, Y + 21.0, Z + 4.7, 0.0, 0.0, 90.0, 10765, "airportgnd_sfse", "black64");
    KuglanaPloca(X, Y + 17.2, Z + 6.5, 0.0, 0.0, 90.0, 14605, "tricas_neon", "sign_neondollar&star");
    for (new s = -1; s <= 1; s += 2)
    {
        KuglanaObjekt(1702, 2.0 * float(s), -3.5, 0.0, 180.0);
        KuglanaObjekt(19786, 2.0 * float(s), 0.6, 3.1, 180.0);
        KuglanaObjekt(948, 4.2 * float(s), -1.0, 0.0, 0.0);
        for (new k = 0; k < 4; k++) KuglanaObjekt(s < 0 ? 18648 : 18651, 4.6 * float(s), -6.0 + 6.0 * float(k), 3.6, 90.0);
    }
    KuglanaObjekt(2112, 0.0, -3.5, 0.0, 0.0);
    KuglanaObjekt(955, -4.2, -9.0, 0.6, 180.0);
    KuglanaObjekt(1776, -3.2, -9.0, 1.0, 180.0);
    KuglanaObjekt(948, 4.2, -9.0, 0.0, 0.0);
}

CMD:kuglana(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    PrebaciIgraca(playerid, KUGLANA_INT, 0, KUGLANA_X, KUGLANA_Y - 6.0, KUGLANA_Z + 1.0);
    return 1;
}
