#include<iostream>
#include<vector>
using namespace std;

//mother class Shape
class Shape {
protected:
	double* _mainParam;
public:

	Shape() {
		_mainParam = new double(0);
	}

	Shape(double parameter) {
		_mainParam = new double(parameter);
	}

	~Shape() {
		delete _mainParam;
		_mainParam = nullptr;
	}

	virtual double getVolume() = 0;
	virtual double getSurface() = 0;
	virtual void introduce() {
		cout << "I am a shape" << endl;
	};
};

//child class Square
class Square : public Shape {
private:
	string _comment;
public:
	Square(double side, string comment) : Shape(side), _comment(comment) {}

	Square(const Square& original) {
		_mainParam = new double(*original._mainParam);
		_comment = original._comment;
	}

	double getVolume() override {
		return 4 * (*_mainParam);
	}

	double getSurface() override {
		return (*_mainParam) * (*_mainParam);
	}

	void introduce() override {
		cout << "I am a square, and my side is: " << (*_mainParam) << endl;
		cout << "Comment while getting created: " << _comment<<endl;
	}

	friend ostream& operator<<(ostream& stream, Square& object)
	{
		stream << "I am a square, and my side is: " << *object._mainParam << endl;
		stream << "Volume: " << object.getVolume() << endl;
		stream << "Surface: " << object.getSurface() << endl;
		cout << "Comment while getting created: " << object._comment << endl;

		return stream;
	}

	Square& operator=(const Square& original) {
		delete _mainParam;
		_mainParam = new double(*original._mainParam);
		_comment = original._comment;
		return *this;
	}
};

//child class Circle
class Circle : public Shape {
private:
	string _comment;
	const double _pi = 3.14;
public:
	Circle(double radius, string comment) : Shape(radius), _comment(comment) {}

	Circle(const Circle& original) {
		_mainParam = new double(*original._mainParam);
		_comment = original._comment;
	}

	double getVolume() override {
		return  2 * (*_mainParam) * _pi;
	}

	double getSurface() override {
		return (*_mainParam) * (*_mainParam) * _pi;
	}

	void introduce() override {
		cout << "I am a circle, and my radius is: " << (*_mainParam) << endl;
		cout << "Comment while getting created: " << _comment<<endl;
	}

	friend ostream& operator<<(ostream& stream, Circle& object)
	{
		stream << "I am a circle, and my radius is: " << *object._mainParam << endl;
		stream << "Volume: " << object.getVolume() << endl;
		stream << "Surface: " << object.getSurface() << endl;
		cout << "Comment while getting created: " << object._comment << endl;

		return stream;
	}

	Circle& operator=(const Circle& original) {
		delete _mainParam;
		_mainParam = new double(*original._mainParam);
		_comment = original._comment;
		return *this;
	}
};

//template funciton for conversion
template<typename type>
type* convertShapePointer(Shape* item) {
	return dynamic_cast<type*>(item);
}

//custom container class
class CustomShapeContainer {
private:
	Shape** _array;
	int _size;
public:
	CustomShapeContainer() {
		_array = nullptr;
		_size = 0;
	}

	CustomShapeContainer(const CustomShapeContainer& original) {
		_array = new Shape* [original._size]{};
		for (int i = 0; i < original._size; i++) {
			if (convertShapePointer<Square>(original._array[i])) _array[i] = new Square(*convertShapePointer<Square>(original._array[i]));
			else if (convertShapePointer<Circle>(original._array[i])) _array[i] = new Circle(*convertShapePointer<Circle>(original._array[i]));
		}
		_size = original._size;
	}

	~CustomShapeContainer() {
		for (int i = 0; i < _size; i++) {
			delete _array[i];
			_array[i] = nullptr;
		}
		delete[] _array;
		_array = nullptr;
		_size = 0;
	}

