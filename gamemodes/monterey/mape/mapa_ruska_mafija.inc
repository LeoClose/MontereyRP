MapaRuskaMafijaUkloni(playerid)
{
    RemoveBuildingForPlayer(playerid, 13691, 258.953, -1366.199, 62.804, 0.250);
    RemoveBuildingForPlayer(playerid, 13745, 258.953, -1366.199, 62.804, 0.250);
    RemoveBuildingForPlayer(playerid, 3619, 249.046, -1370.989, 55.742, 0.250);
    RemoveBuildingForPlayer(playerid, 3738, 249.046, -1370.989, 55.742, 0.250);
    RemoveBuildingForPlayer(playerid, 673, 230.227, -1379.209, 51.546, 0.250);
    RemoveBuildingForPlayer(playerid, 673, 244.031, -1387.209, 51.796, 0.250);
}

MapaRuskaMafija()
{
    gMapaInt = 0;
    new STREAMER_TAG_OBJECT:tmpobjid;
    tmpobjid = CreateDynamicObjectEx(13691, 258.953002, -1366.199951, 62.804698, 0.000000, 0.000000, 38.378559, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 2, 9920, "anotherbuildsfe", "grassgrn256", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 7, 13816, "lahills_safe1", "pord_conc_128", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(3603, 241.547622, -1376.987915, 57.928676, 0.000000, 0.000000, 670.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, 8480, "csrspalace01", "vgspawnroof02_128", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 1, 13816, "lahills_safe1", "pord_conc_128", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 2, 4569, "civic1_lan2", "posh_eagle11_sfe", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 3, 4569, "civic1_lan2", "posh_eagle11_sfe", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 4, 8618, "ceasersign", "ceaserspillar01_128", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 6, 8618, "ceasersign", "ceaserspillar01_128", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 8, 4569, "civic1_lan2", "posh_eagle12_sfe", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(2909, 263.928192, -1333.341064, 53.517715, 181.199996, 0.000000, -54.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, 10806, "airfence_sfse", "ws_leccyfncetop", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 1, 18081, "cj_barb", "barberschr7b", 0x00000000);
    CreateDynamicObjectEx(652, 256.243103, -1341.785278, 51.932098, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    CreateDynamicObjectEx(652, 256.013000, -1348.016479, 51.932098, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    CreateDynamicObjectEx(652, 250.603042, -1348.016479, 51.932098, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
}
