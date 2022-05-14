/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Guess.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 16:25:11 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/14 17:07:45 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <algorithm>
#include <vector>

#include "Guess.hpp"

Guess::Guess(const std::string& word, const std::string& goal) throw (std::length_error)
	: _word(word)
	, _goal(goal)
	, _misplaced(_word.size())
{
	if (word.size() != goal.size())
		throw std::length_error("Guess::Guess (word and goal sizes must be the same)");

	std::vector<char>	unfound_chars;

	unfound_chars.reserve(_word.size());

	for (size_t i=0; i < _goal.size(); ++i)
		if (_word[i] != _goal[i])
			unfound_chars.push_back(_goal[i]);

	for (size_t i=0; i < _goal.size(); ++i)
	{
		if (_word[i] == _goal[i])
			continue;
		auto it = std::find(unfound_chars.begin(), unfound_chars.end(), _word[i]);
		if (it != unfound_chars.end())
		{
			unfound_chars.erase(it);
			_misplaced[i] = true;
		}
	}

}

const std::string&	Guess::get_word() const
{
	return _word;
}

bool	Guess::is_valid(size_t pos) const throw (std::range_error)
{
	if (pos < 0 || pos >= _word.size())
		throw std::range_error("Guess::is_valid(int pos) pos is outside the string.");
	return (_word[pos] == _goal[pos]);
}

bool	Guess::is_misplaced(size_t pos) const throw (std::range_error)
{
	if (pos < 0 || pos >= _word.size())
		throw std::range_error("Guess::is_valid(int pos) pos is outside the string.");
	return (_misplaced[pos]);
}
