# tracex - DOS CALL Tracer

安倍広多さんが作られたtraceの改造版です。  
無保証につき各自の責任で使用して下さい。


## Usage

環境変数`TRACE_LOG`で出力ファイル名を指定できます。

オプション
* `-o <ファイル名>` ... 標準出力の代わりに指定したファイルにログを出力します。
  ファイル名に`-`(ハイフンのみ)を指定すると標準出力に出力します。
* `-a` ... トレース範囲を全範囲にします。


## Build

PCやネット上での取り扱いを用意にするために、src/内のファイルはUTF-8で記述されています。
X680x0上でビルドする際には、UTF-8からShift_JISへの変換が必要です。

### src2buildを使用する場合

必要ツール: [src2build](https://github.com/kg68k/src2build)

srcディレクトリのある場所で以下のコマンドを実行します。
```
src2build src
make -C build
```

### その他の方法

src/内のファイルを適当なツールで適宜Shift_JISに変換して別のディレクトリに保存し、
ディレクトリ内で`make`を実行してください。  
UTF-8のままでは正しくビルドできません。


## License

TRACE.LZH README.DOCの「著作権」の項より引用：
> 　本プログラムはフリーウェアとします。著作権は留保しますが、コピー・転  
>   載・改造は自由です。ただしアーカイブからファイルを削除しての転載は遠  
>   慮願います。  

改造版である tracex については下記のライセンスとします。  
GNU GENERAL PUBLIC LICENSE Version 3 or later.


## Author

原作者は安倍広多さんです。  
* [安倍 広多 - Kota Abe](https://www.sousei.gscc.osaka-cu.ac.jp/~k-abe/)
* [ktabe (kota abe)](https://github.com/ktabe)

tracexでの改造部分の著作権は改造者(TcbnErik)にあります。

TcbnErik / https://github.com/kg68k/tracex
