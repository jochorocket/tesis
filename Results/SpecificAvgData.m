format shortG 

MAXCI = 10;
MAXQI = 10;
MAXIT = 49;
DIM = 100;

for ci=1:8
file = strcat(strcat('/home/usuario/tesis/Results/holdertable_clas',num2str(ci)),'qua');
output = strcat('/home/usuario/tesis/ThesisLatex/Data/holdertable_nthbest_cind',num2str(ci));
output2 = strcat('/home/usuario/tesis/ThesisLatex/Data/holdertable_bestevol_cind',num2str(ci));

acum_qiear = zeros(DIM,ci*2);
acum_qiear_p = zeros(DIM,ci*2);
acum_uqiear = zeros(DIM,ci*2);
acum_uqiear_p = zeros(DIM,ci*2);
acum_qiear_co = zeros(DIM,ci*2);
acum_qiear_pco = zeros(DIM,ci*2);
acum_uqiear_co = zeros(DIM,ci*2);
acum_uqiear_pco = zeros(DIM,ci*2);

for qi=2:MAXQI	  
	file1 = strcat(file,num2str(qi));
	filedir = strcat(file1,'/');
	qiear = importdata(strcat(strcat(filedir,'qiear'),num2str(0)));
	qiear_p = importdata(strcat(strcat(filedir,'qiear_p'),num2str(0)));
	uqiear = importdata(strcat(strcat(filedir,'uqiear'),num2str(0)));
	uqiear_p = importdata(strcat(strcat(filedir,'uqiear_p'),num2str(0)));
	qiear_co = importdata(strcat(strcat(filedir,'qiear_co'),num2str(0)));
	qiear_pco = importdata(strcat(strcat(filedir,'qiear_pco'),num2str(0)));
	uqiear_co = importdata(strcat(strcat(filedir,'uqiear_co'),num2str(0)));
	uqiear_pco = importdata(strcat(strcat(filedir,'uqiear_pco'),num2str(0)));		
	
	for i=1:MAXIT
		qiear2 = importdata(strcat(strcat(filedir,'qiear'),num2str(0)));
		qiear_p2 = importdata(strcat(strcat(filedir,'qiear_p'),num2str(0)));
		uqiear2 = importdata(strcat(strcat(filedir,'uqiear'),num2str(0)));
		uqiear_p2 = importdata(strcat(strcat(filedir,'uqiear_p'),num2str(0)));
		qiear_co2 = importdata(strcat(strcat(filedir,'qiear_co'),num2str(0)));
		qiear_pco2 = importdata(strcat(strcat(filedir,'qiear_pco'),num2str(0)));
		uqiear_co2 = importdata(strcat(strcat(filedir,'uqiear_co'),num2str(0)));
		uqiear_pco2 = importdata(strcat(strcat(filedir,'uqiear_pco'),num2str(0)));
		qiear = qiear + qiear2;
		qiear_p = qiear_p + qiear_p2;
		uqiear = uqiear + uqiear2;
		uqiear_p = uqiear_p + uqiear_p2;
		qiear_co = qiear_co + qiear_co2;
		qiear_pco = qiear_pco + qiear_pco2;
		uqiear_co = uqiear_co + uqiear_co2;
		uqiear_pco = uqiear_pco + uqiear_pco2;
	end
	
	acum_qiear = acum_qiear + qiear/(MAXIT+1);
	acum_qiear_p = acum_qiear_p + qiear_p/(MAXIT+1);
	acum_uqiear = acum_uqiear + uqiear/(MAXIT+1);
	acum_uqiear_p = acum_uqiear_p + uqiear_p/(MAXIT+1);
	acum_qiear_co = acum_qiear_co + qiear_co/(MAXIT+1);
	acum_qiear_pco = acum_qiear_pco + qiear_pco/(MAXIT+1);
	acum_uqiear_co = acum_uqiear_co + uqiear_co/(MAXIT+1);
	acum_uqiear_pco = acum_uqiear_pco + uqiear_pco/(MAXIT+1);
end

acum_qiear = acum_qiear / (MAXQI - 2);
acum_qiear_p = acum_qiear_p / (MAXQI - 2);
acum_uqiear = acum_uqiear / (MAXQI - 2);
acum_uqiear_p = acum_uqiear_p / (MAXQI - 2);
acum_qiear_co = acum_qiear_co / (MAXQI - 2);
acum_qiear_pco = acum_qiear_pco / (MAXQI - 2);
acum_uqiear_co = acum_uqiear_co / (MAXQI - 2);
acum_uqiear_pco = acum_uqiear_pco / (MAXQI - 2);

