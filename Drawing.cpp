#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Pixel {
public:
    sf::CircleShape shape;
    sf::Color color = sf::Color::Black;
};

void DrawPixels(Pixel& pixel, std::vector<Pixel*>& pixels, sf::CircleShape cursor) {
    pixel.shape.setFillColor(pixel.color);
    pixel.shape.setPosition(cursor.getPosition() - sf::Vector2f(pixel.shape.getRadius(), pixel.shape.getRadius()));
    Pixel* newPixel = new Pixel;
    newPixel->color = pixel.color;
    newPixel->shape = pixel.shape;
    pixels.push_back(newPixel);
}

void DeletePixels(Pixel& pixel, std::vector<Pixel*>& pixels, sf::CircleShape cursor) {
    sf::Vector2f cursorCenter = cursor.getPosition();
    float cursorRadius = cursor.getRadius();

    for (size_t i = 0; i < pixels.size(); i++) {
        sf::Vector2f pixelCenter = pixels[i]->shape.getPosition() + sf::Vector2f(pixels[i]->shape.getRadius(), pixels[i]->shape.getRadius());
        float distance = std::sqrt(std::pow(pixelCenter.x - cursorCenter.x, 2) + std::pow(pixelCenter.y - cursorCenter.y, 2));

        if (distance <= cursorRadius) {
            delete pixels[i];
            pixels.erase(pixels.begin() + i);
            break;
        }
    }
}

void Render(sf::RenderWindow& window, std::vector<sf::Sprite> boxes, std::vector<Pixel*> pixels, sf::Sprite eraser, sf::CircleShape cursor, sf::Text eraserText, sf::Sprite arrowUp, sf::Sprite arrowDown) {
    window.clear(sf::Color::White);

    for (auto& box : boxes) {
        window.draw(box);
    }

    for (auto& pixel : pixels) {
        window.draw(pixel->shape);
    }

    window.draw(eraser);
    window.draw(eraserText);
    window.draw(arrowUp);
    window.draw(arrowDown);
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
    eraserText.setCharacterSize(24);

    bool eraserSelected = false;

    eraserTexture.loadFromFile("Textures\\eraser.png");
    eraser.setTexture(eraserTexture);
    eraser.setScale(4, 4);
    eraser.setPosition(WINDOW_WIDTH - eraserTexture.getSize().x * eraser.getScale().x, WINDOW_HEIGHT / 2);
    eraserText.setPosition(eraser.getPosition().x, eraser.getPosition().y - 25);

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
    cursor.setRadius(pixel.shape.getRadius());
    cursor.setOrigin(cursor.getRadius(), cursor.getRadius());
    cursor.setFillColor(sf::Color(100.f, 0, 0, 100));

    // Boxes for Colors
    std::vector<sf::Sprite> boxes = { redBox, blackBox, greenBox, blueBox };

    sf::Texture arrowUpTexture;
    sf::Texture arrowDownTexture;

    arrowUpTexture.loadFromFile("Textures\\arrowUp.png");
    arrowDownTexture.loadFromFile("Textures\\arrowDown.png");

    sf::Sprite arrowUp;
    sf::Sprite arrowDown;

    arrowUp.setTexture(arrowUpTexture);
    arrowDown.setTexture(arrowDownTexture);

    arrowUp.setScale(4, 4);
    arrowDown.setScale(4, 4);

    arrowUp.setPosition(0, WINDOW_HEIGHT / 2);
    arrowDown.setPosition(0, arrowUp.getPosition().y + arrowUpTexture.getSize().y * arrowUp.getScale().y);

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
                    if (blueBox.getGlobalBounds().contains(mousePos)) {
                        pixel.color = sf::Color::Blue;
                        blueSelected = true; redSelected = greenSelected = blackSelected = false;
                    }
                    if (greenBox.getGlobalBounds().contains(mousePos)) {
                        pixel.color = sf::Color::Green;
                        greenSelected = true; redSelected = blueSelected = blackSelected = false;
                    }
                    if (blackBox.getGlobalBounds().contains(mousePos)) {
                        pixel.color = sf::Color::Black;
                        blackSelected = true; redSelected = blueSelected = greenSelected = false;
                    }
                }
            }
        }

        // Pointer config
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
        cursor.setPosition(mousePos);

        // Drawing or Erasing pixels
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!eraserSelected) {
                DrawPixels(pixel, pixels, cursor);
            }
            else {
                DeletePixels(pixel, pixels, cursor);
            }

            if (arrowUp.getGlobalBounds().contains(mousePos)) {
                if (pixel.shape.getRadius() + 0.1 > 0) {
                    pixel.shape.setRadius(pixel.shape.getRadius() + 0.1);
                    cursor.setRadius(pixel.shape.getRadius());
                    cursor.setOrigin(cursor.getRadius(), cursor.getRadius());
                }
            }
            if (arrowDown.getGlobalBounds().contains(mousePos)) {
                if (pixel.shape.getRadius() - 0.1 > 0) {
                    pixel.shape.setRadius(pixel.shape.getRadius() - 0.1);
                    cursor.setRadius(pixel.shape.getRadius());
                    cursor.setOrigin(cursor.getRadius(), cursor.getRadius());
                }
            }
        }

        Render(window, boxes, pixels, eraser, cursor, eraserText, arrowUp, arrowDown);
    }

    //Delete Pixels and clear
    for (auto& pixel : pixels) {
        delete pixel;
    }

    pixels.clear();

    return 0;
}
