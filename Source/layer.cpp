#include "layer.h"

Layer::Layer(short speed,short width,std::string directory){
	//Update variables
	this->speed=speed;
	this->clock=0;
	this->directory=directory;
	this->width=width;
	//Texture
	this->tex=new sf::Texture();
	this->tex->loadFromFile(this->directory,sf::IntRect(0,0,this->width,this->width));
	this->tex->setRepeated(true);
	this->tex->setSmooth(true);
	this->rectangle=new sf::RectangleShape();
	this->rectangle->setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height));
	this->rectangle->setTexture(this->tex);
	this->rectangle->setTextureRect(sf::IntRect(0,0,sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height));
}
//In game
void Layer::move(float x,float y){
	this->rectangle->move(x,y);
}
void Layer::zoom(float factor){
	this->move(
		-this->rectangle->getSize().x/2*factor+this->rectangle->getSize().x/2,
		-this->rectangle->getSize().y/2*factor+this->rectangle->getSize().y/2
	);
	this->rectangle->setSize(sf::Vector2f(this->rectangle->getSize().x*factor,this->rectangle->getSize().y*factor));
	this->rectangle->setTextureRect(sf::IntRect(0,0,this->rectangle->getSize().x,this->rectangle->getSize().y));
}
void Layer::Update(){
	this->clock=(this->clock+speed)%(this->width*2);
	this->tex->loadFromFile(this->directory,sf::IntRect(abs(clock),0,this->width*2,this->width));
}
void Layer::Reset(){
	this->clock=0;
}
//Render
void Layer::Render(sf::RenderWindow *window){
	window->draw(*this->rectangle);
}
//Destructor
Layer::~Layer(){

}