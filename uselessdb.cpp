/*
 * uselessdb.cpp
 *
 *  Created on: 13 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessdb.h"
#include "uselessdb_client.h"
#include "useless_protocol_parser.h"
#include <stdio.h>
#include <assert.h>

UselessDB::UselessDB(){

}

UselessDB::~UselessDB(){

}


int UselessDB::Echo(UselessNLCore &unl_communicator, std::string &u_name, std::string &request,std::string &response){
	int ret=0;

	std::string msg_buf;
	ret=UselessProtocolParser::PrepareStr(request,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_ECHO,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	response.clear();
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_ECHO){
			msg_buf.assign(recv_msg.data,recv_msg.data_len);
			ret=UselessProtocolParser::ParseStr(msg_buf,response);
		}
		else{
			printf("Wrong msg type %d\n",recv_msg.type);
		}
	}
	return ret;
}

UselessDBClient UselessDB::Connect(std::string &u_name, int uniq_thread_id){
	int ret=0;
	UselessDBClient db_client(u_name);

	ret=db_client.Init(uniq_thread_id);
	if(ret==0){
		std::string msg_buf;
		ret=UselessProtocolParser::PrepareStr(u_name,msg_buf);
		UselessNLMsg send_msg(MSGTYPE_REQ_CONNECT,msg_buf);
		UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
		if(ret==0){
			ret=db_client.unl_communicator.SendMsg(send_msg);
		}
		if(ret!=-1){
			ret=db_client.unl_communicator.RecvMsg(recv_msg);
		}
		if(ret!=-1){
			if(recv_msg.type==MSGTYPE_RESP_CONNECT){
				ret=0;
			}
			else{
				ret=1;
			}
		}
	}
	if(ret!=0){
		throw("Connection error");
	}
	return db_client;
}

int UselessDB::Disconnect(UselessNLCore &unl_communicator, std::string &u_name){
	int ret=0;
	std::string msg_buf;

	ret=UselessProtocolParser::PrepareStr(u_name,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_DISCONNECT,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_DISCONNECT){
			ret=0;
		}
		else{
			ret=1;
		}
	}
	return ret;
}

int UselessDB::Get(UselessNLCore &unl_communicator, std::string &u_name, std::string &key, std::string &value){
	int ret=0;

	std::string msg_buf;
	std::list<std::string> str_list;
	str_list.push_back(u_name);
	str_list.push_back(key);
	ret=UselessProtocolParser::PrepareMsgData(str_list,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_GET,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_GET){
			msg_buf.assign(recv_msg.data,recv_msg.data_len);
			ret=UselessProtocolParser::ParseMsg(msg_buf,str_list);
			if(ret==0 && str_list.size()==1){
				value.assign(*(str_list.begin()));
			}
		}
		else{
			ret=1;
		}
	}
	return ret;
}

int UselessDB::Set(UselessNLCore &unl_communicator, std::string &u_name, std::string &key, std::string &value){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	str_list.push_back(u_name);
	str_list.push_back(key);
	str_list.push_back(value);
	ret=UselessProtocolParser::PrepareMsgData(str_list,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_SET,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_SET){
			ret=0;
		}
		else{
			ret=1;
		}
	}
	return ret;
}

int UselessDB::Remove(UselessNLCore &unl_communicator, std::string &u_name, std::string &key){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	str_list.push_back(u_name);
	str_list.push_back(key);
	ret=UselessProtocolParser::PrepareMsgData(str_list,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_REMOVE,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_REMOVE){
			ret=0;
		}
		else{
			ret=1;
		}
	}
	return ret;
}

int UselessDB::UserAdd(UselessNLCore &unl_communicator, std::string &u_name, std::string &new_user){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	str_list.push_back(u_name);
	str_list.push_back(new_user);
	ret=UselessProtocolParser::PrepareMsgData(str_list,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_USERADD,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_USERADD){
			ret=0;
		}
		else{
			ret=1;
		}
	}
	return ret;
}

int UselessDB::UserRemove(UselessNLCore &unl_communicator, std::string &u_name, std::string &user_to_del){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	str_list.push_back(u_name);
	str_list.push_back(user_to_del);
	ret=UselessProtocolParser::PrepareMsgData(str_list,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_USERREMOVE,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_USERREMOVE){
			ret=0;
		}
		else{
			ret=1;
		}
	}
	return ret;
}



