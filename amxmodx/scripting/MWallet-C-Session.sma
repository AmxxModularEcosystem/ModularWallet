#include <amxmodx>
#include <reapi>
#include <ModularWallet>
#include "MWallet/Utils"

#pragma semicolon 1
#pragma compress 1

public stock const PluginName[] = "[MWallet-C] Session";
public stock const PluginVersion[] = _MWALLET_VERSION;
public stock const PluginAuthor[] = "ArKaNeMaN";
public stock const PluginURL[] = "t.me/arkanaplugins";
public stock const PluginDescription[] = "Currency which living during player session";

new Float:g_fUserMoney[MAX_PLAYERS + 1] = {0.0, ...};

public MWallet_OnInit() {
    RegisterPluginByVars();

    new T_Currency:iCurrency = MWallet_Currency_Create("Session");
    MWallet_Currency_AddListener(iCurrency, Currency_OnGet, "@OnGet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnSet, "@OnSet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnDebit, "@OnDebit");
    MWallet_Currency_AddListener(iCurrency, Currency_OnCredit, "@OnCredit");
}

public client_putinserver(UserId) {
    g_fUserMoney[UserId] = 0.0;
}

Float:@OnGet(const UserId) {
    return g_fUserMoney[UserId];
}

bool:@OnSet(const UserId, const Float:fAmount) {
    if (fAmount < 0) {
        return false;
    }

    g_fUserMoney[UserId] = fAmount;
    return true;
}

bool:@OnDebit(const UserId, const Float:fAmount) {
    g_fUserMoney[UserId] += fAmount;
    return true;
}

bool:@OnCredit(const UserId, const Float:fAmount) {
    if (@OnGet(UserId) < fAmount) {
        return false;
    }

    g_fUserMoney[UserId] -= fAmount;
    return true;
}
