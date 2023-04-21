import random

def gerar_entrada():
    """Gera as entradas necessaria para popular as estruturas."""
    # size = 86
    io = 6
    sizes = []
    for i in range(1000):
        sizes.append(io + 20 * i)
    print(sizes)
    
    # sizes = [91, 96, 101, 151, 201, 251, 301, 351, 401, 451, 501, 551, 601, 651, 701, 751, 801, 851, 901,
    #         951, 1001]
    with open("entrada4.txt", "w") as file:
        for size in sizes:
            numbers = list(range(size))
            random.shuffle(numbers)
            file.write(str(size))
            file.write('\n')
            for n in numbers:
                file.write(str(n))
                file.write(' ')
            file.write('\n')

if __name__ == "__main__":
    gerar_entrada()