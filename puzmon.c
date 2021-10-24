/**
 * @file puzmon.c
 * @brief メイン実行ファイル
 * @author lukamenda26
 * @date 2021/10/25
 * @details ダンジョンへ行ってバトルをする。
*/

/*** インクルード宣言 ***/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include "declaration.h"

/**
 * @brief organizeParty
 * @param player プレイヤー名。
 * @param monster パーティーを構成するモンスター構造体配列の先頭アドレス。
 * @return Party 生成されたパーティー構造体。
 * @details パーティーを生成する。
*/
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

/**
 * @brief showParty
 * @param party 表示するパーティー構造体の先頭アドレス。
 * @return void
 * @details パーティー情報をコンソールコマンド上に表示する。
 * 
*/
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

/**
 * @brief doBattle
 * @param battleField BattleField構造体（バトルフィールド情報を格納）の先頭アドレス。
 * @return int 倒したモンスターの数。
 * @details プレイヤーターンと敵ターンを交互に実行してバトル処理の流れを担う。
 */
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

/**
 * @brief goDungeon
 * @param party パーティー構造体（＝パーティー情報）。
 * @param dungeon ダンジョン構造体（＝今いるダンジョンの情報）。
 * @return int 倒したモンスターの数。
 * @details ダンジョンへ行ってバトルを行う一連の流れを担う。
*/
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

/**
 * @brief main
 * @param argc
 * @param argv 実行時に入力されたプレイヤー名。
 * @return int
 * @details メイン実行関数。
 */
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

