# BController

This library module contains the Basic Controller, which is effectively a wrapper around the TinyMPC MPC framework. To use:
1. `pip3 install cvxpy cvxpygen`
2. Define controller in cvx.py
3. `python3 cvx.py` to generate
4. `rm -rf mpc; mkdir mpc`
5. `cp -r gen/LICENSE gen/README.html gen/c/include gen/c/src gen/c/solver_code mpc`
6. `rm -rf gen`
7. `rm mpc/src/cpg_example.c`

Note that this should be compatible with all tinympc controllers.
