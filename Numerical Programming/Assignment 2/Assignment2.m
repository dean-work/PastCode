%Define Function - in degrees
syms f(B,M,theta,gamma)
f(B,M,theta,gamma) = 2*cot(B).*((M^2)*(sin(B).^2)-1)/...
    ((M^2)*(gamma+cos(2*B))+2) - tan(theta);
B = 0:0.01:pi/2;

%2.1)
gamma = 1.4;

%2.1) 
M = 1.5;%CHANGE THIS BACK
figure(7);
theta = 0;
fb1 = f(B,M,theta,gamma);
plot(B,fb1)
z0 = zeros(1,length(fb1));
hold on
axis([theta pi/2 -5 2])
plot(B,z0)
xlabel("b")
ylabel("f(b)")

%a) 2.2 M = 1.5 and theta = 5, 10 and 15 degrees.
%theta = 5 degrees
figure(1);
theta = pi/36;
fb1 = f(B,M,theta,gamma);
plot(B,fb1)
hold on
axis([theta pi/2 -5 2])
plot(B,z0)
title("M = 1.5 | theta = 5")
xlabel("b")
ylabel("f(b)")

%theta = 10 degrees
figure(2);
theta = pi/18;
fb2 = f(B,M,theta,gamma);
plot(B,fb2)
hold on
axis([theta pi/2 -5 2])
plot(B,z0)
title("M = 1.5 | theta = 10")
xlabel("b")
ylabel("f(b)")

%theta = 15 degrees
figure(3);
theta = pi/12;
fb3 = f(B,M,theta,gamma);
plot(B,fb3)
hold on
axis([theta pi/2 -5 2])
plot(B,z0)
title("M = 1.5 | theta = 15")
xlabel("b")
ylabel("f(b)")

%b) M = 5 and theta = 20, 30 and 45 degrees.
M = 5;

%theta = 20 degrees
figure(4);
theta = pi/60;
fb1 = f(B,M,theta,gamma);
plot(B,fb1)
hold on
axis([theta pi/2 -5 2])
plot(B,z0)
title("M = 5 | theta = 20")
xlabel("b")
ylabel("f(b)")

%theta = 30 degrees
figure(5);
theta = pi/6;
fb2 = f(B,M,theta,gamma);
plot(B,fb2)
hold on
axis([theta pi/2 -5 2])
plot(B,z0)
title("M = 5 | theta = 30")
xlabel("b")
ylabel("f(b)")

%theta = 45 degrees
figure(6);
theta = pi/4;
fb3 = f(B,M,theta,gamma);
plot(B,fb3)
hold on
axis([theta pi/2 -5 2])
plot(B,z0)
title("M = 5 | theta = 45")
xlabel("b")
ylabel("f(b)")
