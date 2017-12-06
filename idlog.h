#ifndef IDLOG_H
#define IDLOG_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>


/****************************************
 * ユーザー設定
 ****************************************/
// 内部で使用するバッファーのサイズ
// このサイズのメモリが静的に確保される
#define IDLOG_BUFFER_SIZE   128

// ログ出力の1行の終わりの文字(おそらく改行文字で十分)
#define IDLOG_SUFFIX    "\r\n"

// デフォルトのログレベル(レベルの定義は下記)
#define IDLOG_DEFAULT_LEVEL IDLOG_LEVEL_WARNING
/****************************************/


/****************************************
 * デフォルト設定
 *  - 通常はこのファイル(idlog.h)をincludeする
 *    ファイルで定義される
 *  - その定義がない時のためのデフォルト値
 ****************************************/
// ERROR WARNING INFO DEBUG、どのレベル以上を表示するかの設定
#ifndef IDLOG_LEVEL
#define IDLOG_LEVEL IDLOG_DEFAULT_LEVEL
#endif

// ログ出力の先頭に付ける、ログ出力マクロが呼び出されたモジュール名
#ifndef IDLOG_MODULE_NAME
#define IDLOG_MODULE_NAME   ""
#endif


/****************************************
 * 出力される文字列の設定
 ****************************************/
// ログレベルの定義
#define IDLOG_LEVEL_ERROR   1
#define IDLOG_LEVEL_WARNING 2
#define IDLOG_LEVEL_INFO    3
#define IDLOG_LEVEL_DEBUG   4

// ログの先頭に付ける文字列
// ログレベルによって変えている
#define IDLOG_ERROR_PREFIX      "[" IDLOG_MODULE_NAME ":ERR]"
#define IDLOG_WARNING_PREFIX    "[" IDLOG_MODULE_NAME ":WRN]"
#define IDLOG_INFO_PREFIX       "[" IDLOG_MODULE_NAME ":INF]"
#define IDLOG_DEBUG_PREFIX      "[" IDLOG_MODULE_NAME ":DBG]"

// ソースファイルのファイル名を取得するためのマクロ
#define STRIPPED_FILE_NAME          (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// 関数に入った時、出た時のログ出力を出すための文字列の定義
#define IDLOG_ENTER_FUNCTION_ARG    "Enter %s() %s:%d", __func__, STRIPPED_FILE_NAME ,__LINE__
#define IDLOG_EXIT_FUNCTION_ARG     "Exit %s() %s:%d", __func__, STRIPPED_FILE_NAME ,__LINE__


/****************************************
 *  ログ出力用API
 *  - LEVELごとに専用のマクロが用意されている 
 ****************************************/
#if defined(IDLOG_ENABLE) && IDLOG_LEVEL >= IDLOG_LEVEL_ERROR
#define IDLOG_ERROR(...)                    idlog_internal_log(IDLOG_ERROR_PREFIX, __VA_ARGS__)
#define IDLOG_ERROR_HEXDUMP(data,len)       idlog_internal_hexdump(IDLOG_ERROR_PREFIX, data, len)
#define IDLOG_ERROR_FUNCTION_ENTER()        idlog_internal_log(IDLOG_ERROR_PREFIX, IDLOG_ENTER_FUNCTION_ARG)
#define IDLOG_ERROR_FUNCTION_EXIT()         idlog_internal_log(IDLOG_ERROR_PREFIX, IDLOG_EXIT_FUNCTION_ARG)
#else
#define IDLOG_ERROR(...)
#define IDLOG_ERROR_HEXDUMP(data,len)       (void)data; (void)len;
#define IDLOG_ERROR_FUNCTION_ENTER()
#define IDLOG_ERROR_FUNCTION_EXIT()
#endif

