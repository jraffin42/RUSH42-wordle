/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 16:09:47 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/15 10:40:43 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

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

	for (int t=0; t<5; ++t)
	{
		const Guess&	guess = game.guess_word(game.get_random_word());
		std::cout << "guess : " << guess.get_word() << std::endl;
		std::cout << "       [";
		for (int i=0;i< 5;++i)
		{
			if (guess.is_valid(i))
				std::cout << "V";
			else if (guess.is_misplaced(i))
				std::cout << "m";
			else
				std::cout << ".";
		}
		std::cout << "]" << std::endl;
	}

	return 0;
}
