/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** main
*/

#include "ExempleServer.hpp"

int	main()
{
	ExempleServer::io_context	context;
	ExempleServer				server(context);
	server.run();
	return 0;
}