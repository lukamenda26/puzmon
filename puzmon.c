/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/
# include <stdio.h>
# include <string.h>

/*** 列挙型宣言 ***/
enum {
    FIRE, // 1
    WATER, // 6
    WIND, // 2
    EARTH // 3
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
    Monster* monsterAddr;
    int MonsterCount;  
} Dungeon;


/*** グローバル定数の宣言 ***/
const Elements ELEMENTS[] = {
    {"$", 1},
    {"~", 6},
    {"@", 2},
    {"#", 3}
};

/*** プロトタイプ宣言 ***/
void printMonsterName(Monster*);

/*** 関数宣言 ***/
void doBattle(Monster* monster)
{
    printMonsterName(monster);
    printf("が現れた！\n");
    printMonsterName(monster);
    printf("を倒した！\n");
}

int goDungeon(char* person)
{
    printf("%sはダンジョンに到着した。\n", person);

    Monster monster[] = {
        {"スライム",    100, 100, 1, 10, 5},
        {"ゴブリン",    200, 200, 3, 20, 15},
        {"オオコウモリ", 300, 300, 2, 30, 25},
        {"ウェアウルフ", 400, 400, 2, 40, 30},
        {"ドラゴン",    800, 800, 0, 50, 40}
    };

    Dungeon dungeon = {monster, 5};

    for (int i = 0; i < dungeon.MonsterCount; i++)
    {
        doBattle(&monster[i]);
    }
    
    printf("%sはダンジョンを制覇した！\n", person);
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

    int downedMonsterCount = goDungeon(player);
    
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
