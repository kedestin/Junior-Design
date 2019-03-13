clear;
close all;

% G = R3 / 2 *R1



% R1=160e3;
% R1=80e3;
% R2=240;
% R3=160e3;
C =1e-9;


targetGain = 0;


% low = 0;
% high = 1e5 - 1;
% mid = 0;
% 
% %Binary search for the Threshold N
% while (low <= high)
%         mid = idivide(int32(low + high), 2);
%         [mag, phase] = bodePlot(mid);
%         db(max(mag))
% %         close all;
%         if (db(max(mag)) == targetGain)
%                 break;
% 
%         elseif (db(max(mag)) < targetGain)
%                 low = mid + 1;
%         else
%                 high = mid - 1;
%         end
% end
% H0 = 37.013503;
H0 =10;
bodePlot(H0);
[mag,~] = bodePlot(H0);
db(max(mag))

[1 1; 1 1]

function [mag, phase] = bodePlot(H0)
    s = tf('s');
    C =.5e-9;
    f0 = 18.5e3;
    B=.2*f0;
    Q = f0 / B;
%     R3 = 1 / (pi * B * C);
%     R3 = Q / (2*pi * f0 * C);
%     R1 = R3 / (2*H0);
%     R2 = R3 / (4 * Q^2 - 2*H0);
     Zc = 1 / (s*C);
%      H = R2*R3*Zc / (((R1+R2)*Zc^2)+(2*R1*R2*Zc)-(R1*R2*R3));
%     [mag,phase]= bode(H);

%     R1 = 2e4;
%     R3 = 1 / (C^2/(2*C) * B);
%     R2 = 1 /(C^2*f0^2*R3);

    R1 = Q / (2 * pi * f0 * C * H0);
    R2 = Q / (pi * f0 * C);
    R3 = Q / (2 * pi * f0 * C * (2 * Q^2 - H0));
%     H = (-s * R1 * C) / (s^2*C*C+s*R3*(C+C)+R2*R3);
     H = R2*R3*Zc / (((R1+R2)*Zc^2)+(2*R1*R2*Zc)-(R1*R2*R3));

    bode(H);
%     close 
end

% Center frequency  1/R1 + 1/R2 = R3 / (1/(f0*C*2pi)^2)
% Bandwidth 1e3 = 1/R3*