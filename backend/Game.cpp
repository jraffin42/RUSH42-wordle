/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 17:07:56 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/15 13:48:57 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <string>
#include <locale>

#include "Game.hpp"

const char*	Game::GameNotRunningException::what() const throw() { return "Game::GameNotRunningException Game should have been running."; }
const char*	Game::GameRunningException::what() const throw() { return "Game::GameRunningException Game should have not been running."; }
const char*	Game::FileImportFailedException::what() const throw() { return "Game::FileImportFailedException Error reading file for words."; }
const char*	Game::EmptyDictionaryException::what() const throw() { return "Game::EmptyDictionaryException Dictionary must not be empty"; }
const char*	Game::InvalidWordException::what() const throw() { return "Game::InvalidWordException This word is absent from dictionary"; }

Game::Game() : Game(default_word_length, default_word_length) {}

Game::Game(size_t word_length, size_t max_guesses)
	: _word_length(word_length)
	, _max_guesses(max_guesses)
	, _started(false)
	, _won(false)
	, _goal("")
	, _guesses()
	, _dictionary()
	, _randomPickVector()
	, _randomDevice()
	, _randomGenerator(_randomDevice())
{
	_guesses.reserve(_max_guesses);
	_dictionary.reserve(1024);
	_randomPickVector.reserve(1024);
}

Game::~Game() {}

void				Game::clear_dictionary() throw (GameRunningException)
{
	if (is_running())
		throw GameRunningException();
	_dictionary.clear();
	_randomPickVector.clear();
}

void				Game::add_word_to_dictionary(const std::string& word) throw (GameRunningException, std::length_error)
{
	if (is_running())
		throw GameRunningException();
	if (word.size() != _word_length)
		throw std::length_error("Game::add_word_to_dictionary(const std::string& word) word must be the size of the predefined world_length().");
	std::string uppercaseCopy(word);
	if (_isalpha_and_uppercase_transform(uppercaseCopy))
		_add_word_to_dictionary_internal(uppercaseCopy);
}

void				Game::import_dictionary_file(const std::string& filepath) throw (GameRunningException, FileImportFailedException)
{
	if (is_running())
		throw GameRunningException();
	std::string word;
	std::ifstream file(filepath.c_str());
	while (file >> word)
	{
		if (word.size() == _word_length && _isalpha_and_uppercase_transform(word))
			_add_word_to_dictionary_internal(word);
	}
	if (!file.eof())
	{
		throw FileImportFailedException();
	}
}

size_t				Game::dictionary_size()
{
	return _dictionary.size();
}

void				Game::stop_game()
{
	_guesses.clear();
	_started = false;
}

void				Game::start_game() throw (EmptyDictionaryException)
{
	_goal = get_random_word();
	_guesses.clear();
	_started = true;
	_won = false;
}

bool				Game::is_running()
{
	return (_started && !is_finished());
}

bool				Game::is_finished()
{
	return (_won || _guesses.size() == _max_guesses);
}

bool				Game::is_won()
{
	return _won;
}

size_t				Game::guesses()
{
	return _guesses.size();
}

size_t				Game::word_length()
{
	return _word_length;
}

size_t				Game::max_guesses()
{
	return _max_guesses;
}

bool				Game::is_word_alpha(const std::string& word)
{
	return std::all_of(word.begin(), word.end(), [] (char c) { return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'); });
}

bool				Game::is_word_valid(const std::string& word)
{
	if (word.size() != _word_length)
		return false;
	std::string	cpy(word);
	if (!_isalpha_and_uppercase_transform(cpy))
		return false;
	auto it = _dictionary.find(cpy);
	return (it != _dictionary.end());
}

const Guess&		Game::guess_word(const std::string& word) throw (InvalidWordException, GameNotRunningException, std::length_error)
{
	if (!is_running())
		throw GameNotRunningException();
	if (word.size() != _word_length)
		throw InvalidWordException();
	std::string	cpy(word);
	if (!_isalpha_and_uppercase_transform(cpy))
		throw InvalidWordException();
	auto it = _dictionary.find(cpy);
	if (it == _dictionary.end())
		throw InvalidWordException();
	_guesses.push_back(Guess(cpy, _goal));
	if (cpy == _goal)
		_won = true;
	return _guesses.back();
}

const std::string&	Game::get_goal()
{
	return _goal;
}

const Guess&		Game::get_guess(size_t pos) throw (std::range_error)
{
	if (pos >= _guesses.size())
		throw std::range_error("Game::get_guess(size_t pos) pos is out of range.");
	return _guesses[pos];
}

std::string			Game::get_random_word() throw (EmptyDictionaryException)
{
	if (!_randomPickVector.size())
		throw EmptyDictionaryException();

	std::uniform_int_distribution<size_t>	boundary(0, _randomPickVector.size()-1);

	return *_randomPickVector[boundary(_randomGenerator)];
}

bool				Game::_isalpha_and_uppercase_transform(std::string& str)
{
	const struct UppercaseUnaryOperation
	{
		char	operator()(char c)
		{
			if ('A' <= c && c <= 'Z')
				return c;
			if ('a' <= c && c <= 'z')
				return c - 32;
			throw InvalidWordException();
		}
	} toUpperOp;

	try
	{ std::transform(str.begin(), str.end(), str.begin(), toUpperOp); }
	catch (InvalidWordException e)
	{ return false; }
	return true;
}

void				Game::_add_word_to_dictionary_internal(const std::string& word)
{
	auto pair = _dictionary.insert(word);
	if (pair.second)
		_randomPickVector.push_back(&*pair.first);
}
