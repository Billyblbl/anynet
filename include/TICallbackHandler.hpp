/*
** EPITECH PROJECT, 2019
** anynet
** File description:
** callback handler interface template
*/

#ifndef TICALLBACKHANDLER_HPP_
#define TICALLBACKHANDLER_HPP_

#include <boost/asio.hpp>
#include "TConnection.hpp"

///
///@brief Generic Callback handler interface template
///
/// Defines a common interface for an object capable
/// of handling connections of ConnectionType
///
///@tparam ConnectionType managed by the callback handler
///
template<typename ConnectionType>
class TICallbackHandler {
	public:

		///
		///@brief The type of connection handled by the callback handler
		///
		///
		using Connection = ConnectionType;

		///
		///@brief Destroy the TICallbackHandler object
		///
		///
		virtual ~TICallbackHandler() = default;

		///
		///@brief Handles a connection event on connection
		///
		///
		virtual void onConnect(Connection &connection) = 0;

		///
		///@brief Handles a message event on connection
		///
		///@param message message event data
		///
		virtual void onMessage(Connection &connection, const typename ConnectionType &message) = 0;

		///
		///@brief Handles an error event on connection
		///
		///@param er error event data
		///
		virtual void onError(Connection &connection, const boost::system::error_code &er) = 0;

		virtual void onDisconnect(Connection &connection) = 0;

	protected:
	private:
};

#endif /* !ICALLBACKHANDLER_HPP_ */
