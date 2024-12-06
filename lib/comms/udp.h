/**
 * Skipper UDP utility library.
 */
#if IS_ARDUINO
#else
#ifndef SKIPPER_UDP
#define SKIPPER_UDP
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <vector>

#define MSG_NOBLOCK 0x01
#define BUFFER_LENGTH 2041

namespace udp {
    int sock;
    struct sockaddr_in addr;
    socklen_t fromlen;
    ssize_t recsize;
    std::vector<struct sockaddr_in> remote_addrs;
    char buf[BUFFER_LENGTH];
    int ptr = 0;

    void skipper_udp_init() {
        const char* host = "0.0.0.0";
        int port = 14550;

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(host);
        addr.sin_port = htons(port);
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        int enable = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            printf("setsockopt(SO_REUSEADDR) failed\n");

        struct timeval read_timeout;
        read_timeout.tv_sec = 0;
        read_timeout.tv_usec = 10;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));

        fromlen = sizeof(addr);
    }

    int skipper_udp_bind_socket()
    {
        return bind(sock, (struct sockaddr*)&(addr), sizeof(struct sockaddr));
    }

    int skipper_udp_connect_socket()
    {
        return connect(sock, (struct sockaddr*)&(addr), sizeof(struct sockaddr));
    }

    bool skipper_udp_remote_connection_exists(struct sockaddr_in* addr)
    {
        for (size_t i = 0; i < remote_addrs.size(); ++i)
        {
            if (remote_addrs[i].sin_addr.s_addr == addr->sin_addr.s_addr &&
                    remote_addrs[i].sin_port == addr->sin_port)
                return true;
        }
        return false;
    }

    void skipper_udp_add_destination(char* host, int port)
    {
        struct sockaddr_in dest_addr;

        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_addr.s_addr = inet_addr(host);
        dest_addr.sin_port = htons(port);

        remote_addrs.push_back(dest_addr);
    }

    int skipper_udp_send_bytes(uint8_t *buf, int buflen)
    {
        // When we send, just send back to every
        // stored remote address.
        for (size_t i = 0; i < remote_addrs.size(); ++i)
        {
            struct sockaddr_in dest_addr = remote_addrs[i];
            size_t len = sizeof(struct sockaddr_in);
            sendto(sock, buf, buflen, 0,
                    (struct sockaddr*)&dest_addr, len);
        }

        return 0;
    }

    int skipper_udp_recv_bytes()
    {
        struct sockaddr_in src_addr;
        socklen_t src_addr_len = sizeof(src_addr);

        memset(buf, 0, BUFFER_LENGTH);
        recsize = recvfrom(sock, (void*) buf, BUFFER_LENGTH, 0, (struct sockaddr*)&src_addr, &src_addr_len);
        if (recsize > 0)
        {
            // something received
            ptr = 0;
            if (!skipper_udp_remote_connection_exists(&src_addr))
            {
                remote_addrs.push_back(src_addr);
            }
            return recsize;
        } else {
            return 0;
        }
    }
    char skipper_udp_read_byte()
    {
        char c = buf[ptr];
        ptr += 1;
        return c;
    }
}

#endif // SKIPPER_UDP
#endif