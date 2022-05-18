import socket


def printFunction():
    print(' ______ _____          _ _     ')
    print('|  ____|  __ \        | (_)    ')
    print('| |__  | |__) |___  __| |_ ___ ')
    print('|  __| |  _  // _ \/ _` | / __|')
    print('| |____| | \ \  __/ (_| | \__\\')
    print('|______|_|  \_\___|\__,_|_|___/')
    print('connection success! Initiating your configuration.... ')


def exitFunction():
    print('______            ')
    print('| ___ \           ')
    print('| |_/ /_   _  ___ ')
    print('| ___ \ | | |/ _ \\')
    print('| |_/ / |_| |  __/')
    print('\____/ \__, |\___|')
    print('        __/ |     ')
    print('       |___/      ')


if __name__ == '__main__':

    sender = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sender.connect((socket.gethostname(), 8000))

    printFunction()

    promoptString = 'EasyRedis>'

    while True:
        string = input(promoptString)

        if string == 'exit':
            exitFunction()
            break


        '''
        具体逻辑
        '''

        sender.send(string.encode('utf-8'))  

        recv_data = sender.recv(1024)

        print("\033[0;31;40m"+recv_data.decode('utf-8')+'\033[0m')

