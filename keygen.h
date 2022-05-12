#ifndef KEYGEN_H
#define KEYGEN_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <sstream>

using namespace std;

/**
 * Вызывает коммандную строку
 * return: возвращает результат выполнения команды
*/
string exe_cmd(const char* cmd)
{
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


/**
 * Получает вывод из коммандной строки
 * Парсит на составляющие
 * return: готовый серийный номер устройства
*/
string parse_string_serial(string s, string type) {
    std::istringstream input{ s };
    std::vector<std::string> multiString;
    while (!input.eof()) {
        std::string substring;
        input >> substring;
        multiString.push_back(substring);
      }

    string result;
    int flag = 0;
    for (const std::string& substring : multiString) {
        if (type == "drive") {
            if (flag == 1) {
                result = substring;
                break;
            }
            if (substring == "\\\\.\\PHYSICALDRIVE0") {
                flag = 1;
                continue;
            }
        }
        else if (type == "motherboard") {
            if (substring == "SerialNumber") {
                flag = 1;
                continue;
            }
            if (flag == 1) {
                result = substring;
                break;
            }
        } else {
            if (substring == "ProcessorId") {
                flag = 1;
                continue;
            }
            if (flag == 1) {
                result = substring;
                break;
            }
        }
    }
    return result;
}


/**
 * Комманда получения серийного номера системного жесткого диска или ссд
 * return: серийный номер
*/
string get_system_disk_serial()
{
    string result = parse_string_serial(exe_cmd("wmic diskdrive get DeviceID,serialNumber"), "drive");
    return result;
}


/**
 * Комманда получения серийного номера материнской платы
 * return: серийный номер
*/
string get_motherboard_serial()
{
    string result = parse_string_serial(exe_cmd("wmic BaseBoard get serialnumber"), "motherboard");
    return result;
}


/**
 * Комманда получения серийного номера процессора
 * return: серийный номер
*/
string get_processor_serial()
{
    string result = parse_string_serial(exe_cmd("wmic cpu get ProcessorID"), "processor");
    return result;
}

#endif // KEYGEN_H
