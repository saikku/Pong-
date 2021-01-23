#include "collisions.h"

double findDistance(double fromX, double fromY, double toX, double toY)
{
    double a = std::abs(fromX - toX);
    double b = std::abs(fromY - toY);

    return std::sqrt((a * a) + (b * b));
}
bool RectangleOnRectangle(sf::RectangleShape& rect1, sf::RectangleShape& rect2)
 {//ilman rotatiota toimiva collision kahden suorakulman kanssa

     float distance = (rect1.getPosition().x < rect2.getPosition().x + rect2.getSize().x &&
         rect1.getPosition().x + rect1.getSize().x > rect2.getPosition().x &&
         rect1.getPosition().y < rect2.getPosition().y + rect2.getSize().y &&
         rect1.getPosition().y + rect1.getSize().y > rect2.getPosition().y);

     if (distance) 
     {
         return true;
         //rect1.setFillColor(sf::Color::Green);
         //rect1.move(sf::Vector2f(-2, 0));
         //rect2.move(sf::Vector2f(2, 0));
         //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) rect2.move(0, 2);
         //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rect2.move(2, 0);
         //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) rect2.move(0, -2);
         //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rect2.move(-2, 0);
     }
     else
     {
         return false;
         //rect1.setFillColor(sf::Color::Red);
     }
}//AABB
bool RectangleOnCircle(sf::RectangleShape& rect, sf::CircleShape& circle)
{
    double rectCenterX = rect.getOrigin().x + rect.getSize().x / 2.0;
    double rectCenterY = rect.getOrigin().y + rect.getSize().y / 2.0;

    double rectRotation = rect.getRotation() * 3.14 / 180;
    //rect.setOrigin(sf::Vector2f(20, 20));
    double rectX = rect.getPosition().x;
    double rectY = rect.getPosition().y;

    double rectWidht = rect.getSize().x;
    double rectHeight = rect.getSize().y;

    double circlePosX = circle.getPosition().x;
    double circlePosY = circle.getPosition().y;
    //poista *DEG_TO_RAD vaadittaessa
    //Uncircle = unrotated                                    //pit�� vaihtaa keskelle.. t�ll� hetkell� reuna               keksi� kuinka saadaan rect center piste
    double UnCirclex = std::cos(rectRotation) * (circlePosX - rectCenterX) - std::sin(rectRotation) * (circlePosY - rectCenterY) + rectCenterX;
    double UnCircley = std::sin(rectRotation) * (circlePosX - rectCenterX) + std::cos(rectRotation) * (circlePosY - rectCenterY) + rectCenterY;

    // Closest point in the rectangle to the center of circle rotated backwards(unrotated)
    double closestX, closestY;

    // Find the unrotated closest x point from center of unrotated circle

    if (UnCirclex < rectX)
    {
        closestX = rectX;
    }
    else if (UnCirclex > rectX + rectWidht)
    {
        closestX = rectX + rectWidht;
    }
    else
    {
        closestX = UnCirclex;
    }
    // Find the unrotated closest y point from center of unrotated circle
    if (UnCircley < rectY)
    {
        closestY = rectY;
    }
    else if (UnCircley > rectY + rectHeight)
    {
        closestY = rectY + rectHeight;
    }
    else
    {
        closestY = UnCircley;
    }
    // Determine collision
    bool collision = false;
    double distance = findDistance(UnCirclex, UnCircley, closestX, closestY);

    //std::cout << distance << std::endl;
    if (distance < circle.getRadius())
    {
        collision = true; // Collision
        return true;
    }
    else
    {
        collision = false;
        return false;
    }
}

//void CircleOnCircle(sf::CircleShape& circle1, sf::CircleShape& circle2)
//{
//    float dx = circle1.getPosition().x - circle2.getPosition().x;
//    float dy = circle1.getPosition().y - circle2.getPosition().y;
//    float distance = std::sqrt(dx * dx + dy * dy);
//    //std::cout << distance << std::endl;

