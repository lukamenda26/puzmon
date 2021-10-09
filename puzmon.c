/*** インクルード宣言 ***/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

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
    int maxGemCount;
} BattleField;


/*** グローバル定数の宣言 ***/
const int PARTY_MONSTER_COUNT = 4;
const int MAX_GEMS = 14;

const Elements ELEMENTS[] = {
    {"$", 1},
    {"~", 6},
    {"@", 2},
    {"#", 3}
};

/*** プロトタイプ宣言 ***/
void printMonsterName(Monster*);
void fillGems(char*, int);
void printGems(char*, int);

/*** 関数宣言 ***/
Party organizeParty(char* player, Monster* monster)
{
    int sumHp = 0;
    int sumMaxHp = 0;
    int sumDefence = 0;
    int avgDefence = 0;

    for (int i = 0; i < PARTY_MONSTER_COUNT; i++)
    {
        sumHp += monster[i].hp;
        sumMaxHp += monster[i].maxHp;
        sumDefence += monster[i].defence;
    }

    avgDefence = sumDefence / PARTY_MONSTER_COUNT;

    Party party = {
        player,
        monster,
        sumHp,
        sumMaxHp,
        avgDefence
    };

    return party;
}

void showBattleField()
{

}

void onPlayerTurn(Monster* monster, Party* party)
{
    const int DUMMY_DAMAGE = 80;
    printf("【%sのターン】\n", (*party).playerName);
    printf("-----------------------------\n\n");
    printf("-----------------------------\n");

    const int a_num = 65;
    int alphabet_num = 0;
    for (int i = 0; i < MAX_GEMS; i++)
    {
        alphabet_num = a_num + i;
        printf("%c ", alphabet_num);
    }
    printf("\n");
     printf("-----------------------------\n");

    printf("ダミー攻撃で%dのダメージを与えた。\n\n", DUMMY_DAMAGE);
    (*monster).hp -= DUMMY_DAMAGE;
}

void onEnemyTurn(Monster* monster, Party* party)
{
    const int DUMMY_MY_DAMAGE = 20;
    printf("【%sのターン】\n", (*monster).name);
    printf("%dのダメージを受けた。\n\n", DUMMY_MY_DAMAGE);
    (*party).sumHp -= DUMMY_MY_DAMAGE;
}

int doBattle(Monster* monster, Party* party)
{
    printMonsterName(monster);
    printf("が現れた！\n");
    printf("\n");

    do
    {
        onPlayerTurn(monster, party);
        if ((*monster).hp <= 0)
        {
            printf("\n");
            printMonsterName(monster);
            printf("を倒した！\n");
            printf("残HP:%d\n", party->sumHp);
            return 1;
            break;
        }
        
        onEnemyTurn(monster, party);
    } while ((*monster).hp > 0 && (*party).sumHp > 0);

    return 0;
}

void showParty(Party* party)
{
    printf("＜パーティ編成＞----------\n");

    for (int i = 0; i < PARTY_MONSTER_COUNT; i++)
    {
        printMonsterName(&party->partyMonsterAddr[i]);
        printf(" HP= %d 攻撃= %d 防御= %d\n", party->partyMonsterAddr[i].hp, party->partyMonsterAddr[i].attack, party->partyMonsterAddr[i].defence);
    }
    
    printf("-----------------------\n\n");
}

int goDungeon(Party* party, Dungeon* dungeon)
{
    printf("%sのパーティ(HP=%d)は%sダンジョンに到着した。\n", (*party).playerName, (*party).sumHp, (*dungeon).dungeonName);
    showParty(party);

    int defeatedMonsterCount = 0;
    char gems[MAX_GEMS];

    for (int i = 0; i < (*dungeon).MonsterCount; i++)
    {
        fillGems(gems, MAX_GEMS);
        printGems(gems, MAX_GEMS);
        BattleField newBattleField = {party, (*dungeon).enemyAddr, gems, MAX_GEMS};
        defeatedMonsterCount += doBattle(&dungeon->enemyAddr[i], party);

        if ((*party).sumHp <= 0)
        {
            printf("%sはダンジョンから逃げ出した...\n", (*party).playerName);
            return 0;
            break;
        }
        else
        {
            printf( "%sはさらに奥へと進んだ。\n\n", (*party).playerName);
        }
        
    }
    
    printf("%sはダンジョンを制覇した！\n", (*party).playerName);
    return defeatedMonsterCount;
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

    Monster enemyMonster[] = {
        {"スライム",    100, 100, WATER, 10, 5},
        {"ゴブリン",    200, 200, EARTH, 20, 15},
        {"オオコウモリ", 300, 300, WIND, 30, 25},
        {"ウェアウルフ", 400, 400, WIND, 40, 30},
        {"ドラゴン",    800, 800, FIRE, 50, 40}
    };

    Dungeon dungeon = {"精霊の森", enemyMonster, 5};

    int downedMonsterCount = goDungeon(&newParty, &dungeon);
    
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

void fillGems(char* gems, int gemsCount)
{
    for (int i = 0; i < gemsCount; i++)
    {
        gems[i] = rand() % 4;
    }
}

void printGems(char* gems, int gemsCount)
{
    int gemNum = 0;
    for (int i = 0; i < gemsCount; i++)
    {
        gemNum = gems[i];
        printf("\x1b[4%dm%s\x1b[49m ", ELEMENTS[gemNum].color, ELEMENTS[gemNum].symbol);
    }
    printf("\n");
    
}
