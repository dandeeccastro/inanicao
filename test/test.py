# A ideia desse código é, baseado nos prints, validar as regras de negócio do sistema
# Baseado nos logs, veremos se há alguma violação de regras, e se tiver poderemos
# determinar o porquê dela ter acontecido

# Referência de Log

# Esc 1 entrou
# Var 2
# Esc 1 saiu

def CheckConditionsForErrors(writerCounter, readerCounter, writerSequence, readerSequence):
    if (abs(writerSequence - readerSequence) > 1):
        print ("Inanição detectada!")
    if (writerCounter > 1 and readerCounter is 0):
        print ("Concorrência entre escritores detectada!")
    elif (writerCounter >= 1 and readerCounter >= 1):
        print ("Concorrência entre leitores e escritores detectada!")

def main():
    escCounter = 0
    leitCounter = 0
    escSequence = 0
    leitSequence = 0

    logFileName = input("Nome do arquivo de log: ")
    logFile = open(logFileName,'r')

    for line in logFile:
        workingLine = line.rsplit()

        if (workingLine[0] is "Esc" and workingLine[2] is "entrou"):
            escCounter += 1
            escSequence += 1
        elif (workingLine[0] is "Esc" and workingLine[2] is "saiu"):
            escCounter -= 1
        if (workingLine[0] is "Leit" and workingLine[2] is "entrou"):
            leitCounter += 1 
            leitSequence += 1
        elif (workingLine[0] is "Leit" and workingLine[2] is "saiu"):
            leitCounter -= 1


        CheckConditionsForErrors(escCounter, leitCounter, escSequence, leitSequence)

main()
