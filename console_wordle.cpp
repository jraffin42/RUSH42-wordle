/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_wordle.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 16:09:47 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/15 12:18:40 by jraffin          ###   ########.fr       */
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
		while (true)
		{
			std::cout << "Guess " << game.guesses() << ": ";
			std::getline(std::cin, word);
			std::cout << "[" << word << "]" << std::endl;
			if (game.is_word_valid(word))
				break;
			else
				std::cout << word << std::endl;
		}
		game.guess_word(word);
	}
	return 0;
}
