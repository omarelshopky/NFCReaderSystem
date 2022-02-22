import requests
import time


NODEMCU_IP = '192.168.1.145'
GET_ID_ROUTE = '/getCurrentID'


ids = []

def getCurrentID():
    res = requests.get(f'http://{NODEMCU_IP}{GET_ID_ROUTE}')
    return res.text


def addToQueue(id):
    if not id in ids and id != '':
        ids.append(id)


while True:
    addToQueue(getCurrentID())
    print(ids)
    time.sleep(0.2)