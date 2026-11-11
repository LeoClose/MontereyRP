// FastCMD: commands are dispatched natively by the FastCMD component (Server/components/fastcmd.so).
// Usage: CMD:ban(playerid, params[]) { ... return 1; }  - return 0 shows "Unknown command".
// Names are case-insensitive, max 27 characters. No natives: safe to compile without the component.
#if defined _fastcmd_included
    #endinput
#endif
#define _fastcmd_included

#define CMD:%0(%1) forward cmd_%0(%1); public cmd_%0(%1)
