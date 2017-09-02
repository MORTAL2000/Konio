#include "map.h"
//Constructor
Map::Map(){
	this->tex=new sf::Texture();
	this->tex->loadFromFile("data/game/map/sprite.png");
	this->sprite=new sf::Sprite(*tex);
	this->sprite->setPosition(0,52);
	water=new Layer(1,64,"data/game/map/water.png");
}
//Rendering and update
void Map::Render(sf::RenderWindow *window){
	//Water
	this->water->Render(window);
	//Lands
	window->draw(*sprite);
	//Graph
	for(short i=0;i<(short)this->node.size();i++){
		this->node[i].Render(window);
		window->draw(this->text[i]);
	}
	//Regions
	for(short i=0;i<(short)this->region.size();i++)
		this->region[i].Render(window);
}
void Map::move(short x,short y){
	this->water->move(x,y);
}
void Map::Update(){
	//Water
	this->water->Update();
	//Regions
	for(short i=0;i<(short)this->region.size();i++)
		this->region[i].Update();
}
//Graph
std::vector<Node> Map::getRoute(short begin,short end){
	this->clearRoute();
	this->selected.resize(this->node.size(),0);
	Dijkstra(begin,end);
	return this->route;
}
void Map::Dijkstra(short begin,short end){
	//If begin is adiacent with end
	if(this->node[begin].collision(this->node[end])){
		this->selected[end]=1;
		this->route.push_back(this->node[end]);
		return ;
	}
	//Arrays data for search
	this->selected[begin]=1;
	for(short i=0;i<(short)this->node.size();i++)
		if(!selected[i]){
			if(collision(begin,i)){
				d.push_back(1);
				t.push_back(begin);
			}else{
				d.push_back(999);
				t.push_back(-1);
			}
		}else{
			d.push_back(0);
			t.push_back(-1);
		}
	//Search
	for(short j=1;j<(short)this->node.size();j++){
		//Get nearest unselected node
		short near=-1;
		float minDist=999999999;
		for(short i=0;i<(short)this->node.size();i++)
			if(!selected[i]){
				if(dist(begin,i)<minDist){
					near=i;
					minDist=dist(begin,i);
				}
			}
		selected[near]=1;
		//Get list
		minDist=999999999;
		for(short i=0;i<(short)this->node.size();i++)
			if(collision(near,i)&&!selected[i]){
				if(d[i]>d[near]+1){
					d[i]=d[near]+1;
					t[i]=near;
				}
			}
	}
	//Get route
	while(t[end]!=-1){
		//std::cout<<"t[ "<<end<<" ]= "<<t[end]<<'\n';
		this->route.insert(this->route.begin(),this->node[end]);
		end=t[end];
	}
}
bool Map::collision(short i,short j){
	return (this->node[i].collision(this->node[j]));
}
float Map::dist(short i,short j){
	return this->node[i].dist(this->node[j]);
}
short Map::getNearestNode(sf::Vector2f point){
	short nearest=0;
	float Dist=this->node[0].dist(point);
	if(this->node[0].contains(point))
		return 0;
	for(short i=1;i<(short)this->node.size();i++){
		if(this->node[i].contains(point))
			return i;
		if(Dist>this->node[i].dist(point)){
			Dist=this->node[i].dist(point);
			nearest=i;
		}
	}
	return nearest;
}
void Map::clearRoute(){
	this->t.clear();
	this->route.clear();
	this->selected.clear();
	this->d.clear();
}
Node Map::getNode(short i){
	return this->node[i];
}
//Add functions
void Map::add(Region region){
	this->region.push_back(region);
}
void Map::add(Node node){
	this->text.push_back(sf::Text(sf::String(std::to_string(this->node.size())),*gui.Font(),26));
	this->text[this->text.size()-1].setPosition(node.getPosition());
	this->text[this->text.size()-1].setColor(sf::Color(255,255,255));
	this->node.push_back(node);
}
Map::~Map(){
	
}
//Global variable
Map *map;