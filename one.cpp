#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <vector>
#include <math.h>

const float fps = 60;
const float period = 1000000/fps;
const int width = 800;
const int height = 600;
const float bwidth = 15;
const float bheight = 2;
const float radius = 10;

struct Bullet
{
    sf::Vector2f pos;
    float speed;
};

Bullet createBullet()
{
    Bullet bul;
    sf::Vector2f b(width, rand()% height);
    bul.pos = b;
    bul.speed = rand()% 2;
    bul.speed++;
    return bul;
}

bool isOutX( Bullet b)
{
    if(b.pos.x <= 0)
        return true;
    return false;
}

// CIRCLE/RECTANGLE
bool circleRect(float cx, float cy, float rad, float rx, float ry, float rw, float rh) 
{
  // temporary variables to set edges for testing
  float testX = cx;
  float testY = cy;

  // which edge is closest?
  if (cx < rx)         testX = rx;      // test left edge
  else if (cx > rx+rw) testX = rx+rw;   // right edge
  if (cy < ry)         testY = ry;      // top edge
  else if (cy > ry+rh) testY = ry+rh;   // bottom edge

  // get distance from closest edges
  float distX = cx-testX;
  float distY = cy-testY;
  float distance = sqrt( (distX*distX) + (distY*distY) );

  // if the distance is less than the radius, collision!
  if (distance <= rad) {
    return true;
  }
  return false;
}

int main()
{
    srand(time(NULL));
    sf::Vector2f pos( 20, 20);  ///< Player position
    std::vector<Bullet> bullets;           
    sf::RenderWindow window(sf::VideoMode( 800, 600), "SFML works!");
    
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(  pos);

    bool gameover = true;
    sf::Text text;
    sf::Font font;
    font.loadFromFile("Pacifico.ttf");
    text.setFont(font);
    text.setCharacterSize(40);
    text.setColor(sf::Color::White);

    sf::Clock timer;
    sf::Clock gameTime;

    int score = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (!gameover){     ///< If game is running.
            sf::Time dt = timer.restart();  ///< Frame rate controller.

            ///< Update position of player.
            sf::Vector2f newpos = pos;
            newpos.y -= (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))      ?2 * 100 * dt.asSeconds():0;
            newpos.y += (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))    ?2 * 100 * dt.asSeconds():0;
            newpos.x -= (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))    ?2 * 100 * dt.asSeconds():0;
            newpos.x += (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))   ?2 * 100 * dt.asSeconds():0;
            if (newpos.x >= 0 && newpos.x <= width-radius * 2)
                pos.x = newpos.x;
            if (newpos.y > 0 && newpos.y <= height - radius * 2)
                pos.y = newpos.y;
            shape.setPosition(  pos);

            ///< Update bullets and check for colitio.
            for (auto& b : bullets)
            {
                b.pos.x-= b.speed * 100 * dt.asSeconds();
                if (circleRect(newpos.x+radius, newpos.y+radius, radius, b.pos.x, b.pos.y, bwidth, bheight))
                    gameover = true;
            }
            std::remove_if(bullets.begin(), bullets.end(), isOutX);
            if(rand()% 100 > 98)
                bullets.push_back( createBullet());

            ///Get time for score.
            sf::Time s = gameTime.getElapsedTime();
            score = s.asMilliseconds()/100;
            text.setString(std::to_string(score));
            text.setPosition(width/2 -50, 5);
        }
        else {  ///< If game over.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                pos.x = 20;
                pos.y = height / 2;
                bullets.clear();
                gameover = false;
                gameTime.restart();
                score = 0;
            }
            text.setString("Game Over\r\nYour Score: " + std::to_string(score));
            text.setPosition(width/2 -50, 5);
        }

        ///< Draw Everyting.
        window.clear();
        for (auto& b : bullets)
        {
            sf::RectangleShape r;
            r.setFillColor(sf::Color::Red);
            r.setPosition(b.pos);
            r.setSize(sf::Vector2f(bwidth, bheight));
            window.draw(r);
        }
        window.draw(shape);
        window.draw(text);
        window.display();
        
    }
    return 0;
}