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
	ret=db_client.Init(0);
	if(ret==0){
		code=1;
		goto exit;
	}
	ret=db_client.Connect(root);
	if(ret!=0){
		code=2;
		goto exit;
	}
	ret=db_client.UserAdd(test_usr);
	if(ret!=0){
		code=3;
		goto exit;
	}
	ret=db_client.Disconnect();
	if(ret!=0){
		code=4;
		goto exit;
	}
	ret=db_client.Connect(test_usr);
	if(ret!=0){
		code=5;
		goto exit;
	}
	ret=db_client.Set(key1,val1);
	if(ret!=0){
		code=6;
		goto exit;
	}
	ret=db_client.Get(key1,readback);
	if(ret!=0){
		code=7;
		goto exit;
	}
	if(readback!=val1){
		printf("%s\n",readback.data());
		code=8;
		goto exit;
	}
	ret=db_client.Remove(key1);
	if(ret!=0){
		code=9;
		goto exit;
	}
	ret=db_client.Get(key1,readback);
	if(ret==0){
		code=10;
		goto exit;
	}
	ret=db_client.UserRemove(test_usr);
	if(ret!=0){
		code=11;
		goto exit;
	}
	ret=db_client.Disconnect();
	if(ret!=0){
		code=12;
		goto exit;
	}
	ret=db_client.Connect(test_usr);
	if(ret==0){
		code=13;
		goto exit;
	}

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
