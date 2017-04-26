#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

class ServerCommunicator {
	public:
	bool Authorize();
	void GetFullTree();
	int AddSecret();
	int SetSecret();
	int DeleteSecret();
};

#endif
