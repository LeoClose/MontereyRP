// Kolizija component (kolizija/ in the repo): world collision for ray casts.

#if defined _INC_kolizija
	#endinput
#endif
#define _INC_kolizija

// First thing the segment from (x1, y1, z1) to (x2, y2, z2) hits: its model id and the hit point, or 0 if clear.
native Kolizija_Zraka(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2, &Float:x = 0.0, &Float:y = 0.0, &Float:z = 0.0);

// Adds an object's collision (same rotation as CreateObject). Returns a handle for Kolizija_Makni, 0 if the model has none.
native Kolizija_Dodaj(model, Float:x, Float:y, Float:z, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0);

native bool:Kolizija_Makni(handle);

// Like RemoveBuildingForPlayer for the server's collision: every placement of `model` (-1: any) within `radius`.
native Kolizija_Ukloni(model, Float:x, Float:y, Float:z, Float:radius);
