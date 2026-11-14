//Map Exported with Texture Studio By: [uL]Pottus, Crayder, Svyatoy, encoder, devhub/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Map Information////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Exported on "2026-11-14 01:46:59" by "ghostface"
	Created by "ghostface"
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Remove Buildings///////////////////////////////////////////////////////////////////////////////////////////////
MapaBusTaxiUkloni(playerid)
{
RemoveBuildingForPlayer(playerid, 4002, 1479.869, -1790.400, 56.023, 0.250);
RemoveBuildingForPlayer(playerid, 4024, 1479.869, -1790.400, 56.023, 0.250);
RemoveBuildingForPlayer(playerid, 3980, 1481.189, -1785.069, 22.382, 0.250);
RemoveBuildingForPlayer(playerid, 4044, 1481.189, -1785.069, 22.382, 0.250);
RemoveBuildingForPlayer(playerid, 4003, 1481.079, -1747.030, 33.523, 0.250);
RemoveBuildingForPlayer(playerid, 3997, 1479.339, -1802.290, 12.546, 0.250);
RemoveBuildingForPlayer(playerid, 4045, 1479.339, -1802.290, 12.546, 0.250);
RemoveBuildingForPlayer(playerid, 1527, 1448.229, -1755.900, 14.523, 0.250);
}

