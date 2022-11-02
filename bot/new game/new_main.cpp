// #include <iostream>
// #include <string>
// #include <algorithm>
// #include <cstdlib>
// #include <ctime>
// #include <climits>
// #include <math.h>

#include "json.hpp"
#include "new_bot.h"
#include "new_game.h"
#include <cmath>

using namespace std;
using json = nlohmann::json;


int main()
{
    srand( time(NULL) );

/* init game */
    string geneDataFileName = "/home/implication/eclipse-workspace/SpringChallenge2022-main/MyBot/SpiderAttack/data.json";
    
    fstream geneFile(geneDataFileName); 
    json geneJson;

    geneFile >> geneJson;
    geneFile.close();
    
    ThreatenSystem TS(geneJson);
    GameManager GM(geneJson);
    
    // init base
    Position BasePos;
    std::cin >> BasePos; std::cin.ignore();
    Base myBase(BasePos);
    Base opBase( GM.posTrans(myBase, myBase.getPos()) );

    int heroes_per_player; // Always 3
    cin >> heroes_per_player; cin.ignore();
    
/* game loop */
    while (1) {
    /* turn input */
        GM.nextTurn();
        GM.updateBases(myBase, opBase);
        
        vector<Hero> myHero, opHero;
        vector<Monster> mons; 
        GM.updateEntity(myHero, opHero, mons);

    /* set up threatened data */
        map<int, float> myThreat, opThreat;
        float totalThreat;
        
        myThreat = TS.getMonsThreatenMap(myBase, mons, opHero, 1);
        opThreat = TS.getMonsThreatenMap(opBase, mons, myHero, 2);
        totalThreat = TS.getTotalThreaten(myThreat);

    /* attacker */
    // attacker setup
        Hero atk = myHero[0];
        Position huntPos1 = GM.posTrans(myBase, Position(GameManager::MAP_WIDTH-Base::VIEW_RANGE-2200, GameManager::MAP_HEIGHT-1500)),
                 huntPos2 = GM.posTrans(myBase, Position(GameManager::MAP_WIDTH-Base::VIEW_RANGE-2200*3, GameManager::MAP_HEIGHT-1500));
        atk.setPatrol(huntPos1, huntPos2);

        vector<Monster> nearAtkMons;
        for(Monster mon : mons){
            if( atk.distance(mon.getPos()) <= 2200 ){
                nearAtkMons.push_back(mon);
            }
        }
    
    // Movement condition
        if(totalThreat > GM.getDefThreat()){
        // defend
        } else if(GM.getTurn() > GM.getAtkTurn() && myBase.getMana() > GM.getAtkMana()){
        // attack
        } else if( !nearAtkMons.empty() ){ // hunt
        // get hunt pos
            TS.sortByThreaten(nearAtkMons, myThreat);

            
        } else{ // patrol
            atk.Patrol();            
        }
        

    /* TODO: NOTE */ 
    // patrol pos switch ?
    /* attacker */
        // if( total_threatened > X ){
        //     defend
        // } else if( turn > 110 || mp > 150 ) {
        //     attack
        // } else if(any near mons){
        //     hunt
        // } else{
        //     patrol
        // }
    
    /* defender */
        // if( total_threatend > Y ){
        //     deep defend
        // } else if( any threatend mons ) {
        //     hunt
        // }
    
    
    
    } // game loop end
}
