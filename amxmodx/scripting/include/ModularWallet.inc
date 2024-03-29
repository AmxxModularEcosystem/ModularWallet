#if defined __modular_wallet_included
    #endinput
#endif
#define __modular_wallet_included

#include <amxmodx>
#include <json>

#define _MWALLET_VERSION "1.0.0"
stock const MWALLET_VERSION[] = _MWALLET_VERSION;
stock const MWALLET_PREFIX[] = "MWallet";

stock const MWALLET_CURRENCY_DEFAULT_FORMAT[] = "%.0f";
#define MWALLET_CURRENCY_MAX_NAME_LEN 32
// #define MWALLET_CURRENCY_MAX_TITLE_LEN 64
#define MWALLET_CURRENCY_MAX_FORMAT_LEN 16
#define MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN 64
#define MWALLET_CALLBACK_MAX_LEN 64
#define MWALLET_AMOUNT_STR_AMOUNT_MAX_LEN 32

enum T_Currency { Invalid_Currency = -1 }

/*
    События Get и Set обязательно должны обрабатываться.
    Остальные события имеют механики по-умолчанию, зависящие от Get и Set:
        IsEnough:
            return Get(UserId) >= fAmount;
        Credit:
            if (!IsEnough(UserId, fAmount)) {
                return false;
            }
            Set(UserId, Get(UserId) - fAmount);
            return true;
        Debit:
            Set(UserId, Get(UserId) + fAmount);
*/
enum E_CurrencyEvents {
    /**
     * Вызывается при получении актуального количества валюты игрока.
     *
     * @param UserId    Индекс игрока, для которого запрашивается количество валюты.
     *
     * @note            Сигнатура обработчика:
     *                  Float:(const UserId)
     *
     * @note            Обязательно должно быть обработано.
     *
     * @return          Текущее количество валюты в виде дробного числа.
     */
    Currency_OnGet,

    /**
     * Вызывается при установке количества валюты игрока.
     *
     * @param UserId    Индекс игрока, для которого запрашивается количество валюты.
     * @param fAmount   Новое количество валюты.
     *
     * @note            Сигнатура обработчика:
     *                  bool:(const UserId, const Float:fAmount)
     *
     * @note            Обязательно должно быть обработано.
     *
     * @return          true, если значение успешно установлено, иначе false.
     */
    Currency_OnSet,

    /**
     * Вызывается при проверке достаточности количества валюты игрока.
     *
     * @param UserId    Индекс игрока, для которого запрашивается количество валюты.
     * @param fAmount   Новое количество валюты.
     *
     * @note            Сигнатура обработчика:
     *                  bool:(const UserId, const Float:fAmount)
     *
     * @return          true, если валюты достаточно, иначе false.
     */
    Currency_OnIsEnough,

    /**
     * Вызывается при списании валюты у игрока.
     *
     * @param UserId    Индекс игрока, для которого запрашивается количество валюты.
     * @param fAmount   Списываемое количество валюты.
     *
     * @note            Сигнатура обработчика:
     *                  bool:(const UserId, const Float:fAmount)
     *
     * @return          true, если валюта успешно списана, иначе false.
     */
    Currency_OnCredit,

    /**
     * Вызывается при начислении валюты игроку.
     *
     * @param UserId    Индекс игрока, для которого запрашивается количество валюты.
     * @param fAmount   Начисляемое количество валюты.
     *
     * @note            Сигнатура обработчика:
     *                  bool:(const UserId, const Float:fAmount)
     *
     * @return          true, если валюта успешно начислена, иначе false.
     */
    Currency_OnDebit,
}

/**
 * Вызывается при инициализации контроллера валют.
 *
 * @note    Тут надо регистрировать валюты.
 *
 * @noreturn
 */
forward MWallet_OnInit();

/**
 * Вызывается после инициализации контроллера валют.
 *
 * @note    Тут и позже можно использовать валюты.
 *
 * @noreturn
 */
forward MWallet_OnInited();

/**
 * Вызывается после регистрации валюты.
 *
 * @param iCurrency Хендлер зарегистрированной валюты.
 *
 * @noreturn
 */
forward MWallet_OnCurrencyCreated(const T_Currency:iCurrency);

/**
 * Инициализирует контроллер валют.
 *
 * @note    Нужен в случаях, когда нет уверенности в том, что контроллер уже инициализирован.
 * @note    Если на момент вызова контроллер уже инициализирован, ничего не произойдёт.
 *
 * @noreturn
 */
native MWallet_Init();

