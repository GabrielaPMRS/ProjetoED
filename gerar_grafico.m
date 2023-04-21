filename = 'saida.txt';
m = csvread(filename);

%sorted = sortrows(m, 1);
%disp(sorted);
%plot(sorted(:,1), sorted(:, 2), 'mo', sorted(:,1), sorted(:,3), 'bo');
plot(m(:,1), m(:, 2), 'm', m(:,1), m(:, 3), 'b');
%plot(m(:,1), m(:, 3), 'b');
%plot(m(:, 1), m(:, 2), 'r');
