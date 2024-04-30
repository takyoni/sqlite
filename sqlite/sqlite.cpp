// sqlite.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "sqlite3.h"

using namespace std;
static int my_special_callback(void* unused, int count, char** data, char** columns)
{
    int idx;

    for (idx = 0; idx < count; idx++) {
        printf("The data in column \"%s\" is: %s\n", columns[idx], data[idx]);
    }

    printf("\n");

    return 0;
}
int main()
{
    setlocale(LC_ALL, "Russian");
    sqlite3* Database;
    int openResult = sqlite3_open16(L"../History", &Database);
    if (openResult != SQLITE_OK) {
        wcout << L"Ошибка открытия БД" << endl;
    }
    char* errmsg;
    // Получим данные через sqlite3_exec
    int execResult = sqlite3_exec(Database, "SELECT * FROM urls LIMIT 1", my_special_callback,NULL, &errmsg);
    if (execResult != SQLITE_OK) {
        cout << errmsg << endl;
        wcout << L"Ошибка выполнения запроса" << endl;
    }
    // Получим данные через sqlite3_prepare_v2 и sqlite3_step
    sqlite3_stmt* pStatement;
    int execResult2 = sqlite3_prepare_v2(Database, "SELECT count(*) FROM urls", -1, &pStatement, NULL);
    if (execResult2 != SQLITE_OK) {
        cout << errmsg << endl;
        wcout << L"Ошибка подготовки запроса" << endl;
    }
    int execResult3 = sqlite3_step(pStatement);
    if (execResult3 == SQLITE_ROW) {
        int n = sqlite3_column_int(pStatement, 0);
        wcout << L"В таблицу urls " << n << L" записей" << endl;
    }
    sqlite3_finalize(pStatement);
    // Удалим данные через sqlite3_exec
    int execResult4 = sqlite3_exec(Database, "DELETE FROM urls WHERE id = 1", NULL, NULL, &errmsg);
    if (execResult4 != SQLITE_OK) {
        cout << errmsg << endl;
        wcout << L"Ошибка выполнения запроса" << endl;
    }
}
