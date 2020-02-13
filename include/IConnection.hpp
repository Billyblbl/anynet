/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** IConnection
*/

#ifndef ICONNECTION_HPP_
#define ICONNECTION_HPP_

#include <type_traits>

class IConnection {
	public:

		virtual ~IConnection() = default;

	protected:
	private:
};

template<typename T>
struct isConnection {constexpr static bool value = std::is_base_of_v<IConnection, T>;};

template<typename T>
constexpr bool	isConnection_v = isConnection<T>::value;

#endif /* !ICONNECTION_HPP_ */
