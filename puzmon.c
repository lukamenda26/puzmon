/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/
# include <stdio.h>
# include <string.h>

/*** 列挙型宣言 ***/
enum {
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
    Monster* enemyAddr;
    int MonsterCount;  
} Dungeon;

typedef struct
{
    char* playerName;
    Monster* partyMonsterAddr;
} Party;


/*** グローバル定数の宣言 ***/
const int PARTY_MONSTER_COUNT = 4;

const Elements ELEMENTS[] = {
    {"$", 1},
    {"~", 6},
    {"@", 2},
    {"#", 3}
};

/*** プロトタイプ宣言 ***/
void printMonsterName(Monster*);

/*** 関数宣言 ***/
Party organizeParty(char* player, Monster* monster)
{
    Party party = {player, monster};
    return party;
}

void doBattle(Monster* monster)
{
    printMonsterName(monster);
    printf("が現れた！\n");
    printMonsterName(monster);
    printf("を倒した！\n");
}

// void showParty(Party party)
// {

// }

int goDungeon(Party party)
{
    int sumHp = 0;
    for (int i = 0; i < PARTY_MONSTER_COUNT; i++)
    {
        sumHp += party.partyMonsterAddr->hp;
    }

    printf("%sのパーティ(HP=%d)はダンジョンに到着した。\n", party.playerName, sumHp);

    Monster enemyMonster[] = {
        {"スライム",    100, 100, WATER, 10, 5},
        {"ゴブリン",    200, 200, EARTH, 20, 15},
        {"オオコウモリ", 300, 300, WIND, 30, 25},
        {"ウェアウルフ", 400, 400, WIND, 40, 30},
        {"ドラゴン",    800, 800, FIRE, 50, 40}
    };

    Dungeon dungeon = {enemyMonster, 5};

    for (int i = 0; i < dungeon.MonsterCount; i++)
    {
        doBattle(&enemyMonster[i]);
    }
    
    printf("%sはダンジョンを制覇した！\n", party.playerName);
    return dungeon.MonsterCount;
}

int main(int argc, char** argv)
{
    char* player = argv[1];

    if (player == NULL)
    {
        printf("エラー：プレイヤー名を指定して起動してください。\n");
        return 0;
    }
    
    printf("*** Puzzle & Monsters ***\n");

    Monster partyMonster[] = {
        {"朱雀", 150, 150, FIRE, 25, 10},
        {"青龍", 150, 150, WIND, 15, 10},
        {"白虎", 150, 150, EARTH, 20, 5},
        {"玄武", 150, 150, WATER, 20, 15}
    };
    Party newParty = organizeParty(argv[1], partyMonster);

    int downedMonsterCount = goDungeon(newParty);
    
    printf("*** GAME CREARED! ***\n");
    printf("倒したモンスター数＝%d\n", downedMonsterCount);
    
    return 0;
}

/*** ユーティリティ関数宣言 ***/
void printMonsterName(Monster* monster)
{
    const char* symbol = ELEMENTS[(*monster).type].symbol;
    const int colorNum = ELEMENTS[(*monster).type].color;
    printf("\x1b[3%dm%s%s%s\x1b[0m", colorNum, symbol, (*monster).name, symbol);
}
