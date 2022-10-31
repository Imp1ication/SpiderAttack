#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "json.hpp"
#include "new_bot.h"

using json = nlohmann::json;

class GameManager{
private:
    int turn_;
	int entityCount_ = 0;	

/* gene relative */
	int atkTurn_, atkMana_;
	float defThreat_;
public:
	static constexpr int MAP_WIDTH = 17630;
	static constexpr int MAP_HEIGHT = 9000;

	GameManager(json& geneFile) : turn_(0), entityCount_(0){
		atkTurn_ = geneFile["Atk"]["turn"];
		atkMana_ = geneFile["Atk"]["mana"];
		defThreat_ = geneFile["Def"]["threat"];
	}

/* turn input */
    void nextTurn() { turn_ += 1; }
    int getTurn() { return turn_; }

	void updateBases(Base& my, Base& op){
		int health, mana;

		std::cin >> health >> mana; std::cin.ignore();
		my.statusUpdate(health, mana);

		std::cin >> health >> mana; std::cin.ignore();
		op.statusUpdate(health, mana);
	}
	void updateEntity(std::vector<Hero>& my, std::vector<Hero>& op, std::vector<Monster>& mons){
		std::cin >> entityCount_; std::cin.ignore();

		for(int i=0; i<entityCount_; ++i){
			int id, type;
			std::cin >> id >> type;

			switch(type){
				case 0:{
					Monster temp(id);
					std::cin >> temp; std::cin.ignore();
					mons.push_back(temp);
					break;
				}
				case 1:{
					Hero temp(id, type);
					std::cin >> temp; std::cin.ignore();
					my.push_back(temp);
					break;
				}
				case 2:{
					Hero temp(id, type);
					std::cin >> temp; std::cin.ignore();
					op.push_back(temp);
					break;
				}
			} // switch end
		}
	}

/* coe */
    int getAtkTurn() const { return atkTurn_; }
    int getAtkMana() const { return atkMana_; }
    float getDefThreat() const { return defThreat_; }
    
/* tool function */
	Position posTrans(Base& my, Position tar){
		return (my.getPos().x() < 1000) ? tar : Position(MAP_WIDTH-tar.x(), MAP_HEIGHT-tar.y());
	}

/* Threaten related function */

////////////////////////////////////////////////////

	void getThreatenMons(int side, std::vector<Monster>& mons, std::vector<Monster>& threat){
		for(auto it : mons){
			if( it.isThrearten(side) )
				threat.push_back(it);
		}
	}

	void getDangerMons(Base& base, std::vector<Monster>& mons, std::vector<Monster>& danger){
		for(auto it : mons){
			if( it.isNearBase() && base.isLand( it.getPos() ) ){
				danger.push_back(it);
			}
		}
	}

	void sortByDist(Position p, std::vector<Monster> mons){
		auto cmp = [p](Monster l, Monster r){
			return p.distance(l.getPos()) < p.distance(r.getPos());
		};

		std::sort(mons.begin(), mons.end(), cmp);
	}

	void sortByDist(Position p, std::vector<Hero> heros){
		auto cmp = [p](Hero l, Hero r){
			return p.distance(l.getPos()) < p.distance(r.getPos());
		};

		std::sort(heros.begin(), heros.end(), cmp);
	}



};

class ThreatenSystem {
private:
	float monsHealth_, monsTarget_, monsShield_; // coefficient
	float heroControl_, heroWind_, heroNear_;

	float getHeroThreaten(Monster& mon, std::vector<Hero>& opHeros){
	/* Hero threate to spider */
	// spell threat / mons shieldlife  + near threat

		float totalThreat = 0;

		for(Hero it : opHeros){
			float threat = 0;
			if( mon.distance(it.getPos()) > 2200 ){ continue; }
			
			if( !mon.isShield() ){	// spell threat
				float spellThread = (mon.distance(it.getPos()) > 1280) ? heroControl_ : heroWind_;
				spellThread /= mon.getShildLife();

				threat += spellThread;
			}

			threat += heroNear_; 
			totalThreat += threat;					
		}

		return  totalThreat;
	}

public:
	ThreatenSystem(json& geneFile){
		monsHealth_ = geneFile["MonsCoe"]["health"];
		monsTarget_ = geneFile["MonsCoe"]["target"];
		monsShield_ = geneFile["MonsCoe"]["shield"];
		heroControl_ = geneFile["HeroCoe"]["control"];
		heroWind_ = geneFile["HeroCoe"]["wind"];
		heroNear_ = geneFile["HeroCoe"]["near"];
	}

	float getMonsThreaten(Base& myBase, Monster& mon, std::vector<Hero>& opHeros, int side){	
	/* Spider Threaten */
	/*
		( (spiderHealth * coe) 
		+ (isTargettingBase ? coe : 0)
		+ (shieldLife * coe)
		+ (herosThreat * coe)
		) / (baseDis * baseHealth)
			
		map<id, threaten>, <-1, totalThreaten>
	*/
		float threat = mon.getHealth() * monsHealth_ 
						+ (mon.isThrearten(side) ? monsTarget_ : 0)  
						+ mon.getShildLife() * monsShield_
						+ getHeroThreaten(mon, opHeros);

		threat /= ( myBase.distance(mon.getPos()) * myBase.getHealth() );	
		return threat;
	}
		
	std::map<int, float> getMonsThreatenMap(Base& myBase, std::vector<Monster>& mons, std::vector<Hero>& opHeros, int side){
		std::map<int, float> thrMap;
		float totalThreat = 0;

		for(Monster it : mons){
			float threat = getMonsThreaten(myBase, it, opHeros, side);	
			totalThreat += threat;
			thrMap.insert( std::pair<int, float>(it.getId(), threat) );
		}

		thrMap.insert( std::pair<int, float>(-1, totalThreat) );

		return thrMap;
	}

    float getTotalThreaten(std::map<int, float> threatMap){
        float totalThreat = 0.0;
        for(auto v : threatMap){
           totalThreat += v.second;
        }
        return totalThreat;
    }

	void sortByThreaten(std::vector<Monster>& mons, std::map<int, float>& threatMap){
		auto cmp = [threatMap](Monster l, Monster r){
			return threatMap.at(l.getId()) < threatMap.at(r.getId()); 
		};

		std::sort(mons.begin(), mons.end(), cmp);
	}

	void sortByThreaten(std::vector<Hero>& heros, std::map<int, float>& threatMap){
		auto cmp = [threatMap](Hero l, Hero r){
			return threatMap.at(l.getId()) < threatMap.at(r.getId()); 
		};

		std::sort(heros.begin(), heros.end(), cmp);
	}
};

#endif //_GAME_H_
