#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Pixel {
public:
    sf::CircleShape shape;
    sf::Color color = sf::Color::Black;
};

void Render(sf::RenderWindow& window, std::vector<sf::Sprite> boxes, std::vector<Pixel*> pixels, sf::Sprite eraser, sf::CircleShape cursor) {
    window.clear(sf::Color::White);

    for (auto& box : boxes) {
        window.draw(box);
    }

    for (auto& pixel : pixels) {
        window.draw(pixel->shape);
    }

    window.draw(eraser);
    window.draw(cursor);
    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "InkOcean");
    sf::Vector2i mousePosition = {};

    Pixel pixel;
    pixel.shape.setFillColor(pixel.color);
    pixel.shape.setRadius(20.f);
    std::vector<Pixel*> pixels;

    sf::Texture eraserTexture;
    sf::Sprite eraser;

    bool eraserSelected = false;

    eraserTexture.loadFromFile("Textures\\eraser.png");
    eraser.setTexture(eraserTexture);
    eraser.setScale(4, 4);
    eraser.setPosition(WINDOW_WIDTH - eraserTexture.getSize().x * eraser.getScale().x, WINDOW_HEIGHT / 2);

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

    bool redSelected = false, blueSelected = false, greenSelected = false, blackSelected = true;
    bool colorSelection = false;

    sf::CircleShape cursor;
    cursor.setRadius(20.0f);
    cursor.setFillColor(sf::Color(100.f, 0, 0, 100));

    std::vector<sf::Sprite> boxes;
    boxes.push_back(redBox);
    boxes.push_back(blackBox);
    boxes.push_back(greenBox);
    boxes.push_back(blueBox);

    //Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        //Pointer Config
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
        cursor.setPosition(mousePos - sf::Vector2f(20.f, 20.f));
        colorSelection = false;

        //Inputs
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            // Color selection
            if (redBox.getGlobalBounds().contains(mousePos)) {
                redSelected = true;
                blueSelected = greenSelected = blackSelected = eraserSelected = false;

                redBox.setTexture(redBoxSelectedTexture);
                blueBox.setTexture(blueBoxUnselectedTexture);
                greenBox.setTexture(greenBoxUnselectedTexture);
                blackBox.setTexture(blackBoxUnselectedTexture);

                pixel.color = sf::Color::Red;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }
            if (blueBox.getGlobalBounds().contains(mousePos)) {
                blueSelected = true;
                redSelected = greenSelected = blackSelected = eraserSelected = false;

                redBox.setTexture(redBoxUnselectedTexture);
                blueBox.setTexture(blueBoxSelectedTexture);
                greenBox.setTexture(greenBoxUnselectedTexture);
                blackBox.setTexture(blackBoxUnselectedTexture);

                pixel.color = sf::Color::Blue;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }
            if (greenBox.getGlobalBounds().contains(mousePos)) {
                greenSelected = true;
                redSelected = blueSelected = blackSelected = eraserSelected = false;

                redBox.setTexture(redBoxUnselectedTexture);
                blueBox.setTexture(blueBoxUnselectedTexture);
                greenBox.setTexture(greenBoxSelectedTexture);
                blackBox.setTexture(blackBoxUnselectedTexture);

                pixel.color = sf::Color::Green;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }
            if (blackBox.getGlobalBounds().contains(mousePos)) {
                blackSelected = true;
                redSelected = blueSelected = greenSelected = eraserSelected = false;

                redBox.setTexture(redBoxUnselectedTexture);
                blueBox.setTexture(blueBoxUnselectedTexture);
                greenBox.setTexture(greenBoxUnselectedTexture);
                blackBox.setTexture(blackBoxSelectedTexture);

                pixel.color = sf::Color::Black;
                pixel.shape.setFillColor(pixel.color);
                colorSelection = true;
            }

            if (eraser.getGlobalBounds().contains(mousePos)) {
                eraserSelected = !eraserSelected;
            }
        }

        if (!colorSelection && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            pixel.shape.setPosition(mousePos - sf::Vector2f(20.f, 20.f));
            Pixel* newPixel = new Pixel;
            newPixel->color = pixel.color;
            newPixel->shape = pixel.shape;
            pixels.push_back(newPixel);
        }

        if (!colorSelection && sf::Mouse::isButtonPressed(sf::Mouse::Left) && eraserSelected) {
            for (size_t i = 0; i < pixels.size(); i++) {
                if (pixels[i]->shape.getGlobalBounds().contains(mousePos)) {
                    delete pixels[i];
                    pixels.erase(pixels.begin() + i);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !pixels.empty()) {
            delete pixels.back();
            pixels.pop_back();
        }

        Render(window, boxes, pixels, eraser, cursor);
    }

    for (auto& pixel : pixels) {
        delete pixel;
    }
    pixels.clear();

    return 0;
}
