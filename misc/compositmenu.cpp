#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <ncurses.h>

class MenuComponent {
public:
	virtual ~MenuComponent() {};
	const char *Name() { return _name; }
	MenuComponent *Parent() { return _parent; }
	void SetParent( MenuComponent *parent ) { _parent = parent; }

	virtual void Activate() = 0;
	virtual void Disactivate() = 0;
	virtual void Draw( int x, int y ) = 0;
	virtual void Hide() = 0;

	virtual void Add( MenuComponent * ) = 0;
	virtual void Remove( MenuComponent * ) = 0;
	virtual std::list<MenuComponent *>::iterator BeginIter()
		{ throw "NullIterator exception!"; }
	virtual std::list<MenuComponent *>::iterator EndIter()
		{ throw "NullIterator exception!"; }
	virtual bool IsComposite() { return false; }
protected:
	MenuComponent( const char *name ) { 
			_name = name; 
			_active = false; 
			_x = _y = 0; 
			_parent = NULL; 
	}
	bool _active;
	int _x, _y;
private:
	MenuComponent *_parent;
	const char *_name;
};

class CompositeMenu : public MenuComponent {
public:
	virtual ~CompositeMenu();

	virtual void Activate();
	virtual void Disactivate(); 
	virtual void Draw( int x, int y ) = 0;
	virtual void Hide() = 0;

	virtual void Add( MenuComponent *);
	virtual void Remove( MenuComponent *);
	virtual std::list<MenuComponent *>::iterator BeginIter()
		{ return _items.begin(); }
	virtual std::list<MenuComponent *>::iterator EndIter()
		{ return _items.end(); } 
	virtual bool IsComposite() { return true; }
protected:
	CompositeMenu(const char *name) : MenuComponent(name)
		{ _items.clear(); } 
private:
	std::list<MenuComponent *> _items;
};

CompositeMenu::~CompositeMenu()
{
	std::list<MenuComponent *> :: iterator iter;
	while ( iter != EndIter() ) {
		iter = BeginIter();
		if (iter != EndIter()) {
			delete *iter;
			_items.erase(iter);
		}
	}

}

void CompositeMenu::Activate() 
{
	_active = true;
}

void CompositeMenu::Disactivate() 
{
	_active = false;

	std::list<MenuComponent *> :: iterator iter = BeginIter();
	for (; iter != EndIter(); iter++)
		(*iter)->Hide();
}

void CompositeMenu::Add( MenuComponent *component)
{
	component->SetParent( this );
	_items.push_back(component);
}

void CompositeMenu::Remove( MenuComponent *component)
{
	std::list<MenuComponent *> :: iterator iter;
	iter = BeginIter();
	for (; iter != EndIter(); iter++)
		if (*iter == component) {
			delete component;
			_items.erase(iter);
			return;
		}
}

/************************************************************/

class Window {
public:
	void DrawText( int, int, const char *);
	void DrawBoldText ( int, int, const char *);
	void ClearScreen() { erase(); }
} *window;

void Window::DrawText( int x, int y, const char *text) 
{
	mvprintw(y, x, text);
}

void Window::DrawBoldText( int x, int y, const char *text)
{
	attron(A_BOLD);
	DrawText(x,y,text);
	attroff(A_BOLD);
}

class Menu : public CompositeMenu {
public:
	Menu( const char *name ) : CompositeMenu(name) {} 

	virtual void Draw( int x, int y );
	virtual void Hide() {}
};

void Menu::Draw ( int x, int y )
{
	( _active ) ? window->DrawBoldText(x,y, Name())
		    : window->DrawText(x,y, Name());

	if ( _active ) { 
		std::list<MenuComponent *> :: iterator iter = BeginIter();
		for (int i = 0; iter != EndIter(); iter++, i++)
			(*iter)->Draw( x+15, y+i );
	}

}

typedef int cmdId;

class MenuItem : public MenuComponent {
public:
	MenuItem(const char *name, cmdId cmd)
		:MenuComponent(name) { _cmd = cmd; }
	virtual ~MenuItem() {} 

