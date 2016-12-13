/*
 * uselessdb.h
 *
 *  Created on: 13 дек. 2016 г.
 *      Author: gorec
 */

#ifndef USELESSDB_H_
#define USELESSDB_H_

#include "uselessdb_client.h"

class UselessDB{
public:
	static UselessDBClient Connect(std::string &u_name, int uniq_thread_id);
private:
	UselessDB();
	~UselessDB();
	//static int Init(UselessNLCore &unl_communicator, std::string &u_name, int uniq_thread_id);
	static int Echo(UselessNLCore &unl_communicator, std::string &u_name, std::string &request,std::string &response);
	static int Disconnect(UselessNLCore &unl_communicator, std::string &u_name);
	static int Get(UselessNLCore &unl_communicator, std::string &u_name, std::string &key, std::string &value);
	static int Set(UselessNLCore &unl_communicator, std::string &u_name, std::string &key, std::string &value);
	static int Remove(UselessNLCore &unl_communicator, std::string &u_name, std::string &key);
	static int UserAdd(UselessNLCore &unl_communicator, std::string &u_name, std::string &new_user);
	static int UserRemove(UselessNLCore &unl_communicator, std::string &u_name, std::string &user_to_del);
	friend UselessDBClient;
};

#endif /* USELESSDB_H_ */
