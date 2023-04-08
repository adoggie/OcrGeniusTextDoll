
pub-addr: 1945
sub-addr: 1946

pip install fire websocket-client 

#### message router
python mx-broker.py run_spbb tcp://*:1946 tcp://*:1945

#### recv result and write to mongodb
python ./ocr-recorder.py run

#### test ocr request 
python ./ocr_request_ws.py "ws://172.16.10.143:1944/" "./images/1.jpg"