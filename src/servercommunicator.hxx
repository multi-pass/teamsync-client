#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

class ServerCommunicator {
	public:
	bool authorize();
	void getFullTree();
	int addSecret();
	int setSecret();
	int deleteSecret();
};

#endif
