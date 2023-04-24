filename = 'saida.txt';
m = csvread(filename);

sorted = sortrows(m, 2);
disp(sorted);
disp(m);
%plot(sorted(:,1), sorted(:, 2), 'mo', sorted(:,1), sorted(:,3), 'bo');
%plot(m(:,1), m(:, 2), 'm', m(:,1), m(:, 3), 'b');

fig1 = figure();
plot(m(:,1), m(:, 2), 'b.');

fig2 = figure();
plot(m(:,1), m(:, 3), 'r.');

fig3 = figure();
plot(m(:,1), m(:, 2), 'm', m(:,1), m(:, 3), 'b');

