import time
from gdx import gdx


gdx = gdx.gdx()
gdx.open(connection='usb')

def getDeviceInfo():
    for myinfo in gdx.device_info():
        print(myinfo)


getDeviceInfo()

def getSensorInfo():
    for myinfo in gdx.sensor_info():
        print(myinfo)


gdx.select_sensors([1,2,3,4,5,6,7])


column_headers = gdx.enabled_sensor_info() 


def getUnits(info_headers):
    unit_list = []
    units = ''
    for headers in info_headers:
        units = str(headers[headers.find('(') : headers.find(')') +1])
        unit_list.append(units)
    return unit_list


print(getUnits(column_headers))

gdx.start(100)

for i in range(1,10):
    print(gdx.read())

gdx.stop()

gdx.close()
