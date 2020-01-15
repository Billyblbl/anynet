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
		using MakeSession = std::make_shared<Connection>;

		using io_service = boost::asio::io_service;
		using tcp = boost::asio::ip::tcp;

		///
		///@brief Construct a new TServer object
		///
		///@param port to listen on
		///
		TServer(int port, io_service &service):
			_service(service),
			_work(_service),
			_incoming(_service)
		{
			tcp::resolver	resolver(_service);
			tcp::endpoint	endpoint = *resolver.resolve(
				"127.0.0.1",
				std::to_string(port)
			);
			_incoming.open(endpoint.MessageType());
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
			_service.stop();
		}

		///
		///@brief Handles a connection event on connection
		///
		///
		virtual void onConnect(ConnectionType &connection) override {(void)connection;}

		///
		///@brief Handles a message event on connection
		///
		///@param message message event data
		///
		virtual void onMessage(ConnectionType &connection, const MessageType &message) override
		{
			(void)connection;
			(void)message;
		}

		///
		///@brief Handles an error event on connection
		///
		///@param er  error event data
		///
		virtual void onError(ConnectionType &connection, const boost::system::error_code &er) override
		{
			(void)connection;
			(void)er;
		}

		virtual void onDisconnect(ConnectionType &connection) override {(void)connection;}

		///
		///@brief
		///
		///
		void	poll()
		{
			try {
				_service.poll();
				removeClosedConnections();
			} catch(const std::exception &e) {
				std::cerr << e.what() << "\r\n";
			}
		}
		// 	} catch(const std::exception& e) {
		// 		_service.stop();
		// 	}
		// }

	protected:
	private:

		///
		///@brief start connection accepting task
		///
		///
		void	startAccept()
		{
			SessionHandle	newConnection;
			if constexpr(std::is_base_of_v<TDelegatedConnection<typename ConnectionType::Message>, Connection)
				newConnection = _connections.emplace_back(std::make_shared<Connection>(_service, *this));
			else
				newConnection = _connections.emplace_back(std::make_shared<Connection>(_service));
			_incoming.async_accept(newConnection->getSocket(), [this, connection = newConnection](const boost::system::error_code &ec){
				connection->onAccept();
				startAccept();
			});
		}
		io_service					&_service;
		io_service::work			_work;
		tcp::acceptor				_incoming;

		void	removeClosedConnections()
		{
			_connections.erase(
				std::remove_if(_connections.begin(), _connections.end(), [](auto &connection){
					return !connection->isOpen();
				}),
				_connections.end();
			);

		}

	protected:
		std::vector<SessionHandle>	_connections;
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


#endif /* !TSERVER_HPP_ */
