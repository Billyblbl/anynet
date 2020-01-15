/*
** EPITECH PROJECT, 2019
** anynet
** File description:
** connection template
*/

#ifndef TCONNECTION_HPP_
#define TCONNECTION_HPP_

#include <array>
#include <vector>
#include <boost/asio.hpp>

///
///@brief Generic Connection object
///
///@tparam MessageType Defining data format
/// @warning MessageType must define the void writeTo(std::vector<std:byte>) method, bool hasMessage(std::vector<std::byte>), and MessageType extract(std::vector<std::byte>)
///
///@tparam BufferSize buffer size used on read operations
///
template<typename MessageType, std::size_t BufferSize = 1024>
class TConnection {
	public:

		using tcp = boost::asio::ip::tcp;
		using io_context = boost::asio::io_context;
		using Message = MessageType;

		///
		///@brief Read Buffer
		///
		///
		struct RBuffer {
			std::array<std::byte, BufferSize>	input;
			std::vector<std::byte>				data;
		};

		///
		///@brief Write Buffer
		///
		///
		using WBuffer = std::vector<std::byte>;

		///
		///@brief Construct a new TConnection object
		///
		///@param context asio io_context associated with the connection
		///
		TConnection(io_context &context):
			_socket(context),
			_open(false)
		{}

		TConnection(TConnection &&) = delete;

		///
		///@brief Destroy the TConnection object
		///
		///
		virtual ~TConnection()
		{
			bool	wasOpen = _open;
			_open = false;
			if (wasOpen) {
				close();
			}
		}

		TConnection		&operator=(TConnection &&rhs) = delete;

		///
		///@brief Called when an acceptor links this connection to an incoming device tcp connection
		///
		///
		void			onAccept()
		{
			_open = true;
			onConnect();
			startReceive();
			// startSend();
		}

		///
		///@brief Equality operator
		///
		/// based on memory location as connection are supposed to be immovable unique object
		///
		bool			operator==(const TConnection &rhs) const
		{
			return this == &rhs;
		}

		///
		///@brief Virtual handler
		///
		/// User implementation point
		///
		virtual void	onConnect() {};

		///
		///@brief Virtual handler
		///
		/// User implementation point
		///
		virtual void	onMessage(const Message &message) {(void)message;}

		///
		///@brief Virtual handler
		///
		/// User implementation point
		///
		virtual void	onError(const boost::system::error_code &er)
		{
			std::cerr << er << "\r\n";
		}

		virtual void	onDisconnect() {}

		///
		///@brief Writes message to the write buffer to enqueue for data transmission
		///
		///
		void			send(const Message &message)
		{
			message.writeTo(_wBuffer);
		}

		///
		///@brief Get the Socket object
		///
		///
		tcp::socket			&getSocket()
		{
			return _socket;
		}

		///
		///@brief Get the Socket object
		///
		///
		const tcp::socket	&getSocket() const
		{
			return _socket;
		}

		///
		///@brief Close the connection
		///
		///
		void			close()
		{
			_open = false;
			onDisconnect();
			boost::system::error_code ec;
			_socket.shutdown(tcp::socket::shutdown_both, ec);
			if (ec)
				std::cerr << "E : " << ec << "\r\n";
			_socket.close(ec);
			if (ec)
				std::cerr << "E : " << ec << "\r\n";
		}

		///
		///@brief Connection status Checker
		///
		///
		bool			isOpen() const
		{
			return _open;
		}

		///
		///@brief Connect to a server as clienty
		///
		///@param address IP address of the server endpoint
		///@param port Target port of the server
		///
		void			connect(const std::string &address, int port)
		{
			_socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(address), port));
			onConnect();
			startReceive();
			// startSend();
		}

	protected:
	private:

		///
		///@brief Receiving work
		///
		/// fills Reader buffer and call onMessage when reader buffer
		/// contains a message as defined by MessageType
		///
		void	startReceive()
		{
			auto handler = [this](const boost::system::error_code &er, size_t bytes){
				try {
					if (er) {
						onError(er);
						_open = false;
						return;
					}
					_rBuffer.data.insert(_rBuffer.data.end(), _rBuffer.input.begin(), _rBuffer.input.begin() + bytes);
					if (Message::hasMessage(_rBuffer.data))
						onMessage(Message::extract(_rBuffer.data));
					startReceive();
				} catch(const std::exception& e) {
					std::cerr << __func__ << ' ' << e.what() << "\r\n";
				}
			};
			_socket.async_receive(boost::asio::buffer(_rBuffer.input), handler);
		}

		///
		///@brief Sender work
		///
		/// Sends data from Writer buffer
		///
		void	startSend()
		{
			auto handler = [this](const boost::system::error_code &er, size_t bytes){
				try {
					if (er) {
						onError(er);
						_open = false;
						return;
					}
					_wBuffer.erase(_wBuffer.begin(), _wBuffer.begin() + bytes);
					if (!_wBuffer.empty())
						startSend();
				} catch(const std::exception& e) {
					std::cerr << __func__ << ' ' << e.what() << "\r\n";
				}
			};
			_socket.async_send(boost::asio::buffer(_wBuffer), handler);
		}

		tcp::socket	_socket;
		bool		_open;
		RBuffer		_rBuffer;
		WBuffer		_wBuffer;
};

#endif /* !TCONNECTION_HPP_ */
