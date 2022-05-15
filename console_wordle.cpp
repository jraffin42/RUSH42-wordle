/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_wordle.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 16:09:47 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/15 13:41:28 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "Game.hpp"
#include "Guess.hpp"

int main()
{
	Game	game;

	try
	{
		game.import_dictionary_file("../words.txt");
	}
	catch (Game::FileImportFailedException& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	game.start_game();

	std::cout <<  game.get_goal() << std::endl;
	while (game.is_running())
	{
		std::string	word;
		std::cout << "Take a guess (" << game.max_guesses() - game.guesses() << " left) : ";
		while (std::getline(std::cin, word) && !game.is_word_valid(word))
		{
			std::cout << word << std::endl;
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
	return 0;
}
