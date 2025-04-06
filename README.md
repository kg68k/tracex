# tracex - DOS CALL Tracer

安倍広多さんが作られたtraceの改造版です。  
無保証につき各自の責任で使用して下さい。


## Build

PCやネット上での取り扱いを用意にするために、src/内のファイルはUTF-8で記述されています。
X68000上でビルドする際には、UTF-8からShift_JISへの変換が必要です。

なお、TRACE.LZHにtrace.hファイルが含まれていないためそのままではコンパイルできません。

### u8tosjを使用する方法

あらかじめ、[u8tosj](https://github.com/kg68k/u8tosj)をインストールしておいてください。

トップディレクトリで`make`を実行してください。以下の処理が行われます。
1. `build/`ディレクトリの作成。
3. `src/`内のファイルをShift_JISに変換して`build/`へ保存。

次に、カレントディレクトリを`build/`に変更し、`make`を実行してください。
実行ファイルが作成されます。

### u8tosjを使用しない方法

`src/`内のファイルを適当なツールで適宜Shift_JISに変換してから`make`を実行してください。
UTF-8のままでは正しくビルドできませんので注意してください。


## License

TRACE.LZH README.DOCの「著作権」の項より引用：
> 　本プログラムはフリーウェアとします。著作権は留保しますが、コピー・転  
>   載・改造は自由です。ただしアーカイブからファイルを削除しての転載は遠  
>   慮願います。  


## Author

原作者は安倍広多さんです。  
* [安倍 広多 - Kota Abe](https://www.sousei.gscc.osaka-cu.ac.jp/~k-abe/)
* [ktabe (kota abe)](https://github.com/ktabe)

tracexでの改造部分の著作権は改造者(TcbnErik)にあります。

TcbnErik / https://github.com/kg68k/tracex
