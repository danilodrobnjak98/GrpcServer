#include "client.h"

int main()
{
	grpc::ChannelArguments args;
	args.SetInt(GRPC_ARG_MAX_RECEIVE_MESSAGE_LENGTH, INT_MAX);

	DClient client(grpc::CreateCustomChannel("localhost:8080", grpc::InsecureChannelCredentials(), args));

	std::cout << "Welcome " << client.getClientName() << std::endl;

	client.sendMessage();

	return 0;
}