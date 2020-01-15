/*
** EPITECH PROJECT, 2020
** anynet
** File description:
** TConverter
*/

#ifndef TCONVERTER_HPP_
#define TCONVERTER_HPP_

#include <cstddef>

template<typename T>
union TConverter {
	TConverter(const T &obj): object(obj) {}

	T			object;
	std::byte	data[sizeof(T)];
};

#endif /* !TCONVERTER_HPP_ */
