#include <iostream>
#include <vector>
#include <math.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "collisions.h"
#include "drawables.h"
/*TODO*/
//verkkopeli ominaisuus

sf::CircleShape ball;

sf::RectangleShape player1, player2;
    sf::Text score1, score2;
    sf::Text player1W, player2W;
sf::Font font;
//reunat
sf::RectangleShape bottomLine, topLine, middleLine;


std::size_t received;
std::string text = "Connected ";
std::string valinta;
sf::Vector2f ballMov(1, 0);
int player1Score, player2Score = 0;

void drawables();
void handleCollisions();
void hostGame();
void joinGame();
int main()
{
    
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::TcpSocket socket;
    char connectionType, mode, mode2;
    char buffer[2000];
    char buffer2[2000];

    sf::Packet packet1;
    std::cout << "Enter (h) to host server, Enter (j) to join"<<std::endl;
	std::cin >> connectionType;

    bool update = false;

    if (connectionType == 'h')//host //server
	{
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
		text += "to server. Lets play some pong";
		mode = 'r';

	}

	else if (connectionType == 'j')//join //client
	{
		socket.connect(ip, 2000);
		text += "to client. Lets play some pong.";
		mode = 's';

	}
    socket.send(text.c_str(), text.length() + 1);

	socket.receive(buffer, sizeof(buffer), received);

	std::cout << buffer << std::endl;

	bool done = false;
    drawables();

    score1.setFont(font);

    // Luo ikkunasf::Font font;
    font.loadFromFile("arial.ttf");
    // Create a text

    sf::RenderWindow window(sf::VideoMode(800, 600), "ikkuna");
    window.setFramerateLimit(60);
    // Ikkuna on paalle niin kauan kuin tama looppi kiertaa
   while(!done)//delete loop if hard to implement
   {
    while (window.isOpen())
    {
    sf::Vector2f player1Pos;
    sf::Vector2f player2Pos;
    sf::Vector2f ballPos;

        if (connectionType == 'h')//host
        {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))     player1.move(0, -8);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))     player1.move(0,  8);
        packet1 <<player1.getPosition().x<< player1Pos.y << ball.getPosition().x <<ball.getPosition().y;
        // player1Pos = player1.getPosition();
        socket.send(packet1);
         }
          if (connectionType == 'j')//join
        {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))     player2.move(0, -8);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))     player2.move(0,  8);
        socket.receive(packet1);
        if(packet1 >> player1Pos.x >>player1Pos.y >> ballPos.x >>ballPos.y)
        {
            player1.setPosition(player1Pos);
            ball.setPosition(ballPos);
        }
 
        }

        // if(connectionType == 'l')
        // {
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))     player1.move(0, -8);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))     player1.move(0,  8);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    player2.move(0, -8);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  player2.move(0,  8);
        // }
        sf::Event event;
        while (window.pollEvent(event))
        {
            // close sammuttaa ikkunan...-.-
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                std::cout<<"Peli suljettu"<<std::endl;
                return 0;
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                update = true;
            }
            else if(event.type == sf::Event::LostFocus)
            {
                update = false;
            }
        }

        //Pallo meni pelaaja 1 ohi
        if (ball.getPosition().x < player1.getPosition().x)
        {
            ball.setPosition(400, 300);
            ballMov = sf::Vector2f(-1.0,0.0);
            player2Score +=1;
        }
        //Pallo meni pelaaja 2 ohi
        else if (ball.getPosition().x > player2.getPosition().x)
        {
            ball.setPosition(400, 300);
            ballMov = sf::Vector2f(1.0, 0.0);
            player1Score += 1;
        }


        handleCollisions();
        //Taustavari
        score1.setString(std::to_string(player1Score));
        score2.setString(std::to_string(player2Score));

        window.clear(sf::Color::White);
        window.setFramerateLimit(60);

        window.draw(middleLine);
        window.draw(player1);
        window.draw(player2);
        window.draw(ball);
        window.draw(bottomLine);
        window.draw(topLine);
        window.draw(score1);
        window.draw(score2);

        if (player1Score >= 5)
        {
            window.draw(player1W);
            ballMov = sf::Vector2f(0.0,0.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                player1Score = 0;
                ballMov = sf::Vector2f(-1.0,0.0);
                player1.setPosition(sf::Vector2f( 10, 255 ));
                player2.setPosition(sf::Vector2f( 790 , 255 ));
            }
        }
         if (player2Score >= 5)
        {
            window.draw(player2W);
            ballMov = sf::Vector2f(0.0,0.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                player2Score = 0;
                ballMov = sf::Vector2f(1.0,0.0);
                player1.setPosition(sf::Vector2f( 10, 255 ));
                player2.setPosition(sf::Vector2f( 790 , 255 ));
            }
        }
        window.display();
    }
   }
    return 0;
}

