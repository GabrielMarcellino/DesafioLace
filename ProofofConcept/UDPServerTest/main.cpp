// UDPServerTest.cpp : Este arquivo cont�m a fun��o 'main'. A execu��o do programa come�a e termina ali.
//

#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(int argc, char* argv[])
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
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    string s(argv[1]);
    int send = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    if (send == SOCKET_ERROR) {
        cout << "Unable to bind to socket" << WSAGetLastError() << endl;
        return;
    }

    //Close socket
    closesocket(out);

    //Clear Web Socket
    WSACleanup();

    return;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depura��o
// Depurar programa: F5 ou menu Depurar > Iniciar Depura��o

// Dicas para Come�ar: 
//   1. Use a janela do Gerenciador de Solu��es para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do c�digo-fonte
//   3. Use a janela de Sa�da para ver mensagens de sa�da do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de c�digo, ou Projeto > Adicionar Item Existente para adicionar arquivos de c�digo existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, v� para Arquivo > Abrir > Projeto e selecione o arquivo. sln
