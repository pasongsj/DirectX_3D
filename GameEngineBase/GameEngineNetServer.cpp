#include "PrecompileHeader.h"
#include "GameEngineNetServer.h"
#include "GameEngineDebug.h"

void GameEngineNetServer::AcceptThread(SOCKET _AcceptSocket, GameEngineNetServer* _Net)
{
    int AddressLen = sizeof(SOCKADDR_IN);
    while (true == _Net->IsNet())
    {
        SOCKADDR_IN ClientAdd;

        memset(&ClientAdd, 0, sizeof(ClientAdd));

        SOCKET ClientSocket = accept(_AcceptSocket, (sockaddr*)&ClientAdd, &AddressLen);

        if (SOCKET_ERROR == ClientSocket || INVALID_SOCKET == ClientSocket)
        {
            return;
        }

        // 이 유저가 접속했을때 꼭 해야할게 있을 가능성이 높은데.

        std::shared_ptr<GameEngineThread> NewThread = std::make_shared<GameEngineThread>();
        _Net->RecvThreads.push_back(NewThread);

        std::string ThreadName = std::to_string(ClientSocket);
        ThreadName += "Server Recv Thread";
     
        _Net->AcceptCallBack(ClientSocket, _Net);

        NewThread->Start(ThreadName, std::bind(&GameEngineNet::RecvThreadFunction, ClientSocket, _Net));
    }

    int a = 0;
}

GameEngineNetServer::GameEngineNetServer() 
{
}

GameEngineNetServer::~GameEngineNetServer() 
{
    if (0 != AcceptSocket)
    {
        closesocket(AcceptSocket);
    }
}

void GameEngineNetServer::Send(const char* Data, unsigned int _Size, int _IgnoreID)
{
    for (size_t i = 0; i < Users.size(); i++)
    {
        send(Users[i], Data, _Size, 0);
    }
}

void GameEngineNetServer::ServerOpen(short _Port, int _BackLog)
{
    WSAData WsaData;

    int errorCode = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (SOCKET_ERROR == errorCode)
    {
        MsgAssert("socket Error");
        return;
    }

    SOCKADDR_IN Add;
    Add.sin_family = AF_INET; // ip4주소 체계를 쓰겠다.
    Add.sin_port = htons(_Port); // 네트워크 통신에 유효한 에디안 방식으로 만들어준다.
    if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
    {
        return;
    }

    AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == AcceptSocket)
    {
        return;
    }

    if (SOCKET_ERROR == bind(AcceptSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
    {
        return;
    }

    BackLog = _BackLog;

    if (SOCKET_ERROR == listen(AcceptSocket, _BackLog))
    {
        return;
    }

    AccpetThread.Start("AcceptFunction", std::bind(GameEngineNetServer::AcceptThread, AcceptSocket, this));

}
