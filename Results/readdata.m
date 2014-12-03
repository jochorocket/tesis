format shortG 

MAXIT = 19;

A = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(0)))
for i=1:MAXIT
     A2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     A = A + A2;
end
A = A/(MAXIT+1);


B = importdata(strcat('/home/usuario/tesis/Code/qiear_co',num2str(0)))
for i=1:MAXIT
     B2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     B = B + B2;
end
B = B/(MAXIT+1);

C = importdata(strcat('/home/usuario/tesis/Code/qiear_p',num2str(0)))
for i=1:MAXIT
     C2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     C = C + C2;
end
C = C/(MAXIT+1);

D = importdata(strcat('/home/usuario/tesis/Code/qiear_pco',num2str(0)))
for i=1:MAXIT
     D2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     D = D + D2;
end
D = D/(MAXIT+1);

E = importdata(strcat('/home/usuario/tesis/Code/uqiear',num2str(0)))
for i=1:MAXIT
     E2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     E = E + E2;
end
E = E/(MAXIT+1);

F = importdata(strcat('/home/usuario/tesis/Code/uqiear_co',num2str(0)))
for i=1:MAXIT
     F2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     F = F + F2;
end
F = F/(MAXIT+1);

G = importdata(strcat('/home/usuario/tesis/Code/uqiear_p',num2str(0)))
for i=1:MAXIT
     G2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     G = G + G2;
end
G = G/(MAXIT+1);

H = importdata(strcat('/home/usuario/tesis/Code/uqiear_pco',num2str(0)))
for i=1:MAXIT
     H2 = importdata(strcat('/home/usuario/tesis/Code/qiear',num2str(1)));
     H = H + H2;
end
H = H/(MAXIT+1);

YAXIS = linspace(0,7,8);
YAXIS = transpose(YAXIS);
A0 = transpose(mean(A));
B0 = transpose(mean(B));
C0 = transpose(mean(C));
D0 = transpose(mean(D));
E0 = transpose(mean(E));
F0 = transpose(mean(F));
G0 = transpose(mean(G));
H0 = transpose(mean(H));

AVG = [A0,B0,C0,D0,E0,F0,G0,H0];
plot(YAXIS,AVG);
legend('A','B','C','D','E','F','G','H');

AT = A(:,1);
BT = B(:,1);
CT = C(:,1);
DT = D(:,1);
ET = E(:,1);
FT = F(:,1);
GT = G(:,1);
HT = H(:,1);

XAXIS = linspace(0,100);
XAXIS = transpose(XAXIS);

BEST = [AT,BT,CT,DT,ET,FT,GT,HT];
plot(XAXIS,BEST);
legend('A','B','C','D','E','F','G','H');