#include "server.h"
#include "util.h"

DServer::DServer()
{
	initMap();
}

::grpc::Status DServer::SendMessageClient(::grpc::ServerContext* context,::grpc::ServerReaderWriter<::ChatApp::SendMessageResp,::ChatApp::SendMessageReq>* stream)
{
	ChatApp::SendMessageReq messageRequest;
	ChatApp::SendMessageResp messageResponse;
	ChatApp::SendMessageReq messageRegister;

	// register client
	stream->Read(&messageRegister);
	std::string client = messageRegister.client();
	std::string partner = messageRegister.partnername();
	m_streamMap.insertItem(client, stream);


	while (stream->Read(&messageRequest))
	{
		std::string msg = messageRequest.sendmessagereq();
		std::string clientName = messageRequest.client();
		std::string partnerName = messageRequest.partnername();
	
		auto strm = m_streamMap.getValue(partnerName);
		messageResponse.set_sendmessageresp(msg);
		strm->Write(messageResponse);
	}

	return grpc::Status::OK;
}

void DServer::initMap()
{
	// default clients pair
	m_clientsInChat.pushPair(Names::Alice, Names::Bob);
	m_clientsInChat.pushPair(Names::Bob, Names::Alice);
}

std::string DServer::GetClientInfoFromContext(const grpc::ServerContext* context)
{
	std::string retVal;

	const auto& clientMetaData = context->client_metadata();

	auto it = clientMetaData.find("client-name");
	if (it != clientMetaData.end())
	{
		retVal = it->second.data();

	}

	return retVal;
}

::grpc::Status DServer::RegisterClient(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::ChatApp::RegisterResp, ::ChatApp::RegisterReq>* stream)
{

	return grpc::Status::OK;
}
::grpc::Status DServer::ChooseParticipant(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::ChatApp::ChooseParticipantResp, ::ChatApp::ChooseParticipantReq>* stream)
{
	return grpc::Status::OK;
}