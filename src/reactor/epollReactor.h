/*
 * epollReactor.h
 *
 *  Created on: 2015年5月9日
 *      Author: hebin
 */

#ifndef REACTOR_EPOLLREACTOR_H_
#define REACTOR_EPOLLREACTOR_H_
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <list>
#include <time.h>
#include <string.h>

// Default timeout - http://linux.die.net/man/2/epoll_wait
#define EPOLL_RUN_TIMEOUT -1

// Format of message population
#define STR_MESSAGE "Client #%d>> %s"

// Warning message if you alone in server
#define STR_NOONE_CONNECTED "Noone connected to server except you!"

// Commad to exit
#define CMD_EXIT "EXIT"

using namespace std;

class epollReactor {
public:
	epollReactor();
	virtual ~epollReactor();

	int start();

private:
	struct sockaddr_in m_addr;
	int m_epollSize;
	int m_listenQueue;
	int m_bufSize;
	// To store client's socket list
	list<int>  m_clientsList;
};

#endif /* REACTOR_EPOLLREACTOR_H_ */
