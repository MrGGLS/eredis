import socket
import json
from prompt_toolkit import PromptSession
from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
from prompt_toolkit.completion import WordCompleter


def helpkeyFunction():
    print('         \033[1;31m\"key *\"\033[0m'+' for all keys in databases')
    print('         \033[1;31m\"exists [key]\"\033[0m' + ' for check if key exists')
    print('         \033[1;31m\"del [key]\"\033[0m' + ' for delete the key')
    print('         \033[1;31m\"type [key]\"\033[0m' + ' for type of the key')
    print('         \033[1;31m\"dbsize\"\033[0m' + ' for check the number of keys in databases')
    print('         \033[1;31m\"flushdb\"\033[0m' + ' for clear current databases')
    print('         \033[1;31m\"flushall\"\033[0m' + ' for clear all databases')
    print('         \033[1;31m\"select [n]\"\033[0m' + ' for enter the n-th databases (16 maximum)')
    print('         \033[1;31m\"expire [key] [time]\"\033[0m' + ' for set the expire time (seconds) for key')
    print('         \033[1;31m\"setex [key] [time] [v]\"\033[0m' + ' for set the expire time (seconds) for key while set the key')
    print('         \033[1;31m\"ttl [key]\"\033[0m' + ' for check expire time for key')


def helpStringFunction():
    print('         \033[1;32m\"set [key] [value]\"\033[0m' + ' for append key-value')
    print('         \033[1;32m\"get [key]\"\033[0m' + ' for get the value of key')
    print('         \033[1;32m\"strlen [key]\"\033[0m' + ' for get the length of value')
    print('         \033[1;32m\"append [key] [value]\"\033[0m ' + ' for append value based on original one, return length in total')
    print('         \033[1;32m\"getrange [key] [start] [end]\"\033[0m' + ' for get value[start:end]')
    print('         \033[1;32m\"incr [key]\"\033[0m' + ' for increase the value by 1')
    print('         \033[1;32m\"decr [key]\"\033[0m' + ' for decrease the value by 1')


def helplistFunction():
    print('         \033[1;33m\"lpush/rpush [key] [value1] [value2] ... \"\033[0m' + ' for insert v in left or right of k')
    print('         \033[1;33m\"lrange [key] [start] [end]\"\033[0m' + ' to get data in range [start:end] (You can use 0 for the first, -1 for the last)')
    print('         \033[1;33m\"lpop/rpop [key]\"\033[0m' + ' for get value left or right from key (Mind that key will be deleted after retrieve)')
    print('         \033[1;33m\"lindex [key] [value]\"\033[0m' + ' for value by index')
    print('         \033[1;33m\"llen [key]\"\033[0m' + ' for length of list')
    print('         \033[1;33m\"lset [key] [index] [value]\"\033[0m' + ' for replace value of index')


def helpsaveFunction():
    print('         \033[1;34m\"save\"\033[0m' + ' for save the data')


def helpFunction():
    print('To get help about ERedis command type:')
    print('     \033[1;31m\"help key\"\033[0m' + ' to get help for key command')
    print('     \033[1;32m\"help String\"\033[0m' + ' to get help for String command')
    print('     \033[1;33m\"help list\"\033[0m' + ' to get help for list command')
    print('     \033[1;34m\"help save\"\033[0m' + ' for get help for save command')
    print('     \033[1;35m\"quit\"/\"q\"/\"exit\"\033[0m' + ' to exit')

    global session

    while True:
        helpString = session.prompt('EasyRedis(help)> ')

        if helpString == 'quit' or helpString == 'q' or helpString == 'exit':
            break

        elif helpString == 'help key':
            helpkeyFunction()

        elif helpString == 'help string':
            helpStringFunction()

        elif helpString == 'help list':
            helplistFunction()

        elif helpString == 'help save':
            helpsaveFunction()

        elif helpString == 'help':
            print('To get help about ERedis command type:')
            print('     \033[1;31m\"help key\"\033[0m' + ' to get help for key command')
            print('     \033[1;32m\"help string\"\033[0m' + ' to get help for String command')
            print('     \033[1;33m\"help list\"\033[0m' + ' to get help for list command')
            print('     \033[1;34m\"help save\"\033[0m' + ' for get help for save command')
            print('     \033[1;35m\"quit\"/\"q\"/\"exit\"\033[0m' + ' to exit')

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
         'rpop', 'lindex', 'llen', 'lset', 'quit', 'exit', 'q', 'help'])

    # get welcome message
    try:
        recv_data = sender.recv(1024).decode('utf-8')
        print(recv_data)
    except:
        print('Connection broken, auto exit...')
        exitFunction()
        exit(0)
    '''
    具体逻辑
    '''
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


        sender.send(string.encode('utf-8'))

        try:
            jsonData = sender.recv(1024).decode('utf-8')
            recv_data = json.loads(jsonData)
            processMessage(recv_data)
        except:
            print('Connection broken, auto exit...')
            exitFunction()
            break
