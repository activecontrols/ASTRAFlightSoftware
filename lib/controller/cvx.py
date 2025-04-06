import cvxpy as cp
import autograd as AG
import autograd.numpy as np 

# mc = 0.2 # mass of the cart (kg)
# mp = 0.1 # mass of the pole (kg)
# l = 0.5 # distance to the center of mass (meters)
# g = 9.81

# xdot = dynamics(x, u)
# x = [pos*3, vel*3, ang*3, angvel*3]
# u = [gimbalx, gimbaly, rollthruster, thruster]
def rot_matrix(rx,ry,rz):
    return np.array([
        [np.cos(rz)*np.cos(ry), np.cos(rz)*np.sin(ry)*np.sin(rx) - np.sin(rz)*np.cos(rx), np.cos(rz)*np.sin(ry)*np.cos(rx) + np.sin(rz)*np.sin(rx)],
        [np.sin(rz)*np.cos(ry), np.sin(rz)*np.sin(ry)*np.sin(rx) + np.cos(rz)*np.cos(rx), np.sin(rz)*np.sin(ry)*np.cos(rx) - np.cos(rz)*np.sin(rx)],
        [-np.sin(ry), np.cos(ry)*np.sin(rx), np.cos(ry)*np.cos(rx)]
    ])

def dynamics(X, U):
    r = 0.35
    M = 10
    I = 1/2 * (M / 2) * r**2

    x,y,z = X[0:3]
    vx,vy,vz = X[3:6]
    rx,ry,rz = X[6:9]
    drx,dry,drz = X[9:12]
    gimbalx, gimbaly, rollT, fT = U

    # xyz are in inertial ref frame
    # rotation are in body ref frame
    # xyz pos has no effect on vel or angvel
    # vx, vy, vz directly affect xyz
    # acceleration depends on thrust + direction
    # Rot(Body) @ (Rot(Gimbal) @ [0, 0, fT].T) - [0, 0, 9.81]
    # angvel directly dependent on drx, dry, drz
    # Angular acceleration dependent on torque from gimbaled motor
    # tau = rF sin theta -> angaccel = rF sin theta / I, I is rotational inertia
    # We assume rocket body is solid cylinder and legs are negligible (change this for real world!)
    accel = rot_matrix(rx,ry,rz) @ (rot_matrix(gimbalx, gimbaly, 0) @ np.array([0, 0, fT])) / M - np.array([0, 0, 9.81])
    arx = r * fT * np.sin(gimbalx) / I
    ary = r * fT * np.sin(gimbaly) / I
    arz = rollT
    print(f"arz {arz}, drz {drz}")
    return np.array([
        vx, vy, vz,
        accel[0], accel[1], accel[2],
        drx, dry, drz,
        arx, ary, arz
    ])

def dynamics_rk4(x, u, dt):
    f1 = dt*dynamics(x, u)
    f2 = dt*dynamics(x + f1/2, u)
    f3 = dt*dynamics(x + f2/2, u)
    f4 = dt*dynamics(x + f3, u)
    return x + (1/6)*(f1 + 2*f2 + 2*f3 + f4)

# Neutral position to linearize around
xgoal = np.zeros(12) # On ground, straight up, no movement
ugoal = np.array([0, 0, 0, 98.1]) # Thrust force to counteract gravity

dt = 0.01
npA = AG.jacobian(lambda x_: dynamics_rk4(x_, ugoal, dt))(xgoal)
npB = AG.jacobian(lambda u_: dynamics_rk4(xgoal, u_, dt))(ugoal)

# WARNING: Changing these only affects simulation! Change ControllerModule.cpp
npQ = np.diag(np.sqrt([
    20000.0, 20000.0, 20000.0,
    3000, 3000, 3000,
    5, 5, 5,
    5, 5, 5,
]))
npR = np.diag(np.sqrt([0.0, 0.0, 0.1, 0.1])); # Penalize actuator effort

npQland = np.diag(np.sqrt([
    10000.0, 10000.0, 0.0,
    20, 20, 2000,
    5, 5, 5,
    5, 5, 5,
]))

# define dimensions
H, n, m = 20, 12, 4

# define variables
U = cp.Variable((m, H), name='U')
X = cp.Variable((n, H+1), name='X')

# define parameters
Psqrt = cp.Parameter((n, n), name='Psqrt')
Qsqrt = cp.Parameter((n, n), name='Qsqrt')
Rsqrt = cp.Parameter((m, m), name='Rsqrt')
# TODO: Make A and B parameters when im not lazy
A = npA
B = npB
# A = cp.Parameter((n, n), name='A')
# B = cp.Parameter((n, m), name='B')
# A.value = npA
# B.value = npB
x_init = cp.Parameter(n, name='x_init')

# define objective
objective = cp.Minimize(cp.sum_squares(Psqrt@X[:,H]) + cp.sum_squares(Qsqrt@X[:,:H]) + cp.sum_squares(Rsqrt@U))

# define constraints
constraints = [X[:,1:] == A@X[:,:H]+B@U,
               U[0] <= 0.3,
               U[0] >= -0.3,
               U[1] <= 0.3,
               U[1] >= -0.3,
               U[3] <= 100,
               U[3] >= -98.1,
               X[:,0] == x_init]

# define problem
problem = cp.Problem(objective, constraints)

Qsqrt.value = npQ
Rsqrt.value = npR
Psqrt.value = np.zeros((n, n))

Nsim = 1350
xs = np.zeros((Nsim, Qsqrt.shape[0])) # History of states for plotting
us = np.zeros((Nsim, Rsqrt.shape[0])) # History of controls for plotting
x0 = np.zeros(12)
x0[1] = 2
x0[2] = 5
xtarget = np.zeros(12)
# x0[2] = 5
for i in range(Nsim):
    x_init.value = (x0 - xtarget) # Set the first state in the horizon
    # if i >= 350:
    #     Qsqrt.value = npQland
    #     xtarget[5] = -1

    objval = problem.solve() # Solve the problem
    x0 = dynamics_rk4(x0, ugoal + U.value[:, 0], 0.01)
    # x0 = A@x0 + B@ np.array([0, 0, 0, 100.0])# solution["controls"] # Simulate the system 
    # x0 = A@x0 + B@ solution["controls"] # Simulate the system 
    print(x0[2], U.value[:, 0])
    xs[i] = x0
    us[i] = U.value[:, 0]

# prob.codegen("generated_code", verbose=1)

import matplotlib.pyplot as plt

# Plot trajectory
fig, axs = plt.subplots(5, 1, sharex=True)
axs[0].plot(xs[:,:3], label=["x", "y", "z"])
axs[1].plot(xs[:,3:6], label=["vx", "vy", "vz"])
axs[2].plot(xs[:,6:9], label=["rx", "ry", "rz"])
axs[3].plot(xs[:,9:12], label=["drx", "dry", "drz"])
axs[4].plot(us, label=["u1", "u2", "u3", "u4"])
axs[0].set_title("trajectory over time")
axs[1].set_title("velocity over time")
axs[2].set_title("rotation over time")
axs[3].set_xlabel("angvel over time")
axs[4].set_xlabel("time steps (100Hz)")
axs[0].legend()
axs[1].legend()
axs[2].legend()
axs[3].legend()
axs[4].legend()
plt.show()

# Generate the controller
from cvxpy.reductions.solvers import defines as slv_def
print("Available solvers:", slv_def.INSTALLED_SOLVERS)
from cvxpygen import cpg
cpg.generate_code(problem, code_dir='gen', solver='OSQP', wrapper=False)