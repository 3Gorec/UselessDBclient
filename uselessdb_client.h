/*
 * uselessdb_client.h
 *
 *  Created on: 11 дек. 2016 г.
 *      Author: gorec
 */

#ifndef USELESSDB_CLIENT_H_
#define USELESSDB_CLIENT_H_

#include "uselessnl_core.h"

class UselessDBClient{
public:
	UselessDBClient();
	~UselessDBClient();
	int Init(int uniq_thread_id);
	int Echo(std::string &request,std::string &response);
	int Connect(std::string &u_name);
	int Disconnect();
	int Get(std::string &key, std::string &value);
	int Set(std::string &key, std::string &value);
	int Remove(std::string &key);
	int UserAdd(std::string &new_user);
	int UserRemove(std::string &user_to_del);
private:
	std::string u_name;
	UselessNLCore unl_communicator;
	bool init_flag;
	bool connection_active;
};

#endif /* USELESSDB_CLIENT_H_ */
