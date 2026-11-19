MapaInternetKaficExtUkloni(playerid)
{
    RemoveBuildingForPlayer(playerid, 4683, 1613.260, -1185.449, 29.875, 0.250);
    RemoveBuildingForPlayer(playerid, 4618, 1613.260, -1185.449, 29.875, 0.250);
}

MapaInternetKaficExt()
{
    gMapaInt = 0;
    new STREAMER_TAG_OBJECT:tmpobjid;
    tmpobjid = CreateDynamicObjectEx(4683, 1613.260009, -1185.449951, 29.875000, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 9, 18868, "mobilephone4", "mobilephone4-3", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 10, 10031, "chinatown2", "ws_trans_window1", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 11, 7650, "vgnusedcar", "greenshade4_64", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(1538, 1632.623657, -1173.222412, 23.088125, 0.000000, 0.000000, 180.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, 8501, "casroyale", "royaledoor01_128", 0xFFFFFFFF);
    tmpobjid = CreateDynamicObjectEx(19482, 1637.962158, -1171.834716, 28.465068, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "EAR", 130, "Impact", 100, 1, 0xFF25B32B, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1636.811279, -1171.834716, 28.465068, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "TH", 130, "Impact", 100, 1, 0xFFFFFFFF, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1636.651123, -1171.834716, 27.905059, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "Internet Cafe", 130, "Impact", 50, 1, 0xFFFFFFFF, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1630.812866, -1171.834716, 28.465068, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "EAR", 130, "Impact", 100, 1, 0xFF25B32B, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1629.661987, -1171.834716, 28.465068, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "TH", 130, "Impact", 100, 1, 0xFFFFFFFF, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1629.501831, -1171.834716, 27.905059, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "Internet Cafe", 130, "Impact", 50, 1, 0xFFFFFFFF, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1623.921264, -1171.834716, 28.465068, 0.000015, 0.000000, 89.999954, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "EAR", 130, "Impact", 100, 1, 0xFF25B32B, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1622.770385, -1171.834716, 28.465068, 0.000015, 0.000000, 89.999954, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "TH", 130, "Impact", 100, 1, 0xFFFFFFFF, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(19482, 1622.610229, -1171.834716, 27.905059, 0.000015, 0.000000, 89.999954, 300.00, 300.00);
    SetDynamicObjectMaterialText(tmpobjid, 0, "Internet Cafe", 130, "Impact", 50, 1, 0xFFFFFFFF, 0x00000000, 1);
    tmpobjid = CreateDynamicObjectEx(640, 1636.751586, -1171.702148, 23.538129, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, 18868, "mobilephone4", "mobilephone4-3", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 1, 1597, "centralresac1", "fuzzyplant256", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(640, 1625.760375, -1171.702148, 23.538129, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, 18868, "mobilephone4", "mobilephone4-3", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 1, 1597, "centralresac1", "fuzzyplant256", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(643, 1638.353759, -1169.374267, 23.552024, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 2, 18868, "mobilephone4", "mobilephone4-3", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(643, 1634.714111, -1169.374267, 23.552024, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 2, 18868, "mobilephone4", "mobilephone4-3", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(643, 1628.553710, -1169.374267, 23.552024, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 2, 18868, "mobilephone4", "mobilephone4-3", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(643, 1624.913818, -1169.374267, 23.552024, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
    SetDynamicObjectMaterial(tmpobjid, 2, 18868, "mobilephone4", "mobilephone4-3", 0x00000000);
    tmpobjid = CreateDynamicObjectEx(8673, 1630.173950, -1166.173217, 22.866533, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
    SetDynamicObjectMaterial(tmpobjid, 0, 4714, "lanlacma_tr_lan2", "sl_galleryrail1", 0xFFFFFFFF);
}
