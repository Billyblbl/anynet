/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** ExempleMessageType
*/

#include "ExempleMessageType.hpp"

//--------------Required to be considered a valid Message type-------------

void				ExempleMessageType::writeTo(std::vector<std::byte> &buffer) const
{
	TConverter	header(_data.size());
	buffer.insert(buffer.end(), std::begin(header.data), std::end(header.data));
	buffer.insert(buffer.end(), (std::byte *)_data.begin().base(), (std::byte *)_data.end().base());
}


bool				ExempleMessageType::hasMessage(std::vector<std::byte> &buffer)
{
	if (buffer.size() < sizeof(std::size_t))
		return false;
	const std::size_t	*header = reinterpret_cast<const std::size_t *>(buffer.data());
	return buffer.size() >= *header + sizeof(std::size_t);
}

ExempleMessageType	ExempleMessageType::extractFrom(std::vector<std::byte> &buffer)
{
	const std::size_t	*header = reinterpret_cast<const std::size_t *>(buffer.data());
	auto begin = buffer.begin() + sizeof(std::size_t);
	auto end = begin + *header;
	std::string	data((char *)begin.base(), (char *)end.base());
	return data;
}

//-------------------------------------------------------------------------

std::string			&ExempleMessageType::getData()
{
	return _data;
}

const std::string	&ExempleMessageType::getData() const
{
	return _data;
}
