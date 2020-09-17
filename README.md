# esp32_foxhunting
不正APを探すトレーニング用のAP作成プログラム

## 必要なもの。
* Arduino IDE
* ESP32

## 作成方法
inoファイルをダウンロードして、
macaddress,
password,
ssid,
responseHTMLを変更してください。

arduino IDEからESP32のマイコンボードに書き込んでください。

``` cpp
  WiFi.softAP(ssid,pass,NULL,0,4);
```
ここの第４パラメータの0を1に変更すると、hidden SSID（SSIDを送信しなくなります）になります。
