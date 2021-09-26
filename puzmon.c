/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/
# include <stdio.h>

/*** 列挙型宣言 ***/
enum {
    FIRE,
    WATER,
    WIND,
    EARTH
};

/*** グローバル定数の宣言 ***/
const char TYPE_SYMBOLS[] = {
    '$',
    '~',
    '@',
    '#'
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
} ENEMY_MONSTER;


/*** プロトタイプ宣言 ***/

/*** 関数宣言 ***/
void doBattle(char* nameAdr)
{
    printf("%sが現れた！\n", nameAdr);
    printf("%sを倒した！\n", nameAdr);
}

int goDungeon(char* person)
{
    const int MONSTER_COUNT = 5;
    printf("%sはダンジョンに到着した。\n", person);
    
    ENEMY_MONSTER monster[MONSTER_COUNT] = {
        {"スライム",    100, 100, 1, 10, 5},
        {"ゴブリン",    200, 200, 3, 20, 15},
        {"オオコウモリ", 300, 300, 2, 30, 25},
        {"ウェアウルフ", 400, 400, 2, 40, 30},
        {"ドラゴン",    800, 800, 0, 50, 40}
    };

    for (int i = 0; i < MONSTER_COUNT; i++)
    {
        doBattle(monster[i].name);
    }
    
    printf("%sはダンジョンを制覇した！\n", person);
    return MONSTER_COUNT;
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

    int downedMonsterCount = goDungeon(player);
    
    printf("*** GAME CREARED! ***\n");
    printf("倒したモンスター数＝%d\n", downedMonsterCount);
    
    return 0;
}

/*** ユーティリティ関数宣言 ***/
