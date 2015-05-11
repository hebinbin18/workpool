//============================================================================
// Name        : main.cpp
// Author      : hebin
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "smtp/CSmtp.h"
#include "test/jsontest.h"
#include "reactor/epollReactor.h"

using namespace std;

int main() {

	epollReactor* server = new epollReactor();
	server->start();

	return 0;
}
