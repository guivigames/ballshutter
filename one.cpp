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
bool circleRect(float cx, float cy, float rad, float rx, float ry, float rw, float rh) {

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

    sf::Clock timer;
    while (window.isOpen())
    {
        if (!gameover){
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            
            sf::Time dt = timer.restart();
            sf::Vector2f newpos = pos;

            newpos.y -= (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))      ?2 * 100 * dt.asSeconds():0;
            newpos.y += (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))    ?2 * 100 * dt.asSeconds():0;
            newpos.x -= (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))    ?2 * 100 * dt.asSeconds():0;
            newpos.x += (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))   ?2 * 100 * dt.asSeconds():0;

            for (auto& b : bullets)
                b.pos.x-= b.speed * 100 * dt.asSeconds();

            for (auto& b : bullets)
            {
                if (circleRect(newpos.x+radius, newpos.y+radius, radius, b.pos.x, b.pos.y, bwidth, bheight))
                    gameover = true;//return true;
            }
            std::remove_if(bullets.begin(), bullets.end(), isOutX);

            if(rand()% 100 > 98)
                bullets.push_back( createBullet());

            if (newpos.x >= 0 && newpos.x <= width-radius * 2)
                pos.x = newpos.x;
            if (newpos.y > 0 && newpos.y <= height - radius * 2)
                pos.y = newpos.y;

            shape.setPosition(  pos);


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
            window.display();
        }
        else {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                pos.x = 20;
                pos.y = height / 2;
                bullets.clear();
                gameover = false;
            }
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
            window.display();
        }
        
    }

    return 0;
}