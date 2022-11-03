#ifndef _BOT_H_
#define _BOT_H_

#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <math.h>

class Position{
private:
	int x_;
	int y_;

public:
	Position() :x_(0), y_(0) {}
	Position(int x, int y) :x_(x), y_(y) {}
	int x() const { return x_; }
	int y() const { return y_; }
	std::string toString() const { return std::to_string(x_) + " " + std::to_string(y_); }
	int distance(const Position& pos) const { return sqrt(pow(pos.x()-x_, 2) + pow(pos.y()-y_, 2)); }
	bool operator==(const Position& pos) const { return x_ == pos.x_ && y_ == pos.y_; }
	bool operator!=(const Position& pos) const { return x_ != pos.x_ || y_ != pos.y_; }
	const Position operator+(const Position& pos) const { return Position(x_ + pos.x_, y_ + pos.y_); }
	const Position operator-(const Position& pos) const { return Position(x_ - pos.x_, y_ - pos.y_); }
	friend std::istream& operator>>(std::istream& is, Position& s) { 
        return is >> s.x_ >> s.y_;
	}
	friend std::ostream& operator<<(std::ostream& os, const Position& s) {
		return os << s.x_ << " " << s.y_;
	}
};

class Base{
private:
	Position pos_;
	int health_ = 0;
	int mana_ = 0;

public:
	static constexpr int LAND_RANGE = 5000;	
	static constexpr int VIEW_RANGE = 6000;
	
	Base(Position pos) : pos_(pos) {} 

	void statusUpdate(int health, int mana) {health_ = health; mana_ = mana;}
	//bool isManaEnough() { return mana_ >= 20; }

	Position getPos() const { return pos_; }
    int getHealth() const { return health_; }
    int getMana() const { return mana_; }
	int distance(Position p) const { return pos_.distance(p); }
	bool isLand(const Position& p) const{ return pos_.distance(p) <= LAND_RANGE; }
    bool isView(const Position& p) const{ return pos_.distance(p) <= VIEW_RANGE; }
};

class Entity{
protected:
	int id_;
	int type_;
	Position pos_;
	int shieldLife_;
	int isControlled_;

public:
	Entity() { id_ = -1; }
	Entity(int id, int type) : id_(id), type_(type){}

	int getId() const { return id_; }
	int getType() const { return type_; }
	Position getPos() const { return pos_; }
	int getShildLife() const { return shieldLife_; }
	bool isControlled() const { return isControlled_ == 1; }
	bool isShield() const { return shieldLife_ != 0; }

	int distance(Position pos) const{ return pos_.distance(pos); }

	virtual void printErr(std::string msg) const {
        std::cerr << "print entity" << std::endl;
    }
};

class Monster : public Entity{
private:
	int health_;
	Position vec_;
	int nearBase_;
	int threatFor_;
	/*
		if nearBase is 0:
			0: it will never reach a base.
			1: it will eventually reach your base.
			2: it will eventually reach your opponent's base.

		If nearBase is 1: 
			1: this monster is targeting your base.
			2: otherwise.
	*/

public:
	Monster(int id) : Entity(id, 0){}

	bool isThrearten(int side) const{ return threatFor_ == side; }
	bool isNearBase() const { return nearBase_ == 1; }
	int getHealth() const { return health_; }

	friend std::istream& operator>>(std::istream& is, Monster& m) {
		return is >> m.pos_ >> m.shieldLife_ >> m.isControlled_ 
				  >> m.health_ >> m.vec_ >> m.nearBase_ >> m.threatFor_; 
	}

	void printErr(std::string msg) const override{
		fprintf(stderr, "================\n");
	
		fprintf(stderr, "Monster: %d\n", id_);
		fprintf(stderr, "x: %d   y: %d\n", pos_.x(), pos_.y());
		fprintf(stderr, "shield_life: %d\n", shieldLife_);
		fprintf(stderr, "is_controlled: %d\n", isControlled_);
		fprintf(stderr, "near_base: %d\n", nearBase_);
		fprintf(stderr, "threat_for: %d\n", threatFor_);
		fprintf(stderr, "================\n\n");
	}
};

