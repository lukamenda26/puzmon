#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/*** 列挙型宣言 ***/
enum {
    EMPTY,
    LIFE,
    FIRE,
    WATER,
    WIND,
    EARTH
};

/*** 構造体型宣言 ***/
typedef struct 
{
    char name[1024];
    int hp;
    int maxHp;
    int type;
    int attack;
    int defence;
} Monster;

typedef struct
{
    char symbol[1024];
    int color;
} Elements;

typedef struct
{
    char dungeonName[1024];
    Monster* enemyAddr;
    int MonsterCount;  
} Dungeon;

typedef struct
{
    char* playerName;
    Monster* partyMonsterAddr;
    int sumHp;
    int sumMaxHp;
    int avgDefence;
} Party;

typedef struct
{
    Party* party;
    Monster* enemyMonsterAddr;
    char* gems;
} BattleField;

typedef struct
{
    int type;
    char* startContinuousAddr;
    int continuousCount;
} BanishInfo;



/*** グローバル定数の宣言 ***/
static const int PARTY_MONSTER_COUNT = 4;
static const int MAX_GEMS = 14;
static const int A_NUMBER = 65;

static const Elements ELEMENTS[] = {
    {" ", 0}, // EMPTY
    {"&", 5}, // LIFE
    {"$", 1}, // FIRE
    {"~", 6}, // WATER
    {"@", 2}, // WIND
    {"#", 3}  // EARTH
};

static const double ELEMENT_BOOST[4][4] = {
    {1.0, 0.5, 2.0, 1.0}, // FIREが〜へ攻撃
    {2.0, 1.0, 1.0, 0.5}, // WATERが〜へ攻撃
    {0.5, 1.0, 1.0, 2.0}, // WINDが〜へ攻撃
    {1.0, 2.0, 0.5, 1.0}  // EARTHが〜へ攻撃
};

void printMonsterName(Monster*);
void fillGems(char*, int, int);
void printGems(char*, int);
void moveGem(int, int, char*, bool);
void swapGem(char*, int, int, char*, bool);
void blurPower(int*);
void amplifyPower(int*, int, int);
void doRecover(BattleField*, int, int*);

#endif