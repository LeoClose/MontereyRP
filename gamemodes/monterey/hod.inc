Float:HodTlo(Float:x, Float:y, Float:z)
{
    new Float:hx, Float:hy, Float:hz;
    if (Kolizija_Zraka(x, y, z + 1.0, x, y, z - 3.0, hx, hy, hz)) return hz + 1.0;
    return z;
}
