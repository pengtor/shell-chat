#include <iostream> // std::cin , std::cout
#include <string>
#include <unistd.h> // for close
#include <thread>
#include <sys/socket.h> // for socket, bind, setsockopt...
#include <netinet/in.h>
#include <arpa/inet.h> // convert the ip to binary

int main() {

// st.1 creating the udp socket

int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd < 0) {
std::cerr << "ERROR: socket was not created\n";
return 1;
}

// st. 2 join multicast group

// multi apps bind to same port
int reuse = 1;
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
std::cerr << "ERROR: could not set SO_REUSEADDR\n";
}

//bind socket to local port + INADDR_ANY
struct sockaddr_in local_addr{};
std::memset(&local_addr, 0, sizeof(local_addr));
local_addr.sin_family = AF_INET;
local_addr.sin_port = htons(9999); //multicast port
local_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listen on local                                  // my ip

if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
    std::cerr <<  "ERROR: bind failed\n";
    return 1;
}

struct ip_mreq multicast_request;
inet_pton(AF_INET, "239.0.0.1", &multicast_request.imr_multiaddr.s_addr); // group address set
multicast_request.imr_interface.s_addr = htonl(INADDR_ANY);

//join multicast group
if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast_request, sizeof(multicast_request)) < 0) {
    std::cerr << "ERROR: Could not join multicast group\n";
    return 1;
}
std::cout << "joined multicast group\n";

// receive data using recvfrom()
close(sockfd);
return 0;
}

