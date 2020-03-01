/*
** EPITECH PROJECT, 2019
** anynet
** File description:
** server side connection type
*/

#ifndef TDELEGATEDCONNECTION_HPP_
#define TDELEGATEDCONNECTION_HPP_

#include <boost/asio.hpp>
#include "TConnection.hpp"
#include "TICallbackHandler.hpp"

///
///@brief Delegated Connection template
///
/// Defines a connection type from MessageType which delegates
/// protocol callbacks handling to a Callback handler of the
/// same MessageType type
///
///@tparam MessageType type used to define the nature of the connection and its events
///
template<typename MessageType, typename CRTPToken = void>
class TDelegatedConnection : public TConnection<MessageType> {
	public:

		using SelfType = std::conditional_t<
			std::is_same_v<CRTPToken, void>,
			TDelegatedConnection,
			CRTPToken
		>;

		///
		///@brief Callback handler interface type as defined by MessageType
		///
		///
		using HandlerType = TICallbackHandler<SelfType>;


		///
		///@brief Construct a new TDelegatedConnection object
		///
		///@param context asio io_context associated with the connection
		///@param handler Callback handler object
		///
		TDelegatedConnection(boost::asio::io_context &context, HandlerType &handler):
			TConnection<MessageType>(context),
			_handler(&handler)
		{}

		///
		///@brief Move constructor
		///
		///
		TDelegatedConnection(TDelegatedConnection &&) = default;

		///
		///@brief Destroy the TDelegatedConnection object
		///
		///
		virtual ~TDelegatedConnection() = default;

		///
		///@brief Move assignement
		///
		///@param rhs moving session object
		///
		TDelegatedConnection	&operator=(TDelegatedConnection &&rhs) = default;

		///
		///@brief Set the Callback Handler object
		///
		///
		void		setCallbackHandler(HandlerType &handler)
		{
			_handler = &handler;
		}

		///
		///@brief Get the Callback Handler object
		///
		///
		HandlerType	&getCallbackHandler() const
		{
			return *_handler;
		}

		///
		///@brief Delegates message events management to handler
		///
		///
		void	onMessage(const MessageType &message) final
		{
			// //std::cerr << std::string(__func__) + " shared locking\n" << std::endl;
			std::shared_lock	lock(*_handler);
			// //std::cerr << std::string(__func__) + " shared locked\n" << std::endl;
			_handler->onMessage((SelfType &)*this, message);
			// //std::cerr << std::string(__func__) + " shared unlocking (no idea when unlocked)\n" << std::endl;
		}

		///
		///@brief Delegates connection event management to handler
		///
		///
		void	onConnect() final
		{
			// //std::cerr << std::string(__func__) + " shared locking\n" << std::endl;
			std::shared_lock	lock(*_handler);
			// //std::cerr << std::string(__func__) + " shared locked\n" << std::endl;
			_handler->onConnect((SelfType &)*this);
			// //std::cerr << std::string(__func__) + " shared unlocking (no idea when unlocked)\n" << std::endl;
		}

		///
		///@brief Delegates connection event management to handler
		///
		///
		void	onError(const boost::system::error_code &er) final
		{
			//std::cerr << std::string(__func__) + " shared locking\n" << std::endl;
			std::shared_lock	lock(*_handler);
			//std::cerr << std::string(__func__) + " shared locked\n" << std::endl;
			_handler->onError((SelfType &)*this, er);
			//std::cerr << std::string(__func__) + " shared unlocking (no idea when unlocked)\n" << std::endl;
		}

		void	onDisconnect() final
		{
			//std::cerr << std::string(__func__) + " shared locking\n" << std::endl;
			std::shared_lock	lock(*_handler);
			//std::cerr << std::string(__func__) + " shared locked\n" << std::endl;
			_handler->onDisconnect((SelfType &)*this);
			//std::cerr << std::string(__func__) + " shared unlocking (no idea when unlocked)\n" << std::endl;
		}

	protected:
	private:
		HandlerType	*_handler;
};

#endif /* !Session_HPP_ */
