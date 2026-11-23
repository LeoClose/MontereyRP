//Map Exported with Texture Studio By: [uL]Pottus, Crayder, Svyatoy, encoder, devhub/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Map Information////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Exported on "2026-11-13 23:09:49" by "ghostface"
	Created by "ghostface"
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Remove Buildings///////////////////////////////////////////////////////////////////////////////////////////////
SurenosiUkloni(playerid)
{
RemoveBuildingForPlayer(playerid, 3697, 2744.070, -1965.119, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 3696, 2744.070, -1965.119, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 3697, 2744.080, -1923.880, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 3696, 2744.080, -1923.880, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 5301, 2746.750, -1944.729, 14.703, 0.250);
RemoveBuildingForPlayer(playerid, 5361, 2746.750, -1944.729, 14.703, 0.250);
RemoveBuildingForPlayer(playerid, 3697, 2794.500, -1923.880, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 3696, 2794.500, -1923.880, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 5308, 2791.840, -1944.270, 14.703, 0.250);
RemoveBuildingForPlayer(playerid, 5362, 2791.840, -1944.270, 14.703, 0.250);
RemoveBuildingForPlayer(playerid, 3697, 2794.489, -1965.119, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 3696, 2794.489, -1965.119, 16.843, 0.250);
RemoveBuildingForPlayer(playerid, 5366, 2786.770, -1970.060, 20.062, 0.250);
RemoveBuildingForPlayer(playerid, 5234, 2786.770, -1970.060, 20.062, 0.250);
RemoveBuildingForPlayer(playerid, 5144, 2768.560, -1942.699, 11.304, 0.250);
RemoveBuildingForPlayer(playerid, 5258, 2768.560, -1942.699, 11.304, 0.250);
RemoveBuildingForPlayer(playerid, 620, 2728.239, -1959.329, 9.671, 0.250);
RemoveBuildingForPlayer(playerid, 620, 2730.870, -1960.380, 9.671, 0.250);
RemoveBuildingForPlayer(playerid, 5358, 2783.000, -2023.319, 13.929, 0.250);
RemoveBuildingForPlayer(playerid, 5173, 2768.449, -2012.089, 14.796, 0.250);
RemoveBuildingForPlayer(playerid, 5256, 2768.449, -2012.089, 14.796, 0.250);

}

