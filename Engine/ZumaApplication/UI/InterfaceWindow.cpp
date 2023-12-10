#include "InterfaceWindow.hpp"

int InterfaceWindow::nextID = 0;

InterfaceWindow::InterfaceWindow(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->name = "Iwin_" + std::to_string(nextID++);
}

void InterfaceWindow::setName(std::string newName)
{
	name = newName;
}
