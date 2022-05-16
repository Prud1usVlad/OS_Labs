// Lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "MailSystem.h"
#include <filesystem>
#include <windows.h>
#include <fileapi.h>

int main(int argc, wchar_t* argv[])
{
    std::wcout << "Hello World!\n";

    MailSystem* ms = new MailSystem(".\\Mailbox\\inner");
    
    ms->CreateMailbox("MyMailbox", 100);
    ms->AddMessage("MyMailbox", "First testing message");
    ms->AddMessage("MyMailbox", "Vestibulum sit amet tristique ligula, et volutpat elit.");
    cout << ms->ReadMessage("MyMailbox", 1) << endl;
    int c = ms->GetMailSystemInfo();

    cout << "Mailboxes count: " << c;


    // FUNCTION CALLS EXAMPLES

    /*
    
        MailSystem* ms = new MailSystem(".\\Mailbox\\inner");
        MailSystem* ms = new MailSystem();

        ms->CreateMailbox("MyMailbox", 100);
        ms->CreateMailbox("MyMailbox");

        ms->AddMessage("MyMailbox", "First testing message");
        ms->AddMessage("MyMailbox", "Vestibulum sit amet tristique ligula, et volutpat elit.");

        string str = ms->ReadAndDelete("MyMailbox", 1) << endl;
        string str = ms->ReadMessage("MyMailbox", 1) << endl;

        ms->Delete("MyMailbox", 2);

        ms->Clear("MyMailbox");

        ms->MessageCount("MyMailbox")

        int c = ms->GetMailSystemInfo();
    */

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
