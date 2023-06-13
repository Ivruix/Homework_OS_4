
# Отчёт по ИДЗ 4

## Об отчёте

**Выполнил:** Каверин Максим Вячеславович

**Группа:** БПИ217

**Вариант:** 32

**Задание:** **Вторая задача об Острове Сокровищ.** Шайка пиратов под предводительством Джона Сильвера высадилась на берег Острова Сокровищ. Не смотря на добытую карту старого Флинта, местоположение сокровищ по-прежнему остается загадкой, поэтому искать клад приходится практически на ощупь. Так как Сильвер ходит на деревянной ноге, то самому бродить по джунглям ему не с руки. Джон Сильвер поделил остров на участки, а пиратов на небольшие группы. Каждой группе поручается искать клад на нескольких участках, а сам Сильвер ждет на берегу. Группа пиратов, обшарив один участок, переходит на следующий, еще необследованный. Закончив поиски, пираты возвращаются к Сильверу и докладывают о результатах. Если какая–то из групп находит клад на одном из своих участков, она возвращается к Сильверу, который шлет попугая, инициализирующего прекращение (прерывание) всех работ. ***Требуется создать приложение, моделирующее действия Сильвера и пиратов. Сервер — Сильвер, Каждая из групп пиратов — клиент.***

Данный отчёт разбит на блоки по оценкам для удобства проверки. Работа была выполнена с учетом требований до оценки 10 включительно.

## Схема работы

```mermaid
graph LR;
    A[Джон Сильвер] -->|Участки для поиска| B[Группы пиратов];
    B -->|Результаты поисков| A;
    A -->|Прерывание работ| B;
```

Джон Сильвер назначет участки для поиска группам пиратам. Когда группа пиратов находит клад, она оповщает Джона Сильвера об этом, а тот, в свою очередь, отправляет всем группам сигнал завершения работы.

## На оценку 4-5

> Все относящиеся к данному этапу файлы находятся в папке **4-5**.

Согласно требованиям было разработано клиент–серверное приложение, в котором сервер и клиенты независимо друг от друга отображают только ту информацию, которая поступает им во время обмена. При запуске сервера требуется указать используемый порт. При запуске клиента требуется указать IP и порт сервера.

Пример вывода сервера:

```console
Waiting for clients...
Client 1 located
Client 2 located
Client 3 located
Instructions "search section 10" sent to client 1
Instructions "search section 17" sent to client 2
Instructions "search section 12" sent to client 3
Instructions "search section 1" sent to client 1
Instructions "search section 15" sent to client 2
Instructions "search section 20" sent to client 3
Treasure found by client 1
Instructions "stop work" sent to client 1
Instructions "stop work" sent to client 2
Instructions "stop work" sent to client 3
Stopping work
```

Пример вывода клиентов 1-3:

```console
Server addres 127.0.0.1:12345 is valid
Instructions received: search section 10
Treasure not found
Instructions received: search section 1
Treasure found!
Stopping work
```

```console
Server addres 127.0.0.1:12345 is valid
Instructions received: search section 17
Treasure not found
Instructions received: search section 15
Treasure not found
Instructions received: stop work
Stopping work
```

```console
Server addres 127.0.0.1:12345 is valid
Instructions received: search section 12
Treasure not found
Instructions received: search section 20
Treasure not found
Instructions received: stop work
Stopping work
```

## На оценку 6-7

> Все относящиеся к данному этапу файлы находятся в папке **6-7**.

Согласно требованиям была разработана клиентская программа, подключаемая к серверу, которая предназначена для отображение комплексной информации о выполнении приложения в целом. При ее запуске требуется указать IP и порт сервера.

Вывод данной программы:

```console
Server addres 127.0.0.1:12345 is valid
Client 1 is searching section 18
Client 2 is searching section 14
Client 3 is searching section 19
Client 1 did not find the treasure
Client 3 did not find the treasure
Client 2 did not find the treasure
Client 1 is searching section 8
Client 2 is searching section 12
Client 3 is searching section 17
Client 3 did not find the treasure
Client 1 did not find the treasure
Client 2 did not find the treasure
Client 1 is searching section 3
Client 2 is searching section 6
Client 3 is searching section 9
Client 1 did not find the treasure
Client 2 did not find the treasure
Client 3 did not find the treasure
Client 1 is searching section 11
Client 2 is searching section 13
Client 3 is searching section 16
Client 2 did not find the treasure
Client 1 did not find the treasure
Client 3 did not find the treasure
Client 1 is searching section 15
Client 2 is searching section 5
Client 3 is searching section 1
Client 2 did not find the treasure
Client 1 did not find the treasure
Client 3 found the treasure!
Client 1 is stopping it's work
Client 2 is stopping it's work
Client 3 is stopping it's work
Stopping work
```

