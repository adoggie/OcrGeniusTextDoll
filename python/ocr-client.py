
import os
import string
import sys
import time
import datetime
from random import randint
import base64
import zmq
import fire
import websocket




MX_SUB_ADDR = "tcp://*:1946"
MX_PUB_ADDR = "tcp://*:1945"

def init_keepalive(sock):
    sock.setsockopt(zmq.TCP_KEEPALIVE,1)
    sock.setsockopt(zmq.TCP_KEEPALIVE_IDLE,120)
    sock.setsockopt(zmq.TCP_KEEPALIVE_INTVL,1)
    sock.set_hwm(0)

ctx = zmq.Context()



def ocr_request(req_addr="tcp://172.16.16.143:1944",rep_addr="tcp://172.16.10.169:1946",image="./images/2.jpg"):
    sockReq = ctx.socket(zmq.PUSH)
    sockRep = ctx.socket(zmq.SUB)

    sockReq.connect(req_addr)
    sockRep.connect(rep_addr)
    sockRep.setsockopt(zmq.SUBSCRIBE, b'')
    data = base64.b64encode(open(image, 'rb').read()).decode()

    app_id = 'test'
    sequence = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    type = 'ocr'
    body = data
    message = ",".join([app_id, sequence, type,])
    sockReq.send_string(message)

    print("send:",message)
    rep = sockRep.recv_string()
    print(rep)

def ocr_request_ws(server="ws://172.16.10.143:1944/",image="./images/2.jpg"):
    # 建立 WebSocket 连接
    ws = websocket.WebSocket()
    ws.connect(server)
    # 发送消息
    data = base64.b64encode(open(image, 'rb').read()).decode()

    app_id = 'test'
    sequence = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    type = 'ocr'
    body = data
    message = ",".join([app_id, sequence, type,body ])
    ws.send(message)
    res = ws.recv()
    result = res.split(',')[-1]
    raw = base64.b64decode(result)
    print(raw.decode('gbk'))

    # 关闭连接
    ws.close()


if __name__ == '__main__':
    fire.Fire()