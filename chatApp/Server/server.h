#pragma once

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"
#include "dataStructures.h"
#include "clientsCollection.h"


class DServer : public ChatApp::ChatApplication::Service
{
public:
	DServer();

    // GRPC methods
    ::grpc::Status RegisterClient(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::ChatApp::RegisterResp, ::ChatApp::RegisterReq>* stream);
    ::grpc::Status ChooseParticipant(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::ChatApp::ChooseParticipantResp, ::ChatApp::ChooseParticipantReq>* stream);
    ::grpc::Status SendMessageClient(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::ChatApp::SendMessageResp, ::ChatApp::SendMessageReq>* stream);

private:

    void initMap();
    std::string GetClientInfoFromContext(const grpc::ServerContext* context);

private:

    typedef grpc::ServerReaderWriter< ::ChatApp::SendMessageResp, ::ChatApp::SendMessageReq>* STREAM;
    DataStructures::DMap<STREAM> m_streamMap;
    CCollection m_clientsInChat;
};