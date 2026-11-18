static KuglanaPloca(Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, txd, const lib[], const tex[], boja = 0)
{
    new STREAMER_TAG_OBJECT:o = CreateDynamicObject(19377, x, y, z, rx, ry, rz, -1, KUGLANA_INT, -1, 200.0, 200.0);
    SetDynamicObjectMaterial(o, 0, txd, lib, tex, boja);
}

static KuglanaObjekt(model, Float:x, Float:y, Float:z, Float:rz)
{
    CreateDynamicObject(model, KUGLANA_X + x, KUGLANA_Y + y, KUGLANA_Z + z, 0.0, 0.0, rz, -1, KUGLANA_INT, -1, 200.0, 200.0);
}

static KuglanaKugla(Float:x, Float:y, boja)
{
    new STREAMER_TAG_OBJECT:o = CreateDynamicObject(2114, KUGLANA_X + x, KUGLANA_Y + y, KUGLANA_Z + 0.11, 0.0, 0.0, 0.0, -1, KUGLANA_INT, -1, 200.0, 200.0);
    SetDynamicObjectMaterial(o, 0, 3922, "bistro", "Marble2", boja);
}

static KuglanaBoja(model, Float:x, Float:y, Float:z, Float:rz, boja)
{
    new STREAMER_TAG_OBJECT:o = CreateDynamicObject(model, KUGLANA_X + x, KUGLANA_Y + y, KUGLANA_Z + z, 0.0, 0.0, rz, -1, KUGLANA_INT, -1, 200.0, 200.0);
    SetDynamicObjectMaterial(o, 0, 4828, "airport3_las", "gnhotelwall02_128", boja);
}

