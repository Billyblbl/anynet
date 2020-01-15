/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** ExempleMessageType
*/

#ifndef EXEMPLEMESSAGETYPE_HPP_
#define EXEMPLEMESSAGETYPE_HPP_

#include <string>
#include <vector>

//for std::byte
#include <cstddef>

//for std::transform
#include <algorithm>

#include "TConverter.hpp"

class ExempleMessageType {
	public:

		inline ExempleMessageType(const std::string &data): _data(data) {}

		//--------------Required to be considered a valid Message type-------------

		void						writeTo(std::vector<std::byte> &buffer) const;

		static bool					hasMessage(std::vector<std::byte> &buffer);

		static ExempleMessageType	extractFrom(std::vector<std::byte> &buffer);

		//-------------------------------------------------------------------------


		std::string			&getData();
		const std::string	&getData() const;

	protected:
	private:

		std::string	_data;
};

#endif /* !EXEMPLEMESSAGETYPE_HPP_ */
