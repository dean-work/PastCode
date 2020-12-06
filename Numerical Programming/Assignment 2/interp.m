t = readtable("out_interp.csv");
f = readtable("in_interp.csv");
t = sortrows(t, 2);

plot(t.x,t.f)
hold on 
plot(f.x,f.f_x_)

title("Interpolated Plot - Cubic Spline")
xlabel("x")
ylabel("f(x)")


