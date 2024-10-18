#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Pixel {
public:
    sf::CircleShape shape;
    sf::Color color = sf::Color::Black;
};

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Drawing");

    sf::Vector2i mousePosition = {};

    Pixel pixel;
    pixel.shape.setFillColor(pixel.color);
    pixel.shape.setRadius(20.f);
    std::vector<Pixel> pixels;

    sf::Texture redBoxUnselectedTexture, blueBoxUnselectedTexture, greenBoxUnselectedTexture, blackBoxUnselectedTexture;
    sf::Texture redBoxSelectedTexture, blueBoxSelectedTexture, greenBoxSelectedTexture, blackBoxSelectedTexture;

    redBoxUnselectedTexture.loadFromFile("Textures\\RedBoxUnselected.png");
    blueBoxUnselectedTexture.loadFromFile("Textures\\BlueBoxUnselected.png");
    greenBoxUnselectedTexture.loadFromFile("Textures\\GreenBoxUnselected.png");
    blackBoxUnselectedTexture.loadFromFile("Textures\\BlackBoxUnselected.png");

    redBoxSelectedTexture.loadFromFile("Textures\\RedBoxSelected.png");
    blueBoxSelectedTexture.loadFromFile("Textures\\BlueBoxSelected.png");
    greenBoxSelectedTexture.loadFromFile("Textures\\GreenBoxSelected.png");
    blackBoxSelectedTexture.loadFromFile("Textures\\BlackBoxSelected.png");

    sf::Sprite redBox, blueBox, greenBox, blackBox;

    redBox.setTexture(redBoxUnselectedTexture);
    redBox.setScale(4, 4);
    blueBox.setTexture(blueBoxUnselectedTexture);
    blueBox.setScale(4, 4);
    greenBox.setTexture(greenBoxUnselectedTexture);
    greenBox.setScale(4, 4);
    blackBox.setTexture(blackBoxUnselectedTexture);
    blackBox.setScale(4, 4);

    float textureSize = redBoxUnselectedTexture.getSize().x * redBox.getScale().y;
    blackBox.setPosition(WINDOW_WIDTH / 2 - textureSize * 2, WINDOW_HEIGHT - textureSize);
    redBox.setPosition(WINDOW_WIDTH / 2 - textureSize, WINDOW_HEIGHT - textureSize);
    blueBox.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - textureSize);
    greenBox.setPosition(WINDOW_WIDTH / 2 + textureSize, WINDOW_HEIGHT - textureSize);

    bool redSelected = false, blueSelected = false, greenSelected = false, blackSelected = false;
    bool colorSelection = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        mousePosition = sf::Mouse::getPosition(window);
        colorSelection = false;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f mousePos(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

            if (redBox.getGlobalBounds().contains(mousePos)) {
                redSelected = true;
                blueSelected = false;
                greenSelected = false;
                blackSelected = false;
                redBox.setTexture(redBoxSelectedTexture);
                blueBox.setTexture(blueBoxUnselectedTexture);
                greenBox.setTexture(greenBoxUnselectedTexture);
                blackBox.setTexture(blackBoxUnselectedTexture);
                pixel.color = sf::Color::Red;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }
            else if (blueBox.getGlobalBounds().contains(mousePos)) {
                redSelected = false;
                blueSelected = true;
                greenSelected = false;
                blackSelected = false;
                redBox.setTexture(redBoxUnselectedTexture);
                blueBox.setTexture(blueBoxSelectedTexture);
                greenBox.setTexture(greenBoxUnselectedTexture);
                blackBox.setTexture(blackBoxUnselectedTexture);
                pixel.color = sf::Color::Blue;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }
            else if (greenBox.getGlobalBounds().contains(mousePos)) {
                redSelected = false;
                blueSelected = false;
                greenSelected = true;
                blackSelected = false;
                redBox.setTexture(redBoxUnselectedTexture);
                blueBox.setTexture(blueBoxUnselectedTexture);
                greenBox.setTexture(greenBoxSelectedTexture);
                blackBox.setTexture(blackBoxUnselectedTexture);
                pixel.color = sf::Color::Green;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }
            else if (blackBox.getGlobalBounds().contains(mousePos)) {
                redSelected = false;
                blueSelected = false;
                greenSelected = false;
                blackSelected = true;
                redBox.setTexture(redBoxUnselectedTexture);
                blueBox.setTexture(blueBoxUnselectedTexture);
                greenBox.setTexture(greenBoxUnselectedTexture);
                blackBox.setTexture(blackBoxSelectedTexture);
                pixel.color = sf::Color::Black;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }
        }

        if (!colorSelection && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            pixel.shape.setPosition(static_cast<sf::Vector2f>(sf::Vector2f(mousePosition.x - 20.f, mousePosition.y - 20.f)));
            pixels.push_back(pixel);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !pixels.empty()) {
            pixels.pop_back();
        }

        window.clear(sf::Color::White);

        window.draw(blackBox);
        window.draw(redBox);
        window.draw(blueBox);
        window.draw(greenBox);

        for (auto& pixel : pixels) {
            window.draw(pixel.shape);
        }

        window.display();
    }

    return 0;
}
