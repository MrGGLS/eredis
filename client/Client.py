import socket
import json
from prompt_toolkit import PromptSession
from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
from prompt_toolkit.completion import WordCompleter


def helpkeyFunction():
    print('         \"key *\" for all keys in databases')
    print('         \"exists [key]\" for check if key exists')
    print('         \"del [key]\" for delete the key')
    print('         \"type [key]\" for type of the key')
    print('         \"dbsize\" for check the number of keys in databases')
    print('         \"flushdb\" for clear current databases')
    print('         \"flushall\" for clear all databases')
    print('         \"select [n]\" for enter the n-th databases (16 maximum)')
    print('         \"expire [key] [time]\" for set the expire time (seconds) for key')
    print('         \"setex [key] [time] [v]\" for set the expire time (seconds) for key while set the key')
    print('         \"ttl [key]\" for check expire time for key')


def helpStringFunction():
    print('         \"set [key] [value]\" for append key-value')
    print('         \"get [key]\" for get the value of key')
    print('         \"strlen [key]\" for get the length of value')
    print('         \"append [key] [value]\" for append value based on original one, return length in total')
    print('         \"getrange [key] [value] [start] [end]\" for get value[start:end]')
    print('         \"incr [key]\" for increase the value by 1')
    print('         \"decr [key]\" for decrease the value by 1')


def helplistFunction():
    print('         \"lpush/rpush [key] [value1] [value2] ... \" for insert v in left or right of k')
    print(
        '         \"lrange [key] [start] [end]\" to get data in range [start:end] (You can use 0 for the first, -1 for the last)')
    print(
        '         \"lpop/rpop [key]\" for get value left or right from key (Mind that key will be deleted after retrieve)')
    print('         \"lindex [key] [value]\" for value by index')
    print('         \"llen [key]\" for length of list')
    print('         \"lset [key] [index] [value]\" for replace value of index')


def helpsaveFunction():
    print('         \"save\" for save the data')


def helpFunction():
    print('To get help about ERedis command type:')
    print('     \"help key\" to get help for key command')
    print('     \"help String\" to get help for String command')
    print('     \"help list\" to get help for list command')
    print('     \"help save\" for get help for save command')
    print('     \"quit\"/\"q\"/\"exit\" to exit')

    global session

    while True:
        helpString = session.prompt('EasyRedis(help)> ')

        if helpString == 'quit' or helpString == 'q' or helpString == 'exit':
            break

        elif helpString == 'help key':
            helpkeyFunction()

        elif helpString == 'help String':
            helpStringFunction()

        elif helpString == 'help list':
            helplistFunction()

        elif helpString == 'help save':
            helpsaveFunction()

        else:
            print('No such command')


def processMessage(data):
    response_type = data['type']
    response_message = data['message']

    global promoptString

    if response_type == 0:
        print('No such command or syntax error, you could type \"help\" for more instructions')
        print('Error Type: ' + response_message)

    elif response_type == 1:
        print(response_message)

    elif response_type == 2:
        response_value = data['value']
        print(response_message)

        if str(response_value) == '0':
            promoptString = 'EasyRedis> '
        else:
            promoptString = 'EasyRedis' + '[' + str(response_value) + ']> '


def printFunction():
    print(' ______ _____          _ _     ')
    print('|  ____|  __ \        | (_)    ')
    print('| |__  | |__) |___  __| |_ ___ ')
    print('|  __| |  _  // _ \/ _` | / __|')
    print('| |____| | \ \  __/ (_| | \__\\')
    print('|______|_|  \_\___|\__,_|_|___/')
    print('Connection success! Initiating your configuration.... ')
    print('Initiating complete! 16 databases available.')
    print('You can type \"help\" to get more instructions.')


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

    sender.connect((socket.gethostname(), 8888))

    printFunction()

    global promoptString
    promoptString = 'EasyRedis> '

    session = PromptSession()

    redisCompleter = WordCompleter(
        ['select', 'key', 'save', 'keys', 'type', 'del', 'dbsize', 'flushdb',
         'exists', 'flushall', 'expire', 'setex', 'ttl', 'set', 'get', 'strlen',
         'append', 'getrange', 'incr', 'decr', 'lpush', 'rpush', 'lrange', 'lpop',
         'rpop', 'lindex', 'llen', 'lset'])

    # get welcome message
    recv_data = sender.recv(1024).decode('utf-8')
    print(recv_data)

    while True:

        string = session.prompt(promoptString, completer=redisCompleter, auto_suggest=AutoSuggestFromHistory())

        if len(string) == 0:
            continue

        if string == 'quit' or string == 'exit' or string == 'q':
            exitFunction()
            break

        if string == 'help':
            helpFunction()
            continue

        '''
        具体逻辑
        '''

        sender.send(string.encode('utf-8'))

        recv_data = json.loads(sender.recv(1024).decode('utf-8'))

        processMessage(recv_data)
