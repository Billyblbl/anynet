/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** client
*/

#include <functional>
#include "ExempleClient.hpp"

char						buffer[10000] = {0};

// void	startReadStdin(ExempleClient::io_context &context, std::function<void()> &onInput)
// {
// 	using asioStream = boost::asio::posix::stream_descriptor;
// 	static asioStream					input(context, stdin);
// 	input.async_read_some(boost::asio::buffer(buffer), [&](const auto &, auto){
// 		onInput();
// 		startReadStdin(context, onInput);
// 	});
// }

int	main(int, char *av[])
{
	ExempleClient::io_context	context;
	ExempleClient				client(context);
	// std::function				onInput = [&client](){
	// 	client.send(ExempleMessageType(buffer));
	// };
	// startReadStdin(context, onInput);
	client.connect("127.0.0.1", 8081);
	client.send(ExempleMessageType(av[1]));
	context.run();
	return 0;
}