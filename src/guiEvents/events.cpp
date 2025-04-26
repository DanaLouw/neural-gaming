#include <SFML/System/String.hpp>
#include <iostream>

#include "events.hpp"

void processEvents(sf::Window& window, sf::Text& text) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    } else if (event.type == sf::Event::KeyPressed) {     
      if (event.key.code == sf::Keyboard::Escape) {
        window.close();
      } else if (event.key.code == 26) {
        text.setString(text.getString() + "0");
        std::cout << "0\n";
      } else if (event.key.code == 27) {
        text.setString(text.getString() + "1");
        std::cout << "1\n";
      } else if (event.key.code == 28) {
        text.setString(text.getString() + "2");
        std::cout << "2\n";
      } else if (event.key.code == 29) {
        text.setString(text.getString() + "3");
        std::cout << "3\n";
      } else if (event.key.code == 30) {
        text.setString(text.getString() + "4");
        std::cout << "4\n";
      } else if (event.key.code == 31) {
        text.setString(text.getString() + "5");
        std::cout << "5\n";
      } else if (event.key.code == 32) {
        text.setString(text.getString() + "6");
        std::cout << "6\n";
      } else if (event.key.code == 33) {
        text.setString(text.getString() + "7");
        std::cout << "7\n";
      } else if (event.key.code == 34) {
        text.setString(text.getString() + "8");
        std::cout << "8\n";
      } else if (event.key.code == 35) {
        text.setString(text.getString() + "9");
        std::cout << "9\n";
      } else if (event.key.code == sf::Keyboard::BackSpace) {
        text.setString(text.getString().substring(0, text.getString().getSize() - 1));
        std::cout << "Backspace\n";  
      } else {
        std::cout << event.key.code << std::endl;
      }
    } 
  }
}
