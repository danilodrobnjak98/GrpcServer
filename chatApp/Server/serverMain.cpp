#include "server.h"

int main()
{
	std::string server_address("0.0.0.0:8080");
	DServer server;
	grpc::ServerBuilder builder;

	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&server);

	std::unique_ptr<grpc::Server> serv(builder.BuildAndStart());
	std::cout << "Server listening on : " << server_address << std::endl;

	serv->Wait();

	return 0;
}