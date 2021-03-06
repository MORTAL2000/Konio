#include "options.h"

Options::Options(){
	this->visible=0;
	this->music=new Value(30,70,15,"data/img/buttons/options/music.png");
	this->sound=new Value(50,70,15,"data/img/buttons/options/sound.png");
	this->close=new Button(70,70,15,"data/img/buttons/menu/close.png");
	this->move(gui.x,gui.y);
	this->zoom(gui.factor);
}
void Options::Render(sf::RenderWindow *window){
	this->music->Render(window);
	this->sound->Render(window);
	this->close->Render(window);
}
void Options::Update(){
	//Update
	if(this->close->left("Close","Click or press O to close options section.",sf::Keyboard::O))
		this->visible=0;
	if(
		music->Update(audio.musicVolume,0,100,1,1,"Music Volume",
		"Left click to decrease or right click to increase music volume.")
		||sound->Update(audio.soundVolume,0,100,1,1,"Sound Volume",
		"Left click to decrease or right click to increase sound volume.")
	)
		audio.Update();
}
void Options::show(){
	this->visible=1;
}
void Options::hide(){
	this->visible=0;
}
void Options::move(float x,float y){
	this->music->move(x,y);
	this->sound->move(x,y);
	this->close->move(x,y);
}
void Options::zoom(float factor){
	this->music->zoom(factor);
	this->sound->zoom(factor);
	this->close->zoom(factor);
}
void Options::setPosition(float x,float y){
	this->music->setPosition(x,y);
	this->sound->setPosition(x,y);
	this->close->setPosition(x,y);
}
bool Options::mouseOver(){
	if(this->music->mouseOver())
		return 1;
	if(this->sound->mouseOver())
		return 1;
	return this->close->mouseOver();
}
Options::operator bool(){
	return this->visible;
}
Options::~Options(){

}
//Global Variable
Options *op=NULL;