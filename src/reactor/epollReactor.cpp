/*
 * epollReactor.cpp
 *
 *  Created on: 2015年5月9日
 *      Author: hebin
 */

#include "epollReactor.h"

epollReactor::epollReactor() {
	// TODO Auto-generated constructor stub
	m_addr.sin_family = PF_INET;
	m_addr.sin_port = htons(1402);
	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	m_epollSize = 1000;
	m_listenQueue = 1;
	m_bufSize = 1000;
}

epollReactor::~epollReactor() {
	// TODO Auto-generated destructor stub
}

// Debug epoll_event
void debug_epoll_event(epoll_event ev){
        printf("fd(%d), ev.events:", ev.data.fd);

        if(ev.events & EPOLLIN)
                printf(" EPOLLIN ");
        if(ev.events & EPOLLOUT)
                printf(" EPOLLOUT ");
        if(ev.events & EPOLLET)
                printf(" EPOLLET ");
        if(ev.events & EPOLLPRI)
                printf(" EPOLLPRI ");
        if(ev.events & EPOLLRDNORM)
                printf(" EPOLLRDNORM ");
        if(ev.events & EPOLLRDBAND)
                printf(" EPOLLRDBAND ");
        if(ev.events & EPOLLWRNORM)
                printf(" EPOLLRDNORM ");
        if(ev.events & EPOLLWRBAND)
                printf(" EPOLLWRBAND ");
        if(ev.events & EPOLLMSG)
                printf(" EPOLLMSG ");
        if(ev.events & EPOLLERR)
                printf(" EPOLLERR ");
        if(ev.events & EPOLLHUP)
                printf(" EPOLLHUP ");
        if(ev.events & EPOLLONESHOT)
                printf(" EPOLLONESHOT ");

        printf("\n");

}

// *** Handle incoming message from clients
int handle_message(int client)
{

}



int epollReactor::start(){
	int listener;
	struct sockaddr_in  clients_addr;
	socklen_t socklen;
	socklen = sizeof(struct sockaddr_in);

	//     event template for epoll_ctl(ev)
	//     storage array for incoming events from epoll_wait(events)
	//     and maximum events count could be EPOLL_SIZE
	struct epoll_event ev, events[m_epollSize];
	//     watch just incoming(EPOLLIN)
	//     and Edge Trigged(EPOLLET) events
	ev.events = EPOLLIN | EPOLLET;

	//     chat message buffer
	char message[m_bufSize];

	// other values:
	//     new client descriptor(client)
	//     to keep the results of different functions(res)
	//     to keep incoming epoll_wait's events count(epoll_events_count)
	int client, res, epoll_events_count;

	// *** Setup server listener
	//     create listener with PF_INET(IPv4) and
	//     SOCK_STREAM(sequenced, reliable, two-way, connection-based byte stream)
	//    setup nonblocking socket
	listener = socket(PF_INET, SOCK_STREAM, 0);
	printf("Main listener(fd=%d) created! \n",listener);
	fcntl(listener, F_SETFL, fcntl(listener, F_GETFD, 0)|O_NONBLOCK);
	bind(listener, (struct sockaddr *)&m_addr, sizeof(m_addr));
	//    http://blog.csdn.net/ordeder/article/details/21551567
	listen(listener, m_listenQueue);

	// *** Setup epoll
	//     epoll descriptor to watch events
	int epfd;
	//     create epoll descriptor
	//     and backup store for EPOLL_SIZE of socket events
	epfd = epoll_create(m_epollSize);
	printf("Epoll(fd=%d) created!\n", epfd);
	//     set listener to event template
	ev.data.fd = listener;
	//     add listener to epoll
	epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev);
	printf("Main listener(%d) added to epoll!\n", epfd);

	//     to calculate the execution time of a program
	clock_t tStart;
		// *** Main cycle(epoll_wait)
	while(1)
	{
		epoll_events_count = epoll_wait(epfd, events, m_epollSize, EPOLL_RUN_TIMEOUT);
		 printf("-----Epoll events count: %d\n", epoll_events_count);
		// setup tStart time
		tStart = clock();

		for(int i = 0; i < epoll_events_count ; i++)
		{
			printf("events[%d].data.fd = %d\n", i, events[i].data.fd);
			debug_epoll_event(events[i]);
			// EPOLLIN event for listener(new client connection)
			if(events[i].data.fd == listener)
			{
				client =accept(listener, (struct sockaddr *) &clients_addr, &socklen);
				printf("connection from:%s:%d, socket assigned to:%d \n",
				inet_ntoa(clients_addr.sin_addr),
				ntohs(clients_addr.sin_port),
						client);
				// setup nonblocking socket
				fcntl(client, F_SETFL, fcntl(client, F_GETFD, 0)|O_NONBLOCK);

				// set new client to event template
				ev.data.fd = client;

				// add new client to epoll
				epoll_ctl(epfd, EPOLL_CTL_ADD, client, &ev);

				// save new descriptor to further use
				m_clientsList.push_back(client); // add new connection to list of clients
				printf("Add new client(fd = %d) to epoll and now clients_list.size = %d\n",
						client,
						m_clientsList.size());

				// send initial welcome message to client
				bzero(message, m_bufSize);
				res = sprintf(message,  "Welcome to seChat! You ID is: Client #%d", client);
				res = send(client, message, m_bufSize, 0);

			}else { // EPOLLIN event for others(new incoming message from client)
				res = handle_message(events[i].data.fd);
			}
		}
		// print epoll events handling statistics
		printf("Statistics: %d events handled at: %.2f second(s)\n",
				epoll_events_count,
				(double)(clock() - tStart)/CLOCKS_PER_SEC);
	}

	close(listener);
	close(epfd);

	return 0;
}
