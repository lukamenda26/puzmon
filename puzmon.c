/*** インクルード宣言 ***/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>

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
const int PARTY_MONSTER_COUNT = 4;
const int MAX_GEMS = 14;
const int A_NUMBER = 65;

const Elements ELEMENTS[] = {
    {" ", 0}, // EMPTY
    {"&", 5}, // LIFE
    {"$", 1}, // FIRE
    {"~", 6}, // WATER
    {"@", 2}, // WIND
    {"#", 3}  // EARTH
};

const double ELEMENT_BOOST[4][4] = {
    {1.0, 0.5, 2.0, 1.0}, // FIREが〜へ攻撃
    {2.0, 1.0, 1.0, 0.5}, // WATERが〜へ攻撃
    {0.5, 1.0, 1.0, 2.0}, // WINDが〜へ攻撃
    {1.0, 2.0, 0.5, 1.0}  // EARTHが〜へ攻撃
};

/*** プロトタイプ宣言 ***/
void printMonsterName(Monster*);
void fillGems(char*, int, int);
void printGems(char*, int);
void moveGem(int, int, char*, bool);
void swapGem(char*, int, int, char*, bool);
void blurPower(int*);
void amplifyPower(int*, int, int);
void doRecover(BattleField*, int);

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

void doAttack(BattleField* battleField, Monster* attackMonster, int continuousCount)
{
    const double ATTRIBUTE = ELEMENT_BOOST[attackMonster->type - 2][battleField->enemyMonsterAddr->type - 2];
    int damage = (int)((attackMonster->attack - battleField->enemyMonsterAddr->defence) * ATTRIBUTE);

    // 宝石消滅数とコンボ数に応じてダメージを増幅させる。
    amplifyPower(&damage, continuousCount, 1);

    // ダメージを±10%増加させる。
    blurPower(&damage);

    if (damage <= 0)
    {
        damage = 1;
    }
    

    printf("ダミー攻撃で%dのダメージを与えた。\n\n", damage);
    (*battleField).enemyMonsterAddr->hp -= damage;
}

int checkBanishable(char* gems, BanishInfo* banishInfo)
{
    // 最後に3連続以上があった場合に必ず黒消しを行えるよう、
    // 14つのgemの最後尾+1の配列格納場所に黒gemを格納。
    gems[MAX_GEMS] = EMPTY;

    // 3連続以上の塊の存在数 ＝  返り値。
    int continuousChunkCount = 0;

    int initialNum = EMPTY;
    int continuousCount = 1;

    for (int i = 1; i <= MAX_GEMS; i++)
    {
        if (gems[i - 1] == gems[i])
        {
            continuousCount++;
            if (initialNum != gems[i])
            {
                initialNum = gems[i];
            }
        }
        else
        {
            if (continuousCount >= 3)
            {
                banishInfo[continuousChunkCount].type = initialNum;
                banishInfo[continuousChunkCount].startContinuousAddr = &gems[i - continuousCount];
                banishInfo[continuousChunkCount].continuousCount = continuousCount;

                continuousChunkCount++;
            }
            continuousCount = 1;
            continue;
        }
        
    }

    return continuousChunkCount;
}

int shiftGems(int continuousChunkCount, BattleField* battleField, BanishInfo* banishInfo)
{
    // 黒gemの動かす先 ＝ gem配列の一番右側。
    int gemDestinationNum = MAX_GEMS - 1;

    // 最初に動かす黒gemの番号の初期値を設定。
    int moveGemNum = MAX_GEMS - 1;

    // 最初に動く黒gemのアドレス指定用変数。
    int movedGemCount = 0;

    for (int i = 0; i < continuousChunkCount; i++)
    {
        moveGemNum = (int)(banishInfo[i].startContinuousAddr + banishInfo[i].continuousCount - 1 - battleField->gems - movedGemCount);

        if (moveGemNum < gemDestinationNum)
        {
            for (int j = 0; j < banishInfo[i].continuousCount; j++)
            {
                moveGem((moveGemNum - j), (gemDestinationNum - j), battleField->gems, false);
                printGems(battleField->gems, MAX_GEMS);
                printf("\n");
            }
        }
        
        // 黒gemの動かし先番号を書き換える。
        gemDestinationNum = gemDestinationNum - banishInfo[i].continuousCount;

        movedGemCount += banishInfo[i].continuousCount;

        printf("最初に動かすgem：%d 次に黒gemが動くとしたら%d番目へ動く。\n", (int)(banishInfo[i].startContinuousAddr + banishInfo[i].continuousCount - 1 - battleField->gems), gemDestinationNum);
    }

    return gemDestinationNum;
}

