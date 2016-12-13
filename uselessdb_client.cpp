/*
 * uselessdb_client.cpp
 *
 *  Created on: 11 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessdb_client.h"
#include "uselessdb.h"

UselessDBClient::UselessDBClient(std::string u_name):wrapper(this){
	this->u_name=u_name;
}

UselessDBClient::~UselessDBClient(){
	UselessDB::Disconnect(unl_communicator, u_name);
}


int UselessDBClient::Init(int uniq_thread_id){
	int ret=0;
	ret=unl_communicator.Init(uniq_thread_id);
	if(ret!=0){
		ret=0;
	}
	return ret;
}

int UselessDBClient::Echo(std::string &request,std::string &response){
	int ret=UselessDB::Echo(unl_communicator,u_name,request,response);
	if(ret!=0){
		throw("Echo operation failed");
	}
	return ret;
}


std::string UselessDBClient::operator [](std::string key) const{
	int ret=UselessDB::Get(unl_communicator,u_name,key,value);
	if(ret!=0){
		throw("Get operation failed");
	}
	std::string val("");
	return val;
}

UselessDBClientSetWrapper& UselessDBClient::operator [](std::string key){
	wrapper.SetKey(key);
	return wrapper;
}


//UselessDBClientSetWrapper UselessDBClient::operator [](std::string key){
//	return UselessDBClientSetWrapper(*this);
//}

int UselessDBClient::Get(std::string &key, std::string &value){

}

int UselessDBClient::Set(std::string &key, std::string &value){

}

int UselessDBClient::Remove(std::string &key){
	int ret=UselessDB::Remove(unl_communicator,u_name,key);
	if(ret!=0){
		throw("Remove operation failed");
	}
	return ret;
}

int UselessDBClient::UserAdd(std::string &new_user){
	int ret=UselessDB::UserAdd(unl_communicator,u_name,new_user);
	if(ret!=0){
		throw("UserAdd operation failed");
	}
	return ret;
}

int UselessDBClient::UserRemove(std::string &user_to_del){
	int ret=UselessDB::Remove(unl_communicator,u_name,user_to_del);
	if(ret!=0){
		throw("UserRemove operation failed");
	}
	return ret;
}


UselessDBClientSetWrapper& UselessDBClientSetWrapper::operator=(std::string const& rhs){
	int ret=0;
	std::string value=rhs;
	ret=client_ref.Set(key,value);
	if(ret!=0){
		throw("Set operation failed");
	}
}

void UselessDBClientSetWrapper::SetKey(std::string &key){
	this->key.assign(key);
}