//    if (distance < circle1.getRadius() + circle2.getRadius())
//    {
//        circle1.setFillColor(sf::Color::Green);
//    }
//    if (distance > circle1.getRadius() + circle2.getRadius())
//    {
//        circle1.setFillColor(sf::Color::Red);
//    }
//}//AABB
// bool SAT(sf::RectangleShape& rect1,sf::RectangleShape& rect2)
// {
//     //Step 1: Get the normals of both boxes!
//     sf::Vector2f boxANormals[4];
//     GetNormals(rect1, boxANormals);

//     sf::Vector2f boxBNormals[4];
//     GetNormals(rect2, boxBNormals);

//     //Normals to project our vertices in;
//     //Pick 2 normals from each boxes
//     sf::Vector2f normalsToTest[4];
//     normalsToTest[0] = boxANormals[0];
//     normalsToTest[1] = boxANormals[1];
//     normalsToTest[2] = boxBNormals[2];
//     normalsToTest[3] = boxBNormals[3];

//     //Step 2: Get the points to project onto the axes!
//     sf::Vector2f boxAPoints[4];
//     boxAPoints[0] = rect1.getTransform().transformPoint(sf::Vector2f(0.0f, 0.0f));
//     boxAPoints[1] = rect1.getTransform().transformPoint(sf::Vector2f(rect1.getSize().x, 0.0f));
//     boxAPoints[2] = rect1.getTransform().transformPoint(sf::Vector2f(rect1.getSize().x, rect1.getSize().y));
//     boxAPoints[3] = rect1.getTransform().transformPoint(sf::Vector2f(0.0f, rect1.getSize().y));

//     sf::Vector2f boxBPoints[4];
//     boxBPoints[0] = rect2.getTransform().transformPoint(sf::Vector2f(0.0f, 0.0f));
//     boxBPoints[1] = rect2.getTransform().transformPoint(sf::Vector2f(rect2.getSize().x, 0.0f));
//     boxBPoints[2] = rect2.getTransform().transformPoint(sf::Vector2f(rect2.getSize().x, rect2.getSize().y));
//     boxBPoints[3] = rect2.getTransform().transformPoint(sf::Vector2f(0.0f, rect2.getSize().y));
//     //Step 3: Identify the max & min projection points - The Bounds of a line (axis)
//     Axis a = ProjectOntoAxis(boxAPoints, normalsToTest[0]);
//     Axis b = ProjectOntoAxis(boxAPoints, normalsToTest[1]);
//     Axis c = ProjectOntoAxis(boxAPoints, normalsToTest[2]);
//     Axis d = ProjectOntoAxis(boxAPoints, normalsToTest[3]);

//     Axis e = ProjectOntoAxis(boxBPoints, normalsToTest[0]);
//     Axis f = ProjectOntoAxis(boxBPoints, normalsToTest[1]);
//     Axis g = ProjectOntoAxis(boxBPoints, normalsToTest[2]);
//     Axis h = ProjectOntoAxis(boxBPoints, normalsToTest[3]);

//     //Step 4: Check for any overlaps 
//     if (
//         a.maxProjection < e.minProjection ||
//         e.maxProjection < a.minProjection ||
//         b.maxProjection < f.minProjection ||
//         f.maxProjection < b.minProjection ||
//         c.maxProjection < g.minProjection ||
//         g.maxProjection < c.minProjection ||
//         d.maxProjection < h.minProjection ||
//         h.maxProjection < d.minProjection)
//     {
//         return false;
//     }
//     else
//     {
//     return true;
//     }
// }

// Axis ProjectOntoAxis(const sf::Vector2f boxPoints[], const sf::Vector2f normal)
// {
//     float minProjection = DotProduct(boxPoints[0], normal);
//     float maxProjection = minProjection;

//     for (int i = 0; i < 4; i++)
//     {
//         float currentProjection = DotProduct(boxPoints[i], normal);
//         //Is it a max or min projection on the axis?
//         if (minProjection > currentProjection)	//For min
//             minProjection = currentProjection;
//         if (currentProjection > maxProjection)  //For max
//             maxProjection = currentProjection;
//     }
//     Axis temp;
//     temp.maxProjection = maxProjection;
//     temp.minProjection = minProjection;
//     return temp;
// }