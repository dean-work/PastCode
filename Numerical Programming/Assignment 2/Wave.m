figure(1)
hold on;
xlabel("x meters")
ylabel("f(x)")

syms f(x,t)
f(x,t) = (heaviside(x-0.125-t) - heaviside(x-0.375-t))*0.5*...
    (1-cos(8*pi*(x-0.125-t)));
x = 0:0.001:1;

%Insert your timestep value here, and out iter.
t_step = 0.00125;
out_iter = 300;

plot(x,f(x,0),'k-','LineWidth', 0.5)
plot(x,f(x,t_step*out_iter),'k-','LineWidth', 0.5)