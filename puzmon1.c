/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/

# include <stdio.h>

/*** 列挙型宣言 ***/

/*** グローバル定数の宣言 ***/

/*** 構造体型宣言 ***/

/*** プロトタイプ宣言 ***/

/*** 関数宣言 ***/
void goDungeon(char* person)
{
    printf("%sはダンジョンに到着した。\n", person);
    printf("%sはダンジョンを制覇した！\n", person);
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

    goDungeon(player);
    
    printf("*** GAME CREARED! ***\n");
    printf("倒したモンスター数＝5\n");
    
    return 0;
}

/*** ユーティリティ関数宣言 ***/
