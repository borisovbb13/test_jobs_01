#include <QtCore/QTextStream>

static QHash<QString, int> __variables;  // Набор именнованных переменных

/*
 * Объявление функций
 */
bool parseOperand (const QString &_operand, int &_result);
bool calculate (const QString &_expression, int &_result);

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

    QTextStream _inStream(stdin);       // Поток ввода данных
    QTextStream _outStream(stdout);     // Поток вывода данных

    // Вывод информационных сообщений
    _outStream << "Write down the expression for calculation.\n";
    _outStream << "Type \"exit\" to complete the work.\n";

    while (true)
    {
        // Ввод выражения
        _outStream << "> ";
        _outStream.flush();
        QString _expression = _inStream.readLine().trimmed();

        // Если вместо последовательности указано 'exit', прерываем цикл обработки
        if (_expression.compare("exit") == 0)
            break;

        QString _varName;  // Имя переменной

        // Если имеется присваивание переменной
        if (_expression.contains('='))
        {
            QStringList _splitted = _expression.split('=');

            if (_splitted.count() != 2)
            {
                _outStream << "    Invalid expression!!!\n";
                continue;
            }

            // Находим имя переменной
            _varName = _splitted.at(0).trimmed();

            // Проверяем корректность имени переменной
            if (_varName.isNull() || _varName.isEmpty() || _varName.contains(QRegExp("[^A-Za-z]")))
            {
                _outStream << "    Invalid variable name!!!\n";
                continue;
            }

            _expression = _splitted.at(1).trimmed();
        }

        int _result;

        // Попытка разбора и вычисления выражения
        if (calculate(_expression, _result))
        {
            // Вывод результата
            _outStream << "    " << _result << "\n";

            // Если ранее нашли имя переменной, запоминаем значение переменной
            if (!_varName.isNull() && !_varName.isEmpty())
                __variables[_varName] = _result;
        }
        else
        {
            _outStream << "    Calculation failed!!!\n";
        }

        _outStream.flush();
    }

    return 0;
}

/*!
 * \brief Разбор одного операнда
 * \param _operandStr - Строка операнда
 * \param _result - Значение операнда
 * \return true, если разбор прошел успешно, в противном случае - false
 */
bool parseOperand (const QString &_operand, int &_result)
{
    bool _isOk = false;

    _result = _operand.toInt(&_isOk);

    if (!_isOk)
    {
        // Попытка сопоставления строки операнда и сохраненной переменной
        if (__variables.contains(_operand))
            _result = __variables.value(_operand);
        else
            return false;
    }

    return true;
}

/*!
 * \brief Разбор и вычисление выражения
 * \param _expression - Строка выражения
 * \param _result - Возвращаемый результат
 * \return true, если вычисление прошло успешно, в противном случае - false
 */
bool calculate (const QString &_expression, int &_result)
{
    bool _isOk = true;
    int _firstOperand;
    int _secondOperand;

    // Сложение
    if (_expression.contains('+'))
    {
        QStringList _splitted = _expression.split('+');

        // Если количество операндов не равно двум, возвращаем ошибку
        if (_splitted.count() != 2)
            return false;

        const QString _second = _splitted.at(1).trimmed();

        // Если в выражении имеется сочетание '+-', возвращаем ошибку
        if (_second.front() == '-')
            return false;

        // Разбор операндов
        _isOk = parseOperand(_splitted.at(0).trimmed(), _firstOperand);
        _isOk = _isOk && parseOperand(_second, _secondOperand);

        if (_isOk)
        {
            // Сложение операндов
            _result = _firstOperand + _secondOperand;
            return true;
        }
    }
    // Вычитание
    else if (_expression.contains('-'))
    {
        QStringList _splitted = _expression.split('-');

        // Если первый операнд является отрицательным, производим корректировку
        if (_splitted.count() == 3 && _expression.front() == '-')
        {
            _splitted.pop_front();
            _splitted[0].prepend('-');
        }

        // Если количество операндов не равно двум, возвращаем ошибку
        if (_splitted.count() != 2)
            return false;

        // Разбор операндов
        _isOk = parseOperand(_splitted.at(0).trimmed(), _firstOperand);
        _isOk = _isOk && parseOperand(_splitted.at(1).trimmed(), _secondOperand);

        if (_isOk)
        {
            // Вычитание операндов
            _result = _firstOperand - _secondOperand;
            return true;
        }
    }
    // Предположительно операнд всего один
    else
    {
        return parseOperand(_expression, _result);
    }

    return false;
}
