
clear;
close all;
% Specs
wC = 7.5;
Q  = 10;
BW = 400;
Gain = 10;

% Component Values

C = 1000e-9;

R2 = Q * (pi * wC * C);
R1 = R2 / (2 * Gain);
R3 = R2 * (4*Q^2 - 2*Gain);



% Transfer function 
s = tf('s');
H = -(s/R1/C)/(s^2+2*s/R2/C + (1/R1+1/R3)/R2/C^2);

bode(H);