/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Guess.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 11:52:03 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/14 11:52:03 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUESS_HPP
#define GUESS_HPP

#include <stdexcept>
#include <string>

class Guess
{
	public:
		Guess(const std::string& word, const std::string& goal)
			throw (std::length_error);

		Guess(const Guess& instance);

		~Guess();

		const std::string&	get_word() const;	//	Return the guessed word.

		bool	is_valid(size_t pos) const			//	Returns true if the letter at pos position in word is the same than the letter in goal at the same position.
					throw (std::range_error);
		bool	is_misplaced(size_t pos) const		//	Returns true if the letter at pos position in word can be found somewhere in goal (and if the same letter in goal has not already been flagged).
					throw (std::range_error);

	private:
		const std::string						_word;
		const std::string&						_goal;
		std::vector<bool>						_misplaced;

		Guess();
		Guess&	operator=(const Guess& rhs);
};

#endif
