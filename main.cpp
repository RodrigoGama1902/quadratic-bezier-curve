#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    float x;
    float y;
};

class QuadraticBezierCurve {
public:

    QuadraticBezierCurve(const Point& startPoint, const Point& endPoint, const Point& controlPoint, int numPoints)
        : start_point(startPoint), end_point(endPoint), control_point(controlPoint) {
            
        for (const auto& point : curvePoints) {
            curveVertices.append(sf::Vertex(sf::Vector2f(point.x, point.y)));
        }

        startCircle = sf::CircleShape(5);
        startCircle.setFillColor(sf::Color::Red);

        endCircle = sf::CircleShape(5);
        endCircle.setFillColor(sf::Color::Blue);

        controlCircle = sf::CircleShape(5);
        controlCircle.setFillColor(sf::Color::Green);
 
        updateCurve(startPoint, endPoint, controlPoint, numPoints);
    }

    std::vector<Point> generateCurve(int numPoints) {
        std::vector<Point> points;
        float step = 1.0f / (numPoints - 1);

        for (int i = 0; i < numPoints; i++) {
            float t = i * step;
            float u = 1.0f - t;

            float x = u * u * start_point.x + 2.0f * u * t * control_point.x + t * t * end_point.x;
            float y = u * u * start_point.y + 2.0f * u * t * control_point.y + t * t * end_point.y;

            Point point = { x, y };
            points.push_back(point);
        }

        return points;
    }

    void draw(sf::RenderWindow &window){

        window.draw(startCircle);
        window.draw(endCircle);
        window.draw(controlCircle);
        window.draw(curveVertices);

    }

    void updateCurve(const Point& startPoint, const Point& endPoint, const Point& controlPoint, int numPoints){

        start_point = startPoint;
        end_point = endPoint;
        control_point = controlPoint;

        curvePoints = generateCurve(numPoints);
        curveVertices = sf::VertexArray(sf::LineStrip);

        for (const auto& point : curvePoints) {
            curveVertices.append(sf::Vertex(sf::Vector2f(point.x, point.y)));
        }

        startCircle.setPosition(start_point.x - 5, start_point.y - 5);
        endCircle.setPosition(end_point.x - 5, end_point.y - 5);
        controlCircle.setPosition(control_point.x - 5, control_point.y - 5);
    }

private:
    int numPoints;
    
    Point start_point;
    Point end_point;
    Point control_point;

    std::vector<Point> curvePoints;
    sf::VertexArray curveVertices;

    sf::CircleShape  startCircle;
    sf::CircleShape  endCircle;
    sf::CircleShape  controlCircle;

};

int main() {

    Point start = { 50.0f, 50.0f };
    Point end = { 250.0f, 250.0f };
    Point control = { 150.0f, 20.0f };

    QuadraticBezierCurve curve(start, end, control, 100);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Quadratic Bezier Curve");


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        curve.draw(window);
        window.display();
    }

    return 0;
}
