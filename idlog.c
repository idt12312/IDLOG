#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>

#include "idlog.h"


// 出力用関数
static idlog_output_func_t output_func = NULL;
// タイムスタンプ取得用関数
static idlog_timestamp_func_t timestamp_func = NULL;

// ログ出力用文字列を一時的に格納するバッファ
static char strbuf[IDLOG_BUFFER_SIZE];


// idlogの初期化関数
// 出力関数とタイムスタンプを取得する関数を設定する
void idlog_internal_init(const idlog_output_func_t _output_func, const idlog_timestamp_func_t _timestamp_func)
{
    output_func = _output_func;
    timestamp_func = _timestamp_func;
}


// 文字列を成形して出力する関数
// prefixの後ろにfmtで指定した文字列を連結し、output_funcに出力する
void idlog_internal_log(const char* prefix, const char* fmt, ...)
{
    if (!output_func) {
        return;
    }

    // バッファにprefixを追加する
    // タイムスタンプ関数が設定されている時はタイムスタンプをprefixの後に追加
    size_t prefix_wrote_size;
    if (timestamp_func) {
        prefix_wrote_size = snprintf(strbuf, IDLOG_BUFFER_SIZE, "%s %"PRIu32" ", prefix, timestamp_func());
    }
    else {
        prefix_wrote_size = snprintf(strbuf, IDLOG_BUFFER_SIZE, "%s ", prefix);
    }
    
    const size_t suffix_size = strlen(IDLOG_SUFFIX);
    const size_t remain_buffer_size = IDLOG_BUFFER_SIZE-prefix_wrote_size-suffix_size;

    // フォーマット文字列の部分をバッファに追加
    va_list vlist;
    va_start(vlist, fmt);
    const size_t content_wrote_size = vsnprintf(strbuf+prefix_wrote_size, remain_buffer_size, fmt, vlist);
    va_end(vlist);

    // バッファにsuffixを追加
    strcat(strbuf, IDLOG_SUFFIX);

    output_func((const uint8_t*)strbuf, prefix_wrote_size+content_wrote_size+suffix_size);
}


void idlog_internal_hexdump(const char* prefix, const uint8_t *data, size_t len)
{
    if (!output_func) {
        return;
    }

    // バッファにprefixを追加する
    // タイムスタンプ関数が設定されている時はタイムスタンプをprefixの後に追加
    size_t prefix_wrote_size;
    if (timestamp_func) {
        prefix_wrote_size = snprintf(strbuf, IDLOG_BUFFER_SIZE, "%s %"PRIu32" ", prefix, timestamp_func());
    }
    else {
        prefix_wrote_size = snprintf(strbuf, IDLOG_BUFFER_SIZE, "%s ", prefix);
    }
    
    const size_t suffix_size = strlen(IDLOG_SUFFIX);
    const size_t remain_buffer_size = IDLOG_BUFFER_SIZE-prefix_wrote_size-suffix_size;

    // dataに入っているデータを長さlen、もしくはバッファに入るだけ16進数文字列に変換してバッファに追加
    for (size_t i=0;i<len&&i<remain_buffer_size/3-1;i++) {
        sprintf(strbuf+prefix_wrote_size+i*3, "%02X ", data[i]);
    }

    // バッファにsuffixを追加
    strcat(strbuf, IDLOG_SUFFIX);

    output_func((const uint8_t*)strbuf, prefix_wrote_size+len*3+suffix_size);
}
