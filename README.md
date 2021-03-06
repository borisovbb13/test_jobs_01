## Краткое резюме
Реализация тестовых заданий.

Суть заданий описана в файле `doc/test_jobs.pdf`

## Описание
В данном репозитории содержится два проекта:
*  `test_queue` - реализация задания 1 (Очередь);
*  `test_calculator` - реализация задания 2 (Калькулятор).

## Обоснование выбора средств разработки.
В качестве языка разработки был выбран C++ с использованием фреймворка Qt. Это позволяет создавать высокопроизводительные мультиплатформенные приложения для различных целевых платформ, в том числе и мобильных.

## Комментарии относительно вариантов развития кода
### Задание 1 (Очередь)
*  Можно оптимизировать алгоритм распределения заданий.
*  Есть смысл сформировать отдельный класс для обработки данных.
*  Нужно выровнять отображение последовательностей, т.е. устранить смещение, которое происходит из-за разделителя (символ "`|`"). Для этого достаточно уменьшить количесво символов подчеркивания на единицу для каждой длительности задачи. Например, для последовательности `{3,1,4}` отображение примет вид "`|__||___|`".
*  Можно реализовать графический интерфейс. Для этого имеет смысл использовать QML + Charts.

### Задание 2 (Калькулятор)
*  Целесообразно сформировать отдельный класс для обработки данных.
*  Можно добавить возможность обработки других типов операндов (float, double).
*  Можно добавить обработку других типов операций, например умножение, деление.
*  Можно реализовать графический интерфейс. Для этого имеет смысл использовать QML.

## Зависимости
Приложения требуют наличия Qt 5.9 или новее.

## Сборка
Сборка приложений аналогична сборке любых проектов, использующих qmake.
```
$ cd test_jobs_01
$ mkdir build
$ cd ./build
$ qmake ../test_jobs_01.pro
$ make -j3
```
Для облегчения процесса сборки можно воспользоваться IDE, например QtCreator.
