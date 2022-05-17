// Lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "MailSystem.h"
#include <filesystem>
#include <windows.h>
#include <fileapi.h>

void Help();
bool ExecuteCommand(const char* input, MailSystem** ms);



int main(int argc, wchar_t* argv[])
{
    cout << "Before the start, please make shure that files of the mail system isn't opened in file editors, to avoid mistakes and errors" << endl;
    
    MailSystem* ms = NULL;
    bool cont = true;
    char input[128] = "";

    while (cont) {
        cout << "\nEnter command. To get help type 'help' comand\n\n";
        cin >> input;
        
        cont = ExecuteCommand(input, &ms);

    }
}

bool ExecuteCommand(const char* input, MailSystem** ms) {

    string str1;
    string str2;
    unsigned long num = 0;

    if (strcmp(input, "help") == 0) {
        Help();
        return true;
    } 
    else if (strcmp(input, "start") == 0) {
        
        cout << "Write value of the first argument 'directory' (unnecesary)" << endl;
        cin >> str1;

        if (strcmp(str1.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str1.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }
        else if (strcmp(str1.c_str(), "-") == 0) {
            cout << "Standart directory .\\Mailbox opened" << endl;
            *ms = new MailSystem();
            return true;
        }
        else {
            cout << "Directory " << str1 << " opened" << endl;
            *ms = new MailSystem(str1.c_str());
            return true;
        }
    }
    else if (strcmp(input, "end") == 0) {
        return false;
    } 
    else if (strcmp(input, "create") == 0) {

        if ((*ms) == NULL) {
            cout << "There is no MailSystem object. Call start command first." << endl;
            return true;
        }

        cout << "Write value of the first argument 'name'" << endl;
        cin >> str1;

        if (strcmp(str1.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str1.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }

        cout << "Write value of the second argument 'size' (unnecesary)" << endl;
        cin >> str2;

        if (strcmp(str2.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str2.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }
        else if (strcmp(str2.c_str(), "-") == 0) {
            (* ms)->CreateMailbox(str1.c_str());
            return true;
        }
        else {
            num = atoi(str2.c_str());
            (* ms)->CreateMailbox(str1.c_str(), num);
            return true;
        }
    }
    else if (strcmp(input, "info") == 0) {

        if ((*ms) == NULL) {
            cout << "There is no MailSystem object. Call start command first." << endl;
            return true;
        }

        (*ms)->GetMailSystemInfo();
        return true;
    }
    else if (strcmp(input, "add") == 0) {
        if ((*ms) == NULL) {
            cout << "There is no MailSystem object. Call start command first." << endl;
            return true;
        }

        cout << "Write value of the first argument 'name'" << endl;
        cin >> str1;

        if (strcmp(str1.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str1.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }

        char * buf = new char[4096];
        cout << "Write value of the second argument 'text'" << endl;
        cin.getline(buf, 1);
        cin.getline(buf, 4095);

        if (strcmp(buf, "menue") == 0) {
            cout << "Returning to menue..." << endl;
            delete[] buf;
            return true;
        }
        else if (strcmp(buf, "end") == 0) {
            cout << "Closing app..." << endl;
            delete[] buf;
            return false;
        }
        else {
            (*ms)->AddMessage(str1.c_str(), buf);
            delete[] buf;
            return true;
        }
    }
    else if (strcmp(input, "read") == 0) {
        if ((*ms) == NULL) {
            cout << "There is no MailSystem object. Call start command first." << endl;
            return true;
        }

        cout << "Write value of the first argument 'name'" << endl;
        cin >> str1;

        if (strcmp(str1.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str1.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }

        cout << "Write value of the second argument 'index' (starting from 1)" << endl;
        cin >> str2;

        if (strcmp(str2.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str2.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }
        else {
            num = atoi(str2.c_str());
            string res = (*ms)->ReadMessage(str1.c_str(), num);
            cout << "\n\t" << res << endl;
            return true;
        }
    }
    else if (strcmp(input, "retrieve") == 0) {

        if ((*ms) == NULL) {
            cout << "There is no MailSystem object. Call start command first." << endl;
            return true;
        }

        cout << "Write value of the first argument 'name'" << endl;
        cin >> str1;

        if (strcmp(str1.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str1.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }

        cout << "Write value of the second argument 'index' (starting from 1)" << endl;
        cin >> str2;

        if (strcmp(str2.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str2.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }
        else {
            num = atoi(str2.c_str());
            string res = (*ms)->ReadAndDelete(str1.c_str(), num);
            cout << "\n\t" << res << endl;
            return true;
        }
    }
    else if (strcmp(input, "delete") == 0) {
    if ((*ms) == NULL) {
        cout << "There is no MailSystem object. Call start command first." << endl;
        return true;
    }

    cout << "Write value of the first argument 'name'" << endl;
    cin >> str1;

    if (strcmp(str1.c_str(), "menue") == 0) {
        cout << "Returning to menue..." << endl;
        return true;
    }
    else if (strcmp(str1.c_str(), "end") == 0) {
        cout << "Closing app..." << endl;
        return false;
    }

    cout << "Write value of the second argument 'index' (starting from 1)" << endl;
    cin >> str2;

    if (strcmp(str2.c_str(), "menue") == 0) {
        cout << "Returning to menue..." << endl;
        return true;
    }
    else if (strcmp(str2.c_str(), "end") == 0) {
        cout << "Closing app..." << endl;
        return false;
    }
    else {
        num = atoi(str2.c_str());
        (*ms)->Delete(str1.c_str(), num);
        return true;
    }
    }
    else if (strcmp(input, "clear") == 0) {
        
        if ((*ms) == NULL) {
            cout << "There is no MailSystem object. Call start command first." << endl;
            return true;
        }

        cout << "Write value of the first argument 'name'" << endl;
        cin >> str1;

        if (strcmp(str1.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str1.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }

        (*ms)->Clear(str1.c_str());
        return true;
    }
    else if (strcmp(input, "count") == 0) {

        if ((*ms) == NULL) {
            cout << "There is no MailSystem object. Call start command first." << endl;
            return true;
        }

        cout << "Write value of the first argument 'name'" << endl;
        cin >> str1;

        if (strcmp(str1.c_str(), "menue") == 0) {
            cout << "Returning to menue..." << endl;
            return true;
        }
        else if (strcmp(str1.c_str(), "end") == 0) {
            cout << "Closing app..." << endl;
            return false;
        }

        cout << "Messages count: " << (*ms)->MessageCount(str1.c_str()) << endl;
        return true;
    }
    else if (strcmp(input, "menue") == 0) {
        return true;
    }
    else {
        cout << "Invalid comand value" << endl;
        return true;
    }

    return 0;
}


void Help() {
    cout << "  Some information about commands\n\n";

    cout << "Unnecesary arguments marked with <U>. Symbol '-' used not to set that arguments\n\n";
    cout << "Arguments are added in this way:\n\n";

    cout << "  \tadd \n\t<system message> \n\tname \n\t<system message> \n\ttest message\n";

    cout << "Comands list:\n\n";

    cout << "\t- <start> - Main command, creates or re-creates MailSystem\n";
    cout << "\t  Requires one argument: \n";
    cout << "\t  <U>'directory' - path to the existing directory, where mail system will be created\n";
    cout << "\t  To create mail system in base directory (.\\Mailbox) write '-' symbol\n\n";

    cout << "\t- <end> - Command used to end program execution. Caled without parameters\n\n";

    cout << "\t- <create> - Command used to create mailbox - file with .mail extenction in directory of mail system\n";
    cout << "\t  Requires two arguments: \n";
    cout << "\t  'name' - describes file name without patn and .mail extention\n";
    cout << "\t  <U>'size' - describes maximum size of mailbox\n\n";

    cout << "\t- <info> - Command used to get detailed info about mail system, including info about mailboxes\n";
    cout << "\t  Does not require any arguments\n\n";

    cout << "\t- <add> - Used to add message to the specified mailbox in opened mail system\n";
    cout << "\t  Requires two arguments: \n";
    cout << "\t  'name' - describes file name without patn and .mail extention\n";
    cout << "\t  'text' - describes message that should be added to the mailbox\n\n";

    cout << "\t- <read> - Used to read message with specified index (starting from 1) from mailbox with specified name\n";
    cout << "\t  Requires two arguments: \n";
    cout << "\t  'name' - describes file name without patn and .mail extention\n";
    cout << "\t  'index' - index of message to be read\n\n";

    cout << "\t- <retrieve> - Used to read message with specified index (starting from 1) from mailbox with specified name,\n";
    cout << "\t  after message has been read it is deleted from the mailbox\n";
    cout << "\t  Requires two arguments: \n";
    cout << "\t  'name' - describes file name without patn and .mail extention\n";
    cout << "\t  'index' - index of message to be read and deleted\n\n";

    cout << "\t- <delete> - Used to delete message with specified index (starting from 1) from mailbox with specified name\n";
    cout << "\t  Requires two arguments: \n";
    cout << "\t  'name' - describes file name without patn and .mail extention\n";
    cout << "\t  'index' - index of message to be deleted\n\n";

    cout << "\t- <clear> - Used to delete all messages from mailbox with specified name\n";
    cout << "\t  Requires one argument: \n";
    cout << "\t  'name' - describes file name without patn and .mail extention\n\n";

    cout << "\t- <count> - Used to count messages in mailbox with specified name\n";
    cout << "\t  Requires one argument: \n";
    cout << "\t  'name' - describes file name without patn and .mail extention\n\n";

    cout << "\t- <menue> - Command used to return from any state of program to the menue\n";
    cout << "\t  Does not require any arguments\n\n";
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
