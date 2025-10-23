close all
clearvars
clc

T = readtable('0.1mL-min#2.csv');

res = 0.01; %resolution in seconds (1/Hz)

dz1 = T(:,3);
dz2 = T(:,4);

line1 = table2array(dz1(100:50000,:));
line2 = table2array(dz2(100:50000,:));
% line2 = 0-line2;
l1 = size(line1);
n = res:res:l1*res;

% line1 = movmean(line1,50);
% line2 = movmean(line2,50);
figure(1)
plot(n,line1)
hold on
plot(n,line2)

tmin = 10;
tmax = 40;

[c,lags] = crosscorr(line2, line1,NumLags=10000);
% [c,lags] = xcorr(line1, line2,4000);
lags = lags*res;
% lags2 = lags2*res;

figure(2)
plot(lags,c)


% figure(3)
% plot(lags2,c2)

sws = max(c) %correlation coefficient


[maximum, index] = max(c);

max_x = lags(index) %time delay

% figure(3)
% plot(n(tmin/res:tmax/res),dz1(tmin/res:tmax/res))
% hold on
% plot(n(tmin/res:tmax/res),dz2(tmin/res:tmax/res))
% 
% TT = table(n',dz1,dz2);
% %TT = table(lags',c);
% writetable(TT, 'test.txt')