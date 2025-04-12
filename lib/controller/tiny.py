import tinympc
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
    r = 0.15
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

    # r       = x[0] # cart position
    # theta   = x[1] # pole angle
    # rd      = x[2] # change in cart position
    # theta_d = x[3] # change in pole angle
    # F       = u[0] # force applied to cart

    # theta_dd = (g*np.sin(theta) + np.cos(theta) * ((-F - mp*l*(theta_d**2) * \
    #                 np.sin(theta))/(mc + mp))) / (l*(4/3 - (mp*(np.cos(theta)**2))/(mc + mp)))
    # rdd = (F + mp*l*((theta_d**2)*np.sin(theta) - theta_dd*np.cos(theta))) / (mc + mp)

    # return np.array([rd, theta_d, rdd, theta_dd])

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
A = AG.jacobian(lambda x_: dynamics_rk4(x_, ugoal, dt))(xgoal)
B = AG.jacobian(lambda u_: dynamics_rk4(xgoal, u_, dt))(ugoal)
print("sim", A @ xgoal + B @ np.array([0, 0, 0, 100.0]))

Q = np.diag([
    200.0, 200.0, 200.0,
    0.4, 0.4, 0.4,
    0.4, 0.4, 0.2,
    0.2, 0.2, 0.2,
])
R = np.diag([0.1] * 4); # Penalize actuator effort

N = 20 

# print(dynamics_rk4(np.zeros(12), np.array([0, 0, 0, 1000]), 0.01))

# Set up the problem
prob = tinympc.TinyMPC()
prob.u_min = np.array([-0.5, -0.5, -1.0, -98.1])
prob.u_max = np.array([0.5, 0.5, 1.0, 200.0])
prob.x_min = np.array([-np.inf] * 12)
prob.x_min[2] = 0.1
prob.x_max = np.array([np.inf] * 12)
prob.setup(A, B, Q, R, N, u_min=prob.u_min, u_max=prob.u_max, x_min=prob.x_min)

# Define initial condition
x0 = np.zeros(12)
x0[2] = 9
prob.set_x0(x0)

x_goal = np.zeros(12)
x_goal[1] = 3
x_goal[2] = 0
prob.set_x_ref(x_goal)

# Solve the problem
solution = prob.solve()

# Print the controls at the first time step
print(solution)

Nsim = 350
xs = np.zeros((Nsim, Q.shape[0])) # History of states for plotting
us = np.zeros((Nsim, R.shape[0])) # History of controls for plotting
for i in range(Nsim):
    prob.set_x0(x0) # Set the first state in the horizon
    solution = prob.solve() # Solve the problem
    x0 = dynamics_rk4(x0, ugoal + solution["controls"], 0.01)
    # x0 = A@x0 + B@ np.array([0, 0, 0, 100.0])# solution["controls"] # Simulate the system 
    # x0 = A@x0 + B@ solution["controls"] # Simulate the system 
    print(x0[2], solution["controls"])
    xs[i] = x0
    us[i] = solution["controls"]

# prob.codegen("generated_code", verbose=1)

import matplotlib.pyplot as plt

# Plot trajectory
fig, axs = plt.subplots(4, 1, sharex=True)
axs[0].plot(xs[:,:3], label=["x", "y", "z"])
axs[1].plot(xs[:,3:6], label=["vx", "vy", "vz"])
axs[2].plot(xs[:,6:9], label=["rx", "ry", "rz"])
axs[3].plot(us, label=["u1", "u2", "u3", "u4"])
axs[0].set_title("trajectory over time")
axs[1].set_title("velocity over time")
axs[2].set_title("rotation over time")
axs[3].set_xlabel("time steps (100Hz)")
axs[0].legend()
axs[1].legend()
axs[2].legend()
axs[3].legend()
plt.show()