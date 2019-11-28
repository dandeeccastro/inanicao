# A "id"eia desse código é, baseado nos prints, val"id"ar as regras de negócio do sistema
# Baseado nos logs, veremos se há alguma violação de regras, e se tiver poderemos
# determinar o porquê dela ter acontec"id"o

# Referência de Log

# Esc 1 entrou
# Var 2
# Esc 1 saiu
def ValidateSharedVariableValue(commandLine, previousValue):
    currentValue = commandLine[3]
    if(int(previousValue) >= 0):
        if (commandLine[2] in 'leu' and previousValue != commandLine[3]):
            print("Thread leu um valor modificado! Condição de corrida!")
        elif (commandLine[0] in 'Esc' and commandLine[2] in 'escreveu' and (commandLine[1] != currentValue)):
            print("Escritor leu o que não escreveu!")
    return currentValue

def CheckCountersForErrors(writerCounter, readerCounter, writerSequence, readerSequence):
    if (abs(writerSequence - readerSequence) > 1):
        print ("Inanição detectada!")
    if (writerCounter > 1 and readerCounter is 0):
        print ("Concorrência entre escritores detectada!")
    elif (writerCounter >= 1 and readerCounter >= 1):
        print ("Concorrência entre leitores e escritores detectada!")
    print("")
        
def main():
    # Declarando variáveis
    escCounter = 0
    leitCounter = 0
    escSequence = 0
    leitSequence = 0
    variableValue = -1
    i = 0
    pessoasNaVariavel = []

    # Pegando arquivo de log
    logFileName = input("Nome do arquivo de log: ")
    logFile = open(logFileName,'r')

    # Iterando por cada linha do arquivo  de log
    for line in logFile:

        print ("[Line " + str(i + 1) + "]")
        workingLine = line.rsplit()

        # Pegando pessoas que estão acessando a var global
        if (workingLine[2] in "entrou"):
            pessoasNaVariavel.append({"type":workingLine[0], "id":workingLine[1]})
        elif (workingLine[2] in 'saiu'):
            pessoasNaVariavel.remove({"type":workingLine[0], "id":workingLine[1]})

        # Setando os contadores corretos para cada condição
        if (workingLine[0] in "Esc" and workingLine[2] in "entrou"):
            escCounter += 1
            escSequence += 1
        elif (workingLine[0] in "Esc" and workingLine[2] in "saiu"):
            escCounter -= 1
        if (workingLine[0] in "Leit" and workingLine[2] in "entrou"):
            leitCounter += 1 
            leitSequence += 1
        elif (workingLine[0] in "Leit" and workingLine[2] in "saiu"):
            leitCounter -= 1

        if (len(workingLine) > 3):
            variableValue = ValidateSharedVariableValue(workingLine,variableValue)

        # Quem está na var. global?
        for pessoa in pessoasNaVariavel:
            print(pessoa["type"],pessoa["id"])

        # Conferindo condições e finalizando loop
        CheckCountersForErrors(escCounter, leitCounter, escSequence, leitSequence)
        i += 1

main()