	void push_back(Shape* newShape) {
		Shape** _newArray = new Shape * [_size + 1]{nullptr};
		for (int i = 0; i < _size; i++) _newArray[i] = _array[i];
		_newArray[_size] = newShape;
		delete[] _array;
		_array = _newArray;
		_size++;		
	}

	void operator+=(Shape* newShape) {
		this->push_back(newShape);
	}

	void print() {
		for (int i = 0; i < _size; i++) {
			if (convertShapePointer<Square>(_array[i])) cout << *convertShapePointer<Square>(_array[i]) << endl;
			else if (convertShapePointer<Circle>(_array[i])) cout << *convertShapePointer<Circle>(_array[i]) << endl;
		}
	}

	friend ostream& operator<<(ostream& stream, CustomShapeContainer& object)
	{
		for (int i = 0; i < object._size; i++) {
			if (convertShapePointer<Square>(object._array[i])) stream << *convertShapePointer<Square>(object._array[i]) << endl;
			else if (convertShapePointer<Circle>(object._array[i])) stream << *convertShapePointer<Circle>(object._array[i]) << endl;
		}
		return stream;
	}

	CustomShapeContainer& operator= (const CustomShapeContainer& original){
		for (int i = 0; i < _size; i++) {
			delete _array[i];
			_array[i] = nullptr;
		}
		delete[] _array;
		
		_array = new Shape* [original._size]{};
		for (int i = 0; i < original._size; i++) {
			if (convertShapePointer<Square>(original._array[i])) _array[i] = new Square(*convertShapePointer<Square>(original._array[i]));
			else if (convertShapePointer<Circle>(original._array[i])) _array[i] = new Circle(*convertShapePointer<Circle>(original._array[i]));
		}
		_size = original._size;
		
		return *this;
	}
};

int main() {

	//using built in vectors / containers
	cout << "BUILT IN VECTORS / CONTAINERS: " << endl << endl;

	vector<Shape*> shapes;
	shapes.push_back(new Square(3.5, "first square"));
	shapes.push_back(new Circle(4, "first circle"));
	shapes.push_back(new Square(2.5, "second square"));
	shapes.push_back(new Circle(6, "second circle"));

	//using overwritten method functions 
	cout << "----------------------------------------------------------------------------" << endl;
	cout << "USING OVERWRITTEN METHOD FUNCTIONS:" << endl;
	for (Shape* shape : shapes) {
		shape->introduce();
		cout << endl;
	}

	//using the operator<< functions
	//checking if the pointer can be converted to a child class pointer (if it is not nullptr)
	//and if so converting it, before calling its operator<<
	cout << "----------------------------------------------------------------------------" << endl;
	cout << "USING operator<< FUNCTIONS:" << endl;
	for (auto shape : shapes) {
		if(convertShapePointer<Square>(shape)) cout << *convertShapePointer<Square>(shape)<<endl;
		else if (convertShapePointer<Circle>(shape)) cout << *convertShapePointer<Circle>(shape)<<endl;
	}

	cout << "----------------------------------------------------------------------------" << endl;

	//using a custom build dynamic container
	cout << "USING A CUSTOM BUILD DYNAMIC CONTAINER: " << endl << endl;

	CustomShapeContainer customShapes;
	customShapes.push_back(new Square(3.5, "first square"));
	customShapes.push_back(new Circle(4, "first circle"));
	customShapes.push_back(new Square(3.5, "second square"));
	customShapes += new Circle(2, "second circle");

	cout << customShapes<<endl;

	cout << "----------------------------------------------------------------------------" << endl;
	//copy constructor
	cout << "COPY CONSTRUCTOR: " << endl << endl;
	CustomShapeContainer copy(customShapes);
	cout << copy << endl;

	cout << "----------------------------------------------------------------------------" << endl;
	//operator=
	cout << "OPERATOR=: " << endl << endl;
	CustomShapeContainer copy2;
	copy2 = customShapes;
	cout << copy2 << endl;

return 0;
}