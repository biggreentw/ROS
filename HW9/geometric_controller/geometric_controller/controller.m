function out = controller(u,P)

% input(25*1):desired trajectory and full state feedback, x v R Omega time
% output(4*1): force and moment control input

% process inputs
xd    = u(1:3);
b1d   = u(4:6);

% current state
x     = u(7:9);
v     = u(10:12);
R     = reshape(u(13:21),3,3)
Omega = u(22:24);
t     = u(end)
syms aa
ff=[0.4*aa;0.4*sin(0.2*pi*aa);0.6*cos(0.2*pi*aa)];
ff_1=diff(ff,aa);
ff_2=diff(ff_1,aa);
ex=reshape(x-xd,1,3);

%subs(ff_1,t)
V_A=v-double(subs(ff_1,t));
ev=transpose(V_A);
%ev=reshape(v,1,3)
e3=[0;0;1];

f = 0;
%double(subs(ff_2,t))
%size(double(subs(ff_2,t)))
%reshape(double(subs(ff_2,t)),1,3)
%size(reshape(double(subs(ff_2,t)),1,3))
f=-(-P.kx*ex-P.kv*ev-P.mass*P.gravity*[0 0 1]+P.mass*reshape(double(subs(ff_2,t)),1,3))*R*e3;
%f=-(-P.kx*ex-P.kv*ev-P.mass*P.gravity*[0 0 1])*R*e3
%b3c=-(-P.kx*ex-P.kv*ev-P.mass*P.gravity*e3+P.mass*diff(diff(xd)))/norm(-P.kx*ex-P.kv*ev-P.mass*P.gravity*e3+P.mass*diff(diff(xd)));
%b1c=-cross(b3c,(cross(b3c,b1d)))/norm(cross(b3c,b1d));
Rd=[1.0 0 0;
    0 1.0 0;
    0 0 1.0]
eR=(1/2)*vee(Rd.*R-R.*Rd)
%norm(eR);

%plot(t,norm(eR))
%plot(t,sin(t))


Omega_d=[0;0.1;1];
e_Omega=Omega-R.*Rd*Omega_d;
J=[P.Jxx 0 0;
    0 P.Jyy 0;
    0 0 P.Jzz];

M = [0; 0; 0];
M=-P.kR*eR-P.kOmega*e_Omega+cross(Omega,J*Omega);

out = [f;M];
end