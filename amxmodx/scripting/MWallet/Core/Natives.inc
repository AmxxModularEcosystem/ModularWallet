#if defined __core_natives_included
    #endinput
#endif
#define __core_natives_included

#include <amxmodx>
#include <ModularWallet>
#include "MWallet/Natives"
#include "MWallet/Core/Currency"

T_Currency:get_param_currency(const iArg) {
    new T_Currency:iCurrency = T_Currency:get_param(iArg);
    assert iCurrency > Invalid_Currency && _:iCurrency < Currency_Count();
    return iCurrency;
}

public plugin_natives() {
    Natives_Init(MWALLET_PREFIX);

    Natives_Reg("Init");
    Natives_Reg("Currency_Create");
    Natives_Reg("Currency_AddListener");

    Natives_Reg("Currency_Find");
    Natives_Reg("Currency_AddListener");

    Natives_Reg("Currency_Get");
    Natives_Reg("Currency_Set");
    Natives_Reg("Currency_IsEnough");
    Natives_Reg("Currency_Credit");
    Natives_Reg("Currency_Debit");
}

@_Init() {
    PluginInit();
}

T_Currency:@_Currency_Create() {
    enum {Arg_sName = 1}

    new sName[MWALLET_CURRENCY_MAX_NAME_LEN];
    get_string(Arg_sName, sName, charsmax(sName));

    return Currency_Create(sName);
}

T_Currency:@_Currency_Find() {
    enum {Arg_sName = 1, Arg_bOrError}

    new sName[MWALLET_CURRENCY_MAX_NAME_LEN];
    get_string(Arg_sName, sName, charsmax(sName));
    new bool:bOrError = bool:get_param(Arg_bOrError);

    return bOrError
        ? Currency_FindOrError(sName)
        : Currency_Find(sName);
}

@_Currency_AddListener(const iPlugin) {
    enum {Arg_iCurrency = 1, Arg_iEvent, Arg_sCallback}

    new T_Currency:iCurrency = T_Currency:get_param(Arg_iCurrency);
    new E_CurrencyEvents:iEvent = E_CurrencyEvents:get_param(Arg_iEvent);

    new sCallback[MWALLET_CALLBACK_MAX_LEN];
    get_string(Arg_sCallback, sCallback, charsmax(sCallback));

    Currency_AddListener(iCurrency, iEvent, iPlugin, sCallback);
}

Float:@_Currency_Get() {
    enum {Arg_iCurrency = 1, Arg_UserId}

    return Currency_Get(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId)
    );
}

bool:@_Currency_Set() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    return Currency_Set(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}

bool:@_Currency_IsEnough() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    return Currency_IsEnough(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}

bool:@_Currency_Credit() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    return Currency_Credit(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}

bool:@_Currency_Debit() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    return Currency_Debit(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}
