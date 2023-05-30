#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    float x;
    float y;
};

const sf::Color BACKGROUND_COLOR = sf::Color(150, 197, 247);
const sf::Color LINE_COLOR = sf::Color(238, 229, 191);
const sf::Color POINT_COLOR = sf::Color(105, 109, 125);
const sf::Color SEGMENT_POINT_COLOR = sf::Color::White;

class QuadraticBezierCurve {
public:

    Point start_point;
    Point end_point;
    Point control_point;

    sf::CircleShape  startCircle;
    sf::CircleShape  endCircle;
    sf::CircleShape  controlCircle;

    float lineThickness;
    sf::Color lineColor;

    QuadraticBezierCurve(const Point& startPoint, const Point& endPoint, const Point& controlPoint, int numPoints)
        : start_point(startPoint), end_point(endPoint), control_point(controlPoint) {
            
        startCircle = sf::CircleShape(10);
        startCircle.setOrigin(5, 5);

        controlCircle = sf::CircleShape(10);
        controlCircle.setOrigin(5, 5);

        endCircle = sf::CircleShape(10);
        endCircle.setOrigin(5, 5);

        lineThickness = 5.0f;
        
        // Custom RGB Color
        lineColor = sf::Color(LINE_COLOR);
        startCircle.setFillColor(POINT_COLOR);
        controlCircle.setFillColor(POINT_COLOR);
        endCircle.setFillColor(POINT_COLOR);
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

        // Draw line
        for (const auto& line : generateShapeLine(curvePoints)) {
            window.draw(line);
        }

        // Draw Line Points
        for (const auto& point : curvePoints) {
            sf::CircleShape circle = sf::CircleShape(4);
            circle.setFillColor(SEGMENT_POINT_COLOR);
            circle.setOrigin(2, 2);
            circle.setPosition(point.x - 2, point.y - 2);
            window.draw(circle);
        }

        // Draw Control Points
        window.draw(startCircle);
        window.draw(endCircle);
        window.draw(controlCircle);
    
    }

    std::vector<sf::RectangleShape> generateShapeLine(const std::vector<Point>& points) {
        // Creates an array of rectangles that form a line between the points

        std::vector<sf::RectangleShape> rectangles;

        if (points.size() < 2) {
            return rectangles;
        }

        for (std::size_t i = 1; i < points.size(); ++i) {
            const Point& p1 = points[i - 1];
            const Point& p2 = points[i];

            sf::RectangleShape rectangle;
            rectangle.setPosition(p1.x, p1.y);

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            float length = std::sqrt(dx * dx + dy * dy);

            rectangle.setSize(sf::Vector2f(length, lineThickness));
            rectangle.setFillColor(lineColor);

            // Calculate the rotation angle
            float angle = std::atan2(dy, dx) * 180.0f / static_cast<float>(M_PI);
            rectangle.setRotation(angle);
            rectangle.setOrigin(0.0f, lineThickness / 2.0f);

            rectangles.push_back(rectangle);
        }

        return rectangles;
    }

    void updateCurve(const Point& startPoint, const Point& endPoint, const Point& controlPoint, int numPoints){

        start_point = startPoint;
        end_point = endPoint;
        control_point = controlPoint;

        curvePoints = generateCurve(numPoints);
    
        startCircle.setPosition(start_point.x - 5, start_point.y - 5);
        endCircle.setPosition(end_point.x - 5, end_point.y - 5);
        controlCircle.setPosition(control_point.x - 5, control_point.y - 5);
    }

private:
    int numPoints;
    
    std::vector<sf::RectangleShape> shapeLine;
    std::vector<Point> curvePoints;

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

        window.clear(BACKGROUND_COLOR);
        curve.draw(window);
        window.display();
    }

    return 0;
}