//Objects////////////////////////////////////////////////////////////////////////////////////////////////////////
Surenosi()
{
new tmpobjid, object_world = -1, object_int = -1;
tmpobjid = CreateDynamicObjectEx(5144, 2768.560058, -1942.699951, 11.304697, 0.000000, 0.000000, -0.000009, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 9514, "711_sfw", "ws_carpark2", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 1, 3676, "lawnxref", "lasthoose1blue", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 2, 9920, "anotherbuildsfe", "grassgrn256", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 3, 3676, "lawnxref", "lasthoose1blue", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.790771, -1909.241210, 14.642197, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.790771, -1915.940063, 14.702198, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.810791, -1909.321289, 16.922187, 0.000000, 0.000015, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 1, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.810791, -1916.020141, 16.982189, 0.000000, 0.000015, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 1, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(17575, 2792.613525, -1927.951782, 16.245489, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(17575, 2792.623535, -1959.755004, 15.995483, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.810791, -1952.560668, 14.642197, 0.000000, -0.000015, 179.999801, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.810791, -1945.861816, 14.702198, 0.000000, -0.000015, 179.999801, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.790771, -1952.480590, 16.922187, 0.000000, -0.000000, 179.999801, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 1, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(3783, 2743.790771, -1945.781738, 16.982189, 0.000000, -0.000000, 179.999801, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 1, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 3, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(4199, 2729.364257, -1958.311279, 14.368991, 0.000000, 0.000000, 540.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
SetDynamicObjectMaterial(tmpobjid, 1, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 2, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0x00000000);
tmpobjid = CreateDynamicObjectEx(7914, 2782.163818, -1943.843994, 15.710236, 0.000000, 0.000000, 270.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 17700, "gangblok1_lae2", "mural01_LA", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(19482, 2781.706298, -1937.948974, 18.369695, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 4811, "beach_las", "grifnewtex2x_LAS", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2781.706298, -1943.539062, 18.369695, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 4811, "beach_las", "grifnewtex2x_LAS", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2781.706298, -1949.029296, 18.369695, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 4811, "beach_las", "grifnewtex2x_LAS", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2781.716308, -1949.800048, 18.369695, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 4811, "beach_las", "grifnewtex2x_LAS", 0x00000000);
tmpobjid = CreateDynamicObjectEx(7914, 2734.816406, -1965.577392, 14.790241, 0.000000, 0.000000, 450.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5134, "wasteland_las2", "mural05_LA", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(7914, 2734.616210, -1965.577392, 14.880243, 0.000000, 0.000000, 630.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 18241, "cw_tempstuffcs_t", "bluemetal03", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(5358, 2783.000000, -2023.319946, 13.929697, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2755.428466, -1952.572998, 15.086888, 6.699998, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 14612, "ab_abattoir_box", "ab_bloodfloor", 0xFF0066FF);
SetDynamicObjectMaterialText(tmpobjid, 0, "HASTA", 130, "Impact", 70, 1, 0xFFFFFFFF, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(5173, 2768.449951, -2012.089965, 14.796895, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5706, "ci_studio5", "Bow_sidewalk", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 3, 5040, "shopliquor_las", "mural02_LA", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 4, 5706, "ci_studio5", "Bow_sidewalk", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 5, 9920, "anotherbuildsfe", "grassgrn256", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 6, 14488, "dogsgym", "AH_halfcourt", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 8, 6404, "beafron1_law2", "shutter02LA", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 9, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0x00000000);
SetDynamicObjectMaterial(tmpobjid, 15, 18241, "cw_tempstuffcs_t", "bluemetal03", 0x00000000);
tmpobjid = CreateDynamicObjectEx(645, 2761.314697, -2037.774291, 12.261619, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(645, 2782.468994, -2037.774291, 12.261619, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(645, 2805.717773, -2037.774291, 12.261619, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, -1, "none", "none", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(620, 2772.986083, -2037.493041, 8.337689, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 1597, "centralresac1", "fuzzyplant256", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(620, 2794.938232, -2037.493041, 8.337689, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 1597, "centralresac1", "fuzzyplant256", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(620, 2744.599121, -1981.772705, 8.337689, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 1597, "centralresac1", "fuzzyplant256", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(620, 2810.300292, -1991.583007, 8.337689, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 1597, "centralresac1", "fuzzyplant256", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(620, 2793.919433, -1931.232177, 8.337689, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 1597, "centralresac1", "fuzzyplant256", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(620, 2746.820068, -1927.071289, 8.337689, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 1597, "centralresac1", "fuzzyplant256", 0xFFFFFFFF);
tmpobjid = CreateDynamicObjectEx(8673, 2790.045410, -1900.905883, 13.476029, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2802.728027, -1900.905883, 13.476029, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2779.583496, -1910.899658, 13.476029, 0.000000, 0.000000, 90.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2789.777099, -1984.545776, 13.476029, 0.000000, 0.000000, 180.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2802.452148, -1984.545776, 13.476029, 0.000000, 0.000000, 180.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2748.789794, -1984.385620, 13.476029, 0.000000, 0.000000, 180.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2734.720214, -1984.385620, 13.476029, 0.000000, 0.000000, 180.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2724.519775, -1974.262573, 13.476029, 0.000000, 0.000000, 270.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2759.451904, -1967.083618, 13.476029, 0.000000, 0.000000, 270.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2759.451904, -1959.392700, 13.476029, 0.000000, 0.000000, 270.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2759.451904, -1929.841552, 13.476029, 0.000000, 0.000000, 270.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2759.451904, -1915.020141, 13.476029, 0.000000, 0.000000, 270.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2746.289794, -1904.070312, 13.476029, 0.000000, 0.000000, 360.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2741.277832, -1904.070312, 13.476029, 0.000000, 0.000000, 360.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5630, "blockalpha", "grille2_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2726.924072, -1942.491455, 14.169369, 0.000000, 0.000000, 90.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 1531, "tags_laazteca", "aztecas", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2755.422607, -1948.134521, 13.826879, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5069, "ctscene_las", "cleargraf02_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2756.281738, -2024.207641, 12.564701, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5998, "sunstr_lawn", "ganggraf02_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8674, 2724.244873, -1926.822265, 13.780201, 0.000000, 0.000000, 90.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 17510, "blackwestran1_lae2", "des_indrails", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2812.959228, -1979.921752, 10.151803, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5998, "sunstr_lawn", "ganggraf02_LA", 0x00000000);
tmpobjid = CreateDynamicObjectEx(8673, 2786.800781, -2008.867065, 12.544241, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 17510, "blackwestran1_lae2", "des_indrails", 0x00000000);
tmpobjid = CreateDynamicObjectEx(19482, 2781.729248, -1969.770263, 18.867523, 0.000000, 0.000000, 180.000000, 300.00, 300.00); 
SetDynamicObjectMaterialText(tmpobjid, 0, "SEVILLE FLATS", 130, "Impact", 100, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19482, 2781.729248, -1972.890747, 18.867523, 0.000000, 0.000000, 180.000000, 300.00, 300.00); 
SetDynamicObjectMaterialText(tmpobjid, 0, "A1", 130, "Impact", 100, 1, 0xFF000000, 0x00000000, 1);
tmpobjid = CreateDynamicObjectEx(19482, 2781.729003, -1970.779296, 14.242789, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
SetDynamicObjectMaterial(tmpobjid, 0, 5998, "sunstr_lawn", "ganggraf02_LA", 0x00000000);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tmpobjid = CreateDynamicObjectEx(1495, 2784.910400, -1951.881835, 12.469371, 0.000000, 0.000000, -24.800006, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(1495, 2784.807617, -1920.164184, 12.519372, 0.000000, 0.000000, -24.800006, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(2674, 2788.501220, -1925.580200, 12.566884, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(2676, 2776.670898, -1941.399902, 12.649387, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(2672, 2785.768066, -1960.698486, 12.829380, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(703, 2746.384765, -1979.007934, 11.241724, 0.000000, 0.000000, 31.600004, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(887, 2751.248535, -1929.440185, 12.402145, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(887, 2741.927490, -1928.001464, 12.402145, 0.000000, 0.000000, 20.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(887, 2732.597656, -1930.514160, 12.402145, 0.000000, 0.000000, 20.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(887, 2751.199462, -1981.625366, 12.202141, 0.000000, 0.000000, 20.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(2676, 2744.201171, -1936.388427, 12.649375, 0.000000, 0.000000, 0.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(1415, 2759.971923, -1973.490844, 12.526219, 0.000000, 0.000000, 90.000000, 300.00, 300.00); 
tmpobjid = CreateDynamicObjectEx(1344, 2760.132568, -1934.178588, 13.293838, 0.000000, 0.000000, 90.000000, 300.00, 300.00); 
}
