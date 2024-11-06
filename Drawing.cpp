#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Pixel {
public:
    sf::CircleShape shape;
    sf::Color color = sf::Color::Black;
};

void Render(sf::RenderWindow& window, std::vector<sf::Sprite> boxes, std::vector<Pixel*> pixels, sf::Sprite eraser, sf::CircleShape cursor, sf::Text eraserText) {
    window.clear(sf::Color::White);

    for (auto& box : boxes) {
        window.draw(box);
    }

    for (auto& pixel : pixels) {
        window.draw(pixel->shape);
    }

    window.draw(eraser);
    window.draw(eraserText);
    window.draw(cursor);
    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "InkOcean");

    Pixel pixel;
    pixel.shape.setRadius(20.f);
    std::vector<Pixel*> pixels;

    sf::Texture eraserTexture;
    sf::Sprite eraser;

    sf::Font font;
    sf::Text eraserText;

    font.loadFromFile("Fonts\\Minecraft.ttf");

    eraserText.setFont(font);
    eraserText.setFillColor(sf::Color::Black);
    eraserText.setString("OFF");
    eraserText.setCharacterSize(12);

    bool eraserSelected = false;

    eraserTexture.loadFromFile("Textures\\eraser.png");
    eraser.setTexture(eraserTexture);
    eraser.setScale(4, 4);
    eraser.setPosition(WINDOW_WIDTH - eraserTexture.getSize().x * eraser.getScale().x, WINDOW_HEIGHT / 2);
    eraserText.setPosition(eraser.getPosition().x, eraser.getPosition().y - 14);

    sf::Texture redBoxTexture, blueBoxTexture, greenBoxTexture, blackBoxTexture;
    sf::Sprite redBox, blueBox, greenBox, blackBox;

    redBoxTexture.loadFromFile("Textures\\RedBoxUnselected.png");
    blueBoxTexture.loadFromFile("Textures\\BlueBoxUnselected.png");
    greenBoxTexture.loadFromFile("Textures\\GreenBoxUnselected.png");
    blackBoxTexture.loadFromFile("Textures\\BlackBoxUnselected.png");

    redBox.setTexture(redBoxTexture);
    redBox.setScale(4, 4);
    blueBox.setTexture(blueBoxTexture);
    blueBox.setScale(4, 4);
    greenBox.setTexture(greenBoxTexture);
    greenBox.setScale(4, 4);
    blackBox.setTexture(blackBoxTexture);
    blackBox.setScale(4, 4);

    float textureSize = redBoxTexture.getSize().x * redBox.getScale().y;
    blackBox.setPosition(WINDOW_WIDTH / 2 - textureSize * 2, WINDOW_HEIGHT - textureSize);
    redBox.setPosition(WINDOW_WIDTH / 2 - textureSize, WINDOW_HEIGHT - textureSize);
    blueBox.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - textureSize);
    greenBox.setPosition(WINDOW_WIDTH / 2 + textureSize, WINDOW_HEIGHT - textureSize);

    bool redSelected = false, blueSelected = false, greenSelected = false, blackSelected = true;
    pixel.color = sf::Color::Black; // Default color

    // Cursor for showing where the user draws
    sf::CircleShape cursor;
    cursor.setRadius(20.0f);
    cursor.setFillColor(sf::Color(100.f, 0, 0, 100));

    // Boxes for Colors
    std::vector<sf::Sprite> boxes = { redBox, blackBox, greenBox, blueBox };

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                // Eraser toggle
                if (eraser.getGlobalBounds().contains(mousePos)) {
                    eraserSelected = !eraserSelected;
                    eraserText.setString(eraserSelected ? "ON" : "OFF");
                }
                else {
                    // Color selection
                    if (redBox.getGlobalBounds().contains(mousePos)) {
                        pixel.color = sf::Color::Red;
                        redSelected = true; blueSelected = greenSelected = blackSelected = false;
                    }
                    else if (blueBox.getGlobalBounds().contains(mousePos)) {
                        pixel.color = sf::Color::Blue;
                        blueSelected = true; redSelected = greenSelected = blackSelected = false;
                    }
                    else if (greenBox.getGlobalBounds().contains(mousePos)) {
                        pixel.color = sf::Color::Green;
                        greenSelected = true; redSelected = blueSelected = blackSelected = false;
                    }
                    else if (blackBox.getGlobalBounds().contains(mousePos)) {
                        pixel.color = sf::Color::Black;
                        blackSelected = true; redSelected = blueSelected = greenSelected = false;
                    }
                }
            }
        }

        // Pointer config
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
        cursor.setPosition(mousePos - sf::Vector2f(20.f, 20.f));

        // Drawing or Erasing pixels
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!eraserSelected) {
                pixel.shape.setFillColor(pixel.color);
                pixel.shape.setPosition(mousePos - sf::Vector2f(20.f, 20.f));
                Pixel* newPixel = new Pixel;
                newPixel->color = pixel.color;
                newPixel->shape = pixel.shape;
                pixels.push_back(newPixel);
            }
            else {
                for (size_t i = 0; i < pixels.size(); i++) {
                    if (pixels[i]->shape.getGlobalBounds().contains(mousePos)) {
                        delete pixels[i];
                        pixels.erase(pixels.begin() + i);
                        break;
                    }
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !pixels.empty()) {
            delete pixels.back();
            pixels.pop_back();
        }

        Render(window, boxes, pixels, eraser, cursor, eraserText);
    }

    for (auto& pixel : pixels) {
        delete pixel;
    }
    pixels.clear();

    return 0;
}
