#include "EngineWrapper.hpp"
#include "engine.hpp"
#include "engineRandom.hpp"
#include "engineTest.hpp"
#include "chessEngine0.0.hpp"
#include "Engine01Wrapper.hpp"
#include "Engine02.hpp"
#include "Gui.hpp"


int main()
{
  //these are used to start a couple different tests for the engine, useless for normal operation however
 /*  Engine02* engine1 = new Engine02();
   EngineWrapper engineWrapper1(engine1);
   Engine00 testEngine;
   Engine01* engine2 = new Engine01();
   EngineWrapper engineWrapper2(engine2);
   EngineTest myTest(engineWrapper1, engineWrapper2, 10);
   delete engine1;
*/
// creates and starts the gui application
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Chess Engine", sf::Style::Fullscreen);
    EngineGui myGui(&window);
    myGui.run();

    return 0;
}
