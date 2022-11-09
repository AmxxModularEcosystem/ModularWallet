#if defined __core_currency_included
    #endinput
#endif
#define __core_currency_included

#pragma semicolon 1

#include <amxmodx>
#include "MWallet/Utils"
#include "MWallet/Forwards"
#include "MWallet/ArrayMap"
#include "MWallet/Events"

enum _:S_Currency {
    Currency_Name[MWALLET_CURRENCY_MAX_NAME_LEN],
    T_Events:Currency_Events,
}

new ArrayMap(gCurrencies);

#define Currency_Call(%1,%2,[%3]) CompositeMacros( \
    Events_SetReturnedValue(0); \
    Events_CallP(%1[Currency_Events], %2, [%3]); \
)

#define Currency_CallOrDefault(%1,%2,[%3]) \
    if (Events_HasListeners(%1[Currency_Events], %2)) { \
        Currency_Call(%1, %2, [%3]); \
    } else 

#define Currency_CallOrError(%1,%2,[%3]) CompositeMacros( \
    Currency_CallOrDefault(%1, %2, [%3]) { \
        log_error(0, "Currency `%s` must implements GET and SET methods.", %1[Currency_Name]); \
    } \
)

Currency_Init() {
    InitArrayMap(gCurrencies, S_Currency, 4);
}

Currency_GetById(const T_Currency:iCurrency, Currency[S_Currency]) {
    ArrayMapGetiArray(gCurrencies, iCurrency, Currency);
}

Currency_Count() {
    return ArrayMapSize(gCurrencies);
}

T_Currency:Currency_Create(const sName[]) {
    new Currency[S_Currency];
    
    copy(Currency[Currency_Name], charsmax(Currency[Currency_Name]), sName);
    Currency[Currency_Events] = Events_Init(E_CurrencyEvents);

    return T_Currency:ArrayMapPushArray(gCurrencies, Currency, Currency[Currency_Name]);
}

T_Currency:Currency_Find(const sName[]) {
    return T_Currency:ArrayMapGetIndex(gCurrencies, sName);
}

T_Currency:Currency_FindOrError(const sName[]) {
    new T_Currency:iCurrency = Currency_Find(sName);
    if (iCurrency == Invalid_Currency) {
        log_error(0, "Currency '%s' not found.", sName);
    }
    return iCurrency;
}

bool:Currency_AddListener(
    const T_Currency:iCurrency,
    const E_CurrencyEvents:iEvent,
    const iPlugin,
    const sCallback[]
) {
    new Currency[S_Currency];
    Currency_GetById(iCurrency, Currency);

    new iListener = -1;
    switch (iEvent) {
        case Currency_OnSet, Currency_OnIsEnough, Currency_OnCredit, Currency_OnDebit:
            // (const UserId, const iAmount)
            iListener = CreateOneForward(iPlugin, sCallback, FP_CELL, FP_CELL);

        case Currency_OnGet:
            // (const UserId)
            iListener = CreateOneForward(iPlugin, sCallback, FP_CELL);
    }

    if (iListener < 0) {
        log_error(0, "Can`t create forward for function '%s' in plugin #%d.", sCallback, iPlugin);
    }

    Events_PushListener(Currency[Currency_Events], iEvent, iListener);
}

Float:Currency_Get(const T_Currency:iCurrency, const UserId) {
    new Currency[S_Currency];
    Currency_GetById(iCurrency, Currency);

    Currency_CallOrError(Currency, Currency_OnGet, [UserId]);

    return Events_GetReturnedValue();
}

bool:Currency_Set(const T_Currency:iCurrency, const UserId, const Float:fAmount) {
    new Currency[S_Currency];
    Currency_GetById(iCurrency, Currency);

    Currency_CallOrError(Currency, Currency_OnSet, [UserId, fAmount]);

    return Events_GetReturnedValue();
}

bool:Currency_IsEnough(const T_Currency:iCurrency, const UserId, const Float:fAmount) {
    new Currency[S_Currency];
    Currency_GetById(iCurrency, Currency);

    Currency_CallOrDefault(Currency, Currency_OnIsEnough, [UserId, fAmount]) {
        return Currency_Get(iCurrency, UserId) >= fAmount;
    }

    return Events_GetReturnedValue();
}

bool:Currency_Credit(const T_Currency:iCurrency, const UserId, const Float:fAmount) {
    new Currency[S_Currency]; 
    Currency_GetById(iCurrency, Currency);

    Currency_CallOrDefault(Currency, Currency_OnCredit, [UserId, fAmount]) {
        if (Currency_IsEnough(iCurrency, UserId, fAmount)) {
            return false;
        } else {
            return Currency_Set(iCurrency, UserId, Currency_Get(iCurrency, UserId) - fAmount);
        }
    }

    return Events_GetReturnedValue();
}

bool:Currency_Debit(const T_Currency:iCurrency, const UserId, const Float:fAmount) {
    new Currency[S_Currency];
    Currency_GetById(iCurrency, Currency);

    Currency_CallOrDefault(Currency, Currency_OnDebit, [UserId, fAmount]) {
        return Currency_Set(iCurrency, UserId, Currency_Get(iCurrency, UserId) + fAmount);
    }

    return Events_GetReturnedValue();
}
