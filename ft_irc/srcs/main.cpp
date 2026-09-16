#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <csignal>
#include "Server.hpp"

//서버가 계속 실행될지 판단하는 전역 플래그 : 1이면 실행 0이면 종료
volatile sig_atomic_t g_running = 1;

static void handleSignal(int signal)
{
    (void)signal;
    g_running = 0;
}

static void setupSignals()
{
    //시그널 처리 규칙을 담은 구조체
    struct sigaction sa;
    sa.sa_handler = handleSignal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    struct sigaction ignore;
    ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;

	sigaction(SIGPIPE, &ignore, NULL);
}


static bool parsePort(const char *str, int& port){
    char* end;
    long value;

    if (str[0] == '\0')
        return false;
    errno = 0;
    value = std::strtol(str, &end, 10);
    if (errno != 0 || *end != '\0')
        return false;
    if (value < 1 || value > 65535)
        return false;
    port = static_cast<int>(value);
    return true;
}

int main(int argc, char** argv)
{
    setupSignals();
    if (argc != 3){
        std::cout << "Usage: ./ircserv port password"<<std::endl;
        return 1;
    }
    int port;
    if (!parsePort(argv[1], port)){
        std::cout<<"Invalid port"<<std::endl;
        return 1;
    }
    Server server(port, argv[2]);
    if (!server.run())
        return 1;

    return 0;
}