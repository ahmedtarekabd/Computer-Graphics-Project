#pragma once

#include <iostream>
#include <string>

namespace our
{

    // The game system is responsible for marking the end game state either win or lose
    class GameController
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        static std::string gameState;

        static void setState(std::string state)
        {
            std::cout << "You " << state << "!" << std::endl;

            if (state == "win" || state == "lose")
                gameState = state;
        }
    };
}
