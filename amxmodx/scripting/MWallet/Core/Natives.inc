#if defined __core_natives_included
    #endinput
#endif
#define __core_natives_included

#include <amxmodx>
#include <ModularWallet>
#include "MWallet/Natives"
#include "MWallet/Core/Currency"

T_Currency:get_param_currency(const iArg) {
    return T_Currency:get_param(iArg);
}

#define ValidateCurrencyParam(%1,%2) CompositeMacros( \
    new T_Currency:__iCurrency = get_param_currency(%1); \
    if (__iCurrency <= Invalid_Currency || _:__iCurrency >= Currency_Count()) { \
        log_error(0, "Invalid currency handler (%d).", __iCurrency); \
        return %2; \
    } \
)

#define ValidateUserParam(%1,%2) CompositeMacros( \
    new __UserId = get_param(%1); \
    if (!is_user_connected(__UserId)) { \
        log_error(0, "Invalid player index (%d).", __UserId); \
        return %2; \
    } \
)

#define ValidatePosNumParam(%1,%2) CompositeMacros( \
    new __iNum = get_param(%1); \
    if (__iNum <= 0) { \
        log_error(0, "Param #%d must be a positive number (%d).", %1, __iNum); \
        return %2; \
    } \
)

public plugin_natives() {
    Natives_Init(MWALLET_PREFIX);

    Natives_Reg("Init");
    Natives_Reg("Currency_Create");
    Natives_Reg("Currency_Find");
    Natives_Reg("Currency_All");

    Natives_Reg("Currency_AddListener");
    Natives_Reg("Currency_GetName");
    Natives_Reg("Currency_Format");
    Natives_Reg("Currency_OverrideFormat");

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
    enum {Arg_sName = 1, Arg_sFormat}

    new sName[MWALLET_CURRENCY_MAX_NAME_LEN];
    get_string(Arg_sName, sName, charsmax(sName));

    new sFormat[MWALLET_CURRENCY_MAX_FORMAT_LEN];
    get_string(Arg_sFormat, sFormat, charsmax(sFormat));

    return Currency_Create(sName, sFormat);
}

Array:@_Currency_All() {
    static Array:aCurrencies;

    // Пока валюту нельзя редачить, пойдёт
    if (aCurrencies == Invalid_Array || ArraySize(aCurrencies) != Currency_Count()) {
        aCurrencies = ArrayCreate(1, Currency_Count());
        for (new i = 0, ii = Currency_Count(); i < ii; i++) {
            ArrayPushCell(aCurrencies, i);
        }
    }
    
    return ArrayClone(aCurrencies);
}

@_Currency_AddListener(const iPlugin) {
    enum {Arg_iCurrency = 1, Arg_iEvent, Arg_sCallback}

    ValidateCurrencyParam(Arg_iCurrency, );

    new T_Currency:iCurrency = T_Currency:get_param(Arg_iCurrency);
    new E_CurrencyEvents:iEvent = E_CurrencyEvents:get_param(Arg_iEvent);

    new sCallback[MWALLET_CALLBACK_MAX_LEN];
    get_string(Arg_sCallback, sCallback, charsmax(sCallback));

    Currency_AddListener(iCurrency, iEvent, iPlugin, sCallback);
}

@_Currency_GetName() {
    enum {Arg_iCurrency = 1, Arg_sOut, Arg_iOutLen}

    ValidateCurrencyParam(Arg_iCurrency, 0);

    new T_Currency:iCurrency = T_Currency:get_param(Arg_iCurrency);
    new iOutLen = get_param(Arg_iOutLen);
    
    new Currency[S_Currency];
    Currency_GetById(iCurrency, Currency);

    return set_string(Arg_sOut, Currency[Currency_Name], iOutLen);
}

@_Currency_Format() {
    enum {Arg_iCurrency = 1, Arg_fAmount, Arg_sOut, Arg_iOutLen}

    ValidateCurrencyParam(Arg_iCurrency, 0);

    new T_Currency:iCurrency = T_Currency:get_param(Arg_iCurrency);
    new Float:fAmount = get_param_f(Arg_fAmount);
    new iOutLen = get_param(Arg_iOutLen);

    new sFmt[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN];
    Currency_Format(iCurrency, fAmount, sFmt, charsmax(sFmt));

    return set_string(Arg_sOut, sFmt, iOutLen);
}

@_Currency_OverrideFormat() {
    enum {Arg_sCurrencyName = 1, Arg_sFormat}

    new sCurrencyName[MWALLET_CURRENCY_MAX_NAME_LEN];
    get_string(Arg_sCurrencyName, sCurrencyName, charsmax(sCurrencyName));

    new sFormat[MWALLET_CURRENCY_MAX_FORMAT_LEN];
    get_string(Arg_sFormat, sFormat, charsmax(sFormat));

    return Currency_PutFormatOverride(sCurrencyName, sFormat);
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

Float:@_Currency_Get() {
    enum {Arg_iCurrency = 1, Arg_UserId}

    ValidateCurrencyParam(Arg_iCurrency, 0.0);
    ValidateUserParam(Arg_UserId, 0.0);

    return Currency_OpGet(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId)
    );
}

bool:@_Currency_Set() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    ValidateCurrencyParam(Arg_iCurrency, false);
    ValidateUserParam(Arg_UserId, false);

    return Currency_OpSet(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}

bool:@_Currency_IsEnough() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    ValidatePosNumParam(Arg_fAmount, false);
    ValidateCurrencyParam(Arg_iCurrency, false);
    ValidateUserParam(Arg_UserId, false);

    return Currency_OpIsEnough(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}

bool:@_Currency_Credit() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    ValidatePosNumParam(Arg_fAmount, false);
    ValidateCurrencyParam(Arg_iCurrency, false);
    ValidateUserParam(Arg_UserId, false);

    return Currency_OpCredit(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}

bool:@_Currency_Debit() {
    enum {Arg_iCurrency = 1, Arg_UserId, Arg_fAmount}

    ValidatePosNumParam(Arg_fAmount, false);
    ValidateCurrencyParam(Arg_iCurrency, false);
    ValidateUserParam(Arg_UserId, false);

    return Currency_OpDebit(
        get_param_currency(Arg_iCurrency),
        get_param(Arg_UserId),
        get_param_f(Arg_fAmount)
    );
}