## На оценку 8

> Все относящиеся к данному этапу файлы находятся в папке **8**.

В дополнение к предыдущей программе была реализована возможность подключения множества клиентов, обеспечивающих отображение информации о работе приложения.

Вывод клиентов-наблюдателей идентичен:

```console
Server addres 127.0.0.1:12345 is valid
Client 1 is searching section 17
Client 2 is searching section 4
Client 3 is searching section 8
Client 1 did not find the treasure
Client 2 did not find the treasure
Client 3 did not find the treasure
Client 1 is searching section 1
Client 2 is searching section 15
Client 3 is searching section 9
Client 3 did not find the treasure
Client 2 did not find the treasure
Client 1 found the treasure!
Client 1 is stopping it's work
Client 2 is stopping it's work
Client 3 is stopping it's work
Stopping work
```

```console
Server addres 127.0.0.1:12345 is valid
Client 1 is searching section 17
Client 2 is searching section 4
Client 3 is searching section 8
Client 1 did not find the treasure
Client 2 did not find the treasure
Client 3 did not find the treasure
Client 1 is searching section 1
Client 2 is searching section 15
Client 3 is searching section 9
Client 3 did not find the treasure
Client 2 did not find the treasure
Client 1 found the treasure!
Client 1 is stopping it's work
Client 2 is stopping it's work
Client 3 is stopping it's work
Stopping work
```

## На оценку 9-10

> Все относящиеся к данному этапу файлы находятся в папке **9-10**.

Было разработано приложение, позволяющее отключать и подключать клиентов с сохранением работоспособности сервера. При завершении работы сервера происходит корректное завершение работы всех подключенных клиентов.

Вывод сервера:

```console
maxim@Virtual-PC:/media/sf_Shared/OC/IHW_4/9-10$ ./server.out 12345
Waiting for clients...
Observer 1 located
Client 1 located
Client 2 located
Instructions "search section 4" sent to client 1
Instructions "search section 7" sent to client 2
Waiting for all observers to come back online!
Instructions "search section 9" sent to client 1
Instructions "search section 6" sent to client 2
Waiting for all observers to come back online!
Instructions "search section 8" sent to client 1
Instructions "search section 5" sent to client 2
Instructions "search section 3" sent to client 1
Instructions "search section 2" sent to client 2
Instructions "search section 10" sent to client 1
Instructions "search section 1" sent to client 2
Treasure found by client 2
Instructions "stop work" sent to client 1
Instructions "stop work" sent to client 2
Stopping work
```

Вывод клиента-наблюдателя:

```console
maxim@Virtual-PC:/media/sf_Shared/OC/IHW_4/9-10$ ./observer.out 127.0.0.1 12345
Server addres 127.0.0.1:12345 is valid
Client 1 is searching section 4
Client 2 is searching section 7
Client 1 did not find the treasure
Client 2 did not find the treasure
^C
maxim@Virtual-PC:/media/sf_Shared/OC/IHW_4/9-10$ ./observer.out 127.0.0.1 12345
Server addres 127.0.0.1:12345 is valid
Client 1 is searching section 9
Client 2 is searching section 6
Client 1 did not find the treasure
Client 2 did not find the treasure
^C
maxim@Virtual-PC:/media/sf_Shared/OC/IHW_4/9-10$ ./observer.out 127.0.0.1 12345
Server addres 127.0.0.1:12345 is valid
Client 1 is searching section 8
Client 2 is searching section 5
Client 1 did not find the treasure
Client 2 did not find the treasure
Client 1 is searching section 3
Client 2 is searching section 2
Client 1 did not find the treasure
Client 2 did not find the treasure
Client 1 is searching section 10
Client 2 is searching section 1
Client 1 did not find the treasure
Client 2 found the treasure!
Client 1 is stopping it's work
Client 2 is stopping it's work
Stopping work
```
