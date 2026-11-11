#define DLG:%0(%1)          Dlg_%0(%1)

enum
{
    DLG_NONE,
    DLG_REGISTRACIJA, DLG_PRIJAVA,
    DLG_JEZIK, DLG_JEZIK_PROMJENA, DLG_SKOLA, DLG_PRIJEVOD,
    DLG_ORG_POZIV, DLG_ORG_ZADUZENJA, DLG_ORG_PANEL, DLG_ORG_CLAN, DLG_ORG_POZIV_UNOS,
    DLG_ORG_RANK_UNOS, DLG_ORG_IZBACI, DLG_ORG_NAPUSTI, DLG_ORG_SKIN,
    DLG_INVENTAR, DLG_INVENTAR_PREDMET, DLG_INVENTAR_DAJ, DLG_INVENTAR_BACI,
    DLG_TERITORIJ, DLG_TER_DROGA, DLG_TER_CIJENA, DLG_TER_ORUZJE, DLG_TER_MUNICIJA, DLG_TER_ORG,
    DLG_APANEL_KAT, DLG_APANEL, DLG_APANEL_ORGOVI, DLG_APANEL_TIP_ORG, DLG_APANEL_ORG, DLG_APANEL_BRISANJE_ORG, DLG_APANEL_UNOS,
    DLG_APANEL_TER, DLG_APANEL_TER_MENI, DLG_APANEL_BRISANJE_TER, DLG_TAKSI, DLG_SLUCAJ, DLG_RANJEN, DLG_DILER, DLG_ANIM, DLG_APANEL_TAKSI, DLG_APANEL_TAKSI_BRISI, DLG_APANEL_KAMION, DLG_APANEL_KAMION_BRISI, DLG_KURVA, DLG_APANEL_KURVA, DLG_APANEL_KURVA_BRISI, DLG_FRISK, DLG_ORMAR, DLG_GEPEK, DLG_GEPEK_STAVI, DLG_GEPEK_KOLICINA, DLG_APANEL_HQ, DLG_APANEL_HQ_UNOS, DLG_APANEL_PUMPA, DLG_APANEL_PUMPA_BRISI,
    DLG_GORIVO, DLG_TRGOVINA, DLG_APANEL_TRGOVINA, DLG_APANEL_TRGOVINA_MENI,
    DLG_SALON_KUPI, DLG_APANEL_SALON, DLG_APANEL_SALON_MENI, DLG_APANEL_SALON_DODAJ, DLG_APANEL_SALON_UKLONI,
    DLG_OPSTINA, DLG_OPSTINA_KAZNE, DLG_OPSTINA_IME, DLG_OPSTINA_VOZILA, DLG_OPSTINA_PRODAJA, DLG_OPSTINA_PONUDA, DLG_APANEL_OPSTINA, DLG_APANEL_OPSTINA_INT, DLG_APANEL_OPSTINA_UNOS,
    DLG_PIJACA_PRODAJ, DLG_PIJACA_KUPI, DLG_APANEL_PIJACA, DLG_MEHANICAR
}

static gDlg[MAX_PLAYERS];

PrikaziDialog(playerid, dialogid, DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[] = "")
{
    gDlg[playerid] = dialogid;
    return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
}

bool:DodajStupac(list[], &len, const row[], size = sizeof (list))
{
    new rowlen = strlen(row);
    if (len + rowlen + 2 > size) return false;
    strcat(list[len], row, size - len);
    len += rowlen;
    list[len++] = '\n';
    list[len] = EOS;
    return true;
}

