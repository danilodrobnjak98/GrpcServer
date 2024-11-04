#pragma once

#include <grpcpp/grpcpp.h>
#include "../Server/chat.grpc.pb.h"

class DClient
{
public:
	DClient(std::shared_ptr <grpc::Channel> channel);
	void sendMessage();
	std::string getClientName();
	
private:
	typedef grpc::ClientReaderWriter<ChatApp::SendMessageReq, ChatApp::SendMessageResp> DSTREAM;
	typedef std::shared_ptr<DSTREAM> DSTREAM_PTR;
	void write(DSTREAM_PTR stream);
	void read(DSTREAM_PTR stream);

	void setWorking(bool cond);
	bool isWorking();

	void setGreenColor();
	void resetColor();

private:
	std::mutex m_mutex;
	std::unique_ptr<ChatApp::ChatApplication::Stub> m_stub;
	bool m_isWorking;

	std::string m_clientName;
	std::string m_partnerName;
};
