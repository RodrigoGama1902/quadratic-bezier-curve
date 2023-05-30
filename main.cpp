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

    Point start_point;
    Point end_point;
    Point control_point;

    sf::CircleShape  startCircle;
    sf::CircleShape  endCircle;
    sf::CircleShape  controlCircle;

    QuadraticBezierCurve(const Point& startPoint, const Point& endPoint, const Point& controlPoint, int numPoints)
        : start_point(startPoint), end_point(endPoint), control_point(controlPoint) {
            
        startCircle = sf::CircleShape(10);
        startCircle.setFillColor(sf::Color::Yellow);
        startCircle.setOrigin(5, 5);

        endCircle = sf::CircleShape(10);
        endCircle.setFillColor(sf::Color::Blue);
        endCircle.setOrigin(5, 5);

        controlCircle = sf::CircleShape(10);
        controlCircle.setFillColor(sf::Color::Green);
        controlCircle.setOrigin(5, 5);

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

        window.draw(curveVertices);

        for (const auto& point : curvePoints) {
            sf::CircleShape circle = sf::CircleShape(4);
            circle.setFillColor(sf::Color::Red);
            circle.setOrigin(2, 2);
            circle.setPosition(point.x - 2, point.y - 2);
            window.draw(circle);
        }

        window.draw(startCircle);
        window.draw(endCircle);
        window.draw(controlCircle);
        

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

    void updateCurve(){
        updateCurve(start_point, end_point, control_point, numPoints);
    }



private:
    int numPoints;
    
    std::vector<Point> curvePoints;
    sf::VertexArray curveVertices;

};

int main() {

    Point start = { 50.0f, 50.0f };
    Point end = { 250.0f, 250.0f };
    Point control = { 150.0f, 20.0f };
    int numPoints = 100;

    QuadraticBezierCurve curve(start, end, control, numPoints);
    sf::RenderWindow window(sf::VideoMode(720, 1080), "Quadratic Bezier Curve");

    bool isDragging;
    Point *draggedPoint;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

                if (curve.startCircle.getGlobalBounds().contains(mousePos)) {
                    isDragging = true;
                    draggedPoint = &start;
                }
                else if (curve.endCircle.getGlobalBounds().contains(mousePos)) {
                    isDragging = true;
                    draggedPoint = &end;
                }
                else if (curve.controlCircle.getGlobalBounds().contains(mousePos)) {
                    isDragging = true;
                    draggedPoint = &control;
                }
            }

            if (event.type == sf::Event::MouseMoved && isDragging) {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                draggedPoint->x = mousePos.x;
                draggedPoint->y = mousePos.y;
            }

            if (event.type == sf::Event::MouseButtonReleased && isDragging) {
                isDragging = false;
            }

            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.delta > 0) {
                numPoints += 1;
            }

            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.delta < 0) {
                numPoints -= 1;
            }

        }

        curve.updateCurve(start, end, control, numPoints);

        window.clear();
        curve.draw(window);
        window.display();
    }

    return 0;
}