qiear0 = transpose(mean(acum_qiear));
qiear_p0 = transpose(mean(acum_qiear_p));
uqiear0 = transpose(mean(acum_uqiear));
uqiear_p0 = transpose(mean(acum_uqiear_p));
qiear_co0 = transpose(mean(acum_qiear_co));
qiear_pco0 = transpose(mean(acum_qiear_pco));
uqiear_co0 = transpose(mean(acum_uqiear_co));
uqiear_pco0 = transpose(mean(acum_uqiear_pco));

% Defaults for this blog post
% width = 3;     % Width in inches% width = 3;     % Width in inches
% height = 3;    % Height in inches
% alw = 0.75;    % AxesLineWidth
% fsz = 11;      % Fontsize
% lw = 1.5;      % LineWidth
% msz = 8;       % MarkerSize    
% pos = get(gcf, 'Position');
% set(gca, 'FontSize', fsz, 'LineWidth', alw); %<- Set properties

% height = 3;    % Height in inches
% alw = 0.75;    % AxesLineWidth
% fsz = 11;      % Fontsize
% lw = 1.5;      % LineWidth
% msz = 8;       % MarkerSize    
% pos = get(gcf, 'Position');
% set(gca, 'FontSize', fsz, 'LineWidth', alw); %<- Set properties
pavg = [qiear0,uqiear0,qiear_co0,uqiear_co0];
npavg = [qiear_p0,uqiear_p0,qiear_pco0,uqiear_pco0];
avg = [qiear0,uqiear0,qiear_co0,uqiear_co0,qiear_p0,uqiear_p0,qiear_pco0,uqiear_pco0];

% figure(1);
% %subplot(2,2,1);
% plot(pavg);
% legend('QIEAR','UQIEAR','Crossover QIEAR','Crossover UQIEAR');
% xlabel('x (nth best individual)');
% ylabel('y (average value)');    
% print(strcat('maytas_nthbest_cind',num2str(ci)), '-dpng', '-r100'); %<-Save as PNG with 300 DPI, 
% 10
% figure(2);
% %subplot(2,2,2);
% plot(npavg);
% legend('Partitioned QIEAR','Partitioned UQIEAR','Crossover Partitioned QIEAR','Crossover Partitioned UQIEAR');
% xlabel('x (nth best individual)');
% ylabel('y (average value)');    
% print(strcat('maytas_nthbest_partitioned_cind',num2str(ci)), '-dpng', '-r100'); %<-Save as PNG with 300 DPI, 

qiearT = acum_qiear(:,1);
qiear_pT = acum_qiear_p(:,1);
uqiearT = acum_uqiear(:,1);
uqiear_pT = acum_uqiear_p(:,1);
quiear_coT = acum_qiear_co(:,1);
qiear_pcoT = acum_qiear_pco(:,1);
uqiear_coT = acum_uqiear_co(:,1);
uqiear_pcoT = acum_uqiear_pco(:,1);
pbest = [qiearT,uqiearT,quiear_coT,uqiear_coT];
npbest = [qiear_pT,uqiear_pT,qiear_pcoT,uqiear_pcoT];
best = [qiearT,uqiearT,quiear_coT,uqiear_coT,qiear_pT,uqiear_pT,qiear_pcoT,uqiear_pcoT];

dlmwrite(output,avg,'delimiter','\t');
dlmwrite(output2,best,'delimiter','\t');

% figure(3);
% %subplot(2,2,3);
% plot(xaxis,pbest);
% legend('QIEAR','UQIEAR','Crossover QIEAR','Crossover UQIEAR');
% xlabel('x (number of iterations)');
% ylabel('y (value evolution average of best individuals)');    
% print(strcat('maytas_bestevol_cind',num2str(ci)), '-dpng', '-r100'); %<-Save as PNG with 300 DPI, 
% 
% figure(4);
% %subplot(2,2,4);
% plot(xaxis,npbest);
% legend('Partitioned QIEAR','Partitioned UQIEAR','Crossover Partitioned QIEAR','Crossover Partitioned UQIEAR');
% xlabel('x (number of iterations)');
% ylabel('y (value evolution average of best individuals)');
% print(strcat('maytas_bestevol_partitioned_cind',num2str(ci)), '-dpng', '-r100'); %<-Save as PNG with 300 DPI, 
end