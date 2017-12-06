#include "module1.h"


// このモジュール(ファイル)内でのIDLOGの設定
// これらのマクロは必ずidlog.hをincludeするよりも先に定義する
#define IDLOG_ENABLE
#define IDLOG_MODULE_NAME "module1"
#define IDLOG_LEVEL IDLOG_LEVEL_DEBUG
#include "idlog.h"

// IDLOG_LEVEL=IDLOG_LEVEL_DEBUGと定義されているので、
// このモジュール内の全てのログが出力される



void module1_proc()
{
    // modeule1.cのmodule1_pricという関数に入ったことをログとして出力する
    IDLOG_DEBUG_FUNCTION_ENTER();
    
    // ログメッセージの出力
    IDLOG_ERROR("error message");
    IDLOG_WARNING("warning message");
    IDLOG_INFO("info message");
    IDLOG_DEBUG("debug message");
    
    // バイナリデータの16進数ダンプ
    const uint8_t test_data[] = {0x11, 0x22, 0x33, 0xAA, 0xBB};
    IDLOG_INFO_HEXDUMP(test_data, 5);

    // modeule1.cのmodule1_pricという関数を出ることをログとして出力する
    IDLOG_DEBUG_FUNCTION_EXIT();
}
