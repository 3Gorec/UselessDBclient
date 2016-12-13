/*
 * uselessdb_client.h
 *
 *  Created on: 11 дек. 2016 г.
 *      Author: gorec
 */

#ifndef USELESSDB_CLIENT_H_
#define USELESSDB_CLIENT_H_

#include "uselessnl_core.h"
#include <string>

class UselessDB;
class UselessDBClient;

class UselessDBClientSetWrapper{
public:
	UselessDBClientSetWrapper(UselessDBClient &client):client_ref(client){	}
	~UselessDBClientSetWrapper();
	UselessDBClientSetWrapper& operator=(std::string const& rhs);
	void SetKey(std::string &key);
private:
	UselessDBClient &client_ref;
	std::string key;
};

class UselessDBClient{
public:
	UselessDBClient(std::string u_name);
	~UselessDBClient();
	int Echo(std::string &request,std::string &response);
	std::string operator [](std::string key) const;
	UselessDBClientSetWrapper& operator [](std::string key);
	int Remove(std::string &key);
	int UserAdd(std::string &new_user);
	int UserRemove(std::string &user_to_del);
private:
	int Get(std::string &key, std::string &value);
	int Set(std::string &key, std::string &value);
	int Init(int uniq_thread_id);
	std::string u_name;
	std::string value;
	UselessNLCore unl_communicator;
	UselessDBClientSetWrapper wrapper;
	friend UselessDB;
	friend UselessDBClientSetWrapper;
};



#endif /* USELESSDB_CLIENT_H_ */