	virtual void Activate()	{ 
			_active = true; 
	       	}
	virtual void Disactivate() { _active = false; }
	virtual void Draw( int x, int y );
	virtual void Hide() {}

	virtual void Add( MenuComponent *) { throw "Component leaf Add exception"; } 	
	virtual void Remove( MenuComponent *) { throw "Component leaf Remove exception"; } 	
private:
	cmdId _cmd;
};

void MenuItem::Draw( int x, int y )
{
	( _active ) ? window->DrawBoldText(x,y, Name())
		    : window->DrawText(x,y, Name());
}

void StartControl(Menu *menu)
{
	std::list<MenuComponent *> 
		:: iterator start, end, iter;
	start = menu->BeginIter();
	end = --menu->EndIter();
	iter = start;

	menu->Activate();

	(*iter)->Activate();

	char ch;
	bool exit = false;
	while(!exit) {
		menu->Draw(15, 15);
		ch = getch();
		switch(ch) {
			case 's':
				(*iter)->Disactivate();
				if(iter != end)
					iter++;
				else
					iter = start;
				(*iter)->Activate();
				break;
			case 'w':
				(*iter)->Disactivate();
				if (iter != start)
					iter--;
				else
					iter = end;
				(*iter)->Activate();
				break;
			case 'd':
				if((*iter)->IsComposite()) {
					start = (*iter)->BeginIter();
					end = --(*iter)->EndIter();
					iter = start;
					(*iter)->Activate();
				}
				break;
			case 'a':
				if (!((*iter)->Parent()->Parent())) break;
				(*iter)->Disactivate();
				(*iter)->Parent()->Disactivate();

				start = (*iter)->Parent()->Parent()->BeginIter();
				end = --(*iter)->Parent()->Parent()->EndIter();
				iter = start;

				(*iter)->Activate();
				break;
			case 'q':
				(*iter)->Disactivate();
				exit = true;
		}
		window->ClearScreen();
	}
}

void InitCurses()
{
	initscr();
	cbreak();
	keypad(stdscr, true);
	curs_set(0);
	erase();
}

void QuitCurses()
{
	endwin();
}

int main ()
{
	InitCurses();

	Menu *Faculty = new Menu("");
	Menu *Departments = new Menu("Departments>");
	Menu *Groups = new Menu("Groups>");

	Menu *TeachersSS = new Menu("Solid-state>");
	Menu *TeachersIT = new Menu("IT-technology>");

	Menu *Students312 = new Menu("21312>");
	Menu *Students313 = new Menu("21313>");
	Menu *Students318 = new Menu("21318>");
	Menu *Students314 = new Menu("21314>");

	TeachersSS->Add(new MenuItem("Klimon Igor Viktorovich", 0));
	TeachersSS->Add(new MenuItem("Boriskov", 0));
	TeachersSS->Add(new MenuItem("Sysun", 0));

	TeachersIT->Add(new MenuItem("Pikulev Vitaly Borisovich", 0));
	TeachersIT->Add(new MenuItem("Kogochev", 0));

	Students312->Add(new MenuItem("Okhotnikov Viktor",0));
	Students312->Add(new MenuItem("Philipov Egor",0));
	Students312->Add(new MenuItem("DV:company",0));
	Students312->Add(new MenuItem("Kazakov Artem",0));
	Students312->Add(new MenuItem("Efimov Georgii",0));

	Students313->Add(new MenuItem("Kuzin Konstantin",0));
	Students313->Add(new MenuItem("Elena Mackovich",0));

	Students318->Add(new MenuItem("Elena Geveiler",0));
	Students318->Add(new MenuItem("Alina Danilova",0));

	Students314->Add(new MenuItem("Rahman",0));

	Departments->Add(TeachersSS);
	Departments->Add(TeachersIT);

	Groups->Add(Students312);
	Groups->Add(Students313);
	Groups->Add(Students318);
	Groups->Add(Students314);

	Faculty->Add(Departments);
	Faculty->Add(Groups);

	StartControl(Faculty);

	delete Faculty;

	QuitCurses();

	return 0;
}
