#include "naval.h"

//Constructor
Naval::Naval(std::string directory){
	//Read data from file
	std::ifstream in(directory+".txt");
	in>>this->basic;
	in>>this->rowers;
	in>>this->marines;
	in>>this->cost;
	in.close();
	//Load texture
	this->body.loadFromFile(directory+".png");
	this->ram.loadFromFile(directory+"_ram.png");
	this->rows.loadFromFile(directory+"_rows.png");
	this->body.setSmooth(true);
	this->ram.setSmooth(true);
	this->rows.setSmooth(true);
}
//Get data
std::string Naval::Name(){
	return this->basic.name;
}
std::string Naval::Description(){
	return "Cost: "+Format(this->cost)+" coins    Upkeep: "+Format(this->Upkeep())+" coins    Speed: "+Format(this->basic.speed)+" km/h with "+Format(this->rowers)+" rowers and "+Format(this->marines)+" marines";
}
float Naval::Speed(){
	return this->basic.speed;
}
float Naval::Cost(){
	return this->cost;
}
float Naval::Upkeep(){
	return this->rowers/500.+this->marines/300.;
}
float Naval::Power(){
	return this->basic.speed*this->rowers;
}
short Naval::Rowers(){
	return this->rowers;
}
short Naval::Marines(){
	return this->marines;
}
short Naval::Men(){
	return this->rowers+this->marines;
}
//Size
short Naval::width(){
	return (short)this->body.getSize().x;
}
short Naval::widthRows(){
	return (short)this->rows.getSize().y;
}
short Naval::height(){
	return (short)this->body.getSize().y;
}
//Texture
sf::Texture& Naval::Body(){
	return this->body;
}
sf::Texture& Naval::Ram(){
	return this->ram;
}
sf::Texture& Naval::Rows(){
	return this->rows;
}
//Variables
std::vector<Naval> naval;
sf::Texture sails;
short mercenaries;