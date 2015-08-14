#include <vector>


enum Side
{
	NORTH,
	SOUTH
};


enum MoveType
{
	WALK,
	HUMANATTACK,
	DWARFATTACK,
	DRAGONPAW,
	DRAGONFIRE,
	HEAL,
	FREEZESPELL,
	WEAKENSPELL,
	NOACT
};


enum ObjectType
{
	BATTLEMAGE,
	WIZARD,
	CLERIC,
	HUMAN,
	DWARF,
	DRAGON,
	FOOD,
	WALL
};


struct Move
{
	MoveType mType;
	int relativeX;
	int relativeY;
};


struct Object
{
	Side teamSide;
	ObjectType objType;
	int relativeX;
	int relativeY;
	bool bewitched;
};


class Creature
{


	private:

		Side side;
		int actionPower;
		int toughness;
		bool bewitched;


	public:

		Creature (Side _side, int _power, int _toughness)
			: side(_side),
			  actionPower(_power),
			  toughness(_toughness)
		{
			bewitched = false;
		}


		virtual ~Creature ()
		{
		}


		Side getSide() const
		{
			return side;
		}


		int getActionPower() const
		{
			return actionPower;
		}


		int getToughness() const
		{
			return toughness;
		}


		bool getBewitched() const
		{
			return bewitched;
		}


		void setBewitched (bool _bewitched)
		{
			bewitched = _bewitched;
		}


		void loseToughness (int decreasingToughnessAmount)
		{
			toughness -= decreasingToughnessAmount;
		}


		/* This function is called when there is a healing act on you, so increase your toughness according to the given values mentioned above in the text (in the derived class implementation), also increase mana or leftFireCount if applicable */
		virtual void operator++() 
		{
		}


		virtual Move move (const std::vector <Object> &list) = 0;


};


class Human : public Creature
{


	public:

		Human (Side _side, int _power, int _toughness)
			: Creature (_side, _power, _toughness)
		{
		}


		void operator++();


		Move move (const std::vector <Object> &list);


};


class Dwarf : public Creature
{


	public:
		Dwarf (Side _side, int _power, int _toughness)
			: Creature(_side, _power, _toughness)
		{
		}


		void operator++();


		Move move (const std::vector <Object> &list);


};


class Dragon : public Creature
{


	private:

		int leftFireCount;


	public:

		Dragon (Side _side, int _power, int _toughness, int _leftFireCount)
			: Creature (_side, _power, _toughness),
			  leftFireCount(_leftFireCount)
		{
		}


		int getLeftFireCount() const; // return leftFireCount


		void operator++();


		Move move (const std::vector <Object> &list);


};


class Wizard : public virtual Creature
{


	private:

		int mana;


	public:

		Wizard (Side _side, int _power, int _toughness, int _mana)
			: Creature (_side, _power, _toughness),
			  mana (_mana)
		{
		}


		int getMana() const;


		void loseMana (int t)
		{
			this->mana -= t;
		}

		void operator++();


		Move move (const std::vector <Object> &list);


};


class Cleric : public virtual Creature
{


	public:

		Cleric (Side _side, int _power, int _toughness)
			: Creature (_side, _power, _toughness)
		{
		}


		void operator++();


		Move move (const std::vector<Object> &list);


};


class BattleMage : public Wizard, public Cleric
{


	public:

		BattleMage (Side _side, int _power, int _toughness, int _mana)
			: Wizard(_side, _power, _toughness, _mana),
			  Cleric(_side, _power, _toughness),
			  Creature(_side, _power, _toughness)
		{
		}


		void operator++();


		Move move (const std::vector<Object> &list);


};
