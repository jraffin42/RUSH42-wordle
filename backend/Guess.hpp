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

#include <string>
#include <unordered_set>

class Guess
{
	public:
		Guess();
		Guess(const Guess& instance);
		~Guess();
		Guess&	operator=(const Guess& rhs);

		Guess(const std::string& word, const std::string& goal);

		const std::string&	get_word() const;

		bool	is_valid();
		bool	is_valid_letter(int pos);
		bool	is_letter_in_word(int pos);

	private:
		std::string								_word;
		const std::string&						_goal;
};

#endif
