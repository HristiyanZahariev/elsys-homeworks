#include <iostream>
#include <list>
#include <string>

using namespace std;

class Point {
    int x_, y_;
    
    public:
    
    Point(int x=0, int y=0) : x_(x), y_(y) {}
    
    int get_x() const {
        return x_;
    }
    
    int get_y() const {
        return y_;
    }
};

class Shape {
    public:
    
    virtual void draw() const = 0;
};

class Circle : public Shape {
    int radius_;
    Point center_;
    
    public:
    
    Circle(Point center, int radius): center_(center), radius_(radius) {}
    
    void draw() const {
        cout << "<circle cx=\"" << center_.get_x()
            << "\" cy=\"" << center_.get_y()
            << "\" r=\"" << radius_ << "\" />"
            << endl;
    }
};

class Ellipse : public Shape {
	Point center_point_;
	int width_, height_;

	public:

	Ellipse(Point center_point, int width, int height) : center_point_(center_point), width_(width), height_(height) {}

	void draw() const {
		cout << "<ellipse x=\"" << center_point_.get_x()
			<< "\" y=\"" << center_point_.get_y()
			<< "\" w=\"" << width_ 
			<< "\" h=\"" << height_ << "\" />"
			<< endl; 
	}
};

class Rectangle : public Shape {
	Point start_point_;
	int width_, height_;
    
	public:

	Rectangle(Point start_point, int width, int height) : start_point_(start_point), width_(width), height_(height) {}

	void draw() const {
		cout << "<rect x=\"" << start_point_.get_x()
			    << "\" y=\"" << start_point_.get_y()
                << "\" w=\"" << width_ 
                << "\" h=\"" << height_ << "\" />"
			<< endl;
	}

};

class Line : public Shape {
	Point start_point_;
	Point end_point_;

	public:

	Line(Point start_point, Point end_point) : start_point_(start_point), end_point_(end_point) {}

	void draw() const {
		cout << "<line x1=\"" << start_point_.get_x()
			<< "\" y1=\"" << start_point_.get_y()
			<< "\" x2=\"" << end_point_.get_x() 
			<< "\" y2=\"" << end_point_.get_y() << "\" />"
			<< endl;	
	}
};

class Polygon : public Shape {
    list<Point> point_;
    
    public:
    
    Polygon(list<Point> point) : point_(point) {}
    
    void draw() const {
        cout << "<polygon points=\"";
        for(list<Point>::const_iterator it = point_.begin(); it != point_.end(); it++) {
            cout << (*it).get_x() << "," << (*it).get_y() << " ";
        }
        cout <<"\" />"
        << endl;
    }
    
};

class Polyline : public Shape { 
    list<Point> point_;
    
    public:
    
    Polyline(list<Point> point) : point_(point) {}
    
    void draw() const {
        cout << "<polyline points=\"";
        for(list<Point>::const_iterator it = point_.begin(); it != point_.end(); it++) {
            cout << (*it).get_x() << "," << (*it).get_y() << " ";
        }
        cout <<"\" style=\"fill:none;stroke:black;stroke-width:3\" />"
        << endl;
    }  
};

class Path : public Shape {
    string commands_;
    
    public:

    Path(string commands) : commands_(commands) {}
    
    void draw() const {
        cout << "<path d=\"" << commands_
        << "\" />" 
        << endl;
    }
};

class CompositeFigure : public Shape {
    list<Shape*> content_;
    
    public:
    
    ~CompositeFigure() {
        for(list<Shape*>::iterator it = content_.begin(); it != content_.end(); it++) {
            delete *it;
        }
    }
    
    void add(Shape* shape) {
        content_.push_back(shape);
    }
    
    void draw() const {
        for(list<Shape*>::const_iterator it = content_.begin(); it != content_.end(); it++) {
            (*it)->draw();
        }
    }
};

class Canvas : public CompositeFigure {
    int width_, height_;
    public:
    
    Canvas (int width = 100, int height = 100) : width_(width), height_(height) {}
    
    void draw() const {
        cout << "<svg width=\"" << width_ 
        << "\" height=\"" << height_ 
        << "\">" << endl;
        CompositeFigure::draw();
        cout << "</svg>" << endl;
    }  
};

int main() {
    Canvas c(1000, 1000);
    c.add(new Circle(Point(100, 100), 20)); 
    c.add(new Ellipse(Point(200, 200), 100, 30));
    c.add(new Rectangle(Point(300, 300), 300, 200));  
    c.add(new Line(Point(400,400), Point(500,500)));
    
    list<Point> polygon;
    polygon.push_back(Point(100,100));
    polygon.push_back(Point(240,240));
    polygon.push_back(Point(420,420));
    polygon.push_back(Point(690,100));
    
    list<Point> polyline;
    polyline.push_back(Point(69,690));
    polyline.push_back(Point(42,420));
    polyline.push_back(Point(66,660));
    
    c.add(new Polygon(polygon));
    c.add(new Polyline(polyline));
    
    c.add(new Path("M150 0 L75 200 L225 200 Z"));
    
    c.draw();

    return 0;
}