void handleCollisions()
{
        sf::Time t1 = sf::seconds(5);
        float deltaTime = t1.asSeconds();
        float pad1Mid = player1.getPosition().y + 45;
        float pad2Mid = player2.getPosition().y + 45;
        //ylos miinus alas plussaa , nolla piste vasen ylakulma HOXHOX
        if (RectangleOnCircle(player1, ball) == false && RectangleOnCircle(player2, ball) == false) { ball.move(ballMov.x * deltaTime, ballMov.y);}
        //pelaajan tormaykset reunojen kanssa
        if (RectangleOnRectangle(player1,topLine)){player1.move    (0,  8);}
        if (RectangleOnRectangle(player1, bottomLine)){player1.move(0, -8);}
        if (RectangleOnRectangle(player2,topLine)){player2.move    (0,  8);}
        if (RectangleOnRectangle(player2, bottomLine)){player2.move(0, -8);}
        //pelaajan tormaykset reunojen kanssa

        //pelaajien ja pallon tormaykset
        if (RectangleOnCircle(player1, ball))
        {
            ball.setPosition(ball.getPosition().x + 6, ball.getPosition().y);
            //vaihtaa pallon suuntaa
            ballMov.x = ballMov.x * -1;
            //lisataan jokaisella pelaajan osumalla hiukan vauhtia
            ballMov.x = ballMov.x + .01;
            //PELAAJA YLAPUOLELLA PALLOA -- pallo alempana keskipistetta
            if (pad1Mid < ball.getPosition().y and pad1Mid - ball.getPosition().y < 45) { ballMov.y = ((pad1Mid - ball.getPosition().y) / 10) * -1; }//yl�puoli
            //PELAAJA ALAPUOLELLA PALLOA -- pallo ylempana keskipistetta
            if (pad1Mid > ball.getPosition().y and pad1Mid - ball.getPosition().y > -45) { ballMov.y = ((pad1Mid - ball.getPosition().y) / 10)*-1; }//alapuoli
        }
        if (RectangleOnCircle(player2, ball))
        {
            ball.setPosition(ball.getPosition().x - 6, ball.getPosition().y);
            //vaihtaa pallon suuntaa
            ballMov.x = ballMov.x * -1;
            //lisataan jokaisella pelaajan osumalla hiukan vauhtia
            ballMov.x = ballMov.x - .01;
            //PELAAJA YLAPUOLELLA PALLOA -- pallo alempana keskipistetta
            if (pad2Mid < ball.getPosition().y and pad2Mid - ball.getPosition().y < 45) { ballMov.y = ((pad2Mid - ball.getPosition().y) / 10) * -1; }//yl�puoli
            //PELAAJA ALAPUOLELLA PALLOA -- pallo ylempana keskipistetta
            if (pad2Mid > ball.getPosition().y  and pad2Mid - ball.getPosition().y > -45 ) { ballMov.y = ((pad2Mid - ball.getPosition().y) / 10) * -1; }//alapuoli
        }
        //pelaajien ja pallon tormaykset

        //kaantaa pallon suuntaa y akselilla osuessa kattoon tai pohjaan
        if (RectangleOnCircle(topLine, ball))    {ballMov.y = ballMov.y *-1; }
        if (RectangleOnCircle(bottomLine, ball)) {ballMov.y = ballMov.y * -1;}

}

void drawables()
{
    //pelipallo
    ball.setFillColor(sf::Color::Red);
    ball.setRadius  ( 4 );
    ball.setPosition( 70 , 300);
    ball.setOrigin(ball.getRadius(), ball.getRadius());

    player1.setFillColor(sf::Color::Black);
    player1.setSize(sf::Vector2f    ( 5, 90 ));
    player1.setPosition(sf::Vector2f( 10, 255 ));
    player1.setOrigin(sf::Vector2f  ( 0 , 0 ));

    player2.setFillColor(sf::Color::Black);
    player2.setSize(sf::Vector2f    ( 5, 90 ));
    player2.setPosition(sf::Vector2f( 790 , 255 ));
    player2.setOrigin(sf::Vector2f  ( 0 , 0 ));

    bottomLine.setFillColor(sf::Color::Black);
    bottomLine.setSize(sf::Vector2f    ( 800 , 10 ));
    bottomLine.setPosition(sf::Vector2f( 0 , 590 ));

    topLine.setFillColor(sf::Color::Black);
    topLine.setSize(sf::Vector2f    ( 800 , 10 ));
    topLine.setPosition(sf::Vector2f( 0, 0 ));

    middleLine.setFillColor(sf::Color::Black);
    middleLine.setSize(sf::Vector2f    ( 10 , 600 ));
    middleLine.setPosition(sf::Vector2f( 400 , 2.5 ));


    score1.setFillColor(sf::Color::Black);
    score1.setPosition(sf::Vector2f(350,10));
    score2.setFont(font);
    score2.setFillColor(sf::Color::Black);
    score2.setPosition(sf::Vector2f(450,10));
    player1W.setFont(font);
    player1W.setString("player 1 won\npress r to play another game");
    player1W.setFillColor(sf::Color::Black);
    player1W.setPosition(sf::Vector2f(410,30));
    player2W.setFont(font);
    player2W.setString("player 1 won\npress r to play another game");
    player2W.setPosition(sf::Vector2f(410,30));
    player2W.setFillColor(sf::Color::Black);
}
// sf::RectangleShape player2;
    // sf::Text score2;
    // sf::Text player2W;
// sf::RectangleShape topLine;
// sf::RectangleShape middleLine;
// int player2Score = 0;

//g++ -c main.cpp
//g++ main.cpp collisions.cpp -o multiplayer -lm -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
//./multiplayer