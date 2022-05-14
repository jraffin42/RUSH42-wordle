/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 17:07:56 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/14 17:35:40 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

virtual const char*	Game::GameHasEndedException::what() { return ""; }
virtual const char*	Game::GameRunningException::what() { return ""; }
virtual const char*	Game::FileImportFailedException::what() { return ""; }
virtual const char*	Game::InvalidWordException::what() { return ""; }

Game::Game() : Game(default_word_length, default_word_length) {}

Game::Game(int word_length, int max_guesses)
	: _word_length(word_length)
	, _max_guesses(max_guesses)
	, _turn(-1)
	, _won(false)
{
	_guesses.reserve(_max_guesses);

}

Game::Game(const std::string& dictionarypath)										//	Creates a new game object with a word length of 5 and 5 maximum guesses from a given dictionary file, then starts it.
	throw (FileImportFailedException);
Game::Game(int word_length, int max_guesses, const std::string& dictionarypath)	//	Creates a new game object from a given dictionary file, then starts it.
	throw (FileImportFailedException);

void				Game::clear_dictionary()										//	Clears the whole dictionary. (Game must not be running).
						throw (GameRunningException);
void				Game::add_word_to_dictionary(const std::string& word)			//	Adds a single word to the dictionary. (Game must not be running).
						throw (GameRunningException, InvalidWordException);
void				Game::import_dictionary_file(const std::string& filepath)		//	Imports a whitespace separated word list file into the dictionary.
						throw (GameRunningException, FileImportFailedException);

void				Game::stop_game()
{
	_guesses.clear();
	_turn = -1;
}

void				Game::new_game()
{
	_guesses.clear();
	_turn = 0;
	_won = false;
	_goal = get_random_word_from_dictionary(_dictionary);
}

bool				Game::is_running();		//	Is the game running ?
bool				Game::is_finished();	//	Is the game finished ?
bool				Game::is_won();			//	Is the game won ?
int					Game::guesses();		//	Number of valid guesses submitted so far.

int					Game::word_length();	//	Get the game word length.
int					Game::max_guesses();	//	Maximum Guesses before loosing the game.

bool				Game::is_word_valid(const std::string& word);		//	Is this word part of the dictionary ?
const Guess&		Game::guess_word(const std::string& word)			//	Takes a turn guessing a word.
						throw (InvalidWordException, GameHasEndedException);

const std::string&	Game::get_goal();			//	Returns the word to find.
const Guess&		Game::get_guess(int pos)	//	Returns a reference to the guess object at (zero based) pos position ( pos must be < guesses() ).
						throw (std::range_error);

static std::string					Game::get_random_word_from_dictionary(const std::unordered_set<std::string>& dictionary);