DialogConnect(playerid)
{
    gDlg[playerid] = DLG_NONE;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid != gDlg[playerid]) return 1;
    gDlg[playerid] = DLG_NONE;
    switch (dialogid)
    {
        case DLG_REGISTRACIJA:          return Dlg_registracija(playerid, response, inputtext);
        case DLG_PRIJAVA:             return Dlg_prijava(playerid, response, inputtext);
        case DLG_JEZIK:          return Dlg_jezik(playerid, response, listitem);
        case DLG_JEZIK_PROMJENA: return Dlg_jezik_promjena(playerid, response, listitem);
        case DLG_SKOLA:            return Dlg_skola(playerid, response, listitem);
        case DLG_PRIJEVOD:         return Dlg_prijevod(playerid, response, listitem);
        case DLG_ORG_POZIV:        return Dlg_org_poziv(playerid, response);
        case DLG_ORG_ZADUZENJA:         return Dlg_org_zaduzenja(playerid, response, listitem);
        case DLG_ORG_PANEL:         return Dlg_org_panel(playerid, response, listitem);
        case DLG_ORG_CLAN:        return Dlg_org_clan(playerid, response, listitem);
        case DLG_ORG_POZIV_UNOS:  return Dlg_org_poziv_unos(playerid, response, inputtext);
        case DLG_ORG_RANK_UNOS:    return Dlg_org_rank_unos(playerid, response, inputtext);
        case DLG_ORG_IZBACI:          return Dlg_org_izbaci(playerid, response);
        case DLG_ORG_NAPUSTI:         return Dlg_org_napusti(playerid, response);
        case DLG_ORG_SKIN:          return Dlg_org_skin(playerid, response, listitem);
        case DLG_INVENTAR:               return Dlg_inventar(playerid, response, listitem);
        case DLG_INVENTAR_PREDMET:          return Dlg_inventar_predmet(playerid, response, listitem);
        case DLG_INVENTAR_DAJ:          return Dlg_inventar_daj(playerid, response, inputtext);
        case DLG_INVENTAR_BACI:          return Dlg_inventar_baci(playerid, response, inputtext);
        case DLG_TERITORIJ:         return Dlg_teritorij(playerid, response, listitem);
        case DLG_TER_ORG:           return Dlg_ter_org(playerid, response, listitem);
        case DLG_TER_DROGA:         return Dlg_ter_droga(playerid, response, inputtext);
        case DLG_TER_CIJENA:        return Dlg_ter_cijena(playerid, response, inputtext);
        case DLG_TER_ORUZJE:        return Dlg_ter_oruzje(playerid, response, listitem);
        case DLG_TER_MUNICIJA:      return Dlg_ter_municija(playerid, response, inputtext);
        case DLG_APANEL_KAT:        return Dlg_apanel_kat(playerid, response, listitem);
        case DLG_APANEL:            return Dlg_apanel(playerid, response, listitem);
        case DLG_APANEL_ORGOVI:       return Dlg_apanel_orgovi(playerid, response, listitem);
        case DLG_APANEL_TIP_ORG:    return Dlg_apanel_tip_org(playerid, response, listitem);
        case DLG_APANEL_ORG:        return Dlg_apanel_org(playerid, response, listitem);
        case DLG_APANEL_BRISANJE_ORG:  return Dlg_apanel_brisanje_org(playerid, response);
        case DLG_APANEL_UNOS:      return Dlg_apanel_unos(playerid, response, inputtext);
        case DLG_APANEL_TER:        return Dlg_apanel_ter(playerid, response, listitem);
        case DLG_APANEL_TER_MENI:   return Dlg_apanel_ter_meni(playerid, response, listitem);
        case DLG_APANEL_BRISANJE_TER: return Dlg_apanel_brisanje_ter(playerid, response);
        case DLG_TAKSI:             return Dlg_taksi(playerid, response);
        case DLG_SLUCAJ:             return Dlg_slucaj(playerid, response);
        case DLG_RANJEN:            return Dlg_ranjen(playerid, response);
        case DLG_DILER:             return Dlg_diler(playerid, response, inputtext);
        case DLG_ANIM:              return Dlg_anim(playerid, response, listitem);
        case DLG_APANEL_TAKSI:      return Dlg_apanel_taksi(playerid, response, listitem);
        case DLG_APANEL_TAKSI_BRISI: return Dlg_apanel_taksi_brisi(playerid, response);
        case DLG_APANEL_KAMION:     return Dlg_apanel_kamion(playerid, response, listitem);
        case DLG_APANEL_KAMION_BRISI: return Dlg_apanel_kamion_brisi(playerid, response);
        case DLG_KURVA:       return Dlg_kurva(playerid, response);
        case DLG_FRISK:             return Dlg_frisk(playerid, response);
        case DLG_ORMAR:             return Dlg_ormar(playerid, response, listitem);
        case DLG_GEPEK:             return Dlg_gepek(playerid, response, listitem);
        case DLG_GEPEK_STAVI:       return Dlg_gepek_stavi(playerid, response, listitem);
        case DLG_GEPEK_KOLICINA:    return Dlg_gepek_kolicina(playerid, response, inputtext);
        case DLG_APANEL_HQ:         return Dlg_apanel_hq(playerid, response, listitem);
        case DLG_APANEL_HQ_UNOS:    return Dlg_apanel_hq_unos(playerid, response, inputtext);
        case DLG_APANEL_PUMPA:      return Dlg_apanel_pumpa(playerid, response, listitem);
        case DLG_APANEL_PUMPA_BRISI: return Dlg_apanel_pumpa_brisi(playerid, response);
        case DLG_GORIVO:            return Dlg_gorivo(playerid, response, inputtext);
        case DLG_TRGOVINA:          return Dlg_trgovina(playerid, response, listitem);
        case DLG_APANEL_TRGOVINA:   return Dlg_apanel_trgovina(playerid, response, listitem);
        case DLG_APANEL_TRGOVINA_MENI: return Dlg_apanel_trgovina_meni(playerid, response, listitem);
        case DLG_APANEL_KURVA:      return Dlg_apanel_kurve(playerid, response, listitem);
        case DLG_APANEL_KURVA_BRISI: return Dlg_apanel_kurve_brisi(playerid, response);
        case DLG_SALON_KUPI:        return Dlg_salon_kupi(playerid, response);
        case DLG_APANEL_SALON:      return Dlg_apanel_salon(playerid, response, listitem);
        case DLG_APANEL_SALON_MENI: return Dlg_apanel_salon_meni(playerid, response, listitem);
        case DLG_APANEL_SALON_DODAJ: return Dlg_apanel_salon_dodaj(playerid, response, inputtext);
        case DLG_APANEL_SALON_UKLONI: return Dlg_apanel_salon_ukloni(playerid, response, listitem);
        case DLG_OPSTINA:           return Dlg_opstina(playerid, response, listitem);
        case DLG_OPSTINA_KAZNE:     return Dlg_opstina_kazne(playerid, response, listitem);
        case DLG_OPSTINA_IME:       return Dlg_opstina_ime(playerid, response, inputtext);
        case DLG_OPSTINA_VOZILA:    return Dlg_opstina_vozila(playerid, response, listitem);
        case DLG_OPSTINA_PRODAJA:   return Dlg_opstina_prodaja(playerid, response, inputtext);
        case DLG_OPSTINA_PONUDA:    return Dlg_opstina_ponuda(playerid, response);
        case DLG_APANEL_OPSTINA:    return Dlg_apanel_opstina(playerid, response, listitem);
        case DLG_APANEL_OPSTINA_INT: return Dlg_apanel_opstina_int(playerid, response, listitem);
        case DLG_APANEL_OPSTINA_UNOS: return Dlg_apanel_opstina_unos(playerid, response, inputtext);
        case DLG_PIJACA_PRODAJ:     return Dlg_pijaca_prodaj(playerid, response);
        case DLG_PIJACA_KUPI:       return Dlg_pijaca_kupi(playerid, response);
        case DLG_APANEL_PIJACA:     return Dlg_apanel_pijaca(playerid, response, listitem);
        case DLG_MEHANICAR:         return Dlg_mehanicar(playerid, response, listitem);
    }
    return 1;
}

