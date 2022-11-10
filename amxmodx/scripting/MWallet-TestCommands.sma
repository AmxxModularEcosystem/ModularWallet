#include <amxmodx>
#include <ModularWallet>
#include "MWallet/Utils"
#include "MWallet/DebugMode"

new const __TestPrint_template[] = "[MWallet-Test] %s => %s (#%d)";
#define TestPrint(%1,%2) \
    client_print(%1, print_console, __TestPrint_template, fmt(%2), GetUserBalanceFmt(%1), g_iUserSelectedCurrency[%1])

#define GetUserBalance(%1) \
    MWallet_Currency_Get(g_iUserSelectedCurrency[%1], %1)

#define GetUserFormat(%1,%2) \
    MWallet_Currency_Fmt(g_iUserSelectedCurrency[%1], %2)

#define GetUserBalanceFmt(%1) \
    GetUserFormat(%1, GetUserBalance(%1))

#pragma semicolon 1
#pragma compress 1

public stock const PluginName[] = "[MWallet] Test Commands";
public stock const PluginVersion[] = _MWALLET_VERSION;
public stock const PluginAuthor[] = "ArKaNeMaN";
public stock const PluginURL[] = "t.me/arkaneman";
public stock const PluginDescription[] = "Some commands for testing Modular Wallet";

new T_Currency:g_iUserSelectedCurrency[MAX_PLAYERS + 1] = {Invalid_Currency, ...};

public MWallet_OnInited() {
    RegisterPluginByVars();
    
    register_clcmd("mwallet_test_select", "@Cmd_Select");
    register_clcmd("mwallet_test_get", "@Cmd_Get");
    register_clcmd("mwallet_test_set", "@Cmd_Set");
    register_clcmd("mwallet_test_credit", "@Cmd_Credit");
    register_clcmd("mwallet_test_debit", "@Cmd_Debit");
    register_clcmd("mwallet_test_enough", "@Cmd_Enough");
}

@Cmd_Select(const UserId) {
    new sCurrencyName[MWALLET_CURRENCY_MAX_NAME_LEN];
    read_argv(1, sCurrencyName, charsmax(sCurrencyName));

    g_iUserSelectedCurrency[UserId] = MWallet_Currency_Find(sCurrencyName);

    if (g_iUserSelectedCurrency[UserId] == Invalid_Currency) {
        client_print(UserId, print_console, "[MWallet-Test] Currency `%s` not found.", sCurrencyName);
    } else {
        client_print(UserId, print_console, "[MWallet-Test] Select `%s` (#%d) currency.", sCurrencyName, g_iUserSelectedCurrency[UserId]);
    }
}

@Cmd_Get(const UserId) {
    TestPrint(UserId, "Get");
}

@Cmd_Set(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_Set(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Set %.0f", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Can`t Set %.0f", GetUserFormat(UserId, fAmount));
    }
}

@Cmd_Credit(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_Credit(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Credit %.0f", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Can`t Credit %.0f", GetUserFormat(UserId, fAmount));
    }
}

@Cmd_Debit(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_Debit(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Debit %.0f", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Can`t Debit %.0f", GetUserFormat(UserId, fAmount));
    }
}

@Cmd_Enough(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_IsEnough(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Enough %.0f", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Not Enough %.0f", GetUserFormat(UserId, fAmount));
    }
}
