#if defined __mwallet_utils_included
    #endinput
#endif
#define __mwallet_utils_included

#include amxmodx

stock const __INT_TEMPLATE_STR[] = "%d";
stock const __CLANG_TEMPLATE_STR[] = "%L";
stock const __USER_NAME_TEMPLATE_STR[] = "%n";

#define CompositeMacros(%1) \
    do { %1 } while(is_linux_server() == 0xDEADBEEF)

#define IntToStr(%1) \
    fmt(__INT_TEMPLATE_STR, %1)

#define IsUserIdValid(%1) \
    (%1 >= 1 && %1 <= 32)

#define IsUserValid(%1) \
    ( \
        IsUserIdValid(%1) \
        && is_user_connected(%1) \
    )

#define IsUserAlive(%1) \
    ( \
        IsUserIdValid(%1) \
        && is_user_alive(%1) \
    )

stock __utils_ChatPrint_LayoutKey[64] = "MSG_LAYOUT";

#define ChatPrint_Init(%1) \
    copy(__utils_ChatPrint_LayoutKey, charsmax(__utils_ChatPrint_LayoutKey), %1);

#define ChatPrint(%1,%2) \
    client_print_color(%1, print_team_default, __CLANG_TEMPLATE_STR, IsUserIdValid(%1) ? %1 : -1, __utils_ChatPrint_LayoutKey, fmt(%2))

#define ChatPrintL(%1,%2) \
    ChatPrint(%1, __CLANG_TEMPLATE_STR, %1, %2)

stock __CMD_NULL_ARG = 0;
stock CMD_INIT_PARAMS() {
    __CMD_NULL_ARG = 0;
    static Cmd[32];
    read_argv(__CMD_NULL_ARG, Cmd, charsmax(Cmd));
    if (equali(Cmd, "say", 3)) {
        __CMD_NULL_ARG++;
    }
}

#define CMD_ARG(%1) \
    (__CMD_NULL_ARG + %1)

#define CMD_ARG_NUM() \
    (read_argc() - __CMD_NULL_ARG - 1)

#define BitSet(%1,%2) \
    %1 |= (1 << (%2 - 1))

#define BitReset(%1,%2) \
    %1 &= ~(1 << (%2 - 1))

#define BitIs(%1,%2) \
    (bool:(%1 & (1 << (%2 - 1))))

stock bool:HasUserFlagsStr(const UserId, const sFlags[], const bool:bStrict = false) {
    return HasUserFlags(UserId, read_flags(sFlags), bStrict);
}

stock bool:HasUserFlags(const UserId, const bitFlags, const bool:bStrict = false) {
    return HasBits(get_user_flags(UserId), bitFlags, bStrict);
}

stock bool:HasBits(const bits, const bitSearch, const bool:bStrict = false) {
    new bitRes = bits & bitSearch;
    return bStrict
        ? bitRes == bitSearch
        : bitRes > 0
}

// []GetUserName(const UserId)
#define GetUserName(%1) \
    fmt(__USER_NAME_TEMPLATE_STR,%1)

// bool:IsEqualUserName(const UserId, const sName[])
#define IsEqualUserName(%1,%2) \
    equal(GetUserName(%1), %2)

stock CreateConstCvar(const sCvarName[], const sCvarValue[]) {
    set_pcvar_string(create_cvar(sCvarName, sCvarValue), sCvarValue);
}

#define CallOnce() CompositeMacros( \
    static bool:__CallOnce_bCalled; \
    if (__CallOnce_bCalled) return; \
    __CallOnce_bCalled = true; \
)

#define CallOnceR(%1) CompositeMacros( \
    static bool:__CallOnce_bCalled; \
    if (__CallOnce_bCalled) return %1; \
    __CallOnce_bCalled = true; \
)

// https://github.com/Nord1cWarr1or/Universal-AFK-Manager/blob/6272afbb8c27f8b7ad770e3036b5960042001e6b/scripting/UAFKManager.sma#L298-L321
stock GetAmxxVersionNum() {
    static iRes;
    if (iRes) {
        return iRes;
    }

    new sAmxxVer[16];
    get_amxx_verstring(sAmxxVer, charsmax(sAmxxVer));

    if (strfind(sAmxxVer, "1.10.0") != -1) {
        iRes = 1100;
    } else if (strfind(sAmxxVer, "1.9.0") != -1) {
        iRes = 190;
    } else if (strfind(sAmxxVer, "1.8.3") != -1) {
        iRes = 183;
    } else if (strfind(sAmxxVer, "1.8.2") != -1) {
        iRes = 182;
    } else {
        iRes = 1;
    }

    return iRes;
}

stock RegisterPlugin(
    const sName[],
    const sVersion[],
    const sAuthor[],
    const sUrl[] = "",
    const sDescription[] = ""
) {
    #if AMXX_VERSION_NUM >= 1100
    if (GetAmxxVersionNum() >= 1100) {
        register_plugin(sName, sVersion, sAuthor, sUrl, sDescription);
        return;
    }
    #endif
    register_plugin(sName, sVersion, sAuthor);
}

#define RegisterPluginByVars() CompositeMacros( \
    if (GetAmxxVersionNum() < 1100) { \
        register_plugin(PluginName, PluginVersion, PluginAuthor); \
    } \
)

stock bool:StrEqualEx(const sA[], const sB[], const iCount = 0, const bool:bIgnoreCase = false) {
    return bIgnoreCase
        ? equali(sA, sB, iCount)
        : equal(sA, sB, iCount);
}
