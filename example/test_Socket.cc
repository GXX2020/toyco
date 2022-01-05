#include "tinyCoApi.h"
#include "SSCall.h"
#include <functional>

using namespace toyco;

int cpIdx = 0;

class ConnProcessor
{
public:
    explicit ConnProcessor(int cfd)
     : connfd_(cfd),
       idx_(cpIdx++)
    {}
    ~ConnProcessor() = default;

    void runInCor()
    {
        char buf[128];
        while (true)
        {
            fprintf(stdout, "idx = %d : waiting for reading\n", idx_); fflush(stdout);
            int n = SSCall::read(connfd_, buf, sizeof(buf));
            fprintf(stdout, "idx = %d : waiting over\n", idx_); fflush(stdout);
            if (n == 0)
            {
                break;
            }
            buf[n] = 0;
            fprintf(stdout, "idx = %d : msg: %s\n", idx_, buf); fflush(stdout);
        }
    }

private:
    int connfd_;
    int idx_;
};

int main(int argc, char* argv[])
{
    int sockFd = SSCall::socket(AF_INET, SOCK_STREAM, 0);
    SSCall::setBlock(sockFd);

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    socklen_t clientLen;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
    serverAddr.sin_port = htons(atoi(argv[2]));

    SSCall::bind(sockFd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));

    SSCall::listen(sockFd);
    
    fprintf(stdout, "waiting......\n");

    while (true)
    {
        int connfd = SSCall::accept(sockFd, (struct sockaddr *)&clientAddr, &clientLen);
        SSCall::setNoBlock(connfd);
        fprintf(stdout, "new connfd was created : connfd = %d \n", connfd); fflush(stdout);
        ConnProcessor *cp = new ConnProcessor(connfd);
        toyco::go(std::bind(&ConnProcessor::runInCor, cp));
    }
    
    return 0;
}