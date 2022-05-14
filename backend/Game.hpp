/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 11:35:56 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/14 11:52:19 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <unordered_set>
#include <vector>

#include "Guess.hpp"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
	class GameEndedException : public std::exception { virtual const char* what(); };
	class FileImportFailedException : public std::exception { virtual const char* what(); };

	public:
		static const int	_max_guesses = 5;

		Game();

		void				import_new_dictionary(const std::string& filepath) throw (FileImportFailedException);

		bool				is_word_valid(const std::string& word);

		const Guess&		new_guess(const std::string& word) throw (GameEndedException);

		int					guesses();
		int					guesses_left();

		const Guess&		get_guess(int pos) throw (std::out_of_range);

	private:
		int									_turn;
		std::string							_goal;
		std::vector<Guess>					_guesses;
		std::unordered_set<std::string>		_dictionary;
};

#endif
