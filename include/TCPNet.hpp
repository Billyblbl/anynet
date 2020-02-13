/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** Net
*/

#ifndef NET_HPP_
#define NET_HPP_

#include "TConnection.hpp"
#include "TDelegatedConnection.hpp"
#include "TICallbackHandler.hpp"
#include "TServer.hpp"

namespace TCPNet
{
	template<typename MessageType>
	using Client = TConnection<MessageType>;

	template<typename MessageType, typename CRTPToken = void>
	using Session = TDelegatedConnection<MessageType, CRTPToken>;

	template<typename T>
	using Server = typename ServerImpl<T, isConnection_v<T>>::type;

} // namespace TCPNet


#endif /* !NET_HPP_ */
