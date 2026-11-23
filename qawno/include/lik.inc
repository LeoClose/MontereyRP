#if defined _INC_lik
	#endinput
#endif
#define _INC_lik

#define LIK_HOD     0
#define LIK_TRK     1
#define LIK_SPRINT  2
#define INVALID_LIK -1
#define LIK_PACIJENT 5000

#define LIK_REAGIRA   1
#define LIK_IZBJEGAVA 2
#define LIK_RANJIV    4

native Lik_Stvori(skin, Float:x, Float:y, Float:z, Float:kut = 0.0, svijet = 0, interijer = 0);
native bool:Lik_Ukloni(lik);
native bool:Lik_Idi(lik, Float:x, Float:y, Float:z, nacin = LIK_HOD);
native bool:Lik_IdiPutem(lik, const Float:tocke[], broj, nacin = LIK_HOD);
native bool:Lik_Stani(lik);
native bool:Lik_Postavi(lik, Float:x, Float:y, Float:z);
native bool:Lik_Pozicija(lik, &Float:x, &Float:y, &Float:z);
native bool:Lik_Kut(lik, Float:kut);
native bool:Lik_DajKut(lik, &Float:kut);
native bool:Lik_Skin(lik, skin);
native bool:Lik_Svijet(lik, svijet, interijer);
native bool:Lik_Animacija(lik, const lib[], const ime[], Float:delta = 4.1, petlja = 0, lockx = 1, locky = 1, zamrzni = 0, vrijeme = 0);
native bool:Lik_OcistiAnimaciju(lik);
native bool:Lik_Krece(lik);

native bool:Lik_Zdravlje(lik, Float:zdravlje);
native bool:Lik_DajZdravlje(lik, &Float:zdravlje);
native bool:Lik_Neranjiv(lik, bool:neranjiv);
native bool:Lik_Mrtav(lik);
native bool:Lik_Ozivi(lik, Float:x, Float:y, Float:z, Float:kut);
native bool:Lik_Oruzje(lik, oruzje, municija = 0);
native Lik_DajOruzje(lik);
native Lik_DajMuniciju(lik);
native bool:Lik_Preciznost(lik, Float:preciznost);
native bool:Lik_Nacilja(lik, playerid, bool:pucaj = true, interval = 400);
native bool:Lik_NaciljaTocku(lik, Float:x, Float:y, Float:z, bool:pucaj = true);
native bool:Lik_PrestaniCiljati(lik);
native bool:Lik_Cilja(lik, playerid);
native bool:Lik_Udaraj(lik, playerid, interval = 800);
native bool:Lik_Udara(lik);
native bool:Lik_PrestaniUdarati(lik);
native bool:Lik_Prati(lik, playerid, nacin = LIK_TRK, Float:razmak = 1.0);
native bool:Lik_PrestaniPratiti(lik);

forward OnLikStigao(lik);
forward OnLikPogoden(lik, playerid, Float:steta, oruzje, dio);
forward OnLikPogodio(lik, playerid, Float:steta, oruzje, dio);
native Lik_CiljIgraca(playerid);
native bool:Lik_IgracRanjen(playerid, bool:ranjen);

native bool:Lik_IdiDo(lik, Float:x, Float:y, Float:z, nacin = LIK_HOD);
native bool:Lik_Ponasanje(lik, zastavice, Float:prag = 0.0);
native bool:Lik_Pridigni(lik);
native bool:Lik_Ranjen(lik);
native bool:Lik_Napadni(lik, playerid);
native bool:Lik_Povuci(lik);
native bool:Lik_UVozilo(lik, vozilo, sjedalo = 0);
native bool:Lik_Udji(lik, vozilo, sjedalo = 0, nacin = LIK_HOD);
native bool:Lik_Izadji(lik, bool:odmah = false);
native bool:Lik_TipkeVozila(lik, tipke);
native Lik_DajVozilo(lik);
native bool:Lik_Vozi(lik, Float:x, Float:y, Float:z, Float:brzina = 0.0, bool:sirena = false);
native bool:Lik_Sirena(lik, bool:upaljena);
native bool:Lik_Trubi(lik, ms = 500);
native bool:Lik_Zaglavio(lik, ms);
native bool:Lik_TockaStaze(Float:x, Float:y, Float:min, Float:max, &Float:ox, &Float:oy, &Float:oz);

forward OnLikUmro(lik, ubojica, oruzje);
forward OnLikSmiren(lik);
forward OnLikRanjen(lik, napadac);
forward OnLikUsao(lik, vozilo);
forward OnLikIzasao(lik, vozilo);
forward OnLikIzbacen(lik, playerid);
