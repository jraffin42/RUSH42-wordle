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
#include <stdexcept>
#include <random>
#include <unordered_set>
#include <vector>
#include <string>

#include "Guess.hpp"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
	class GameHasEndedException : public std::exception { virtual const char* what(); };
	class GameRunningException : public std::exception { virtual const char* what(); };
	class FileImportFailedException : public std::exception { virtual const char* what(); };
	class InvalidWordException : public std::exception { virtual const char* what(); };

	public:
		static const size_t	default_word_length = 5;
		static const size_t	default_max_guesses = 5;

		Game();																		//	Creates a new game object, not yet running with default word length and default maximum guesses.
		Game(int word_length, int max_guesses);										//	Creates a new game object, not yet running.
		Game(const std::string& dictionarypath)										//	Creates a new game object with default word length and default maximum guesses from a given dictionary file, then starts it.
			throw (FileImportFailedException);
		Game(int word_length, int max_guesses, const std::string& dictionarypath)	//	Creates a new game object from a given dictionary file, then starts it.
			throw (FileImportFailedException);

		void				clear_dictionary()										//	Clears the whole dictionary.
								throw (GameRunningException);
		void				add_word_to_dictionary(const std::string& word)			//	Adds a single word to the dictionary.
								throw (GameRunningException, InvalidWordException);
		void				import_dictionary_file(const std::string& filepath)		//	Imports a whitespace separated word list file into the dictionary.
								throw (GameRunningException, FileImportFailedException);

		void				stop_game();	//	Clears game state.
		void				new_game();		//	Clears game state and starts a new game.

		bool				is_running();	//	Is the game running ?
		bool				is_finished();	//	Is the game finished ?
		bool				is_won();		//	Is the game won ?
		int					guesses();		//	Number of valid guesses submitted so far.

		int					word_length();	//	Get the game word length.
		int					max_guesses();	//	Maximum Guesses before loosing the game.

		bool				is_word_valid(const std::string& word);		//	Is this word part of the dictionary ?
		const Guess&		guess_word(const std::string& word)			//	Takes a turn guessing a word.
								throw (InvalidWordException, GameHasEndedException);

		const std::string&	get_goal();			//	Returns the word to find.
		const Guess&		get_guess(int pos)	//	Returns a reference to the guess object at (zero based) pos position ( pos must be < guesses() ).
								throw (std::range_error);

		static std::string					get_random_word_from_dictionary(const std::unordered_set<std::string>& dictionary);

	private:
		const size_t						_word_length;
		const size_t						_max_guesses;
		int									_turn;
		bool								_won;
		std::random_device					_rand;
		std::string							_goal;
		std::vector<Guess>					_guesses;
		std::unordered_set<std::string>		_dictionary;

		Game(const Game& instance);
		Game&	operator=(const Game& rhs);
		~Game();
};

#endif
