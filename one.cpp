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

int main()
{
    srand(time(NULL));
    sf::Vector2f pos( 20, 20);  ///< Player position
    std::vector<Bullet> bullets;           
    sf::RenderWindow window(sf::VideoMode( 800, 600), "SFML works!");
    
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(  pos);

    sf::Clock timer;
    while (window.isOpen())
    {
        
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
            float dist = pow ( pow(b.pos.x - newpos.x, 2)+pow(b.pos.y - newpos.y,2), 0.5);
            if (dist < radius)  // Gameover.
                return 0;
        }
        
        std::remove_if(bullets.begin(), bullets.end(), isOutX);

        if(rand()% 100 > 98)
            bullets.push_back( createBullet());

        if (newpos.x >= 0 && newpos.x <= 780)
            pos.x = newpos.x;
        if (newpos.y > 0 && newpos.y <= 580)
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

    return 0;
}