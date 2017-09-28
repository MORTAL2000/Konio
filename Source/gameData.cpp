#include "gameData.h"
//Players
float getIncome(short player){
	float income=0;
	//Get income from settlements
	for(short i=0;i<(short)::settlement.size();i++)
		if(isOf(i,player))
			income+=getIncomeOf(i);
	//Upkeep of ships
	for(short i=0;i<(short)::fleet.size();i++)
		if(::fleet[i].Player()==player)
			income-=::fleet[i].Upkeep();
	return income;
}
void Defeated(short winner,short defeated){
	::player[winner].WonBattle();
	::player[defeated].LostBattle();
}
//Fleets
float distFleet(short i,short j){
	return ::fleet[i].dist(fleet[j].getPosition());
}
short getShips(short player){
	short ships=0;
	for(short i=0;i<(short)::fleet.size();i++)
		if(::fleet[i].Player()==player)
			ships+=::fleet[i].size();
	return ships;
}
short getNearestFleet(sf::Vector2f point){
	short nearest=0;
	float Dist=dist(point,fleet[5].getPosition());
	for(short i=1;i<(short)fleet.size();i++)
		if(Dist>dist(point,fleet[i].getPosition())){
			Dist=dist(point,fleet[i].getPosition());
			nearest=i;
		}
	return nearest;
}
std::string FleetInfo(short i){
	return gui.Format(::fleet[i].size())+" ships - "+gui.Format(::fleet[i].Provision())+"%";
}
//Settlements
bool isYourSett(short settlement){
	return (::settlement[settlement].getOwner()==human);
}

float getIncomeOf(short i){
	float taxes=0,trade=0,bonuses=0;
	//Tax from population
	taxes+=::settlement[i].getPopulation()/1000.;
	if(hasGood(i,0))
		bonuses+=taxes/10.;
	//Export
	if(isAllyOf(i,settlement[getImporter(i)].getOwner())){
		trade+=::good[settlement[i].getGood()].Price();
		if(hasGood(i,2))
			bonuses+=trade/5.;
		if(hasGood(i,5))
			bonuses+=trade/10.;
	}
	//Import
	if(isAllyOf(i,settlement[settlement[i].getImport()].getOwner()))
		trade-=::good[settlement[settlement[i].getImport()].getGood()].Price();
	return taxes+trade+bonuses;
}
short getNearestSett(sf::Vector2f point){
	short nearest,begin=0,end=::settlement.size();
	while(begin+1<end){
		nearest=(begin+end)/2;
		if(dist(::settlement[begin].getPosition(),point)<dist(::settlement[end].getPosition(),point))
			end=nearest;
		else
			begin=nearest;
	}
	return nearest;
}
short getPopulation(short player){
	unsigned pop=0;
	for(short i=0;i<(short)::settlement.size();i++){
		if(isOf(i,player))
			pop+=::settlement[i].getPopulation();
	}
	return pop;
}
//Diplomacy
bool isOf(short i,short player){
	return (::settlement[i].getOwner()==player);
}
bool isAllyOf(short i,short player){
	return (areAllies(::settlement[i].getOwner(),player));
}
bool isEnemyOf(short i,short player){
	return (areEnemies(::settlement[i].getOwner(),player));
}
bool isYourFleet(short i){
	return ::fleet[i].Player()==human;
}
bool isEnemyFleet(short i,short j){
	return ::player[::fleet[i].Player()].Team()!=::player[::fleet[j].Player()].Team();
}
bool isAlliedFleet(short i,short j){
	return ::player[::fleet[i].Player()].Team()==::player[::fleet[j].Player()].Team();
}
//Goods
bool hasGood(short settlement,short good){
	return(::settlement[settlement].getGood()==good||(getImportedGood(settlement)==good&&!isBlockedImport(settlement)));
}
bool isBlockedExport(short sett){
	return isEnemyOf(sett,::settlement[getImporter(sett)].getOwner());
}
bool isBlockedImport(short sett){
	return isEnemyOf(sett,::settlement[::settlement[sett].getImport()].getOwner());
}
bool playerHasGood(short player,short good){
	for(short i=0;i<(short)::settlement.size();i++)
		if(isOf(i,player)&&hasGood(i,good))
			return 1;
	return 0;
}
short getImporter(short settlement){
	for(short i=0;i<(short)::settlement.size();i++)
		if(::settlement[i].getImport()==settlement)
			return i;
	return -1;
}
short getImportedGood(short settlement){
	return (::settlement[::settlement[settlement].getImport()].getGood());
}
std::string ExportedGoodStatus(short sett){
	if(isBlockedExport(sett))
		return good[::settlement[sett].getGood()].Name()+" to "+(settlement[getImporter(sett)].getName())+" (blocked)";
	return good[::settlement[sett].getGood()].Name()+" to "+(settlement[getImporter(sett)].getName());
}
std::string ImportedGoodStatus(short sett){
	if(isBlockedImport(sett))
		return good[getImportedGood(sett)].Name()+" from "+::settlement[::settlement[sett].getImport()].getName()+ "(blocked)";
	return good[getImportedGood(sett)].Name()+" from "+::settlement[::settlement[sett].getImport()].getName();
}
std::string EconomicStatus(short sett){
	return "Export "+ExportedGoodStatus(sett)+".   Import "+ImportedGoodStatus(sett)+".";
}