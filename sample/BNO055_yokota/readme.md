BNO055 サンプルコード
====
written by yokota

## 概要
Arduino 9軸モーションシールド用のサンプルコードを改造して作成。

~~「動いたときInterrupt」させたり「動いていないときInterrupt」させたりできる。~~

~~センサに設定したInterruptのタイミングでINT_PINがHIGHになる。~~

update(2017/12/14)

Interruptの機能を作り直し、重力方向が一定時間デフォルトと異なっている場合にTrue Falseが変わる関数(isWearerArrived())に置き換え。

## 使用ライブラリ
(arduino-org/arduino-library-nine-axes-motion)

https://github.com/arduino-org/arduino-library-nine-axes-motion

※ArduinoIDE 1.7以前ならデフォルトで入ってるらしい

※ESPでの開発時はGCCのエラーが出るためライブラリの書き換えが必要



(Switch Science 製品ページ https://www.switch-science.com/catalog/2213/ )
