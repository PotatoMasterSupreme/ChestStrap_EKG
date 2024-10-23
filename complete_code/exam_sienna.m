
%% 
% Initial conditions
ics = [2; 0; 0; 0; 0; 0];  % [x1(0), x2(0), x3(0), x4(0), x5(0), x6(0)]

% Time span
tspan = [0 1.25];

% Solve the system using ode45
[t, sol] = ode45(@odefun, tspan, ics);

% Plot the results
figure;
plot(t, sol(:, 1), 'r', t, sol(:, 2), 'g', t, sol(:, 3), 'b');
legend('x1', 'x2', 'x3');
xlabel('Time');
ylabel('Displacement');
title('Solution of the system using ode45');


% Solve using RK4
[t_rk4, y_rk4] = rk4_solver(@odefun, tspan, ics, 0.05);

% Plot RK4 results
figure;
plot(t_rk4, y_rk4(1,:), 'r', t_rk4, y_rk4(2,:), 'g', t_rk4, y_rk4(3,:), 'b');
legend('x1 (RK4)', 'x2 (RK4)', 'x3 (RK4)');
xlabel('Time');
ylabel('Displacement');
title('Runge-Kutta 4 Method');



% Solve using Heun's Method
[t_heun, y_heun] = heun_solver(@odefun, tspan, ics, 0.01);

% Plot Heun's results
figure;
plot(t_heun, y_heun(1,:), 'r', t_heun, y_heun(2,:), 'g', t_heun, y_heun(3,:), 'b');
legend('x1 (Heun)', 'x2 (Heun)', 'x3 (Heun)');
xlabel('Time');
ylabel('Displacement');
title('Heuns Method (Predictor-Corrector)');

% Solve using Trapezoidal Method
[t_trap, y_trap] = trapezoidal_solver(@odefun, tspan, ics, 0.05, 1e-6);

% Plot Trapezoidal results
figure;
plot(t_trap, y_trap(1,:), 'r', t_trap, y_trap(2,:), 'g', t_trap, y_trap(3,:), 'b');
legend('x1 (Trapezoidal)', 'x2 (Trapezoidal)', 'x3 (Trapezoidal)');
xlabel('Time');
ylabel('Displacement');
title('Trapezoidal Method');


function df = odefun(t, x)
    df = zeros(6,1);  % Initialize the derivative vector
    % Positions and velocities
    x1 = x(1);
    x2 = x(2);
    x3 = x(3);
    x4 = x(4);  % Velocity of x1
    x5 = x(5);  % Velocity of x2
    x6 = x(6);  % Velocity of x3

    % Differential equations for the system
    df(1) = x4;
    df(2) = x5;
    df(3) = x6;
    df(4) = (-(0.2*x4 + 0.1*(x4-x6) + 10*x1 + 10*((x1-x2)^5) + 2*(x1-x3))) / 5;
    df(5) = -10*(x2-x1)^5;
    df(6) = (-0.1*(x6-x4) - (x3-x1));
end


function [t, y] = rk4_solver(odefun, tspan, ics, h)
    t = tspan(1):h:tspan(2);
    n = length(t);
    y = zeros(length(ics), n);
    y(:,1) = ics;

    for i = 1:n-1
        k1 = odefun(t(i), y(:,i));
        k2 = odefun(t(i) + 0.5*h, y(:,i) + 0.5*h*k1);
        k3 = odefun(t(i) + 0.5*h, y(:,i) + 0.5*h*k2);
        k4 = odefun(t(i) + h, y(:,i) + h*k3);
        y(:,i+1) = y(:,i) + (h/6)*(k1 + 2*k2 + 2*k3 + k4);
    end
end

