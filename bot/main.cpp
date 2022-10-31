#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <math.h>
#include <fstream> 
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#include "game.h"

enum Motion {
    GO_PLACE,
    HUNT,
    ATTACK
};

int main()
{

    ifstream inf("/home/implication/eclipse-workspace/SpringChallenge2022-main/MyBot/SpiderAttack/data.json");
    json j;
    inf >> j;
    inf.close();
    
// init game
    srand( time(NULL) );
    int turn = 0;

    Position BasePos;
    std::cin >> BasePos; std::cin.ignore();

    Base myBase(BasePos);
    Base opBase( Position(GameManager::MAP_WIDTH-BasePos.x(), GameManager::MAP_HEIGHT-BasePos.y()) );

    GameManager GM;

    int heroes_per_player; // Always 3
    cin >> heroes_per_player; cin.ignore();

    Motion atk_m = GO_PLACE, def1_m = GO_PLACE, def2_m = GO_PLACE;

// pos for atk
    Position startPos = GM.posTrans(myBase, Position(GameManager::MAP_WIDTH-Base::LAND_RANGE, GameManager::MAP_HEIGHT-1500)),
             huntPos1 = GM.posTrans(myBase, Position(GameManager::MAP_WIDTH-Base::VIEW_RANGE-2200, GameManager::MAP_HEIGHT-1500)),
             huntPos2 = GM.posTrans(myBase, Position(GameManager::MAP_WIDTH-Base::VIEW_RANGE-2200*3, GameManager::MAP_HEIGHT-1500)),
             atkPos = GM.posTrans(myBase, Position(GameManager::MAP_WIDTH-5000, GameManager::MAP_HEIGHT-3000));
    Position atkPatrol = huntPos2;

// game loop
    while (1) {
    // turn input
        turn += 1;
        



        
        GM.updateBases(myBase, opBase);
        
        vector<Hero> myHero, opHero;
        vector<Monster> mons;

        GM.setEntity(myHero, opHero, mons);

/* attacker *////////////////////////////////////////////
        Hero atk = myHero[0];
        vector<Monster> nearAtk;

        for(Monster it : mons){
            if(atk.distance(it.getPos())<=2200 && !it.isThreartFor(2)){
                // hunting logic
                nearAtk.push_back(it);
            }
        }
        GM.sortByDist(atk.getPos(), nearAtk);
        
        if(atk_m == GO_PLACE){
            if (atk.getPos() != startPos){
                atk.Move(startPos, "go_place");
            }
            else{
                atk_m = HUNT;
                atkPatrol = huntPos1;
                atk.Move(huntPos2, "tonight_we_hunt");
            }

        }

        else if(atk_m == HUNT){
            if(turn >= j["Turn"]){
                atk_m = ATTACK;
                atk.Move(opBase.getPos(), "go_attack_" + to_string(j["Turn"]));
            }
            else{
                if(nearAtk.empty()){ // patrol
                    if(atkPatrol==huntPos1 && atk.getPos()==huntPos1){
                        atkPatrol = huntPos2;
                    }

                    if(atkPatrol==huntPos2 && atk.getPos()==huntPos2){
                        atkPatrol = huntPos1;
                    }

                    atk.Move(atkPatrol, "patrol");
                }
                else{ // hunt
                    atk.Move(nearAtk[0].getPos(), "hunt");
                }
            }
            
        }
        else{ // atk
            if(atk.getPos() != atkPos){
                if( !nearAtk.empty() ){
                    bool isAct = false;
                    for(Monster it : nearAtk){
                        if(/*it.isThreartFor(1) 
                            && */atk.inRangeControl(it.getPos()) 
                            && myBase.isManaEnough()){
                            atk.Control(it, opBase.getPos());
                            isAct = true;
                            break;
                        }
                    }
                    if(!isAct){
                        atk.Move(atkPos, "go_atk");
                    }                        
                }
                else{
                    atk.Move(atkPos, "go_atk");
                }
            }    
            else{
                atk.Wait("ATK");
            }
            
        }
/*



    // set patrol route (random) 
        
        attacker.setPatrol(Position((rand()%17630)*2, rand()%9000));
    
    //get target
        vector<Monster> attackMons;
        vector<Hero> attackHero;

        for( Monster it : mons ){
            if( attacker.inRangeControl(it.getPos()) )
                attackMons.push_back(it);
        }
        for( Hero it : opHero ){
            if( attacker.inRangeControl(it.getPos()) )
                attackHero.push_back(it);
        }
        
        if( !attackHero.empty() && !attacker.isShield() && myBase.getMana() > 30){
            attacker.Shield(attacker);
        }
        else if( !attackMons.empty() ){
            Monster target = attackMons[0];
            Hero ht;

            for(Monster it : attackMons){
                if( !it.isShield() ){
                    target = it;
                    break;
                }
            }
            for(Hero it : attackHero){
                if( !it.isShield() ){
                    ht = it;
                    break;
                }
            }
           
            if(opBase.distance( target.getPos() ) > 9000){ // ignore
                attacker.Patrol();
            }
            else if( !myBase.isManaEnough() ){ // out of mana
                if( target.isThreartFor(2) ){
                    attacker.Patrol();
                }
                else
                    attacker.Move( target.getPos() );
            }
            else if( opBase.distance( target.getPos() ) > 5000 + 1280 ){
                if( target.isThreartFor(2)  ){ // wind
                    if( attacker.inRangeWind(target.getPos()) )
                        attacker.Wind( opBase.getPos() );
                    else
                        attacker.Move( Position(target.getPos().x()-1000, target.getPos().y()) );
                }
                else{ // control
                    attacker.Control(target, opBase.getPos());
                }
            }
            else{
                if( attacker.inRangeWind(target.getPos()) ){
                    attacker.Wind( opBase.getPos() );
                }
                else if( !ht.isNull ){
                    attacker.Control(ht, myBase.getPos());
                }
                else{
                    attacker.Move( Position(target.getPos().x()-1000, target.getPos().y()) );
                }
            }
        }
        else{ // patrol
            attacker.Patrol();
        }
*/
/* attacker end *////////////////////////////////////////////////////////
/*
    monsteer speed 400
    shield keep 12 round
     => 4800 


*/
/* defender *////////////////////////////////////////////////////////
        myHero[1].setPatrol(Position(rand()%17630/2, rand()%9000));
        myHero[2].setPatrol(Position(rand()%17630/2, rand()%9000));
        vector<Monster> threaten, danger;

        GM.getThreatenMons(1, mons, threaten);
        GM.getDangerMons(myBase, mons, danger);

        vector<Monster> intarget;

        for (int i = 1; i < heroes_per_player; i++) {

            if( !danger.empty() ){
                if( myBase.distance(myHero[i].getClosestHero(opHero).getPos()) < 8000
                    && !myHero[i].isShield() && myBase.isManaEnough() ){
                    myHero[i].Shield(myHero[i]);
                }
                else{

                    auto target = danger.begin();

                    for(auto it=danger.begin(); it != danger.end(); ++it){
                        if( myBase.distance((*it).getPos()) < myBase.distance((*target).getPos()) )
                            target = it;
                    }

                    if(myBase.isManaEnough() && myHero[i].inRangeWind((*target).getPos()) && !(*target).isShield()){
                        myHero[i].Wind(opBase.getPos());
                    }
                    else{
                        myHero[i].Move((*target).getPos());
                    }

                    intarget.push_back(*target);
                    danger.erase(target); 
                }
            }
            else if( !threaten.empty() ){
                auto target = threaten.begin();

                for(auto it=threaten.begin(); it != threaten.end(); ++it){
                    if( myBase.distance((*it).getPos()) < myBase.distance((*target).getPos()) )
                        target = it;
                }

                if(myBase.isManaEnough() && myHero[i].inRangeControl((*target).getPos()) && !(*target).isShield() && myBase.getMana() > 100){
                    myHero[i].Control((*target) ,opBase.getPos());
                }
                else{
                    myHero[i].Move((*target).getPos());
                }

                intarget.push_back(*target);
                threaten.erase(target); 
            }
            else if( !intarget.empty() ){
                auto target = threaten.begin();

                for(auto it=threaten.begin(); it != threaten.end(); ++it){
                    if( myBase.distance((*it).getPos()) < myBase.distance((*target).getPos()) )
                        target = it;
                }

                myHero[i].Move((*target).getPos());
            
            }
            else{ 
                myHero[i].Patrol();
            }
            

            // Write an action using cout. DON'T FORGET THE "<< endl"
            // To debug: cerr << "Debug messages..." << endl;


            
            // In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
            
        }        
    }

}

/*

二防守 一進攻(過半場)

巡邏 存魔



*/
