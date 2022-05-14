/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraffin <jraffin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 16:09:47 by jraffin           #+#    #+#             */
/*   Updated: 2022/05/14 21:29:50 by jraffin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Game.hpp"
#include "Guess.hpp"

int main()
{



/*	Game	game;

	game.add_word_to_dictionary("oooaa");
	game.add_word_to_dictionary("oaaoa");
	try
	{
		game.import_dictionary_file("../words.txt");
	}
	catch (Game::FileImportFailedException& e)
	{
		std::cout << e.what() << std::endl;
	}
	game.start_game();
	const Guess&	guess = game.guess_word("oaaoa");*/

	Guess	guess("oaaoa", "oooaa");

	for (int t=0; t<5; ++t)
	{
		for (int i=0;i< 5;++i)
		{
			if (guess.is_valid(i))
				std::cout << " [  VALID  ] ";
			else if (guess.is_misplaced(i))
				std::cout << " [MISPLACED] ";
			else
				std::cout << " [ ABSENT  ] ";
		}
		std::cout << std::endl;
	}

	return 0;
}
