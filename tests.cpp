/*
 * tests.cpp
 *
 *  Created on: 11 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessdb_client.h"
#include <pthread.h>
#include <stdio.h>
#include <string>

static void* Thread_EchoFunction(void* argc);

#define PROCESS_RET(expr, err_code) do{ \
										if(expr){	\
											printf("success\n");	\
										}	\
										else{	\
											printf("FAIL\n");	\
											code=(err_code);	\
											goto exit;	\
										}	\
									}while(0)



void TestEchoMultithreading(){
	pthread_t thread1;
	pthread_t thread2;
	int thread_num1=1;
	int thread_num2=2;
	pthread_create(&thread1,NULL,Thread_EchoFunction,&thread_num1);
	pthread_create(&thread2,NULL,Thread_EchoFunction,&thread_num2);
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	printf("Test finished\n");
}

void TestUseslessClient(){
	int ret=0;
	int code=0;
	std::string root("root");
	std::string test_usr("test_usr");
	std::string key1("key 1_1");
	std::string val1("value 1 1");
	std::string readback;
	UselessDBClient db_client;
	printf("DB client initializing: ");
	ret=db_client.Init(0);
	PROCESS_RET(ret!=0,1);

	printf("Connecting as %s: ",root.data());
	ret=db_client.Connect(root);
	PROCESS_RET(ret==0,2);

	printf("Add user %s: ",test_usr.data());
	ret=db_client.UserAdd(test_usr);
	PROCESS_RET(ret==0,3);

	printf("Disconnect: ");
	ret=db_client.Disconnect();
	PROCESS_RET(ret==0,4);

	printf("Connecting as %s: ",test_usr.data());
	ret=db_client.Connect(test_usr);
	PROCESS_RET(ret==0,5);

	printf("Set key='%s' value='%s': ",key1.data(),val1.data());
	ret=db_client.Set(key1,val1);
	PROCESS_RET(ret==0,6);

	printf("Get key='%s': ",key1.data());
	ret=db_client.Get(key1,readback);
	PROCESS_RET(ret==0,7);

	printf("Value comparing value='%s' readback='%s': ",val1.data(),readback.data());
	PROCESS_RET(ret==0,8);

	printf("Removing key='%s': ",key1.data());
	ret=db_client.Remove(key1);
	PROCESS_RET(ret==0,9);

	printf("Get removed key='%s': ",key1.data());
	ret=db_client.Get(key1,readback);
	PROCESS_RET(ret!=0,10);

	printf("User remove '%s': ",test_usr.data());
	ret=db_client.UserRemove(test_usr);
	PROCESS_RET(ret==0,11);

	printf("Disconnect: ");
	ret=db_client.Disconnect();
	PROCESS_RET(ret==0,12);

	printf("Connecting as removed user %s: ",test_usr.data());
	ret=db_client.Connect(test_usr);
	PROCESS_RET(ret!=0,13);

exit:
	if(code!=0){
		printf("UselessDBClient test error. Code = %d\n",code);
	}
	else{
		printf("UselessDBClient test succesfull\n");
	}

}


static void* Thread_EchoFunction(void* argc){
	int ret=0,i=0,pid=0;
	int thread_num=*((int*)argc);
	std::string request, response;
	request.assign("Echo TEST!!!");
	UselessDBClient client;
	pid=client.Init(thread_num);
	if(pid!=0){
		for(i=0;i<100;i++){
			ret=client.Echo(request,response);
			if(ret==0){
				printf("Thread %d: Echo #%d request:'%s' response:'%s'\n\n",thread_num,i,request.data(),response.data());
				if(request!=response){
					ret=1;
				}
			}
			if(ret!=0){
				printf("Thread %d: Echo error!\n",thread_num);
				break;
			}
		}
	}
}
