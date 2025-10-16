close all
clearvars
clc

T = readtable('2mL-min#12.csv');
%T = readtable('CoolTermSampleData1.csv');

res = 0.01; %resolution in seconds (1/Hz)
%-100 to 0 to 100

dz1 = T(:,3);
dz2 = T(:,4);


line1 = table2array(dz1(100:5000,:));
%look at the size of dz1 in workspace and subtract out the last couple of
%values
%excluding first 100 and last 100
line2 = table2array(dz2(100:5000,:));
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


[c,lags] = crosscorr(line2, line1,NumLags=500);
%og 10k
%NumLags cannot be bigger than array itself of l1
%NumLags * .01 is the limit
% [c,lags] = xcorr(line1, line2,4000);
lags = lags*res;
% lags2 = lags2*res;

figure(2)
plot(lags,c)


% figure(3)
% plot(lags2,c2)

sws = max(c);
% figure(3)
% plot(n(tmin/res:tmax/res),dz1(tmin/res:tmax/res))
% hold on
% plot(n(tmin/res:tmax/res),dz2(tmin/res:tmax/res))
% 
% TT = table(n',dz1,dz2);
% %TT = table(lags',c);
% writetable(TT, 'test.txt')