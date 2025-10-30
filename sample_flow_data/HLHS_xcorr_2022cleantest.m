close all
clearvars
clc

%T = readtable('0.1mL-min#2.csv');
%T = readtable('SampleData10-21-2.csv');
%T = readtable('SampleData10-21-4.csv');
T = readtable('15msdelay10-23-1.csv');
T = readtable('15msdelay-10-23-2-dot88uF.csv');
T = readtable('10-30-3-buf500.csv');


%res = 0.01; %resolution in seconds (1/Hz)
%res = .005; %sample every 5 ms 
res = .015;

dz1 = T(:,1);
dz2 = T(:,2);

line1 = table2array(dz1(505:1004,:));
line1 = real(line1);
line2 = table2array(dz2(505:1004,:));
line2 = real(line2);


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

NumLags = 10;
NumLags = 100;
%[c,lags] = crosscorr(line2, line1,NumLags);
[c,lags] = crosscorr(line1, line2, NumLags);
% [c,lags] = xcorr(line1, line2,4000);
lags = lags*res;
% lags2 = lags2*res;

figure(2)
plot(lags,c)


% figure(3)
% plot(lags2,c2)

sws = max(c); %cross correlation coefficient
highr = sws


[maximum, index] = max(c);

max_x = lags(index); %time delay
highshift = max_x

% figure(3)
% plot(n(tmin/res:tmax/res),dz1(tmin/res:tmax/res))
% hold on
% plot(n(tmin/res:tmax/res),dz2(tmin/res:tmax/res))
% 
% TT = table(n',dz1,dz2);
% %TT = table(lags',c);
% writetable(TT, 'test.txt')