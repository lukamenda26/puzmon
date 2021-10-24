# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include "utilities.h"

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

// -20%〜+20% の値をpowerに加算する。
void blurPower(int* power)
{
    int fluctationCoefficient = rand() % 41 + 90;

    *power = *power * fluctationCoefficient / 100;
}

void amplifyPower(int* power, int continuousGemsCount, int combo)
{
    float multiplier = 1.5;

    for (int i = 1; i < (continuousGemsCount - 3 + combo); i++)
    {
        multiplier = multiplier * 1.5;
    }

    *power = *power * (int)multiplier;
}

void doRecover(BattleField* battleField, int continuousCount, int* combo)
{
    int chargePower = 20;

    // 宝石消滅数とコンボ数に応じてパワーを増幅させる。
    amplifyPower(&chargePower, continuousCount, *combo);

    // パワーを±10%増加させる。
    blurPower(&chargePower);

    battleField->party->sumHp += chargePower;

    if (battleField->party->sumHp > battleField->party->sumMaxHp)
    {
        battleField->party->sumHp = battleField->party->sumMaxHp;
    }
    
    printf("HPを%d回復。\n\n", chargePower);
}