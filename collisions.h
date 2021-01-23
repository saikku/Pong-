#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <iostream>
#include <vector>
#include <math.h>
#include "vectors.h"
#include "Operators.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

#define PI 3.14159
#define RAD_TO_DEG 180/3.14
#define DEG_TO_RAD 3.14f/180.f


double findDistance(double fromX, double fromY, double toX, double toY);

//void SAT(sf::ConvexShape& rect1, sf::ConvexShape& rect2);
//void CircleOnCircle(sf::CircleShape& circle1, sf::CircleShape& circle2);
bool RectangleOnRectangle(sf::RectangleShape& rect1, sf::RectangleShape& rect2);
bool RectangleOnCircle(sf::RectangleShape& rect, sf::CircleShape& circle);
//bool SAT(sf::RectangleShape& rect1, sf::RectangleShape& rect2);
//Axis ProjectOntoAxis(const sf::Vector2f boxPoints[], const sf::Vector2f normal);

#endif