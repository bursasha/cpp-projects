#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <initializer_list>

using namespace std;

struct Point
{
	int x;
	int y;
};

class AbstractShape
{
public:
	virtual bool occupiesPixel(Point p) const = 0;
	virtual void print(ostream &os) const = 0;
	// virtual 

};

ostream &operator << (ostream &os, const AbstractShape &shape)
{
	shape.print(os);
	return os;
}

/** @brief Represents a line segment by the two endpoints */
class LineSegment : public AbstractShape
{
	Point m_start;
	Point m_end;
	double m_k;
	double m_q;

public:
	LineSegment(Point start, Point end)
		 : m_start(start), m_end(end), m_k(static_cast<double>(m_start.y - m_end.y) / (m_start.x - m_end.x)) // compute k and q in $$y = k * x + q$$
			,
			m_q(m_end.y - m_k * m_end.x)
	{
	}

	/** @brief point must be on the line but not outside the segment */
	bool occupiesPixel(Point p) const
	{
		auto xBounds = std::min(m_start.x, m_end.x) <= p.x && p.x <= std::max(m_start.x, m_end.x);
		auto yBounds = std::min(m_start.y, m_end.y) <= p.y && p.y <= std::max(m_start.y, m_end.y);
		return xBounds && yBounds && p.y == m_k * p.x + m_q;
	}

	void print(ostream &os) const override
	{
		os << "(Line " << m_start << m_end << ")";
	}
};

/** @brief Represents a rectangle with the top-left point and width and height */
class Rectangle : public AbstractShape
{
	Point m_point;
	int m_w, m_h;

public:
	Rectangle(Point point, int w, int h)
		 : m_point(point), m_w(w), m_h(h)
	{
	}

	/** @brief point must be inside the rectangle */
	bool occupiesPixel(Point p) const
	{
		return m_point.x <= p.x && p.x <= m_point.x + m_w && m_point.y <= p.y && p.y <= m_point.y + m_h;
	}

	void print(ostream &os) const override
	{
		os << "(Rectangle " << m_point << " w=" << m_w << " h=" << m_h << ")";
	}
};

/** @brief Represents a circle with the center and radius */
class Circle : public AbstractShape
{
	Point m_center;
	int m_radius;

public:
	Circle(Point center, int radius)
		 : m_center(center), m_radius(radius)
	{
	}

	/** @brief point must be inside the circle */
	bool occupiesPixel(Point p) const
	{
		// the distance between the center and p must be less or equal the radius (pythagoras theorem)
		return (p.x - m_center.x) * (p.x - m_center.x) + (p.y - m_center.y) * (p.y - m_center.y) <= m_radius * m_radius;
	}

	void print(ostream &os) const override
	{
		os << "(Circle " << m_center << " r=" << m_radius << ")";
	}
};

void test(const AbstractShape &shape, initializer_list<Point> valid, initializer_list<Point> invalid)
{
	// std::cout << shape << std::endl;
	assert(std::all_of(valid.begin(), valid.end(), [&shape](const auto &point)
							 { return shape.occupiesPixel(point); }));
	assert(std::none_of(invalid.begin(), invalid.end(), [&shape](const auto &point)
							  { return shape.occupiesPixel(point); }));
}

int main()
{
	Circle c({10, 10}, 3);
	test(c, {{10, 13}, {10, 11}, {10, 10}}, {{18, 13}});

	Rectangle r({0, 0}, 3, 5);
	test(r, {{0, 0}, {2, 2}, {3, 4}}, {{4, 3}});

	LineSegment l({0, 0}, {5, 5});
	test(l, {{1, 1}, {0, 0}}, {{-1, -1}, {6, 6}});

#if 0 // remove for ComposedShape tests
    ComposedShape* pack = new ComposedShape();
    pack->add(c).add(r).add(l);
    test(*pack, {{10, 13}, {10, 11}, {10, 10}, {0, 0}, {2, 2}, {3, 4}, {1, 1}, {0, 0}}, {{18, 13}, {4, 3}, {-1, -1}, {26, 26}});

    ComposedShape pack2(*pack);
    delete pack;

    test(pack2, {{10, 13}, {10, 11}, {10, 10}, {0, 0}, {2, 2}, {3, 4}, {1, 1}, {0, 0}}, {{18, 13}, {4, 3}, {-1, -1}, {26, 26}});
#endif

	return 0;
}
