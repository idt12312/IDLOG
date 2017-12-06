#include <stdio.h>
#include <stdlib.h>

#include "module1.h"
#include "module2.h"

// このモジュール(ファイル)内でのIDLOGの設定
// これらのマクロは必ずidlog.hをincludeするよりも先に定義する
#define IDLOG_ENABLE
#define IDLOG_MODULE_NAME "main"
#define IDLOG_LEVEL IDLOG_LEVEL_WARNING
#include "idlog.h"

// IDLOG_LEVEL=IDLOG_LEVEL_WARNINGと定義されているので、
// このモジュール内のERROR WARNINGのログが出力される
// INFO DEBUGのログは出力されない


// ダミーのタイムスタンプ用のカウンタ
static uint32_t time_cnt = 1000;


// ログ出力用関数
void log_output(const uint8_t *data, size_t len)
{
    (void)len;
    
    // 標準出力にdataから受け取った文字列を出力する
    printf("%s", data);
}

// タイムスタンプ取得用関数
uint32_t log_timestamp()
{
    // 適当な数字を返す
    return time_cnt++;
}


int main()
{   
    // idlogの初期化
    // 実際には...
    // log_outputはUARTから文字列を出力する関数、
    // log_timestampはマイコン内部のtick値などを返す関数に置き換える
    idlog_internal_init(log_output, log_timestamp);

    // ログメッセージの出力
    IDLOG_ERROR("error message");
    IDLOG_WARNING("warning message");
    IDLOG_INFO("info message");
    IDLOG_DEBUG("debug message");

    // バイナリデータの16進数ダンプ
    const uint8_t test_data[] = {0x11, 0x22, 0x33, 0xAA, 0xBB};
    IDLOG_INFO_HEXDUMP(test_data, 5);

    // 他のモジュールの呼び出し
    module1_proc();
    module2_proc();

    return 0;
}
