// UDPServerTest.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
    //Open socket
    WSADATA data;
    WORD version = MAKEWORD(2, 2);

    int wsCheck = WSAStartup(version, &data);

    if (wsCheck != 0) {
        cout << "Unable to start web socket" << wsCheck;
        return;
    }

    //Specify ip and port
    SOCKET portListener = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);

    if (bind(portListener, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
        cout << "Unable to bind to socket" << WSAGetLastError() << endl;
        return;
    }

    sockaddr_in client;
    int clientLength = sizeof(client);
    ZeroMemory(&client, clientLength);

    char buf[1024];

    //Run loop
    while (true) {
        ZeroMemory(buf, 1024);
        //Wait for data
        int receivedBytes = recvfrom(portListener, buf, 1024, 0, (sockaddr*)&client, &clientLength);
        if (receivedBytes == SOCKET_ERROR) {
            cout << "Reception Error." << WSAGetLastError() << endl;
            continue;
        }

        //Disply Message
        char clientIp[256];
        ZeroMemory(clientIp, 256);

        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        cout << "Message received from " << clientIp << " : " << buf << endl;
    }


    //Close socket
    closesocket(portListener);

    //Clear Web Socket
    WSACleanup();

    return;
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
