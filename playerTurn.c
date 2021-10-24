# include <stdio.h>
# include <stdbool.h>
# include "declaration.h"

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

void doAttack(BattleField* battleField, Monster* attackMonster, int continuousCount, int* combo)
{
    const double ATTRIBUTE = ELEMENT_BOOST[attackMonster->type - 2][battleField->enemyMonsterAddr->type - 2];
    int damage = (int)((attackMonster->attack - battleField->enemyMonsterAddr->defence) * ATTRIBUTE);

    // 宝石消滅数とコンボ数に応じてダメージを増幅させる。
    amplifyPower(&damage, continuousCount, *combo);

    // ダメージを±10%増加させる。
    blurPower(&damage);

    if (damage <= 0)
    {
        damage = 1;
    }
    

    printf("ダミー攻撃で%dのダメージを与えた。\n\n", damage);
    (*battleField).enemyMonsterAddr->hp -= damage;
}

bool checkBanishable(char* gems, BanishInfo* banishInfo, int startEmptyNum)
{
    // 最後に3連続以上があった場合に必ず黒消しを行えるよう、
    // 14つのgemの最後尾+1の配列格納場所に黒gemを格納。
    gems[startEmptyNum] = EMPTY;

    bool shouldBanish = false;
    int initialNum = EMPTY;
    int continuousCount = 1;

    for (int i = 1; i <= startEmptyNum; i++)
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
                banishInfo->type = initialNum;
                banishInfo->startContinuousAddr = &gems[i - continuousCount];
                banishInfo->continuousCount = continuousCount;

                shouldBanish = true;
                break;
            }
            continuousCount = 1;
            continue;
        }
        
    }

    return shouldBanish;
}

int shiftGems(BattleField* battleField, BanishInfo* banishInfo)
{
    // 黒gemの動かす先 ＝ gem配列の一番右側。
    int gemDestinationNum = MAX_GEMS - 1;

    // 最初に動かす黒gemの番号の初期値を設定。
    int moveGemNum = MAX_GEMS - 1;

    // 最初に動く黒gemのアドレス指定用変数。
    int movedGemCount = 0;

    // for (int i = 0; i < continuousChunkCount; i++)
    // {
        moveGemNum = (int)(banishInfo->startContinuousAddr + banishInfo->continuousCount - 1 - battleField->gems - movedGemCount);

        if (moveGemNum < gemDestinationNum)
        {
            for (int j = 0; j < banishInfo->continuousCount; j++)
            {
                moveGem((moveGemNum - j), (gemDestinationNum - j), battleField->gems, false);
                printGems(battleField->gems, MAX_GEMS);
                printf("\n");
            }
        }
        
        // 黒gemの動かし先番号を書き換える。
        gemDestinationNum = gemDestinationNum - banishInfo->continuousCount;

        movedGemCount += banishInfo->continuousCount;

        // printf("最初に動かしたgem：%d 次に黒gemが動くとしたら%d番目へ動く。\n", (int)(banishInfo->startContinuousAddr + banishInfo->continuousCount - 1 - battleField->gems), gemDestinationNum);
    // }

    return gemDestinationNum;
}

bool evaluateGems(BattleField* battleField, BanishInfo* banishInfo, int startEmptyNum, int* combo)
{
    // gemが1列当たり並ぶ列 ÷ 3 + 1 以上の配列は入りようがない。
    // int maxContinuous = MAX_GEMS / 3 + 1;
    // BanishInfo banishInfo[maxContinuous];

    bool shouldBanish = checkBanishable(battleField->gems, banishInfo, startEmptyNum);

    if (shouldBanish)
    {
        *combo = *combo + 1;
        
        // for (int i = 0; i < continuousChunkCount; i++)
        // {
            for (int j = 0; j < banishInfo->continuousCount; j++)
            {
                *(banishInfo->startContinuousAddr + j) = 0;
            }

            printGems(battleField->gems, MAX_GEMS);
            printf("\n");

            if (*combo > 1)
            {
                printf("%dコンボ!!\n", *combo);
            }

            if (banishInfo->type > 1)
            {
                for (int k = 0; k < PARTY_MONSTER_COUNT; k++)
                {
                    if (battleField->party->partyMonsterAddr[k].type == banishInfo->type)
                    {
                        printMonsterName(&battleField->party->partyMonsterAddr[k]);
                        printf("の攻撃!!\n");
                        doAttack(battleField, &battleField->party->partyMonsterAddr[k], banishInfo->continuousCount, combo);
                    }
                }
            }

            if (banishInfo->type == 1)
            {
                doRecover(battleField, banishInfo->continuousCount, combo);
            }
            

            printGems(battleField->gems, MAX_GEMS);
            printf("\n");
        // }
    }

    // 宝石消滅がある＝宝石の空き詰めが必要。
    return shouldBanish;
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

    // gem消滅情報を格納する構造体を定義。
    BanishInfo newBanishInfo;

    int newEndGemNum = MAX_GEMS - 1;
    int combo = 0;

    bool shouldFillUp = evaluateGems(battleField, &newBanishInfo, MAX_GEMS, &combo);

    while (shouldFillUp)
    {
        // 黒gem（＝ブランク部分）を一番右側へ移動させる。
        newEndGemNum = shiftGems(battleField, &newBanishInfo);

        // 黒gem部分を色付きgemで埋める。
        fillGems(battleField->gems, (newEndGemNum + 1), MAX_GEMS);
        printGems(battleField->gems, MAX_GEMS);
        printf("\n");

        shouldFillUp = evaluateGems(battleField, &newBanishInfo, MAX_GEMS, &combo);
    }
}