import sys
import time
import argparse
from pySerialTransfer import pySerialTransfer as txfer

if __name__ == '__main__':

    parser = argparse.ArgumentParser("Read temperatures from attached arduino")

    parser.add_argument("-p", '--port', action="store", help="USB Port", default="COM3")
    parser.add_argument("-o", '--output', action="store", help="Readings output file", default="temperatures.csv")

    arguments = parser.parse_args()


class TemperatureReading(object):
    timeStamp = 0
    temperature = 0.0


arr = ''

if __name__ == '__main__':
    try:
        testStruct = TemperatureReading
        link = txfer.SerialTransfer(arguments.port)

        link.open()
        time.sleep(5)

        with open(arguments.output, "w") as temps:
            while True:
                if link.available():
                    recSize = 0

                    testStruct.timeStamp = link.rx_obj(obj_type='i', start_pos=recSize)
                    recSize += txfer.STRUCT_FORMAT_LENGTHS['i']

                    testStruct.temperature = link.rx_obj(obj_type='f', start_pos=recSize)
                    recSize += txfer.STRUCT_FORMAT_LENGTHS['f']

                    print(f'Temp: {testStruct.temperature}')
                    temps.write(str(testStruct.temperature))
                    temps.write("\n")

                elif link.status < 0:
                    if link.status == txfer.CRC_ERROR:
                        print('ERROR: CRC_ERROR')
                    elif link.status == txfer.PAYLOAD_ERROR:
                        print('ERROR: PAYLOAD_ERROR')
                    elif link.status == txfer.STOP_BYTE_ERROR:
                        print('ERROR: STOP_BYTE_ERROR')
                    else:
                        print('ERROR: {}'.format(link.status))


    except KeyboardInterrupt:
        link.close()
