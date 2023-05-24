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
    
    register_clcmd("mwallet_test_list", "@Cmd_List");
    register_clcmd("mwallet_test_select", "@Cmd_Select");
    register_clcmd("mwallet_test_get", "@Cmd_Get");
    register_clcmd("mwallet_test_set", "@Cmd_Set");
    register_clcmd("mwallet_test_credit", "@Cmd_Credit");
    register_clcmd("mwallet_test_debit", "@Cmd_Debit");
    register_clcmd("mwallet_test_enough", "@Cmd_Enough");
}

public MWallet_OnCurrencyCreated(const T_Currency:iCurrency) {
    server_print("[TEST] MWallet_Currency_OnCreated(%s)", MWallet_Currency_iGetName(iCurrency));
}

@Cmd_List(const UserId) {
    new Array:aCurrencies = MWallet_Currency_All();

    client_print(UserId, print_console, "Registered currencies:");
    for (new i = 0, ii = ArraySize(aCurrencies); i < ii; i++) {
        new T_Currency:iCurrency = ArrayGetCell(aCurrencies, i);
        client_print(UserId, print_console, "    - #%d %s (%s)", iCurrency, MWallet_Currency_iGetName(iCurrency), MWallet_Currency_Fmt(iCurrency, 123.0));
    }
    client_print(UserId, print_console, "%d total.", ArraySize(aCurrencies));
    client_print(UserId, print_console, "");

    ArrayDestroy(aCurrencies);

    return PLUGIN_HANDLED;
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

    return PLUGIN_HANDLED;
}

@Cmd_Get(const UserId) {
    TestPrint(UserId, "Get");

    return PLUGIN_HANDLED;
}

@Cmd_Set(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_Set(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Set %s", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Can`t Set %s", GetUserFormat(UserId, fAmount));
    }

    return PLUGIN_HANDLED;
}

@Cmd_Credit(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_Credit(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Credit %s", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Can`t Credit %s", GetUserFormat(UserId, fAmount));
    }

    return PLUGIN_HANDLED;
}

@Cmd_Debit(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_Debit(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Debit %s", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Can`t Debit %s", GetUserFormat(UserId, fAmount));
    }

    return PLUGIN_HANDLED;
}

@Cmd_Enough(const UserId) {
    new Float:fAmount = read_argv_float(1);
    if (MWallet_Currency_IsEnough(g_iUserSelectedCurrency[UserId], UserId, fAmount)) {
        TestPrint(UserId, "Enough %s", GetUserFormat(UserId, fAmount));
    } else {
        TestPrint(UserId, "Not Enough %s", GetUserFormat(UserId, fAmount));
    }

    return PLUGIN_HANDLED;
}
