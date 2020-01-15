/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** ExempleServer
*/

#ifndef EXEMPLESERVER_HPP_
#define EXEMPLESERVER_HPP_

#include "TCPNet.hpp"
#include "ExempleMessageType.hpp"

class ExempleServer : public TCPNet::Server<ExempleMessageType> {
	public:

		using Base = TCPNet::Server<ExempleMessageType>;

		inline ExempleServer(io_context	&context):
			Base(8081, context)
		{}

		virtual void onConnect(Connection &connection) override;
		virtual void onMessage(Connection &connection, const Connection::Message &message) override;
		virtual void onError(Connection &connection, const boost::system::error_code &ec) override;
		virtual void onDisconnect(Connection &connection) override;

	protected:
	private:
};

#endif /* !EXEMPLESERVER_HPP_ */
