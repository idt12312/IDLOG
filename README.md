# IDLOG
マイコンなどで簡単にログを出力するためのライブラリ


## 使用例
ソースコード中にこのようなコードを書きます。

```c
#define IDLOG_ENABLE
#define IDLOG_MODULE_NAME "module1"
#define IDLOG_LEVEL IDLOG_LEVEL_DEBUG
#include "idlog.h"

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
```


すると次のような出力を得ることができます

```
[module1:DBG] 1001 Enter module1_proc() module1.c:19
[module1:ERR] 1002 error message
[module1:WRN] 1003 warning message
[module1:INF] 1004 info message
[module1:DBG] 1005 debug message
[module1:INF] 1006 11 22 33 AA BB
[module1:DBG] 1007 Exit module1_proc() module1.c:32
```

※1001や1002といった数字はタイムスタンプを意味しています。


### 特徴1:便利な表示機能
次の3つの形式でログを表示することができます。

* printfのようなフォーマット付き文字列の表示
* バイナリデータの16進数表示
* 関数に入った、出たの表示


### 特徴2:モジュール名を表示可能
そのログがどのモジュール(ファイル)から出力されたものなのかを知ることができます。

例えばログを"module1"というモジュールから出力されたと表示するには、
IDLOG_MODULE_NAMEにモジュール名を設定します。

ソース

```c
#define IDLOG_MODULE_NAME "module1"

IDLOG_ERROR("error message");
```

出力

```
[module1:ERR] error message
```


### 特徴3:ログの出力レベルを設定可能
モジュールごとに出力するログのレベルをファイルごとに設定することができます。
ログのレベルにはERROR WARNING INFO DEBUGがあります。

開発の段階に合わせて余計なログを消したり、逆に詳細にログを表示したり、簡単に切り替えることができます。

例えば表示レベルをWARNINGに設定すると、ERRORとWARNINGのログは出力されますが、
INFOとDEBUGのログは出力されません。

ソース

```c
#define IDLOG_LEVEL IDLOG_LEVEL_WARNING

IDLOG_ERROR("error message");
IDLOG_WARNING("warning message");
IDLOG_INFO("info message");
IDLOG_DEBUG("debug message");
```

出力

```
[:ERR] error message
[:WRN] warning message
```

### 特徴4:出力関数、タイムスタンプ関数を設定可能
IDLOGの初期化時にログを出力する先の関数と、タイムスタンプを取得する関数を設定することができます。

例えばマイコンのプロジェクトにおいて次のような出力関数をIDLOGに設定すると、
ログ文字列をUARTから出力することができます。

```c
void log_output_uart(const uint8_t* data, uint8_t len)
{
    // UARTからdataにあるデータをサイズlenだけ出力する処理
    Uart_write(data, len);
}
```

また、タイムスタンプを取得する関数を以下のような関数にIDLOGに設定すると、
ログにマイコン内部のTickを含めて表示することができます。

```c
uint32_t log_timestamp_tick()
{
    // マイコンのTickを返す関数
    return Tick;
}
```


## 使い方
具体的な例は以下のものや、[exampleディレクトリのコード](https://github.com/idt12312/IDLOG/tree/master/example)も参考にしてください。

### 使用例
```c
#include <stdio.h>

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

    return 0;
}
```

### インストール方法
IDLOGを使用したいプロジェクトにidlog.cとidlog.hを含めてビルドするだけです。

### idlog.hの設定
基本的にはいじらなくてもいいような気がしますが、用途に合わせて次の設定を適宜修正してください

#### IDLOG_BUFFER_SIZE
内部で使用するバッファーのサイズ。
このサイズのメモリが静的に確保されます。

#### IDLOG_SUFFIX
ログ出力の1行の終わりの文字。
大抵の場合改行文字を設定します。

#### IDLOG_DEFAULT_LEVEL
デフォルトのログレベル。
明示的にどのログレベルまで表示するのかが設定されなかったときのデフォルト値です。


### idlog.hをincludeしたファイルでの設定
次のマクロを**idlog.hをincludeするよりも上の行で**定義してください。
これらの設定はそのファイル内のみで有効です。

#### IDLOG_ENABLE
ログ出力をするかしないかの設定。

このマクロは値を設定しないくても、定義するだけで十分です。
定義するとIDLOGの機能が有効になり、定義しないと全てのログが出力されなくなります。

#### IDLOG_MODULE_NAME
ログ出力時のモジュール名。

定義しないとidlog.hにあるデフォルト値が使われます。

#### IDLOG_LEVEL
ログをどこまで詳細に表示するかの設定。

IDLOG_LEVEL_ERROR、IDLOG_LEVEL_WARNING、IDLOG_LEVEL_INFO、IDLOG_LEVEL_DEBUGのいずれかの値を設定します。

定義しないとidlog.hにあるデフォルト値が使われます。


### IDLOGの初期化関数

```c
void idlog_internal_init(const idlog_output_func_t _output_func, const idlog_timestamp_func_t _timestamp_func);
```

IDLOGの機能を使う前に1度だけ呼び出して初期化する必要があります。

引数の_output_funcはログを出力するための関数へのポインタで、idlog_output_func_tは以下のように定義されています。

```c
typedef void (*idlog_output_func_t)(const uint8_t *data, size_t len);
```

引数の_timestamp_funcはタイムスタンプを取得する関数へのポインタで、idlog_timestamp_func_tは以下のように定義されています。

```c
typedef uint32_t (*idlog_timestamp_func_t)();
```

ログの出力関数の設定は必須ですが、タイムスタンプを取得する関数はNULLを設定することもできます。
その場合、タイムスタンプが表示されなくなります。

### ログの出力API
以下の説明でのLEVELはERROR、WARNING、INFO、DEBUGに置き換えて使用してください。

#### フォーマットされた文字列を表示

```c
void IDLOG_LEVEL(const char* fmt, ...);
```

ログレベルLEVELで整形された文字列を表示します。
引数はprintfと同じ感じです。


#### バイナリデータの16進数ダンプ

```c
void IDLOG_LEVEL_HEXDUMP(const uint8_t* data, size_t len);
```

ログレベルLEVELで長さlenのバイトデータdataを16進数表示します。
dataはuint8_t*で渡してください。

#### 関数の入出表示

```c
void IDLOG_LEVEL_FUNCTION_ENTER();
void IDLOG_LEVEL_FUNCTION_EXIT();
```

ログレベルLEVELで関数に入った、出たことを表示します。
このマクロを呼び出したファイル名と関数名が表示されます。