MapaKuglana()
{
    new const Float:X = KUGLANA_X, Float:Y = KUGLANA_Y, Float:Z = KUGLANA_Z, boje[] = { 0xFF1E5AA8, 0xFFC62828, 0xFF6A1B9A, 0xFF2E7D32, 0xFFF9A825, 0xFF212121 };
    KuglanaPloca(X, Y - 4.8, Z - 0.05, 0.0, 90.0, 0.0, 19595, "lsappartments1", "carpet4-256x256", 0xFF262046);
    KuglanaPloca(X, Y + 4.8, Z - 0.05, 0.0, 90.0, 0.0, 14650, "ab_trukstpc", "mp_CJ_WOOD5");
    KuglanaPloca(X, Y + 14.4, Z - 0.05, 0.0, 90.0, 0.0, 14650, "ab_trukstpc", "mp_CJ_WOOD5");
    KuglanaPloca(X, Y + 24.0, Z - 0.05, 0.0, 90.0, 0.0, 10765, "airportgnd_sfse", "black64");
    for (new i = 0; i < 4; i++)
    {
        new Float:y = Y - 4.8 + 9.6 * float(i);
        if (i < 3) KuglanaPloca(X, y, Z + 3.6, 0.0, 90.0, 0.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF5C6066);
        for (new st = -1; st <= 1; st += 2)
        {
            KuglanaPloca(X + 4.8 * float(st), y, Z + 4.7, 0.0, 0.0, 0.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF1E9E92);
            KuglanaPloca(X + 4.78 * float(st), y, Z + 2.55 - 4.8, 0.0, 0.0, 0.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFFE0592A);
            KuglanaPloca(X + 4.76 * float(st), y, Z + 2.35 - 4.8, 0.0, 0.0, 0.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF1E9E92);
            KuglanaPloca(X + 4.74 * float(st), y, Z + 0.15 - 4.8, 0.0, 0.0, 0.0, 10765, "airportgnd_sfse", "black64");
        }
    }
    KuglanaPloca(X, Y - 9.6, Z + 4.7, 0.0, 0.0, 90.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF1E9E92);
    KuglanaPloca(X, Y - 9.58, Z + 2.55 - 4.8, 0.0, 0.0, 90.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFFE0592A);
    KuglanaPloca(X, Y - 9.56, Z + 2.35 - 4.8, 0.0, 0.0, 90.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF1E9E92);
    KuglanaPloca(X, Y - 9.54, Z + 0.15 - 4.8, 0.0, 0.0, 90.0, 10765, "airportgnd_sfse", "black64");
    KuglanaPloca(X, Y + 21.0, Z + 4.7, 0.0, 0.0, 90.0, 10765, "airportgnd_sfse", "black64");
    KuglanaPloca(X, Y + 17.2, Z + 1.6 + 4.8, 0.0, 0.0, 90.0, 14650, "ab_trukstpc", "mp_CJ_WOOD5");
    KuglanaPloca(X, Y + 17.18, Z + 1.75 - 4.8, 0.0, 0.0, 90.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF1E9E92);
    KuglanaPloca(X, Y, Z + 0.01 - 4.8, 0.0, 0.0, 90.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFFC62828);
    for (new l = 0; l < 4; l++)
    {
        new Float:c = -3.0 + 2.0 * float(l);
        for (new st = -1; st <= 1; st += 2)
        {
            for (new k = 0; k < 2; k++)
            {
                KuglanaPloca(X + c + 0.62 * float(st), Y + 4.8 + 9.6 * float(k), Z + 0.05 - 4.8, 0.0, 0.0, 0.0, 10765, "airportgnd_sfse", "black64");
                KuglanaPloca(X + c + 0.76 * float(st), Y + 4.8 + 9.6 * float(k), Z + 0.07 - 4.8, 0.0, 0.0, 0.0, 4828, "airport3_las", "gnhotelwall02_128", 0xFF26C6B5);
            }
        }
        for (new k = 0; k < 3; k++) KuglanaObjekt(18652, c, 3.0 + 6.0 * float(k), 3.5, 90.0);
    }
    for (new st = -1; st <= 1; st += 2)
    {
        new Float:x = 2.0 * float(st);
        KuglanaObjekt(19786, x, 0.3, 2.9, 0.0);
        KuglanaObjekt(19786, x, -0.3, 2.9, 180.0);
        KuglanaBoja(19789, x, -0.45, -0.1, 0.0, 0xFFF2EEE4);
        for (new k = 0; k < 6; k++) KuglanaKugla(x - 0.6 + 0.24 * float(k), -1.15, boje[k]);
        for (new k = 0; k < 3; k++) KuglanaBoja(1720, x - 0.6 + 0.6 * float(k), -2.6, 0.0, 180.0, 0xFFE0592A);
        KuglanaBoja(19789, 3.6 * float(st), -6.2, 0.5, 0.0, 0xFF4A3424);
        KuglanaBoja(19789, 3.6 * float(st), -7.2, 0.5, 0.0, 0xFF4A3424);
        for (new k = 0; k < 2; k++) KuglanaBoja(1805, 3.6 * float(st) - 0.8 * float(st), -6.2 - float(k), 0.0, 0.0, 0xFFE0592A);
        for (new k = 0; k < 3; k++) KuglanaObjekt(st < 0 ? 18648 : 18650, 4.6 * float(st), -6.0 + 7.0 * float(k), 3.45, 90.0);
    }
    KuglanaObjekt(2681, -1.6, -9.0, 0.0, 180.0);
    KuglanaObjekt(2779, -0.6, -9.0, 0.0, 180.0);
    KuglanaObjekt(2778, 0.4, -9.0, 0.0, 180.0);
    KuglanaObjekt(955, 1.6, -9.1, 0.6, 180.0);
    KuglanaObjekt(948, 4.3, -9.1, 0.0, 0.0);
    KuglanaObjekt(948, -4.3, -9.1, 0.0, 0.0);
    KuglanaObjekt(1328, -4.3, -2.4, 0.0, 0.0);
    KuglanaObjekt(2229, 4.3, 0.2, 0.0, 0.0);
    KuglanaObjekt(2229, -4.3, 0.2, 0.0, 0.0);
    KuglanaNatpis("LET'S GO BOWLING!", -4.7, -4.0, 3.05, 0xFFE0592A, 90, OBJECT_MATERIAL_SIZE_512x128, 180.0);
    KuglanaNatpis("MONTEREY LANES", 4.7, -4.0, 3.05, 0xFFE0592A, 90, OBJECT_MATERIAL_SIZE_512x128, 0.0);
    KuglanaNatpis("SNACKS ~ ARCADE", 0.0, -9.5, 3.0, 0xFFE0592A, 60, OBJECT_MATERIAL_SIZE_512x128, 270.0);
    for (new l = 0; l < 4; l++)
    {
        new broj[2];
        broj[0] = '1' + l;
        KuglanaNatpis(broj, -3.0 + 2.0 * float(l), 17.05, 2.4, 0xFF263238, 80, OBJECT_MATERIAL_SIZE_64x64);
    }
}

static KuglanaNatpis(const tekst[], Float:x, Float:y, Float:z, boja, velicina, OBJECT_MATERIAL_SIZE:format, Float:rz = 90.0)
{
    new STREAMER_TAG_OBJECT:o = CreateDynamicObject(19482, KUGLANA_X + x, KUGLANA_Y + y, KUGLANA_Z + z, 0.0, 0.0, rz, -1, KUGLANA_INT, -1, 200.0, 200.0);
    SetDynamicObjectMaterialText(o, 0, tekst, format, "Arial", velicina, true, boja, 0, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
}

CMD:kuglana(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    PrebaciIgraca(playerid, KUGLANA_INT, 0, KUGLANA_X, KUGLANA_Y - 4.5, KUGLANA_Z + 1.0);
    return 1;
}
