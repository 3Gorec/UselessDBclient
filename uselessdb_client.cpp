/*
 * uselessdb_client.cpp
 *
 *  Created on: 11 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessdb_client.h"
#include "useless_protocol_parser.h"
#include <stdio.h>
#include <assert.h>

UselessDBClient::UselessDBClient(){
	connection_active=false;
	u_name.clear();
}

UselessDBClient::~UselessDBClient(){

}

int UselessDBClient::Init(int uniq_thread_id){
	int ret;
	ret=unl_communicator.Init(uniq_thread_id);
	if(ret!=0){
		init_flag=true;
	}
	return ret;
}

int UselessDBClient::Echo(std::string &request,std::string &response){
	int ret=0;

	assert(init_flag);

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

int UselessDBClient::Connect(std::string &u_name){
	int ret=0;

	assert(init_flag);

	std::string msg_buf;
	ret=UselessProtocolParser::PrepareStr(u_name,msg_buf);
	UselessNLMsg send_msg(MSGTYPE_REQ_CONNECT,msg_buf);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	if(ret==0){
		ret=unl_communicator.SendMsg(send_msg);
	}
	if(ret!=-1){
		ret=unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(recv_msg.type==MSGTYPE_RESP_CONNECT){
			this->u_name.assign(u_name);
			connection_active=true;
			ret=0;
		}
		else{
			ret=1;
		}
	}
	return ret;
}

int UselessDBClient::Disconnect(){
	int ret=0;
	std::string msg_buf;

	assert(init_flag);

	if(!connection_active){
		return 1;
	}
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
			connection_active=false;
			u_name.clear();
			ret=0;
		}
		else{
			ret=1;
		}
	}
	return ret;
}

int UselessDBClient::Get(std::string &key, std::string &value){
	int ret=0;

	assert(init_flag);

	if(!connection_active){
		return 1;
	}
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

int UselessDBClient::Set(std::string &key, std::string &value){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	assert(init_flag);

	if(!connection_active){
		return 1;
	}
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

int UselessDBClient::Remove(std::string &key){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	assert(init_flag);

	if(!connection_active){
		return 1;
	}
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

int UselessDBClient::UserAdd(std::string &new_user){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	assert(init_flag);

	if(!connection_active){
		return 1;
	}
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

int UselessDBClient::UserRemove(std::string &user_to_del){
	int ret=0;
	std::string msg_buf;
	std::list<std::string> str_list;

	assert(init_flag);

	if(!connection_active){
		return 1;
	}
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
