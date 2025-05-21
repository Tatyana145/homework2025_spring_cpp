#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <vector>
#include <string>

// Enum для уровней важности
enum LogLevel {
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR
};

// Класс Log (Singleton)
class Log {
private:
    std::vector<std::pair<LogLevel, std::string>> entries;
    const size_t maxEntries = 10;

    // Приватный конструктор (для Singleton)
    Log() {}

    // Приватный деструктор (для Singleton)
    ~Log() {}

    // Удаляем конструктор копирования и оператор присваивания, чтобы запретить копирование
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

public:
    // Получение единственного экземпляра
    static Log* Instance();

    // Метод для записи сообщения в лог
    void message(LogLevel level, const std::string& message);

    // Метод для вывода последних записей лога
    void print() const;
};

#endif