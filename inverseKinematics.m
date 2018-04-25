syms theta1 theta2 real;
baseLen = 79; % mm
lowArmLen = 122;
midArmLen = 122;
offset = 12;

endEffX = 100;
endEffY = 110;

eqns = [lowArmLen*sin(theta1) + midArmLen*sin(theta1 + theta2) == endEffX-offset, baseLen+lowArmLen*cos(theta1) + midArmLen*cos(theta1 + theta2) == endEffY];
vars = [theta1 theta2];
[soltheta1, soltheta2] = solve(eqns, vars);

theta2Deg = double(soltheta1)*180/pi
theta1Deg = double(soltheta2)*180/pi



