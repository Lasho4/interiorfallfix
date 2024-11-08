/*
 - This include helps fix the annoying issue where a player, after teleporting to an interior,
 - falls through the floor due to connection problems.
 - The script will teleport the player back to their last teleportation location if it detects that the player is falling.

 - This include works on a Plug and Play principle.
 - In other words, to make it work, you just need to include it as an include right after the main includes (a_samp or open.mp and anti-cheat).
 - You can customize the script�s settings using a set of defines.
 - For a complete fix of the issue, it�s recommended to either freeze players during teleportation or use static platforms in interiors (CreateObject).

 - Version: 1.2.
 - Date: 08.10.2024.
 - GitHub Repo: https://github.com/timmylich/interiorfallfix

 - Connect with author:
 - VKontake: https://vk.com/timmylich
 - Telegram: https://t.me/timmylich
*/

#tryinclude <a_samp>

/*
[EN] The time that after teleportation will be checked whether the player falls in milliseconds.
[RU] �����, ������� ����� ������������ ����� ����������� ������ �� ����� � �������������.
*/
static ifx_FixTime = 7000;

/*
[EN] Function for getting the player's interior.
[RU] ������� ��� ��������� ��������� ������.
 - %0 - playerid
*/
#if !defined ifx_GetPlayerInterior
    #define ifx_GetPlayerInterior(%0) GetPlayerInterior(%0)
#endif

/*
[EN] Timing between checks in OnPlayerUpdate (ms).
[RU] ������� ����� �������������� �������� � OnPlayerUpdate (� �������������).
*/
#if !defined ifx_PlayerUpdateCD
    #define ifx_PlayerUpdateCD 250
#endif

/*
[EN] The player's falling speed required for the script to trigger.
[RU] �������� ������� ������, ����������� ��� ������������ �������.
*/
#if !defined ifx_FallVelocity
    #define ifx_FallVelocity 0.3
#endif


enum e_ifx_player_data
{
    
    Float:_LastTeleportPos[3],
    _LastTeleportTime,
    _LastPlayerUpdate
}
static
        ifx_Data[MAX_PLAYERS][e_ifx_player_data];


// SetPlayerPos hook
stock ifx_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
    ifx_Data[playerid][_LastTeleportTime] = GetTickCount();
    
    ifx_Data[playerid][_LastTeleportPos][0] = x;
    ifx_Data[playerid][_LastTeleportPos][1] = y;
    ifx_Data[playerid][_LastTeleportPos][2] = z;
    return SetPlayerPos(playerid, x, y, z);
}
#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif
#define SetPlayerPos ifx_SetPlayerPos


// OnPlayerUpdate hook
public OnPlayerUpdate(playerid)
{
    if(GetTickCount()-ifx_PlayerUpdateCD > ifx_Data[playerid][_LastPlayerUpdate])
    {
        ifx_Data[playerid][_LastPlayerUpdate] = GetTickCount();
        if(ifx_Data[playerid][_LastTeleportTime]+ifx_FixTime > GetTickCount() && ifx_GetPlayerInterior(playerid))
        {
            static Float:FallVelocity;
            GetPlayerVelocity(playerid, FallVelocity, FallVelocity, FallVelocity);

            if(FallVelocity < -ifx_FallVelocity)
            {
                SetPlayerPos(playerid, ifx_Data[playerid][_LastTeleportPos][0], ifx_Data[playerid][_LastTeleportPos][1], ifx_Data[playerid][_LastTeleportPos][2]);
            }
        }
    }
	#if defined ifx_OnPlayerUpdate
		return ifx_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate ifx_OnPlayerUpdate
#if defined ifx_OnPlayerUpdate
	forward ifx_OnPlayerUpdate(playerid);
#endif

stock ifx_SetFixTime(time_ms)
{
    ifx_FixTime = time_ms;
}
stock ifx_GetFixTime()
{
    return ifx_FixTime;
}
