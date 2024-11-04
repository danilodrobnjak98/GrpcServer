#include "client.h"
#include "util.h"
#include <iomanip>

#define DEFAULT_BUFLEN 512

DClient::DClient(std::shared_ptr <grpc::Channel> channel)
	:
	m_stub(ChatApp::ChatApplication::NewStub(channel)),
	m_clientName(Names::AliceName),
	m_partnerName(Names::BobName)
{
	setWorking(false);
}

void DClient::sendMessage()
{
	grpc::ClientContext context;
	context.AddMetadata("client-name", m_clientName);
	DSTREAM_PTR stream(m_stub->SendMessageClient(&context));

	setWorking(true);

	// Register client
	ChatApp::SendMessageReq msgReq;
	msgReq.set_partnername(m_partnerName);
	msgReq.set_client(m_clientName);
	msgReq.set_sendmessagereq("");

	stream->Write(msgReq);

	std::thread writerThread(&DClient::write, this, stream);
	std::thread readerThread(&DClient::read, this, stream);

	grpc::Status status = stream->Finish();

	writerThread.join();
	readerThread.join();

	if (status.ok())
	{
		std::cout << "RPC stream completed successfully!" << std::endl;
	}
	else
	{
		std::cout << "RPC failed " << status.error_message() << std::endl;
	}
}

void DClient::write(DSTREAM_PTR stream)
{
	while (isWorking())
	{
		ChatApp::SendMessageReq messageRequest;
		char msg[DEFAULT_BUFLEN];
		int buflen = DEFAULT_BUFLEN;

		fgets(msg, buflen, stdin);
		std::string strMsg(msg);

		if (strMsg.compare(ChatUtil::FinishChat) == 0 || strMsg.compare(ChatUtil::FinishChatLower) == 0)
		{
			messageRequest.set_sendmessagereq(m_clientName + " left chat.");
			messageRequest.set_client(m_clientName);
			messageRequest.set_partnername(m_partnerName);

			stream->Write(messageRequest);
			stream->WritesDone();
			setWorking(false);

			system("pause");
			exit(0);
		}
		else
		{
			messageRequest.set_sendmessagereq(msg);
			messageRequest.set_client(m_clientName);
			messageRequest.set_partnername(m_partnerName);
			
			stream->Write(messageRequest);
		}
	}
}

void DClient::read(DSTREAM_PTR stream)
{
	while (isWorking())
	{
		ChatApp::SendMessageResp messageResponse;
		while (stream->Read(&messageResponse))
		{
			setGreenColor();
			std::cout << m_partnerName << " : ";
			std::cout << messageResponse.sendmessageresp();
			resetColor();
		}
	}
}

void DClient::setWorking(bool cond)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_isWorking = true;

}

bool DClient::isWorking()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_isWorking;
}

std::string DClient::getClientName()
{
	return m_clientName;
}

void DClient::setGreenColor()
{
	std::cout << "\033[1;32m"; // ANSI escape code for green text
}

void DClient::resetColor()
{
	std::cout << "\033[0m"; // ANSI escape code for resetting text color
}