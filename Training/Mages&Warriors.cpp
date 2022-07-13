#include <iostream>
#include <map>
#include <string>
#include <set>
using namespace std;

// class A
// {
// public:
// 	int a = 1;
// 	void foo()
// 	{
// 		cout << "Foo" << endl;
// 	}

// protected:
// 	int b = 2;

// };

// class B: public A
// {
// 	void bar()
// 	{
// 		cout << a << endl;
// 		cout << b << endl;
// 	}
// };

class CHero
{
protected:
	bool is_alive;
	int hp_current;
	const int hp_max;
	const int power_attack;
	const string name;

	void onHit (int power_attack)
	{
		cout << name << " loses " << power_attack << "HP!" << endl;
		hp_current -= power_attack;
	}

public:
	CHero (const string set_name, const int set_power_attack, const int set_hp_max)
		 : is_alive(true), hp_max(set_hp_max), power_attack(set_power_attack), name(set_name)
	{
	}
	~CHero ()
	{
	}

	void attack (CHero &attacked)
	{
		if (!attacked.is_alive)
			cout << attacked.name << " cannot be attacked, he is dead!";
		else
		{
			cout << name << " attacks " << attacked.name << "!" << endl;
			attacked.onHit(power_attack);
			if (attacked.hp_current <= 0)
			{
				cout << attacked.name << " was killed!" << endl;
				is_alive = false;
			}
		}
	}

};

class CWarrior : public CHero
{
public:
	CWarrior (const string set_name, const int set_power_attack, const int set_hp_max)
	: CHero (set_name, set_power_attack, set_hp_max)
	{
	}

	void onHitWithShield (int power_attack)
	{
		if (rand() % 2 == 0)
		{
			cout << name << " blocked attack for " << power_attack << "DMG!" << endl;
			return;
		}
		onHit(power_attack);
	}

};

class CMage : public CHero
{
protected:
	int mana_current;
	const int mana_max;
	static const int MANA_GROWTH = 10;
	static const int SPELL_MULTIPLIER = 2;

public:
	CMage (const string set_name, const int set_power_attack, const int set_hp_max, const int set_mana_max)
	: CHero (set_name, set_power_attack, set_hp_max), mana_current(0), mana_max(set_mana_max)
	{
	}

	void attackWithSpell (CHero &attacked)
	{
		if (mana_current == mana_max)
		{
			cout << name << " throws fireball on " << attacked.name << "!" << endl;
			attacked.onHit(power_attack * SPELL_MULTIPLIER);
			mana_current = 0;
		}
		else
		{
			mana_current = min(mana_max, mana_current + MANA_GROWTH);
			attack(attacked);
		}
	}

};


int main (void)
{

	return 0;
}