/**
 * Возвращает массив зарегистрированных валют.
 *
 * @note            Рекомендуется вызывать не раньше срабатывания форварда MWallet_OnInited.
 * @note            Возвращаемый этим нативом массив нужно очищать после использования (ArrayDestroy).
 *
 * @return          Хендлер динамического массива с хендлерами валют.
 */
native Array:MWallet_Currency_All();

/**
 * Создаёт новую валюту с указанным названием.
 *
 * @param sName     Название валюты.
 * @param sFormat   Правило форматирования для валюты. Например: "$%.0f". По умолчанию "%.0f".
 *
 * @note            При форматировании, единственным параметром передаётся кол-во валюты в виде дробного числа.
 *
 * @return          Хендлер созданной валюты.
 */
native T_Currency:MWallet_Currency_Create(const sName[], const sFormat[] = "");

/**
 * Регистрирует обработчик указанного события.
 *
 * @param iCurrency Хендлер валюты.
 * @param iEvent    Событие (см. перечисление E_CurrencyEvents).
 * @param sCallback Название функции, которая будет обрабатывать событие.
 *
 * @note            На одно событие может быть зарегистрировано несколько обработчиков.
 *
 * @noreturn
 */
native MWallet_Currency_AddListener(const T_Currency:iCurrency, const E_CurrencyEvents:iEvent, const sCallback[]);

/**
 * Ищет валюту с указанным названием и возвращает её хендлер.
 *
 * @param sName Название валюты.
 *
 * @return      Хендлер найденной валюты или Invalid_Currency, если валюта не найдена.
 */
native T_Currency:MWallet_Currency_Find(const sName[]);

/**
 * Получает название валюты.
 *
 * @param iCurrency Хендлер валюты.
 * @param sOut      Буфер для записи ответа.
 * @param iOutLen   Размер буфера.
 *
 * @return          Количество записанных в буфер ячеек.
 */
native MWallet_Currency_GetName(const T_Currency:iCurrency, sOut[], const iOutLen);

/**
 * Получает название валюты.
 *
 * @param iCurrency Хендлер валюты.
 *
 * @note            Inline-версия натива MWallet_Currency_GetName.
 *
 * @return          Название валюты.
 */
stock MWallet_Currency_iGetName(const T_Currency:iCurrency) {
    new sName[MWALLET_CURRENCY_MAX_NAME_LEN];
    MWallet_Currency_GetName(iCurrency, sName, charsmax(sName));
    return sName;
}

/**
 * Создаёт строку с количеством валюты, в соответствии с правилами форматирования, указанными при регистрации валюты.
 *
 * @param iCurrency Хендлер валюты.
 * @param fAmount   Количество валюты.
 * @param sOut      Буфер для записи ответа.
 * @param iOutLen   Размер буфера.
 *
 * @return          Количество записанных в буфер ячеек.
 */
native MWallet_Currency_Format(const T_Currency:iCurrency, const Float:fAmount, sOut[], const iOutLen);

/**
 * Создаёт строку с количеством валюты, в соответствии с правилами форматирования, указанными при регистрации валюты.
 *
 * @param iCurrency Хендлер валюты.
 * @param fAmount   Количество валюты.
 *
 * @note            Inline-версия натива MWallet_Currency_Format.
 *
 * @return          Форматированную строку.
 */
stock MWallet_Currency_Fmt(const T_Currency:iCurrency, const Float:fAmount) {
    new sFormat[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN];
    MWallet_Currency_Format(iCurrency, fAmount, sFormat, charsmax(sFormat));
    return sFormat;
}

/**
 * Переопределяет правило форматирования для указанной валюты.
 *
 * @param sCurrencyName Название валюты.
 * @param sFormat       Правило форматирования для валюты. Например: "$%.0f".
 *
 * @note Валюта с указанным названием не обязана существовать.
 *
 * @noreturn
 */
native MWallet_Currency_OverrideFormat(const sCurrencyName[], const sFormat[]);

/**
 * Получает актуальное количество валюты.
 *
 * @param iCurrency Хендлер валюты.
 * @param UserId    Индекс игрока.
 *
 * @note            Вызывает событие Currency_OnGet.
 *
 * @error           Будет выброшена ошибка, если передан невалидный индекс игрока или невалидный хендлер валюты.
 *
 * @return          Количество валюты в виде дробного числа.
 */
native Float:MWallet_Currency_Get(const T_Currency:iCurrency, const UserId);

