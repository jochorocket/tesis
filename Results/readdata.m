format shortG 

MAXIT = 49;

S = '/home/usuario/tesis/Results/arckley_clas10qua10/';

qiear = importdata(strcat(strcat(S,'qiear'),num2str(0)))
for i=1:MAXIT
     qiear2 = importdata(strcat(strcat(S,'qiear'),num2str(i)));
     qiear = qiear + qiear2;
end
qiear = qiear/(MAXIT+1);

qiear_p = importdata(strcat(strcat(S,'qiear_p'),num2str(0)))
for i=1:MAXIT
     qiear_p2 = importdata(strcat(strcat(S,'qiear_p'),num2str(i)));
     qiear_p = qiear_p + qiear_p2;
end
qiear_p = qiear_p/(MAXIT+1);

uqiear = importdata(strcat(strcat(S,'uqiear'),num2str(0)))
for i=1:MAXIT
     uqiear2 = importdata(strcat(strcat(S,'uqiear'),num2str(i)));
     uqiear = uqiear + uqiear2;
end
uqiear = uqiear/(MAXIT+1);

uqiear_p = importdata(strcat(strcat(S,'uqiear_p'),num2str(0)))
for i=1:MAXIT
     uqiear_p2 = importdata(strcat(strcat(S,'uqiear_p'),num2str(i)));
     uqiear_p = uqiear_p + uqiear_p2;
end
uqiear_p = uqiear_p/(MAXIT+1);

qiear_co = importdata(strcat(strcat(S,'qiear_co'),num2str(0)))
for i=1:MAXIT
     qiear_co2 = importdata(strcat(strcat(S,'qiear_co'),num2str(i)));
     qiear_co = qiear_co + qiear_co2;
end
qiear_co = qiear_co/(MAXIT+1);

qiear_pco = importdata(strcat(strcat(S,'qiear_pco'),num2str(0)))
for i=1:MAXIT
     qiear_pco2 = importdata(strcat(strcat(S,'qiear_pco'),num2str(i)));
     qiear_pco = qiear_pco + qiear_pco2;
end
qiear_pco = qiear_pco/(MAXIT+1);

uqiear_co = importdata(strcat(strcat(S,'uqiear_co'),num2str(0)))
for i=1:MAXIT
     uqiear_co2 = importdata(strcat(strcat(S,'uqiear_co'),num2str(i)));
     uqiear_co = uqiear_co + uqiear_co2;
end
uqiear_co = uqiear_co/(MAXIT+1);

uqiear_pco = importdata(strcat(strcat(S,'uqiear_pco'),num2str(0)))
for i=1:MAXIT
     uqiear_pco2 = importdata(strcat(strcat(S,'uqiear_pco'),num2str(i)));
     uqiear_pco = uqiear_pco + uqiear_pco2;
end
uqiear_pco = uqiear_pco/(MAXIT+1);

pavg = [qiear_p0,uqiear_p0];
plot(pavg);
legend('Partitioned QIEAR','Partitioned UQIEAR');

pbest = [qiear_p(:,1),uqiear_p(:,1)];
plot(pbest);
legend('Partitioned QIEAR','Partitioned UQIEAR');



qiear0 = transpose(mean(qiear));
qiear_p0 = transpose(mean(qiear_p));
uqiear0 = transpose(mean(uqiear));
uqiear_p0 = transpose(mean(uqiear_p));
qiear_co0 = transpose(mean(qiear_co));
qiear_pco0 = transpose(mean(qiear_pco));
uqiear_co0 = transpose(mean(uqiear_co));
uqiear_pco0 = transpose(mean(uqiear_pco));

pavg = [qiear_p0,uqiear_p0];
plot(pavg);
legend('Partitioned QIEAR','Partitioned UQIEAR');

pbest = [qiear_p(:,1),uqiear_p(:,1)];
plot(pbest);
legend('Partitioned QIEAR','Partitioned UQIEAR');


pavg = [qiear0,uqiear0,qiear_co0,uqiear_co0];
npavg = [qiear_p0,uqiear_p0,qiear_pco0,uqiear_pco0];
avg = [qiear0,qiear_p0,uqiear0,uqiear_p0,qiear_co0,qiear_pco0,uqiear_co0,uqiear_pco0];
subplot(2,2,1);
plot(pavg);
legend('QIEAR','UQIEAR','Crossover QIEAR','Crossover UQIEAR');
subplot(2,2,2);
plot(npavg);
legend('Partitioned QIEAR','Partitioned UQIEAR','Crossover Partitioned QIEAR','Crossover Partitioned UQIEAR');
% subplot(1,2,1);
% plot(avg);
%legend('QIEAR','Partitioned QIEAR','UQIEAR','Partitioned UQIEAR','Crossover QIEAR','Crossover Partitioned QIEAR','Crossover UQIEAR','Crossover Partitioned UQIEAR')
xlabel('x (nth best individual)');
ylabel('y (average value)');

xaxis = linspace(0,99);
xaxis = transpose(xaxis);
qiearT = qiear(:,1);
qiear_pT = qiear_p(:,1);
uqiearT = uqiear(:,1);
uqiear_pT = uqiear_p(:,1);
quiear_coT = qiear_co(:,1);
qiear_pcoT = qiear_pco(:,1);
uqiear_coT = uqiear_co(:,1);
uqiear_pcoT = uqiear_pco(:,1);
pbest = [qiearT,uqiearT,quiear_coT,uqiear_coT];
npbest = [qiear_pT,uqiear_pT,qiear_pcoT,uqiear_pcoT];
best = [qiearT,qiear_pT,uqiearT,uqiear_pT,quiear_coT,qiear_pcoT,uqiear_coT,uqiear_pcoT];
subplot(2,2,3)
plot(xaxis,pbest);
legend('QIEAR','UQIEAR','Crossover QIEAR','Crossover UQIEAR');
subplot(2,2,4)
plot(xaxis,npbest);
legend('Partitioned QIEAR','Partitioned UQIEAR','Crossover Partitioned QIEAR','Crossover Partitioned UQIEAR');
%subplot(1,2,2)
%plot(best);
%legend('QIEAR','Partitioned QIEAR','UQIEAR','Partitioned UQIEAR','Crossover QIEAR','Crossover Partitioned QIEAR','Crossover UQIEAR','Crossover Partitioned UQIEAR')
xlabel('x (number of iterations)');
ylabel('y (value evolution average of best individuals)');
