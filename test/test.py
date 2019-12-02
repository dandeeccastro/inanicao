def CheckCountersForErrors(writer_counter, reader_counter, writer_streak, reader_streak):
    if ( abs( writer_streak - reader_streak) ):
        print('Inanição detectada!')
    if ( writer_counter > 1 and reader_counter is 0):
        print ('Concorrência entre escritores detectada')
    elif ( writer_counter >= 1 and reader_counter >= 1):
        print ('Concorrência entre leitores e escritores detectada')
    print('')

def ValidateSharedVariableValue(command_line, previous_value):
    current_value = command_line[3]
    if ( int(previous_value) >= 0):
        if (command_line[2] in 'leu' and previous_value != command_line[3]):
            print('Thread leu um valor modificado! Condição de corrida!')
        elif (command_line[0] in 'Esc' and command_line[2] in 'escreveu' and (command_line[1] != current_value)):
            print('Escritor leu o que não escreveu')
    return current_value

def main():

    people_inside_shared_value = []
    readers_inside = 0
    writers_inside = 0
    reader_sequence = 0
    writer_sequence = 0
    shared_variable_value = -1
    i = 0

    log_file_name = input("Nome do arquivo de log: ")
    log_file = open(log_file_name,'r')

    for line in log_file:
        print('[Line ' + str(i + 1) + ']')
        line_arr = line.rsplit()

        if (line_arr[2] in 'entrou'):
            people_inside_shared_value.append(line_arr[0] + ' ' + line_arr[1])
            if (line_arr[0] in 'Esc'):
                writers_inside += 1
                writer_sequence += 1
            elif (line_arr[0] in 'Leit'):
                readers_inside += 1
                reader_sequence += 1
        elif (line_arr[2] in 'saiu'):
            people_inside_shared_value.remove(line_arr[0] + ' ' + line_arr[1])
            if (line_arr[0] in 'Esc'):
                writers_inside -= 1
            elif (line_arr[0] in 'Leit'):
                readers_inside -= 1

        if (len(line_arr) > 3):
            shared_variable_value = ValidateSharedVariableValue(line_arr,shared_variable_value)

        for person in people_inside_shared_value:
            print(person)

        CheckCountersForErrors(writers_inside,readers_inside,writer_sequence,reader_sequence)
        i += 1
main()