/**
 * Устанавливает указанное количество валюты.
 *
 * @param iCurrency Хендлер валюты.
 * @param UserId    Индекс игрока.
 * @param fAmount   Количество валюты.
 *
 * @note            Вызывает событие Currency_OnSet.
 * @note            Для списания/начисления валюты следует использовать специальные нативы.
 *
 * @error           Будет выброшена ошибка, если передан невалидный индекс игрока или невалидный хендлер валюты.
 *
 * @return          true, если операция выполнена успешно, иначе false.
 */
native bool:MWallet_Currency_Set(const T_Currency:iCurrency, const UserId, const Float:fAmount);

/**
 * Проверяет, доступно ли указанное количество валюты для трат.
 *
 * @param iCurrency Хендлер валюты.
 * @param UserId    Индекс игрока.
 * @param fAmount   Количество валюты.
 *
 * @note            Вызывает событие Currency_OnIsEnough.
 *
 * @error           Будет выброшена ошибка, если передан невалидный индекс игрока или невалидный хендлер валюты.
 *
 * @return          true, если доступно, иначе false.
 */
native bool:MWallet_Currency_IsEnough(const T_Currency:iCurrency, const UserId, const Float:fAmount);

/**
 * Снимает указанное количество валюты.
 *
 * @param iCurrency Хендлер валюты.
 * @param UserId    Индекс игрока.
 * @param fAmount   Количество валюты.
 *
 * @note            Вызывает событие Currency_OnCredit.
 *
 * @error           Будет выброшена ошибка, если передан невалидный индекс игрока или невалидный хендлер валюты.
 *
 * @return          true, если операция выполнена успешно, иначе false.
 */
native bool:MWallet_Currency_Credit(const T_Currency:iCurrency, const UserId, const Float:fAmount);

/**
 * Начисляет указанное количество валюты.
 *
 * @param iCurrency Хендлер валюты.
 * @param UserId    Индекс игрока.
 * @param fAmount   Количество валюты.
 *
 * @note            Вызывает событие Currency_OnDebit.
 *
 * @error           Будет выброшена ошибка, если передан невалидный индекс игрока или невалидный хендлер валюты.
 *
 * @return          true, если операция выполнена успешно, иначе false.
 */
native bool:MWallet_Currency_Debit(const T_Currency:iCurrency, const UserId, const Float:fAmount);

/**
 * Получает название из JSON-значения и ищет по нему валюту.
 *
 * @param jValue    JSON-значение.
 * @param bFree     Надо ли очистить JSON-значение после использования.
 *
 * @return          Хендлер найденной валюты или Invalid_Currency, если валюта не найдена.
 */
stock T_Currency:MWallet_JsonGetCurrency(JSON:jValue, const bool:bFree = false) {
    new sCurrencyName[MWALLET_CURRENCY_MAX_NAME_LEN];
    json_get_string(jValue, sCurrencyName, charsmax(sCurrencyName));

    if (bFree) {
        json_free(jValue);
    }

    return MWallet_Currency_Find(sCurrencyName);
}

/**
 * Получает название из JSON-обьекта и ищет по нему валюту.
 *
 * @param jObject   JSON-обьект.
 * @param sKey      Строковый ключ.
 * @param bDotNot   Использовать ли доступ к полям через точку.
 *
 * @return          Хендлер найденной валюты или Invalid_Currency, если валюта не найдена.
 */
stock T_Currency:MWallet_JsonObjectGetCurrency(const JSON:jObject, const sKey[], const bool:bDotNot = false) {
    return MWallet_JsonGetCurrency(json_object_get_value(jObject, sKey, bDotNot), true);
}

/**
 * Получает название из JSON-массива и ищет по нему валюту.
 *
 * @param jArray    JSON-обьект.
 * @param Index     Индекс элемента.
 *
 * @return          Хендлер найденной валюты или Invalid_Currency, если валюта не найдена.
 */
stock T_Currency:MWallet_JsonArrayGetCurrency(const JSON:jArray, const Index) {
    return MWallet_JsonGetCurrency(json_array_get_value(jArray, Index), true);
}

/**
 * Получение форматированной строки с текущим балансом игрока.
 *
 * @param iCurrency Хендлер валюты.
 * @param UserId    Индекс игрока.
 * @param sOut      Буфер для записи форматированной строки.
 * @param iOutLen   Размер буфера.
 *
 * @return          Количество записанных в буфер ячеек.
 */
stock MWallet_Currency_AccFormat(const T_Currency:iCurrency, const UserId, sOut[], const iOutLen) {
    return MWallet_Currency_Format(iCurrency, MWallet_Currency_Get(iCurrency, UserId), sOut, iOutLen);
}