InitDialog()
{
    new l[8], n = 0;
    assert(DodajStupac(l, n, "ab") && n == 3 && !strcmp(l, "ab\n"));
    assert(DodajStupac(l, n, "cd") && n == 6 && !strcmp(l, "ab\ncd\n"));
    assert(!DodajStupac(l, n, "x") && n == 6 && !strcmp(l, "ab\ncd\n"));
}

bool:SljedeciParam(const src[], &idx, dest[], size = sizeof (dest))
{
    while (src[idx] == ' ') idx++;
    new start = idx;
    while (src[idx] > ' ' && src[idx] < 127) idx++;
    strmid(dest, src, start, idx, size);
    return idx > start;
}

bool:ProvjeriBroj(const s[])
{
    if (!s[0]) return false;
    for (new i = 0; s[i]; i++) if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

ZadnjiId()
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT last_insert_rowid()");
    new id = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    return id;
}

PozicijaIgraca(playerid, &Float:x, &Float:y, &Float:z, &Float:a)
{
    new v = GetPlayerVehicleID(playerid);
    if (v)
    {
        GetVehiclePos(v, x, y, z);
        GetVehicleZAngle(v, a);
    }
    else
    {
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
    }
}

bool:DialogInterijer(const unos[], &interior, &Float:x, &Float:y, &Float:z)
{
    new idx = 0, p1[8], p2[16], p3[16], p4[16];
    if (!SljedeciParam(unos, idx, p1) || !ProvjeriBroj(p1) || strval(p1) > 255 || !SljedeciParam(unos, idx, p2) || !SljedeciParam(unos, idx, p3) || !SljedeciParam(unos, idx, p4)) return false;
    interior = strval(p1);
    x = floatstr(p2);
    y = floatstr(p3);
    z = floatstr(p4);
    return true;
}

bool:ProvjeriOvlasti(playerid, level)
{
    if (gAdmin[playerid] >= level) return true;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate ovlasti za ovu naredbu."));
    return false;
}
