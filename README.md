# How to run and change setting
Typical way to run the program:
    ./LabSchrodinger a=0.5 N=512

You can optionaly changer parameters using command line options as is shown above by writing 'VAR=VALUE', you can change
    - highPrecision=True/False (default = True)
    - onlyValues=True/False    (default = False)
    - N=int                    (default = 256)
    - a=double                 (default = 1.0)
    - omega=double             (default = 0.05)
    - x0=int                   (default = -N/2)
    - HO=True/False            (default = True)

Program return:
    -evalues.txt  (file with eigenvalues in columnt, if HO=True there is second column with deltaE/hw)
    -evectors.txt (file with eigenvectors, first columnt first eigenvector etc.)