/**
 * Получение форматированной строки с текущим балансом игрока.
 *
 * @param iCurrency Хендлер валюты.
 * @param UserId    Индекс игрока.
 *
 * @return          Форматированная строки.
 */
stock MWallet_Currency_AccFmt(const T_Currency:iCurrency, const UserId) {
    new sFormat[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN];
    MWallet_Currency_AccFormat(iCurrency, UserId, sFormat, charsmax(sFormat));

    return sFormat;
}

/**
 * TODO: docs
 */
stock bool:MWallet_AmountStr_Parse(const sAmountString[], &T_Currency:iCurrency = Invalid_Currency, &Float:fAmount = 0.0) {
    static sCurrencyName[MWALLET_CURRENCY_MAX_NAME_LEN], sAmountStr[MWALLET_AMOUNT_STR_AMOUNT_MAX_LEN];
    split(sAmountString, sCurrencyName, charsmax(sCurrencyName), sAmountStr, charsmax(sAmountStr), ":");

    iCurrency = MWallet_Currency_Find(sCurrencyName);
    if (iCurrency == Invalid_Currency) {
        return false;
    }
    fAmount = str_to_float(sAmountStr);

    return true;
}

stock bool:MWallet_AmountStr_Validate(const sAmountString[]) {
    return MWallet_AmountStr_Parse(sAmountString);
}

stock Float:MWallet_AmountStr_Get(const UserId, const sAmountString[]) {
    new T_Currency:iCurrency;
    if (!MWallet_AmountStr_Parse(sAmountString, iCurrency)) {
        return 0.0;
    }

    return MWallet_Currency_Get(iCurrency, UserId);
}

stock bool:MWallet_AmountStr_Set(const UserId, const sAmountString[]) {
    new T_Currency:iCurrency, Float:fAmount;
    if (!MWallet_AmountStr_Parse(sAmountString, iCurrency, fAmount)) {
        return false;
    }

    return MWallet_Currency_Set(iCurrency, UserId, fAmount);
}

stock bool:MWallet_AmountStr_IsEnough(const UserId, const sAmountString[]) {
    new T_Currency:iCurrency, Float:fAmount;
    if (!MWallet_AmountStr_Parse(sAmountString, iCurrency, fAmount)) {
        return false;
    }

    return MWallet_Currency_IsEnough(iCurrency, UserId, fAmount);
}

stock bool:MWallet_AmountStr_Credit(const UserId, const sAmountString[]) {
    new T_Currency:iCurrency, Float:fAmount;
    if (!MWallet_AmountStr_Parse(sAmountString, iCurrency, fAmount)) {
        return false;
    }

    return MWallet_Currency_Credit(iCurrency, UserId, fAmount);
}

stock bool:MWallet_AmountStr_Debit(const UserId, const sAmountString[]) {
    new T_Currency:iCurrency, Float:fAmount;
    if (!MWallet_AmountStr_Parse(sAmountString, iCurrency, fAmount)) {
        return false;
    }

    return MWallet_Currency_Debit(iCurrency, UserId, fAmount);
}

stock MWallet_AmountStr_Format(const sAmountString[], sOut[], const iOutLen) {
    new T_Currency:iCurrency, Float:fAmount;
    if (!MWallet_AmountStr_Parse(sAmountString, iCurrency, fAmount)) {
        return 0;
    }

    return MWallet_Currency_Format(iCurrency, fAmount, sOut, iOutLen);
}

stock MWallet_AmountStr_Fmt(const sAmountString[]) {
    new T_Currency:iCurrency, Float:fAmount;

    new sFormat[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN] = "";
    if (MWallet_AmountStr_Parse(sAmountString, iCurrency, fAmount)) {
        MWallet_AmountStr_Format(iCurrency, sAmountString, sFormat, charsmax(sFormat));
    }
    
    return sFormat;
}

stock MWallet_AmountStr_AccFormat(const UserId, const sAmountString[], sOut[], const iOutLen) {
    new T_Currency:iCurrency;
    if (!MWallet_AmountStr_Parse(sAmountString, iCurrency)) {
        return 0;
    }

    return MWallet_Currency_Format(iCurrency, MWallet_Currency_Get(iCurrency, UserId), sOut, iOutLen);
}

stock MWallet_AmountStr_AccFmt(const UserId, const sAmountString[]) {
    new sFormat[MWALLET_CURRENCY_MAX_FORMAT_OUT_LEN] = "";
    MWallet_AmountStr_AccFormat(UserId, sAmountString, sFormat, charsmax(sFormat));
    
    return sFormat;
}
