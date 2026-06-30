## Description
-A terminal based UDP multicast chat application written in C++. Built as a first C++ networking project to learn socket programming, UDP, and multithreading from the ground up.

## Background
- Every instance of shello running on the same local network automatically joins a multicast group.
- Any message typed in one terminal is instantly broadcast to every other active instance.

## Details
- Uses raw POSIX UDP sockets with no external networking libraries.
- Joins a multicast group at 239.0.0.1:9999 so all instances receive each other's messages.
- Runs two concurrent threads, one for receiving incoming messages and one for reading user input and sending.


## References
- IBM sockets quick reference
- cppreference.com

## Build
- g++ shello.cpp -o terminalchat -lpthread
## Run
- ./terminalchat

