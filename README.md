# EARLEY ALGORITHM

## Описание
В этой ветки репозитория formal-language_2023 хранится алгоритм эрли.
Сам код программы хранится в разбитом на библиотеки-классы виде в папке source
В месте с этим в репозитории присутствует папка grammar с граматикой для теста программы.
Также есть папка test в котором хранятся тесты для граматик из папки grammar.
### Граматики
В папке grammar лежит два файла grammar_1.txt и grammar_2.txt
В первом файле лежит граматика описывающая ПСП. Во втором файле граматика описывающая слова начинающиеся с произвольного количества букв a, в том числе и 0го, "а" закаанчивающихся на произвольное количества букв "b".

### Тесты
В папке хранятся тесты для граматик из папки grammar.
test_1.txt для grammar_1.txt. test_2.txt для grammar_2.txt.

## Запуск
Для запуска вам необходимо провести следующие процедуры:
Скопировать репозиторий к себе по SSH ключу:
```

```

Если вы запускаете программу впервые необходимо собрать проект. Для этого запустите 
```
./build.sh
```
После чего запустите файл:
```
./start.sh
```
И так вы запустили программу!

## Данные
Когда программа запущена необходим ввод.
Сначала вводится граматика в следующем формате:
В первой строке содержатся 3 целых числа 
∣N∣,∣Σ∣ и ∣P∣ --- количество нетерминальных символов, терминальных символов и правил в порождающей грамматике. Все числа неотрицательные и не превосходят 100.

Во второй строке содержатся ∣N∣ нетерминальных символов. Нетерминальные символы являются заглавными латинскими буквами.

В третьей строке содержатся ∣Σ∣ символов алфавита. Символы являются строчными латинскими буквами, цифрами, скобками или знаками арифметических операций.

В каждой из следующих |P| строк записано одно правило грамматики в формате левая часть правила -> правая части правила. 
ε в правой части правила обозначается отсутствием правой части (концом строки после ->).

Следующая строка состоит из одного нетерминального символа --- стартового символа грамматики.

Следующая строка состоит из одного целого числа 1 ≤ m ≤ 100000 --- количества слов, принадлежности которых языку надо проверить.

В каждой из следующих m строк содержится одно слово, состоящее из символов алфавита грамматики, принадлежность которого языку надо проверить. Суммарная длина всех слов не превосходит 10000000.

## Вывод
Есле слово принимается граматикой, то программы должна вывести OK иначе NO.
