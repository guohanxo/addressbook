/*---------------------------*/
/* ヘッダーファイルの読み込み */
/*---------------------------*/
#include <stdio.h>
#include <ctype.h>
#include <ncurses.h>
#include <locale.h>

/*-------------------*/
/* システム定数の宣言 */
/*-------------------*/

#define TURE  1
#define FALSE  0

#define BUFFER_LEN  80 /* 入力バッファの文字数 */

#define RECORD_MAX  20 /* 最大データ件数 */
#define NAME_LEN  40 /* 名前の文字数 */
#define ADDE_LEN  60 /* 住所の文字数 */
#define TEL_LEN   15 /* 電話番号の文字数 */
#define ZIP_LEN   9 /* 郵便番号の文字数 */

#define TITLE_LINE  1 /* タイトル行の位置 */
#define DATA_LINE  3 /* データ表示行の先頭位置 */
#define DISP_REC_MAX  5 /* 1ページに書くデータ件数 */
#define MSG_LINE   18 /* メッセージ行の先頭位置 */

#define UP   1 /* 前頁 */
#define DOWN  0 /* 次頁 */


/* コマンドの定義 */

#define CMD_MAX   4 /* 第1版 */

#define APPEND   'A' /* データの追加 */
#define UP_PAGE   'U' /* 前ページ */
#define DOWN_PAGE  'D' /* 次ページ */
#define QUIT     'Q' /* 終了 */

char cmd_list[] = {
  APPEND,
  UP_PAGE,
  DOWN_PAGE,
  QUIT
};

/*----------------------*/
/* グローバル変数の宣言 */
/*----------------------*/

int cursor = 0; /* 注目行 */
int rec_qty = 0; /* データの数 */

char j_name[RECORD_MAX][NAME_LEN]; /* 名前(漢字) */
char k_name[RECORD_MAX][NAME_LEN]; /* 名前(フリガナ) */
char address[RECORD_MAX][ADDR_LEN]; /* 住所 */
char tel_no[RECORD_MAX][TEL_LEN]; /* 電話番号 */
char zip[RECORD_MAX][ZIP_LEN]; /* 郵便番号 */

/*----------------------*/
/* 使用する関数の宣言 */
/*----------------------*/

/* コマンド処理用部品 */
char input_cmd( void );
void exec_cmd( char cmd );

/* 住所録管理コマンド */
void data_append( void );
void paging( int direction );

void init_buff();

/* 画面表示用部品 */
void init_disp( void );
void show_one_page( void );
void show_record( int num, int pos);

/* main */
/*------------------------------------*/

int main()
{
  char command;

  /* 初期化する */
  init_disp();
  init_buff();
  show_one_page();

  /* コマンドの入力と実行 */
  while(( command = input_cmd() ) != QUIT ){
    exec_cmd( command );
  }

  /* 後始末する */
  clear();
  endwin();
  return 0;
}

/*-------------------*/
/* コマンド処理用部品 */
/*-------------------*/

#define CMD_MSG_LINE MSG_LINE
#define CMD_ERR_LINE MSG_LINE+1
#define CMD_LINE MSG_LINE+2

#define CMD_MSG  "コマンド(A:追加 U:次頁 D:前頁 Q:終了)"
#define CMD_ERR_MSG "不適当なコマンドでした"
#define CMD_PROMPT ">>"

/* input_cmd */
/*---------------------------------*/

char input_cmd()
{
  int cmd;
  int match = FALSE;
  int i;
  char buffer[BUFFER_LEN];

  /* 前処理 */
  move(CMD_MSG_LINE, 1);
  printw(CMD_MSG);

  /* コマンドの入力 */
  for(;;){
    move(CMD_LINE, 1);
    clrtoeol();
    printw(CMD_PROMPT);
    getstr(buffer);
    cmd = toupper(buffer[0]);
    for(i = 0 ; i < CMD_MAX ; i++){
      if( cmd == cmd_list[i] ){
        match = TRUE;
        break;
      }
    }
    if( !match ){
      move(CMD_ERR_LINE, 1);
      printw( CMD_ERR_MSG );
    } else {
      break;
    }
  }

  /* 後始末 */
  move(CMD_MSG_LINE, 1);
  for( i = CMD_MSG_LINE; i <= CMD_LINE; i++ ){
    clrtoeol();
    addch('\n');
  }
  return (cmd);
}

