/*
** EPITECH PROJECT, 2019
** anynet
** File description:
** server template
*/

#ifndef TSERVER_HPP_
#define TSERVER_HPP_

#include <vector>
#include <memory>
#include <type_traits>
#include <iomanip>
#include <boost/asio.hpp>
#include "TDelegatedConnection.hpp"
#include "TICallbackHandler.hpp"

template<typename ConnectionType>
class TServer : public TICallbackHandler<ConnectionType> {
	public:

		///
		///@brief Session type as defined by MessageType and meatadata type
		///
		///
		using Connection = ConnectionType;

		using SessionHandle = std::shared_ptr<Connection>;
		// using MakeSession = std::make_shared<Connection>;

		using io_context = boost::asio::io_context;
		using tcp = boost::asio::ip::tcp;

		///
		///@brief Construct a new TServer object
		///
		///@param port to listen on
		///
		TServer(int port, io_context &context):
			_context(context),
			_work(_context),
			_incoming(_context)
		{
			tcp::resolver	resolver(_context);
			tcp::endpoint	endpoint = *resolver.resolve(
				"127.0.0.1",
				std::to_string(port)
			);
			_incoming.open(endpoint.protocol());
			_incoming.set_option(tcp::acceptor::reuse_address(true));
			_incoming.bind(endpoint);
			_incoming.listen(boost::asio::socket_base::max_connections);
			std::cout << "Listening on: " << endpoint << std::endl;
			startAccept();
		}

		///
		///@brief Destroy the TServer object
		///
		///
		virtual ~TServer()
		{
			_context.stop();
		}

		///
		///@brief Handles a connection event on connection
		///
		///
		virtual void onConnect(Connection &connection) override {(void)connection;}

		///
		///@brief Handles a message event on connection
		///
		///@param message message event data
		///
		virtual void onMessage(Connection &connection, const typename Connection::Message &message) override
		{
			(void)connection;
			(void)message;
		}

		///
		///@brief Handles an error event on connection
		///
		///@param er  error event data
		///
		virtual void onError(Connection &connection, const boost::system::error_code &er) override
		{
			(void)connection;
			(void)er;
		}

		virtual void onDisconnect(Connection &connection) override {(void)connection;}

		///
		///@brief
		///
		///
		void	poll()
		{
			try {
				_context.poll();
				removeClosedConnections();
			} catch(const std::exception &e) {
				std::cerr << e.what() << "\r\n";
			}
		}

		void	run()
		{
			while(true) {
				poll();
			}
		}

	protected:
		std::vector<SessionHandle>	_connections;
	private:

		///
		///@brief start connection accepting task
		///
		///
		void	startAccept()
		{
			SessionHandle	newConnection;
			if constexpr(std::is_base_of_v<TDelegatedConnection<typename ConnectionType::Message>, Connection>)
				newConnection = _connections.emplace_back(std::make_shared<Connection>(_context, *this));
			else
				newConnection = _connections.emplace_back(std::make_shared<Connection>(_context));
			_incoming.async_accept(newConnection->getSocket(), [this, connection = newConnection](const boost::system::error_code &ec){
				connection->onAccept();
				startAccept();
			});
		}
		io_context					&_context;
		io_context::work			_work;
		tcp::acceptor				_incoming;

		void	removeClosedConnections()
		{
			_connections.erase(
				std::remove_if(_connections.begin(), _connections.end(), [](auto &connection){
					return !connection->isOpen();
				}),
				_connections.end()
			);

		}

};

template<typename T, bool isConnection>
struct ServerImpl {};

template<typename ConnectionType>
struct ServerImpl<ConnectionType, true> {
	using type = TServer<ConnectionType>;
};

template<typename MessageType>
struct ServerImpl<MessageType, false> {
	using type = TServer<TDelegatedConnection<MessageType>>;
};

template<typename T>
constexpr bool	isConnection = false;

template<typename MessageType>
constexpr bool	isConnection<TConnection<MessageType>> = true;

#endif /* !TSERVER_HPP_ */
