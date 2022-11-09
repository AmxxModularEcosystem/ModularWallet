#include <amxmodx>
#include <ModularWallet>
#include "MWallet/Utils"
#include "MWallet/Forwards"
#include "MWallet/DebugMode"

#pragma semicolon 1
#pragma compress 1

public stock const PluginName[] = "Modular Wallet";
public stock const PluginVersion[] = _MWALLET_VERSION;
public stock const PluginAuthor[] = "ArKaNeMaN";
public stock const PluginURL[] = "t.me/arkaneman";
public stock const PluginDescription[] = "Centralized currency controller";

public plugin_precache() {
    PluginInit();
}

PluginInit() {
    CallOnce();

    RegisterPluginByVars();
    ForwardsInit();
    Currency_Init();

    // Тут должны регаться валюты
    Forwards_RegAndCall("Init", ET_IGNORE);
    
    // А тут использоваться
    Forwards_RegAndCall("Inited", ET_IGNORE);

    Dbg_PrintServer("%s run in debug mode.", PluginName);
}

ForwardsInit() {
    Forwards_Init(MWALLET_PREFIX);
}

#include "MWallet/Core/Natives"
