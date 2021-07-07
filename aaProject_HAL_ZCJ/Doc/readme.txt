
本例程演示如何使用串口调试助手测试WF-ESP8266 WiFi模块

通过串口调试助手配置模块，并与外界进行数据传输

ESP8266 作为 STA TCP Client 向网络助手上传 DHT11 温湿度



AT+GMRAT version:0.22.0.0(Mar 20 2015 10:04:26)
SDK version:1.0.0
compile time:Mar 20 2015 11:00:32

OK

TTL 红色5v 黑色 GND 绿色 RXD 白色TXD
PA10 U1RXD PA9 U1TXD
ESP8266 引脚1 接RX 引脚8 接TX 357接3.3  2 6接地
stlink 绿色接板左4 蓝色左5 红色 左8
DS18B20	PINB10
pwm输出 pb8
引脚连接：
UTXD2  --- A3---紫 -------
URXD2  --- A2---蓝 ---------------
VCC   --- 3V3--红
GND   --- GND--黑
CH_PD --- A0---白
RST   --- A1---灰

//串口1连接电脑端
USART1_TX    PA9
USART1_RX    PA10
	