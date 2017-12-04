#include "fleet.h"

Fleet::Fleet(sf::Vector2f pos,float angle,short player,short formation,float provision){
	//Main properties
	this->player=player;
	this->provision=provision;
	this->formation=formation;
	this->speed=0;
	//Body
	this->body=new sf::Sprite(fleetBody);
	//Sails
	this->sails=new sf::Sprite(::sails);
	this->sails->setTextureRect(sf::IntRect(13*(::player[player].Faction()),0,12,100));
	//Origin
	this->body->setOrigin(fleetBody.getSize().x/2,fleetBody.getSize().y/2);
	this->sails->setOrigin(0,::sails.getSize().y/2);
	//Orientation
	this->setPosition(pos);
	this->setRotation(angle);
	//Scale
	this->sails->setScale(0.6,0.6);
}
//Update and rendering
void Fleet::Render(sf::RenderWindow *window){
	window->draw(*this->body);
	window->draw(*this->sails);
}
void Fleet::Update(){
	this->provision-=0.011;
	if(this->route.size()){
		//Get the angles
		float angleShip=this->body->getRotation();
		float angle=getAngle(this->getPosition(),this->route[0].getPosition());
		int way=1;
		//If direction is change
		if(this->speed<0){
			this->speed=0;
		}
		if(!this->route[0].contains(this->getPosition())){
			//Get way
			if(fabs(angleShip-angle)>180)
				way*=-1;
			//Change angle
			if(angleShip<angle){
				if(angle<angleShip+way*this->Randament())
					this->setRotation(angle);
				else
					this->rotateTo(way*this->Randament());
			}
			else if(angleShip>angle){
				if(angle>angleShip-way*this->Randament())
					this->setRotation(angle);
				else
					this->rotateTo(-way*this->Randament());
			}
			//Modify speed
			this->speed=10*this->Randament();
		//Stop
		}else
			this->route.erase(this->route.begin());
		//Move
		this->Move(
			this->SpeedOnFrame()*cos(this->getRotationRad()),
			this->SpeedOnFrame()*sin(this->getRotationRad())
		);
	}else if(this->speed){
		this->Stop();
	}
}
void Fleet::addShip(short ship,short integrity){
	this->ship.push_back(ship);
	this->integrity.push_back(integrity);
}
void Fleet::removeShip(short ship){
	this->ship.erase(this->ship.begin()+ship);
	this->integrity.erase(this->integrity.begin()+ship);
}
void Fleet::setIntegrity(short ship,short integrity){
	this->integrity[ship]=integrity;
}
void Fleet::Reform(short formation){
	this->formation=formation;
}
void Fleet::Retreat(){
	this->integrity.clear();
	this->ship.clear();
}
void Fleet::Supply(){
	this->provision=100;
	for(short i=0;i<(short)this->integrity.size();i++)
		this->integrity[i]=100;
}
void Fleet::Supply(float provision){
	this->provision+=provision;
	if(this->provision>100)
		this->provision=100;
}
//Route
void Fleet::addNodeRoute(Node node){
	this->route.push_back(node);
}
void Fleet::getRoute(std::vector<Node> route){
	this->route.clear();
	this->route=route;
}
void Fleet::resetRoute(){
	this->route.clear();
}
//Move
void Fleet::Move(float x,float y){
	this->body->move(x,y);
	this->sails->move(x,y);
}
void Fleet::Stop(){
	this->speed=0;
	this->route.clear();
}
//Get data
short Fleet::Player(){
	return this->player;
}
short Fleet::Formation(){
	return this->formation;
}
short Fleet::Provision(){
	return this->provision;
}
short Fleet::Direction(){
	if(this->speed<0)
		return -1;
	return 1;
}
short Fleet::size(){
	return this->ship.size();
}
short Fleet::getShips(short id){
	short number=0;
	for(short i=0;i<(short)this->ship.size();i++)
		if(ship[i]==id)
			number++;
	return number;
}
short Fleet::Ship(short i){
	return this->ship[i];
}
short Fleet::Integrity(short i){
	return this->integrity[i];
}
float Fleet::Upkeep(){
	return this->integrity.size()*0.1;
}
float Fleet::Speed(){
	return this->speed;
}
float Fleet::SpeedOnFrame(){
	return this->speed*0.032;
}
float Fleet::Power(){
	float power=0;
	for(short i=0;i<(short)this->ship.size();i++)
		power+=::naval[this->ship[i]].Power();
	return (power*this->formation*this->integrity.size());
}
float Fleet::Randament(){
	return this->provision/100;
}
//Rotation
void Fleet::rotate(float angle){
	this->setRotation(this->getRotation()+angle);
}
void Fleet::rotateTo(float angle){
	this->rotate(angle);
}
void Fleet::setRotation(float angle){
	//Set angle of fleet
	this->body->setRotation(angle);
	//Set angle of sail
	if(angle>=0&&angle<=180)
		this->sails->setRotation(angle/2);
	else
		this->sails->setRotation(360-(360-angle)/2);
}
float Fleet::getRotation(){
	return this->body->getRotation();
}
float Fleet::getRotationRad(){
	return this->body->getRotation()*0.017453293;
}
//Points
void Fleet::setPosition(sf::Vector2f point){
	this->body->setPosition(point);
	this->sails->setPosition(point);
}
float Fleet::dist(sf::Vector2f point){
	return ::dist(this->getPosition(),point);
}
bool Fleet::contains(sf::Vector2f point){
	return (distSquare(this->getPosition(),point)<=4225);
}
sf::Vector2f Fleet::getPosition(){
	return this->body->getPosition();
}
sf::Vector2f Fleet::getTarget(){
	if(this->route.size())
		return this->route[this->route.size()-1].getPosition();
	return this->body->getPosition();
}
std::vector<Node> Fleet::getRoute(){
	return this->route;
}
//Mouse
bool Fleet::mouseOver(){
	if(this->contains(gui.mousePosition())){
		about.show(
			::player[this->player].Name()+"'s fleet"+getDiplomaticStatus(this->player)+" - Provision "+Format((int)this->provision)+"%",
			FormationName(this->formation)+"\n"+Format(this->size())+" ships  "+Format(this->Upkeep())+" upkeep"
		);
		return 1;
	}
	return 0;
}
bool Fleet::left(){
	if(this->mouseOver()&&gui.canLeft(200)){
		gui.clickRestart();
		return 1;
	}
	return 0;
}
Fleet::~Fleet(){
	
}
//Global variables
std::vector<Fleet> fleet;
//Formations
std::string FormationName(short formation){
	std::ifstream in("data/game/formations/"+std::to_string(formation)+".txt");
	if(in.is_open()){
		std::string text;
		std::getline(in,text);
		in.close();
		return text;
	}
	return "";
}
std::string FormationText(short formation){
	std::ifstream in("data/game/formations/"+std::to_string(formation)+".txt");
	if(in.is_open()){
		std::string text;
		std::getline(in,text);
		std::getline(in,text);
		in.close();
		return text;
	}
	return "";
}
sf::Texture fleetBody;