format shortG 

MAXIT = 49;

S = '/home/usuario/tesis/Results/arckleyset14/';

A = importdata(strcat(strcat(S,'qiear'),num2str(0)))
for i=1:MAXIT
     A2 = importdata(strcat(strcat(S,'qiear'),num2str(1)));
     A = A + A2;
end
A = A/(MAXIT+1);


B = importdata(strcat(strcat(S,'qiear_co'),num2str(0)))
for i=1:MAXIT
     B2 = importdata(strcat(strcat(S,'qiear_co'),num2str(1)));
     B = B + B2;
end
B = B/(MAXIT+1);

C = importdata(strcat(strcat(S,'qiear_p'),num2str(0)))
for i=1:MAXIT
     C2 = importdata(strcat(strcat(S,'qiear_p'),num2str(1)));
     C = C + C2;
end
C = C/(MAXIT+1);

D = importdata(strcat(strcat(S,'qiear_pco'),num2str(0)))
for i=1:MAXIT
     D2 = importdata(strcat(strcat(S,'qiear_pco'),num2str(1)));
     D = D + D2;
end
D = D/(MAXIT+1);

E = importdata(strcat(strcat(S,'uqiear'),num2str(0)))
for i=1:MAXIT
     E2 = importdata(strcat(strcat(S,'uqiear'),num2str(1)));
     E = E + E2;
end
E = E/(MAXIT+1);

F = importdata(strcat(strcat(S,'uqiear_co'),num2str(0)))
for i=1:MAXIT
     F2 = importdata(strcat(strcat(S,'uqiear_co'),num2str(1)));
     F = F + F2;
end
F = F/(MAXIT+1);

G = importdata(strcat(strcat(S,'uqiear_p'),num2str(0)))
for i=1:MAXIT
     G2 = importdata(strcat(strcat(S,'uqiear_p'),num2str(1)));
     G = G + G2;
end
G = G/(MAXIT+1);

H = importdata(strcat(strcat(S,'uqiear_pco'),num2str(0)))
for i=1:MAXIT
     H2 = importdata(strcat(strcat(S,'uqiear_pco'),num2str(1)));
     H = H + H2;
end
H = H/(MAXIT+1);

A0 = transpose(mean(A));
B0 = transpose(mean(B));
C0 = transpose(mean(C));
D0 = transpose(mean(D));
E0 = transpose(mean(E));
F0 = transpose(mean(F));
G0 = transpose(mean(G));
H0 = transpose(mean(H));

PAVG = [A0,C0,E0,G0];
NPAVG = [B0,D0,F0,H0];
plot(PAVG);
legend('QIEAR','UQIEAR','Crossover QIEAR','Crossover UQIEAR');
plot(NPAVG);
legend('Partitioned QIEAR','Partitioned UQIEAR','Crossover Partitioned QIEAR','Crossover Partitioned UQIEAR');


AT = A(:,1);
BT = B(:,1);
CT = C(:,1);
DT = D(:,1);
ET = E(:,1);
FT = F(:,1);
GT = G(:,1);
HT = H(:,1);

XAXIS = linspace(0,99);
XAXIS = transpose(XAXIS);

PBEST = [AT,CT,ET,GT];
NPBEST = [BT,DT,FT,HT];
plot(XAXIS,PBEST);
legend('QIEAR','UQIEAR','Crossover QIEAR','Crossover UQIEAR');
plot(XAXIS,NPBEST);
legend('Partitioned QIEAR','Partitioned UQIEAR','Crossover Partitioned QIEAR','Crossover Partitioned UQIEAR');
