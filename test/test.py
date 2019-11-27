# A "id"eia desse código é, baseado nos prints, val"id"ar as regras de negócio do sistema
# Baseado nos logs, veremos se há alguma violação de regras, e se tiver poderemos
# determinar o porquê dela ter acontec"id"o

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
    print("")

def main():
    escCounter = 0
    leitCounter = 0
    escSequence = 0
    leitSequence = 0

    i = 0
    pessoasNaVariavel = []

    logFileName = input("Nome do arquivo de log: ")
    logFile = open(logFileName,'r')

    for line in logFile:
        print ("Line " + str(i + 1))
        workingLine = line.rsplit()
        if (workingLine[0] in "Esc" and workingLine[2] in "entrou"):
            pessoasNaVariavel.append({"type":workingLine[0], "id":workingLine[1]})
            escCounter += 1
            escSequence += 1
        elif (workingLine[0] in "Esc" and workingLine[2] in "saiu"):
            pessoasNaVariavel.remove({"type":workingLine[0], "id":workingLine[1]})
            escCounter -= 1
        if (workingLine[0] in "Leit" and workingLine[2] in "entrou"):
            pessoasNaVariavel.append({"type":workingLine[0], "id":workingLine[1]})
            leitCounter += 1 
            leitSequence += 1
        elif (workingLine[0] in "Leit" and workingLine[2] in "saiu"):
            pessoasNaVariavel.remove({"type":workingLine[0], "id":workingLine[1]})
            leitCounter -= 1

        for pessoa in pessoasNaVariavel:
            print(pessoa["type"],pessoa["id"])

        CheckConditionsForErrors(escCounter, leitCounter, escSequence, leitSequence)
        i += 1

main()
