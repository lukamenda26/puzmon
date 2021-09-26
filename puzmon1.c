/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/

# include <stdio.h>

/*** 列挙型宣言 ***/

/*** グローバル定数の宣言 ***/

/*** 構造体型宣言 ***/
typedef struct 
{
    char name[1024];
} MONSTER;


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
    MONSTER monster[MONSTER_COUNT] = {
        "スライム",
        "ゴブリン",
        "オオコウモリ",
        "ウェアウルフ",
        "ドラゴン"
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
