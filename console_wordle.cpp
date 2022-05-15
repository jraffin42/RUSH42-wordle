/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_wordle.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 16:09:47 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/15 15:06:45 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "Game.hpp"
#include "Guess.hpp"

static void	display_board(const Game& game)
{
	std::cout << "\x1B[2J\x1B[H" << "W O R D L E" << std::endl << std::endl;
	size_t y=0;
	while (y<game.guesses())
	{
		const Guess& guess = game.get_guess(y);
		const std::string& word = guess.get_word();
		std::cout << " [";
		for (size_t x=0; x<game.word_length(); ++x)
		{
			if (guess.is_valid(x))
				std::cout << "\e[0;32m";
			else if (guess.is_misplaced(x))
				std::cout << "\e[0;33m";
			std::cout << word[x] << "\e[0m";
		}
		std::cout << "]" << std::endl;
		++y;
	}
	while (y++<game.max_guesses())
		std::cout << " [     ]" << std::endl;
	std::cout << std::endl;
}

int main()
{
	Game	game;

	try
	{
		game.import_dictionary_file("words.txt");
	}
	catch (Game::FileImportFailedException& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	game.start_game();

	while (game.is_running())
	{
		display_board(game);
		std::string	word;
		std::cout << std::endl << "Take a guess (" << game.max_guesses() - game.guesses() << " left) : ";
		while (std::getline(std::cin, word) && !game.is_word_valid(word))
		{
			display_board(game);
			if (word.size() != game.word_length())
				std::cout << "Your guessed word must be " << game.word_length() << " characters long." << std::endl;
			else if (!game.is_word_alpha("word"))
				std::cout << "Your guessed word must be composed of alphabetic characters only ([a-z][A-Z])." << std::endl;
			else
				std::cout << "Your guessed word is not found in the game dictionary." << std::endl;
			std::cout << "Take another guess : (" << game.max_guesses() - game.guesses() << " left) : ";
		}
		if (!std::cin.good())
		{
			std::cout << "Error : standard input was closed." << std::endl;
			return 1;
		}
		game.guess_word(word);
	}
	display_board(game);
	if (game.is_won())
		std::cout << "Congratulations, you won in " << game.guesses() << " guesses!" << std::endl;
	else
		std::cout << "Sorry, you lost! The word to find was : " << game.get_goal() << "." << std::endl;
	return 0;
}
