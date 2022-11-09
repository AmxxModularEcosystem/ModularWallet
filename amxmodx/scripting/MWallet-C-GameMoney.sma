#include <amxmodx>
#include <reapi>
#include <ModularWallet>
#include "MWallet/Utils"
#include "MWallet/DebugMode"

#pragma semicolon 1
#pragma compress 1

public stock const PluginName[] = "[MWallet-C] Game Money";
public stock const PluginVersion[] = _MWALLET_VERSION;
public stock const PluginAuthor[] = "ArKaNeMaN";
public stock const PluginURL[] = "t.me/arkaneman";
public stock const PluginDescription[] = "Game money currency for Modular Wallet";

public MWallet_OnInit() {
    RegisterPluginByVars();

    new T_Currency:iCurrency = MWallet_Currency_Create("GameMoney");
    MWallet_Currency_AddListener(iCurrency, Currency_OnGet, "@OnGet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnSet, "@OnSet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnDebit, "@OnDebit");
    MWallet_Currency_AddListener(iCurrency, Currency_OnCredit, "@OnCredit");

    // Хватило бы реализации Get и Set, но такой вариант позволит избежать лишних вызовов форвардов
    // А IsEnough в любом случае вызовет только один форвард, и стандартная логика в данном случае подходит, так что толку от реализации мало
}

Float:@OnGet(const UserId) {
    Dbg_Log("@OnGet(%n): %.0f", UserId, float(get_member(UserId, m_iAccount)));
    return float(get_member(UserId, m_iAccount));
}

bool:@OnSet(const UserId, const Float:fAmount) {
    if (fAmount < 0) {
        Dbg_Log("@OnSet(%n, %.0f): false", UserId, fAmount);
        return false;
    }
    
    rg_add_account(UserId, floatround(fAmount), AS_SET);
    Dbg_Log("@OnSet(%n, %.0f): true", UserId, fAmount);
    return true;
}

bool:@OnDebit(const UserId, const Float:fAmount) {
    rg_add_account(UserId, floatround(fAmount), AS_ADD);
    Dbg_Log("@OnDebit(%n, %.0f): true", UserId, fAmount);
    return true;
}

bool:@OnCredit(const UserId, const Float:fAmount) {
    if (@OnGet(UserId) < fAmount) {
        Dbg_Log("@OnCredit(%n, %.0f): false", UserId, fAmount);
        return false;
    }

    rg_add_account(UserId, -floatround(fAmount), AS_ADD);
    Dbg_Log("@OnCredit(%n, %.0f): true", UserId, fAmount);
    return true;
}
