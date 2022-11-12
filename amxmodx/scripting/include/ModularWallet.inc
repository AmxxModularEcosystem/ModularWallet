#if defined __modular_wallet_included
    #endinput
#endif
#define __modular_wallet_included

#define _MWALLET_VERSION "1.0.0-beta.1"
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
    /**
     * Вызывается при получении актуального количества валюты игрока.
     *
     * @param UserId    Индекс игрока, для которого запрашивается количество валюты.
     *
     * @note            Float:(const UserId)
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
     * @note            bool:(const UserId, const Float:fAmount)
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
     * @note            bool:(const UserId, const Float:fAmount)
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
     * @note            bool:(const UserId, const Float:fAmount)
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
     * @note            bool:(const UserId, const Float:fAmount)
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
 * Инициализирует контроллер валют.
 *
 * @note    Нужен в случаях, когда нет уверенности в том, что контроллер уже инициализирован.
 * @note    Если на момент вызова контроллер уже инициализирован, ничего не произойдёт.
 *
 * @noreturn
 */
native MWallet_Init();

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
