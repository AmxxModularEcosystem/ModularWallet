#include <amxmodx>
#include <aes_v>
#include <ModularWallet>
#include "MWallet/Utils"
#include "MWallet/DebugMode"

#pragma semicolon 1
#pragma compress 1

public stock const PluginName[] = "[MWallet-C] AES";
public stock const PluginVersion[] = _MWALLET_VERSION;
public stock const PluginAuthor[] = "ArKaNeMaN";
public stock const PluginURL[] = "t.me/arkaneman";
public stock const PluginDescription[] = "AES exp and ANew for Modular Wallet";

public MWallet_OnInit() {
    RegisterPluginByVars();

    new T_Currency:iCurrency = MWallet_Currency_Create("AES-Exp", "%.0f Exp");
    MWallet_Currency_AddListener(iCurrency, Currency_OnGet, "@OnExpGet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnSet, "@OnExpSet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnDebit, "@OnExpDebit");
    MWallet_Currency_AddListener(iCurrency, Currency_OnCredit, "@OnExpCredit");

    iCurrency = MWallet_Currency_Create("AES-ANew", "%.0f ANew");
    MWallet_Currency_AddListener(iCurrency, Currency_OnGet, "@OnAnewGet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnSet, "@OnAnewSet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnDebit, "@OnAnewDebit");
    MWallet_Currency_AddListener(iCurrency, Currency_OnCredit, "@OnAnewCredit");
}

// Exp

Float:@OnExpGet(const UserId) {
    return aes_get_player_exp(UserId);
}

bool:@OnExpSet(const UserId, const Float:fAmount) {
    if (fAmount < 0) {
        return false;
    }
    
    aes_set_player_exp(UserId, fAmount, false, true);
    return true;
}

bool:@OnExpDebit(const UserId, const Float:fAmount) {
    return @OnExpSet(UserId, @OnExpGet(UserId) + fAmount);
}

bool:@OnExpCredit(const UserId, const Float:fAmount) {
    return @OnExpSet(UserId, @OnExpGet(UserId) - fAmount);
}

// ANew

Float:@OnAnewGet(const UserId) {
    return float(aes_get_player_bonus(UserId));
}

bool:@OnAnewSet(const UserId, const Float:fAmount) {
    if (fAmount < 0) {
        return false;
    }
    
    aes_set_player_bonus(UserId, floatround(fAmount), true);
    return true;
}

bool:@OnAnewDebit(const UserId, const Float:fAmount) {
    return @OnAnewSet(UserId, @OnAnewGet(UserId) + fAmount);
}

bool:@OnAnewCredit(const UserId, const Float:fAmount) {
    return @OnAnewSet(UserId, @OnAnewGet(UserId) - fAmount);
}
