#if defined __simple_modular_wallet_included
    #endinput
#endif
#define __simple_modular_wallet_included

#include <amxmodx>
#include <ModularWallet>

static T_Currency:g_iCurrency = Invalid_Currency;

stock T_Currency:SMW_SetCurrency(const T_Currency:iCurrency) {
    return g_iCurrency = iCurrency;
}

stock T_Currency:SMW_SetCurrencyByName(const sCurrencyName[], const bool:bFail = false) {
    new T_Currency:iCurrency = SMW_SetCurrency(MWallet_Currency_Find(sCurrencyName));
    if (iCurrency == Invalid_Currency) {
        if (bFail) {
            set_fail_state("[SimpleModularWallet] Currency '%s' not found.", sCurrencyName);
        } else {
            abort(AMX_ERR_NOTFOUND, "[SimpleModularWallet] Currency '%s' not found.", sCurrencyName);
        }
    }

    return iCurrency;
}

stock T_Currency:SMW_GetCurrency(const T_Currency:iOverride = Invalid_Currency) {
    if (iOverride != Invalid_Currency) {
        return iOverride;
    }

    if (g_iCurrency != Invalid_Currency) {
        return g_iCurrency;
    }

    abort(AMX_ERR_NOTFOUND, "[SimpleModularWallet] Tried to use the global currency before setting it up.");
    return Invalid_Currency;
}

stock Float:SMW_Get(const UserId, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    return MWallet_Currency_Get(SMW_GetCurrency(iCurrencyOverride), UserId);
}

stock Float:SMW_Set(const UserId, const Float:fAmount, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    return MWallet_Currency_Set(SMW_GetCurrency(iCurrencyOverride), UserId, fAmount);
}

stock bool:SMW_Debit(const UserId, const Float:fAmount, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    return MWallet_Currency_Debit(SMW_GetCurrency(iCurrencyOverride), UserId, fAmount);
}

stock bool:SMW_Credit(const UserId, const Float:fAmount, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    return MWallet_Currency_Credit(SMW_GetCurrency(iCurrencyOverride), UserId, fAmount);
}

stock bool:SMW_IsEnough(const UserId, const Float:fAmount, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    return MWallet_Currency_IsEnough(SMW_GetCurrency(iCurrencyOverride), UserId, fAmount);
}

stock SMW_Format(const Float:fAmount, sOut[], const iOutLen, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    return MWallet_Currency_Format(SMW_GetCurrency(iCurrencyOverride), fAmount, sOut, iOutLen);
}

stock SMW_Fmt(const Float:fAmount, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    new sOut[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN];
    SMW_Format(fAmount, sOut, charsmax(sOut), iCurrencyOverride);
    
    return sOut;
}

stock SMW_AccFormat(const UserId, sOut[], const iOutLen, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    return MWallet_Currency_AccFormat(SMW_GetCurrency(iCurrencyOverride), UserId, sOut, iOutLen);
}

stock SMW_AccFmt(const UserId, const T_Currency:iCurrencyOverride = Invalid_Currency) {
    new sOut[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN];
    SMW_AccFormat(UserId, sOut, charsmax(sOut), iCurrencyOverride);
    
    return sOut;
}
