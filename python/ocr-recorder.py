import os,os.path,sys,time,datetime,traceback,json
import zmq
import fire
import base64
import json


MX_PUB_ADDR="tcp://127.0.0.1:1945"
MX_SUB_ADDR="tcp://127.0.0.1:1946"

"""
app_id,sequence,type,result(base64),image(base64)
"""
def run(sub_addr=MX_SUB_ADDR):
    ctx = zmq.Context()
    sock = ctx.socket(zmq.SUB)
    sock.setsockopt(zmq.TCP_KEEPALIVE,1)
    sock.setsockopt(zmq.SUBSCRIBE, b'')
    sock.connect(sub_addr)
    os.makedirs('data',exist_ok=True)
    print("Recorder service started.")
    while True:
        m = sock.recv()
        fs = m.decode().split(',')
        if len(fs) < 5:
            print("invalid message:",m)
            continue
        app_id,sequence,type,result,image = fs[:5]
        datadir = f"data/{app_id}"
        os.makedirs(datadir,exist_ok=True)
        try:
            fn = os.path.join(datadir, f"{sequence}.txt")
            result = base64.b64decode(result).decode()
            with open(fn,'w') as f:
                f.write( result )
            image = base64.b64decode(image)
            fn = os.path.join(datadir,f"{sequence}.jpg")
            with open(fn,'wb') as f:
                f.write( image )
            print( str(datetime.datetime.now()).split(".")[0] , app_id ,sequence,result)
        except: pass


if __name__ == '__main__':
    fire.Fire()