#include <QtCore/QTextStream>
#include <QtCore/QVector>

#include <queue>

#define SRV_COUNT 3  ///< Количество серверов в системе

/*!
 * \brief Данные о задачах на сервере
 */
struct ServerInfo
{
    int length {0};             // Общая длительность всех заданий
    QVector<int> durations;     // Вектор длительностей

    /*!
     * \brief Добавление данных о задаче
     * \param _duration - Длительность задачи
     */
    void addTask (int _duration)
    {
        length += _duration;
        durations.push_back(_duration);
    }
};

static QVector<ServerInfo> __data; ///< Данные по серверам

/*
 * Объявление функций
 */
void init (QTextStream &_stream);
bool parse (const QString &_sequence, std::priority_queue<int> &_result);
int findLessBusy ();
void displayResult (QTextStream &_stream);

/*!
 * \brief Основная функция приложения
 * \param _argc - Количество аргументов командной строки
 * \param _argv - Значения аргументов командной строки
 * \return Результат выполнения приложения
 */
int main (int _argc, char *_argv[])
{
    Q_UNUSED(_argc)
    Q_UNUSED(_argv)

    QTextStream _inStream(stdin);   // Поток ввода данных
    QTextStream _outStream(stdout); // Поток вывода данных

    init(_outStream);

    while (true)
    {
        // Ввод последовательности
        _outStream << "> ";
        _outStream.flush();
        QString _inputString = _inStream.readLine().trimmed();

        // Если вместо последовательности указано 'exit', прерываем цикл обработки
        if (_inputString.compare("exit") == 0)
            break;

        std::priority_queue<int> _sequence;

        // Парсинг введенной последовательности
        if (! parse(_inputString, _sequence))
        {
            _outStream << "    Invalid sequence!!!\n";
            continue;
        }

        // Распределяем задания по серверам
        while (!_sequence.empty())
        {
            int _index = findLessBusy();
            __data[_index].addTask(_sequence.top());
            _sequence.pop();
        }

        displayResult(_outStream);
    }

    return 0;
}

/*!
 * \brief Инициализация
 * \param _stream - Поток вывода
 */
void init (QTextStream &_stream)
{
    // Начальное заполнение
    for (int i = 0; i < SRV_COUNT; ++i)
    {
        ServerInfo _serverInfo;
        __data.push_back(_serverInfo);
    }

    // Вывод информационных сообщений
    _stream << "Specify a task sequence.\n";
    _stream << "Type \"exit\" to complete the work.\n";
}

/*!
 * \brief Разбор строки последовательности
 * \param _sequence - Строка, содержащая последовательность
 * \param _result - Результирующий вектор, содержащий последовательность
 * \return true, если разбор прошел успешно, в противном случае - false
 */
bool parse (const QString &_sequence, std::priority_queue<int> &_result)
{
    QStringList _splitted = _sequence.split(',');

    for (const auto &_item : _splitted)
    {
        bool _isOk = true;
        int _duration = _item.trimmed().toInt(&_isOk);

        if (_isOk && _duration >= 1 && _duration <= 9)
            _result.push(_duration);
        else
            return false;
    }

    return true;
}

/*!
 * \brief Поиск наименее загруженного сервера
 * \return Индекс сервера
 */
int findLessBusy ()
{
    int _result = 0;

    for (int i = 1; i < SRV_COUNT; ++i)
    {
        if (__data[i].length < __data[i-1].length)
            _result = i;
    }

    return _result;
}

/*!
 * \brief Вывод результата на экран
 * \param _stream - Поток вывода
 */
void displayResult (QTextStream &_stream)
{
    // Обрабатываем данные по "серверам"
    for (int i = 0; i < SRV_COUNT; ++i)
    {
        // Вывод начала строки
        _stream << "    " << (i + 1) << ": ";

        QVector<int> &_durations = __data[i].durations;

        // Вывод начального символа
        if (!_durations.empty())
            _stream << "|";

        // Вывод длительностей в псевдографике
        for (int j = 0; j < _durations.size(); ++j)
        {
            QString _str;
            _stream << _str.fill('_', _durations.at(j)) << "|";
        }

        _stream << "\n";
    }

    _stream.flush();
}