//Objects////////////////////////////////////////////////////////////////////////////////////////////////////////
MapaBusTaxi()
{
gMapaInt = -1;
new STREAMER_TAG_OBJECT:tmpobjid;
tmpobjid = CreateDynamicObjectEx(3997, 1479.339965, -1802.290039, 12.546895, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 2, 9514, "711_sfw", "ws_carpark2", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8409, 1489.413208, -1811.140502, 15.223416, 0.000000, 0.000015, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10755, "airportrminl_sfse", "ws_airportwin2", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 1, 9903, "pier69", "pier69_blue3", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 2, 10755, "airportrminl_sfse", "ws_airportwin2", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 3, 9903, "pier69", "pier69_blue5", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 4, 10755, "airportrminl_sfse", "ws_airportwin2", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 5, 9514, "711_sfw", "ws_carpark2", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 7, 10755, "airportrminl_sfse", "ws_airportwin2", 0x00000000);
tmpobjid = CreateDynamicObjectEx(7315, 1461.636474, -1822.962280, 16.390506, 0.000000, -0.000015, 179.999908, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 1, 10765, "airportgnd_sfse", "white", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 2, 19480, "signsurf", "sign", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19481, 1460.850585, -1774.875854, 12.546895, 0.000000, 0.000015, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "None", 10, "Arial", 20, 0, 0x00000000, 0x00000000, 0);
tmpobjid = CreateDynamicObjectEx(19481, 1461.886718, -1793.144653, 32.016933, 0.000015, 0.000000, 89.999954, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "B", 130, "Impact", 150, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19481, 1461.886718, -1793.144653, 26.136920, 0.000015, 0.000000, 89.999954, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "U", 130, "Impact", 150, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19481, 1461.886718, -1793.144653, 20.556941, 0.000015, 0.000000, 89.999954, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "S", 130, "Impact", 150, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(18766, 1504.825683, -1798.990600, 24.349273, 0.000000, 0.000015, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 17547, "eastbeach4a_lae2", "bluestucco1", 0x00000000);
tmpobjid = CreateDynamicObjectEx(18766, 1494.895996, -1798.990600, 24.349273, 0.000000, 0.000015, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 17547, "eastbeach4a_lae2", "bluestucco1", 0x00000000);
tmpobjid = CreateDynamicObjectEx(7914, 1496.485473, -1798.756958, 25.736921, 0.000000, -0.000015, 179.999908, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "BUS", 130, "Impact", 100, 0, 0xFF3AA5E0, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(7914, 1503.404785, -1798.756958, 25.736921, 0.000000, -0.000015, 179.999908, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "FLYAWAY", 130, "Impact", 100, 0, 0xFFFFFFFF, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(8578, 1418.968383, -1786.970458, 19.858066, 0.000000, 0.000000, 450.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "black64", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 8391, "ballys01", "CJ_blackplastic", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 4, 10755, "airportrminl_sfse", "ws_airportwin2", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 5, 5719, "sunrise10_lawn", "holbuild03b_law", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 7, 14476, "carlslounge", "ah_blu_paper", 0x00000000);
tmpobjid = CreateDynamicObjectEx(7313, 1433.630249, -1802.931640, 16.886922, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "PERON 1", 120, "Impact", 120, 0, 0xFF000000, 0xFF3AA5E0, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1433.630249, -1793.840576, 16.886922, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "PERON 2", 120, "Impact", 120, 0, 0xFF000000, 0xFF3AA5E0, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1433.630249, -1783.989990, 16.886922, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "PERON 3", 120, "Impact", 120, 0, 0xFF000000, 0xFF3AA5E0, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1433.630249, -1774.078369, 16.886922, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "PERON 4", 120, "Impact", 120, 0, 0xFF000000, 0xFF3AA5E0, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1433.630249, -1764.268310, 16.886922, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "PERON 5", 120, "Impact", 120, 0, 0xFF000000, 0xFF3AA5E0, 1);
tmpobjid = CreateDynamicObjectEx(19377, 1434.553833, -1806.390869, 7.326888, 0.000007, -0.000002, 109.999977, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1434.553833, -1796.999633, 7.326888, 0.000014, -0.000005, 109.999954, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1434.553833, -1788.577880, 7.326888, 0.000021, -0.000007, 109.999931, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1434.553833, -1779.847167, 7.326888, 0.000028, -0.000010, 109.999908, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1434.553833, -1770.704589, 7.326888, 0.000035, -0.000013, 109.999885, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1434.553833, -1761.463134, 7.326888, 0.000043, -0.000015, 109.999862, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1430.183471, -1803.161254, 7.346889, 0.000000, -0.000007, 179.999954, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1430.183471, -1793.811279, 7.346889, 0.000000, -0.000007, 179.999954, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1430.183471, -1784.482788, 7.346889, 0.000000, -0.000007, 179.999954, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1430.183471, -1775.063598, 7.346889, 0.000000, -0.000007, 179.999954, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19377, 1430.183471, -1767.854858, 7.346889, 0.000000, -0.000007, 179.999954, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "white", 0x00000000);
tmpobjid = CreateDynamicObjectEx(4199, 1489.679687, -1802.058105, 14.516913, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(4199, 1489.679687, -1820.218139, 14.516913, -0.000007, -0.000000, -89.999977, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(8661, 1489.973999, -1774.983886, 12.576896, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 9920, "anotherbuildsfe", "grassgrn256", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(1508, 1502.098388, -1796.114379, 14.056897, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 4001, "civic03_lan", "downtshop5_LAn", 0x00000000);
tmpobjid = CreateDynamicObjectEx(1508, 1508.368530, -1796.114379, 14.056897, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 4001, "civic03_lan", "downtshop5_LAn", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 1504.954467, -1796.115966, 16.496915, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "BUY TICKET", 130, "Impact", 100, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19482, 1504.934448, -1796.095947, 16.536916, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "BUY TICKET", 130, "Impact", 100, 1, 0xFF3AA5E0, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19482, 1504.895385, -1796.130004, 15.756906, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
tmpobjid = CreateDynamicObjectEx(1508, 1472.338867, -1796.114379, 14.056897, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 4001, "civic03_lan", "downtshop5_LAn", 0x00000000);
tmpobjid = CreateDynamicObjectEx(1508, 1478.609008, -1796.114379, 14.056897, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 4001, "civic03_lan", "downtshop5_LAn", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 1475.194946, -1796.115966, 16.496915, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "TAKE JOB", 130, "Impact", 100, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19482, 1475.174926, -1796.095947, 16.536916, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "TAKE JOB", 130, "Impact", 100, 1, 0xFFFFFFFF, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19482, 1475.135864, -1796.130004, 15.756906, 0.000007, 0.000000, 89.999977, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8578, 1542.329956, -1774.934692, 19.858066, 0.000000, 0.000000, -89.999969, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10765, "airportgnd_sfse", "black64", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 8391, "ballys01", "CJ_blackplastic", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 4, 10755, "airportrminl_sfse", "ws_airportwin2", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 5, 5719, "sunrise10_lawn", "holbuild03b_law", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 7, 14476, "carlslounge", "ah_blu_paper", 0x00000000);
tmpobjid = CreateDynamicObjectEx(7313, 1527.668090, -1758.973510, 16.886922, 0.000000, 0.000000, -89.999969, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "TAXI STAT. 1", 120, "Impact", 120, 0, 0xFF000000, 0xFFF0C816, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1527.668090, -1768.064575, 16.886922, 0.000000, 0.000000, -89.999969, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "TAXI STAT. 2", 120, "Impact", 120, 0, 0xFF000000, 0xFFF0C816, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1527.668090, -1777.915161, 16.886922, 0.000000, 0.000000, -89.999969, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "TAXI STAT. 3", 120, "Impact", 120, 0, 0xFF000000, 0xFFF0C816, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1527.668090, -1787.826782, 16.886922, 0.000000, 0.000000, -89.999969, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "TAXI STAT. 4", 120, "Impact", 120, 0, 0xFF000000, 0xFFF0C816, 1);
tmpobjid = CreateDynamicObjectEx(7313, 1527.668090, -1797.636840, 16.886922, 0.000000, 0.000000, -89.999969, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1560, "7_11_door", "CJ_CHROME2", 0x00000000);
SetDynamicObjectMaterialText(tmpobjid, 0, "TAXI STAT. 5", 120, "Impact", 120, 0, 0xFF000000, 0xFFF0C816, 1);
tmpobjid = CreateDynamicObjectEx(7315, 1517.659179, -1825.404663, 16.390506, 0.000000, -0.000022, 179.999862, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 18245, "cw_junkyardmachin", "Was_scrpyd_light_yellow", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 1, 18245, "cw_junkyardmachin", "Was_scrpyd_light_yellow", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 2, 19480, "signsurf", "sign", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19481, 1516.873291, -1777.318237, 12.546895, 0.000000, 0.000022, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "None", 10, "Arial", 20, 0, 0x00000000, 0x00000000, 0);
tmpobjid = CreateDynamicObjectEx(19481, 1517.909423, -1795.587036, 32.016933, 0.000022, 0.000000, 89.999931, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "T", 130, "Impact", 150, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19481, 1517.909423, -1795.587036, 26.136920, 0.000022, 0.000000, 89.999931, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "A", 130, "Impact", 150, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19481, 1517.909423, -1795.587036, 20.556941, 0.000022, 0.000000, 89.999931, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "X", 130, "Impact", 150, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19481, 1517.909423, -1795.587036, 15.296937, 0.000022, 0.000000, 89.999931, 300.00, 300.00);
SetDynamicObjectMaterialText(tmpobjid, 0, "I", 130, "Impact", 150, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(8661, 1537.603637, -1784.649658, 12.566896, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10778, "airportcpark_sfse", "ws_carpark3", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(8661, 1537.603637, -1773.309204, 12.586896, 0.000000, 0.000000, 90.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 10778, "airportcpark_sfse", "ws_carpark3", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(732, 1503.457519, -1778.329956, 12.265579, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1597, "centralresac1", "kbtree4_test", 0x00000000);
tmpobjid = CreateDynamicObjectEx(732, 1498.276733, -1772.019042, 12.265579, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 728, "gtatreeshi", "berrybush1", 0x00000000);
tmpobjid = CreateDynamicObjectEx(732, 1478.743896, -1770.750610, 12.265579, 0.000000, 0.000000, 179.999893, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 1597, "centralresac1", "kbtree4_test", 0x00000000);
tmpobjid = CreateDynamicObjectEx(732, 1483.924682, -1777.061523, 12.265579, 0.000000, 0.000000, 179.999893, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 1, 728, "gtatreeshi", "berrybush1", 0x00000000);
tmpobjid = CreateDynamicObjectEx(874, 1478.041748, -1774.853515, 12.896883, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 825, "gta_proc_bigbush", "veg_bush1", 0x00000000);
tmpobjid = CreateDynamicObjectEx(874, 1486.080322, -1774.853515, 12.896883, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 825, "gta_proc_bigbush", "veg_bush1", 0x00000000);
tmpobjid = CreateDynamicObjectEx(874, 1500.442016, -1774.853515, 12.896883, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 825, "gta_proc_bigbush", "veg_bush1", 0x00000000);
tmpobjid = CreateDynamicObjectEx(874, 1506.042114, -1774.853515, 12.896883, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
SetDynamicObjectMaterial(tmpobjid, 0, 825, "gta_proc_bigbush", "veg_bush1", 0x00000000);
CreateDynamicObjectEx(732, 1505.927246, -1770.750732, 12.265579, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
CreateDynamicObjectEx(3515, 1491.106567, -1775.900634, 11.856882, 0.000000, 0.000000, 0.000000, 300.00, 300.00);
CreateDynamicObjectEx(732, 1476.274169, -1778.329833, 12.265579, 0.000000, 0.000000, 179.999893, 300.00, 300.00);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