function [t, y] = heun_solver(odefun, tspan, ics, h)
    t = tspan(1):h:tspan(2);
    n = length(t);
    y = zeros(length(ics), n);
    y(:,1) = ics;

    for i = 1:n-1
        % Predictor (Euler's method)
        y_pred = y(:,i) + h * odefun(t(i), y(:,i));
        % Corrector (Trapezoidal step)
        y(:,i+1) = y(:,i) + (h/2) * (odefun(t(i), y(:,i)) + odefun(t(i+1), y_pred));
    end
end

function [t, y] = trapezoidal_solver(odefun, tspan, ics, h, tol)
    t = tspan(1):h:tspan(2);
    n = length(t);
    y = zeros(length(ics), n);
    y(:,1) = ics;

    for i = 1:n-1
        y_guess = y(:,i);  % Initial guess (Euler step)
        err = inf;
        while err > tol
            F = y_guess - y(:,i) - (h/2)*(odefun(t(i), y(:,i)) + odefun(t(i+1), y_guess));
            J = eye(length(ics)) - (h/2) * jacobian(odefun, t(i+1), y_guess, h);
            y_new = y_guess - J\F;
            err = norm(y_new - y_guess);
            y_guess = y_new;
        end
        y(:,i+1) = y_guess;
    end
end

function J = jacobian(odefun, t, y, h)
    % Approximate Jacobian of odefun using finite differences
    J = zeros(length(y));
    f0 = odefun(t, y);
    perturb = sqrt(eps) * max(abs(y), h);
    for j = 1:length(y)
        y_pert = y;
        y_pert(j) = y_pert(j) + perturb(j);
        J(:, j) = (odefun(t, y_pert) - f0) / perturb(j);
    end
end


%% question 2

x = [3, 4, 5, 7, 8, 9, 10];
fx = [5, 8, 10.333, 15, 32.333, 15.5, 2];

% Fit polynomial of degree 6 (since you have 7 data points)
p = polyfit(x, fx, 6);

% Evaluate polynomial at given points
xi = [3.5, 8.5, 6];
fi = polyval(p, xi);


pp = spline(x, fx);

% Evaluate spline at given points
fi_spline = ppval(pp, xi);

% Fine grid for plotting
x_fine = linspace(min(x), max(x), 100);
f_poly = polyval(p, x_fine);
f_spline = ppval(pp, x_fine);

% Create the plot
figure;
plot(x, fx, 'x', 'MarkerSize', 10, 'DisplayName', 'Data Points');
hold on;
plot(x_fine, f_poly, 'g-', 'DisplayName', 'Polynomial Interpolation');
plot(x_fine, f_spline, 'b-', 'DisplayName', 'Spline Interpolation');
plot(xi, fi, 'go', 'DisplayName', 'Interpolated Points (Poly)');
plot(xi, fi_spline, 'bo', 'DisplayName', 'Interpolated Points (Spline)');
legend show;
xlabel('x');
ylabel('f(x)');
title('Single vs. Piecewise Polynomial Interpolation');
hold off;

% Define function for root finding
target_f = 5.3;
root_func = @(x) polyval(p, x) - target_f;

% Find roots using fzero in a loop with small steps
x_roots = [];
step_size = 0.2;
for x_test = 3:step_size:10
    try
        root = fzero(root_func, x_test);
        % Round to avoid duplicate roots due to floating-point errors
        root = round(root, 4);
        if all(abs(x_roots - root) > 0.01)
            x_roots = [x_roots, root];
        end
    catch
        % Ignore errors from fzero when no root is found
    end
end

% Plot these roots on the graph
hold on;
plot(x_roots, repmat(target_f, size(x_roots)), 'r+', 'MarkerSize', 8, 'DisplayName', 'Roots');
legend show;
hold off;

%% question 3
function df = odefun1(r, f, S)
    df = zeros(2,1);
    if r < 1e-6  % Using a small epsilon to approximate near-zero behavior
        df(1) = 0;  % Setting derivative of T to zero initially if needed
        df(2) = S;  % Assuming source term S dominates as r approaches zero
    else
        df(1) = f(2);
        df(2) = -f(1)/r - S;
    end
end

% Define the source terms
S = [1, 10, 20];

% Plot setup
figure;
hold on;
colors = ['b', 'r', 'g'];  % Color for each source term plot

for i = 1:length(S)
    % Solve ODE from r=0.0001 to r=1 with initial conditions [1 0] for T(0) and T'(0)
    [R, F] = ode45(@(r, f) odefun1(r, f, S(i)), [0.0001 1], [1 0]);
    plot(R, F(:, 1), colors(i), 'DisplayName', sprintf('S = %d', S(i)));
end

title('Temperature distribution in a circular rod for different S');
xlabel('Radial distance r');
ylabel('Temperature T(r)');
legend show;
hold off;


%% question 4

% Load matrix A and vector b
load('TransientHeatFlux.mat'); % Adjust with the correct file path

% Decompose A using LU decomposition
[L, U, P] = lu(A); % P is the permutation matrix

% Initial conditions
T_initial = b;  % Assuming b is your initial temperature vector at T(0)

% Number of timesteps
numSteps = 75;

% Storage for temperature profiles at specific times for plotting
snapshots = [25, 50, 75]; % Example: capturing snapshots at these timesteps
temperatureProfiles = zeros(size(T_initial, 1), length(snapshots));

% Time-stepping loop
for step = 1:numSteps
    % Solve Ly = Pb (forward substitution)
    y = L \ (P * T_initial);

    % Solve Ux = y (backward substitution)
    T_next = U \ y;

    % Update the temperature vector for the next step
    T_initial = T_next; % Set T_initial to the new temperature for the next iteration

    % Store snapshots
    if ismember(step, snapshots)
        temperatureProfiles(:, find(snapshots == step)) = T_next;
    end
end

% Assuming the domain dimensions for plotting are known, e.g., N x N grid
N = sqrt(length(b));  % If b is a vector representing an N by N grid
x = linspace(0, 1, N);
y = linspace(0, 1, N);
[X, Y] = meshgrid(x, y);

% Plot the temperature distribution at specified timesteps
figure;
for i = 1:size(temperatureProfiles, 2)
    subplot(1, size(temperatureProfiles, 2), i);
    Z = reshape(temperatureProfiles(:, i), [N, N]);
    surf(X, Y, Z);
    title(['Temperature at timestep ', num2str(snapshots(i))]);
    xlabel('X');
    ylabel('Y');
    zlabel('Temperature (°C)');
    axis tight;
end
