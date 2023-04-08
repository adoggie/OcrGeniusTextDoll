# OcrGeniusTextDoll

百度Ocr识别文字的服务封装，在内网中提供ocr功能，pc 的自动化脚本 pyautoui 模拟人机的鼠标键盘操作，并截屏，发送给ocr服务，识别文字，然后再进行下一步操作。

baidu-ocr-sdk仅支持windows系统， 相关的库包含: ocr , opencv , sdk 并不提供debug版本链接库，所以开发中不能进行debug调试运行。
ocr-sdk (windows) 存在的问题是图文识别的效果不好，识别率低，很多小数点无法识别出来，测试图片: `./images/2.jpg`

这东西不大好玩，由于识别率存在问题，仅仅比默认的 `easyocr` 好一点点，所以兴趣不大了，作为以后工作的辅助工具使用。

> 今天2023.4.8 ，边看片子边搞这个花了3，4小时，没意思。 
> 下午弄了个币安历史行情数据下载的脚本，花了2小时，还不错。

*项目： vs2022 / qt 5.12 64bit* 

本来设想 在windows上跑ocr服务，通过 zmq 进行消息转发，没料到 zmq for windows 的dll 有问题，zmq调用会卡顿在哪里，搞了2小时弄msys2环境，又想着去编译zmq。 

路径 : 
```
  app -> mx -> ocr-server(baidu sdk) -> mx -> recorder -> mongodb 
      push   pull(router/dealer)     pub   sub  
```
不想继续搞zmq的事情了，改变思路 ，直接 ocr-server 代码中使用qt的websocket server 。 

python/ocr-client.py 为测试客户端，可以通过websocket 连接到 ocr-server ，发送图片文件，接收识别结果。

简单测试，流程跑通： 

#### test ocr request 
python ./ocr_request_ws.py "ws://172.16.10.143:1944/" "./images/1.jpg"