void evaluateGems(BattleField* battleField)
{
    // gemが1列当たり並ぶ列 ÷ 3 + 1 以上の配列は入りようがない。
    int maxContinuous = MAX_GEMS / 3 + 1;
    BanishInfo banishInfo[maxContinuous];

    int continuousChunkCount = checkBanishable(battleField->gems, banishInfo);

    if (continuousChunkCount > 0)
    {
        for (int i = 0; i < continuousChunkCount; i++)
        {
            for (int j = 0; j < banishInfo[i].continuousCount; j++)
            {
                *(banishInfo[i].startContinuousAddr + j) = 0;
            }

            printGems(battleField->gems, MAX_GEMS);
            printf("\n");

            if (banishInfo[i].type > 1)
            {
                for (int k = 0; k < PARTY_MONSTER_COUNT; k++)
                {
                    if (battleField->party->partyMonsterAddr[k].type == banishInfo[i].type)
                    {
                        printMonsterName(&battleField->party->partyMonsterAddr[k]);
                        printf("の攻撃!!\n");
                        doAttack(battleField, &battleField->party->partyMonsterAddr[k], banishInfo[i].continuousCount);
                    }
                }
            }

            if (banishInfo[i].type == 1)
            {
                doRecover(battleField, banishInfo[i].continuousCount);
            }
            

            printGems(battleField->gems, MAX_GEMS);
            printf("\n");
        }
    
        // 黒gem（＝ブランク部分）を一番右側へ移動させる。
        int newEndGemNum = shiftGems(continuousChunkCount, battleField, banishInfo);

        // 黒gem部分を色付きgemで埋める。
        fillGems(battleField->gems, (newEndGemNum + 1), MAX_GEMS);
        printGems(battleField->gems, MAX_GEMS);
        printf("\n");
    }
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

    int startNum = command[0] - A_NUMBER;
    int endNum = command[1] - A_NUMBER;

    moveGem(startNum, endNum, battleField->gems, true);

    evaluateGems(battleField);
}

void doEnemyAttack(BattleField* battleField)
{
    int damage = battleField->enemyMonsterAddr->attack - battleField->party->avgDefence;

    // ダメージを±10%増加させる。
    blurPower(&damage);

    if (damage <= 0)
    {
        damage = 1;
    }

    printf("%dのダメージを受けた。\n\n", damage);
    (*battleField).party->sumHp -= damage;
}

void onEnemyTurn(BattleField* battleField)
{
    printf("【%sのターン】\n", (*battleField).enemyMonsterAddr->name);
    doEnemyAttack(battleField);
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
        fillGems(gems, 0, MAX_GEMS);
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
        {"玄武", 150, 150, WATER, 20, 15},
        {"青龍", 150, 150, WIND, 15, 10},
        {"白虎", 150, 150, EARTH, 20, 5}
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

void fillGems(char* gems, int startGemNum, int endGemNum)
{
    for (int i = 0; i < endGemNum; i++)
    {
        gems[i + startGemNum] = rand() % 4 + 1;
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

void moveGem(int firstGemNum, int secondGemNum, char* gems, bool isPrint)
{
    char fomerGems[1024];
    strcpy(fomerGems, &gems[firstGemNum]);

    swapGem(fomerGems, firstGemNum, secondGemNum, gems, isPrint);
}

void swapGem(char* fomerGems, int startGemNum, int endGemNum, char* gems, bool isPrint)
{
    // NOTE::formerGemsの参照元配列の先頭に、移動するgem情報が格納されている。

    if(startGemNum < endGemNum)
    {
        // gemを右へ動かす。
        for (int i = startGemNum; i < endGemNum; i++)
        {
            gems[i] = gems[i + 1];
            gems[i + 1] = fomerGems[0];

            // FixMe::gemを1つずつ右へずらす前提で実装していて、複数gemを一気にずらすことを
            // 想定していなかったため、isPrintというフラグで1個ずつ動く様を表示する・しないを
            // 指定するようにした。が、本来は複数gemが一気にずれるのが良い。
            if(isPrint)
            {
                printGems(gems, MAX_GEMS);
                printf("\n");
            }
        }
    }
    else
    {
        // gemを左へ動かす。
        for (int i = startGemNum; i > endGemNum; i--)
        {
            gems[i] = gems[i - 1];
            gems[i - 1] = fomerGems[0];

            // FieMe::上に同じ。
            if(isPrint)
            {
                printGems(gems, MAX_GEMS);
                printf("\n");
            }
        }
    }
}

// -10%〜+10% の値をpowerに加算する。
void blurPower(int* power)
{
    int fluctationCoefficient = rand() % 21 + 90;

    *power = *power * fluctationCoefficient / 100;
}

void amplifyPower(int* power, int continuousGemsCount, int combo)
{
    float multiplier = 1.5;

    for (int i = 1; i < (continuousGemsCount - 3 + 1); i++)
    {
        multiplier = multiplier * 1.5;
    }

    *power = *power * (int)multiplier;
}

void doRecover(BattleField* battleField, int continuousCount)
{
    int chargePower = 20;

    // 宝石消滅数とコンボ数に応じてパワーを増幅させる。
    amplifyPower(&chargePower, continuousCount, 1);

    // パワーを±10%増加させる。
    blurPower(&chargePower);

    battleField->party->sumHp += chargePower;

    if (battleField->party->sumHp > battleField->party->sumMaxHp)
    {
        battleField->party->sumHp = battleField->party->sumMaxHp;
    }
    
    printf("HPを%d回復。\n", chargePower);
}
