#if defined __modular_wallet_included
    #endinput
#endif
#define __modular_wallet_included

#define _MWALLET_VERSION "1.0.0-alpha"
stock const MWALLET_VERSION[] = _MWALLET_VERSION;
stock const MWALLET_PREFIX[] = "MWallet";

stock const MWALLET_CURRENCY_DEFAULT_FORMAT[] = "%.0f";
#define MWALLET_CURRENCY_MAX_NAME_LEN 32
#define MWALLET_CURRENCY_MAX_FORMAT_LEN 16
#define MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN 32
#define MWALLET_CALLBACK_MAX_LEN 64

enum T_Currency { Invalid_Currency = -1 }

/*
    События Get и Set обязательно должны обрабатываться.
    Остальные события имеют механики по-умолчанию, зависящие от Get и Set:
        IsEnough:
            return Get(UserId) >= fAmount;
        Credit:
            if (!IsEnought(UserId, fAmount)) {
                return false;
            }
            Set(UserId, Get(UserId) - fAmount);
            return true;
        Debit:
            Set(UserId, Get(UserId) + fAmount);
*/
enum E_CurrencyEvents {
    // Float:(const UserId)
    Currency_OnGet,

    // bool:(const UserId, const Float:fAmount)
    Currency_OnSet,

    // bool:(const UserId, const Float:fAmount)
    Currency_OnIsEnough,

    // bool:(const UserId, const Float:fAmount)
    Currency_OnCredit,

    // bool:(const UserId, const Float:fAmount)
    Currency_OnDebit,
}

// TODO: Расписать доки)

// Это для расширений
forward MWallet_OnInit();

native MWallet_Init();
native T_Currency:MWallet_Currency_Create(const sName[], const sFormat[] = "");
native MWallet_Currency_AddListener(const T_Currency:iCurrency, const E_CurrencyEvents:iEvent, const sCallback[]);

// Это для использования
forward MWallet_OnInited();

native T_Currency:MWallet_Currency_Find(const sName[]);
native MWallet_Currency_Format(const T_Currency:iCurrency, const Float:fAmount, sOut[], const iOutLen);

// Thx: the_hunter за способ реализации inline функции форматирования
stock MWallet_Currency_Fmt(const T_Currency:iCurrency, const Float:fAmount) {
    new sFormat[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN];
    MWallet_Currency_Format(iCurrency, fAmount, sFormat, charsmax(sFormat));
    return sFormat;
}

native Float:MWallet_Currency_Get(const T_Currency:iCurrency, const UserId);
native bool:MWallet_Currency_Set(const T_Currency:iCurrency, const UserId, const Float:fAmount);
native bool:MWallet_Currency_IsEnough(const T_Currency:iCurrency, const UserId, const Float:fAmount);
native bool:MWallet_Currency_Credit(const T_Currency:iCurrency, const UserId, const Float:fAmount);
native bool:MWallet_Currency_Debit(const T_Currency:iCurrency, const UserId, const Float:fAmount);
