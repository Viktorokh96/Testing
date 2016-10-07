#include <iostream>
#include <list>
#include <ncurses.h>

class Point {		/* Точка */
	int _x, _y;
public:
	Point( int x, int y ) { _x = x; _y = y; }
	int GetX() { return _x; }
	int GetY() { return _y; }
};

class Rect {		/* Прямоугольник */ 
	Point _leftUpperCorner;
	Point _rightLownerCorner;
public:
	Rect ( int x1, int y1, int x2, int y2 ) :
	       	_leftUpperCorner( x1, y1 ), _rightLownerCorner( x2, y2 ) {} 
	int GetHeight() { return _leftUpperCorner.GetY() - _rightLownerCorner.GetY(); } // Возврат высоты прямоугольника
	int GetWidth() { return _rightLownerCorner.GetX() - _leftUpperCorner.GetX(); }	// Возврат ширины прямоугольникаё
	Point GetLeftUpper() { return _leftUpperCorner; }				// Возврат вершин
	Point GetRightLowner() { return _rightLownerCorner; }
};

class Snake {					// Класс Змея
public:
	void Move();				// Двинутся на одну клетку
	enum MoveDirection { left, up, right, down };
	void Turn( MoveDirection dir );		// Изменить направление движения
private:
	MoveDirection _mvdir;
	Point _head;
};

class Environment {							// Класс окружение
public:
	virtual bool Crossed( Point coord ) = 0;			// Пересекает ли координата данное окружение?
	virtual void Draw() = 0;					// Отобразить окружение
};

class EnvironmentList : public Environment {				// Список окружений
	std::list <Environment *> _envList; 
public:
	EnvironmentList() { _envList.clear(); }
	~EnvironmentList() { _envList.clear(); }

	virtual bool Crossed( Point coord );
	virtual void Draw();
	void Register ( Environment *env ) { _envList.push_back(env); }
};

bool EnvironmentList::Crossed( Point coord )
{
	std::list <Environment *>::
		iterator it = _envList.begin();

	for(; it != _envList.end(); ++it)
		if((*it)->Crossed( coord )) 
			return true;

	return false;
}

void EnvironmentList::Draw()
{
	std::list <Environment *>::
		iterator it = _envList.begin();

	for(; it != _envList.end(); ++it)
		(*it)->Draw();
}


class GameArea {
	Rect _area;
	Snake _player;
	EnvironmentList _env;
pubclic:
	void InitGame() {} 
	void StartGame() {} 
	void EndGame() {} 
};

int main()
{

	return 0;
}
