# Modular Wallet

Централизованный контроллер для различных валют, таких как игровые деньги, ANew, опыт и т.д.

[Пример использования валюты через MWallet...](https://github.com/ArKaNeMaN/amxx-SimpleExtendedShop)


## Зачем нужен MWallet?

MWallet призван упростить скриптерам процесс интеграции различных валют в свои плагины. Интеграция плагина с MWallet позволит пользователям самим выбирать какую валюту они будут использовать в Вашем плагине, без необходимости как-либо модифицировать плагин под каждую валюту.

API MWallet позволяет добавить реализацию любой валюты, которая будет доступна для использования во всех плагинах, использующих MWallet.

Доступные для реализации/использования операции с валютой:
- `Get` - Получение текущего баланса пользователя;
- `Set` - Установка нового значения баланса пользователя (Не рекомендуется для использования без крайней необходимости);
- `IsEnough` - Проверка наличия необходимой суммы на балансе пользователя.
- `Credit` - Снятие указанной суммы с баланса пользователя;
- `Debit` - Начисление указанной суммы на баланс пользователя;


## Реализации различных валют

- [GameMoney](./amxmodx/scripting/MWallet-C-GameMoney.sma) - Стандартные игровые деньги
- [AES-Exp](./amxmodx/scripting/MWallet-C-Aes.sma) - Опыт из [Advanced Experience System](https://dev-cs.ru/resources/362/)
- [AES-ANew](./amxmodx/scripting/MWallet-C-Aes.sma) - Бонусные очки из [Advanced Experience System](https://dev-cs.ru/resources/362/)


## Плагины, использующие MWallet

- [Simple Extended Shop](https://github.com/ArKaNeMaN/amxx-SimpleExtendedShop) - Простенький мазагин различных предметов


## [Документация](./docs/home.md)