/*
可視範圍: base 6000 hero 2200
攻擊 & 移動範圍: 800
*/

class Hero : public Entity{
private:
    const int atkRange_ = 800, windRange_ = 1280, spellRange_ = 2200;
	
    Position patrolPos_[2];
    int patrolTar_ = -1;     // -1: error unset, 0: pos1, 1: pos2
    
/* Tool function*/
    Position getCenter(Position a, Position b) const{
        Position mid( (a.x() + b.x()) / 2, (a.y() + b.y()) / 2 );
        double angle = std::atan2(b.y() - a.y(), b.x() - a.x());
        double len = a.distance(b) / 2.0;
        double dis = std::sqrt(1.0 - len * len);

        return Position( mid.x() - dis * std::sin(angle), mid.y() + dis * std::cos(angle) );
    }
public:
	Hero(int id, int type) : Entity(id, type) {}

	bool inRangeWind(const Position& p) const{ return pos_.distance(p) <= windRange_; }
	bool inRangeShield(const Position& p) const{ return pos_.distance(p) <= spellRange_; }
	bool inRangeControl(const Position& p) const{ return pos_.distance(p) <= spellRange_; }
	
    void setPatrol(Position pos1, Position pos2) { patrolPos_[0] = pos1; patrolPos_[1] = pos2; patrolTar_ = 0; }
	
/* Movement */ 
    void Patrol(std::string msg = ""){
        if(patrolTar_ == -1) {
            printErr("Error: No patrol pos!");
            return;
        }
        
        if(this->getPos() == patrolPos_[patrolTar_]){
            patrolTar_ = (patrolTar_ + 1) % 2;
        }

        std::cout << "MOVE " << patrolPos_[patrolTar_] << " " << "patrol" << std::endl;
        
    // build msg
        std::string log = "Patrol: " + patrolPos_[patrolTar_].toString();  
        
        if(msg != "") { log = log + " // " + msg; }
        printErr(log);

        return;
    }

    void Move(Position p, std::string msg = ""){
        std::cout << "MOVE " << p << " " << "move" << std::endl;

    // build msg
        std::string log = "Move: " + p.toString();  
        
        if(msg != "") { log = log + " // " + msg; }
        printErr(log);

        return;
    }


	void Wind(Position dir){ std::cout << "SPELL WIND " << dir << std::endl; }
	void Control(Entity target, Position dir) { std::cout << "SPELL CONTROL " << std::to_string(target.getId()) << " " << dir << std::endl; }
	void Shield(Entity target){ std::cout << "SPELL SHIELD " << std::to_string(target.getId()) << std::endl; }
	void Wait(std::string msg = ""){ std::cout << "WAIT " << msg << std::endl; }

/* Tool function */
    Position getHuntPos(std::vector<Monster> mons, int tarInd = 0){
        Position tar(mons[tarInd].getPos());
        
        Position result;
        int max = 0;

        for(int i=0; i < mons.size(); ++i){
            if(i==tarInd || mons[i].distance(tar)>this->atkRange_*2) continue;
            
            Position center = getCenter(tar, mons[i].getPos());
            int count = 0;

            for(int j=0; j<mons.size(); ++j){
                if(center.distance(mons[j].getPos()) <= this->atkRange_) ++count;
            }

            if(count >= max){
                max = count;
                result = center;
            }
        }

        return result;
    }
/* Print stderr message */
    void printErr(std::string msg) const override {
        std::cerr << "Hero " + std::to_string(this->id_) + ": " + msg << std::endl;
    }

/* input */
	friend std::istream& operator>>(std::istream& is, Hero& h) {
		int temp;
		Position tPos;
		return is >> h.pos_ >> h.shieldLife_ >> h.isControlled_ 
				  >> temp >> tPos >> temp >> temp; 
	}
};

#endif //_BOT_H_
