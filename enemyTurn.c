# include <stdio.h>
# include <stdbool.h>
# include "declaration.h"

/** 
 * 敵の攻撃を実施し味方にダメージを与える。
*/
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

/** 
 * 敵のターン。
*/
void onEnemyTurn(BattleField* battleField)
{
    printf("【%sのターン】\n", (*battleField).enemyMonsterAddr->name);
    doEnemyAttack(battleField);
}