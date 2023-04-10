import random

def gerar_entrada():
    """Gera as entradas necessaria para popular as estruturas."""
    size = 1001
    numbers = list(range(size))
    random.shuffle(numbers)

    with open("entrada.txt", "w") as file:
        file.write(str(size))
        file.write('\n')
        for n in numbers:
            file.write(str(n))
            file.write(' ')

if __name__ == "__main__":
    gerar_entrada()