// UDPServerTest.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//


// Include Libraries
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment (lib, "ws2_32.lib")

// Avoid std::
using namespace std;

// Functions Prototypes
int* GetContactIp();
void PrintErrorIp();
int GetContactPort();
void PrintErrorContactPort();
int GetSelfPort();
void PrintErrorSelfPort();
void ClearPrompt();


// Main Execution
void main()
{
    // Get the  ip of the target machine
    int* contactIp = GetContactIp();

    // On debug, function to_string was modifying the original therefore the `safes` to avoid errors
    int safe0 = contactIp[0];
    int safe1 = contactIp[1];
    int safe2 = contactIp[2];
    int safe3 = contactIp[3];
    string ipAddress;
    ipAddress = to_string(safe0);
    ipAddress = ipAddress + ".";
    ipAddress = ipAddress + to_string(safe1);
    ipAddress = ipAddress + ".";
    ipAddress = ipAddress + to_string(safe2);
    ipAddress = ipAddress + ".";
    ipAddress = ipAddress + to_string(safe3);

    // Get the port listening to this connection
    int contactPort = GetContactPort();

    int selfPort = GetSelfPort();

    //Open socket to write
    WSADATA data;
    WORD version = MAKEWORD(2, 2);

    int wsCheck = WSAStartup(version, &data);

    if (wsCheck != 0) {
        cout << "Unable to start web socket" << wsCheck;
        return;
    }

    //Specify ip and port to read from
    SOCKET portListener = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(selfPort);

    fd_set fd;
    timeval tv;
    FD_ZERO(&fd);
    FD_SET(portListener, &fd);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if (bind(portListener, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
        cout << "Unable to bind to socket" << WSAGetLastError() << endl;
        return;
    }

    sockaddr_in client;
    int clientLength = sizeof(client);
    ZeroMemory(&client, clientLength);

    char bufRead[1024];

    //Specify ip and port to write
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(contactPort);
    inet_pton(AF_INET, ipAddress.c_str(), &server.sin_addr);

    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    ClearPrompt();

    cout << "Succesfully connected to " << ipAddress + ":" + to_string(contactPort) << endl;
    cout << "You are now chatting." << endl;

    string buf;

    while (1) {
        cout << ">>> ";
        getline(cin, buf);
        int send = sendto(out, buf.c_str(), buf.size() + 1, 0, (sockaddr*)&server, sizeof(server));

        if (send == SOCKET_ERROR) {
            cout << "Unable to bind to socket" << WSAGetLastError() << endl;
            return;
        }

        ZeroMemory(bufRead, 1024);
        //Wait for data
        int receivedBytes = recvfrom(portListener, bufRead, 1024, 0, (sockaddr*)&client, &clientLength);
        if (receivedBytes == SOCKET_ERROR) {
            cout << "Reception Error." << WSAGetLastError() << endl;
            continue;
        }

        //Disply Message
        char clientIp[256];
        ZeroMemory(clientIp, 256);

        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        cout << "Message received from " << clientIp << " : " << bufRead << endl;
    }

    //Close socket
    closesocket(out);
    closesocket(portListener);

    //Clear Web Socket
    WSACleanup();

    return;
}


// Functions definitions


// Asks the user for the ip they want to connect to.
int* GetContactIp()
{
    char ipAddressIn[15];

    int ipAddressNumbers[4];

    // Waits for valid inputs
    while (1) {
        cout << "Please insert the target ip address (XXX.XXX.XXX.XXX format):" << endl;
        cin >> ipAddressIn;

        // Checks if the inputed data matches ip's formatting criterias
        try{
            string s(ipAddressIn);

            // Check for the dots
            int countDot = count(s.begin(), s.end(), '.');
            if (countDot != 3){
                throw 'E';
            }

            // Splits the string and tries to convert to int
            // If conversion fails, throws error and keeps waiting for valid input 
            char* token = NULL;
            char* nextToken = NULL;
            token = strtok_s(ipAddressIn, ".", &nextToken);
            ipAddressNumbers[0] = stoi(token);

            // Check if the number lies between bounderies
            if (ipAddressNumbers[0] < 0 || ipAddressNumbers[0]>255) {
                throw 'E';
            }

            //repeats the process
            int counter = 1;
            while (token != NULL) {
                token = strtok_s(NULL, ".", &nextToken);
                ipAddressNumbers[counter] = stoi(token);
                if (ipAddressNumbers[counter] < 0 || ipAddressNumbers[counter]>255){
                    throw 'E';
                }

                counter++;
                if (counter == 4) {
                    break;
                }
            }

            return ipAddressNumbers;
        }
        // Prints invalid ip message
        catch(...) {
            PrintErrorIp();
        }
    }
}

// Prints ip error message
void PrintErrorIp() {
    cout << "The informed ip address is not valid." << endl;
}


// Asks the user which port they want to send the message to
int GetContactPort()
{
    char portIn[6];

    int ipAddressNumbers;

    // Waits for valid input
    while (1) {
        cout << "Please insert the target port:" << endl;
        cin >> portIn;

        // Tries to convert input to int
        // If conversion fails, throws error and keeps waiting for valid input 
        try {
            string s(portIn);
            ipAddressNumbers = stoi(s);

            // Check if the number lies between bounderies
            if (ipAddressNumbers < 0 || ipAddressNumbers > 65535) {
                throw 'E';
            }

            return ipAddressNumbers;
        }
        // Prints invalid port message
        catch (...) {
            PrintErrorContactPort();
        }
    }
}

// Prints port error message
void PrintErrorContactPort() {
    cout << "The informed port is not valid." << endl;
}

// Asks the user which port they want to send the message to
int GetSelfPort()
{
    char portIn[6];

    int ipAddressNumbers;

    // Waits for valid input
    while (1) {
        cout << "Please insert the listening port:" << endl;
        cin >> portIn;

        // Tries to convert input to int
        // If conversion fails, throws error and keeps waiting for valid input 
        try {
            string s(portIn);
            ipAddressNumbers = stoi(s);

            // Check if the number lies between bounderies
            if (ipAddressNumbers < 0 || ipAddressNumbers > 65535) {
                throw 'E';
            }

            return ipAddressNumbers;
        }
        // Prints invalid port message
        catch (...) {
            PrintErrorSelfPort();
        }
    }
}

// Prints port error message
void PrintErrorSelfPort() {
    cout << "The informed port is not valid." << endl;
}


//Clears command prompt after connection established
void ClearPrompt() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