#if defined(IDLOG_ENABLE) && IDLOG_LEVEL >= IDLOG_LEVEL_WARNING
#define IDLOG_WARNING(...)                  idlog_internal_log(IDLOG_WARNING_PREFIX, __VA_ARGS__)
#define IDLOG_WARNING_HEXDUMP(data,len)     idlog_internal_hexdump(IDLOG_WARNING_PREFIX, data, len)
#define IDLOG_WARNING_FUNCTION_ENTER()      idlog_internal_log(IDLOG_WARNING_PREFIX, IDLOG_ENTER_FUNCTION_ARG)
#define IDLOG_WARNING_FUNCTION_EXIT()       idlog_internal_log(IDLOG_WARNING_PREFIX, IDLOG_EXIT_FUNCTION_ARG)
#else
#define IDLOG_WARNING(...)
#define IDLOG_WARNING_HEXDUMP(data,len)     (void)data; (void)len;
#define IDLOG_WARNING_FUNCTION_ENTER()
#define IDLOG_WARNING_FUNCTION_EXIT()
#endif

#if defined(IDLOG_ENABLE) && IDLOG_LEVEL >= IDLOG_LEVEL_INFO
#define IDLOG_INFO(...)                     idlog_internal_log(IDLOG_INFO_PREFIX, __VA_ARGS__)
#define IDLOG_INFO_HEXDUMP(data,len)        idlog_internal_hexdump(IDLOG_INFO_PREFIX, data, len)
#define IDLOG_INFO_FUNCTION_ENTER()         idlog_internal_log(IDLOG_INFO_PREFIX, IDLOG_ENTER_FUNCTION_ARG)
#define IDLOG_INFO_FUNCTION_EXIT()          idlog_internal_log(IDLOG_INFO_PREFIX, IDLOG_EXIT_FUNCTION_ARG)
#else
#define IDLOG_INFO(...)
#define IDLOG_INFO_HEXDUMP(data,len)        (void)data; (void)len;
#define IDLOG_INFO_FUNCTION_ENTER()
#define IDLOG_INFO_FUNCTION_EXIT()
#endif

#if defined(IDLOG_ENABLE) && IDLOG_LEVEL >= IDLOG_LEVEL_DEBUG
#define IDLOG_DEBUG(...)                    idlog_internal_log(IDLOG_DEBUG_PREFIX, __VA_ARGS__)
#define IDLOG_DEBUG_HEXDUMP(data,len)       idlog_internal_hexdump(IDLOG_DEBUG_PREFIX, data, len)
#define IDLOG_DEBUG_FUNCTION_ENTER()        idlog_internal_log(IDLOG_DEBUG_PREFIX, IDLOG_ENTER_FUNCTION_ARG)
#define IDLOG_DEBUG_FUNCTION_EXIT()         idlog_internal_log(IDLOG_DEBUG_PREFIX, IDLOG_EXIT_FUNCTION_ARG)
#else
#define IDLOG_DEBUG(...)
#define IDLOG_DEBUG_HEXDUMP(data,len)       (void)data; (void)len;
#define IDLOG_DEBUG_FUNCTION_ENTER()
#define IDLOG_DEBUG_FUNCTION_EXIT()
#endif


/****************************************
 *  コールバック用関数の定義
 ****************************************/
// ログデータを出力するための関数の定義
// [引数]
//  data : 出力すべきデータ(文字列)の先頭へのポインタ
//  len  : 出力すべきデータのサイズ(byte)
// [戻り値]なし
typedef void (*idlog_output_func_t)(const uint8_t *data, size_t len);

// タイムスタンプを表示するための時刻を取得する関数の定義
// [引数]なし
// [戻り値] uint32_t 時刻(マイコン内部のtickなど)
typedef uint32_t (*idlog_timestamp_func_t)();


/****************************************
 *  idlogの初期化関数
 *  - idlogを使用する前にこの関数を1度だけ呼び出して初期化をする
 ****************************************/
// 初期化関数
// [引数]
//  _output_func : ログを出力する関数を設定する
//  _timestamp_func : タイムスタンプを取得する関数を設定する
//                     NULLを設定するとタイムスタンプが出力されない(タイムスタンプを表示しない時はNULLにする)
void idlog_internal_init(const idlog_output_func_t _output_func, const idlog_timestamp_func_t _timestamp_func);


/****************************************
 *  内部用関数
 *  - ユーザーがこの関数を明示的に呼び出すことはない
 ****************************************/
// 文字列出力用
void idlog_internal_log(const char* prefix, const char* fmt, ...);
// バイナリダンプ用
void idlog_internal_hexdump(const char* prefix, const uint8_t *data, size_t len);


#ifdef	__cplusplus
}
#endif

#endif
