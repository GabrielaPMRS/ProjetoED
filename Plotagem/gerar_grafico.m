filename = 'saida.txt';
m = csvread(filename);

subplot(2,2, [3,4]);
plot(m(:,1), m(:, 2), 'r', m(:,1), m(:, 3), 'b');
title("Lista versus Árvore de busca binária");
xlabel('Tamanho (n)');
ylabel('Iterações');

subplot(2,2,1)
plot(m(:,1), m(:, 3), 'b');
title("Árvore de busca binária");
xlabel('Tamanho (n)');
ylabel('Iterações');

subplot(2,2,2)
plot(m(:, 1), m(:, 2), 'r');
title("Lista");
xlabel('Tamanho (n)');
ylabel('Iterações');
