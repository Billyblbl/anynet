/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** ExempleServer
*/

#include <iostream>
#include "ExempleServer.hpp"

void ExempleServer::onConnect(Connection &connection)
{
	std::cout << "Client connected" << std::endl;
}

void ExempleServer::onMessage(Connection &connection, const Connection::Message &message)
{
	std::cout << "Incoming message: " << message.getData() << std::endl;
}

void ExempleServer::onError(Connection &connection, const boost::system::error_code &ec)
{
	std::cout << "Network error: " << ec.message() << std::endl;
	connection.close();
}

void ExempleServer::onDisconnect(Connection &connection)
{
	std::cout << "Client disconnected" << std::endl;
}


