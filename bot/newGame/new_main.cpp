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
    string path_gene = "/home/implication/JavaWorkspace/SpiderAttack/bot/data.json";
    
    fstream geneFile(path_gene); 
    json jene;

    geneFile >> jene;
    geneFile.close();

    cerr << jene; 


    ThreatenSystem TS(jene);
    GameManager GM(jene);
    
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
        if(0){
        // defend
            atk.Wait("atk Def");
        } else if(0){
        // attack
            atk.Wait("atk Atk");
        } else if( !nearAtkMons.empty() ){ // hunt
        // get hunt pos
            TS.sortByThreaten(nearAtkMons, myThreat);
            Position tarPos = atk.getHuntPos(nearAtkMons);
            atk.Move(tarPos, "Hunt Pos " + tarPos.toString());
        } else{ // patrol
            atk.Patrol();            
        }
        
        myHero[1].Wait("def 1");
        myHero[2].Wait("def 2");

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
