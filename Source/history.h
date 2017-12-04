#ifndef HISTORY_H
#define HISTORY_H
#include "button.h"
#include "background.h"
#include "label.h"

class History{
private:
	Label *info;
	Button *menu;
public:
	History();
	void Render(sf::RenderWindow *window);
	void Update();
	~History();
}extern *history;
#endif