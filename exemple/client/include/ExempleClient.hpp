/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** ExempleClient
*/

#ifndef EXEMPLECLIENT_HPP_
#define EXEMPLECLIENT_HPP_

#include "TCPNet.hpp"
#include "ExempleMessageType.hpp"

class ExempleClient : public TCPNet::Client<ExempleMessageType> {
	public:

		using Base = TCPNet::Client<ExempleMessageType>;

		inline ExempleClient(io_context &context):
			Base(context)
		{}

		virtual void onConnect() override;
		virtual void onMessage(const Message &message) override;
		virtual void onError(const boost::system::error_code &er) override;
		virtual void onDisconnect() override;

	protected:
	private:
};

#endif /* !EXEMPLECLIENT_HPP_ */
