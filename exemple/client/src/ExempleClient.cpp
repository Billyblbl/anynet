/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** ExempleClient
*/

#include <iostream>
#include "ExempleClient.hpp"

void ExempleClient::onConnect()
{
	std::cout << "Connected" << std::endl;
}

void ExempleClient::onMessage(const Message &message)
{
	std::cout << "Received message: " << message.getData() << std::endl;
}

void ExempleClient::onError(const boost::system::error_code &er)
{
	std::cout << "Network error: " << er.message() << std::endl;
}

void ExempleClient::onDisconnect()
{
	std::cout << "Disconnected" << std::endl;
}


