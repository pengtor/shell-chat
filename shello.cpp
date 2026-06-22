#include <iostream> 
#include <string>
#include <cstring>
#include <unistd.h> 
#include <thread>
#include <sys/socket.h> // for socket, bind, setsockopt...
#include <netinet/in.h>
#include <arpa/inet.h> // convert the ip to binary

// listening func + printing it out.
void listeningforthread(std::string userInput) {
    std::cout << "Type your message: ";
    std::getline(std::cin, userInput); // listens
    std::cout << " ' " << userInput << " ' \n" << "Waiting for someone to respond... \n" << std::endl;
}

const char* multicast_ip = "239.0.0.1";
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
close(sockfd);
return 1;
}

//bind socket to local port + INADDR_ANY
struct sockaddr_in local_addr{};
std::memset(&local_addr, 0, sizeof(local_addr));
local_addr.sin_family = AF_INET;
local_addr.sin_port = htons(9999); //multicast port
local_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listen on local                                  // my ip

if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
    std::cerr <<  "ERROR: bind failed\n";
    close(sockfd);
    return 1;
}

// config multicast req struct.
struct ip_mreq multicast_request;
inet_pton(AF_INET, multicast_ip, &multicast_request.imr_multiaddr.s_addr); // group address set
multicast_request.imr_interface.s_addr = htonl(INADDR_ANY);

//join multicast group
if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast_request, sizeof(multicast_request)) < 0) {
    std::cerr << "ERROR: Could not join multicast group\n";
    close(sockfd);
    return 1;
}

std::cout << "joined multicast group\n" << multicast_ip << "\n";

// receive loop placeholder
char buffer[1024];
struct sockaddr_in sender_addr{};
socklen_t addr_len = sizeof(sender_addr);

ssize_t bytes_received = recvfrom(sockfd, buffer,sizeof(buffer) - 1, 0, (struct sockaddr*)&sender_addr, &addr_len);

if (bytes_received > 0) {
    buffer[bytes_received] = '\0';
    std::cout << "data received: " << buffer << "\n";
}

setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicast_request, sizeof(multicast_request));
close(sockfd);
return 0;
}

