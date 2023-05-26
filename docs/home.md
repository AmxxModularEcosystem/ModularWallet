# Документация Modular Wallet

Modular Wallet API имеет две "стороны":
- [Реализация валюты](#реализация-валюты);
- [Использование валюты](#использование-валюты).

## Реализация валюты

Реализация валюты заключается в реализации операций над ней, таких как:
- `Get` - Получение текущего баланса пользователя;
- `Set` - Установка нового значения баланса пользователя;
- `Credit` - Снятие указанной суммы с баланса пользователя;
- `Debit` - Начисление указанной суммы на баланс пользователя;
- `IsEnough` - Проверка наличия необходимой суммы на балансе пользователя.

Операции `Set` и `Get` обязательны к реализации, остальные по надобности.

### Поведение по умолчанию для необязательных к реализации операций

`IsEnough`:
```
return Get(UserId) >= fAmount;
```
`Credit`:
```
if (!IsEnough(UserId, fAmount)) {
    return false;
}
return Set(UserId, Get(UserId) - fAmount);
```
`Debit`:
```
return Set(UserId, Get(UserId) + fAmount);
```

Где `UserId` - индекс игрока, а `fAmount` - количество валюты.

### Процесс реализации валюты

Для добавления новой валюты в систему необходимо зарегистрировать её при помощи натива `MWallet_Currency_Create`, передав название и правила форматирования суммы для вывода.

Регистрация валюты и обработчиков операций над ней должны производиться в рамках форварда `MWallet_OnInit`.

```pawn
public MWallet_OnInit() {
    new T_Currency:iCurrency = MWallet_Currency_Create("ExampleCurrency", "$%.2f");
}
```

Натив `MWallet_Currency_Create` возвращает хендлер зарегистрированной валюты, который далее пригодится для регистрации обработчиков операций на валютой.

Для регистрации обработиков операций над валютой используется натив `MWallet_Currency_AddListener`. Тип операции определяется вторым параметром, в который передаётся одно из значений перечисления `E_CurrencyEvents` (см. файл `ModularWallet.inc`, там же описаны параметры обработчиков). Третьим параметром указывается название публичной функции-обработчика.

Для примера валюты возьмём простой глобальный массив с ячейками для каждого игрока:
```pawn
new Float:g_fExampleCurrency[MAX_PLAYERS + 1] = {0.0, ...};
```

Зарегистрируем обработчики операций:
```pawn
public MWallet_OnInit() {
    new T_Currency:iCurrency = MWallet_Currency_Create("ExampleCurrency", "$%.2f");

    MWallet_Currency_AddListener(iCurrency, Currency_OnGet, "@OnGet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnSet, "@OnSet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnIsEnough, "@OnIsEnough");
    MWallet_Currency_AddListener(iCurrency, Currency_OnCredit, "@OnCredit");
    MWallet_Currency_AddListener(iCurrency, Currency_OnDebit, "@OnDebit");
}
```

#### Обработчик операции `Get`

Функция-обработчик операции `Get` должна возвращать актуальное значение баланса пользователя в виде дробного числа (`Float`).

```pawn
Float:@OnGet(const UserId) {
    return g_fExampleCurrency[UserId];
}
```

#### Обработчик операции `Set`

Функция-обработчик операции `Set` должна изменять значение баланса пользователя на полученное значение и возвращать `true` в случае успеха или `false` в случае неудачи.

```pawn
bool:@OnSet(const UserId, const Float:fAmount) {
    // Если валюта может быть отрицательной, можно убрать эту проверку
    if (fAmount < 0) {
        return false;
    }
    
    g_fExampleCurrency[UserId] = fAmount;
    return true;
}
```

#### Обработчик операции `IsEnough`

Функция-обработчик операции `IsEnough` должна возвращать `true`, если на балансе пользователя достаточно средств для списания полученной суммы, в противном случае функция должна возвращать `false`.

```pawn
bool:@OnIsEnough(const UserId, const Float:fAmount) {
    return g_fExampleCurrency[UserId] >= fAmount;
}
```

#### Обработчик операции `Credit`

Функция-обработчик операции `Credit` должна списывать с баланса полученную сумму средств с баланса пользователя. Если списание прошло успешно, функция должна вернуть `true`, в противном случае `false`.

Операции `IsEnough` и `Credit` должны быть реализованы таким образом, чтобы при выполнении операции `IsEnough`, операция `Credit` также успешно выполнялась. В обратную сторону это правило не действует.

```pawn
bool:@OnCredit(const UserId, const Float:fAmount) {
    if (g_fExampleCurrency[UserId] < fAmount) {
        // На балансе недостаточно средств для списания
        return false;
    }

    g_fExampleCurrency[UserId] -= fAmount;
    return true;
}
```

#### Обработчик операции `Debit`

Функция-обработчик операции `Debit` должна начислять указанную сумму средств на баланс пользователя.

```pawn
bool:@OnDebit(const UserId, const Float:fAmount) {
    g_fExampleCurrency[UserId] += fAmount;

    // Если вдруг по какой-то причине валюта не может быть больше какого-то значения, можно возвращать false при переполнении
    return true;
}
```

### Полный код примера реализации валюты

```pawn
new Float:g_fExampleCurrency[MAX_PLAYERS + 1] = {0.0, ...};

public MWallet_OnInit() {
    new T_Currency:iCurrency = MWallet_Currency_Create("ExampleCurrency", "$%.2f");

    MWallet_Currency_AddListener(iCurrency, Currency_OnGet, "@OnGet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnSet, "@OnSet");
    MWallet_Currency_AddListener(iCurrency, Currency_OnIsEnough, "@OnIsEnough");
    MWallet_Currency_AddListener(iCurrency, Currency_OnCredit, "@OnCredit");
    MWallet_Currency_AddListener(iCurrency, Currency_OnDebit, "@OnDebit");
}

Float:@OnGet(const UserId) {
    return g_fExampleCurrency[UserId];
}

bool:@OnSet(const UserId, const Float:fAmount) {
    // Если валюта может быть отрицательной, можно убрать эту проверку
    if (fAmount < 0) {
        return false;
    }
    
    g_fExampleCurrency[UserId] = fAmount;
    return true;
}

bool:@OnIsEnough(const UserId, const Float:fAmount) {
    return g_fExampleCurrency[UserId] >= fAmount;
}

bool:@OnCredit(const UserId, const Float:fAmount) {
    if (g_fExampleCurrency[UserId] < fAmount) {
        // На балансе недостаточно средств для списания
        return false;
    }

    g_fExampleCurrency[UserId] -= fAmount;
    return true;
}

bool:@OnDebit(const UserId, const Float:fAmount) {
    g_fExampleCurrency[UserId] += fAmount;

    // Если вдруг по какой-то причине валюта не может быть больше какого-то значения, можно возвращать false при переполнении
    return true;
}
```

## Использование валюты

Использование валюты допускается только после вызова форварда `MWallet_OnInited`, но, чтобы не дожидаться его вызова, можно инициировать загрузку MWallet при помощи натива `MWallet_Init`, сразу после вызова которого можно использоваться валюту.

Перед использованием валюты надо каким-то способом получить хендлер нужной валюты. Обычно для этого используется натив `MWallet_Currency_Find`, который по имени валюты получает её хендлер. Имя валюты, скорее всего, будет указываться в настройках Вашего плагина, например, в кваре.

```pawn
// Как самый простой пример, но не очень удобный для пользователей...
new const CURRENCY_NAME[] = "ExampleCurrency";

new T_Currency:g_iCurrency = Invalid_Currency;

public plugin_init() {
    MWallet_Init();

    g_iCurrency = MWallet_Currency_Find(CURRENCY_NAME);
}
```

После получения хендлера валюты, можно проводить с ней различные операции при помощи следующих нативов (см. файл `ModularWallet.inc`):
- `MWallet_Currency_Get` - получение актуального баланса пользователя
- `MWallet_Currency_Set` - установка значения баланса пользователя
- `MWallet_Currency_IsEnough` - проверка возможности списания указанной суммы с баланса пользователя
- `MWallet_Currency_Credit` - списание средств с баланса пользователя
- `MWallet_Currency_Debit` - начисление средств на баланс пользователя

Также, можно получить название валюты при поможи натива `MWallet_Currency_GetName` (или его inline-версии `MWallet_Currency_iGetName`):

```pawn
server_print("Plugin using '%s' currency.", MWallet_Currency_iGetName(g_iCurrency)); // => Plugin using 'ExampleCurrency' currency.
```

И получить форматированную строку с суммой валюты при помощи натива `MWallet_Currency_Format` (или его inline-версии `MWallet_Currency_Fmt`):

```pawn
server_print("Player %n has %s.", UserId, MWallet_Currency_Fmt(g_iCurrency, MWallet_Currency_Get(g_iCurrency, UserId))); // => Player ArKaNeMaN has $123.
```


### Пример списания валюты

Для примера создадим команду продажи Deagle за валюту из MWallet:

```pawn
public plugin_init() {
    // ...
    register_clcmd("say /dgl", "@Cmd_BuyDeagle");
}

@Cmd_BuyDeagle(const UserId) {
    const Float:PRICE = 10.0;

    // Настоятельно рекомендуется использовать в подобных местах именно IsEnough, а не Get(...) >= PRICE.
    if (!MWallet_Currency_IsEnough(g_iCurrency, UserId, PRICE)) {
        client_print(UserId, print_center, "У Вас недостаточно средств.");
        return PLUGIN_HANDLED;
    }

    new ItemId = rg_give_item(UserId, "weapon_deagle", GT_DROP_AND_REPLACE);

    if (ItemId == -1) {
        // Если по какой-то причине предмет не выдался, средства не спишутся.
        client_print(UserId, print_center, "Не удалось выдать предмет.");
        return PLUGIN_HANDLED;
    }

    // Не рекомендуется для снятия средств использовать операцию Set.
    MWallet_Currency_Credit(g_iCurrency, UserId, PRICE);

    client_print(UserId, print_center, "Вы купили Deagle за %s.", MWallet_Currency_Fmt(g_iCurrency, PRICE));

    return PLUGIN_HANDLED;
}
```


### Пример начисления валюты

Для примера будем начислять валюту за каждое убийство:

```pawn
public plugin_init() {
    // ...
    RegisterHookChain(RG_CBasePlayer_Killed, "@OnPlayerKilled", true);
}

@OnPlayerKilled(const VictimId, const KillerId) {
    const Float:BONUS = 1.0;

    if (VictimId != KillerId && is_user_connected(KillerId)) {
        if (MWallet_Currency_Debit(g_iCurrency, KillerId, BONUS)) {
            client_print(KillerId, print_center, "Вы получили %s за убийство.", MWallet_Currency_Fmt(g_iCurrency, BONUS));
        }
    }
}
```


### Полный код примера использования валюты

```pawn
#include <amxmodx>
#include <reapi>
#include <ModularWallet>

new const CURRENCY_NAME[] = "ExampleCurrency";
new T_Currency:g_iCurrency = Invalid_Currency;

public plugin_init() {
    MWallet_Init();
    g_iCurrency = MWallet_Currency_Find(CURRENCY_NAME);
    
    RegisterHookChain(RG_CBasePlayer_Killed, "@OnPlayerKilled", true);
    
    register_clcmd("say /dgl", "@Cmd_BuyDeagle");
}

@OnPlayerKilled(const VictimId, const KillerId) {
    const Float:BONUS = 1.0;

    if (VictimId != KillerId && is_user_connected(KillerId)) {
        if (MWallet_Currency_Debit(g_iCurrency, KillerId, BONUS)) {
            client_print(KillerId, print_center, "Вы получили %s за убийство.", MWallet_Currency_Fmt(g_iCurrency, BONUS));
        }
    }
}

@Cmd_BuyDeagle(const UserId) {
    const Float:PRICE = 10.0;

    if (!MWallet_Currency_IsEnough(g_iCurrency, UserId, PRICE)) {
        client_print(UserId, print_center, "У Вас недостаточно средств.");
        return PLUGIN_HANDLED;
    }

    new ItemId = rg_give_item(UserId, "weapon_deagle", GT_DROP_AND_REPLACE);

    if (ItemId == -1) {
        client_print(UserId, print_center, "Не удалось выдать предмет.");
        return PLUGIN_HANDLED;
    }

    MWallet_Currency_Credit(g_iCurrency, UserId, PRICE);

    client_print(UserId, print_center, "Вы купили Deagle за %s.", MWallet_Currency_Fmt(g_iCurrency, PRICE));

    return PLUGIN_HANDLED;
}
```