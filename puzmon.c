/*** インクルード宣言 ***/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>

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
} BattleField;


/*** グローバル定数の宣言 ***/
const int PARTY_MONSTER_COUNT = 4;
const int MAX_GEMS = 14;
const int A_NUMBER = 65;

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
void moveGem(char*, char*);
void swapGem(char*, int, int, char*);

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

bool checkValidCommand(char* command)
{
    bool returnBal = true;

    for (int i = 0; i < 3; i++)
    {
        if (i < 2 && (command[i] < A_NUMBER || command[i] > (A_NUMBER + MAX_GEMS)))
        {
            printf("%c\n", command[i]);
            returnBal = false;
            break;
        }
        

        if (i == 2 && command[i] != 0)
        {
            returnBal = false;
            break;
        }
    }

    return returnBal;
}

void doAttack(BattleField* battleField)
{
    const int DUMMY_DAMAGE = 80;
    printf("ダミー攻撃で%dのダメージを与えた。\n\n", DUMMY_DAMAGE);
    (*battleField).enemyMonsterAddr->hp -= DUMMY_DAMAGE;
}

void evaluateGems(BattleField* battleField)
{
    doAttack(battleField);
}

void onPlayerTurn(BattleField* battleField)
{
    char command[1024];

    printf("【%sのターン】\n", (*battleField).party->playerName);
    printf("-----------------------------\n\n");
    printf("         ");
    printMonsterName((*battleField).enemyMonsterAddr);
    printf("\n        HP= %d / %d\n\n", (*battleField).enemyMonsterAddr->hp, (*battleField).enemyMonsterAddr->maxHp);

    for (int i = 0; i < PARTY_MONSTER_COUNT; i++)
    {
        printMonsterName(&(*battleField).party->partyMonsterAddr[i]);
        printf(" ");
    }
    printf("\n        HP= %d / %d\n", (*battleField).party->sumHp, (*battleField).party->sumMaxHp);
    
    printf("-----------------------------\n");

    int alphabetNum = 0;

    for (int i = 0; i < MAX_GEMS; i++)
    {
        alphabetNum = A_NUMBER + i;
        printf("%c ", alphabetNum);
    }
    printf("\n");
    printGems((*battleField).gems, MAX_GEMS);
    printf("-----------------------------\n");

    printf("コマンド？ > ");
    scanf("%s", command);

    bool isVarid = checkValidCommand(command);

    while (isVarid == false)
    {
        printf("不正な値です。表示されているアルファベットを2つ指定する必要があります。\n");
        printf("コマンド？ > ");
        scanf("%s", command);
        isVarid = checkValidCommand(command);
    }

    moveGem(command, battleField->gems);
    
    printf("\n");

    evaluateGems(battleField);
}

void onEnemyTurn(BattleField* battleField)
{
    const int DUMMY_MY_DAMAGE = 20;
    printf("【%sのターン】\n", (*battleField).enemyMonsterAddr->name);
    printf("%dのダメージを受けた。\n\n", DUMMY_MY_DAMAGE);
    (*battleField).party->sumHp -= DUMMY_MY_DAMAGE;
}

int doBattle(BattleField* battleField)
{
    printMonsterName((*battleField).enemyMonsterAddr);
    printf("が現れた！\n");
    printf("\n");

    do
    {
        onPlayerTurn(battleField);
        if ((*battleField).enemyMonsterAddr->hp <= 0)
        {
            printf("\n");
            printMonsterName((*battleField).enemyMonsterAddr);
            printf("を倒した！\n");
            printf("残HP:%d\n",(*battleField).party->sumHp);
            return 1;
            break;
        }
        
        onEnemyTurn(battleField);
    } while ((*battleField).enemyMonsterAddr->hp > 0 && (*battleField).party->sumHp > 0);

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
        BattleField newBattleField = {party, &dungeon->enemyAddr[i], gems};
        defeatedMonsterCount += doBattle(&newBattleField);

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

void moveGem(char* fromTo, char* gems)
{
    int firstGemNum = fromTo[0] - A_NUMBER;
    int secondGemNum = fromTo[1] - A_NUMBER;

    char fomerGems[1024];
    strcpy(fomerGems, &gems[firstGemNum]);

    swapGem(fomerGems, firstGemNum, secondGemNum, gems);
}

void swapGem(char* fomerGems, int startGemNum, int endGemNum, char* gems)
{
    // NOTE::formerGemsの参照元配列の先頭に、移動するgem情報が格納されている。

    if(startGemNum < endGemNum)
    {
        // gemを右へ動かす。
        for (int i = startGemNum; i < endGemNum; i++)
        {
            gems[i] = gems[i + 1];
            gems[i + 1] = fomerGems[0];
            printGems(gems, MAX_GEMS);
            printf("\n");
        }
    }
    else
    {
        // gemを左へ動かす。
        for (int i = startGemNum; i > endGemNum; i--)
        {
            gems[i] = gems[i - 1];
            gems[i - 1] = fomerGems[0];
            printGems(gems, MAX_GEMS);
            printf("\n");
        }
    }
}
