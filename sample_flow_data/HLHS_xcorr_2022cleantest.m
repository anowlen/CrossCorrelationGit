close all
clearvars
clc

%T = readtable('0.1mL-min#2.csv');
%T = readtable('SampleData10-21-2.csv');
%T = readtable('SampleData10-21-4.csv');
T = readtable('15msdelay10-23-1.csv');
T = readtable('15msdelay-10-23-2-dot88uF.csv');
T = readtable('10-30-3-buf500.csv');
T = readtable('10-30-4-buf3000Ts5.csv');
T = readtable('11-12-3-buf3250Ts5micros');
T = readtable('11-13-1-buf3250Ts5microsvoltage');
T = readtable('11-18-1-buf3440Ts5micros5Hz.csv');
%T = readtable('11-18-2-buf600Ts5micros5Hz.csv');
%T = readtable('11-18-3-buf300Ts5micros5Hz.csv');
T = readtable('11-18-4-buf400Ts5micros10Hz.csv');
T = readtable('11-18-5-buf3440Ts5micros10Hz.csv');
%T = readtable('11-20-3-buf1000Ts5micros.csv');
T = readtable('11-20-3-buf200Ts5microsreal.csv');

%res = 0.01; %resolution in seconds (1/Hz)
res = .005; %sample every 5 ms 
%res = .015;


dz1 = T(:,1);
dz2 = T(:,2);

line1 = table2array(dz1(35:234,:)); 
line1 = real(line1);
line2 = table2array(dz2(35:234,:));
line2 = real(line2);


% line2 = 0-line2;
l1 = size(line1);
n = res:res:l1*res;

% line1 = movmean(line1,50);
% line2 = movmean(line2,50);
figure();
subplot(1,2,1);
plot(n,line1)
xlim([0 .2]);
hold on
plot(n,line2)

tmin = 10;
tmax = 40;

NumLags = 10;
NumLags = 100;
%[c,lags] = crosscorr(line2, line1,NumLags);
[c,lags] = crosscorr(line1, line2, NumLags);
legend('Signal 1','Signal 2');
xlabel('Time(s)');
ylabel('Amplitude (V)');
title('Input Signals');
% [c,lags] = xcorr(line1, line2,4000);
lags = lags*res;
% lags2 = lags2*res;

%figure();
subplot(1,2,2);
plot(lags,c)
xlabel('Time(s)');
ylabel('Cross-Correlation Coefficient');
title('Cross-Correlation Coefficient vs Time');


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